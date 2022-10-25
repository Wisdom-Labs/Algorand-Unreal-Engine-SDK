// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class AlgorandLibrary : ModuleRules
{
	public AlgorandLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		AddEngineThirdPartyPrivateStaticDependencies(Target, "libcurl");

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			// Add the import library
			//
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "x64", "Release", "vertices.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "x64", "Release", "cjson.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Libs", "libsodium.lib"));

			// Delay-load the DLL, so we can load it from the right place first
			PublicDelayLoadDLLs.Add("vertices.dll");
			PublicDelayLoadDLLs.Add("cjson.dll");
			PublicDelayLoadDLLs.Add("libsodium.dll");

			// Ensure that the DLL is staged along with the executable
			RuntimeDependencies.Add("$(PluginDir)/Binaries/ThirdParty/AlgorandLibrary/Win64/vertices.dll");
			RuntimeDependencies.Add("$(PluginDir)/Binaries/ThirdParty/AlgorandLibrary/Win64/cjson.dll");
			RuntimeDependencies.Add("$(PluginDir)/Binaries/ThirdParty/AlgorandLibrary/Win64/libsodium.dll");
		}
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, "Mac", "Release", "libExampleLibrary.dylib"));
            RuntimeDependencies.Add("$(PluginDir)/Source/ThirdParty/AlgorandLibrary/Mac/Release/libExampleLibrary.dylib");
        }
        else if (Target.Platform == UnrealTargetPlatform.Linux)
		{
			string ExampleSoPath = Path.Combine("$(PluginDir)", "Binaries", "ThirdParty", "AlgorandLibrary", "Linux", "x86_64-unknown-linux-gnu", "libExampleLibrary.so");
			PublicAdditionalLibraries.Add(ExampleSoPath);
			PublicDelayLoadDLLs.Add(ExampleSoPath);
			RuntimeDependencies.Add(ExampleSoPath);
		}
	}
}
