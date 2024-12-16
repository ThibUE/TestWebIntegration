#pragma once

#include "MeshToTerrainPrivatePCH.h"
#include "MeshToTerrainModelPixel.h"
#include "MeshToTerrainModelVertex.h"

class FMeshToTerrainModelFace
{
	public:
        /* Indicates the indexes in the vertex array
           that belong to this face */
		TArray<int> Vertexes;

	public:
		FMeshToTerrainModelFace(TArray<int> & Vrtx);

        /* Computes the height and location of the pixels onto 
           which this face gets projected.
           Vrts      = Array of the mesh vertices 
           Origin    = Minimum point of the mesh's bounding box
           Increment = Size of a heightmap pixel in mesh world
           Pixs      = Array of model pixels that gets populated 
           FaceId    = Id of this face */
		void ProjectPixel(const TArray<FMeshToTerrainModelVertex> & Vrts, 
						  const FVector & Origin, 
						  float Increment,
						  TArray<FMeshToTerrainModelPixel> & Pixs,
						  int FaceId);
	
	private:
		enum class EPixelPosition{
			FACE_OUTSIDE = 1,
			FACE_INSIDE  = 2,
			FACE_BORDER  = 4
		};
        
        /* Computes the bounding box of the face in mesh coordinates*/
		FBox ComputeBoundingBox(const TArray<FMeshToTerrainModelVertex> & vrts);

        /* Computes the relative position of a pixel to the face.
           The pixel position is in the mesh world. */
		EPixelPosition CheckPixelPosition(const FVector & Pixel, const TArray<FMeshToTerrainModelVertex> & vrts, float Epsilon);

        /* Computes if a pixel is on the left side of a given vector*/
		float IsLeft(const FVector & Pixel, const FVector & Start, const FVector &End, float Epsilon);

        /* Computes the height of a pixel onto which the face projects and
           the ratio of the pixel that the face covers*/
		float ComputePixelHeight(const FVector & Pixel,
								 const TArray<FMeshToTerrainModelVertex> & Vrts,
								 float Increment,
								 const FPlane & FacePlane,
								 int & Ratio,
								 const FIntPoint & Index);

        // REVISIT: this function is no longer used
		float ComputeFaceArea(const TArray<FMeshToTerrainModelVertex> & Vrts);

		TArray<EPixelPosition> PixelPositions;
		FIntPoint FaceOriginPixel;
		FIntPoint FaceSizePixel;

        /* Returns the number of corners of the pixel at index Index at the 
           relative position Pos to the face */
		int NumCornersAt(const FIntPoint & Index, const EPixelPosition Pos);

        /* Clips the face to a given pixel. Returns true if the clipped area 
           has at least 3 vertices and false otherwise */
		bool ClipToPixel(const FVector & Index, float Increment, const TArray<FMeshToTerrainModelVertex> & Vrts, TArray<FVector> & ClippedArea);

        /* Computes the estimated area of a polygon*/
		float ApproximateArea(const FBox & PolygonBoundigBox, const TArray<FVector> & ClippedArea);
};
