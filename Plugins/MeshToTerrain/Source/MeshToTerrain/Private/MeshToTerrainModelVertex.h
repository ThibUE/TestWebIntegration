#pragma once

#include "MeshToTerrainPrivatePCH.h"

/* Wrapper class over a mesh vertex*/
class FMeshToTerrainModelVertex
{
	public:
		FVector Position;

	public:
		FMeshToTerrainModelVertex(FVector Pos);
};