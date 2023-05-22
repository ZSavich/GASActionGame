// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GASActionGame : ModuleRules
{
	public GASActionGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"HeadMountedDisplay", 
			"EnhancedInput",
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks",
			"MotionWarping"
		});
		
		PublicIncludePaths.AddRange(new string[]
		{
			"GASActionGame/",
		});
	}
}
