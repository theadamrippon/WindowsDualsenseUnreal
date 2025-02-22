#include "Public/DualSenseLibrary.h"

#include <Windows.h>
#include <iostream>
#include <algorithm>
#include "InputCoreTypes.h"

#define MAX_DEVICES 8

DS5W::_DeviceEnumInfo* UDualSenseLibrary::Infos;
TMap<int32, DS5W::_DS5InputState> UDualSenseLibrary::InputState;
TMap<int32, DS5W::DS5OutputState> UDualSenseLibrary::OutputState;
TMap<int32, DS5W::DeviceContext> UDualSenseLibrary::DeviceContexts;

int32 UDualSenseLibrary::ControllersCount = 0;
TMap<int32, bool> UDualSenseLibrary::IsInitialized;

bool UDualSenseLibrary::Reconnect(int32 ControllerId)
{
	if (!DeviceContexts.Contains(ControllerId))
	{
		return false;
	}

	if (DS5W_SUCCESS(DS5W::reconnectDevice(&DeviceContexts[ControllerId])))
	{
		return true;
	}

	return false;
}


bool UDualSenseLibrary::InitializeLibrary()
{
	Infos = new DS5W::_DeviceEnumInfo[MAX_DEVICES];

	unsigned int Count = 0;
	switch (DS5W::enumDevices(Infos, MAX_DEVICES, &Count))
	{
	case DS5W_OK:

	case DS5W_E_INSUFFICIENT_BUFFER:
		break;

	default:
		std::cout << "Error enumDevices default: " << GetLastError() << std::endl;
		return false;
	}

	ControllersCount = static_cast<int32>(Count);
	return Connection();
}

bool UDualSenseLibrary::Connection()
{
	for (int32 i = 0; i < ControllersCount; i++)
	{
		if (OutputState.Contains(i))
		{
			ZeroMemory(&OutputState[i], sizeof(DS5W::DS5OutputState));
		}

		if (InputState.Contains(i))
		{
			ZeroMemory(&InputState[i], sizeof(DS5W::_DS5InputState));
		}

		if (DeviceContexts.Contains(i))
		{
			DS5W::freeDeviceContext(&DeviceContexts[i]);
		}

		DS5W::DeviceContext Context;
		if (DS5W_SUCCESS(DS5W::initDeviceContext(&Infos[i], &Context)))
		{
			DS5W::_DS5InputState InState;
			if (!DS5W_SUCCESS(DS5W::getDeviceInputState(&Context, &InState)))
			{
				UE_LOG(LogTemp, Error, TEXT("Error DeviceInputState: %d"), i);
				continue;
			}

			InputState.Add(i, InState);
			DeviceContexts.Add(i, Context);
			OutputState.Add(i, DS5W::DS5OutputState());

			ZeroMemory(&Context, sizeof(DS5W::DeviceContext));
			ZeroMemory(&InState, sizeof(DS5W::_DS5InputState));

			if (IsInitialized.Contains(i))
			{
				IsInitialized[i] = true;
				continue;
			}

			IsInitialized.Add(i, true);
			continue;
		}

		if (IsInitialized.Contains(i))
		{
			IsInitialized[i] = false;
			continue;
		}

		IsInitialized.Add(i, false);
	}

	return true;
}

void UDualSenseLibrary::ShutdownLibrary()
{
	for (int32 i = 0; i < ControllersCount; i++)
	{
		if (!IsInitialized.Contains(i))
		{
			continue;
		}

		if (IsInitialized[i])
		{
			ZeroMemory(&OutputState[i], sizeof(DS5W::DS5OutputState));
			DS5W::freeDeviceContext(&DeviceContexts[i]);
			UE_LOG(LogTemp, Log, TEXT("DualSense ControllerId %d disconnected with success."), i);
		}
	}

	IsInitialized.Reset();
}

bool UDualSenseLibrary::IsConnected(int32 ControllerId)
{
	if (!IsInitialized.Contains(ControllerId))
	{
		return false;
	}

	return IsInitialized[ControllerId];
}

void UDualSenseLibrary::SetConnectionIsValid(int32 ControllerId, bool IsValid)
{
	if (!IsInitialized.Contains(ControllerId))
	{
		return;
	}
	IsInitialized[ControllerId] = IsValid;
}

void UDualSenseLibrary::SendOut(int32 ControllerId)
{
	if (!DeviceContexts.Contains(ControllerId) || !OutputState.Contains(ControllerId))
	{
		return;
	}

	DS5W::setDeviceOutputState(&DeviceContexts[ControllerId], &OutputState[ControllerId]);
}

bool GBPreviousSelect = false;
bool GBPreviousStart = false;
bool GBPreviousPsButton = false;
bool GBPreviousTouchPad = false;
bool GBPreviousMic = false;

bool UDualSenseLibrary::UpdateInput(
	const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
	const FPlatformUserId UserId,
	const FInputDeviceId InputDeviceId
)
{
	if (!DeviceContexts.Contains(InputDeviceId.GetId()) && !InputState.Contains(InputDeviceId.GetId()))
	{
		return false;
	}

	if (
		DS5W_SUCCESS
		(
			DS5W::getDeviceInputState(&DeviceContexts[InputDeviceId.GetId()], &InputState[InputDeviceId.GetId()])
		)
	)
	{
		// Buttons
		CheckCrossInput(InMessageHandler, UserId, InputDeviceId);
		CheckCircleInput(InMessageHandler, UserId, InputDeviceId);
		CheckSquareInput(InMessageHandler, UserId, InputDeviceId);
		CheckTriangleInput(InMessageHandler, UserId, InputDeviceId);

		// DPads
		CheckDPadLeftInput(InMessageHandler, UserId, InputDeviceId);
		CheckDPadRightInput(InMessageHandler, UserId, InputDeviceId);
		CheckDPadUpInput(InMessageHandler, UserId, InputDeviceId);
		CheckDPadDownInput(InMessageHandler, UserId, InputDeviceId);

		// Shoulders
		CheckLeftBumperInput(InMessageHandler, UserId, InputDeviceId);
		CheckRightBumperInput(InMessageHandler, UserId, InputDeviceId);

		// Push Stick
		CheckLeftPushInput(InMessageHandler, UserId, InputDeviceId);
		CheckRightPushInput(InMessageHandler, UserId, InputDeviceId);

		// Specials Actions
		bool TouchPad = InputState[InputDeviceId.GetId()].buttonsB & DS5W_ISTATE_BTN_B_PAD_BUTTON;
		if (TouchPad && !GBPreviousTouchPad)
		{
			InMessageHandler.Get().OnControllerButtonPressed(FName("PS_TouchButtom"), UserId, InputDeviceId, false);
		}
		if (!TouchPad && GBPreviousTouchPad)
		{
			InMessageHandler.Get().OnControllerButtonReleased(FName("PS_TouchButtom"), UserId,
			                                                  InputDeviceId, false);
		}
		GBPreviousTouchPad = TouchPad;

		bool Playstation = InputState[InputDeviceId.GetId()].buttonsB & DS5W_ISTATE_BTN_B_PLAYSTATION_LOGO;
		if (Playstation && !GBPreviousPsButton)
		{
			InMessageHandler.Get().OnControllerButtonPressed(FName("PS_Button"), UserId, InputDeviceId, false);
		}
		if (!Playstation && GBPreviousPsButton)
		{
			InMessageHandler.Get().OnControllerButtonReleased(FName("PS_Button"), UserId, InputDeviceId, false);
		}
		GBPreviousPsButton = Playstation;

		bool Mic = InputState[InputDeviceId.GetId()].buttonsB & DS5W_ISTATE_BTN_B_MIC_BUTTON;
		if (Mic && !GBPreviousMic)
		{
			InMessageHandler.Get().OnControllerButtonPressed(FName("PS_Mic"), UserId, InputDeviceId, false);
		}
		if (!Mic && GBPreviousMic)
		{
			InMessageHandler.Get().OnControllerButtonReleased(FName("PS_Mic"), UserId, InputDeviceId, false);
		}
		GBPreviousMic = Mic;

		bool Select = InputState[InputDeviceId.GetId()].buttonsA & DS5W_ISTATE_BTN_A_SELECT;
		if (Select && !GBPreviousSelect)
		{
			InMessageHandler.Get().OnControllerButtonPressed(FName("PS_Share"), UserId,
			                                                 InputDeviceId, false);
		}
		if (!Select && GBPreviousSelect)
		{
			InMessageHandler.Get().OnControllerButtonReleased(FName("PS_Share"), UserId,
			                                                  InputDeviceId, false);
		}
		GBPreviousSelect = Select;

		bool Start = InputState[InputDeviceId.GetId()].buttonsA & DS5W_ISTATE_BTN_A_MENU;
		if (Start && !GBPreviousStart)
		{
			InMessageHandler.Get().OnControllerButtonPressed(FName("PS_Menu"), UserId,
			                                                 InputDeviceId, false);
		}
		if (!Start && GBPreviousStart)
		{
			InMessageHandler.Get().OnControllerButtonReleased(FName("PS_Menu"), UserId,
			                                                  InputDeviceId, false);
		}
		GBPreviousStart = Start;

		// Analog left
		const FVector2D LeftStick(InputState[InputDeviceId.GetId()].leftStick.x / 128.0f,
		                          InputState[InputDeviceId.GetId()].leftStick.y / 128.0f);;
		InMessageHandler.Get().OnControllerAnalog(EKeys::Gamepad_LeftStick_Left.GetFName(), UserId, InputDeviceId,
		                                          LeftStick.X);
		InMessageHandler.Get().OnControllerAnalog(EKeys::Gamepad_LeftStick_Up.GetFName(), UserId, InputDeviceId,
		                                          LeftStick.Y);


		// Analog right
		const FVector2D RightStick(InputState[InputDeviceId.GetId()].rightStick.x / 128.0f,
		                           InputState[InputDeviceId.GetId()].rightStick.y / 128.0f);
		InMessageHandler.Get().OnControllerAnalog(EKeys::Gamepad_RightStick_Left.GetFName(), UserId, InputDeviceId,
		                                          RightStick.X);
		InMessageHandler.Get().OnControllerAnalog(EKeys::Gamepad_RightStick_Up.GetFName(), UserId, InputDeviceId,
		                                          RightStick.Y);

		// Triggers
		float LeftTrigger = InputState[InputDeviceId.GetId()].leftTrigger / 256.0f;
		float RightTrigger = InputState[InputDeviceId.GetId()].rightTrigger / 256.0f;
		InMessageHandler.Get().OnControllerAnalog(EKeys::Gamepad_LeftTrigger.GetFName(), UserId, InputDeviceId,
		                                          LeftTrigger);
		InMessageHandler.Get().OnControllerAnalog(EKeys::Gamepad_RightTrigger.GetFName(), UserId, InputDeviceId,
		                                          RightTrigger);

		OutputState[InputDeviceId.GetId()].leftRumble = InputState[InputDeviceId.GetId()].leftTriggerFeedback;
		OutputState[InputDeviceId.GetId()].rightRumble = InputState[InputDeviceId.GetId()].rightTriggerFeedback;
		return true;
	}
	return false;
}

bool GBPreviousCross = false;

void UDualSenseLibrary::CheckCrossInput(
	const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
	const FPlatformUserId UserId,
	const FInputDeviceId InputDeviceId
)
{
	auto buttonsAndDpad = InputState[InputDeviceId.GetId()].buttonsAndDpad;
	bool bCross = buttonsAndDpad & DS5W_ISTATE_BTX_CROSS;

	if (bCross && !GBPreviousCross)
	{
		InMessageHandler.Get().OnControllerButtonPressed(EKeys::Gamepad_FaceButton_Bottom.GetFName(), UserId,
		                                                 InputDeviceId, false);
	}

	if (!bCross && GBPreviousCross)
	{
		InMessageHandler.Get().OnControllerButtonReleased(EKeys::Gamepad_FaceButton_Bottom.GetFName(), UserId,
		                                                  InputDeviceId, false);
	}

	GBPreviousCross = bCross;
}


bool GBPreviousCircle = false;

void UDualSenseLibrary::CheckCircleInput(
	const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
	const FPlatformUserId UserId,
	const FInputDeviceId InputDeviceId
)
{
	auto buttonsAndDpad = InputState[InputDeviceId.GetId()].buttonsAndDpad;
	bool bCircle = buttonsAndDpad & DS5W_ISTATE_BTX_CIRCLE;

	if (bCircle && !GBPreviousCircle)
	{
		InMessageHandler.Get().OnControllerButtonPressed(EKeys::Gamepad_FaceButton_Right.GetFName(), UserId,
		                                                 InputDeviceId, false);
	}

	if (!bCircle && GBPreviousCircle)
	{
		InMessageHandler.Get().OnControllerButtonReleased(EKeys::Gamepad_FaceButton_Right.GetFName(), UserId,
		                                                  InputDeviceId, false);
	}

	GBPreviousCircle = bCircle;
}

bool GBPreviousSquare = false;

void UDualSenseLibrary::CheckSquareInput(
	const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
	const FPlatformUserId UserId,
	const FInputDeviceId InputDeviceId
)
{
	auto buttonsAndDpad = InputState[InputDeviceId.GetId()].buttonsAndDpad;
	bool bSquare = buttonsAndDpad & DS5W_ISTATE_BTX_SQUARE;

	if (bSquare && !GBPreviousSquare)
	{
		InMessageHandler.Get().OnControllerButtonPressed(EKeys::Gamepad_FaceButton_Left.GetFName(), UserId,
		                                                 InputDeviceId, false);
	}

	if (!bSquare && GBPreviousSquare)
	{
		InMessageHandler.Get().OnControllerButtonReleased(EKeys::Gamepad_FaceButton_Left.GetFName(), UserId,
		                                                  InputDeviceId, false);
	}

	GBPreviousSquare = bSquare;
}

bool GBPreviousTriangle = false;

void UDualSenseLibrary::CheckTriangleInput(
	const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
	const FPlatformUserId UserId,
	const FInputDeviceId InputDeviceId
)
{
	auto buttonsAndDpad = InputState[InputDeviceId.GetId()].buttonsAndDpad;
	bool bTriangle = buttonsAndDpad & DS5W_ISTATE_BTX_TRIANGLE;

	if (bTriangle && !GBPreviousTriangle)
	{
		InMessageHandler.Get().OnControllerButtonPressed(EKeys::Gamepad_FaceButton_Top.GetFName(), UserId,
		                                                 InputDeviceId, false);
	}

	if (!bTriangle && GBPreviousTriangle)
	{
		InMessageHandler.Get().OnControllerButtonReleased(EKeys::Gamepad_FaceButton_Top.GetFName(), UserId,
		                                                  InputDeviceId, false);
	}

	GBPreviousTriangle = bTriangle;
}

bool GBPreviousDPadLeft = false;

void UDualSenseLibrary::CheckDPadLeftInput(
	const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
	const FPlatformUserId UserId,
	const FInputDeviceId InputDeviceId
)
{
	auto buttonsAndDpad = InputState[InputDeviceId.GetId()].buttonsAndDpad;
	bool dPadLeft = buttonsAndDpad & DS5W_ISTATE_DPAD_LEFT;

	if (dPadLeft && !GBPreviousDPadLeft)
	{
		InMessageHandler.Get().OnControllerButtonPressed(EKeys::Gamepad_DPad_Left.GetFName(), UserId,
		                                                 InputDeviceId, false);
	}

	if (!dPadLeft && GBPreviousDPadLeft)
	{
		InMessageHandler.Get().OnControllerButtonReleased(EKeys::Gamepad_DPad_Left.GetFName(), UserId,
		                                                  InputDeviceId, false);
	}

	GBPreviousDPadLeft = dPadLeft;
}

bool GBPreviousDPadRigth = false;

void UDualSenseLibrary::CheckDPadRightInput(
	const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
	const FPlatformUserId UserId,
	const FInputDeviceId InputDeviceId
)
{
	auto buttonsAndDpad = InputState[InputDeviceId.GetId()].buttonsAndDpad;
	bool dPadRight = buttonsAndDpad & DS5W_ISTATE_DPAD_RIGHT;

	if (dPadRight && !GBPreviousDPadRigth)
	{
		InMessageHandler.Get().OnControllerButtonPressed(EKeys::Gamepad_DPad_Right.GetFName(), UserId,
		                                                 InputDeviceId, false);
	}

	if (!dPadRight && GBPreviousDPadRigth)
	{
		InMessageHandler.Get().OnControllerButtonReleased(EKeys::Gamepad_DPad_Right.GetFName(), UserId,
		                                                  InputDeviceId, false);
	}

	GBPreviousDPadRigth = dPadRight;
}

bool GBPreviousDPadUp = false;

void UDualSenseLibrary::CheckDPadUpInput(
	const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
	const FPlatformUserId UserId,
	const FInputDeviceId InputDeviceId
)
{
	auto buttonsAndDpad = InputState[InputDeviceId.GetId()].buttonsAndDpad;
	bool dPadUp = buttonsAndDpad & DS5W_ISTATE_DPAD_UP;

	if (dPadUp && !GBPreviousDPadUp)
	{
		InMessageHandler.Get().OnControllerButtonPressed(EKeys::Gamepad_DPad_Up.GetFName(), UserId,
		                                                 InputDeviceId, false);
	}

	if (!dPadUp && GBPreviousDPadUp)
	{
		InMessageHandler.Get().OnControllerButtonReleased(EKeys::Gamepad_DPad_Up.GetFName(), UserId,
		                                                  InputDeviceId, false);
	}

	GBPreviousDPadUp = dPadUp;
}

bool GBPreviousDPadDown = false;

void UDualSenseLibrary::CheckDPadDownInput(
	const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
	const FPlatformUserId UserId,
	const FInputDeviceId InputDeviceId
)
{
	auto buttonsAndDpad = InputState[InputDeviceId.GetId()].buttonsAndDpad;
	bool dPadDown = buttonsAndDpad & DS5W_ISTATE_DPAD_UP;

	if (dPadDown && !GBPreviousDPadDown)
	{
		InMessageHandler.Get().OnControllerButtonPressed(EKeys::Gamepad_DPad_Down.GetFName(), UserId,
		                                                 InputDeviceId, false);
	}

	if (!dPadDown && GBPreviousDPadDown)
	{
		InMessageHandler.Get().OnControllerButtonReleased(EKeys::Gamepad_DPad_Down.GetFName(), UserId,
		                                                  InputDeviceId, false);
	}

	GBPreviousDPadDown = dPadDown;
}

bool GBPreviousLeftBumper = false;

void UDualSenseLibrary::CheckLeftBumperInput(
	const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
	const FPlatformUserId UserId,
	const FInputDeviceId InputDeviceId
)
{
	auto buttonsA = InputState[InputDeviceId.GetId()].buttonsA;
	bool leftBumper = buttonsA & DS5W_ISTATE_BTN_A_LEFT_BUMPER;

	if (leftBumper && !GBPreviousLeftBumper)
	{
		InMessageHandler.Get().OnControllerButtonPressed(EKeys::Gamepad_LeftShoulder.GetFName(), UserId,
		                                                 InputDeviceId, false);
	}

	if (!leftBumper && GBPreviousLeftBumper)
	{
		InMessageHandler.Get().OnControllerButtonReleased(EKeys::Gamepad_LeftShoulder.GetFName(), UserId,
		                                                  InputDeviceId, false);
	}

	GBPreviousLeftBumper = leftBumper;
}

bool GBPreviousRightBumper = false;

void UDualSenseLibrary::CheckRightBumperInput(
	const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
	const FPlatformUserId UserId,
	const FInputDeviceId InputDeviceId
)
{
	auto buttonsA = InputState[InputDeviceId.GetId()].buttonsA;
	bool rightBumper = buttonsA & DS5W_ISTATE_BTN_A_RIGHT_BUMPER;

	if (rightBumper && !GBPreviousRightBumper)
	{
		InMessageHandler.Get().OnControllerButtonPressed(EKeys::Gamepad_RightShoulder.GetFName(), UserId,
		                                                 InputDeviceId, false);
	}

	if (!rightBumper && GBPreviousRightBumper)
	{
		InMessageHandler.Get().OnControllerButtonReleased(EKeys::Gamepad_RightShoulder.GetFName(), UserId,
		                                                  InputDeviceId, false);
	}

	GBPreviousRightBumper = rightBumper;
}

bool GBPreviousPushLeftStick = false;

void UDualSenseLibrary::CheckLeftPushInput(
	const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
	const FPlatformUserId UserId,
	const FInputDeviceId InputDeviceId
)
{
	auto buttonsA = InputState[InputDeviceId.GetId()].buttonsA;
	bool PushLeftStick = buttonsA & DS5W_ISTATE_BTN_A_LEFT_STICK;

	if (PushLeftStick && !GBPreviousPushLeftStick)
	{
		InMessageHandler.Get().OnControllerButtonPressed(FName("PS_PushLeftStick"), UserId, InputDeviceId, false);
	}

	if (!PushLeftStick && GBPreviousPushLeftStick)
	{
		InMessageHandler.Get().OnControllerButtonReleased(FName("PS_PushLeftStick"), UserId, InputDeviceId, false);
	}

	GBPreviousPushLeftStick = PushLeftStick;
}

bool GBPreviousPushRightStick = false;

void UDualSenseLibrary::CheckRightPushInput(
	const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
	const FPlatformUserId UserId,
	const FInputDeviceId InputDeviceId
)
{
	auto buttonsA = InputState[InputDeviceId.GetId()].buttonsA;
	bool RightPushStick = buttonsA & DS5W_ISTATE_BTN_A_RIGHT_STICK;

	if (RightPushStick && !GBPreviousPushRightStick)
	{
		InMessageHandler.Get().OnControllerButtonPressed(FName("PS_PushRightStick"), UserId, InputDeviceId, false);
	}

	if (!RightPushStick && GBPreviousPushRightStick)
	{
		InMessageHandler.Get().OnControllerButtonReleased(FName("PS_PushRightStick"), UserId, InputDeviceId, false);
	}

	GBPreviousPushRightStick = RightPushStick;
}

void UDualSenseLibrary::UpdateColorOutput(int32 ControllerId, const FColor Color)
{
	if (!OutputState.Contains(ControllerId))
	{
		return;
	}

	OutputState[ControllerId].lightbar.r = Color.R;
	OutputState[ControllerId].lightbar.g = Color.G;
	OutputState[ControllerId].lightbar.b = Color.B;
}

void UDualSenseLibrary::SetVibration(int32 ControllerId, FForceFeedbackValues Vibration)
{
	if (!OutputState.Contains(ControllerId))
	{
		return;
	}

	OutputState[ControllerId].leftRumble = max(ConvertTo255(Vibration.LeftLarge), ConvertTo255(Vibration.RightLarge));
	OutputState[ControllerId].rightRumble = max(ConvertTo255(Vibration.LeftSmall), ConvertTo255(Vibration.RightSmall));
}

void UDualSenseLibrary::SetTriggerVibration(int32 ControllerId,
                                            const FInputDeviceTriggerVibrationProperty& TriggerVibrationProperty)
{
	if (!OutputState.Contains(ControllerId))
	{
		return;
	}

	if (TriggerVibrationProperty.AffectedTriggers == EInputDeviceTriggerMask::All)
	{
		OutputState[ControllerId].rightTriggerEffect.effectType = DS5W::_TriggerEffectType::ContinuousResitance;
		OutputState[ControllerId].rightTriggerEffect.Continuous.startPosition = 0;
		OutputState[ControllerId].rightTriggerEffect.Continuous.force = TriggerVibrationProperty.VibrationFrequency;
	}
}

void UDualSenseLibrary::SetTriggerResistance(int32 ControllerId,
                                             const FInputDeviceTriggerResistanceProperty& TriggerResistenceProperty)
{
	if (!OutputState.Contains(ControllerId))
	{
		return;
	}

	if (TriggerResistenceProperty.AffectedTriggers == EInputDeviceTriggerMask::All)
	{
		if (OutputState.Contains(ControllerId))
		{
			OutputState[ControllerId].leftTriggerEffect.effectType = DS5W::_TriggerEffectType::SectionResitance;
			OutputState[ControllerId].leftTriggerEffect.Section.startPosition = TriggerResistenceProperty.EndStrengh;
			OutputState[ControllerId].leftTriggerEffect.Section.endPosition = TriggerResistenceProperty.EndStrengh;
			OutputState[ControllerId].leftTriggerEffect.effectType = DS5W::_TriggerEffectType::ContinuousResitance;
			OutputState[ControllerId].leftTriggerEffect.Continuous.startPosition = TriggerResistenceProperty.
				StartPosition;
			OutputState[ControllerId].leftTriggerEffect.Continuous.force = ConvertForceTriggersTo255(
				TriggerResistenceProperty.EndStrengh);

			OutputState[ControllerId].rightTriggerEffect.effectType = DS5W::_TriggerEffectType::SectionResitance;
			OutputState[ControllerId].rightTriggerEffect.Section.startPosition = TriggerResistenceProperty.
				StartPosition;
			OutputState[ControllerId].rightTriggerEffect.Section.endPosition = TriggerResistenceProperty.EndPosition;
			OutputState[ControllerId].rightTriggerEffect.effectType = DS5W::_TriggerEffectType::ContinuousResitance;
			OutputState[ControllerId].rightTriggerEffect.Continuous.startPosition = TriggerResistenceProperty.
				StartPosition;
			OutputState[ControllerId].rightTriggerEffect.Continuous.force = ConvertForceTriggersTo255(
				TriggerResistenceProperty.EndStrengh);
		}
	}
}

int UDualSenseLibrary::ConvertForceTriggersTo255(int Value)
{
	float Min = 0;
	float Max = 7;
	float NormalizedPosition = (Value - Min) / (Max - Min);
	int Value255 = static_cast<int>(NormalizedPosition * 255);
	return std::clamp(Value255, 0, 255);
}

int UDualSenseLibrary::ConvertTo255(float Value)
{
	float Min = 0;
	float Max = 1.0;
	float NormalizedPosition = (Value - Min) / (Max - Min);
	int Value255 = static_cast<int>(NormalizedPosition * 255);
	return std::clamp(Value255, 0, 255);
}
