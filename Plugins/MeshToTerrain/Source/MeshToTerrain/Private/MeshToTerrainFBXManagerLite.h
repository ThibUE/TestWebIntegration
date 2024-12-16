#pragma once

#include "MeshToTerrainPrivatePCH.h"
#include <fbxsdk.h>

class FMeshToTerrainFBXManagerLite
{
private:
	const TCHAR * Filename;
	FbxManager * Manager = nullptr;
	FbxScene   * Scene   = nullptr;

public:
	FMeshToTerrainFBXManagerLite(const TCHAR *Filename);
	~FMeshToTerrainFBXManagerLite();

	bool Init();

	/* Extracts the vertices of the first found mesh */
	void ExtractMeshVertices(TArray<FVector> & Vrtx);

	/* Extracts the faces of the first found mesh */
	void ExtractMeshFaces(TArray<TArray<int>> & Faces);

private:
	/* Finds the first mesh node by doing a DFS search from the root node*/
	FbxNode * FindFirstMesh(FbxNode * Root);
};