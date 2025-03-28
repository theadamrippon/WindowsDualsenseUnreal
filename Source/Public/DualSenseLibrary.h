// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "ds5w.h"
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InputCoreTypes.h"
#include "Runtime/ApplicationCore/Public/GenericPlatform/IInputInterface.h"
#include "Runtime/ApplicationCore/Public/GenericPlatform/GenericApplicationMessageHandler.h"
#include "DualSenseLibrary.generated.h"

/**
 * 
 */
UCLASS()
class WINDOWSDUALSENSE_DS5W_API UDualSenseLibrary : public UObject
{
	GENERATED_BODY()
public:
	virtual ~UDualSenseLibrary() override
	{
		UE_LOG(LogTemp, Log, TEXT("Dualsense UDualSenseLibrary Destruct"));
	}

	bool InitializeLibrary(DS5W::DeviceContext& Context);
	void ShutdownLibrary();

	bool Reconnect();
	bool IsConnected();

	// Input
	virtual bool UpdateInput(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler, const FPlatformUserId UserId, const FInputDeviceId InputDeviceId);
	virtual void CheckButtonInput(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler, const FPlatformUserId UserId, const FInputDeviceId InputDeviceId, const FName ButtonName, const bool IsButtonPressed);
	
	// Haptic Feedback and Effects
	virtual void SetHapticFeedbackValues(int32 Hand, const FHapticFeedbackValues* Values);
	virtual int32 GetTrirggersFeedback(const EControllerHand& HandTrigger);
	virtual void ConfigTriggerHapticFeedbackEffect(int32 StartPosition, int32 BeginForce, int32 MiddleForce, int32 EndForce, const EControllerHand& Hand, bool KeepEffect);

	// Effects
	void NoResitance(const EControllerHand& Hand);
	void ContinuousResitance(int32 StartPosition, int32 Force, const EControllerHand& Hand);
	void SectionResitance(int32 StartPosition, int32 EndPosition, const EControllerHand& Hand);
	void StopEffect(const EControllerHand& Hand);
	void StopAllEffects();
	void StopAll();
	void SetLedPlayerEffects(int32 NumberLeds, int32 BrightnessValue);
	void SetLedMicEffects(int32 LedMic);

	// Status
	float GetLevelBattery();

	// Colors, vibration and triggers config
	void UpdateColorOutput(FColor Color);
	void SetVibration(const FForceFeedbackValues& Vibration);
	void SetTriggers(const FInputDeviceProperty* Property);

	void SetAcceleration(bool bIsAccelerometer);
	void SetGyroscope(bool bIsGyroscope);
	void SetTouch1(bool bIsGyroscope);
	void SetTouch2(bool bIsGyroscope);

	TMap<const FName, bool> ButtonStates;
private:
	DS5W::DS5InputState InputState;
	DS5W::DS5OutputState OutputState;
	DS5W::DeviceContext DeviceContexts;
	
	bool EnableAccelerometer;
	bool EnableGyroscope;
	bool EnableTouch1;
	bool EnableTouch2;

	bool Connection();
	void SendOut();
	
	unsigned char CalculateLeftRumble(const FForceFeedbackValues& Values);
	unsigned char CalculateRightRumble(const FForceFeedbackValues& Values);
	
	int ConvertTo255(float Value);
	unsigned char ConvertTo255(unsigned char value, unsigned char maxInput);
};
