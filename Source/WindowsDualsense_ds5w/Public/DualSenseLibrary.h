// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include <algorithm>

#include "CoreMinimal.h"
#include "DualSenseHIDManager.h"
#include "UObject/Object.h"
#include "InputCoreTypes.h"
#include "Misc/CoreDelegates.h"
#include "Runtime/ApplicationCore/Public/GenericPlatform/IInputInterface.h"
#include "Runtime/ApplicationCore/Public/GenericPlatform/GenericApplicationMessageHandler.h"
#include "DualSenseLibrary.generated.h"

enum class EDualSenseAudioFeatureReport : uint8;
enum class EDualSenseDeviceFeatureReport : uint8;

struct FTouchPoint1
{
	unsigned char X;
	unsigned char Y;
	unsigned char Down;
	unsigned char Id;
};

struct FTouchPoint2
{
	unsigned char X;
	unsigned char Y;
	unsigned char Down;
	unsigned char Id;
};

struct FAccelerometer
{
	unsigned char X;
	unsigned char Y;
	unsigned char Z;
};

struct FGyro
{
	unsigned char X;
	unsigned char Y;
	unsigned char Z;
};


struct FDualSenseLibrarySettings
{
};

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

	void ShutdownLibrary();
	bool InitializeLibrary(const FHIDDeviceContext& Context);

	bool Reconnect() const;
	bool IsConnected() const;

	// Input
	virtual bool UpdateInput(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler, const FPlatformUserId UserId, const FInputDeviceId InputDeviceId);
	virtual void CheckButtonInput(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler, const FPlatformUserId UserId, const FInputDeviceId InputDeviceId, const FName ButtonName, const bool IsButtonPressed);

	virtual void SetHapticFeedbackValues(int32 Hand, const FHapticFeedbackValues* Values);
	virtual void SetVibrationTrigger(int32 BeginStrength, int32 MiddleStrength, int32 EndStrength, const EControllerHand& Hand, bool KeepEffect);

	void ContinuousResistance(int32 StartPosition, int32 Strength, const EControllerHand& Hand);
	void SectionResistance(int32 StartPosition, int32 EndPosition, int32 Strength, const EControllerHand& Hand);
	void Feedback(int32 BeginStrength, int32 MiddleStrength, int32 EndStrength, const EControllerHand& Hand);
	void SetWeapon(int32 StartPosition, int32 EndPosition, int32 Strength, const EControllerHand& Hand);
	void SetBowEffects(int32 StartPosition, int32 EndPosition, int32 BegingStrength, int32 EndStrength, const EControllerHand& Hand);
	void SetMachineEffects(int32 StartPosition, int32 EndPosition, int32 AmplitudeBegin, int32 AmplitudeEnd, float Frequency, float Period, const EControllerHand& Hand);
	void SetGallopingEffects(int32 StartPosition, int32 EndPosition, int32 FirstFoot, int32 SecondFoot, float Frequency, const EControllerHand& Hand);
	void SetLedPlayerEffects(int32 NumberLeds, int32 BrightnessValue);
	void SetLedMicEffects(int32 LedMic);

	
	void NoResistance(const EControllerHand& Hand);
	void StopEffect(const EControllerHand& Hand);
	void StopAllEffects();
	void StopAll();

	// Status
	float GetLevelBattery() const;
	virtual int32 GetTrirggersFeedback(const EControllerHand& HandTrigger);

	// Colors, vibration and triggers config
	void UpdateColorOutput(FColor Color);
	void SetTriggers(const FInputDeviceProperty* Property);
	void SetVibration(const FForceFeedbackValues& Vibration);
	void SetVibrationAudioBased(const FForceFeedbackValues& Vibration, float Threshold, float ExponentCurve, float BaseMultiplier);
	
	void SetHasPhoneConnected(bool bHasConnected);
	void SetLevelBattery(float Level, bool FullyCharged, bool Charging);
	void SetLeftTriggerFeedback(float L2Feedback);
	void SetRightTriggerFeedback(float R2Feedback);

	void SetAcceleration(bool bIsAccelerometer);
	void SetGyroscope(bool bIsGyroscope);
	void SetTouch(bool bIsTouch);
	void RegisterSettings(
		EDualSenseAudioFeatureReport MicStatus,
		EDualSenseAudioFeatureReport AudioHeadset,
		EDualSenseAudioFeatureReport AudioSpeaker,
		EDualSenseDeviceFeatureReport VibrationMode,
		int32 MicVolume,
		int32 AudioVolume,
		int32 SoftRumbleReduce,
		int32 TriggerSoftnessLevel,
		bool SoftRumble
	);

	int32 ControllerID;
	TMap<const FName, bool> ButtonStates;
protected:
	void SendOut();
	static FGenericPlatformInputDeviceMapper PlatformInputDeviceMapper;

private:
	bool EnableTouch;
	bool HasPhoneConnected;
	bool EnableAccelerometerAndGyroscope;
	float LevelBattery;
	float LeftTriggerFeedback;
	float RightTriggerFeedback;

	FHIDDeviceContext HIDDeviceContexts;

	// Helpers
	static int To255(const float Value)
	{
		if (Value <= 0) return 0;
		if (Value >= 1.0f) return 255;
		
		constexpr float Min = 0;
		constexpr float Max = 1.0;
		const float NormalizedPosition = (Value - Min) / (Max - Min);
		return static_cast<int>(NormalizedPosition * 255);
	}

	static unsigned char To255(const unsigned char Value, const unsigned char MaxInput)
	{
		if (Value <= 0) return 0;
		if (Value >= MaxInput) return 255;

		return static_cast<unsigned char>((Value * 255) / MaxInput);
	}

	static void PrintBufferAsHex(const unsigned char* Buffer, int BufferSize)
	{
		// FString HexString;
		// for (int i = 0; i < BufferSize; i++)
		// {
		// 	// Adiciona o valor hexadecimal de cada byte ao HexString
		// 	HexString += FString::Printf(TEXT("%02X "), Buffer[i]);
		// }
		//
		// UE_LOG(LogTemp, Log, TEXT("Buffer as Hex String: %s"), *HexString);
	}
};
