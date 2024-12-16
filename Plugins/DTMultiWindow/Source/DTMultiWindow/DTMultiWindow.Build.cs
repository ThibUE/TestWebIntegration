// Copyright 2023 Dexter.Wan. All Rights Reserved. 
// EMail: 45141961@qq.com

using UnrealBuildTool;

public class DTMultiWindow : ModuleRules
{
	public DTMultiWindow(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
				{
					"Core",
					"UMG"
				}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
				{
					"CoreUObject",
					"Engine",
					"Slate",
					"SlateCore",
				}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
				{
				}
			);
	}
}
