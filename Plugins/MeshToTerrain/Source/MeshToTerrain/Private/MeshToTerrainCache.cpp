#include "MeshToTerrainCache.h"
#include "Runtime/Core/Public/HAL/FileManager.h"

bool FMeshToTerrainCache::ReadData(const TCHAR * Filename, const FIntPoint & Size, TArray<uint16>& Data)
{
	if (Filename == nullptr)
	{
		return false;
	}

	/* For simplicity we cache data only for one file */
	if (!CurrentFilename.Equals(FString(Filename)))
	{
		return false;
	}

	FDateTime AccessDate          = IFileManager::Get().GetTimeStamp(Filename);
	FMeshToTerrainCacheLine *Line = CacheLines.Find(Size);
	
	if ((Line != nullptr) && (Line->ModifiedDate == AccessDate))
	{
		Data = Line->Data;
		return true;
	}

	return false;
}

void FMeshToTerrainCache::CacheData(const TCHAR * Filename, const FIntPoint & Size, const TArray<uint16>& Data)
{
	if (Filename == nullptr)
	{
		return;
	}

	/* For simplicity we cache data only for one file */
	if (!CurrentFilename.Equals(FString(Filename)))
	{
		CacheLines.Empty();
		this->CurrentFilename = FString(Filename);
	}

	FDateTime AccessDate         = IFileManager::Get().GetTimeStamp(Filename);
	FMeshToTerrainCacheLine * Line = CacheLines.Find(Size);
	if (Line == nullptr)
	{
		CacheLines.Add(Size, FMeshToTerrainCacheLine(AccessDate, Data));
	}
	else
	{
		// Callers responsability not to cache already cached data
		Line->ModifiedDate  = AccessDate;
		Line->Data          = Data;
	}
}
