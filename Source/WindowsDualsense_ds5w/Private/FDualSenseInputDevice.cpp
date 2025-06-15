// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025


#include "FDualSenseInputDevice.h"
#include "DualSenseLibrary.h"
#include "FDualSenseLibraryManager.h"
#include "Windows/WindowsApplication.h"
#include "Windows/WindowsPlatformApplicationMisc.h"
#include "Misc/CoreDelegates.h"

FDualSenseInputDevice::FDualSenseInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler): MessageHandler(InMessageHandler)
{
	DeviceMapper = FWindowsPlatformApplicationMisc::CreatePlatformInputDeviceManager();
	DeviceMapper->Get().GetOnInputDeviceConnectionChange().AddRaw(this, &FDualSenseInputDevice::OnConnectionChange);
	FCoreDelegates::OnUserLoginChangedEvent.AddRaw(this, &FDualSenseInputDevice::OnUserLoginChangedEvent);
}

FDualSenseInputDevice::~FDualSenseInputDevice()
{
	FCoreDelegates::OnUserLoginChangedEvent.RemoveAll(this);
	DeviceMapper->Get().GetOnInputDeviceConnectionChange().RemoveAll(this);
	UE_LOG(LogTemp, Log, TEXT("Destroying DualSense Input Device"));
}

void FDualSenseInputDevice::Tick(float DeltaTime)
{
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
		
		if (UDualSenseLibrary* DsLibrary = UFDualSenseLibraryManager::Get()->GetLibraryInstance(DeviceId.GetId()); IsValid(DsLibrary) && DsLibrary != nullptr)
		{
			if (!DsLibrary->UpdateInput(MessageHandler, UserId, Device))
			{
				UE_LOG(LogTemp, Log, TEXT("DualSense: Disconnecting DeviceId=%d"), DeviceId.GetId());
				Disconnect(DeviceId);
			}
		}
		else
		{
			Disconnect(DeviceId);
		}
	}
}

void FDualSenseInputDevice::SetDeviceProperty(int32 ControllerId, const FInputDeviceProperty* Property)
{
	if (!Property)
	{
		return;
	}

	if (Property->Name == "InputDeviceLightColor")
	{
		const FInputDeviceLightColorProperty* ColorProperty = static_cast<const FInputDeviceLightColorProperty*>(Property);
		SetLightColor(ControllerId, ColorProperty->Color);
	}
	
	if (Property->Name != "InputDeviceLightColor")
	{
		UDualSenseLibrary* DsLibrary = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
		if (!DsLibrary)
		{
			return;
		}
		
		DsLibrary->SetTriggers(Property);
	}
}

void FDualSenseInputDevice::SetLightColor(const int32 ControllerId, const FColor Color)
{
	UDualSenseLibrary* DsLibrary = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DsLibrary)
	{
		return;
	}
	DsLibrary->UpdateColorOutput(Color);
}

void FDualSenseInputDevice::ResetLightColor(const int32 ControllerId)
{
	UDualSenseLibrary* DsLibrary = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DsLibrary)
	{
		return;
	}
	DsLibrary->UpdateColorOutput(FColor::Blue);
}

void FDualSenseInputDevice::OnUserLoginChangedEvent(bool bLoggedIn, int32 UserId, int32 UserIndex)
{
	if (!bLoggedIn)
	{
		Disconnect(FInputDeviceId::CreateFromInternalId(UserId));
		return;
	}
	
	const FInputDeviceId& Device = FInputDeviceId::CreateFromInternalId(UserId);
	const FPlatformUserId& User = FPlatformMisc::GetPlatformUserForUserIndex(UserIndex);
	if (const FPlatformUserId& UserPair = DeviceMapper->Get().GetUserForInputDevice(Device); UserPair != User)
	{
		if (DeviceMapper->Get().GetInputDeviceConnectionState(Device) == EInputDeviceConnectionState::Connected)
		{
			return;
		}
		
		UE_LOG(LogTemp, Log, TEXT("DualSense: IsLoggin=%d, UserId=%d, UserIndex=%d"), bLoggedIn, UserId, UserIndex);
		DeviceMapper->Get().Internal_MapInputDeviceToUser(Device, User, EInputDeviceConnectionState::Connected);
	}
}


void FDualSenseInputDevice::OnConnectionChange(bool Connected, FPlatformUserId PlatformUserId, int32 InputDeviceId)
{
	if (!IsConnectionChange.Contains(InputDeviceId))
	{
		IsConnectionChange.Add(InputDeviceId, false);
		return;
	}
	
	if (IsConnectionChange[InputDeviceId] == Connected)
	{
		return;
	}

	IsConnectionChange[InputDeviceId] = Connected;
	if (!Connected)
	{
		DeviceMapper->Get().Internal_MapInputDeviceToUser(FInputDeviceId::CreateFromInternalId(InputDeviceId), PlatformUserId, EInputDeviceConnectionState::Disconnected);
		return;
	}
	
	DeviceMapper->Get().Internal_MapInputDeviceToUser(FInputDeviceId::CreateFromInternalId(InputDeviceId), PlatformUserId, EInputDeviceConnectionState::Connected);
}

void FDualSenseInputDevice::OnConnectionChange(EInputDeviceConnectionState Connected, FPlatformUserId PlatformUserId, FInputDeviceId InputDeviceId)
{
	bool bIsConnected = (Connected == EInputDeviceConnectionState::Connected);
	int32 Device = InputDeviceId.GetId();
	if (!IsConnectionChange.Contains(Device))
	{
		IsConnectionChange.Add(Device, false);
	}
	
	if (IsConnectionChange[Device] == bIsConnected)
	{
		return;
	}

	IsConnectionChange[Device] = bIsConnected;
	if (!bIsConnected)
	{
		DeviceMapper->Get().Internal_MapInputDeviceToUser(InputDeviceId, PlatformUserId, EInputDeviceConnectionState::Disconnected);
		return;
	}

	DeviceMapper->Get().Internal_MapInputDeviceToUser(InputDeviceId, PlatformUserId, EInputDeviceConnectionState::Connected);
}

void FDualSenseInputDevice::Reconnect(const FInputDeviceId& Device) const
{
	if (DeviceMapper->Get().GetInputDeviceConnectionState(Device) != EInputDeviceConnectionState::Connected)
	{
		DeviceMapper->Get().Internal_SetInputDeviceConnectionState(Device, EInputDeviceConnectionState::Connected);
	}
}

void FDualSenseInputDevice::Disconnect(const FInputDeviceId& Device) const
{
	if (DeviceMapper->Get().GetInputDeviceConnectionState(Device) != EInputDeviceConnectionState::Disconnected)
	{
		DeviceMapper->Get().Internal_SetInputDeviceConnectionState(Device, EInputDeviceConnectionState::Disconnected);	
	}
}

void FDualSenseInputDevice::SetHapticFeedbackValues(const int32 ControllerId, const int32 Hand, const FHapticFeedbackValues& Values)
{
	UDualSenseLibrary* DsLibrary = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DsLibrary)
	{
		return;
	}
	DsLibrary->SetHapticFeedbackValues(Hand, &Values);
}

void FDualSenseInputDevice::GetHapticFrequencyRange(float& MinFrequency, float& MaxFrequency) const
{
	MinFrequency = 0.0f;
	MaxFrequency = 1.0f;
}

bool FDualSenseInputDevice::SupportsForceFeedback(int32 ControllerId)
{
	return true;
}

void FDualSenseInputDevice::SetChannelValues(int32 ControllerId, const FForceFeedbackValues& values)
{
	UDualSenseLibrary* DsLibrary = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DsLibrary)
	{
		return;
	}
	DsLibrary->SetVibration(values);
}
