#pragma once

#include "MeshToTerrainPrivatePCH.h"

/* Given that the Landscape editor module performs redundant reads
   of the input file, a simple caching mechanism is employed.
*/
class FMeshToTerrainCache
{
	public:
		/* Reads the cached heightmap of the file Filename for the size Size.
		   Returns true if there's cached data and false otherwise */
		bool ReadData(const TCHAR * Filename, const FIntPoint & Size, TArray<uint16> & Data);

		/* Caches the content of the file for a specific size*/
		void CacheData(const TCHAR * Filename, const FIntPoint & Size, const TArray<uint16> & Data);

	private:
		
		/* Private class for recording the cached data*/
		class FMeshToTerrainCacheLine
		{
			public:
				FDateTime ModifiedDate;
				TArray<uint16> Data;

			public:
				FMeshToTerrainCacheLine(const FDateTime & Time, const TArray<uint16> & Data)
					: ModifiedDate(Time), Data(Data) {};

		};

	private:
		TMap<FIntPoint, FMeshToTerrainCacheLine> CacheLines;
		FString CurrentFilename = FString(TEXT(""));
};