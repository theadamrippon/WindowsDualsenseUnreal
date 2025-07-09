// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025


#include "DualSenseInputDevice.h"
#include "DualSenseLibrary.h"
#include "DualSenseLibraryManager.h"
#include "Windows/WindowsApplication.h"
#include "Windows/WindowsPlatformApplicationMisc.h"
#include "Misc/CoreDelegates.h"


DualSenseInputDevice::DualSenseInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
                                           bool Lazily): MessageHandler(InMessageHandler)
{
	LazyLoading = Lazily;
	DeviceMapper = FWindowsPlatformApplicationMisc::CreatePlatformInputDeviceManager();
	DeviceMapper->Get().GetOnInputDeviceConnectionChange().AddRaw(this, &DualSenseInputDevice::OnConnectionChange);
	FCoreDelegates::OnUserLoginChangedEvent.AddRaw(this, &DualSenseInputDevice::OnUserLoginChangedEvent);
}

DualSenseInputDevice::~DualSenseInputDevice()
{
	FCoreDelegates::OnUserLoginChangedEvent.RemoveAll(this);
	DeviceMapper->Get().GetOnInputDeviceConnectionChange().RemoveAll(this);
}

void DualSenseInputDevice::Tick(float DeltaTime)
{
	if (LazyLoading)
	{
		return;
	}

	TArray<FInputDeviceId> OutInputDevices;
	OutInputDevices.Reset();

	DeviceMapper->Get().GetAllConnectedInputDevices(OutInputDevices);
	for (const FInputDeviceId& DeviceId : OutInputDevices)
	{
		const FInputDeviceId& Device = FInputDeviceId::CreateFromInternalId(DeviceId.GetId());
		const FPlatformUserId& UserId = IPlatformInputDeviceMapper::Get().GetUserForInputDevice(Device);

		if (const int32 ControllerId = FPlatformMisc::GetUserIndexForPlatformUser(UserId); ControllerId == -1)
		{
			continue;
		}

		FInputDeviceScope InputScope(this, TEXT("DualSenseInputDevice"), Device.GetId(),TEXT("DualSense"));
		if (UDualSenseLibrary* DsLibrary = UDualSenseLibraryManager::Get()->GetLibraryInstance(DeviceId.GetId());
			IsValid(DsLibrary))
		{
			if (!DsLibrary->UpdateInput(MessageHandler, UserId, Device))
			{
				Disconnect(DeviceId);
				UDualSenseLibraryManager::Get()->RemoveLibraryInstance(DeviceId.GetId());
				UE_LOG(LogTemp, Warning, TEXT("DualSense: Disconnecting DeviceId=%d"), DeviceId.GetId());
			}
		}
		else
		{
			Disconnect(DeviceId);
		}
	}
}

void DualSenseInputDevice::SetDeviceProperty(int32 ControllerId, const FInputDeviceProperty* Property)
{
	if (LazyLoading)
	{
		return;
	}

	if (!Property)
	{
		return;
	}

	if (Property->Name == "InputDeviceLightColor")
	{
		const FInputDeviceLightColorProperty* ColorProperty = static_cast<const FInputDeviceLightColorProperty*>(
			Property);
		SetLightColor(ControllerId, ColorProperty->Color);
	}

	if (Property->Name == FName("InputDeviceTriggerResistance"))
	{
		UDualSenseLibrary* DsLibrary = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
		if (!DsLibrary)
		{
			return;
		}

		DsLibrary->SetTriggers(Property);
	}
}


void DualSenseInputDevice::SetLightColor(const int32 ControllerId, const FColor Color)
{
	if (LazyLoading)
	{
		return;
	}

	UDualSenseLibrary* DsLibrary = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DsLibrary)
	{
		return;
	}

	DsLibrary->UpdateColorOutput(Color);
}

void DualSenseInputDevice::ResetLightColor(const int32 ControllerId)
{
	if (LazyLoading)
	{
		return;
	}

	UDualSenseLibrary* DsLibrary = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DsLibrary)
	{
		return;
	}
	DsLibrary->UpdateColorOutput(FColor::Blue);
}

void DualSenseInputDevice::SetLazyLoading(const bool IsLazy)
{
	LazyLoading = IsLazy;
}

void DualSenseInputDevice::OnUserLoginChangedEvent(bool bLoggedIn, int32 UserId, int32 UserIndex) const
{
	if (!bLoggedIn)
	{
		return;
	}

	const FInputDeviceId& Device = FInputDeviceId::CreateFromInternalId(UserId);
	const FPlatformUserId& User = FPlatformMisc::GetPlatformUserForUserIndex(UserIndex);
	if (const FPlatformUserId& UserPair = DeviceMapper->Get().GetUserForInputDevice(Device); UserPair != User)
	{
		if (DeviceMapper->Get().GetInputDeviceConnectionState(Device) != EInputDeviceConnectionState::Connected)
		{
			DeviceMapper->Get().Internal_MapInputDeviceToUser(Device, User, EInputDeviceConnectionState::Connected);
		}
	}
}

void DualSenseInputDevice::OnConnectionChange(EInputDeviceConnectionState Connected, FPlatformUserId PlatformUserId,
                                              FInputDeviceId InputDeviceId) const
{
	const bool bIsConnected = (Connected == EInputDeviceConnectionState::Connected);

	if (DeviceMapper->Get().GetInputDeviceConnectionState(InputDeviceId) != EInputDeviceConnectionState::Connected &&
		bIsConnected)
	{
		DeviceMapper->Get().Internal_MapInputDeviceToUser(InputDeviceId, PlatformUserId,
		                                                  EInputDeviceConnectionState::Connected);
	}

	if (DeviceMapper->Get().GetInputDeviceConnectionState(InputDeviceId) != EInputDeviceConnectionState::Disconnected &&
		!bIsConnected)
	{
		DeviceMapper->Get().Internal_MapInputDeviceToUser(InputDeviceId, PlatformUserId,
		                                                  EInputDeviceConnectionState::Disconnected);
	}
}

void DualSenseInputDevice::Reconnect(const FInputDeviceId& Device) const
{
	if (LazyLoading)
	{
		return;
	}

	if (DeviceMapper->Get().GetInputDeviceConnectionState(Device) == EInputDeviceConnectionState::Disconnected)
	{
		DeviceMapper->Get().Internal_SetInputDeviceConnectionState(Device, EInputDeviceConnectionState::Connected);
	}
}

void DualSenseInputDevice::Disconnect(const FInputDeviceId& Device) const
{
	if (LazyLoading)
	{
		return;
	}

	if (DeviceMapper->Get().GetInputDeviceConnectionState(Device) == EInputDeviceConnectionState::Connected)
	{
		DeviceMapper->Get().Internal_SetInputDeviceConnectionState(Device, EInputDeviceConnectionState::Disconnected);
	}
}

void DualSenseInputDevice::SetHapticFeedbackValues(const int32 ControllerId, const int32 Hand,
                                                   const FHapticFeedbackValues& Values)
{
	if (LazyLoading)
	{
		return;
	}

	UDualSenseLibrary* DsLibrary = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DsLibrary)
	{
		return;
	}
	DsLibrary->SetHapticFeedbackValues(Hand, &Values);
}

void DualSenseInputDevice::GetHapticFrequencyRange(float& MinFrequency, float& MaxFrequency) const
{
	MinFrequency = 0.0f;
	MaxFrequency = 1.0f;
}

bool DualSenseInputDevice::SupportsForceFeedback(int32 ControllerId)
{
	return true;
}

void DualSenseInputDevice::SetChannelValues(int32 ControllerId, const FForceFeedbackValues& Values)
{
	// UE_LOG(LogTemp, Log, TEXT("DualSense: %f, %f"), values.RightLarge, values.LeftLarge);
	if (LazyLoading)
	{
		return;
	}

	UDualSenseLibrary* DsLibrary = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DsLibrary)
	{
		return;
	}

	DsLibrary->SetVibration(Values);
}
