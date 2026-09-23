// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class graphics_game_engine : ModuleRules
{
	public graphics_game_engine(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Niagara" });
	}
}
