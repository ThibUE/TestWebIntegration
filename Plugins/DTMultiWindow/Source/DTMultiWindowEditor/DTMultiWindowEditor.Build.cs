// Copyright 2023 Dexter.Wan. All Rights Reserved. 
// EMail: 45141961@qq.com

using UnrealBuildTool;

public class DTMultiWindowEditor : ModuleRules
{
    public DTMultiWindowEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", 
                
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "UnrealEd",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "UMG",
                "UMGEditor", 
                "GraphEditor",
                "BlueprintGraph",
                "KismetCompiler",
                "Blutility",
                "AssetTools",
                "DTMultiWindow", 
            }
        );
    }
}