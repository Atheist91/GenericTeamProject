// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class GenericTeamProjectEditor : ModuleRules
{
	public GenericTeamProjectEditor(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

        // Game module
        PublicDependencyModuleNames.AddRange(new string[] { "GenericTeamProject" });

        // For custom pin
        PublicDependencyModuleNames.AddRange(new string[] { "UnrealEd", "BlueprintGraph", "GraphEditor", "KismetCompiler",
                                                            "PropertyEditor", "SlateCore", "Slate", "EditorStyle", "Kismet" });

        PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");
		// if ((Target.Platform == UnrealTargetPlatform.Win32) || (Target.Platform == UnrealTargetPlatform.Win64))
		// {
		//		if (UEBuildConfiguration.bCompileSteamOSS == true)
		//		{
		//			DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
		//		}
		// }
	}
}
