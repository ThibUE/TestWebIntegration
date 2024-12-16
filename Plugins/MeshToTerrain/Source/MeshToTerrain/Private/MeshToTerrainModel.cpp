#include "MeshToTerrainModel.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "MeshToTerrainModelPixel.h"
#include "MeshToTerrain.h"
DEFINE_LOG_CATEGORY(MeshToTerrainLog);

FMeshToTerrainModel::FMeshToTerrainModel()
{}

FMeshToTerrainModel::~FMeshToTerrainModel()
{
	Vertex.Empty();
	Face.Empty();
}

void FMeshToTerrainModel::AddShape(FMeshToTerrainModelVertex Vrtx)
{
	Vertex.Add(Vrtx);
}

void FMeshToTerrainModel::AddShape(FMeshToTerrainModelFace Fce)
{
	Face.Add(Fce);
}

void FMeshToTerrainModel::ConvertData(TArray<uint16>& Data, const FIntPoint & MapSize)
{
	// Initialize the placeholder with the default data (0 height level)
	// REVISIT: remove hardcoded value
	for (int i = 0; i < MapSize.X *MapSize.Y; i++)
		Data.Add(32768);

	if (Vertex.Num() == 0)
	{
		return;
	}

	PreProprocessModelData();

	// Key = pixel position (pixel_width * max_width + pixel_height)
	// Data = Array of model pixels belonging to different faces that project on this pixel
	TMap<int, TArray<FMeshToTerrainModelPixel>> PixelMap;

	FBox BoundingBox = ComputeBoundingBox();

	float Increment = 0;
	float Width = BoundingBox.GetSize().X;
	float Height = BoundingBox.GetSize().Y;

	// Find the pixel size in term of mesh coordinates
	// I.e. a square of size Increment x Increment (in the mesh world) will be
	// covered by one pixel
	// The largest side is taken as reference (fills the heightmap along that direction)
	if (Width > Height)
	{
		Increment = Width / MapSize.X;
	}
	else
	{
		Increment = Height / MapSize.Y;
	}

	int j = 0;
	for (auto &Item : Face)
	{
		j++;
		TArray<FMeshToTerrainModelPixel> ProjectedPixels;

		// Get all the model pixels onto which the current face is projected
		Item.ProjectPixel(Vertex, BoundingBox.Min, Increment, ProjectedPixels, j);
		if (ProjectedPixels.Num() == 0)
		{
			UE_LOG(MeshToTerrainLog, Warning, TEXT("--- no pixels for face %d"), j);
		}

		// Keep track of all model pixels that cover a pixel of the heightmap
		for (auto & Pixel : ProjectedPixels)
		{
			// Find the index of the heightmap pixel corresponding to the
			// current model pixel
			int Index = Pixel.Position.Y * MapSize.X + Pixel.Position.X;

			// Save the data
			TArray<FMeshToTerrainModelPixel> * Value = PixelMap.Find(Index);
			if  (Value != nullptr)
			{
				Value->Add(Pixel);
			}
			else
			{
				TArray<FMeshToTerrainModelPixel> newEntry = { Pixel };
				PixelMap.Add(Index, newEntry);
			}
		}

		// Empty the container for the next iteration
		ProjectedPixels.Empty();
	}

	// Value for the sea level (heightmap coordinates)
	const int ZeroLevelHeight       = ((uint16(-1) >> 1) + 1);
	// Value for the highest point (heightmap coordinates)
	const int MaxLevelHeight        = (int)((uint16)(-1));
	// Maximum absolute difference between any two points (heightmap coordinates)
	const int MaxLevelDifference    = MaxLevelHeight - ZeroLevelHeight;
	// Mesh maximum height (mesh coordinates)
	const float MeshMaxHeight = FMath::Max<float>(BoundingBox.Max.Z, FMath::Abs(BoundingBox.Min.Z));

	// For every pixel of the heightmap compute the weighted height of its
	// corresponding model pixels
	for (auto & Elem : PixelMap)
	{
		TArray<FMeshToTerrainModelPixel> Overlaps = Elem.Value; 
		int PixelIndex      = Overlaps[0].Position.Y * MapSize.X + Overlaps[0].Position.X;
		float HeightMean    = 0;
		int   TotalWeight   = 0;
		for (int i = 0; i < Overlaps.Num(); i++)
		{
			HeightMean  += Overlaps[i].ActualHeight * Overlaps[i].Ratio;
			TotalWeight += Overlaps[i].Ratio;
		}
		HeightMean /= TotalWeight;

		float HeightRatio = HeightMean / MeshMaxHeight;

		int ActualPixelHeight = FMath::CeilToInt(MaxLevelDifference*HeightRatio) + ZeroLevelHeight;
		ActualPixelHeight = FMath::Clamp(ActualPixelHeight, 0, (int)((uint16)(-1)));
		Data[PixelIndex] = ActualPixelHeight;
	}
}

FBox FMeshToTerrainModel::ComputeBoundingBox()
{
	if (Vertex.Num() == 0)
	{
		return FBox(ForceInitToZero);
	}

	FBox RetVal = FBox(&(Vertex[0].Position), 1);

	for (auto &Item : Vertex)
	{
		RetVal += Item.Position;
	}

	return RetVal;
}

void FMeshToTerrainModel::PreProprocessModelData()
{
	// Sometimes the vertex positions are represented by very small floating point
	// numbers and in these cases the precision error can cause significant problems
	// when the face planes are computed.
	// The most robust solution is to actually compute the required scale factor, but
	// this computation is going to take as much as the face projection computation. 
	// The fastest solution is to scale the positions by a factor of 1e6. If this factor
	// proves to be too small, the user should regenerate the mesh.

	for (auto &Item : Vertex)
	{
		Item.Position *= 1000000;
	}
}
