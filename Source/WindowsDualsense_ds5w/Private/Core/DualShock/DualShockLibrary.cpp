// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/DualShock/DualShockLibrary.h"
#include <Windows.h>
#include "Core/DeviceHIDManager.h"
#include "InputCoreTypes.h"
#include "VREditorMode.h"
#include "Core/Structs/FOutputContext.h"
#include "Helpers/ValidateHelpers.h"

void UDualShockLibrary::Settings(const FSettings<FFeatureReport>& Settings)
{
}

void UDualShockLibrary::Settings(const FDualShockFeatureReport& Settings)
{
}

bool UDualShockLibrary::InitializeLibrary(const FDeviceContext& Context)
{
	HIDDeviceContexts = Context;

	UE_LOG(LogTemp, Log, TEXT("Initializing device model (DualShock 4)"));
	return true;
}

void UDualShockLibrary::ShutdownLibrary()
{
	ButtonStates.Reset();

	FOutputContext* HidOutput = &HIDDeviceContexts.Output;
	memset(&HidOutput, 0, sizeof(HidOutput));

	CloseHandle(HIDDeviceContexts.Handle);
	UDeviceHIDManager::FreeContext(&HIDDeviceContexts);
}

void UDualShockLibrary::Reconnect()
{
	PlatformInputDeviceMapper.Get().GetOnInputDeviceConnectionChange().Broadcast(
		EInputDeviceConnectionState::Connected, FPlatformUserId::CreateFromInternalId(ControllerID),
		FInputDeviceId::CreateFromInternalId(ControllerID));
}

bool UDualShockLibrary::IsConnected()
{
	return HIDDeviceContexts.IsConnected;
}

void UDualShockLibrary::SendOut()
{
	if (!HIDDeviceContexts.IsConnected)
	{
		return;
	}

	UDeviceHIDManager::OutputBuffering(&HIDDeviceContexts);
}

void UDualShockLibrary::CheckButtonInput(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
	const FPlatformUserId UserId, const FInputDeviceId InputDeviceId, const FName ButtonName,
	const bool IsButtonPressed)
{
	const bool PreviousState = ButtonStates.Contains(ButtonName) ? ButtonStates[ButtonName] : false;
	if (IsButtonPressed && !PreviousState)
	{
		InMessageHandler.Get().OnControllerButtonPressed(ButtonName, UserId, InputDeviceId, false);
	}

	if (!IsButtonPressed && PreviousState)
	{
		InMessageHandler.Get().OnControllerButtonReleased(ButtonName, UserId, InputDeviceId, false);
	}

	ButtonStates.Add(ButtonName, IsButtonPressed);
}

bool UDualShockLibrary::UpdateInput(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
	const FPlatformUserId UserId, const FInputDeviceId InputDeviceId)
{
	// TODO: Implement the logic for reading the DualShock buffer
	return false;
}

void UDualShockLibrary::SetLightbar(FColor Color)
{
	// TODO: Implement the logic for the DualShock Lightbar buffer
	SendOut();
}

void UDualShockLibrary::SetPlayerLed(ELedPlayerEnum Led, ELedBrightnessEnum Brightness)
{
	// TODO: Implement the logic for the DualShock PlayerLed buffer
	SendOut();
}

void UDualShockLibrary::SetMicrophoneLed(ELedMicEnum Led)
{
	// TODO: Implement the logic for the DualShock MicrophoneLed buffer
	SendOut();
}

void UDualShockLibrary::SetTouch(const bool bIsTouch)
{
	EnableTouch = bIsTouch;
}

void UDualShockLibrary::SetAcceleration(bool bIsAccelerometer)
{
	EnableAccelerometerAndGyroscope = bIsAccelerometer;
}

void UDualShockLibrary::SetGyroscope(bool bIsGyroscope)
{
	EnableAccelerometerAndGyroscope = bIsGyroscope;
}

void UDualShockLibrary::StopAll()
{
	// TODO: Implement the logic for the DualShock Reset buffer
	SendOut();
}

void UDualShockLibrary::SetVibration(const FForceFeedbackValues& Values)
{
	// TODO: Implement the logic for output the DualShock Vibrations
	SendOut();
}
