// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

using UnrealBuildTool;
using System.IO;

public class WindowsDualsense_ds5w : ModuleRules
{
	public WindowsDualsense_ds5w(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
        var thirdPartyPath = Path.Combine(ModuleDirectory, "ThirdParty/DualSenseWindows_V0.1");
        PublicIncludePaths.Add(thirdPartyPath);
        PublicAdditionalLibraries.Add(Path.Combine(thirdPartyPath, "ds5w_x64.lib"));

        var dllFullPath = Path.Combine(thirdPartyPath, "ds5w_x64.dll");
        RuntimeDependencies.Add("$(BinaryOutputDir)/ds5w_x64.dll", dllFullPath);
        
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "ApplicationCore", "InputCore", "InputDevice" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
	}
}