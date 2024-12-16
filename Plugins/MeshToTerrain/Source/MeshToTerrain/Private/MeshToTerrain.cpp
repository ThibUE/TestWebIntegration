// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "MeshToTerrain.h"
#include "ISettingsModule.h"
#include "LandscapeFileFormatInterface.h"
#include "LandscapeEditorModule.h"
#include "MeshToTerrainHeightmapFormat.h"
#include "MeshToTerrainRuntimeSettings.h"

#define LOCTEXT_NAMESPACE "FMeshToTerrainModule"

void FMeshToTerrainModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	TSharedRef<ILandscapeHeightmapFileFormat> MyInstance(new FMeshToTerrainHeightmapFormat());
	FModuleManager & ModuleManager = FModuleManager::Get();
	bool bExists = ModuleManager.ModuleExists(TEXT("LandscapeEditor"));
	FName ModuleName = FName(TEXT("LandscapeEditor"));
	IModuleInterface * LandscapeEditor;
	ILandscapeEditorModule * LandscapeModule;

	if (!ModuleManager.IsModuleLoaded(ModuleName))
	{
		FModuleStatus ModuleStatus;
		TArray<FName> OutModules;
		EModuleLoadResult Reason;
		ModuleManager.FindModules(TEXT("*"), OutModules);
		LandscapeEditor = ModuleManager.LoadModuleWithFailureReason(ModuleName, Reason);
		bExists = ModuleManager.QueryModule(ModuleName, ModuleStatus);
	}
	else
	{
		LandscapeEditor = ModuleManager.GetModule(ModuleName);
	}

	if (bExists)
	{
		LandscapeModule = (ILandscapeEditorModule *)(LandscapeEditor);
		LandscapeModule->RegisterHeightmapFileFormat(MyInstance);
	}

	RegisterSettings();
}

void FMeshToTerrainModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	UnregisterSettings();
}

void FMeshToTerrainModule::RegisterSettings()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "MeshToTerrain",
			LOCTEXT("RuntimeSettingsName", "MeshToTerrain"),
			LOCTEXT("RuntimeSettingsDescription", "Configure the MeshToTerrain plugin"),
			GetMutableDefault<UMeshToTerrainRuntimeSettings>());
	}
}

void FMeshToTerrainModule::UnregisterSettings()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "MeshToTerrain");
	}

}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMeshToTerrainModule, MeshToTerrain)