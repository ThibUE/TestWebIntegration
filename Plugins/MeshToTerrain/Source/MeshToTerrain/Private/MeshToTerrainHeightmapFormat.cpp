#include "MeshToTerrainHeightmapFormat.h"
#include "MeshToTerrainParserFactory.h"
#include "MeshToTerrainRuntimeSettings.h"

#define LOCTEXT_NAMESPACE "MeshToTerrain_loctext_namespace"

FMeshToTerrainHeightmapFormat::FMeshToTerrainHeightmapFormat()
{
	FileTypeInfo.Description = LOCTEXT("FileFormatRaw_HeightmapDesc", "Heightmap .obj/.fbx files");
	FileTypeInfo.Extensions.Add(".obj");
	FileTypeInfo.Extensions.Add(".fbx");
	FileTypeInfo.bSupportsExport = false;
}

const FLandscapeFileTypeInfo& FMeshToTerrainHeightmapFormat::GetInfo() const 
{
	return FileTypeInfo;
}

TArray<FLandscapeFileResolution> FMeshToTerrainHeightmapFormat::CalculatePossibleRawResolutions(const FVector & MeshSize) const
{
	TArray<FLandscapeFileResolution> PossibleResolutions;
	float                            ScaleRatio;
	bool                             bIsScaledOnX = (MeshSize.X > MeshSize.Y);
	const   TArray<int>              PossibleRawSizes = { 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192 };
	unsigned int                     ScaleFactor;
	FIntPoint                        Scale;
	FLandscapeFileResolution         ImportResolution;

	if ((MeshSize.X <= 0) || (MeshSize.Y <= 0))
	{
		return PossibleResolutions;
	}

	if (bIsScaledOnX)
	{
		ScaleRatio = MeshSize.Y / MeshSize.X;
	}
	else
	{
		ScaleRatio = MeshSize.X / MeshSize.Y;
	}

	ScaleFactor = floorf(1 / ScaleRatio);
	if (ScaleFactor == 0)
	{
		ScaleFactor = 1;
	}

	// Round the ScaleFactor down to the first power of 2
	// smaller than equal to it
	ScaleFactor = ScaleFactor | (ScaleFactor >> 1);
	ScaleFactor = ScaleFactor | (ScaleFactor >> 2);
	ScaleFactor = ScaleFactor | (ScaleFactor >> 4);
	ScaleFactor = ScaleFactor | (ScaleFactor >> 8);
	ScaleFactor = ScaleFactor | (ScaleFactor >> 16);
	ScaleFactor = ScaleFactor - (ScaleFactor >> 1);

	if (bIsScaledOnX)
	{
		Scale.X = 1;
		Scale.Y = ScaleFactor;
	}
	else
	{
		Scale.X = ScaleFactor;
		Scale.Y = 1;
	}

	for (int i = 0; i < PossibleRawSizes.Num(); i++) 
	{
		if ((PossibleRawSizes[i] / ScaleFactor) >= (unsigned int)PossibleRawSizes[0])
		{
			ImportResolution.Width  = PossibleRawSizes[i] / Scale.X;
			ImportResolution.Height = PossibleRawSizes[i] / Scale.Y;
			PossibleResolutions.Add(ImportResolution);
		}
	}

	return PossibleResolutions;
}

FLandscapeFileInfo FMeshToTerrainHeightmapFormat::Validate(const TCHAR* Filename, FName LayerName) const
{
	return Validate(Filename);
}

FLandscapeHeightmapInfo FMeshToTerrainHeightmapFormat::Validate(const TCHAR* HeightmapFilename) const 
{
	FLandscapeHeightmapInfo Result;
	unsigned MaxInputSize = GetDefault<UMeshToTerrainRuntimeSettings>()->MaximumInputFileSize;
	int64 ImportFileSize = IFileManager::Get().FileSize(HeightmapFilename);

	// Basic sanity checks
	if (ImportFileSize < 0)
	{
		Result.ResultCode = ELandscapeImportResult::Error;
		Result.ErrorMessage = LOCTEXT("Import_HeightmapFileReadError", "Error reading heightmap file");
	}
	else if (ImportFileSize == 0)
	{
		Result.ResultCode = ELandscapeImportResult::Error;
		Result.ErrorMessage = LOCTEXT("Import_HeightmapFileInvalidSize", "The heightmap file has an invalid size (possibly not 16-bit?)");
	}
	else if (ImportFileSize >= MaxInputSize * 1024 * 1024)
	{
		Result.ResultCode = ELandscapeImportResult::Error;
		Result.ErrorMessage = FText::Format(LOCTEXT("Import_HeightmapFileInvalidSize", "File too large (Max {0}MB). You can increase the size by going to ProjectSettings->Plugins->MeshToTerrain"), MaxInputSize);
	}
	else
	{
		const  TSharedPtr<IMeshToTerrainParser> Parser = FMeshToTerrainParserFactory::CreateParser(HeightmapFilename);
		if (!Parser.IsValid())
		{
			Result.ResultCode = ELandscapeImportResult::Error;
			Result.ErrorMessage = LOCTEXT("Import_HeightmapFileInvalidSize", "Internal error: can't create a parser instance");
		}

		FVector MeshBoundingBoxSize;
		if (Parser->ParseFile(MeshBoundingBoxSize) == 0)
		{
			Result.ResultCode = ELandscapeImportResult::Error;
			Result.ErrorMessage = LOCTEXT("Import_HeightmapFileInvalidSize", "Can't parse input file");
			return Result;
		}

		if (CheckAbnormalMeshSize(MeshBoundingBoxSize))
		{
			Result.ResultCode = ELandscapeImportResult::Warning;
			Result.ErrorMessage = LOCTEXT("Import_HeightmapFileInvalidSize", "Mesh width/height ratio is unusual. Please check the mesh orientation if not expected.");
		}
		Result.PossibleResolutions = CalculatePossibleRawResolutions(MeshBoundingBoxSize);

		if (Result.PossibleResolutions.Num() == 0)
		{
			Result.ResultCode = ELandscapeImportResult::Error;
			Result.ErrorMessage = LOCTEXT("Import_HeightmapFileInvalidSize", "Heightmap resolution can't be computed to due invalid mesh size");
		}
	}

	return Result;
}

FLandscapeHeightmapImportData FMeshToTerrainHeightmapFormat::Import(const TCHAR* Filename, FName LayerName, FLandscapeFileResolution ExpectedResolution) const
{
	return Import(Filename, ExpectedResolution);
}

FLandscapeHeightmapImportData FMeshToTerrainHeightmapFormat::Import(const TCHAR* HeightmapFilename, FLandscapeFileResolution ExpectedResolution) const
{
	FLandscapeHeightmapImportData   Result;
	FMeshToTerrainModel               Model = FMeshToTerrainModel();
	unsigned                        MaxInputSize = GetDefault<UMeshToTerrainRuntimeSettings>()->MaximumInputFileSize;

	int64 ImportFileSize = IFileManager::Get().FileSize(HeightmapFilename);

	// Basic Sanity Checks
	if (ImportFileSize < 0)
	{
		Result.ResultCode = ELandscapeImportResult::Error;
		Result.ErrorMessage = LOCTEXT("Import_HeightmapFileReadError", "Error reading heightmap file");
	}
	else if (ImportFileSize == 0)
	{
		Result.ResultCode = ELandscapeImportResult::Error;
		Result.ErrorMessage = LOCTEXT("Import_HeightmapFileInvalidSize", "The heightmap file has an invalid size (possibly not 16-bit?)");
	}
	else if (ImportFileSize >= MaxInputSize * 1024 * 1024)
	{
		Result.ResultCode = ELandscapeImportResult::Error;
		Result.ErrorMessage = FText::Format(LOCTEXT("Import_HeightmapFileInvalidSize", "File too large (Max {0}MB). You can increase the size by going to ProjectSettings->Plugins->MeshToTerrain"), MaxInputSize);
	}
	else
	{
		FIntPoint ExpectedSize = FIntPoint(ExpectedResolution.Width, ExpectedResolution.Height);
		if (Cache.ReadData(HeightmapFilename, ExpectedSize, Result.Data))
		{
			return Result;
		}

		const  TSharedPtr<IMeshToTerrainParser> Parser = FMeshToTerrainParserFactory::CreateParser(HeightmapFilename);
		if (!Parser.IsValid())
		{
			Result.ResultCode = ELandscapeImportResult::Error;
			Result.ErrorMessage = LOCTEXT("Import_HeightmapFileInvalidSize", "Internal error: can't create a parser instance");
		}

		if (Parser->ParseFile(Model) == 0)
		{
			Result.ResultCode = ELandscapeImportResult::Error;
			Result.ErrorMessage = LOCTEXT("Import_HeightmapFileInvalidSize", "Can't parse input file");
			return Result;
		}
	}

	
	if (Result.ResultCode == ELandscapeImportResult::Success)
	{
		FIntPoint ExpectedSize = FIntPoint(ExpectedResolution.Width, ExpectedResolution.Height);
		Model.ConvertData(Result.Data, ExpectedSize);
		Cache.CacheData(HeightmapFilename, ExpectedSize, Result.Data);
	}
	return Result;
}

#undef LOCTEXT_NAMESPACE

bool FMeshToTerrainHeightmapFormat::CheckAbnormalMeshSize(const FVector & MeshSize) const
{   
	// Strict comparison with 0 is fine as we support very small mesh sizes
	if ((MeshSize.X <= 0) || (MeshSize.Y <= 0))
		return true;

	return (fmaxf(MeshSize.X, MeshSize.Y) / fminf(MeshSize.X, MeshSize.Y) > 4);
}
