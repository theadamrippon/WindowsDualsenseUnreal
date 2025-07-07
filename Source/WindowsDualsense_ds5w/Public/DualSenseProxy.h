// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InputCoreTypes.h"
#include "Runtime/ApplicationCore/Public/GenericPlatform/IInputInterface.h"
#include "DualSenseProxy.generated.h"

UENUM(BlueprintType)
enum class ELedMicEnum : uint8
{
	MicOff UMETA(DisplayName = "Mic Off"),
	MicOn UMETA(DisplayName = "Mic On"),
	Pulse UMETA(DisplayName = "Pulse")
};

UENUM(BlueprintType)
enum class ELedPlayerEnum : uint8
{
	Off UMETA(DisplayName = "Led Player Off"),
	One UMETA(DisplayName = "Player One"),
	Two UMETA(DisplayName = "Player Two"),
	Three UMETA(DisplayName = "Player Three"),
	Four UMETA(DisplayName = "Player Four")
};

UENUM(BlueprintType)
enum class ELedBrightnessEnum : uint8
{
	Low UMETA(DisplayName = "Brightness Low"),
	Medium UMETA(DisplayName = "Brightness Medium"),
	High UMETA(DisplayName = "Brightness High")
};


UENUM(BlueprintType)
enum class EDualSenseAudioFeatureReport : uint8
{
	On UMETA(DisplayName = "Audio On"),
	Off UMETA(DisplayName = "Audio Off"),
};

UENUM(BlueprintType)
enum class EDualSenseDeviceFeatureReport : uint8
{
	DefaultRumble UMETA(DisplayName = "Default Rumble"),
	HapictSoftRumble UMETA(DisplayName = "Hapict Soft Rumble")
};


USTRUCT(BlueprintType)
struct FDualSenseFeatureReport
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DualSense")
	EDualSenseAudioFeatureReport MicStatus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DualSense")
	EDualSenseAudioFeatureReport AudioHeadset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DualSense")
	EDualSenseAudioFeatureReport AudioSpeaker;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DualSense")
	EDualSenseDeviceFeatureReport VibrationMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DualSense")
	int32 MicVolume;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DualSense")
	bool SoftRumble;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DualSense")
	int32 AudioVolume;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DualSense")
	int32 SoftRumbleReduce;

	FDualSenseFeatureReport()
		: MicStatus(EDualSenseAudioFeatureReport::Off)
		, AudioHeadset(EDualSenseAudioFeatureReport::Off)
		, AudioSpeaker(EDualSenseAudioFeatureReport::On)
		, VibrationMode(EDualSenseDeviceFeatureReport::HapictSoftRumble)
		, MicVolume(0)
		, SoftRumble(true)
		, AudioVolume(100)
		, SoftRumbleReduce(0)
	{}
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class WINDOWSDUALSENSE_DS5W_API UDualSenseProxy : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "DualSense Settings")
	static void DeviceSettings(int32 ControllerId, FDualSenseFeatureReport Settings);
	
	UFUNCTION(BlueprintCallable, Category = "DualSense")
	static bool DeviceIsConnected(int32 ControllerId);

	UFUNCTION(BlueprintCallable, Category = "DualSense")
	static bool DeviceReconnect(int32 ControllerId);

	UFUNCTION(BlueprintCallable, Category = "DualSense")
	static bool DeviceDisconnect(int32 ControllerId);

	UFUNCTION(BlueprintCallable, Category = "DualSense")
	static float LevelBatteryDevice(int32 ControllerId);

	UFUNCTION(BlueprintCallable, Category = "DualSense Triggers Feedback")
	static void SetFeedback(int32 ControllerId, int32 BeginForce, int32 MiddleForce, int32 EndForce,
	                        EControllerHand Hand);

	// return feedback trigger
	UFUNCTION(BlueprintCallable, Category = "DualSense Triggers Feedback")
	static int32 GetTriggerLeftForceFeedback(int32 ControllerId);

	UFUNCTION(BlueprintCallable, Category = "DualSense Audio Vibration")
	static void SetVibrationFromAudio(
		const int32 ControllerId,
		const float AverageEnvelopeValue,
		const float MaxEnvelopeValue,
		const int32 NumWaveInstances,
		const float EnvelopeToVibrationMultiplier = 0.5,
		const float PeakToVibrationMultiplier = 0.8,
		const float Threshold = 0.015f,
		const float ExponentCurve = 2.f,
		const float BaseMultiplier = 1.5f
	);

	// return feedback trigger
	UFUNCTION(BlueprintCallable, Category = "DualSense Triggers Feedback")
	static int32 GetTriggerRightForceFeedback(int32 ControllerId);

	UFUNCTION(BlueprintCallable, Category = "DualSense Trigger Haptic Effects")
	static void SetTriggerHapticFeedbackEffect(int32 ControllerId, int32 StartPosition, int32 BeginForce,
	                                           int32 MiddleForce, int32 EndForce, EControllerHand Hand,
	                                           bool KeepEffect);

	UFUNCTION(BlueprintCallable, Category = "DualSense Effects")
	static void EffectNoResitance(int32 ControllerId, EControllerHand HandResistence);

	UFUNCTION(BlueprintCallable, Category = "DualSense Effects")
	static void EffectSectionResitance(int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 Force,
	                                   EControllerHand ResistenceHand);

	UFUNCTION(BlueprintCallable, Category = "DualSense Effects")
	static void EffectContinuousResitance(int32 ControllerId, int32 StartPosition, int32 Force,
	                                      EControllerHand ContinuousHand);

	UFUNCTION(BlueprintCallable, Category = "DualSense Effects")
	static void EffectWeapon(int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 Force,
	                         EControllerHand Hand);

	UFUNCTION(BlueprintCallable, Category = "DualSense Effects")
	static void EffectGalloping(int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 BeginForce,
	                            int32 EndForce, float Frequency, EControllerHand Hand);

	UFUNCTION(BlueprintCallable, Category = "DualSense Effects")
	static void EffectMachine(int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 FirstFoot,
	                          int32 LasFoot, float Frequency, float Period, EControllerHand Hand);

	UFUNCTION(BlueprintCallable, Category = "DualSense Effects")
	static void EffectBow(int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 BegingForce, int32 EndForce,
	                      EControllerHand Hand);

	UFUNCTION(BlueprintCallable, Category = "DualSense Effects")
	static void StopTriggerEffect(int32 ControllerId, EControllerHand HandStop);

	UFUNCTION(BlueprintCallable, Category = "DualSense Effects")
	static void StopAllTriggersEffects(int32 ControllerId);

	UFUNCTION(BlueprintCallable, Category = "DualSense Effects")
	static void ResetEffects(int32 ControllerId);

	UFUNCTION(BlueprintCallable, Category = "DualSense Led Effects")
	static void LedColorEffects(int32 ControllerId, FColor Color);

	UFUNCTION(BlueprintCallable, Category = "DualSense Led Effects")
	static void LedPlayerEffects(int32 ControllerId, ELedPlayerEnum Value, ELedBrightnessEnum Brightness);

	UFUNCTION(BlueprintCallable, Category = "DualSense Led Effects")
	static void LedMicEffects(int32 ControllerId, ELedMicEnum Value);

	UFUNCTION(BlueprintCallable, Category = "DualSense Gyroscope and Accelerometer")
	static void EnableAccelerometerValues(int32 ControllerId, bool bEnableAccelerometer);

	UFUNCTION(BlueprintCallable, Category = "DualSense Gyroscope and Accelerometer")
	static void EnableGyroscopeValues(int32 ControllerId, bool bEnableGyroscope);

	UFUNCTION(BlueprintCallable, Category = "DualSense Touch Pad")
	static void EnableTouch1(int32 ControllerId, bool bEnableTouch);

	UFUNCTION(BlueprintCallable, Category = "DualSense Touch Pad")
	static void EnableTouch2(int32 ControllerId, bool bEnableTouch);

	
};
