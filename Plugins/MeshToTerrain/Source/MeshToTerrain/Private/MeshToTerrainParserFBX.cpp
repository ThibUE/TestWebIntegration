#include "MeshToTerrainParserFBX.h"
#include "MeshToTerrainFBXManagerLite.h"


FMeshToTerrainParserFBX::FMeshToTerrainParserFBX(const TCHAR * Filename)
	: Filename(Filename)
{}

int FMeshToTerrainParserFBX::ParseFile(FVector & BoundingBoxSize) const
{
	FMeshToTerrainFBXManagerLite FbxManager = FMeshToTerrainFBXManagerLite(Filename);
	TArray<FVector> Vertices;

	FbxManager.Init();
	FbxManager.ExtractMeshVertices(Vertices);

	if (Vertices.Num() < 3)
		return 0;
	FBox MeshBoundingBox = FBox(Vertices);

	BoundingBoxSize = MeshBoundingBox.GetSize();

	return 1;
}

int FMeshToTerrainParserFBX::ParseFile(FMeshToTerrainModel & Model) const
{
	FMeshToTerrainFBXManagerLite FbxManager = FMeshToTerrainFBXManagerLite(Filename);
	TArray<FVector> Vertices;

	FbxManager.Init();
	FbxManager.ExtractMeshVertices(Vertices);

    if (Vertices.Num() < 3)
    {
        return 0;
    }

    for (auto & Elem : Vertices)
    {
        Model.AddShape(Elem);
    }
	
	TArray<TArray<int>> Faces;
	FbxManager.ExtractMeshFaces(Faces);

    for (auto & Elem : Faces)
    {
        Model.AddShape(Elem);
    }

	return 1;
}
