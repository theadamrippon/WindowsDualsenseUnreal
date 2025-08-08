// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InputCoreTypes.h"
#include "Runtime/ApplicationCore/Public/GenericPlatform/IInputInterface.h"
#include "Core/Enums/EDeviceCommons.h"
#include "DualSenseProxy.generated.h"

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

	UFUNCTION(BlueprintCallable, Category = "DualSense Status")
	static bool DeviceIsConnected(int32 ControllerId);

	UFUNCTION(BlueprintCallable, Category = "DualSense Status")
	static bool DeviceReconnect(int32 ControllerId);

	UFUNCTION(BlueprintCallable, Category = "DualSense Status")
	static bool DeviceDisconnect(int32 ControllerId);

	UFUNCTION(BlueprintCallable, Category = "DualSense Status")
	static float LevelBatteryDevice(int32 ControllerId);

	// Return feedback trigger
	UFUNCTION(BlueprintCallable, Category = "DualSense Status")
	static int32 GetTriggerRightStrengthFeedback(int32 ControllerId);

	// Return feedback trigger
	UFUNCTION(BlueprintCallable, Category = "DualSense Status")
	static int32 GetTriggerLeftStrengthFeedback(int32 ControllerId);

	UFUNCTION(BlueprintCallable, Category = "DualSense Audio Vibration")
	static void SetVibrationFromAudio(
		const int32 ControllerId,
		const float AverageEnvelopeValue,
		const float MaxEnvelopeValue,
		const int32 NumWaveInstances,
		UPARAM(meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		const float EnvelopeToVibrationMultiplier = 0.5,
		UPARAM(meta = (ClampMin = "0.0", ClampMax = "3.0", UIMin = "0.0", UIMax = "3.0"))
		const float PeakToVibrationMultiplier = 0.8,
		UPARAM(meta = (ClampMin = "0.015", ClampMax = "0.1", UIMin = "0.015", UIMax = "0.1"))
		const float Threshold = 0.015f,
		UPARAM(meta = (ClampMin = "0.0", ClampMax = "5.0", UIMin = "0.0", UIMax = "5.0"))
		const float ExponentCurve = 2.f,
		const float BaseMultiplier = 1.5f
	);

	UFUNCTION(BlueprintCallable, Category = "DualSense Effects")
	static void AutomaticGun(
		int32 ControllerId,
		UPARAM(meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
		int32 BeginStrength,
		UPARAM(meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
		int32 MiddleStrength,
		UPARAM(meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
		int32 EndStrength,
		EControllerHand Hand, bool KeepEffect
	);

	UFUNCTION(BlueprintCallable, Category = "DualSense Effects")
	static void SetFeedback(
		int32 ControllerId,
		UPARAM(meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
		int32 BeginStrength,
		UPARAM(meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
		int32 MiddleStrength,
		UPARAM(meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
		int32 EndStrength,
		EControllerHand Hand
	);

	UFUNCTION(BlueprintCallable, Category = "DualSense Effects")
	static void Resistance(
		int32 ControllerId,
		UPARAM(meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
		int32 StartPosition,
		UPARAM(meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
		int32 EndPosition,
		UPARAM(meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
		int32 Strength,
		EControllerHand Hand
	);

	UFUNCTION(BlueprintCallable, Category = "DualSense Effects")
	static void ContinuousResistance(
		int32 ControllerId,
		UPARAM(meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
		int32 StartPosition,
		UPARAM(meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
		int32 Strength,
		EControllerHand Hand
	);

	UFUNCTION(BlueprintCallable, Category = "DualSense Effects")
	static void Bow(
		int32 ControllerId,
		UPARAM(meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
		int32 StartPosition,
		UPARAM(meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
		int32 EndPosition,
		UPARAM(meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
		int32 BeginStrength,
		UPARAM(meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
		int32 EndStrength,
		EControllerHand Hand
	);

	UFUNCTION(BlueprintCallable, Category = "DualSense Effects")
	static void Galloping(
		int32 ControllerId,
		UPARAM(meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
		int32 StartPosition,
		UPARAM(meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
		int32 EndPosition,
		UPARAM(meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
		int32 BeginStrength,
		UPARAM(meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
		int32 EndStrength,
		UPARAM(meta = (ClampMin = "0.01", ClampMax = "1.0", UIMin = "0.01", UIMax = "1.0"))
		float Frequency,
		EControllerHand Hand
	);

	UFUNCTION(BlueprintCallable, Category = "DualSense Effects")
	static void Machine(
		int32 ControllerId,
		UPARAM(meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
		int32 StartPosition,
		UPARAM(meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
		int32 EndPosition,
		UPARAM(meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
		int32 FirstFoot,
		UPARAM(meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
		int32 LasFoot,
		UPARAM(meta = (ClampMin = "0.015", ClampMax = "1.0", UIMin = "0.01", UIMax = "1.0"))
		float Frequency,
		UPARAM(meta = (ClampMin = "0.015", ClampMax = "1.0", UIMin = "0.01", UIMax = "1.0"))
		float Period,
		EControllerHand Hand
	);

	UFUNCTION(BlueprintCallable, Category = "DualSense Effects")
	static void Weapon(
		int32 ControllerId,
		UPARAM(meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
		int32 StartPosition,
		UPARAM(meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
		int32 EndPosition,
		UPARAM(meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
		int32 Strength,
		EControllerHand Hand
	);

	UFUNCTION(BlueprintCallable, Category = "DualSense Led Effects")
	static void LedColorEffects(int32 ControllerId, FColor Color);

	UFUNCTION(BlueprintCallable, Category = "DualSense Led Effects")
	static void LedPlayerEffects(int32 ControllerId, ELedPlayerEnum Value, ELedBrightnessEnum Brightness);

	UFUNCTION(BlueprintCallable, Category = "DualSense Led Effects")
	static void LedMicEffects(int32 ControllerId, ELedMicEnum Value);

	// Touch, Gyroscope and Accelerometer
	UFUNCTION(BlueprintCallable, Category = "DualSense Touch, Gyroscope and Accelerometer")
	static void EnableTouch(int32 ControllerId, bool bEnableTouch);

	UFUNCTION(BlueprintCallable, Category = "DualSense Touch, Gyroscope and Accelerometer")
	static void EnableAccelerometerValues(int32 ControllerId, bool bEnableAccelerometer);

	UFUNCTION(BlueprintCallable, Category = "DualSense Touch, Gyroscope and Accelerometer")
	static void EnableGyroscopeValues(int32 ControllerId, bool bEnableGyroscope);

	// Reset Effects
	UFUNCTION(BlueprintCallable, Category = "DualSense Reset Effects")
	static void NoResistance(int32 ControllerId, EControllerHand Hand);

	UFUNCTION(BlueprintCallable, Category = "DualSense Reset Effects")
	static void StopTriggerEffect(int32 ControllerId, EControllerHand HandStop);

	UFUNCTION(BlueprintCallable, Category = "DualSense Reset Effects")
	static void StopAllTriggersEffects(int32 ControllerId);

	UFUNCTION(BlueprintCallable, Category = "DualSense Reset Effects")
	static void ResetEffects(int32 ControllerId);


	// DEPRECATED
	UE_DEPRECATED(
		5.1, "Methods refactored and deprecated as of plugin version v1.2.1. Use EnableTouch instead of EnableTouch1.")
	UFUNCTION(BlueprintCallable, Category="DualSense Touch Pad",
		meta=(DeprecatedFunction, DeprecationMessage="Use EnableTouch"))
	static void EnableTouch1(int32 ControllerId, bool bEnableTouch)
	{
		EnableTouch(ControllerId, bEnableTouch);
	}

	UE_DEPRECATED(
		5.1, "Methods refactored and deprecated as of plugin version v1.2.1. Use AutomaticGun instead of SetTriggerHapticFeedbackEffect.")
	UFUNCTION(BlueprintCallable, Category="DualSense Effects",
		meta=(DeprecatedFunction, DeprecationMessage="Use AutomaticGun"))
	static void SetTriggerHapticFeedbackEffect(
		int32 ControllerId,
		int32 StartPosition,
		int32 BeginStrength,
		int32 MiddleStrength,
		int32 EndStrength,
		EControllerHand Hand, bool KeepEffect
	)
	{
		AutomaticGun(ControllerId, BeginStrength, MiddleStrength, EndStrength, Hand, KeepEffect);
	}

	UE_DEPRECATED(
		5.1, "Methods refactored and deprecated as of plugin version v1.2.1. Use EnableTouch instead of EnableTouch2.")
	UFUNCTION(BlueprintCallable, Category="DualSense Touch Pad",
		meta=(DeprecatedFunction, DeprecationMessage="Use EnableTouch"))
	static void EnableTouch2(int32 ControllerId, bool bEnableTouch)
	{
		EnableTouch(ControllerId, bEnableTouch);
	}

	UE_DEPRECATED(
		5.1, "Methods refactored and deprecated as of plugin version v1.2.1. Use Machine instead of EffectMachine.")
	UFUNCTION(BlueprintCallable, Category = "DualSense Effects",
		meta=(DeprecatedFunction, DeprecationMessage="Use Machine"))
	static void EffectMachine(int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 FirstFoot,
	                          int32 LasFoot, float Frequency, float Period, EControllerHand Hand)
	{
		Machine(ControllerId, StartPosition, EndPosition, FirstFoot, LasFoot, Frequency, Period, Hand);
	}

	UE_DEPRECATED(5.1, "Methods refactored and deprecated as of plugin version v1.2.1. Use Bow instead of EffectBow.")
	UFUNCTION(BlueprintCallable, Category="DualSense Effects", meta=(DeprecatedFunction, DeprecationMessage="Use Bow"))
	static void EffectBow(int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 BeginStrength, int32 EndStrength,
	                      EControllerHand Hand)
	{
		Bow(ControllerId, StartPosition, EndPosition, BeginStrength, EndStrength, Hand);
	}

	UE_DEPRECATED(
		5.1,
		"Methods refactored and deprecated as of plugin version v1.2.1. Use EffectNoResistance instead of EffectNoResitance.")
	UFUNCTION(BlueprintCallable, Category="DualSense Reset Effects",
		meta=(DeprecatedFunction, DeprecationMessage="Use NoResistance"))
	static void EffectNoResitance(int32 ControllerId, EControllerHand Hand)
	{
		NoResistance(ControllerId, Hand);
	}

	UE_DEPRECATED(
		5.1,
		"Methods refactored and deprecated as of plugin version v1.2.1. Use Resistance instead of EffectSectionResitance.")
	UFUNCTION(BlueprintCallable, Category="DualSense Effects",
		meta=(DeprecatedFunction, DeprecationMessage="Use Resistance"))
	static void EffectSectionResitance(int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 Strength,
	                                   EControllerHand ResistanceHand)
	{
		Resistance(ControllerId, StartPosition, EndPosition, Strength, ResistanceHand);
	}

	UE_DEPRECATED(
		5.1,
		"Methods refactored and deprecated as of plugin version v1.2.1. Use ContinuousResistance instead of EffectContinuousResitance.")
	UFUNCTION(BlueprintCallable, Category="DualSense Effects",
		meta=(DeprecatedFunction, DeprecationMessage="Use ContinuousResistance"))
	static void EffectContinuousResitance(int32 ControllerId, int32 StartPosition, int32 Strength,
	                                      EControllerHand ContinuousHand)
	{
		ContinuousResistance(ControllerId, StartPosition, Strength, ContinuousHand);
	}

	UE_DEPRECATED(
		5.1, "Methods refactored and deprecated as of plugin version v1.2.1. Use Weapon instead of EffectWeapon.")
	UFUNCTION(BlueprintCallable, Category = "DualSense Effects",
		meta=(DeprecatedFunction, DeprecationMessage="Use Weapon"))
	static void EffectWeapon(int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 Strength,
	                         EControllerHand Hand)
	{
		Weapon(ControllerId, StartPosition, EndPosition, Strength, Hand);
	}

	UE_DEPRECATED(
		5.1, "Methods refactored and deprecated as of plugin version v1.2.1. Use Galloping instead of EffectGalloping.")
	UFUNCTION(BlueprintCallable, Category = "DualSense Effects",
		meta=(DeprecatedFunction, DeprecationMessage="Use Galloping"))
	static void EffectGalloping(int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 BeginStrength,
	                            int32 EndStrength, float Frequency, EControllerHand Hand)
	{
		Galloping(ControllerId, StartPosition, EndPosition, BeginStrength, EndStrength, Frequency, Hand);
	}
};
