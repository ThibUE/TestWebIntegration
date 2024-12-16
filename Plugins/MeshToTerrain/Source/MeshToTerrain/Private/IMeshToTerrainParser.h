#pragma once

#include "MeshToTerrainModel.h"

// Parser interface
class IMeshToTerrainParser
{

public:
    /* Parses a file and sets the size of the mesh's bounding box in BoundingBoxSize.
       Returns 0 if there's an error while parsing the file and a positive value otherwise*/
	virtual int ParseFile(FVector & BoundingBoxSize) const = 0;

    /* Parses a file and populates the model with the mesh's vertices and faces.
       Returns 0 if there's an error while parsing the file and a positive value otherwise*/
	virtual int ParseFile(FMeshToTerrainModel & Model) const = 0;

};