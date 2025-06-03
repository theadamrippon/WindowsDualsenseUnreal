// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

using System.IO;
using UnrealBuildTool;

public class WindowsDualsense_ds5w : ModuleRules
{
	public WindowsDualsense_ds5w(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
 		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "ApplicationCore", "InputCore", "InputDevice"});
	    PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
	    if (Target.Platform == UnrealTargetPlatform.Win64)
	    {
		    PublicAdditionalLibraries.Add("hid.lib");
		    PublicDefinitions.Add("PLATFORM_SUPPORTS_HID=1");
		}
	}
}