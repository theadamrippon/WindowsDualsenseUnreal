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
	void ShutdownLibrary();

	bool IsConnected(int32 ControllerId);
	void SetConnectionIsValid(int32 ControllerId, bool IsValid);

	static bool UpdateInput(
		const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
		const FPlatformUserId UserId,
		const FInputDeviceId InputDeviceId
	);

	static void ProcessHapticFeedbackBuffer(const FHapticFeedbackBuffer& HapticBuffer);
	static bool Reconnect(int32 ControllerId);
	static void UpdateColorOutput(int32 ControllerId, FColor Color);
	static void SetVibration(int32 ControllerId, FForceFeedbackValues Vibration);
	static void SetTriggers(int32 ControllerId, const FInputDeviceProperty* Property);

	static void SetHapticFeedbackValues(int32 ControllerId, int32 Hand, const  FHapticFeedbackValues* Values);

	static void SendOut(int32 ControllerId);
	static int ConvertForceTriggersTo255(int32 Value);
	static int ConvertTo255(float Value);

	static int32 ControllersCount;

private:
	// Send context DualSense to Unreal
	static DS5W::_DeviceEnumInfo* Infos;
	static TMap<int32, DS5W::_DS5InputState> InputState;
	static TMap<int32, DS5W::DS5OutputState> OutputState;
	static TMap<int32, DS5W::DeviceContext> DeviceContexts;
	static TMap<int32, bool> IsInitialized;

	

	static void CheckCrossInput(
		const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
		const FPlatformUserId UserId,
		const FInputDeviceId InputDeviceId);

	static void CheckCircleInput(
		const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
		const FPlatformUserId UserId,
		const FInputDeviceId InputDeviceId);

	static void CheckSquareInput(
		const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
		const FPlatformUserId UserId,
		const FInputDeviceId InputDeviceId);

	static void CheckTriangleInput(
		const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
		const FPlatformUserId UserId,
		const FInputDeviceId InputDeviceId);

	static void CheckDPadLeftInput(
		const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
		const FPlatformUserId UserId,
		const FInputDeviceId InputDeviceId);

	static void CheckDPadRightInput(
		const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
		const FPlatformUserId UserId,
		const FInputDeviceId InputDeviceId);

	static void CheckDPadUpInput(
		const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
		const FPlatformUserId UserId,
		const FInputDeviceId InputDeviceId);

	static void CheckDPadDownInput(
		const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
		const FPlatformUserId UserId,
		const FInputDeviceId InputDeviceId);

	static void CheckLeftBumperInput(
		const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
		const FPlatformUserId UserId,
		const FInputDeviceId InputDeviceId);

	static void CheckRightBumperInput(
		const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
		const FPlatformUserId UserId,
		const FInputDeviceId InputDeviceId);

	static void CheckLeftPushInput(
		const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
		const FPlatformUserId UserId,
		const FInputDeviceId InputDeviceId);

	static void CheckRightPushInput(
		const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
		const FPlatformUserId UserId,
		const FInputDeviceId InputDeviceId);

	bool Connection();
};
