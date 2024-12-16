#pragma once

#include "MeshToTerrainPrivatePCH.h"
#include "IMeshToTerrainParser.h"
#include "MeshToTerrainModel.h"
#include <fbxsdk.h>


class FMeshToTerrainParserFBX : public IMeshToTerrainParser
{
private:
	const TCHAR * Filename;

public :
	FMeshToTerrainParserFBX(const TCHAR * Filename);

    /* IMeshToTerrainParser interface methods */
	virtual int   ParseFile(FVector & BoundingBoxSize) const override;
	virtual int   ParseFile(FMeshToTerrainModel & Model) const override;

};