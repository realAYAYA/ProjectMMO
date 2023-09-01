// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
using System;

public class MySQLPlugin : ModuleRules
{
    public MySQLPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
        //libmysql.lib 动态库 mysqlclient.lib 静态库 编辑器模式下可以运行，但打包不可以
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "../../ThirdParty/include"));
        //PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "../../ThirdParty/lib", "mysqlclient.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "../../ThirdParty/lib", "libmysql.lib"));
        PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, "../../ThirdParty/bin", "libmysql.dll"));

        PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...	
			}
			);
	}
}
