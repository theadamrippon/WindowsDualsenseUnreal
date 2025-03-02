// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InputCoreTypes.h" 
#include "DualSenseProxy.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class WINDOWSDUALSENSE_DS5W_API UDualSenseProxy : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "DualSense")
	static bool DeviceIsConnected(int32 ControllerId);
	
	UFUNCTION(BlueprintCallable, Category = "DualSense")
	static bool DeviceReconnect(int32 ControllerId);

	// return feedback trigger
	UFUNCTION(BlueprintCallable, Category = "DualSense Triggers Feedback")
	static uint8 GetTriggerLeftForceFeedback(int32 ControllerId);

	// return feedback trigger
	UFUNCTION(BlueprintCallable, Category = "DualSense Triggers Feedback")
	static uint8 GetTriggerRightForceFeedback(int32 ControllerId);

	UFUNCTION(BlueprintCallable, Category = "DualSense Trigger Haptic Effects")
	static void SetTriggerHapticFeedbackEffect(int32 ControllerId, uint8 StartPosition, uint8 BeginForce, uint8 MiddleForce, uint8 EndForce, const EControllerHand& Hand, bool KeepEffect = false);

	UFUNCTION(BlueprintCallable, Category = "DualSense Effects")
	static void EffectNoResitance(int32 ControllerId, const EControllerHand& Hand);

	UFUNCTION(BlueprintCallable, Category = "DualSense Effects")
	static void EffectContinuousResitance(int32 ControllerId, uint8 StartPosition, uint8 Force, const EControllerHand& Hand);

	UFUNCTION(BlueprintCallable, Category = "DualSense Effects")
	static void EffectSectionResitance(int32 ControllerId, uint8 StartPosition, uint8 EndPosition, const EControllerHand& Hand);

	UFUNCTION(BlueprintCallable, Category = "DualSense Effects")
	static void StopTriggerEffect(int32 ControllerId, const EControllerHand& Hand);

	UFUNCTION(BlueprintCallable, Category = "DualSense Effects")
	static void StopAllTriggersEffects(int32 ControllerId);

	UFUNCTION(BlueprintCallable, Category = "DualSense Effects")
	static void ResetEffects(int32 ControllerId);
};
