#pragma once

#include "ds5w.h"
#include "CoreMinimal.h"
#include "UObject/Object.h"
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
	bool InitializeLibrary();
	static void ShutdownLibrary();

	// States and Connection Devices
	static int32 ControllersCount;
	static bool Reconnect(int32 ControllerId);
	static bool IsConnected(int32 ControllerId);
	static void SetConnectionIsValid(int32 ControllerId, bool IsValid);

	// Input
	static bool UpdateInput(
		const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
		const FPlatformUserId UserId,
		const FInputDeviceId InputDeviceId
	);

	// Haptic Feedback and Effects
	static void SetHapticFeedbackValues(int32 ControllerId, int32 Hand, const FHapticFeedbackValues* Values);
	static uint8 GetTrirggersFeedback(int32 ControllerId, const EControllerHand& Hand);
	static void ConfigTriggerHapticFeedbackEffect(int32 ControllerId, uint8 StartPosition, uint8 BeginForce,
	uint8 MiddleForce, uint8 EndForce, const EControllerHand& Hand, bool KeepEffect);

	// Triggers Effects
	static void NoResitance(int32 ControllerId, const EControllerHand& Hand);
	static void ContinuousResitance(int32 ControllerId, uint8 StartPosition, uint8 Force, const EControllerHand& Hand);
	static void SectionResitance(int32 ControllerId, uint8 StartPosition, uint8 EndPosition, const EControllerHand& Hand);
	static void StopEffect(int32 ControllerId, const EControllerHand& Hand);
	static void StopAllEffects(int32 ControllerId);
	static void StopAll(int32 ControllerId);

	// Colors, vibration and triggers config
	static void UpdateColorOutput(int32 ControllerId, FColor Color);
	static void SetVibration(int32 ControllerId, FForceFeedbackValues Vibration);
	static void SetTriggers(int32 ControllerId, const FInputDeviceProperty* Property);

	

private:
	static DS5W::_DeviceEnumInfo* Infos;
	static TMap<int32, DS5W::_DS5InputState> InputState;
	static TMap<int32, DS5W::DS5OutputState> OutputState;
	static TMap<int32, DS5W::DeviceContext> DeviceContexts;
	static TMap<int32, bool> IsInitialized;
	static TMap<int32, uint8> LeftTriggerFeedback;
	static TMap<int32, uint8> RightTriggerFeedback;
	static TMap<FInputDeviceId, TMap<FName, bool>> ButtonStates;

	static bool Connection();
	static void CheckButtonInput(
		const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
		const FPlatformUserId UserId,
		const FInputDeviceId InputDeviceId,
		const FName ButtonName,
		const bool IsButtonPressed
	);

	// Output
	static void SendOut(int32 ControllerId);

	static int ConvertTo255(float Value);
	static unsigned char ConvertTo255(unsigned char value, unsigned char maxInput);
};
