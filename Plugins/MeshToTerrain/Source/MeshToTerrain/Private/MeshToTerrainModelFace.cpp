#include "MeshToTerrainModelFace.h"
#include "MeshToTerrainModelPixel.h"

FMeshToTerrainModelFace::FMeshToTerrainModelFace(TArray<int> & Vrtx)
{
	Vertexes = Vrtx;
	// Create loop for easier manipulation of edges: V[n] == V[0]
	Vertexes.Add(Vrtx[0]);

    // Adjust the vertex indexes 
	for (int i = 0; i < Vertexes.Num(); i++)
		Vertexes[i]--;
}

void FMeshToTerrainModelFace::ProjectPixel(const TArray<FMeshToTerrainModelVertex> & Vrts, 
										 const FVector & Origin, 
										 float Increment, 
										 TArray<FMeshToTerrainModelPixel> & Pixs,
										 int FaceId)
{
	// Face plane equation
	FPlane FacePlane = FPlane(Vrts[Vertexes[0]].Position, Vrts[Vertexes[1]].Position, Vrts[Vertexes[2]].Position);

	// Invalid face plane
    if (FacePlane.ContainsNaN())
    {
        return;
    }

	// Check if the face is paralel to the Z axis.
    // Strict comparison is ok, no need for epsilon comparison
    if (FacePlane.Z == 0.0f)
    {
        return;
    }

	PixelPositions.Empty();

	// Compute bounding box
	FBox FaceBoundingBox = ComputeBoundingBox(Vrts);

    // Compute the face origin in heightmap coordinates
	FaceOriginPixel  = { FGenericPlatformMath::FloorToInt32((FaceBoundingBox.Min.X - Origin.X)/Increment),
				         FGenericPlatformMath::FloorToInt32((FaceBoundingBox.Min.Y - Origin.Y)/Increment)};
    // Compute the size of the face in heightmap coordinates ( i.e. number of pixels along X and Y coordinates)
	FaceSizePixel = { FGenericPlatformMath::CeilToInt32(FaceBoundingBox.GetSize().X / Increment),
				      FGenericPlatformMath::CeilToInt32(FaceBoundingBox.GetSize().Y / Increment) };

	// Extend the size by one pixel
    if ((Origin.X + (FaceOriginPixel.X + FaceSizePixel.X)*Increment) < FaceBoundingBox.Max.X) FaceSizePixel.X++;
	if ((Origin.Y + (FaceOriginPixel.Y + FaceSizePixel.Y)*Increment) < FaceBoundingBox.Max.Y) FaceSizePixel.Y++;

	//Might not need this depending on how we decide to do the face blending
    //REVISIT: Might remove this optimization
    for (int i=FaceOriginPixel.X ; i<FaceOriginPixel.X + FaceSizePixel.X + 1; i++)
	{
		for (int j = FaceOriginPixel.Y; j < FaceOriginPixel.Y + FaceSizePixel.Y + 1; j++)
		{
			FVector Pixel = { Origin.X + i*Increment, Origin.Y + j* Increment, 0 };
            // Find out for every pixel the face might be projected on if its corners 
            // are outside/inside of the face or on a edge
			EPixelPosition Pos = CheckPixelPosition(Pixel, Vrts, Increment*0.001);
			PixelPositions.Add(Pos);
        }
	}

	//Sub pixel projection (face smaller than a pixel)
	if ((FaceSizePixel.X == 1) && (FaceSizePixel.Y == 1))
	{ 
		int NumPixelsOutside = (PixelPositions[0] == EPixelPosition::FACE_OUTSIDE) +
							   (PixelPositions[1] == EPixelPosition::FACE_OUTSIDE) +
							   (PixelPositions[2] == EPixelPosition::FACE_OUTSIDE) +
							   (PixelPositions[3] == EPixelPosition::FACE_OUTSIDE);

		if (NumPixelsOutside)
		{
            // Approximate the face area to its bounding box area
            // If we have to do this either the rezolution is really bad or
            // this face won't really matter
			float FaceArea = FaceBoundingBox.GetCenter().X * FaceBoundingBox.GetCenter().Y;
            if (!FMath::IsFinite(FaceArea))
            {
                return;
            }
			const float PixelArea = Increment * Increment;
			int   Ratio = FGenericPlatformMath::FloorToInt(FaceArea / PixelArea * 100);
			// Ignore the face if the area is less than 1% of the pixel area
            if (Ratio > 1)
			{
                // Compute the pixel height
				FVector BoundingBoxCenter   = { FaceBoundingBox.GetCenter().X, FaceBoundingBox.GetCenter().Y , 0 };
				float Height                = (FacePlane.W - FacePlane.X *BoundingBoxCenter.X - FacePlane.Y *BoundingBoxCenter.Y) / FacePlane.Z;
				FIntPoint CurrentPixel      = FIntPoint(FaceOriginPixel.X, FaceOriginPixel.Y);
				FMeshToTerrainModelPixel Pix  = FMeshToTerrainModelPixel(CurrentPixel, Height, Ratio, FaceId);
				Pixs.Add(Pix);
				return;
			}
		}

	}
	 
    // Compute the height of each pixel this face projects onto
	for (int i = FaceOriginPixel.X; i < FaceOriginPixel.X +FaceSizePixel.X; i++)
		for (int j = FaceOriginPixel.Y; j < FaceOriginPixel.Y + FaceSizePixel.Y; j++)
		{
			FVector   Pixel        = { Origin.X + i*Increment, Origin.Y + j* Increment, 0 };
			int       Ratio        = 0;
			FIntPoint CurrentPixel = FIntPoint(i, j);

			float Height = ComputePixelHeight(Pixel, Vrts, Increment, FacePlane, Ratio, CurrentPixel);

			if (Ratio != 0)
			{
				FMeshToTerrainModelPixel Pix = FMeshToTerrainModelPixel(CurrentPixel, Height, Ratio, FaceId);
				Pixs.Add(Pix);
			}
		}

	return;
}

FMeshToTerrainModelFace::EPixelPosition FMeshToTerrainModelFace::CheckPixelPosition(const FVector & Pixel, const TArray<FMeshToTerrainModelVertex>& Vrts, float Epsilon)
{
    EPixelPosition RetValue = EPixelPosition::FACE_OUTSIDE;
    int wn = 0;

    // Winding count algorithm. 
    for (int i = 0; i < Vertexes.Num() - 1; i++) // duplicated first element at the end
    {
        FVector Start = Vrts[Vertexes[i]].Position;
        FVector End = Vrts[Vertexes[i + 1]].Position;

        // Reset z component 
        Start.Z = 0;
        End.Z = 0;

        // Consider as internal all pixels that are within epsilon distance w.r.t to an edge
        if (FMath::PointDistToSegment(Pixel, Start, End) < Epsilon)
        {
            return EPixelPosition::FACE_BORDER;
        }

        if (Start.Y <= Pixel.Y)
        {
            if (End.Y > Pixel.Y)    // an upward crossing
            {
                if (IsLeft(Pixel, Start, End, Epsilon) > 0) // pixel left of edge
                {
                    wn++;
                }
            }
        }
        else if (End.Y <= Pixel.Y)  // a downward crossing
        {
            if (IsLeft(Pixel, Start, End, Epsilon) < 0)  // pixel right of edge
            {
                wn--;
            }
        }
    }

    if (wn > 0)
    {
        RetValue = EPixelPosition::FACE_INSIDE;
    }
	return RetValue;
}

// > pixle P2 is left of the line start-end
// = 0 on line
// <0 right of the line
float FMeshToTerrainModelFace::IsLeft(const FVector & Pixel, const FVector & Start, const FVector & End, float epsilon)
{
	return (Start.X - Pixel.X) * (End.Y - Pixel.Y) - (End.X - Pixel.X) * (Start.Y - Pixel.Y);
}

float FMeshToTerrainModelFace::ComputePixelHeight(const FVector & Pixel, const TArray<FMeshToTerrainModelVertex>& Vrts, float Increment, const FPlane & FacePlane, int & Ratio, const FIntPoint & Index)
{
	int NumPixelsCornersOutside = NumCornersAt(Index, EPixelPosition::FACE_OUTSIDE);
	int NumPixelsCornersInside  = NumCornersAt(Index, EPixelPosition::FACE_INSIDE);
	int NumPixelsCornersBorder  = NumCornersAt(Index, EPixelPosition::FACE_BORDER);
 
	if (NumPixelsCornersOutside == 0)
	{
        // The entire pixel lies inside of the face
		FVector Center = { Pixel.X+ Increment/2, Pixel.Y + Increment/2, 0 };
		float Height = (FacePlane.W - FacePlane.X *Center.X - FacePlane.Y *Center.Y) / FacePlane.Z;
		Ratio = 100;
		return Height;
	}
	else
	{
		TArray<FVector> ClippedFace;
        // Find the overlapping area between the pixel and the face
		if (ClipToPixel(Pixel, Increment, Vrts, ClippedFace))
		{
            // Get the height of the center of the overlapped area
			FBox    ClipBB        = FBox(ClippedFace);
			FVector CenterBB      = { ClipBB.GetCenter().X, ClipBB.GetCenter().Y, 0 };
			float   Height        = (FacePlane.W - FacePlane.X *CenterBB.X - FacePlane.Y *CenterBB.Y) / FacePlane.Z;

            // Find the overlap ratio
			float   OverlapArea   = ApproximateArea(ClipBB, ClippedFace);
			float   PixelArea    = Increment * Increment;
			Ratio = FMath::TruncToInt(OverlapArea / PixelArea * 100);
			return Height;
		}
	}
	return 0.0f;
}

float FMeshToTerrainModelFace::ComputeFaceArea(const TArray<FMeshToTerrainModelVertex> & Vrts)
{
    if (Vertexes.Num() < 3)
    {
        return NAN;
    }
	
	FBox Bb = ComputeBoundingBox(Vrts);
	return Bb.GetSize().X * Bb.GetSize().Y;
}

int FMeshToTerrainModelFace::NumCornersAt(const FIntPoint & Index, const EPixelPosition Pos)
{
    // Check if the pixel is outside of the bounding box of the face
	if ((Index.X < FaceOriginPixel.X) ||
		(Index.Y < FaceOriginPixel.Y) ||
		(Index.X >= (FaceOriginPixel.X + FaceSizePixel.X)) ||
		(Index.Y >= (FaceOriginPixel.Y + FaceSizePixel.Y)))
	{
        if (Pos == EPixelPosition::FACE_OUTSIDE)
        {
            return 4;
        }
        else
        {
            return 0;
        }
	}

	int SwX = Index.X - FaceOriginPixel.X;
	int SwY = Index.Y - FaceOriginPixel.Y;
	int SeX = SwX + 1;
	int SeY = SwY;
	int NwX = SwX;
	int NwY = SwY + 1;
	int NeX = SwX + 1;
	int NeY = SwY + 1;
	
	int RetVal = (PixelPositions[SwX*(FaceSizePixel.Y + 1) + SwY] == Pos) +
				 (PixelPositions[SeX*(FaceSizePixel.Y + 1) + SeY] == Pos) +
				 (PixelPositions[NwX*(FaceSizePixel.Y + 1) + NwY] == Pos) +
				 (PixelPositions[NeX*(FaceSizePixel.Y + 1) + NeY] == Pos);

	return RetVal;
}

bool FMeshToTerrainModelFace::ClipToPixel(const FVector & Index, float Increment, const TArray<FMeshToTerrainModelVertex>& Vrts, TArray<FVector>& ClippedArea)
{
    // Simple Sutherland–Hodgman algorithm implementation
    // The clip polygon is just the pixel square
    ClippedArea.Empty();
    for (auto &item : Vertexes)
    {
        ClippedArea.Push(Vrts[item].Position);
    }

    // Left edge plane
    float LeftX = Index.X;
    FPlane LeftEdge = FPlane(1, 0, 0, Index.X);

    TArray<FVector> Left;
    for (int i = 0; i < ClippedArea.Num() - 1; i++)
    {
        FVector Start = ClippedArea[i];
        FVector End = ClippedArea[i + 1];

        if (Start.X >= LeftX)
        {
            Left.Add(Start);
            if (End.X < LeftX)
            {
                FVector IntersectionPoint;
                if (FMath::SegmentPlaneIntersection(Start, End, LeftEdge, IntersectionPoint))
                {
                    Left.Add(IntersectionPoint);
                }
            }
        }
        else
        {
            if (End.X > LeftX)
            {
                FVector IntersectionPoint;
                if (FMath::SegmentPlaneIntersection(Start, End, LeftEdge, IntersectionPoint))
                {
                    Left.Add(IntersectionPoint);
                }
            }
        }
    }
    if (Left.Num() < 3)
    {
        return false;
    }
    
    // Duplicate the first pixel for easier further processing
	FVector LeftFirst = Left[0];
	Left.Add(LeftFirst);

	//Right edge Plane
	float RightX = Index.X+Increment;
	FPlane RightEdge = FPlane(1, 0, 0, RightX);

	TArray<FVector> Right;
	for (int i = 0; i < Left.Num() - 1; i++)
	{
		FVector Start = Left[i];
		FVector End   = Left[i + 1];
		
		if (Start.X <= RightX)
		{
			Right.Add(Start);
			if (End.X > RightX)
			{
				FVector IntersectionPoint;
				if (FMath::SegmentPlaneIntersection(Start, End, RightEdge, IntersectionPoint))
				{
					Right.Add(IntersectionPoint);
				}
			}
		}
		else
		{
			if (End.X < RightX)
			{
				FVector IntersectionPoint;
				if (FMath::SegmentPlaneIntersection(Start, End, RightEdge, IntersectionPoint))
				{
					Right.Add(IntersectionPoint);
				}
			}
		}
	}
    if (Right.Num() < 3)
    {
        return false;
    }

    // Duplicate the first pixel for easier further processing
	FVector RightFirst = Right[0];
	Right.Add(RightFirst);

	//Bottom edge Plane
	float BottomY = Index.Y;
	FPlane BottomEdge = FPlane(0, 1, 0, BottomY);

	TArray<FVector> Bottom;
	for (int i = 0; i < Right.Num() - 1; i++)
	{
		FVector Start = Right[i];
		FVector End   = Right[i + 1];
		
		if (Start.Y >= BottomY)
		{
			Bottom.Add(Start);
			if (End.Y < BottomY)
			{
				FVector IntersectionPoint;
				if (FMath::SegmentPlaneIntersection(Start, End, BottomEdge, IntersectionPoint))
				{
					Bottom.Add(IntersectionPoint);
				}
			}
		}
		else
		{
			if (End.Y > BottomY)
			{
				FVector IntersectionPoint;
				if (FMath::SegmentPlaneIntersection(Start, End, BottomEdge, IntersectionPoint))
				{
					Bottom.Add(IntersectionPoint);
				}
			}
		}
	}
    if (Bottom.Num() < 3)
    {
        return false;
    }

    // Duplicate the first pixel for easier further processing
	FVector BottomFirst = Bottom[0];
	Bottom.Add(BottomFirst);

	//Top edge Plane
	float TopY = Index.Y+Increment;
	FPlane TopEdge = FPlane(0, 1, 0, TopY);

	TArray<FVector> Top;
	for (int i = 0; i < Bottom.Num() - 1; i++)
	{
		FVector Start = Bottom[i];
		FVector End   = Bottom[i + 1];
		
		if (Start.Y <= TopY)
		{
			Top.Add(Start);
			if (End.Y > TopY)
			{
				FVector IntersectionPoint;
				if (FMath::SegmentPlaneIntersection(Start, End, TopEdge, IntersectionPoint))
				{
					Top.Add(IntersectionPoint);
				}
			}
		}
		else
		{
			if (End.Y < TopY)
			{
				FVector IntersectionPoint;
				if (FMath::SegmentPlaneIntersection(Start, End, TopEdge, IntersectionPoint))
				{
					Top.Add(IntersectionPoint);
				}
			}
		}
	}
    if (Top.Num() < 3)
    {
        return false;
    }

    // Duplicate the first pixel for easier further processing
	FVector TopFirst = Top[0];
	Top.Add(TopFirst);

	ClippedArea.Empty();
	ClippedArea = Top;
	
	return true;
}

float FMeshToTerrainModelFace::ApproximateArea(const FBox & PolygonBoundingBox, const TArray<FVector>& ClippedArea)
{
    if (ClippedArea.Num() < 3)
    {
        return 0.0f;
    }

    // If the polygon is not a triangle return the area of the bounding box
    // Best trade-off between accuracy and speed.
    if (ClippedArea.Num() > 3)
    {
        return PolygonBoundingBox.GetSize().X * PolygonBoundingBox.GetSize().Y;
    }

	FVector EdgeA = ClippedArea[0] - ClippedArea[1];
	FVector EdgeB = ClippedArea[1] - ClippedArea[2];
	FVector EdgeC = ClippedArea[2] - ClippedArea[0];
	
	float LengthEdgeA = EdgeA.Size2D();
	float LengthEdgeB = EdgeB.Size2D();
	float LengthEdgeC = EdgeC.Size2D();

    // Triangle area knowing the length of the 3 edges
	float SemiPerimeter = (LengthEdgeA + LengthEdgeB + LengthEdgeC) / 2;
	return FMath::Sqrt(SemiPerimeter * (SemiPerimeter - LengthEdgeA) * (SemiPerimeter - LengthEdgeB) * (SemiPerimeter - LengthEdgeC));
}

FBox FMeshToTerrainModelFace::ComputeBoundingBox(const TArray<FMeshToTerrainModelVertex> & Vrts)
{
	FBox RetVal = FBox(&(Vrts[Vertexes[0]].Position), 1);
	for (auto &item : Vertexes)
	{
		RetVal += Vrts[item].Position;
	}

	return RetVal;
}