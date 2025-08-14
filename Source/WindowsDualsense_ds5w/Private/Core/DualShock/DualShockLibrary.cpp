// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/DualShock/DualShockLibrary.h"
#include <Windows.h>
#include "Core/DeviceHIDManager.h"
#include "InputCoreTypes.h"
#include "Core/Structs/FOutputContext.h"
#include "Helpers/ValidateHelpers.h"

void UDualShockLibrary::Settings(const FSettings<FFeatureReport>& Settings)
{
}

void UDualShockLibrary::Settings(const FDualShockFeatureReport& Settings)
{
}

bool UDualShockLibrary::InitializeLibrary(const FDeviceContext& Context)
{
	HIDDeviceContexts = Context;
	SetLightbar(FColor::Green, 0.0f, 0.0f);
	UE_LOG(LogTemp, Log, TEXT("Initializing device model (DualShock 4)"));
	return true;
}

void UDualShockLibrary::ShutdownLibrary()
{
	ButtonStates.Reset();
	UDeviceHIDManager::FreeContext(&HIDDeviceContexts);
}

void UDualShockLibrary::Reconnect()
{
	PlatformInputDeviceMapper.Get().GetOnInputDeviceConnectionChange().Broadcast(
		EInputDeviceConnectionState::Connected, FPlatformUserId::CreateFromInternalId(ControllerID),
		FInputDeviceId::CreateFromInternalId(ControllerID));
}

bool UDualShockLibrary::IsConnected()
{
	return HIDDeviceContexts.IsConnected;
}

void UDualShockLibrary::SendOut()
{
	if (!HIDDeviceContexts.IsConnected)
	{
		return;
	}
	
	UDeviceHIDManager::OutputDualShock(&HIDDeviceContexts);
}

void UDualShockLibrary::CheckButtonInput(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
	const FPlatformUserId UserId, const FInputDeviceId InputDeviceId, const FName ButtonName,
	const bool IsButtonPressed)
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

bool UDualShockLibrary::UpdateInput(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
	const FPlatformUserId UserId, const FInputDeviceId InputDeviceId)
{
	if (UDeviceHIDManager::GetDeviceInputState(&HIDDeviceContexts))
	{
		const unsigned char* HIDInput;
		if (HIDDeviceContexts.ConnectionType == Bluetooth)
		{
			HIDInput = &HIDDeviceContexts.BufferDS4[3];
		}
		else
		{
			HIDInput = &HIDDeviceContexts.Buffer[1];
		}
		
		// Triggers
		const bool bLeftTriggerThreshold = HIDInput[0x05] & BTN_LEFT_TRIGGER;
		const bool bRightTriggerThreshold = HIDInput[0x05] & BTN_RIGHT_TRIGGER;
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::LeftTriggerThreshold, bLeftTriggerThreshold);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::RightTriggerThreshold, bRightTriggerThreshold);

		// Triggers Analog 1D
		const float TriggerL = HIDInput[0x07] / 256.0f;
		const float TriggerR = HIDInput[0x08] / 256.0f;
		InMessageHandler.Get().OnControllerAnalog(FGamepadKeyNames::LeftTriggerAnalog, UserId, InputDeviceId, TriggerL);
		InMessageHandler.Get().OnControllerAnalog(FGamepadKeyNames::RightTriggerAnalog, UserId, InputDeviceId, TriggerR);

		// Analogs
		const float LeftAnalogX = static_cast<char>(static_cast<short>(HIDInput[0x00] - 128));
		const float LeftAnalogY = static_cast<char>(static_cast<short>(HIDInput[0x01] - 127) * -1);
		InMessageHandler.Get().OnControllerAnalog(FGamepadKeyNames::LeftAnalogX, UserId, InputDeviceId, LeftAnalogX / 128.0f);
		InMessageHandler.Get().OnControllerAnalog(FGamepadKeyNames::LeftAnalogY, UserId, InputDeviceId, LeftAnalogY / 128.0f);

		const float RightAnalogX = static_cast<char>(static_cast<short>(HIDInput[0x02] - 128));
		const float RightAnalogY = static_cast<char>(static_cast<short>(HIDInput[0x03] - 127) * -1);
		InMessageHandler.Get().OnControllerAnalog(FGamepadKeyNames::RightAnalogX, UserId, InputDeviceId, RightAnalogX / 128.0f);
		InMessageHandler.Get().OnControllerAnalog(FGamepadKeyNames::RightAnalogY, UserId, InputDeviceId, RightAnalogY / 128.0f);

		uint8_t ButtonsMask = HIDInput[0x04] & 0xF0;
		const bool bCross = ButtonsMask & BTN_CROSS;
		const bool bSquare = ButtonsMask & BTN_SQUARE;
		const bool bCircle = ButtonsMask & BTN_CIRCLE;
		const bool bTriangle = ButtonsMask & BTN_TRIANGLE;

		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::FaceButtonBottom, bCross);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::FaceButtonLeft, bSquare);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::FaceButtonRight, bCircle);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::FaceButtonTop, bTriangle);

		switch (HIDInput[0x04] & 0x0F)
		{
			case 0x0:
				ButtonsMask |= BTN_DPAD_UP;
				break;
			case 0x4:
				ButtonsMask |= BTN_DPAD_DOWN;
				break;
			case 0x6:
				ButtonsMask |= BTN_DPAD_LEFT;
				break;
			case 0x2:
				ButtonsMask |= BTN_DPAD_RIGHT;
				break;
			case 0x5:
				ButtonsMask |= BTN_DPAD_LEFT | BTN_DPAD_DOWN;
				break;
			case 0x7:
				ButtonsMask |= BTN_DPAD_LEFT | BTN_DPAD_UP;
				break;
			case 0x1:
				ButtonsMask |= BTN_DPAD_RIGHT | BTN_DPAD_UP;
				break;
			case 0x3:
				ButtonsMask |= BTN_DPAD_RIGHT | BTN_DPAD_DOWN;
				break;
			default: ;
		}
		const bool bDPadLeft = ButtonsMask & BTN_DPAD_LEFT;
		const bool bDPadDown = ButtonsMask & BTN_DPAD_DOWN;
		const bool bDPadRight = ButtonsMask & BTN_DPAD_RIGHT;
		const bool bDPadUp = ButtonsMask & BTN_DPAD_UP;
		
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::DPadUp, bDPadUp);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::DPadDown, bDPadDown);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::DPadLeft, bDPadLeft);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::DPadRight, bDPadRight);

		// Shoulders
		const bool bLeftShoulder = HIDInput[0x05] & BTN_LEFT_SHOLDER;
		const bool bRightShoulder = HIDInput[0x05] & BTN_RIGHT_SHOLDER;
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::LeftShoulder, bLeftShoulder);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::RightShoulder, bRightShoulder);
		
		// Push Stick
		const bool PushLeftStick = HIDInput[0x05] & BTN_LEFT_STICK;
		const bool PushRightStick = HIDInput[0x05] & BTN_RIGHT_STICK;
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_PushLeftStick"), PushLeftStick);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_PushRightStick"), PushRightStick);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::LeftThumb, PushLeftStick);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::RightThumb, PushRightStick);

		const bool Start = HIDInput[0x05] & BTN_START;
		const bool Select = HIDInput[0x05] & BTN_SELECT;
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_Menu"), Start);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_Share"), Select);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::SpecialRight, Start);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::SpecialLeft, Select);
		
		// UValidateHelpers::PrintBufferAsHex(HIDInput, 78);
		return true;
	}
	return false;
}


void UDualShockLibrary::SetVibration(const FForceFeedbackValues& Values)
{
	FOutputContext* HidOutput = &HIDDeviceContexts.Output;
	const float LeftRumble = FMath::Max(Values.LeftLarge, Values.LeftSmall);
	const float RightRumble = FMath::Max(Values.RightLarge, Values.RightSmall);

	const unsigned char OutputLeft = static_cast<unsigned char>(UValidateHelpers::To255(LeftRumble));
	const unsigned char OutputRight = static_cast<unsigned char>(UValidateHelpers::To255(RightRumble));
	if (HidOutput->Rumbles.Left != OutputLeft || HidOutput->Rumbles.Right != OutputRight)
	{
		HidOutput->Rumbles = {OutputLeft, OutputRight};
		SendOut();
	}
}

void UDualShockLibrary::SetLightbar(FColor Color, float BrithnessTime, float ToggleTime)
{
	FOutputContext* HidOutput = &HIDDeviceContexts.Output;
	HidOutput->Lightbar.R = Color.R;
	HidOutput->Lightbar.G = Color.G;
	HidOutput->Lightbar.B = Color.B;

	HidOutput->FlashLigthbar.Bright_Time = static_cast<unsigned char>(UValidateHelpers::To255(BrithnessTime));
	HidOutput->FlashLigthbar.Toggle_Time = static_cast<unsigned char>(UValidateHelpers::To255(ToggleTime));
	SendOut();
}

void UDualShockLibrary::SetPlayerLed(ELedPlayerEnum Led, ELedBrightnessEnum Brightness)
{
}

void UDualShockLibrary::SetMicrophoneLed(ELedMicEnum Led)
{
}

void UDualShockLibrary::SetTouch(const bool bIsTouch)
{
	EnableTouch = bIsTouch;
}

void UDualShockLibrary::SetAcceleration(bool bIsAccelerometer)
{
	EnableAccelerometerAndGyroscope = bIsAccelerometer;
}

void UDualShockLibrary::SetGyroscope(bool bIsGyroscope)
{
	EnableAccelerometerAndGyroscope = bIsGyroscope;
}

void UDualShockLibrary::StopAll()
{
	SendOut();
}

