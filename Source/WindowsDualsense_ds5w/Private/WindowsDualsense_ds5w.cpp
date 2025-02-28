// Copyright Epic Games, Inc. All Rights Reserved.

#include "WindowsDualsense_ds5w/Public/WindowsDualsense_ds5w.h"
#include "InputCoreTypes.h"
#include "Runtime/ApplicationCore/Public/GenericPlatform/IInputInterface.h"
#include "Misc/Paths.h"

#include "FDualSenseInputDevice.h"
#define LOCTEXT_NAMESPACE "FWindowsDualsense_ds5wModule"
#define MAX_CONTROLLERS_SUPPORTED 8

TSharedPtr<IInputDevice> FWindowsDualsense_ds5wModule::CreateInputDevice(
	const TSharedRef<FGenericApplicationMessageHandler>& InCustomMessageHandler)
{
	DeviceInstances = MakeShareable(new FDualSenseInputDevice(InCustomMessageHandler));
	
	for (int32 i = 0; i < DualSenseLibraryInstance->ControllersCount; i++)
	{
		FPlatformUserId UserId = i > 0 ? DeviceInstances->AllocateNewUserId() : FPlatformUserId::CreateFromInternalId(i);
		FInputDeviceId InputDeviceId = i > 0 ? DeviceInstances->AllocateNewInputDeviceId() : FInputDeviceId::CreateFromInternalId(i);

		const EInputDeviceConnectionState ConnectionState =
			DualSenseLibraryInstance->IsConnected(i)
				? EInputDeviceConnectionState::Connected
				: EInputDeviceConnectionState::Disconnected;

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
	// Definindo uma nova Key
	const FKey PlayStationButton("PS_Button"); 
	const FKey Mic("PS_Mic"); 
	const FKey TouchButtom("PS_TouchButtom"); 
	const FKey Shared("PS_Share"); 
	const FKey Menu("PS_Menu"); 
	const FKey PS_PushLeftStick("PS_PushLeftStick"); 
	const FKey PS_PushRightStick("PS_PushRightStick"); 

	// Registrando a Key no Input Manager
	EKeys::AddKey(FKeyDetails(
		PS_PushLeftStick,
		FText::FromString("PS Push Left Stick"),  // Nome amigável
		FKeyDetails::GamepadKey                  // Tipo da Key (Gamepad, no caso)
	));

	EKeys::AddKey(FKeyDetails(
		PS_PushRightStick,
		FText::FromString("PS Push Right Stick"),  // Nome amigável
		FKeyDetails::GamepadKey                  // Tipo da Key (Gamepad, no caso)
	));
	
	EKeys::AddKey(FKeyDetails(
		Shared,
		FText::FromString("PlayStation Share"),  // Nome amigável
		FKeyDetails::GamepadKey                  // Tipo da Key (Gamepad, no caso)
	));

	EKeys::AddKey(FKeyDetails(
		Menu,
		FText::FromString("PlayStation Menu"),  // Nome amigável
		FKeyDetails::GamepadKey                  // Tipo da Key (Gamepad, no caso)
	));

	EKeys::AddKey(FKeyDetails(
		PlayStationButton,
		FText::FromString("PlayStation Button"),  // Nome amigável
		FKeyDetails::GamepadKey                  // Tipo da Key (Gamepad, no caso)
	));

	EKeys::AddKey(FKeyDetails(
		Mic,
		FText::FromString("PlayStation Mic"),  // Nome amigável
		FKeyDetails::GamepadKey                  // Tipo da Key (Gamepad, no caso)
	));

	EKeys::AddKey(FKeyDetails(
		TouchButtom,
		FText::FromString("PlayStation Touch Buttom"),  // Nome amigável
		FKeyDetails::GamepadKey                  // Tipo da Key (Gamepad, no caso)
	));
}

void FWindowsDualsense_ds5wModule::StartupModule()
{
	IModularFeatures::Get().RegisterModularFeature(IInputDeviceModule::GetModularFeatureName(), this);
	RegisterCustomKeys();
	
	FString EnginePluginPath = FPaths::Combine(FPaths::EnginePluginsDir(), TEXT("WindowsDualsense_ds5w/Binaries/Win64/ds5w_x64.dll"));
	if (EnginePluginPath.IsEmpty())
	{
		FString LocalPluginPath = FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("WindowsDualsense_ds5w/Binaries/Win64/ds5w_x64.dll"));
		DS5WdllHandle = FPlatformProcess::GetDllHandle(*EnginePluginPath);
	}
	else
	{
		DS5WdllHandle = FPlatformProcess::GetDllHandle(*EnginePluginPath);
	}

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

IMPLEMENT_MODULE(FWindowsDualsense_ds5wModule, WindowsDualsense_ds5w)
