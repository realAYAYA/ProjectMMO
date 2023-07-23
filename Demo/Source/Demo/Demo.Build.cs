// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Demo : ModuleRules
{
	public Demo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"HeadMountedDisplay", 
			
			"EnhancedInput", 
			"GameplayAbilities",
			
			"GameTables"
		});
		
		
		PublicIncludePaths.Add("Demo/");
		
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"GameplayAbilities", 
			"GameplayTags", 
			"GameplayTasks",
			
			"UMG",
			"Slate",
			"SlateCore",
		});
	}
}
