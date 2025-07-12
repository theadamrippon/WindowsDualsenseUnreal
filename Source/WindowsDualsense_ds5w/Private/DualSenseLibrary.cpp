// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "DualSenseLibrary.h"

#include <Windows.h>
#include "DualSenseProxy.h"
#include "InputCoreTypes.h"


//
bool UDualSenseLibrary::Reconnect() const
{
	PlatformInputDeviceMapper.Get().GetOnInputDeviceConnectionChange().Broadcast(
		EInputDeviceConnectionState::Connected, FPlatformUserId::CreateFromInternalId(ControllerID),
		FInputDeviceId::CreateFromInternalId(ControllerID));
	return true;
}

bool UDualSenseLibrary::InitializeLibrary(const FHIDDeviceContext& Context)
{
	EnableTouch = false;
	EnableAccelerometerAndGyroscope = false;

	UE_LOG(LogTemp, Log, TEXT("DualSense: Connection type (0=Usb, 1=Bluetooth, 2=Unknown) = %d"), Context.Internal.Connection);
	HIDDeviceContexts = Context;

	StopAll();
	return true;
}

void UDualSenseLibrary::ShutdownLibrary()
{
	ButtonStates.Reset();

	CloseHandle(HIDDeviceContexts.Internal.DeviceHandle);
	UDualSenseHIDManager::FreeContext(&HIDDeviceContexts);
	UE_LOG(LogTemp, Log, TEXT("DualSense: Disconnected with success... ShutdownLibrary"));
}

bool UDualSenseLibrary::IsConnected() const
{
	return HIDDeviceContexts.Internal.Connected;
}

void UDualSenseLibrary::SendOut()
{
	if (!HIDDeviceContexts.Internal.Connected || HIDDeviceContexts.Internal.Connection == EHIDDeviceConnection::Unknown)
	{
		return;
	}

	UDualSenseHIDManager::OutputBuffering(&HIDDeviceContexts, HidOutput);
}

int32 UDualSenseLibrary::GetTrirggersFeedback(const EControllerHand& HandTrigger)
{
	return 0;
}

void UDualSenseLibrary::SetTouch(const bool bIsTouch)
{
	EnableTouch = bIsTouch;
}

void UDualSenseLibrary::SetHasPhoneConnected(const bool bHasConnected)
{
	HasPhoneConnected = bHasConnected;
}

void UDualSenseLibrary::SetLevelBattery(const float Level, bool FullyCharged, bool Charging)
{
	LevelBattery = Level;
}

void UDualSenseLibrary::SetLeftTriggerFeedback(const float L2Feedback)
{
	LeftTriggerFeedback = L2Feedback;
}

void UDualSenseLibrary::SetRightTriggerFeedback(const float R2Feedback)
{
	RightTriggerFeedback = R2Feedback;
}

void UDualSenseLibrary::SetAcceleration(bool bIsAccelerometer)
{
	EnableAccelerometerAndGyroscope = bIsAccelerometer;
}

void UDualSenseLibrary::SetGyroscope(bool bIsGyroscope)
{
	EnableAccelerometerAndGyroscope = bIsGyroscope;
}

void UDualSenseLibrary::RegisterSettings(
	EDualSenseAudioFeatureReport MicStatus,
	EDualSenseAudioFeatureReport AudioHeadset,
	EDualSenseAudioFeatureReport AudioSpeaker,
	EDualSenseDeviceFeatureReport VibrationMode,
	int32 MicVolume,
	int32 AudioVolume,
	int32 SoftRumbleReduce,
	bool SoftRumble
)
{
	HidOutput.FeatureConfigHid.SoftRumble = SoftRumble;
	HidOutput.FeatureConfigHid.SoftRumbleReduce = SoftRumbleReduce;

	HidOutput.AudioConfigHid.MicVolume = MicVolume;
	HidOutput.AudioConfigHid.SpeakerVolume = AudioVolume;
	HidOutput.AudioConfigHid.HeadsetVolume = AudioVolume;

	HidOutput.FeatureConfigHid.VibrationMode =
		VibrationMode == EDualSenseDeviceFeatureReport::DefaultRumble ? 0xFF : 0xFC;
	HidOutput.AudioConfigHid.MicStatus = MicStatus == EDualSenseAudioFeatureReport::On ? 0x0 : 0x10;
	if (AudioHeadset == EDualSenseAudioFeatureReport::On && AudioSpeaker == EDualSenseAudioFeatureReport::On)
	{
		HidOutput.AudioConfigHid.Mode = 0x21;
	}

	if (AudioHeadset == EDualSenseAudioFeatureReport::On && AudioSpeaker == EDualSenseAudioFeatureReport::Off)
	{
		HidOutput.AudioConfigHid.Mode = 0x05;
	}

	if (AudioHeadset == EDualSenseAudioFeatureReport::Off && AudioSpeaker == EDualSenseAudioFeatureReport::On)
	{
		HidOutput.AudioConfigHid.Mode = 0x31;
	}
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
	const size_t Padding = HIDDeviceContexts.Internal.Connection == EHIDDeviceConnection::Bluetooth ? 2 : 1;
	if (
			UDualSenseHIDManager::GetDeviceInputState(&HIDDeviceContexts)
		)
	{
		const unsigned char* HIDInput = &HIDDeviceContexts.Internal.Buffer[Padding];
		const float LeftAnalogX = static_cast<char>(static_cast<short>(HIDInput[0x00] - 128));
		const float LeftAnalogY = static_cast<char>(static_cast<short>(HIDInput[0x01] - 127) * -1);
		InMessageHandler.Get().OnControllerAnalog(FGamepadKeyNames::LeftAnalogX, UserId, InputDeviceId,
		                                          LeftAnalogX / 128.0f);
		InMessageHandler.Get().OnControllerAnalog(FGamepadKeyNames::LeftAnalogY, UserId, InputDeviceId,
		                                          LeftAnalogY / 128.0f);

		const float RightAnalogX = static_cast<char>(static_cast<short>(HIDInput[0x02] - 128));
		const float RightAnalogY = static_cast<char>(static_cast<short>(HIDInput[0x03] - 127) * -1);
		InMessageHandler.Get().OnControllerAnalog(FGamepadKeyNames::RightAnalogX, UserId, InputDeviceId,
		                                          RightAnalogX / 128.0f);
		InMessageHandler.Get().OnControllerAnalog(FGamepadKeyNames::RightAnalogY, UserId, InputDeviceId,
		                                          RightAnalogY / 128.0f);

		// Triggers
		const float TriggerL = HIDInput[0x04] / 256.0f;
		const float TriggerR = HIDInput[0x05] / 256.0f;
		InMessageHandler.Get().OnControllerAnalog(FGamepadKeyNames::LeftTriggerAnalog, UserId, InputDeviceId, TriggerL);
		InMessageHandler.Get().
		                 OnControllerAnalog(FGamepadKeyNames::RightTriggerAnalog, UserId, InputDeviceId, TriggerR);

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

		// Shoulders
		const bool bLeftShoulder = HIDInput[0x08] & BTN_LEFT_SHOLDER;
		const bool bRightShoulder = HIDInput[0x08] & BTN_RIGHT_SHOLDER;

		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::LeftShoulder, bLeftShoulder);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::RightShoulder, bRightShoulder);

		// L2 Threshold and R2 Threshold
		const bool bLeftTriggerThreshold = HIDInput[0x09] & BTN_LEFT_TRIGGER;
		const bool bRightTriggerThreshold = HIDInput[0x09] & BTN_RIGHT_TRIGGER;
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::LeftTriggerThreshold,
		                 bLeftTriggerThreshold);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::RightTriggerThreshold,
		                 bRightTriggerThreshold);

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

		// Actions
		SetLeftTriggerFeedback(HIDInput[0x2A]);
		SetRightTriggerFeedback(HIDInput[0x29]);
		SetHasPhoneConnected(HIDInput[0x35] & 0x01);
		SetLevelBattery(((HIDInput[0x34] & 0x0F) * 100) / 8, (HIDInput[0x35] & 0x00), (HIDInput[0x36] & 0x20));

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

		// PrintBufferAsHex(HIDInput, 78);
		return true;
	}

	return false;
}

void UDualSenseLibrary::UpdateColorOutput(const FColor Color)
{
	HidOutput.ColorHid = {Color.R, Color.G, Color.B, Color.A};
	SendOut();
}

void UDualSenseLibrary::SetVibrationAudioBased(
	const FForceFeedbackValues& Vibration,
	const float Threshold = 0.015f,
	const float ExponentCurve = 2.f,
	const float BaseMultiplier = 1.5f
)
{
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

	const unsigned char OutputLeft = static_cast<unsigned char>(To255(IntensityLeftRumble));
	const unsigned char OutputRight = static_cast<unsigned char>(To255(IntensityRightRumble));
	HidOutput.MotorsHid = {OutputLeft, OutputRight};
	SendOut();
}

bool IsResetVibration = false;

void UDualSenseLibrary::SetVibration(const FForceFeedbackValues& Vibration)
{
	const float LeftRumble = FMath::Max(Vibration.LeftLarge, Vibration.LeftSmall);
	const float RightRumble = FMath::Max(Vibration.RightLarge, Vibration.RightSmall);

	if (IsResetVibration && (RightRumble <= 0 || LeftRumble <= 0))
	{
		HidOutput.MotorsHid = {0, 0};
		SendOut();

		IsResetVibration = true;
	}

	if (RightRumble > 0 || LeftRumble > 0)
	{
		IsResetVibration = true;
		HidOutput.MotorsHid.Left = static_cast<unsigned char>(To255(LeftRumble));
		HidOutput.MotorsHid.Right = static_cast<unsigned char>(To255(RightRumble));
		SendOut();
	}
}


void UDualSenseLibrary::SetHapticFeedbackValues(int32 Hand, const FHapticFeedbackValues* Values)
{
	// Config (L2)
	if (Hand == static_cast<int32>(EControllerHand::Left) || Hand == static_cast<int32>(EControllerHand::AnyHand))
	{
		HidOutput.LeftTrigger.Frequency = To255(Values->Frequency);
	}

	// Config (R2)
	if (Hand == static_cast<int32>(EControllerHand::Right) || Hand == static_cast<int32>(EControllerHand::AnyHand))
	{
		HidOutput.RightTrigger.Frequency = To255(Values->Frequency);
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
			HidOutput.LeftTrigger.StartPosition = To255(Resistance->StartPosition, 8);
			HidOutput.LeftTrigger.EndPosition = To255(Resistance->EndPosition, 8);
			HidOutput.LeftTrigger.Strengths.Start = To255(Resistance->StartStrengh, 9);
			HidOutput.LeftTrigger.Strengths.End = To255(Resistance->EndStrengh, 9);
		}

		if (
			Resistance->AffectedTriggers == EInputDeviceTriggerMask::Right ||
			Resistance->AffectedTriggers == EInputDeviceTriggerMask::All
		)
		{
			HidOutput.RightTrigger.Mode = 0x02;
			HidOutput.RightTrigger.StartPosition = To255(Resistance->StartPosition, 8);
			HidOutput.RightTrigger.EndPosition = To255(Resistance->EndPosition, 8);
			HidOutput.RightTrigger.Strengths.Start = To255(Resistance->StartStrengh, 9);
			HidOutput.RightTrigger.Strengths.End = To255(Resistance->EndStrengh, 9);
		}
	}

	SendOut();
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
		HidOutput.LeftTrigger.Mode = 0x26;
		HidOutput.LeftTrigger.Strengths.ActiveZones = ActiveZones;
		HidOutput.LeftTrigger.Strengths.StrengthZones = StrengthZones;
		HidOutput.LeftTrigger.Frequency = To255(0.2f);
	}

	if (Hand == EControllerHand::Right || Hand == EControllerHand::AnyHand)
	{
		HidOutput.RightTrigger.Mode = 0x26;
		HidOutput.RightTrigger.Strengths.ActiveZones = ActiveZones;
		HidOutput.RightTrigger.Strengths.StrengthZones = StrengthZones;
		HidOutput.RightTrigger.Frequency = To255(0.2f);
	}

	SendOut();
}

void UDualSenseLibrary::NoResistance(const EControllerHand& Hand)
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

void UDualSenseLibrary::ContinuousResistance(int32 StartPosition, int32 Force, const EControllerHand& Hand)
{
	if (Hand == EControllerHand::Left || Hand == EControllerHand::AnyHand)
	{
		HidOutput.LeftTrigger.Mode = 0x01;
		HidOutput.LeftTrigger.StartPosition = To255(StartPosition, 8);
		HidOutput.LeftTrigger.Strengths.Start = To255(Force, 9);
	}

	if (Hand == EControllerHand::Right || Hand == EControllerHand::AnyHand)
	{
		HidOutput.RightTrigger.Mode = 0x01;
		HidOutput.RightTrigger.StartPosition = To255(StartPosition, 8);
		HidOutput.RightTrigger.Strengths.Start = To255(Force, 9);
	}

	SendOut();
}

void UDualSenseLibrary::SectionResistance(int32 StartPosition, int32 EndPosition, int32 Force,
                                         const EControllerHand& Hand)
{
	if (Hand == EControllerHand::Left || Hand == EControllerHand::AnyHand)
	{
		HidOutput.LeftTrigger.Mode = 0x02;
		HidOutput.LeftTrigger.StartPosition = To255(StartPosition, 8);
		HidOutput.LeftTrigger.EndPosition = To255(EndPosition, 8);
		HidOutput.LeftTrigger.Strengths.Start = To255(Force, 9);
	}

	if (Hand == EControllerHand::Right || Hand == EControllerHand::AnyHand)
	{
		HidOutput.RightTrigger.Mode = 0x02;
		HidOutput.RightTrigger.StartPosition = To255(StartPosition, 8);
		HidOutput.RightTrigger.EndPosition = To255(EndPosition, 8);
		HidOutput.RightTrigger.Strengths.Start = To255(Force, 9);
	}

	SendOut();
}

void UDualSenseLibrary::Feedback(int32 BeginForce, int32 MiddleForce, int32 EndForce, const EControllerHand& Hand)
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
	PositionalAmplitudes[8] = EndForce;
	PositionalAmplitudes[9] = EndForce;

	int32 ActiveZones = 0;
	int16 ForceValues = 0;
	for (int i = 0; i < 3; i++)
	{
		if (PositionalAmplitudes[i] > 0)
		{
			const int8_t ForceValue = static_cast<int8_t>((PositionalAmplitudes[i] - 1) & 0x07);
			ForceValues |= (ForceValue << (3 * i));
			ActiveZones |= static_cast<int16>(1 << i);
		}
	}

	if (Hand == EControllerHand::Left || Hand == EControllerHand::AnyHand)
	{
		HidOutput.LeftTrigger.Mode = 0x21;
		HidOutput.LeftTrigger.Strengths.ActiveZones = ActiveZones;
		HidOutput.LeftTrigger.Strengths.StrengthZones = ForceValues;
	}

	if (Hand == EControllerHand::Right || Hand == EControllerHand::AnyHand)
	{
		HidOutput.RightTrigger.Mode = 0x21;
		HidOutput.RightTrigger.Strengths.ActiveZones = ActiveZones;
		HidOutput.RightTrigger.Strengths.StrengthZones = ForceValues;
	}

	SendOut();
}

void UDualSenseLibrary::SetWeaponEffects(int32 StartPosition, int32 EndPosition, int32 Force,
                                         const EControllerHand& Hand)
{
	if (Hand == EControllerHand::Left || Hand == EControllerHand::AnyHand)
	{
		HidOutput.LeftTrigger.Mode = 0x25;
		HidOutput.LeftTrigger.StartPosition = To255(StartPosition);
		HidOutput.LeftTrigger.EndPosition = To255(EndPosition);
		HidOutput.LeftTrigger.Strengths.Start = To255(Force);
	}

	if (Hand == EControllerHand::Right || Hand == EControllerHand::AnyHand)
	{
		HidOutput.RightTrigger.Mode = 0x25;
		HidOutput.RightTrigger.StartPosition = To255(StartPosition);
		HidOutput.RightTrigger.EndPosition = To255(EndPosition);
		HidOutput.RightTrigger.Strengths.Start = To255(Force);
	}

	SendOut();
}

void UDualSenseLibrary::SetGallopingEffects(int32 StartPosition, int32 EndPosition, int32 FirstFoot, int32 SecondFoot,
                                            float Frequency, const EControllerHand& Hand)
{
	const uint32_t ActiveZones = ((1 << StartPosition) | (1 << EndPosition));
	const uint32_t TimeAndRatio = (((SecondFoot & 0x07) << (3 * 0)) | ((FirstFoot & 0x07) << (3 * 1)));
	if (Hand == EControllerHand::Left || Hand == EControllerHand::AnyHand)
	{
		HidOutput.LeftTrigger.Mode = 0x23;
		HidOutput.LeftTrigger.Strengths.ActiveZones = ActiveZones;
		HidOutput.LeftTrigger.Strengths.TimeAndRatio = TimeAndRatio;
		HidOutput.LeftTrigger.Frequency = To255(Frequency);
	}

	if (Hand == EControllerHand::Right || Hand == EControllerHand::AnyHand)
	{
		HidOutput.RightTrigger.Mode = 0x23;
		HidOutput.RightTrigger.Strengths.ActiveZones = ActiveZones;
		HidOutput.RightTrigger.Strengths.TimeAndRatio = TimeAndRatio;
		HidOutput.RightTrigger.Frequency = To255(Frequency);
	}

	SendOut();
}

void UDualSenseLibrary::SetMachineEffects(int32 StartPosition, int32 EndPosition, int32 AmplitudeBegin,
                                          int32 AmplitudeEnd, float Frequency, float Period,
                                          const EControllerHand& Hand)
{
	const uint32_t ActiveZones = ((1 << StartPosition) | (1 << EndPosition));
	const uint32_t Forces = (((AmplitudeBegin & 0x07) << (3 * 0)) | ((AmplitudeEnd & 0x07) << (3 * 1)));

	if (Period < 0.0f || Period > 3.f)
	{
		Period = 3.f;
	}

	if (Hand == EControllerHand::Left || Hand == EControllerHand::AnyHand)
	{
		HidOutput.LeftTrigger.Mode = 0x27;
		HidOutput.LeftTrigger.Strengths.ActiveZones = ActiveZones;
		HidOutput.LeftTrigger.Strengths.StrengthZones = Forces;
		HidOutput.LeftTrigger.Strengths.Period = To255(Period);
		HidOutput.LeftTrigger.Frequency = To255(Frequency);
	}

	if (Hand == EControllerHand::Right || Hand == EControllerHand::AnyHand)
	{
		HidOutput.RightTrigger.Mode = 0x27;
		HidOutput.RightTrigger.Strengths.ActiveZones = ActiveZones;
		HidOutput.RightTrigger.Strengths.StrengthZones = Forces;
		HidOutput.RightTrigger.Strengths.Period = To255(Period);
		HidOutput.RightTrigger.Frequency = To255(Frequency);
	}

	SendOut();
}

void UDualSenseLibrary::SetBowEffects(int32 StartPosition, int32 EndPosition, int32 BegingForce, int32 EndForce,
                                      const EControllerHand& Hand)
{
	const uint32_t ActiveZones = ((1 << StartPosition) | (1 << EndPosition));
	const uint32_t Forces = ((((BegingForce - 1) & 0x07) << (3 * 0)) | (((EndForce - 1) & 0x07) << (3 * 1)));
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
	HidOutput.LedPlayerHid.Brightness = 0x0;
	HidOutput.LedPlayerHid.Led = PLAYER_LED_MIDDLE;
	HidOutput.LedPlayerHid.Player = 0x2;
	HidOutput.LeftTrigger.Mode = 0x0;
	HidOutput.RightTrigger.Mode = 0x0;
	SendOut();
}

void UDualSenseLibrary::StopAll()
{
	HidOutput.LedPlayerHid.Brightness = 0;
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
		HidOutput.LedPlayerHid.Led = PLAYER_LED_LEFT | PLAYER_LED_RIGHT | PLAYER_LED_MIDDLE_LEFT |
			PLAYER_LED_MIDDLE_RIGHT;
	}

	HidOutput.LedPlayerHid.Player = 0x1;
	HidOutput.LeftTrigger.Mode = 0x0;
	HidOutput.RightTrigger.Mode = 0x0;
	SendOut();
}

void UDualSenseLibrary::SetLedPlayerEffects(int32 NumberLeds, int32 BrightnessValue)
{
	if (NumberLeds == 0)
	{
		HidOutput.LedPlayerHid.Player = 0x04;
		HidOutput.LedPlayerHid.Led = PLAYER_LED_MIDDLE;
		HidOutput.LedPlayerHid.Brightness = 0x00;
		return;
	}

	HidOutput.LedPlayerHid.Player = 0x01;
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

float UDualSenseLibrary::GetLevelBattery() const
{
	return LevelBattery;
}
