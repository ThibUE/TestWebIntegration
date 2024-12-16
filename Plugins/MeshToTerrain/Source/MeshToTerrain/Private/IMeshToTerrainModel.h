#pragma once

#include "MeshToTerrainModelVertex.h"
#include "MeshToTerrainModelFace.h"

// Shape model interface
class IMeshToTerrainModel
{
public:
	virtual void AddShape(FMeshToTerrainModelVertex Vertex) = 0;
	virtual void AddShape(FMeshToTerrainModelFace   Face) = 0;

};