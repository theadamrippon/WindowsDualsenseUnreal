// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "DualSenseLibrary.h"

#include <Windows.h>
#include <algorithm>
#include "InputCoreTypes.h"


//
bool UDualSenseLibrary::Reconnect()
{
	PlatformInputDeviceMapper.Get().GetOnInputDeviceConnectionChange().Broadcast(EInputDeviceConnectionState::Connected, FPlatformUserId::CreateFromInternalId(ControllerID), FInputDeviceId::CreateFromInternalId(ControllerID));
	return true;
}


bool UDualSenseLibrary::InitializeLibrary(const FHIDDeviceContext& Context)
{
	EnableTouch1 = false;
	EnableTouch2 = false;
	EnableGyroscope = false;
	EnableAccelerometer = false;

	HIDDeviceContexts = Context;
	StopAll();
	return true;
}

bool UDualSenseLibrary::Connection()
{
	return IsConnected();
}

void UDualSenseLibrary::ShutdownLibrary()
{
	ButtonStates.Reset();
	
	HIDDeviceContexts.Internal.Connected = false;
	HIDDeviceContexts.Internal.Connection = EHIDDeviceConnection::Unknown;
	ZeroMemory(&HIDDeviceContexts.Internal.DevicePath, sizeof(HIDDeviceContexts.Internal.DevicePath));
	ZeroMemory(&HIDDeviceContexts.Internal.Buffer, sizeof(HIDDeviceContexts.Internal.Buffer));
	CloseHandle(HIDDeviceContexts.Internal.DeviceHandle);
	UE_LOG(LogTemp, Log, TEXT("DualSense: Disconnected with success... ShutdownLibrary"));
}

bool UDualSenseLibrary::IsConnected()
{
	return HIDDeviceContexts.Internal.Connected;
}

void UDualSenseLibrary::SendOut()
{
	if (!HIDDeviceContexts.Internal.Connected || HIDDeviceContexts.Internal.Connection == EHIDDeviceConnection::Unknown)
	{
		return;
	}
	
	DualSenseHIDManager::OutputBuffering(&HIDDeviceContexts, HidOutput);
}

int32 UDualSenseLibrary::GetTrirggersFeedback(const EControllerHand& HandTrigger)
{
	// if (DS5W_ReturnValue::OK == getDeviceInputState(&DeviceContexts, &InputState))
	// {
	// 	if (HandTrigger == EControllerHand::Left)
	// 	{
	// 		return InputState.leftTriggerFeedback;
	// 	}
	//
	// 	return InputState.rightTriggerFeedback;	
	// }
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
	unsigned char* HIDInput = HIDDeviceContexts.Internal.Connection ==  EHIDDeviceConnection::Bluetooth ? &HIDDeviceContexts.Internal.Buffer[2] : &HIDDeviceContexts.Internal.Buffer[1];
	if (DualSenseHIDManager::GetDeviceInputState(&HIDDeviceContexts, HIDInput))
	{
		// Shoulders
		const bool bLeftShoulder = HIDInput[0x08] & BTN_LEFT_SHOLDER;
		const bool bRightShoulder = HIDInput[0x08] & BTN_RIGHT_SHOLDER;

		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::LeftShoulder, bLeftShoulder);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::RightShoulder, bRightShoulder);

		// Push Stick
		const bool PushLeftStick = HIDInput[0x08] & BTN_LEFT_STICK;
		const bool PushRightStick = HIDInput[0x08] & BTN_RIGHT_STICK;
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_PushLeftStick"), PushLeftStick);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_PushRightStick"), PushRightStick);
		// mapped urenal native gamepad Push Stick
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::LeftThumb, PushLeftStick);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::RightThumb, PushRightStick);

		// Function & Special Actions
		const bool Playstation = HIDInput[0x09] & BTN_PLAYSTATION_LOGO;
		const bool TouchPad = HIDInput[0x09] & BTN_PAD_BUTTON;
		const bool Mic = HIDInput[0x09] & BTN_MIC_BUTTON;
		const bool bFn1 = HIDInput[0x09] & BTN_FN1;
		const bool bFn2 = HIDInput[0x09] & BTN_FN2;
		const bool bPaddleLeft = HIDInput[0x09] & BTN_PADDLE_LEFT;
		const bool bPaddleRight = HIDInput[0x09] & BTN_PADDLE_RIGHT;
		
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_Mic"), Mic);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_TouchButtom"), TouchPad);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_Button"), Playstation);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_FunctionL"), bFn1);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_FunctionR"), bFn2);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_PaddleL"),  bPaddleLeft);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_PaddleR"),  bPaddleRight);
		
		const bool Start = HIDInput[0x08] & BTN_START;
		const bool Select = HIDInput[0x08] & BTN_SELECT;
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_Menu"), Start);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_Share"), Select);
		// mapped urenal native gamepad Start and Select
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::SpecialRight, Start);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::SpecialLeft, Select);

		const float LeftAnalogX = static_cast<char>(static_cast<short>(HIDInput[0x00] - 128));
		const float LeftAnalogY = static_cast<char>(static_cast<short>(HIDInput[0x01] - 127) * -1);
		InMessageHandler.Get().OnControllerAnalog(FGamepadKeyNames::LeftAnalogX, UserId, InputDeviceId, LeftAnalogX / 128.0f);
		InMessageHandler.Get().OnControllerAnalog(FGamepadKeyNames::LeftAnalogY, UserId, InputDeviceId, LeftAnalogY / 128.0f);
		
		const float RightAnalogX = static_cast<char>(static_cast<short>(HIDInput[0x02] - 128));
		const float RightAnalogY = static_cast<char>(static_cast<short>(HIDInput[0x03] - 127) * -1);
		InMessageHandler.Get().OnControllerAnalog(FGamepadKeyNames::RightAnalogX, UserId, InputDeviceId, RightAnalogX / 128.0f);
		InMessageHandler.Get().OnControllerAnalog(FGamepadKeyNames::RightAnalogY, UserId, InputDeviceId, RightAnalogY / 128.0f);


		// Triggers
		const float TriggerL = HIDInput[0x04] / 256.0f;
		const float TriggerR = HIDInput[0x05] / 256.0f;
		InMessageHandler.Get().OnControllerAnalog(EKeys::Gamepad_LeftTrigger.GetFName(), UserId, InputDeviceId, TriggerL);
		InMessageHandler.Get().OnControllerAnalog(EKeys::Gamepad_RightTrigger.GetFName(), UserId, InputDeviceId, TriggerR);


		uint8_t ButtonsMask = HIDInput[0x07] & 0xF0;
		const bool bCross = ButtonsMask & BTN_CROSS;
		const bool bSquare = ButtonsMask & BTN_SQUARE;
		const bool bCircle = ButtonsMask & BTN_CIRCLE;
		const bool bTriangle = ButtonsMask & BTN_TRIANGLE;

		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::FaceButtonBottom, bCross);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::FaceButtonLeft, bSquare);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::FaceButtonRight, bCircle);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::FaceButtonTop, bTriangle);
		
		switch (HIDInput[0x07] & 0x0F) {
				// Up
			case 0x0:
				ButtonsMask |= BTN_DPAD_UP;
				break;
				// Down
			case 0x4:
				ButtonsMask |= BTN_DPAD_DOWN;
				break;
				// Left
			case 0x6:
				ButtonsMask |= BTN_DPAD_LEFT;
				break;
				// Right
			case 0x2:
				ButtonsMask |= BTN_DPAD_RIGHT;
				break;
				// Left Down
			case 0x5:
				ButtonsMask |= BTN_DPAD_LEFT | BTN_DPAD_DOWN;
				break;
				// Left Up
			case 0x7:
				ButtonsMask |= BTN_DPAD_LEFT | BTN_DPAD_UP;
				break;
				// Right Up
			case 0x1:
				ButtonsMask |= BTN_DPAD_RIGHT | BTN_DPAD_UP;
				break;
				// Right Down
			case 0x3:
				ButtonsMask |= BTN_DPAD_RIGHT | BTN_DPAD_DOWN;
				break;
			default: ;
		}

		// D-pad buffer
		const bool bDPadLeft = ButtonsMask & BTN_DPAD_LEFT;
		const bool bDPadDown = ButtonsMask & BTN_DPAD_DOWN;
		const bool bDPadRight = ButtonsMask & BTN_DPAD_RIGHT;
		const bool bDPadUp = ButtonsMask & BTN_DPAD_UP;

		// DPad
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::DPadUp, bDPadUp);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::DPadDown, bDPadDown);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::DPadLeft, bDPadLeft);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::DPadRight, bDPadRight);
	
		if (EnableTouch1 || EnableTouch2)
		{
			// Evaluate touch state 1
			FTouchPoint1 Touch;
			const UINT32 Touchpad1Raw = *reinterpret_cast<UINT32*>(&HIDInput[0x20]);
			Touch.Y = (Touchpad1Raw & 0xFFF00000) >> 20;
			Touch.X = (Touchpad1Raw & 0x000FFF00) >> 8;
			Touch.Down = (Touchpad1Raw & (1 << 7)) == 0;
			Touch.Id = (Touchpad1Raw & 127);

			// // Evaluate touch state 2
			FTouchPoint2 Touch2;
			const UINT32 Touchpad2Raw = *reinterpret_cast<UINT32*>(&HIDInput[0x20]);
			Touch2.Y = (Touchpad2Raw & 0xFFF00000) >> 20;
			Touch2.X = (Touchpad2Raw & 0x000FFF00) >> 8;
			Touch2.Down = (Touchpad2Raw & (1 << 7)) == 0;
			Touch2.Id = (Touchpad2Raw & 127);

		    if (Touch.Down) // pressed
		    {
		        InMessageHandler->OnTouchStarted(
		            nullptr,
		            FVector2D(Touch.X, Touch.Y),
		            1.0f,
		            Touch.Id,
		            UserId,
		            InputDeviceId
		        );
		    }
		    else
		    {
		    	// OnTouchEnded
		        InMessageHandler->OnTouchEnded(
		            FVector2D(Touch.X, Touch.Y),
		            Touch.Id,
		            UserId,
		            InputDeviceId
		        );
		    }

		    if (Touch2.Down) // pressed
		    {
		        InMessageHandler->OnTouchStarted(
		            nullptr,
		            FVector2D(Touch2.X, Touch2.Y),
		            1.0f,
		            Touch2.Id,
		            UserId,
		            InputDeviceId
		        );
		    }
		    else
		    {
		    	// OnTouchEnded
		        InMessageHandler->OnTouchEnded(
		            FVector2D(Touch2.X, Touch2.Y),
		            Touch2.Id,
		            UserId,
		            InputDeviceId
		        );
		    }
		}
		
		
		if (EnableAccelerometer || EnableGyroscope)
		{
			// &HIDInput[0x1B], &HIDInput[0x1D], &HIDInput[0x1F]
			
			FGyro Gyro;
			Gyro.X = static_cast<int16_t>((HIDInput[15]) | (HIDInput[16] << 8));
			Gyro.Y = static_cast<int16_t>((HIDInput[17]) | (HIDInput[18] << 8));
			Gyro.Z = static_cast<int16_t>((HIDInput[19]) | (HIDInput[20] << 8));

			FAccelerometer Acc;
			Acc.X = static_cast<int16_t>((HIDInput[21]) | (HIDInput[22] << 8));
			Acc.Y = static_cast<int16_t>((HIDInput[23]) | (HIDInput[24] << 8));
			Acc.Z = static_cast<int16_t>((HIDInput[25]) | (HIDInput[26] << 8));

			constexpr float RealGravityValue = 9.81f;
			const float GravityMagnitude = FMath::Sqrt(FMath::Square(static_cast<float>(Acc.X)) + FMath::Square(static_cast<float>(Acc.Y)) + FMath::Square(static_cast<float>(Acc.Z)));

			const FVector Tilts = FVector(Acc.X +  Gyro.X, Acc.Y + Gyro.Y, Acc.Z +  Gyro.Z);
			const FVector Gravity = FVector(static_cast<float>(Acc.X) / GravityMagnitude, static_cast<float>(Acc.Y) / GravityMagnitude, static_cast<float>(Acc.Z) / GravityMagnitude) * RealGravityValue;
			const FVector Gyroscope = FVector(Gyro.X, Gyro.Y, Gyro.Z);
			const FVector Accelerometer = FVector(Acc.X, Acc.Y, Acc.Z);

			InMessageHandler.Get().OnMotionDetected( Tilts, Gyroscope, Gravity, Accelerometer, UserId, InputDeviceId);
		}

		// PrintBufferAsHex(HIDDeviceContexts.Internal.Buffer, 64);
		return true;
	}
	
	return false;
}

void UDualSenseLibrary::PrintBufferAsHex(const unsigned char* Buffer, int BufferSize)
{
	FString HexString;
	for (int i = 0; i < BufferSize; i++)
	{
		// Adiciona o valor hexadecimal de cada byte ao HexString
		HexString += FString::Printf(TEXT("%02X "), Buffer[i]);
	}

	// Imprime no log usando UE_LOG
	UE_LOG(LogTemp, Log, TEXT("Buffer as Hex String: %s"), *HexString);
}


void UDualSenseLibrary::UpdateColorOutput(const FColor Color)
{
	HidOutput.ColorHid = {Color.R, Color.G, Color.B, Color.A};
	SendOut();
}

bool IsResetVibration = false;
void UDualSenseLibrary::SetVibration(const FForceFeedbackValues& Vibration)
{
	const float LeftRumble = CalculateLeftRumble(Vibration);
	const float RightRumble = CalculateRightRumble(Vibration);

	if (IsResetVibration && (RightRumble <= 0 || LeftRumble <= 0))
	{
		HidOutput.MotorsHid = { 0x00, 0x00};
	}
	
	if (RightRumble > 0 || LeftRumble > 0)
	{
		IsResetVibration = true;
		HidOutput.MotorsHid.Left = static_cast<unsigned char>(LeftRumble);
		HidOutput.MotorsHid.Right = static_cast<unsigned char>(RightRumble);
	}

	SendOut();
}

unsigned char UDualSenseLibrary::CalculateLeftRumble(const FForceFeedbackValues& Values)
{
	const float CombinedLeft = Values.LeftLarge * 0.8f + Values.LeftSmall * 0.2f;
	return static_cast<unsigned char>(FMath::Clamp(CombinedLeft * 255.0f, 0.0f, 255.0f));
}

unsigned char UDualSenseLibrary::CalculateRightRumble(const FForceFeedbackValues& Values)
{
	const float CombinedRight = Values.RightLarge * 0.8f + Values.RightSmall * 0.2f;
	return static_cast<unsigned char>(FMath::Clamp(CombinedRight * 255.0f, 0.0f, 255.0f));
}

void UDualSenseLibrary::SetHapticFeedbackValues(int32 Hand, const FHapticFeedbackValues* Values)
{
	// // Config (L2)
	if (Hand == static_cast<int32>(EControllerHand::Left) || Hand == static_cast<int32>(EControllerHand::AnyHand))
	{
		HidOutput.LeftTrigger.Frequency = ConvertTo255(Values->Frequency);
	}
	
	// Config (R2)
	if (Hand == static_cast<int32>(EControllerHand::Right) || Hand == static_cast<int32>(EControllerHand::AnyHand))
	{
		HidOutput.RightTrigger.Frequency = ConvertTo255(Values->Frequency);
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
			HidOutput.LeftTrigger.Mode = 0x02;
			HidOutput.LeftTrigger.StartPosition = ConvertTo255(Resistance->StartPosition, 8);
			HidOutput.LeftTrigger.EndPosition = ConvertTo255(Resistance->EndPosition, 8);
			HidOutput.LeftTrigger.Strengths.Start = ConvertTo255(Resistance->StartStrengh, 9);
			HidOutput.LeftTrigger.Strengths.End = ConvertTo255(Resistance->EndStrengh, 9);
		}
	
		if (
			Resistance->AffectedTriggers == EInputDeviceTriggerMask::Right ||
			Resistance->AffectedTriggers == EInputDeviceTriggerMask::All
		)
		{
			HidOutput.RightTrigger.Mode = 0x02;
			HidOutput.RightTrigger.StartPosition = ConvertTo255(Resistance->StartPosition, 8);
			HidOutput.RightTrigger.EndPosition = ConvertTo255(Resistance->EndPosition, 8);
			HidOutput.RightTrigger.Strengths.Start = ConvertTo255(Resistance->StartStrengh, 9);
			HidOutput.RightTrigger.Strengths.End = ConvertTo255(Resistance->EndStrengh, 9);
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

void UDualSenseLibrary::SetTouch1(bool bIsTouch)
{
	EnableTouch1 = bIsTouch;
}

void UDualSenseLibrary::SetTouch2(bool bIsTouch)
{
	EnableTouch2 = bIsTouch;
}

void UDualSenseLibrary::ConfigTriggerHapticFeedbackEffect(
	int32 StartPosition, int32 BeginForce, int32 MiddleForce, int32 EndForce,
	const EControllerHand& Hand, bool KeepEffect
)
{
	unsigned char PositionalAmplitudes[10];
	PositionalAmplitudes[0] = BeginForce;
	PositionalAmplitudes[1] = BeginForce;
	PositionalAmplitudes[2] = BeginForce;
	PositionalAmplitudes[3] = BeginForce;
	PositionalAmplitudes[4] = MiddleForce;
	PositionalAmplitudes[5] = MiddleForce;
	PositionalAmplitudes[6] = MiddleForce;
	PositionalAmplitudes[7] = MiddleForce;
	PositionalAmplitudes[8] = KeepEffect ? 8 : EndForce;
	PositionalAmplitudes[9] = KeepEffect ? 8 : EndForce;

	unsigned char Strengths[10];
	for (int i = 0; i < 10; i++)
	{
		Strengths[i] = static_cast<uint64_t>(PositionalAmplitudes[i] * 8.0f);
	}

	
	int64 StrengthZones = 0;
	int32 ActiveZones   = 0;
	for (int i = 0; i < 10; i++)
	{
		if (PositionalAmplitudes[i] > 0)
		{
			const uint64_t StrengthValue = static_cast<uint64_t>((Strengths[i] - 1) & 0x07);
			StrengthZones |= static_cast<int64>(StrengthValue << (3 * i));
			ActiveZones   |= (1 << i);
		}
	}
	
	if (Hand == EControllerHand::Left || Hand == EControllerHand::AnyHand)
	{
		HidOutput.LeftTrigger.Mode = 0x26;
		HidOutput.LeftTrigger.Strengths.ActiveZones = ActiveZones;
		HidOutput.LeftTrigger.Strengths.StrengthZones = StrengthZones;
		HidOutput.LeftTrigger.Frequency = ConvertTo255(0.2f);
	}
	
	if (Hand == EControllerHand::Right || Hand == EControllerHand::AnyHand)
	{
		HidOutput.RightTrigger.Mode = 0x26;
		HidOutput.RightTrigger.Strengths.ActiveZones = ActiveZones;
		HidOutput.RightTrigger.Strengths.StrengthZones = StrengthZones;
		HidOutput.RightTrigger.Frequency = ConvertTo255(0.2f);
	}
	
	SendOut();
}

void UDualSenseLibrary::NoResitance(const EControllerHand& Hand)
{
	if (Hand == EControllerHand::Left || Hand == EControllerHand::AnyHand)
	{
		HidOutput.LeftTrigger.Mode = 0x05;
	}
	
	if (Hand == EControllerHand::Right || Hand == EControllerHand::AnyHand)
	{
		HidOutput.RightTrigger.Mode = 0x05;
	}
	
	SendOut();
}

void UDualSenseLibrary::ContinuousResitance(int32 StartPosition, int32 Force, const EControllerHand& Hand)
{
	if (Hand == EControllerHand::Left || Hand == EControllerHand::AnyHand)
	{
		HidOutput.LeftTrigger.Mode = 0x01;
		HidOutput.LeftTrigger.StartPosition = ConvertTo255(StartPosition, 8);
		HidOutput.LeftTrigger.Strengths.Start = ConvertTo255(Force, 9);
	}
	
	if (Hand == EControllerHand::Right || Hand == EControllerHand::AnyHand)
	{
		HidOutput.RightTrigger.Mode = 0x01;
		HidOutput.RightTrigger.StartPosition = ConvertTo255(StartPosition, 8);
		HidOutput.RightTrigger.Strengths.Start = ConvertTo255(Force, 9);
	}
	
	SendOut();
}

void UDualSenseLibrary::SectionResitance(int32 StartPosition, int32 EndPosition, int32 Force, const EControllerHand& Hand)
{
	
	if (Hand == EControllerHand::Left || Hand == EControllerHand::AnyHand)
	{
		HidOutput.LeftTrigger.Mode = 0x02;
		HidOutput.LeftTrigger.StartPosition = ConvertTo255(StartPosition, 8);
		HidOutput.LeftTrigger.EndPosition = ConvertTo255(EndPosition, 8);
		HidOutput.LeftTrigger.Strengths.Start = ConvertTo255(Force, 9);
	}
	
	if (Hand == EControllerHand::Right || Hand == EControllerHand::AnyHand)
	{
		HidOutput.RightTrigger.Mode = 0x02;
		HidOutput.RightTrigger.StartPosition = ConvertTo255(StartPosition, 8);
		HidOutput.RightTrigger.EndPosition = ConvertTo255(EndPosition, 8);
		HidOutput.RightTrigger.Strengths.Start = ConvertTo255(Force, 9);
	}
	
	SendOut();
}

void UDualSenseLibrary::Feedback(int32 StartPosition, int32 EndPosition, int32 Force, const EControllerHand& Hand)
{
	if (Hand == EControllerHand::Left || Hand == EControllerHand::AnyHand)
	{
		HidOutput.LeftTrigger.Mode = 0x21;
		HidOutput.LeftTrigger.StartPosition = ConvertTo255(StartPosition, 8);
		HidOutput.LeftTrigger.EndPosition = ConvertTo255(EndPosition, 8);
		HidOutput.LeftTrigger.Strengths.Start = ConvertTo255(Force, 9) / 3;
		HidOutput.LeftTrigger.Strengths.Middle = ConvertTo255(Force, 9) / 2;
		HidOutput.LeftTrigger.Strengths.End = ConvertTo255(Force, 9);
	}

	if (Hand == EControllerHand::Right || Hand == EControllerHand::AnyHand)
	{
		HidOutput.RightTrigger.Mode = 0x21;
		HidOutput.RightTrigger.StartPosition = ConvertTo255(StartPosition, 8);
		HidOutput.RightTrigger.EndPosition = ConvertTo255(EndPosition, 8);
		HidOutput.RightTrigger.Strengths.Start = ConvertTo255(Force, 9) / 3;
		HidOutput.RightTrigger.Strengths.Middle = ConvertTo255(Force, 9) / 2;
		HidOutput.RightTrigger.Strengths.End = ConvertTo255(Force, 9);
	}

	SendOut();
}

void UDualSenseLibrary::SetWeaponEffects(int32 StartPosition, int32 EndPosition, int32 Force,
                                         const EControllerHand& Hand)
{
	if (Hand == EControllerHand::Left || Hand == EControllerHand::AnyHand)
	{
		HidOutput.LeftTrigger.Mode = 0x25;
		HidOutput.LeftTrigger.StartPosition = ConvertTo255(StartPosition);
		HidOutput.LeftTrigger.EndPosition = ConvertTo255(EndPosition);
		HidOutput.LeftTrigger.Strengths.Start = ConvertTo255(Force);
	}
	
	if (Hand == EControllerHand::Right || Hand == EControllerHand::AnyHand)
	{
		HidOutput.RightTrigger.Mode = 0x25;
		HidOutput.RightTrigger.StartPosition = ConvertTo255(StartPosition);
		HidOutput.RightTrigger.EndPosition = ConvertTo255(EndPosition);
		HidOutput.RightTrigger.Strengths.Start = ConvertTo255(Force);
	}

	SendOut();
}

void UDualSenseLibrary::SetGallopingEffects(int32 StartPosition, int32 EndPosition, int32 FirstFoot, int32 SecondFoot, float Frequency, const EControllerHand& Hand)
{

	const uint32_t ActiveZones = ((1 << StartPosition) | (1 << EndPosition));
	const uint32_t TimeAndRatio = (((SecondFoot & 0x07) << (3 * 0)) | ((FirstFoot  & 0x07) << (3 * 1)));
	if (Hand == EControllerHand::Left || Hand == EControllerHand::AnyHand)
	{
		HidOutput.LeftTrigger.Mode = 0x23;
		HidOutput.LeftTrigger.Strengths.ActiveZones = ActiveZones;
		HidOutput.LeftTrigger.Strengths.TimeAndRatio = TimeAndRatio;
		HidOutput.LeftTrigger.Frequency = ConvertTo255(Frequency);
	}
	
	if (Hand == EControllerHand::Right || Hand == EControllerHand::AnyHand)
	{
		HidOutput.RightTrigger.Mode = 0x23;
		HidOutput.RightTrigger.Strengths.ActiveZones = ActiveZones;
		HidOutput.RightTrigger.Strengths.TimeAndRatio = TimeAndRatio;
		HidOutput.RightTrigger.Frequency = ConvertTo255(Frequency);
	}

	SendOut();
}

void UDualSenseLibrary::SetMachineEffects(int32 StartPosition, int32 EndPosition, int32 AmplitudeBegin, int32 AmplitudeEnd, float Frequency, float Period, const EControllerHand& Hand)
{

	const uint32_t ActiveZones = ((1 << StartPosition) | (1 << EndPosition));
	const uint32_t Forces = (((AmplitudeBegin & 0x07) << (3 * 0)) | ((AmplitudeEnd & 0x07) << (3 * 1)));

	if (Period < 0.0f || Period > 3.f) {
		Period = 3.f;
	}

	if (Hand == EControllerHand::Left || Hand == EControllerHand::AnyHand)
	{
		HidOutput.LeftTrigger.Mode = 0x27;
		HidOutput.LeftTrigger.Strengths.ActiveZones = ActiveZones;
		HidOutput.LeftTrigger.Strengths.StrengthZones = Forces;
		HidOutput.LeftTrigger.Strengths.Period = ConvertTo255(Period);
		HidOutput.LeftTrigger.Frequency = ConvertTo255(Frequency);
	}
	
	if (Hand == EControllerHand::Right || Hand == EControllerHand::AnyHand)
	{
		HidOutput.RightTrigger.Mode = 0x27;
		HidOutput.RightTrigger.Strengths.ActiveZones = ActiveZones;
		HidOutput.RightTrigger.Strengths.StrengthZones = Forces;
		HidOutput.RightTrigger.Strengths.Period = ConvertTo255(Period);
		HidOutput.RightTrigger.Frequency = ConvertTo255(Frequency);
	}

	SendOut();
}

void UDualSenseLibrary::SetBowEffects(int32 StartPosition, int32 EndPosition, int32 BegingForce, int32 EndForce,
	const EControllerHand& Hand)
{
	const uint32_t ActiveZones = ((1 << StartPosition) | (1 << EndPosition));
	const uint32_t Forces = ((((BegingForce  - 1) & 0x07) << (3 * 0)) | (((EndForce - 1) & 0x07) << (3 * 1)));
	if (Hand == EControllerHand::Left || Hand == EControllerHand::AnyHand)
	{
		HidOutput.LeftTrigger.Mode = 0x22;
		HidOutput.LeftTrigger.Strengths.ActiveZones = ActiveZones;
		HidOutput.LeftTrigger.Strengths.StrengthZones = Forces;
	}
	
	if (Hand == EControllerHand::Right || Hand == EControllerHand::AnyHand)
	{
		HidOutput.RightTrigger.Mode = 0x22;
		HidOutput.RightTrigger.Strengths.ActiveZones = ActiveZones;
		HidOutput.RightTrigger.Strengths.StrengthZones = Forces;
	}

	SendOut();
}


void UDualSenseLibrary::StopEffect(const EControllerHand& Hand)
{
	if (Hand == EControllerHand::Left || Hand == EControllerHand::AnyHand)
	{
		HidOutput.ResetEffectsLeftTrigger = true;
	}
	
	if (Hand == EControllerHand::Right || Hand == EControllerHand::AnyHand)
	{
		HidOutput.ResetEffectsRightTrigger = true;
	}
	
	SendOut();
}

void UDualSenseLibrary::StopAllEffects()
{
	HidOutput.ColorHid = {0, 0, 255, 255};
	HidOutput.LedPlayerHid.Brightness = 0x00;
	HidOutput.LedPlayerHid.Led = PLAYER_LED_MIDDLE;
	HidOutput.LedPlayerHid.Player = 0x02;
	HidOutput.LeftTrigger.Mode = 0x05;
	HidOutput.RightTrigger.Mode = 0x05;
	SendOut();
}

void UDualSenseLibrary::StopAll()
{
	HidOutput.LedPlayerHid.Brightness = 0x00;
	if (ControllerID == 0)
	{
		HidOutput.ColorHid = {0, 0, 255, 255};
		HidOutput.LedPlayerHid.Led = PLAYER_LED_MIDDLE;
	}

	if (ControllerID == 1)
	{
		HidOutput.ColorHid = {255, 0, 0, 255};
		HidOutput.LedPlayerHid.Led = PLAYER_LED_MIDDLE_RIGHT | PLAYER_LED_MIDDLE_LEFT;
	}

	if (ControllerID == 2)
	{
		HidOutput.ColorHid = {0, 255, 0, 255};
		HidOutput.LedPlayerHid.Led = PLAYER_LED_LEFT | PLAYER_LED_MIDDLE | PLAYER_LED_RIGHT;
	}

	if (ControllerID == 3)
	{
		HidOutput.ColorHid = {255, 255, 255, 255};
		HidOutput.LedPlayerHid.Led = PLAYER_LED_LEFT | PLAYER_LED_RIGHT | PLAYER_LED_MIDDLE_LEFT | PLAYER_LED_MIDDLE_RIGHT;	
	}
	
	HidOutput.LedPlayerHid.Player = 0x02;
	HidOutput.LeftTrigger.Mode = 0x05;
	HidOutput.RightTrigger.Mode = 0x05;
	SendOut();
}

void UDualSenseLibrary::SetLedPlayerEffects(int32 NumberLeds, int32 BrightnessValue)
{
	if (NumberLeds == 0)
	{
		HidOutput.LedPlayerHid.Player = 0x00;
		HidOutput.LedPlayerHid.Led = 0x00;
		HidOutput.LedPlayerHid.Brightness = 0x00;
		return;
	}
	
	HidOutput.LedPlayerHid.Player = 0x02;
	HidOutput.LedPlayerHid.Led = PLAYER_LED_MIDDLE;
	if (NumberLeds == 2)
	{
		HidOutput.LedPlayerHid.Led = PLAYER_LED_MIDDLE;
		HidOutput.LedPlayerHid.Led |= PLAYER_LED_MIDDLE_LEFT | PLAYER_LED_MIDDLE_RIGHT;
	}
	
	if (NumberLeds == 3)
	{
		HidOutput.LedPlayerHid.Led = PLAYER_LED_MIDDLE;
		HidOutput.LedPlayerHid.Led |= PLAYER_LED_MIDDLE_LEFT | PLAYER_LED_MIDDLE_RIGHT;
	}
	
	if (NumberLeds == 4)
	{
		HidOutput.LedPlayerHid.Led = PLAYER_LED_MIDDLE;
		HidOutput.LedPlayerHid.Led |= PLAYER_LED_RIGHT | PLAYER_LED_LEFT;
	}
	
	switch (BrightnessValue)
	{
		case 0:
			HidOutput.LedPlayerHid.Brightness = 0x02;
			break;
		case 1:
			HidOutput.LedPlayerHid.Brightness = 0x01;
			break;
		case 2:
		default:
			HidOutput.LedPlayerHid.Brightness = 0x00;
			break;
	}
	SendOut();
}

void UDualSenseLibrary::SetLedMicEffects(int32 LedMic)
{
	HidOutput.MicLed.Mode = 0x00;
	if (LedMic == 1)
	{
		HidOutput.MicLed.Mode = 0x01;
	}
	
	if (LedMic == 2)
	{
		HidOutput.MicLed.Mode = 0x02;
	}
	
	SendOut();
}

float UDualSenseLibrary::GetLevelBattery()
{
	if (unsigned char* HIDInput = &HIDDeviceContexts.Internal.Buffer[2]; DualSenseHIDManager::GetDeviceInputState(&HIDDeviceContexts, HIDInput))
	{
		return ((HIDInput[0x34] & 0x0F) * 100) / 8;
	}
	
	return 0.00f;
}

unsigned char UDualSenseLibrary::ConvertTo255(unsigned char Value, unsigned char MaxInput)
{
	if (Value <= 0) return 0;
	if (Value >= MaxInput) return 255;

	return static_cast<unsigned char>((Value * 255) / MaxInput);
}


int UDualSenseLibrary::ConvertTo255(const float Value)
{
	const float Min = 0;
	const float Max = 1.0;
	const float NormalizedPosition = (Value - Min) / (Max - Min);
	const int Value255 = static_cast<int>(NormalizedPosition * 255);
	return std::clamp(Value255, 0, 255);
}
