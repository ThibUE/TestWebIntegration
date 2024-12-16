#pragma once

#include "MeshToTerrainPrivatePCH.h"
#include "IMeshToTerrainParser.h"

class FMeshToTerrainParserFactory
{
public:
	static TSharedPtr<IMeshToTerrainParser> CreateParser(const TCHAR * Filename);
};