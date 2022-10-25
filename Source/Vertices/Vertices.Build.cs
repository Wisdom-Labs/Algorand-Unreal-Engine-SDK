// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Vertices : ModuleRules
{
    public Vertices(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "AlgorandLibrary"
            }
            );
    }
}
