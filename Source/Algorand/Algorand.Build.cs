// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class Algorand : ModuleRules
{
	public Algorand(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Engine",
				"CoreUObject",
				"Json", 
				"JsonUtilities", 
				"HTTP",
				"AlgorandAPI",
                "Blockchain",
				"Wallet",
                "Vertices",
				"Projects",
                "OpenSSL"
				// ... add other public dependencies that you statically link with here ...
			}
			);

	}
}
