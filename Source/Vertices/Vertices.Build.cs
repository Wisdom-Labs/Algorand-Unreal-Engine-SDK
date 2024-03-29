// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class Vertices : ModuleRules
{
    public Vertices(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        AddEngineThirdPartyPrivateStaticDependencies(Target, new string[] { "libcurl", "nghttp2" , "OpenSSL", "zlib" });   // add 3rd party lib for libcurl

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			// Add the import library
			//
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Libs", "libsodium.lib"));

			// Delay-load the DLL, so we can load it from the right place first
			PublicDelayLoadDLLs.Add("libsodium.dll");

			// Ensure that the DLL is staged along with the executable
			/*RuntimeDependencies.Add("$(PluginDir)/Binaries/ThirdParty/AlgorandLibrary/Win64/libsodium.dll");*/
			RuntimeDependencies.Add("$(PluginDir)/Source/Vertices/Libs/libsodium.dll");
		}

		PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "AlgorandLibrary",
                "Engine",
                "Projects"
            }
        );

    }
}
