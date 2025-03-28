// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025


#include "FDualSenseInputDevice.h"
#include "DualSenseLibrary.h"
#include "FDualSenseLibraryManager.h"
#include "Windows/WindowsApplication.h"
#include "Windows/WindowsPlatformApplicationMisc.h"

FDualSenseInputDevice::FDualSenseInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler): MessageHandler(InMessageHandler)
{
	DeviceMapper = FWindowsPlatformApplicationMisc::CreatePlatformInputDeviceManager();
}

FDualSenseInputDevice::~FDualSenseInputDevice()
{
	UE_LOG(LogTemp, Log, TEXT("Destroying DualSense Input Device"));
}

void FDualSenseInputDevice::Tick(float DeltaTime)
{
	TArray<FInputDeviceId> OutInputDevices;
	DeviceMapper->Get().GetAllConnectedInputDevices(OutInputDevices);
	for (const FInputDeviceId& DeviceId : OutInputDevices)
	{
		if (!PlatformUserIdToDeviceContainer.Contains(DeviceId.GetId()))
		{
			if (UDualSenseLibrary* DsLibrary = UFDualSenseLibraryManager::Get()->GetLibraryInstance(DeviceId.GetId()); IsValid(DsLibrary))
			{
				PlatformUserIdToDeviceContainer.Add(DeviceId.GetId(), DsLibrary);
			}
		}
	}

	SendControllerEvents();
}

void FDualSenseInputDevice::SendControllerEvents()
{
	for (const auto& Pair : PlatformUserIdToDeviceContainer)
	{
		UDualSenseLibrary* DsLibrary = Pair.Value;
		if (!DsLibrary)
		{
			continue;
		}
		
		const FInputDeviceId& Device = FInputDeviceId::CreateFromInternalId(Pair.Key);
		const FPlatformUserId& UserId = IPlatformInputDeviceMapper::Get().GetUserForInputDevice(Device);

		if (const int32 ControllerId = FPlatformMisc::GetUserIndexForPlatformUser(UserId); ControllerId == -1)
		{
			continue;
		}

		if (DeviceMapper->Get().GetInputDeviceConnectionState(Device) == EInputDeviceConnectionState::Connected)
		{
			DsLibrary->UpdateInput(MessageHandler, UserId, Device);
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

void FDualSenseInputDevice::Reconnect(FInputDeviceId& Device)
{
}

void FDualSenseInputDevice::Disconnect(FInputDeviceId& Device)
{
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
