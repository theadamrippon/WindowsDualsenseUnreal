// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"
#include "DualSenseLibrary.h"
#include "IHapticDevice.h"
#include "IInputDevice.h"

/**
 * 
 */
class WINDOWSDUALSENSE_DS5W_API FDualSenseInputDevice final : public IInputDevice, public IHapticDevice
{

public:
	explicit FDualSenseInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler);
	virtual ~FDualSenseInputDevice() override;
	
	virtual void Tick(float DeltaTime) override;
	virtual void SendControllerEvents() override {};
	virtual void SetMessageHandler(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler) override {}
	virtual bool SupportsForceFeedback(int32 ControllerId) override;
	virtual void SetChannelValues(int32 ControllerId, const FForceFeedbackValues &values) override;
	virtual void SetDeviceProperty(int32 ControllerId, const FInputDeviceProperty* Property) override;
	virtual void SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value) override {};
	virtual void SetLightColor(int32 ControllerId, FColor Color) override;
	virtual void ResetLightColor(int32 ControllerId) override;
	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override { return true; }
	virtual void SetHapticFeedbackValues(int32 ControllerId, int32 Hand, const FHapticFeedbackValues& Values) override;
	virtual void GetHapticFrequencyRange(float& MinFrequency, float& MaxFrequency) const override;
	virtual float GetHapticAmplitudeScale() const override { return 1.0f; }

	virtual IHapticDevice* GetHapticDevice() override
	{
		return this;
	}

	virtual bool IsGamepadAttached() const override
	{
		return true;
	}
	
	void SetController(const FInputDeviceId Device) const
	{
		if (Device.GetId() == 0)
		{
			return;
		}

		const FPlatformUserId& User = FPlatformMisc::GetPlatformUserForUserIndex(Device.GetId());
		DeviceMapper->Get().Internal_MapInputDeviceToUser(Device, User, EInputDeviceConnectionState::Connected);
	}

	static void OnUserLoginChangedEvent(bool bLoggedIn, const int32 UserId, int32 UserIndex)
	{
		UE_LOG(LogTemp, Log, TEXT("DualSense: OnUserLoginChangedEvent: %d"), UserId);
	}
	
protected:
	void Reconnect(FInputDeviceId& Device);
	void Disconnect(FInputDeviceId& Device);
	
private:
	IPlatformInputDeviceMapper* DeviceMapper;
	const TSharedRef<FGenericApplicationMessageHandler>& MessageHandler;
	
	TMap<int32, UDualSenseLibrary*> PlatformUserIdToDeviceContainer;
	TMap<FPlatformUserId, TArray<FInputDeviceId>> PlatformUserIdToDevices;
};
