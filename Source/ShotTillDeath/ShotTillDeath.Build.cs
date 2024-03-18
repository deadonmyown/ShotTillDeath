// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ShotTillDeath : ModuleRules
{
	public ShotTillDeath(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
