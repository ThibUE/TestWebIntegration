#pragma once

#include "MeshToTerrainPrivatePCH.h"

/* Wrapper class containg info about a pixel covered by mesh face*/
class FMeshToTerrainModelPixel
{
	public:
		int       Id;
		FIntPoint Position;     /* Pixel position in the heightmap*/
		float     ActualHeight; /* The height this pixel contributes to the heightmap pixel */
		int       Ratio;        /* The ratio of the heightmap pixel this pixel covers*/
	public:
		FMeshToTerrainModelPixel(FIntPoint & Position, float MeshHeight, int Ratio, int Id);
};