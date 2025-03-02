// Copyright Epic Games, Inc. All Rights Reserved.

#include "WindowsDualsense_ds5w/Public/WindowsDualsense_ds5w.h"
#include "InputCoreTypes.h"
#include "Runtime/ApplicationCore/Public/GenericPlatform/IInputInterface.h"
#include "Misc/Paths.h"
#include "FDualSenseInputDevice.h"

#define LOCTEXT_NAMESPACE "FWindowsDualsense_ds5wModule"
#define MAX_CONTROLLERS_SUPPORTED 8

void FWindowsDualsense_ds5wModule::StartupModule()
{
	IModularFeatures::Get().RegisterModularFeature(IInputDeviceModule::GetModularFeatureName(), this);

	FString EnginePluginPath = FPaths::Combine(FPaths::EnginePluginsDir(), TEXT("WindowsDualsense_ds5w/Binaries/Win64/ds5w_x64.dll"));
	if (EnginePluginPath.IsEmpty())
	{
		EnginePluginPath = FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("WindowsDualsense_ds5w/Binaries/Win64/ds5w_x64.dll"));
		DS5WdllHandle = FPlatformProcess::GetDllHandle(*EnginePluginPath);
	}
	else
	{
		DS5WdllHandle = FPlatformProcess::GetDllHandle(*EnginePluginPath);
	}

	RegisterCustomKeys();
	DualSenseLibraryInstance = NewObject<UDualSenseLibrary>();
	DualSenseLibraryInstance->InitializeLibrary();
}

void FWindowsDualsense_ds5wModule::ShutdownModule()
{
	if (DualSenseLibraryInstance)
	{
		DualSenseLibraryInstance->ShutdownLibrary();
		DualSenseLibraryInstance = nullptr;
	}

	if (DS5WdllHandle)
	{
		FPlatformProcess::FreeDllHandle(DS5WdllHandle);
		DS5WdllHandle = nullptr;
	}
}

TSharedPtr<IInputDevice> FWindowsDualsense_ds5wModule::CreateInputDevice(
	const TSharedRef<FGenericApplicationMessageHandler>& InCustomMessageHandler)
{
	DeviceInstances = MakeShareable(new FDualSenseInputDevice(InCustomMessageHandler));

	for (int32 i = 0; i < DualSenseLibraryInstance->ControllersCount; i++)
	{
		FPlatformUserId UserId;
		FInputDeviceId InputDeviceId;
		if (i > 0)
		{
			UserId = DeviceInstances->AllocateNewUserId();
			InputDeviceId = DeviceInstances->AllocateNewInputDeviceId();
		}
		else
		{
			UserId = FPlatformUserId::CreateFromInternalId(i);
			InputDeviceId = FInputDeviceId::CreateFromInternalId(i);
		}


		EInputDeviceConnectionState ConnectionState = EInputDeviceConnectionState::Disconnected;
		if (DualSenseLibraryInstance->IsConnected(i))
		{
			ConnectionState = EInputDeviceConnectionState::Connected;
		}

		FPlatformInputDeviceState State;
		State.OwningPlatformUser = UserId;
		State.ConnectionState = ConnectionState;
		DeviceInstances->SetController(InputDeviceId, State);

		if (DeviceInstances->RemapUserAndDeviceToControllerId(UserId, i, InputDeviceId))
		{
			UE_LOG(LogTemp, Log, TEXT("Success mapper Device registred: %d and User %d"), i, UserId.GetInternalId());
		}
	}

	return DeviceInstances;
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
