#pragma once

#include "LandscapeFileFormatInterface.h"
#include "MeshToTerrainPrivatePCH.h"
#include "MeshToTerrainCache.h"

class FMeshToTerrainHeightmapFormat : public ILandscapeHeightmapFileFormat
{
  private:
	FLandscapeFileTypeInfo FileTypeInfo;
	mutable FMeshToTerrainCache      Cache;

  public:
	FMeshToTerrainHeightmapFormat();

	/* ILandscapeHeightmapFileFormat interface*/
	virtual const FLandscapeFileTypeInfo& GetInfo() const override;

	/** Validate a file for Imporrt
	 * Gives the file format the opportunity to reject a file or return warnings
	 * as well as return information about the file for the import UI (e.g. resolution and scale)
	 * @param HeightmapFilename path to the file to validate for import
	 * @return information about the file and (optional) error message
	 */
	virtual FLandscapeFileInfo Validate(const TCHAR* Filename, FName LayerName) const override;
	virtual FLandscapeHeightmapInfo Validate(const TCHAR* HeightmapFilename) const override;

	/** Import a file
	 * @param HeightmapFilename path to the file to import
	 * @param LayerName name of layer being imported (in case of Heightmap this will be NAME_None)
	 * @param ExpectedResolution resolution selected in the import UI (mostly for the benefit of .raw)
	 * @return imported data and (optional) error message
	 */
	virtual FLandscapeHeightmapImportData Import(const TCHAR* Filename, FName LayerName, FLandscapeFileResolution ExpectedResolution) const override;
	virtual FLandscapeHeightmapImportData Import(const TCHAR* HeightmapFilename, FLandscapeFileResolution ExpectedResolution) const override;

private:
	virtual TArray<FLandscapeFileResolution> CalculatePossibleRawResolutions(const FVector & MeshSize) const;

    /*  Checks whether the mesh width/height ratio is too large in order to indicate that 
        there might be a problem with the mesh orientation*/
    bool CheckAbnormalMeshSize(const FVector & MeshSize) const;
};