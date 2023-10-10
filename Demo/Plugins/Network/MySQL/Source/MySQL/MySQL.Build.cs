// Some copyright should be here...

using UnrealBuildTool;
using System.IO;

public class MySQL : ModuleRules
{
	public void CopyToProjectBinaries(string FilePath, ReadOnlyTargetRules Target)
	{
		string PluginBinariesPath = Path.Combine(ModuleDirectory, "../../Binaries", Target.Platform.ToString());
		string ProjectBinariesPath = Path.Combine(ModuleDirectory, "../../../../Binaries", Target.Platform.ToString());
		
		string FileName = Path.GetFileName(FilePath);
		string PluginFullFileName = Path.Combine(PluginBinariesPath, FileName);

		if (Directory.Exists(PluginBinariesPath) && !File.Exists(PluginFullFileName))
		{
			File.Copy(FilePath, PluginFullFileName);
		}
		
		string ProjectFullFileName = Path.Combine(ProjectBinariesPath, FileName);
		if (Directory.Exists(ProjectBinariesPath) && !File.Exists(ProjectFullFileName))
		{
			File.Copy(FilePath, ProjectFullFileName);
		}
	}
	
	public MySQL(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "../../ThirdParty/include"));
        
		// Select file name by platform
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			string DllPath = Path.Combine(ModuleDirectory, "../../ThirdParty/lib", "libmysql.dll");
			
			CopyToProjectBinaries(DllPath, Target);
			
			RuntimeDependencies.Add(DllPath);
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "../../ThirdParty/lib", "libmysql.lib"));
		}
		
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
				//"Slate",
				//"SlateCore",
				// ... add private dependencies that you statically link with here ...	
			}
		);
	}
}
