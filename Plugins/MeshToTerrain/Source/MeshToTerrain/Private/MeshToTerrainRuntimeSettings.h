// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "MeshToTerrainRuntimeSettings.generated.h"

/**
* Implements the settings for the MeshToTerrain plugin.
*/
UCLASS(config = Engine, defaultconfig)
class UMeshToTerrainRuntimeSettings : public UObject
{
	GENERATED_UCLASS_BODY()

	// Indicates the maximum size in MB the mesh file can have
	UPROPERTY(EditAnywhere, config, Category = Settings)
	unsigned  MaximumInputFileSize;
};
