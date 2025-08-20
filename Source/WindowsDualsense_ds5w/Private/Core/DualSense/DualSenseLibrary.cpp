// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "Core/DualSense/DualSenseLibrary.h"

#include <Windows.h>
#include "Core/DeviceHIDManager.h"
#include "InputCoreTypes.h"
#include "Core/Structs/FOutputContext.h"
#include "Helpers/ValidateHelpers.h"


bool UDualSenseLibrary::InitializeLibrary(const FDeviceContext& Context)
{
	HIDDeviceContexts = Context;
	StopAll();
	UE_LOG(LogTemp, Log, TEXT("Initializing device model (%s)"), Context.DeviceType == DualSenseEdge ? TEXT("DualSense Edge") : TEXT("DualSense Default"));
	return true;
}

void UDualSenseLibrary::ShutdownLibrary()
{
	ButtonStates.Reset();
	CloseHandle(HIDDeviceContexts.Handle);
	UDeviceHIDManager::FreeContext(&HIDDeviceContexts);
	UE_LOG(LogTemp, Log, TEXT("UDualSenseLibrary ShutdownLibrary()"));
}

void UDualSenseLibrary::Reconnect()
{
	PlatformInputDeviceMapper.Get().GetOnInputDeviceConnectionChange().Broadcast(
		EInputDeviceConnectionState::Connected, FPlatformUserId::CreateFromInternalId(ControllerID),
		FInputDeviceId::CreateFromInternalId(ControllerID));
}

bool UDualSenseLibrary::IsConnected()
{
	return HIDDeviceContexts.IsConnected;
}

void UDualSenseLibrary::SendOut()
{
	if (!HIDDeviceContexts.IsConnected)
	{
		return;
	}
	
	UDeviceHIDManager::OutputDualSense(&HIDDeviceContexts);
}
void UDualSenseLibrary::Settings(const FSettings<FFeatureReport>& Settings)
{
}

void UDualSenseLibrary::Settings(const FDualSenseFeatureReport& Settings)
{
	FOutputContext* HidOutput = &HIDDeviceContexts.Output;
	HidOutput->Feature.VibrationMode = Settings.VibrationMode == EDualSenseDeviceFeatureReport::Off ? 0xFF : static_cast<uint8_t>(Settings.VibrationMode);
	HidOutput->Feature.SoftRumbleReduce = static_cast<uint8_t>(Settings.SoftRumbleReduce);
	HidOutput->Feature.TriggerSoftnessLevel = static_cast<uint8_t>(Settings.TriggerSoftnessLevel);

	HidOutput->Audio.MicStatus = static_cast<uint8_t>(Settings.MicStatus);
	HidOutput->Audio.MicVolume = static_cast<uint8_t>(Settings.MicVolume);
	HidOutput->Audio.HeadsetVolume = static_cast<uint8_t>(Settings.AudioVolume);
	HidOutput->Audio.SpeakerVolume = static_cast<uint8_t>(Settings.AudioVolume);

	if (Settings.AudioHeadset == EDualSenseAudioFeatureReport::On && Settings.AudioSpeaker == EDualSenseAudioFeatureReport::Off)
	{
		HidOutput->Audio.Mode = 0x05;
	}
	
	if (Settings.AudioHeadset == EDualSenseAudioFeatureReport::On && Settings.AudioSpeaker == EDualSenseAudioFeatureReport::On)
	{
		HidOutput->Audio.Mode = 0x21;
	}
	
	if (Settings.AudioHeadset == EDualSenseAudioFeatureReport::Off && Settings.AudioSpeaker == EDualSenseAudioFeatureReport::On)
	{
		HidOutput->Audio.Mode = 0x31;
	}
	
	SendOut();
}

void UDualSenseLibrary::CheckButtonInput(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
                                         const FPlatformUserId UserId, const FInputDeviceId InputDeviceId,
                                         const FName ButtonName, const bool IsButtonPressed)
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

bool UDualSenseLibrary::UpdateInput(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
                                    const FPlatformUserId UserId, const FInputDeviceId InputDeviceId)
{
	const size_t Padding = HIDDeviceContexts.ConnectionType == Bluetooth ? 2 : 1;
	if (UDeviceHIDManager::GetDeviceInputState(&HIDDeviceContexts))
	{
		const unsigned char* HIDInput = &HIDDeviceContexts.Buffer[Padding];
		
		// Analogs
		const float LeftAnalogX = static_cast<char>(static_cast<short>(HIDInput[0x00] - 128));
		const float LeftAnalogY = static_cast<char>(static_cast<short>(HIDInput[0x01] - 127) * -1);
		InMessageHandler.Get().OnControllerAnalog(FGamepadKeyNames::LeftAnalogX, UserId, InputDeviceId, LeftAnalogX / 128.0f);
		InMessageHandler.Get().OnControllerAnalog(FGamepadKeyNames::LeftAnalogY, UserId, InputDeviceId, LeftAnalogY / 128.0f);

		const float RightAnalogX = static_cast<char>(static_cast<short>(HIDInput[0x02] - 128));
		const float RightAnalogY = static_cast<char>(static_cast<short>(HIDInput[0x03] - 127) * -1);
		InMessageHandler.Get().OnControllerAnalog(FGamepadKeyNames::RightAnalogX, UserId, InputDeviceId, RightAnalogX / 128.0f);
		InMessageHandler.Get().OnControllerAnalog(FGamepadKeyNames::RightAnalogY, UserId, InputDeviceId, RightAnalogY / 128.0f);
		
		const float TriggerL = HIDInput[0x04] / 256.0f;
		const float TriggerR = HIDInput[0x05] / 256.0f;
		InMessageHandler.Get().OnControllerAnalog(FGamepadKeyNames::LeftTriggerAnalog, UserId, InputDeviceId, TriggerL);
		InMessageHandler.Get().OnControllerAnalog(FGamepadKeyNames::RightTriggerAnalog, UserId, InputDeviceId, TriggerR);

		uint8_t ButtonsMask = HIDInput[0x07] & 0xF0;
		const bool bCross = ButtonsMask & BTN_CROSS;
		const bool bSquare = ButtonsMask & BTN_SQUARE;
		const bool bCircle = ButtonsMask & BTN_CIRCLE;
		const bool bTriangle = ButtonsMask & BTN_TRIANGLE;

		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::FaceButtonBottom, bCross);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::FaceButtonLeft, bSquare);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::FaceButtonRight, bCircle);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::FaceButtonTop, bTriangle);

		switch (HIDInput[0x07] & 0x0F)
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
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_PaddleL"), bPaddleLeft);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_PaddleR"), bPaddleRight);

		const bool Start = HIDInput[0x08] & BTN_START;
		const bool Select = HIDInput[0x08] & BTN_SELECT;
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_Menu"), Start);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_Share"), Select);

		// mapped urenal native gamepad Start and Select
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::SpecialRight, Start);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::SpecialLeft, Select);

		
		const bool bLeftTriggerThreshold = HIDInput[0x08] & BTN_LEFT_TRIGGER;
		const bool bRightTriggerThreshold = HIDInput[0x08] & BTN_RIGHT_TRIGGER;
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::LeftTriggerThreshold,
						 bLeftTriggerThreshold);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::RightTriggerThreshold,
						 bRightTriggerThreshold);
		if (EnableTouch)
		{
			FTouchPoint1 Touch;
			const UINT32 Touchpad1Raw = *reinterpret_cast<const UINT32*>(&HIDInput[0x20]);
			Touch.Y = (Touchpad1Raw & 0xFFF00000) >> 20;
			Touch.X = (Touchpad1Raw & 0x000FFF00) >> 8;
			Touch.Down = (Touchpad1Raw & (1 << 7)) == 0;
			Touch.Id = (Touchpad1Raw & 127);

			// // Evaluate touch state 2
			FTouchPoint2 Touch2;
			const UINT32 Touchpad2Raw = *reinterpret_cast<const UINT32*>(&HIDInput[0x20]);
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

		if (EnableAccelerometerAndGyroscope)
		{
			FGyro Gyro;
			Gyro.X = static_cast<int16_t>((HIDInput[16]) | (HIDInput[17] << 8));
			Gyro.Y = static_cast<int16_t>((HIDInput[18]) | (HIDInput[19] << 8));
			Gyro.Z = static_cast<int16_t>((HIDInput[20]) | (HIDInput[21] << 8));

			FAccelerometer Acc;
			Acc.X = static_cast<int16_t>((HIDInput[22]) | (HIDInput[23] << 8));
			Acc.Y = static_cast<int16_t>((HIDInput[24]) | (HIDInput[25] << 8));
			Acc.Z = static_cast<int16_t>((HIDInput[25]) | (HIDInput[27] << 8));

			constexpr float RealGravityValue = 9.81f;
			const float GravityMagnitude = FMath::Sqrt(
				FMath::Square(static_cast<float>(Acc.X)) + FMath::Square(static_cast<float>(Acc.Y)) + FMath::Square(
					static_cast<float>(Acc.Z)));

			const FVector Tilts = FVector(Acc.X + Gyro.X, Acc.Y + Gyro.Y, Acc.Z + Gyro.Z);
			const FVector Gravity = FVector(static_cast<float>(Acc.X) / GravityMagnitude,
			                                static_cast<float>(Acc.Y) / GravityMagnitude,
			                                static_cast<float>(Acc.Z) / GravityMagnitude) * RealGravityValue;
			const FVector Gyroscope = FVector(Gyro.X, Gyro.Y, Gyro.Z);
			const FVector Accelerometer = FVector(Acc.X, Acc.Y, Acc.Z);

			InMessageHandler.Get().OnMotionDetected(Tilts, Gyroscope, Gravity, Accelerometer, UserId, InputDeviceId);
		}

		// Actions
		SetHasPhoneConnected(HIDInput[0x35] & 0x01);
		SetLevelBattery(((HIDInput[0x34] & 0x0F) * 100) / 8, (HIDInput[0x35] & 0x00), (HIDInput[0x36] & 0x20));
		
		return true;
	}

	return false;
}

void UDualSenseLibrary::SetVibration(const FForceFeedbackValues& Vibration)
{
	FOutputContext* HidOutput = &HIDDeviceContexts.Output;
	const float LeftRumble = FMath::Max(Vibration.LeftLarge, Vibration.LeftSmall);
	const float RightRumble = FMath::Max(Vibration.RightLarge, Vibration.RightSmall);

	const unsigned char OutputLeft = static_cast<unsigned char>(UValidateHelpers::To255(LeftRumble));
	const unsigned char OutputRight = static_cast<unsigned char>(UValidateHelpers::To255(RightRumble));
	if (HidOutput->Rumbles.Left != OutputLeft || HidOutput->Rumbles.Right != OutputRight)
	{
		HidOutput->Rumbles = {OutputLeft, OutputRight};
		SendOut();
	}
}

void UDualSenseLibrary::SetVibrationAudioBased(
	const FForceFeedbackValues& Vibration,
	const float Threshold = 0.015f,
	const float ExponentCurve = 2.f,
	const float BaseMultiplier = 1.5f
)
{
	FOutputContext* HidOutput = &HIDDeviceContexts.Output;
	const float InputLeft = FMath::Max(Vibration.LeftLarge, Vibration.LeftSmall);
	const float InputRight = FMath::Max(Vibration.RightLarge, Vibration.RightSmall);

	float IntensityLeftRumble = 0.0f;
	if (InputLeft >= Threshold)
	{
		IntensityLeftRumble = BaseMultiplier *
			FMath::Pow((InputLeft - Threshold) / (1.0f - Threshold), ExponentCurve);
	}

	float IntensityRightRumble = 0.0f;
	if (InputRight >= Threshold)
	{
		IntensityRightRumble = BaseMultiplier *
			FMath::Pow((InputRight - Threshold) / (1.0f - Threshold), ExponentCurve);
	}

	const unsigned char OutputLeft = static_cast<unsigned char>(UValidateHelpers::To255(IntensityLeftRumble));
	const unsigned char OutputRight = static_cast<unsigned char>(UValidateHelpers::To255(IntensityRightRumble));
	HidOutput->Rumbles = {OutputLeft, OutputRight};

	SendOut();
}

void UDualSenseLibrary::SetHapticFeedback(int32 Hand, const FHapticFeedbackValues* Values)
{
	FOutputContext* HidOutput = &HIDDeviceContexts.Output;
	if (Hand == static_cast<int32>(EControllerHand::Left) || Hand == static_cast<int32>(EControllerHand::AnyHand))
	{
		HidOutput->LeftTrigger.Frequency = UValidateHelpers::To255(Values->Frequency);
	}

	if (Hand == static_cast<int32>(EControllerHand::Right) || Hand == static_cast<int32>(EControllerHand::AnyHand))
	{
		HidOutput->RightTrigger.Frequency = UValidateHelpers::To255(Values->Frequency);
	}

	SendOut();
}

void UDualSenseLibrary::SetTriggers(const FInputDeviceProperty* Values)
{
	FOutputContext* HidOutput = &HIDDeviceContexts.Output;
	if (Values->Name == FName("InputDeviceTriggerResistance"))
	{
		const FInputDeviceTriggerResistanceProperty* Resistance = static_cast<const
			FInputDeviceTriggerResistanceProperty*>(Values);
		
		const uint8_t Start = Resistance->StartPosition;
		const uint8_t End   = Resistance->EndPosition;
		const float StartStr = static_cast<float>(Resistance->StartStrengh);
		const float EndStr   = static_cast<float>(Resistance->EndStrengh);

		constexpr int NumZones = 10;
		uint8_t Strengths[NumZones] = {0};
		if (End > Start)
		{
			for (int i = Start; i <= End && i < NumZones; ++i)
			{
				const float Alpha = (End == Start) ? 0.0f : (i - Start) / (End - Start);
				Strengths[i] = static_cast<uint8>(StartStr + Alpha * (EndStr - StartStr));
			}
		}


		int32 ActiveZones = 0;
		int64 StrengthZones = 0;
		for (int i = 0; i < 10; i++)
		{
			if (Strengths[i] > 0)
			{
				const uint64_t StrengthValue = static_cast<uint64_t>((Strengths[i] - 1) & 0x07);
				StrengthZones |= static_cast<int64>(StrengthValue << (3 * i));
				ActiveZones |= (1 << i);
			}
		}
		
		if (
			Resistance->AffectedTriggers == EInputDeviceTriggerMask::Left ||
			Resistance->AffectedTriggers == EInputDeviceTriggerMask::All
		)
		{
			HidOutput->LeftTrigger.Mode = 0x02;
			HidOutput->LeftTrigger.Strengths.ActiveZones = ActiveZones;
			HidOutput->LeftTrigger.Strengths.StrengthZones = StrengthZones;
		}

		if (
			Resistance->AffectedTriggers == EInputDeviceTriggerMask::Right ||
			Resistance->AffectedTriggers == EInputDeviceTriggerMask::All
		)
		{
			HidOutput->RightTrigger.Mode = 0x02;
			HidOutput->RightTrigger.Strengths.ActiveZones = ActiveZones;
			HidOutput->RightTrigger.Strengths.StrengthZones = StrengthZones;
		}
	}

	SendOut();
}

void UDualSenseLibrary::SetAutomaticGun(int32 BeginStrength, int32 MiddleStrength, int32 EndStrength, const EControllerHand& Hand, bool KeepEffect)
{
	FOutputContext* HidOutput = &HIDDeviceContexts.Output;
	unsigned char PositionalAmplitudes[10];
	PositionalAmplitudes[0] = BeginStrength;
	PositionalAmplitudes[1] = BeginStrength;
	PositionalAmplitudes[2] = BeginStrength;
	PositionalAmplitudes[3] = BeginStrength;
	PositionalAmplitudes[4] = MiddleStrength;
	PositionalAmplitudes[5] = MiddleStrength;
	PositionalAmplitudes[6] = MiddleStrength;
	PositionalAmplitudes[7] = MiddleStrength;
	PositionalAmplitudes[8] = KeepEffect ? 8 : EndStrength;
	PositionalAmplitudes[9] = KeepEffect ? 8 : EndStrength;

	unsigned char Strengths[10];
	for (int i = 0; i < 10; i++)
	{
		Strengths[i] = static_cast<uint64_t>(PositionalAmplitudes[i] * 8.0f);
	}

	int64 StrengthZones = 0;
	int32 ActiveZones = 0;
	for (int i = 0; i < 10; i++)
	{
		if (PositionalAmplitudes[i] > 0)
		{
			const uint64_t StrengthValue = static_cast<uint64_t>((Strengths[i] - 1) & 0x07);
			StrengthZones |= static_cast<int64>(StrengthValue << (3 * i));
			ActiveZones |= (1 << i);
		}
	}

	if (Hand == EControllerHand::Left || Hand == EControllerHand::AnyHand)
	{
		HidOutput->LeftTrigger.Mode = 0x26;
		HidOutput->LeftTrigger.Strengths.ActiveZones = ActiveZones;
		HidOutput->LeftTrigger.Strengths.StrengthZones = StrengthZones;
		HidOutput->LeftTrigger.Frequency = UValidateHelpers::To255(0.05f);
	}

	if (Hand == EControllerHand::Right || Hand == EControllerHand::AnyHand)
	{
		HidOutput->RightTrigger.Mode = 0x26;
		HidOutput->RightTrigger.Strengths.ActiveZones = ActiveZones;
		HidOutput->RightTrigger.Strengths.StrengthZones = StrengthZones;
		HidOutput->RightTrigger.Frequency = UValidateHelpers::To255(0.05f);
	}

	SendOut();
}

void UDualSenseLibrary::SetContinuousResistance(int32 StartPosition, int32 Strength, const EControllerHand& Hand)
{
	FOutputContext* HidOutput = &HIDDeviceContexts.Output;
	if (Hand == EControllerHand::Left || Hand == EControllerHand::AnyHand)
	{
		HidOutput->LeftTrigger.Mode = 0x01;
		HidOutput->LeftTrigger.Strengths.ActiveZones = UValidateHelpers::To255(StartPosition, 8);
		HidOutput->LeftTrigger.Strengths.StrengthZones = UValidateHelpers::To255(Strength, 9);
	}

	if (Hand == EControllerHand::Right || Hand == EControllerHand::AnyHand)
	{
		HidOutput->RightTrigger.Mode = 0x01;
		HidOutput->RightTrigger.Strengths.ActiveZones = UValidateHelpers::To255(StartPosition, 8);
		HidOutput->RightTrigger.Strengths.StrengthZones = UValidateHelpers::To255(Strength, 9);
	}

	SendOut();
}

void UDualSenseLibrary::SetResistance(int32 BeginStrength, int32 MiddleStrength, int32 EndStrength, const EControllerHand& Hand)
{
	FOutputContext* HidOutput = &HIDDeviceContexts.Output;
	unsigned char PositionalAmplitudes[10];
	PositionalAmplitudes[0] = BeginStrength;
	PositionalAmplitudes[1] = BeginStrength;
	PositionalAmplitudes[2] = BeginStrength;
	PositionalAmplitudes[3] = BeginStrength;
	PositionalAmplitudes[4] = MiddleStrength;
	PositionalAmplitudes[5] = MiddleStrength;
	PositionalAmplitudes[6] = MiddleStrength;
	PositionalAmplitudes[7] = MiddleStrength;
	PositionalAmplitudes[8] = EndStrength;
	PositionalAmplitudes[9] = EndStrength;

	int32 ActiveZones = 0;
	int16 StrengthValues = 0;
	for (int i = 0; i < 3; i++)
	{
		if (PositionalAmplitudes[i] > 0)
		{
			const int8_t StrengthValue = static_cast<int8_t>((PositionalAmplitudes[i] - 1) & 0x07);
			StrengthValues |= (StrengthValue << (3 * i));
			ActiveZones |= static_cast<int16>(1 << i);
		}
	}

	if (Hand == EControllerHand::Left || Hand == EControllerHand::AnyHand)
	{
		HidOutput->LeftTrigger.Mode = 0x21;
		HidOutput->LeftTrigger.Strengths.ActiveZones = ActiveZones;
		HidOutput->LeftTrigger.Strengths.StrengthZones = StrengthValues;
	}

	if (Hand == EControllerHand::Right || Hand == EControllerHand::AnyHand)
	{
		HidOutput->RightTrigger.Mode = 0x21;
		HidOutput->RightTrigger.Strengths.ActiveZones = ActiveZones;
		HidOutput->RightTrigger.Strengths.StrengthZones = StrengthValues;
	}

	SendOut();
}

void UDualSenseLibrary::SetWeapon(int32 StartPosition, int32 EndPosition, int32 Strength,
                                         const EControllerHand& Hand)
{
	const uint32_t ActiveZones = (1 << StartPosition) | (1 << EndPosition);
	FOutputContext* HidOutput = &HIDDeviceContexts.Output;
	if (Hand == EControllerHand::Left || Hand == EControllerHand::AnyHand)
	{
		HidOutput->LeftTrigger.Mode = 0x25;
		HidOutput->LeftTrigger.Strengths.ActiveZones = ActiveZones;
		HidOutput->LeftTrigger.Strengths.StrengthZones = UValidateHelpers::To255(Strength);
	}

	if (Hand == EControllerHand::Right || Hand == EControllerHand::AnyHand)
	{
		HidOutput->RightTrigger.Mode = 0x25;
		HidOutput->RightTrigger.Strengths.ActiveZones = ActiveZones;
		HidOutput->RightTrigger.Strengths.StrengthZones = UValidateHelpers::To255(Strength);
	}

	SendOut();
}

void UDualSenseLibrary::SetGalloping(int32 StartPosition, int32 EndPosition, int32 FirstFoot, int32 SecondFoot,
                                            float Frequency, const EControllerHand& Hand)
{
	FOutputContext* HidOutput = &HIDDeviceContexts.Output;
	const uint32_t ActiveZones = (1 << StartPosition) | (1 << EndPosition);
	const uint32_t TimeAndRatio = (SecondFoot & 0x07) << (3 * 0) | (FirstFoot & 0x07);
	if (Hand == EControllerHand::Left || Hand == EControllerHand::AnyHand)
	{
		HidOutput->LeftTrigger.Mode = 0x23;
		HidOutput->LeftTrigger.Strengths.ActiveZones = ActiveZones;
		HidOutput->LeftTrigger.Strengths.TimeAndRatio = TimeAndRatio;
		HidOutput->LeftTrigger.Frequency = UValidateHelpers::To255(Frequency);
	}

	if (Hand == EControllerHand::Right || Hand == EControllerHand::AnyHand)
	{
		HidOutput->RightTrigger.Mode = 0x23;
		HidOutput->RightTrigger.Strengths.ActiveZones = ActiveZones;
		HidOutput->RightTrigger.Strengths.TimeAndRatio = TimeAndRatio;
		HidOutput->RightTrigger.Frequency = UValidateHelpers::To255(Frequency);
	}

	SendOut();
}

void UDualSenseLibrary::SetMachine(int32 StartPosition, int32 EndPosition, int32 AmplitudeBegin,
                                          int32 AmplitudeEnd, float Frequency, float Period,
                                          const EControllerHand& Hand)
{
	FOutputContext* HidOutput = &HIDDeviceContexts.Output;
	const uint32_t ActiveZones = ((1 << StartPosition) | (1 << EndPosition));
	const uint32_t Strengths = (((AmplitudeBegin & 0x07) << (3 * 0)) | ((AmplitudeEnd & 0x07) << (3 * 1)));

	if (Period < 0.0f || Period > 3.f)
	{
		Period = 3.f;
	}

	if (Hand == EControllerHand::Left || Hand == EControllerHand::AnyHand)
	{
		HidOutput->LeftTrigger.Mode = 0x27;
		HidOutput->LeftTrigger.Strengths.ActiveZones = ActiveZones;
		HidOutput->LeftTrigger.Strengths.StrengthZones = Strengths;
		HidOutput->LeftTrigger.Strengths.Period = UValidateHelpers::To255(Period);
		HidOutput->LeftTrigger.Frequency = UValidateHelpers::To255(Frequency);
	}

	if (Hand == EControllerHand::Right || Hand == EControllerHand::AnyHand)
	{
		HidOutput->RightTrigger.Mode = 0x27;
		HidOutput->RightTrigger.Strengths.ActiveZones = ActiveZones;
		HidOutput->RightTrigger.Strengths.StrengthZones = Strengths;
		HidOutput->RightTrigger.Strengths.Period = UValidateHelpers::To255(Period);
		HidOutput->RightTrigger.Frequency = UValidateHelpers::To255(Frequency);
	}

	SendOut();
}

void UDualSenseLibrary::SetBow(int32 StartPosition, int32 EndPosition, int32 BegingStrength, int32 EndStrength,
                                      const EControllerHand& Hand)
{
	FOutputContext* HidOutput = &HIDDeviceContexts.Output;
	const uint32_t ActiveZones = ((1 << StartPosition) | (1 << EndPosition));
	const uint32_t Strengths = ((((BegingStrength - 1) & 0x07) << (3 * 0)) | (((EndStrength - 1) & 0x07) << (3 * 1)));
	if (Hand == EControllerHand::Left || Hand == EControllerHand::AnyHand)
	{
		HidOutput->LeftTrigger.Mode = 0x22;
		HidOutput->LeftTrigger.Strengths.ActiveZones = ActiveZones;
		HidOutput->LeftTrigger.Strengths.StrengthZones = Strengths;
	}

	if (Hand == EControllerHand::Right || Hand == EControllerHand::AnyHand)
	{
		HidOutput->RightTrigger.Mode = 0x22;
		HidOutput->RightTrigger.Strengths.ActiveZones = ActiveZones;
		HidOutput->RightTrigger.Strengths.StrengthZones = Strengths;
	}

	SendOut();
}


void UDualSenseLibrary::StopTrigger(const EControllerHand& Hand)
{
	FOutputContext* HidOutput = &HIDDeviceContexts.Output;
	if (Hand == EControllerHand::Left || Hand == EControllerHand::AnyHand)
	{
		HidOutput->LeftTrigger.Mode = 0x0;
	}

	if (Hand == EControllerHand::Right || Hand == EControllerHand::AnyHand)
	{
		HidOutput->RightTrigger.Mode = 0x0;
	}

	SendOut();
}

void UDualSenseLibrary::StopAll()
{
	if (HIDDeviceContexts.ConnectionType == Bluetooth)
	{
		FOutputContext* HidOutput = &HIDDeviceContexts.Output;
		HidOutput->Feature.VibrationMode = 0xFF;
		HidOutput->Feature.FeatureMode = 0x1 | 0x2 | 0x4 | 0x8 | 0x10 | 0x40;
		SendOut();
	}

	FOutputContext* HidOutput = &HIDDeviceContexts.Output;
	HidOutput->Feature.VibrationMode = 0xFF;
	HidOutput->Feature.FeatureMode = 0xF7;
	HidOutput->PlayerLed.Brightness = 0x00;
	if (ControllerID == 0)
	{
		HidOutput->Lightbar = {0, 0, 255, 255};
		HidOutput->PlayerLed.Led = static_cast<unsigned char>(ELedPlayerEnum::One);
	}
	
	if (ControllerID == 1)
	{
		HidOutput->Lightbar = {255, 0, 0, 255};
		HidOutput->PlayerLed.Led = static_cast<unsigned char>(ELedPlayerEnum::Two);
	}
	
	if (ControllerID == 2)
	{
		HidOutput->Lightbar = {0, 255, 0, 255};
		HidOutput->PlayerLed.Led = static_cast<unsigned char>(ELedPlayerEnum::Three);
	}
	
	if (ControllerID == 3)
	{
		HidOutput->Lightbar = {255, 255, 255, 255};
		HidOutput->PlayerLed.Led = static_cast<unsigned char>(ELedPlayerEnum::All);
	}
	SendOut();
}

void UDualSenseLibrary::SetLightbar(FColor Color, float BrithnessTime, float ToggleTime)
{
	FOutputContext* HidOutput = &HIDDeviceContexts.Output;
	if ((HidOutput->Lightbar.R != Color.R) || (HidOutput->Lightbar.G != Color.G) || (HidOutput->Lightbar.B != Color.B))
	{
		HidOutput->Lightbar.R = Color.R;
		HidOutput->Lightbar.G = Color.G;
		HidOutput->Lightbar.B = Color.B;
		SendOut();
	}
}

void UDualSenseLibrary::SetPlayerLed(ELedPlayerEnum Led, ELedBrightnessEnum Brightness)
{
	FOutputContext* HidOutput = &HIDDeviceContexts.Output;
	if ((HidOutput->PlayerLed.Led != static_cast<unsigned char>(Led)) || (HidOutput->PlayerLed.Brightness != static_cast<unsigned char>(Brightness)))
	{
		HidOutput->PlayerLed.Led = static_cast<unsigned char>(Led);
		HidOutput->PlayerLed.Brightness = static_cast<unsigned char>(Brightness);
		SendOut();
	}
}

void UDualSenseLibrary::SetMicrophoneLed(ELedMicEnum Led)
{
	FOutputContext* HidOutput = &HIDDeviceContexts.Output;
	if (HidOutput->MicLight.Mode != static_cast<unsigned char>(Led))
	{
		HidOutput->MicLight.Mode = static_cast<unsigned char>(Led);
		SendOut();
	}
}

void UDualSenseLibrary::SetTouch(const bool bIsTouch)
{
	EnableTouch = bIsTouch;
}

void UDualSenseLibrary::SetGyroscope(bool bIsGyroscope)
{
	EnableAccelerometerAndGyroscope = bIsGyroscope;
}

void UDualSenseLibrary::SetAcceleration(bool bIsAccelerometer)
{
	EnableAccelerometerAndGyroscope = bIsAccelerometer;
}

void UDualSenseLibrary::SetHasPhoneConnected(const bool HasConnected)
{
	HasPhoneConnected = HasConnected;
}

void UDualSenseLibrary::SetLevelBattery(const float Level, bool FullyCharged, bool Charging)
{
	LevelBattery = Level;
}
