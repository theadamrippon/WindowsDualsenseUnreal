// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "Public/DualSenseLibrary.h"

#include <Windows.h>
#include <algorithm>
#include "InputCoreTypes.h"

bool UDualSenseLibrary::Reconnect()
{
	if (IsConnected())
	{
		return true;
	}

	if (DS5W_SUCCESS(DS5W::reconnectDevice(&DeviceContexts)))
	{
		return true;
	}

	return false;
}

bool UDualSenseLibrary::InitializeLibrary(DS5W::DeviceContext& Context)
{
	EnableTouch1 = false;
	EnableTouch2 = false;
	EnableGyroscope = false;
	EnableAccelerometer = false;
	
	DeviceContexts = Context;
	InputState = DS5W::DS5InputState();
	OutputState = DS5W::DS5OutputState();
	return true;
}

bool UDualSenseLibrary::Connection()
{
	return IsConnected();
}

void UDualSenseLibrary::ShutdownLibrary()
{
	DS5W::freeDeviceContext(&DeviceContexts);
	ButtonStates.Reset();
	UE_LOG(LogTemp, Log, TEXT("DualSense: Disconnected with success... ShutdownLibrary"));
}

bool UDualSenseLibrary::IsConnected()
{
	if (DS5W_SUCCESS(DS5W::getDeviceInputState(&DeviceContexts, &InputState)))
	{
		UE_LOG(LogTemp, Log, TEXT("DualSense: Connected with success. %s"), DeviceContexts._internal.connected ? TEXT("YES") : TEXT("NO"));
		return DeviceContexts._internal.connected;
	}
	
	return false;
}

void UDualSenseLibrary::SendOut()
{
	DS5W::setDeviceOutputState(&DeviceContexts, &OutputState);
}

int32 UDualSenseLibrary::GetTrirggersFeedback(const EControllerHand& HandTrigger)
{
	if (DS5W_ReturnValue::OK == getDeviceInputState(&DeviceContexts, &InputState))
	{
		if (HandTrigger == EControllerHand::Left)
		{
			return InputState.leftTriggerFeedback;
		}

		return InputState.rightTriggerFeedback;	
	}
	return 0;
}


void UDualSenseLibrary::CheckButtonInput(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler, const FPlatformUserId UserId, const FInputDeviceId InputDeviceId, const FName ButtonName, const bool IsButtonPressed)
{
	const bool PreviousState = ButtonStates.Contains(ButtonName) ? ButtonStates[ButtonName] : false;
	if (IsButtonPressed && !PreviousState)
	{
		InMessageHandler.Get().OnControllerButtonPressed(ButtonName, UserId, InputDeviceId, false);
	}

	if (!IsButtonPressed && PreviousState)
	{
		InMessageHandler.Get().OnControllerButtonReleased(ButtonName, UserId, InputDeviceId, false);
	}

	ButtonStates.Add(ButtonName, IsButtonPressed);
}

bool UDualSenseLibrary::UpdateInput(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler, const FPlatformUserId UserId, const FInputDeviceId InputDeviceId)
{
	if (
		DS5W_SUCCESS(DS5W::getDeviceInputState(&DeviceContexts, &InputState))
	)
	{
		const auto ButtonsAndDpad = InputState.buttonsAndDpad;
		const auto ButtonsA = InputState.buttonsA;
		const auto ButtonsB = InputState.buttonsB;

		// Buttons
		const bool bCross = ButtonsAndDpad & DS5W_ISTATE_BTX_CROSS;
		const bool bCircle = ButtonsAndDpad & DS5W_ISTATE_BTX_CIRCLE;
		const bool bSquare = ButtonsAndDpad & DS5W_ISTATE_BTX_SQUARE;
		const bool bTriangle = ButtonsAndDpad & DS5W_ISTATE_BTX_TRIANGLE;

		// Face Buttoms
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::FaceButtonTop, bTriangle);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::FaceButtonBottom, bCross);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::FaceButtonRight, bCircle);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::FaceButtonLeft, bSquare);

		// DPad
		const bool bDPadUp = ButtonsAndDpad & DS5W_ISTATE_DPAD_UP;
		const bool bDPadDown = ButtonsAndDpad & DS5W_ISTATE_DPAD_DOWN;
		const bool bDPadLeft = ButtonsAndDpad & DS5W_ISTATE_DPAD_LEFT;
		const bool bDPadRight = ButtonsAndDpad & DS5W_ISTATE_DPAD_RIGHT;
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::DPadUp, bDPadUp);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::DPadDown, bDPadDown);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::DPadLeft, bDPadLeft);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::DPadRight, bDPadRight);
		
		// Shoulders
		const bool bLeftBumper = ButtonsA & DS5W_ISTATE_BTN_A_LEFT_BUMPER;
		const bool bRightBumper = ButtonsA & DS5W_ISTATE_BTN_A_RIGHT_BUMPER;
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::LeftShoulder, bLeftBumper);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::RightShoulder, bRightBumper);
		
		// Push Stick
		const bool PushLeftStick = ButtonsA & DS5W_ISTATE_BTN_A_LEFT_STICK;
		const bool PushRightStick = ButtonsA & DS5W_ISTATE_BTN_A_RIGHT_STICK;
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_PushLeftStick"), PushLeftStick);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_PushRightStick"), PushRightStick);
		
		
		// Specials Actions
		const bool Mic = ButtonsB & DS5W_ISTATE_BTN_B_MIC_BUTTON;
		const bool TouchPad = ButtonsB & DS5W_ISTATE_BTN_B_PAD_BUTTON;
		const bool Playstation = ButtonsB & DS5W_ISTATE_BTN_B_PLAYSTATION_LOGO;
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_Mic"), Mic);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_TouchButtom"), TouchPad);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_Button"), Playstation);
		
		const bool Start = ButtonsA & DS5W_ISTATE_BTN_A_MENU;
		const bool Select = ButtonsA & DS5W_ISTATE_BTN_A_SELECT;
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_Menu"), Start);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_Share"), Select);
		
		// Analog left
		const float LeftX = InputState.leftStick.x / 128.f;
		const float LeftY = InputState.leftStick.y / 128.f;
		InMessageHandler.Get().OnControllerAnalog(FGamepadKeyNames::LeftAnalogX, UserId, InputDeviceId, LeftX);
		InMessageHandler.Get().OnControllerAnalog(FGamepadKeyNames::LeftAnalogY, UserId, InputDeviceId, LeftY);
		
		// Analog right
		const float RightX = InputState.rightStick.x / 128.0f;
		const float RightY = InputState.rightStick.y / 128.0f;
		InMessageHandler.Get().OnControllerAnalog(FGamepadKeyNames::RightAnalogX, UserId, InputDeviceId, RightX);
		InMessageHandler.Get().OnControllerAnalog(FGamepadKeyNames::RightAnalogY, UserId, InputDeviceId, RightY);
		
		// Triggers
		const float TriggerL = InputState.leftTrigger / 256.0f;
		const float TriggerR = InputState.rightTrigger / 256.0f;
		InMessageHandler.Get().OnControllerAnalog(EKeys::Gamepad_LeftTrigger.GetFName(), UserId, InputDeviceId, TriggerL);
		InMessageHandler.Get().OnControllerAnalog(EKeys::Gamepad_RightTrigger.GetFName(), UserId, InputDeviceId, TriggerR);
		
		
		if (EnableTouch1 || EnableTouch2)
		{
			const unsigned int MaxX = 2000;
			const unsigned int MaxY = 2048;
		
			if (EnableTouch1)
			{
				DS5W::Touch TouchPoint1 = InputState.touchPoint1;
				float Touch1X = (2.0f * TouchPoint1.x / MaxX) - 1.0f;
				float Touch1Y = (2.0f * TouchPoint1.y / MaxY) - 1.0f;
				InMessageHandler->OnControllerAnalog(FName("Dualsense_Touch1_X"), UserId, InputDeviceId, Touch1X);
				InMessageHandler->OnControllerAnalog(FName("Dualsense_Touch1_Y"), UserId, InputDeviceId, Touch1Y);
			}
		
			if (EnableTouch2)
			{
				DS5W::Touch TouchPoint2 = InputState.touchPoint2;
				float Touch2X = (2.0f * TouchPoint2.x / MaxX) - 1.0f;
				float Touch2Y = (2.0f * TouchPoint2.y / MaxY) - 1.0f;
				InMessageHandler->OnControllerAnalog(FName("Dualsense_Touch1_X"), UserId, InputDeviceId, Touch2X);
				InMessageHandler->OnControllerAnalog(FName("Dualsense_Touch2_Y"), UserId, InputDeviceId, Touch2Y);
			}
		}
		
		
		if (EnableAccelerometer || EnableGyroscope)
		{
			DS5W::Vector3 Accelerometer = InputState.accelerometer;
			DS5W::Vector3 Gyroscope = InputState.gyroscope;
		
			if (EnableAccelerometer)
			{
				InMessageHandler.Get().OnControllerAnalog(EKeys::Acceleration.GetFName(), UserId, InputDeviceId, Accelerometer.x);
				InMessageHandler.Get().OnControllerAnalog(EKeys::Acceleration.GetFName(), UserId, InputDeviceId, Accelerometer.y);
				InMessageHandler.Get().OnControllerAnalog(EKeys::Acceleration.GetFName(), UserId, InputDeviceId, Accelerometer.z);
			}
		
			if (EnableGyroscope)
			{
				InMessageHandler.Get().OnControllerAnalog(EKeys::RotationRate.GetFName(), UserId, InputDeviceId, Gyroscope.x);
				InMessageHandler.Get().OnControllerAnalog(EKeys::RotationRate.GetFName(), UserId, InputDeviceId, Gyroscope.y);
				InMessageHandler.Get().OnControllerAnalog(EKeys::RotationRate.GetFName(), UserId, InputDeviceId, Gyroscope.z);
			}
		
			if (EnableGyroscope && EnableAccelerometer)
			{
				FVector Tilt = FVector(Accelerometer.x + Gyroscope.x, Accelerometer.y + Gyroscope.y, Accelerometer.z + Gyroscope.z);
				InMessageHandler.Get().OnControllerAnalog(EKeys::Tilt.GetFName(), UserId, InputDeviceId, Tilt.X);
				InMessageHandler.Get().OnControllerAnalog(EKeys::Tilt.GetFName(), UserId, InputDeviceId, Tilt.Y);
				InMessageHandler.Get().OnControllerAnalog(EKeys::Tilt.GetFName(), UserId, InputDeviceId, Tilt.Z);
			}
		}

		return true;
	}
	return false;
}

void UDualSenseLibrary::UpdateColorOutput(const FColor Color)
{
	OutputState.lightbar.r = Color.R;
	OutputState.lightbar.g = Color.G;
	OutputState.lightbar.b = Color.B;
	SendOut();
}

bool IsResetVibration = false;
void UDualSenseLibrary::SetVibration(const FForceFeedbackValues& Vibration)
{
	const float LeftRumble = CalculateLeftRumble(Vibration);
	const float RightRumble = CalculateRightRumble(Vibration);

	if (IsResetVibration && (RightRumble <= 0 || LeftRumble <= 0))
	{
		IsResetVibration = false;
		OutputState.leftRumble = 0;
		OutputState.rightRumble = 0;
		SendOut();
	}
	
	if (RightRumble > 0 || LeftRumble > 0)
	{
		IsResetVibration = true;
		OutputState.leftRumble = LeftRumble;
		OutputState.rightRumble = RightRumble;
		SendOut();
	}
}

unsigned char UDualSenseLibrary::CalculateLeftRumble(const FForceFeedbackValues& Values)
{
	float CombinedLeft = Values.LeftLarge * 0.8f + Values.LeftSmall * 0.2f;
	return static_cast<unsigned char>(FMath::Clamp(CombinedLeft * 255.0f, 0.0f, 255.0f));
}

unsigned char UDualSenseLibrary::CalculateRightRumble(const FForceFeedbackValues& Values)
{
	float CombinedRight = Values.RightLarge * 0.8f + Values.RightSmall * 0.2f;
	return static_cast<unsigned char>(FMath::Clamp(CombinedRight * 255.0f, 0.0f, 255.0f));
}

void UDualSenseLibrary::SetHapticFeedbackValues(int32 Hand, const FHapticFeedbackValues* Values)
{
	// Config (L2)
	if (Hand == static_cast<int32>(EControllerHand::Left) || Hand == static_cast<int32>(EControllerHand::AnyHand))
	{
		OutputState.leftTriggerEffect.EffectEx.frequency = ConvertTo255(Values->Frequency);
	}

	// Config (R2)
	if (Hand == static_cast<int32>(EControllerHand::Right) || Hand == static_cast<int32>(EControllerHand::AnyHand))
	{
		OutputState.rightTriggerEffect.EffectEx.frequency = ConvertTo255(Values->Frequency);
	}

	SendOut();
}

void UDualSenseLibrary::SetTriggers(const FInputDeviceProperty* Property)
{
	
	if (Property->Name == FName("InputDeviceTriggerResistance"))
	{
		const FInputDeviceTriggerResistanceProperty* Resistance = static_cast<const
			FInputDeviceTriggerResistanceProperty*>(Property);
		if (
			Resistance->AffectedTriggers == EInputDeviceTriggerMask::Left ||
			Resistance->AffectedTriggers == EInputDeviceTriggerMask::All
		)
		{
			OutputState.leftTriggerEffect.effectType = DS5W::_TriggerEffectType::SectionResitance;
			OutputState.leftTriggerEffect._u1_raw[0] = ConvertTo255(Resistance->StartPosition, 8);
			OutputState.leftTriggerEffect._u1_raw[1] = ConvertTo255(Resistance->EndPosition, 8);
			OutputState.leftTriggerEffect._u1_raw[2] = ConvertTo255(Resistance->StartStrengh, 9);
			OutputState.leftTriggerEffect._u1_raw[3] = ConvertTo255(Resistance->EndStrengh, 9);
		}

		if (
			Resistance->AffectedTriggers == EInputDeviceTriggerMask::Right ||
			Resistance->AffectedTriggers == EInputDeviceTriggerMask::All
		)
		{
			OutputState.rightTriggerEffect.effectType = DS5W::_TriggerEffectType::SectionResitance;
			OutputState.rightTriggerEffect._u1_raw[0] = ConvertTo255(Resistance->StartPosition, 8);
			OutputState.rightTriggerEffect._u1_raw[1] = ConvertTo255(Resistance->EndPosition, 8);
			OutputState.rightTriggerEffect._u1_raw[2] = ConvertTo255(Resistance->StartStrengh, 9);
			OutputState.rightTriggerEffect._u1_raw[3] = ConvertTo255(Resistance->EndStrengh, 9);
		}
	}

	SendOut();
}

void UDualSenseLibrary::SetAcceleration(bool bIsAccelerometer)
{
	EnableAccelerometer = bIsAccelerometer;
}

void UDualSenseLibrary::SetGyroscope(bool bIsGyroscope)
{
	EnableGyroscope = bIsGyroscope;
}

void UDualSenseLibrary::SetTouch1(bool bIsGyroscope)
{
	EnableTouch1 = bIsGyroscope;
}

void UDualSenseLibrary::SetTouch2(bool bIsGyroscope)
{
	EnableTouch2 = bIsGyroscope;
}

void UDualSenseLibrary::ConfigTriggerHapticFeedbackEffect(
	int32 StartPosition, int32 BeginForce, int32 MiddleForce, int32 EndForce,
	const EControllerHand& Hand, bool KeepEffect
)
{
	if (Hand == EControllerHand::Left || Hand == EControllerHand::AnyHand)
	{
		OutputState.leftTriggerEffect.effectType = DS5W::_TriggerEffectType::EffectEx;
		OutputState.leftTriggerEffect.EffectEx.startPosition = ConvertTo255(StartPosition, 8);
		OutputState.leftTriggerEffect.EffectEx.keepEffect = KeepEffect;
		OutputState.leftTriggerEffect.EffectEx.beginForce = ConvertTo255(BeginForce, 9);
		OutputState.leftTriggerEffect.EffectEx.middleForce = ConvertTo255(MiddleForce, 9);
		OutputState.leftTriggerEffect.EffectEx.endForce = ConvertTo255(EndForce, 9);
		OutputState.leftTriggerEffect.EffectEx.frequency = 0;
	}

	if (Hand == EControllerHand::Right || Hand == EControllerHand::AnyHand)
	{
		OutputState.rightTriggerEffect.effectType = DS5W::_TriggerEffectType::EffectEx;
		OutputState.rightTriggerEffect.EffectEx.startPosition = ConvertTo255(StartPosition, 8);
		OutputState.rightTriggerEffect.EffectEx.keepEffect = KeepEffect;
		OutputState.rightTriggerEffect.EffectEx.beginForce = ConvertTo255(BeginForce, 9);
		OutputState.rightTriggerEffect.EffectEx.middleForce = ConvertTo255(MiddleForce, 9);
		OutputState.rightTriggerEffect.EffectEx.endForce = ConvertTo255(EndForce, 9);
		OutputState.rightTriggerEffect.EffectEx.frequency = 0;
	}

	SendOut();
}

void UDualSenseLibrary::NoResitance(const EControllerHand& Hand)
{
	if (Hand == EControllerHand::Left || Hand == EControllerHand::AnyHand)
	{
		OutputState.leftTriggerEffect.effectType = DS5W::_TriggerEffectType::NoResitance;
	}

	if (Hand == EControllerHand::Right || Hand == EControllerHand::AnyHand)
	{
		OutputState.rightTriggerEffect.effectType = DS5W::_TriggerEffectType::NoResitance;
	}

	SendOut();
}

void UDualSenseLibrary::ContinuousResitance(int32 StartPosition, int32 Force, const EControllerHand& Hand)
{
	
	if (Hand == EControllerHand::Left || Hand == EControllerHand::AnyHand)
	{
		OutputState.leftTriggerEffect.effectType = DS5W::_TriggerEffectType::ContinuousResitance;
		OutputState.leftTriggerEffect.Continuous.startPosition = ConvertTo255(StartPosition, 8);
		OutputState.leftTriggerEffect.Continuous.force = ConvertTo255(Force, 9);
	}

	if (Hand == EControllerHand::Right || Hand == EControllerHand::AnyHand)
	{
		OutputState.rightTriggerEffect.effectType = DS5W::_TriggerEffectType::ContinuousResitance;
		OutputState.rightTriggerEffect.Continuous.startPosition = ConvertTo255(StartPosition, 8);
		OutputState.rightTriggerEffect.Continuous.force = ConvertTo255(Force, 9);
	}

	SendOut();
}

void UDualSenseLibrary::SectionResitance(int32 StartPosition, int32 EndPosition, const EControllerHand& Hand)
{
	
	if (Hand == EControllerHand::Left || Hand == EControllerHand::AnyHand)
	{
		OutputState.leftTriggerEffect.effectType = DS5W::_TriggerEffectType::SectionResitance;
		OutputState.leftTriggerEffect.Section.startPosition = ConvertTo255(StartPosition, 8);
		OutputState.leftTriggerEffect.Section.endPosition = ConvertTo255(EndPosition, 8);
	}

	if (Hand == EControllerHand::Right || Hand == EControllerHand::AnyHand)
	{
		OutputState.rightTriggerEffect.effectType = DS5W::_TriggerEffectType::SectionResitance;
		OutputState.rightTriggerEffect.Section.startPosition = ConvertTo255(StartPosition, 8);
		OutputState.rightTriggerEffect.Section.endPosition = ConvertTo255(EndPosition, 8);
	}


	SendOut();
}

void UDualSenseLibrary::StopEffect(const EControllerHand& Hand)
{
	
	if (Hand == EControllerHand::Left || Hand == EControllerHand::AnyHand)
	{
		OutputState.leftTriggerEffect.effectType = DS5W::_TriggerEffectType::NoResitance;
	}

	if (Hand == EControllerHand::Right || Hand == EControllerHand::AnyHand)
	{
		OutputState.rightTriggerEffect.effectType = DS5W::_TriggerEffectType::NoResitance;
	}

	SendOut();
}

void UDualSenseLibrary::StopAllEffects()
{
	
	OutputState.leftTriggerEffect.effectType = DS5W::_TriggerEffectType::NoResitance;
	OutputState.rightTriggerEffect.effectType = DS5W::_TriggerEffectType::NoResitance;
	SendOut();
}

void UDualSenseLibrary::StopAll()
{
	ZeroMemory(&OutputState, sizeof(DS5W::DS5OutputState));
	SendOut();
}

void UDualSenseLibrary::SetLedPlayerEffects(int32 NumberLeds, int32 BrightnessValue)
{
	OutputState.playerLeds.bitmask = 0x00;
	NumberLeds = FMath::Clamp(NumberLeds, 0, 3);
	if (NumberLeds == 1)
	{
		OutputState.playerLeds.bitmask = DS5W_OSTATE_PLAYER_LED_MIDDLE;
	}

	if (NumberLeds == 2)
	{
		OutputState.playerLeds.bitmask = DS5W_OSTATE_PLAYER_LED_LEFT | DS5W_OSTATE_PLAYER_LED_RIGHT;
	}

	if (NumberLeds == 3)
	{
		OutputState.playerLeds.bitmask = DS5W_OSTATE_PLAYER_LED_MIDDLE;
		OutputState.playerLeds.bitmask |= DS5W_OSTATE_PLAYER_LED_LEFT | DS5W_OSTATE_PLAYER_LED_RIGHT;
		OutputState.playerLeds.bitmask |= DS5W_OSTATE_PLAYER_LED_MIDDLE_RIGHT | DS5W_OSTATE_PLAYER_LED_MIDDLE_LEFT;
	}

	DS5W::LedBrightness Brightness;
	switch (BrightnessValue)
	{
	case 1:
		Brightness = DS5W::LedBrightness::MEDIUM;
		break;
	case 2:
		Brightness = DS5W::LedBrightness::HIGH;
		break;
	case 3:
	default:
		Brightness = DS5W::LedBrightness::LOW;
		break;
	}
	OutputState.playerLeds.brightness = Brightness;

	SendOut();
}

void UDualSenseLibrary::SetLedMicEffects(int32 LedMic)
{
	OutputState.microphoneLed = DS5W::MicLed::OFF;
	if (LedMic == 1)
	{
		OutputState.microphoneLed = DS5W::MicLed::ON;
	}

	if (LedMic == 2)
	{
		OutputState.microphoneLed = DS5W::MicLed::PULSE;
	}

	SendOut();
}

float UDualSenseLibrary::GetLevelBattery()
{
	if (DS5W_SUCCESS(DS5W::getDeviceInputState(&DeviceContexts, &InputState)))
	{
		return ((DeviceContexts._internal.hidBuffer[0x34] & 0x0F) * 100) / 15;
	}
	
	return 0.00f;
}

unsigned char UDualSenseLibrary::ConvertTo255(unsigned char value, unsigned char maxInput)
{
	return static_cast<unsigned char>((value * 255) / maxInput);
}

int UDualSenseLibrary::ConvertTo255(const float Value)
{
	const float Min = 0;
	const float Max = 1.0;
	const float NormalizedPosition = (Value - Min) / (Max - Min);
	const int Value255 = static_cast<int>(NormalizedPosition * 255);
	return std::clamp(Value255, 0, 255);
}
