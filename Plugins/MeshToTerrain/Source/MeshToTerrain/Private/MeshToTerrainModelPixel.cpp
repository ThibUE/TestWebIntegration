#include "MeshToTerrainModelPixel.h"

FMeshToTerrainModelPixel::FMeshToTerrainModelPixel(FIntPoint & Position, float MeshHeight, int Ratio, int Id)
	: Id(Id), Position(Position), ActualHeight(MeshHeight), Ratio(Ratio)
{}
