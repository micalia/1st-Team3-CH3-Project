// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ThreeFPS : ModuleRules
{
	public ThreeFPS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "NavigationSystem", "AIModule", "GameplayTasks" });

		PublicIncludePaths.AddRange(new string[] { "ThreeFPS" });
	}
}
