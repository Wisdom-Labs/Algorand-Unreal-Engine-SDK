// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class AlgorandAPI : ModuleRules
{
	public AlgorandAPI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		AddEngineThirdPartyPrivateStaticDependencies(Target, new string[] { "libcurl", "nghttp2" , "OpenSSL", "zlib" });   // add 3rd party lib for libcurl
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Projects",
				"Vertices",
				"Wallet",
				"Json",
				"JsonUtilities"
			}
			);

	}
}
