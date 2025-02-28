// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IHapticDevice.h"
#include "Haptics/HapticFeedbackEffect_Base.h"
#include "Haptics/HapticFeedbackEffect_Buffer.h"
#include "Runtime/InputDevice/Public/IInputDevice.h"

/**
 * 
 */
class WINDOWSDUALSENSE_DS5W_API FDualSenseInputDevice : public IHapticDevice, public IInputDevice,  public FGenericPlatformInputDeviceMapper
{
public:
	FDualSenseInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler);
	~FDualSenseInputDevice() = default;


	void PlayHapticFeedbackEffect(int32 ControllerId, const UHapticFeedbackEffect_Base* HapticEffect)
	{
		if (const UHapticFeedbackEffect_Buffer* BufferEffect = Cast<UHapticFeedbackEffect_Buffer>(HapticEffect))
		{
			UE_LOG(LogTemp, Error, TEXT("BufferEffect: %s"), *BufferEffect->GetName())
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("HapticEffect não é do tipo Buffer"));
		}
	}


	virtual void Tick(float DeltaTime) override;
	virtual void SendControllerEvents() override {};
	virtual void SetMessageHandler(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler) override {};

	virtual bool SupportsForceFeedback(int32 ControllerId) override;
	virtual void SetChannelValues(int32 ControllerId, const FForceFeedbackValues& Values) override;
	virtual void SetDeviceProperty(int32 ControllerId, const FInputDeviceProperty* Property) override;

	virtual void SetLightColor(int32 ControllerId, FColor Color) override;
	virtual void ResetLightColor(int32 ControllerId) override;
	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override { return true; }

	void SetController(const FInputDeviceId& Device, const FPlatformInputDeviceState& State)
	{
		LastInputDeviceId = Device;
		LastPlatformUserId = GetPlatformUserForUserIndex(GetUserForInputDevice(Device));
		MappedInputDevices.Add(Device, State);
	}

	virtual bool ShouldCreateUniqueUserForEachDevice() const override
	{
		return false;
	}

	virtual bool IsUsingControllerIdAsUserId() const override
	{
		return true;
	}

	virtual IHapticDevice* GetHapticDevice() override
	{
		return this;
	}

	virtual bool IsGamepadAttached() const override
	{
		return true;
	}

	virtual void SetHapticFeedbackValues(int32 ControllerId, int32 Hand, const FHapticFeedbackValues& Values) override;
	virtual void GetHapticFrequencyRange(float& MinFrequency, float& MaxFrequency) const override;
	virtual float GetHapticAmplitudeScale() const override { return 1.0f; }

	virtual void SetChannelValue (int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value) override;
	const TSharedRef<FGenericApplicationMessageHandler>& GetMessageHandler();
	
private:
	const TSharedRef<FGenericApplicationMessageHandler>& MessageHandler;
};
