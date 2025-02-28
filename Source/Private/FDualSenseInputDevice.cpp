// Fill out your copyright notice in the Description page of Project Settings.


#include "FDualSenseInputDevice.h"
#include "DualSenseLibrary.h"
#include "Windows/WindowsApplication.h"


FDualSenseInputDevice::FDualSenseInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler):
	FGenericPlatformInputDeviceMapper(true, false),
	MessageHandler(InMessageHandler)
{
}

void FDualSenseInputDevice::Tick(float DeltaTime)
{
	TArray<FInputDeviceId> DeviceIds;
	Get().GetAllInputDevices(DeviceIds);
	
	for (FInputDeviceId& Device : DeviceIds)
	{
		if (MappedInputDevices.Contains(Device))
		{
			if (MappedInputDevices[Device].ConnectionState == EInputDeviceConnectionState::Connected)
			{
				const FPlatformUserId UserId = GetUserForInputDevice(Device);
				UDualSenseLibrary::UpdateInput(MessageHandler, UserId, Device);
			}
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
		UDualSenseLibrary::SetTriggers(ControllerId, Property);
	}
	
	UDualSenseLibrary::SendOut(ControllerId);
}

void FDualSenseInputDevice::SetLightColor(int32 ControllerId, FColor Color)
{
	UDualSenseLibrary::UpdateColorOutput(ControllerId, Color);
}

void FDualSenseInputDevice::ResetLightColor(int32 ControllerId)
{
}

void FDualSenseInputDevice::SetHapticFeedbackValues(int32 ControllerId, int32 Hand, const FHapticFeedbackValues& Values)
{
	if (Hand == static_cast<int32>(EControllerHand::Left))
	{
		const FHapticFeedbackValues Left = Values;
		UDualSenseLibrary::SetHapticFeedbackValues(ControllerId, Hand, &Left);
	}

	// Configurar pulsos no gatilho direito (R2)
	else if (Hand == static_cast<int32>(EControllerHand::Right))
	{
		const FHapticFeedbackValues Right = Values;
		UDualSenseLibrary::SetHapticFeedbackValues(ControllerId, Hand, &Right);
	}
}

void FDualSenseInputDevice::GetHapticFrequencyRange(float& MinFrequency, float& MaxFrequency) const
{
	MinFrequency = 0.0f; // Frequência mínima suportada
	MaxFrequency = 1.0f; // Frequência máxima suportada
}

void FDualSenseInputDevice::SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value)
{
	UE_LOG(LogTemp, Error, TEXT("FForceFeedbackChannelType"));
}

const TSharedRef<FGenericApplicationMessageHandler>& FDualSenseInputDevice::GetMessageHandler()
{
	return MessageHandler;
}

bool FDualSenseInputDevice::SupportsForceFeedback(int32 ControllerId)
{
	return true;
}


void FDualSenseInputDevice::SetChannelValues(int32 ControllerId, const FForceFeedbackValues& Values)
{
	UDualSenseLibrary::SetVibration(ControllerId, Values);
	UDualSenseLibrary::SendOut(ControllerId);
}
