// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MechanicsHW : ModuleRules
{
	public MechanicsHW(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"MechanicsHW",
			"MechanicsHW/Variant_Platforming",
			"MechanicsHW/Variant_Platforming/Animation",
			"MechanicsHW/Variant_Combat",
			"MechanicsHW/Variant_Combat/AI",
			"MechanicsHW/Variant_Combat/Animation",
			"MechanicsHW/Variant_Combat/Gameplay",
			"MechanicsHW/Variant_Combat/Interfaces",
			"MechanicsHW/Variant_Combat/UI",
			"MechanicsHW/Variant_SideScrolling",
			"MechanicsHW/Variant_SideScrolling/AI",
			"MechanicsHW/Variant_SideScrolling/Gameplay",
			"MechanicsHW/Variant_SideScrolling/Interfaces",
			"MechanicsHW/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
