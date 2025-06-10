// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025


#include "WindowsDualsense_ds5w/Public/WindowsDualsense_ds5w.h"

#include "InputCoreTypes.h"
#include "Misc/Paths.h"
#include "FDualSenseInputDevice.h"
#include "FDualSenseLibraryManager.h"
#include "Microsoft/AllowMicrosoftPlatformTypes.h"
#include <stdio.h>
#define LOCTEXT_NAMESPACE "FWindowsDualsense_ds5wModule"
#define MAX_CONTROLLERS_SUPPORTED 16

void FWindowsDualsense_ds5wModule::StartupModule()
{
	IModularFeatures::Get().RegisterModularFeature(IInputDeviceModule::GetModularFeatureName(), this);
	RegisterCustomKeys();
}

void FWindowsDualsense_ds5wModule::ShutdownModule()
{
}

TSharedPtr<IInputDevice> FWindowsDualsense_ds5wModule::CreateInputDevice(
	const TSharedRef<FGenericApplicationMessageHandler>& InCustomMessageHandler)
{
	DeviceInstance = MakeShareable(new FDualSenseInputDevice(InCustomMessageHandler));

	const UFDualSenseLibraryManager* DualSenseLibraryManager = UFDualSenseLibraryManager::Get();
	if (!DualSenseLibraryManager)
	{
		UE_LOG(LogTemp, Error, TEXT("DualSense: Failed to create DualSense Library Manager"));
		return DeviceInstance;
	}

	DualSenseLibraryManager->CreateLibraryInstances();
	for (int32 i = 0; i < DualSenseLibraryManager->GetAllocatedDevices(); i++)
	{
		if (i < 1) continue;
		
		DeviceInstance->SetController(FInputDeviceId::CreateFromInternalId(i));
	}
	
	return DeviceInstance;
}

void FWindowsDualsense_ds5wModule::RegisterCustomKeys()
{
	const FKey Mic("PS_Mic");
	const FKey Menu("PS_Menu");
	const FKey Shared("PS_Share");
	const FKey TouchButtom("PS_TouchButtom");
	const FKey PlayStationButton("PS_Button");
	const FKey PS_PushLeftStick("PS_PushLeftStick");
	const FKey PS_PushRightStick("PS_PushRightStick");
	

	EKeys::AddKey(FKeyDetails(
		PS_PushLeftStick,
		FText::FromString("PS Push Left Stick"),
		FKeyDetails::GamepadKey
	));

	EKeys::AddKey(FKeyDetails(
		PS_PushRightStick,
		FText::FromString("PS Push Right Stick"),
		FKeyDetails::GamepadKey
	));

	EKeys::AddKey(FKeyDetails(
		Shared,
		FText::FromString("PlayStation Share"),
		FKeyDetails::GamepadKey
	));

	EKeys::AddKey(FKeyDetails(
		Menu,
		FText::FromString("PlayStation Menu"),
		FKeyDetails::GamepadKey
	));

	EKeys::AddKey(FKeyDetails(
		PlayStationButton,
		FText::FromString("PlayStation Button"),
		FKeyDetails::GamepadKey
	));

	EKeys::AddKey(FKeyDetails(
		Mic,
		FText::FromString("PlayStation Mic"),
		FKeyDetails::GamepadKey
	));

	EKeys::AddKey(FKeyDetails(
		TouchButtom,
		FText::FromString("PlayStation Touch Buttom"),
		FKeyDetails::GamepadKey
	));
}

IMPLEMENT_MODULE(FWindowsDualsense_ds5wModule, WindowsDualsense_ds5w)
