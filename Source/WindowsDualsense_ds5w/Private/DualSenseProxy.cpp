// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025


#include "DualSenseProxy.h"
#include "DualSenseLibrary.h"
#include "DualSenseLibraryManager.h"
#include "ValidateHelpers.h"
#include "Runtime/ApplicationCore/Public/GenericPlatform/IInputInterface.h"


void UDualSenseProxy::DeviceSettings(int32 ControllerId, FDualSenseFeatureReport Settings)
{
	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->RegisterSettings(
		Settings.MicStatus,
		Settings.AudioHeadset,
		Settings.AudioSpeaker,
		Settings.VibrationMode,
		Settings.MicVolume,
		Settings.AudioVolume,
		Settings.SoftRumbleReduce,
		Settings.SoftRumble
	);
}

bool UDualSenseProxy::DeviceDisconnect(int32 ControllerId)
{
	UDualSenseLibraryManager::Get()->RemoveLibraryInstance(ControllerId);
	return true;
}

bool UDualSenseProxy::DeviceReconnect(int32 ControllerId)
{
	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryOrReconnect(ControllerId);
	if (!DualSenseInstance)
	{
		return false;
	}

	return true;
}

bool UDualSenseProxy::DeviceIsConnected(int32 ControllerId)
{
	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return false;
	}

	return true;
}

float UDualSenseProxy::LevelBatteryDevice(int32 ControllerId)
{
	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);

	if (!DualSenseInstance)
	{
		return 0.0f;
	}

	return DualSenseInstance->GetLevelBattery();
}

int32 UDualSenseProxy::GetTriggerLeftForceFeedback(int32 ControllerId)
{
	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return 0;
	}

	EControllerHand HandToUse = EControllerHand::Left;
	return DualSenseInstance->GetTrirggersFeedback(HandToUse);
}

int32 UDualSenseProxy::GetTriggerRightForceFeedback(int32 ControllerId)
{
	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return 0;
	}

	EControllerHand HandToUse = EControllerHand::Right;
	return DualSenseInstance->GetTrirggersFeedback(HandToUse);
}

void UDualSenseProxy::SetVibrationFromAudio(
	const int32 ControllerId,
	const float AverageEnvelopeValue,
	const float MaxEnvelopeValue,
	const int32 NumWaveInstances,
	const float EnvelopeToVibrationMultiplier,
	const float PeakToVibrationMultiplier,
	const float Threshold,
	const float ExponentCurve,
	const float BaseMultiplier
)
{
	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	const float VibrationLeft = FMath::Clamp(AverageEnvelopeValue * EnvelopeToVibrationMultiplier * NumWaveInstances,
	                                         0.0f, 1.0f);
	const float VibrationRight = FMath::Clamp(MaxEnvelopeValue * PeakToVibrationMultiplier * NumWaveInstances, 0.0f,
	                                          1.0f);

	FForceFeedbackValues FeedbackValues;
	FeedbackValues.LeftLarge = VibrationLeft;
	FeedbackValues.RightLarge = VibrationRight;
	DualSenseInstance->SetVibrationAudioBased(FeedbackValues, Threshold, ExponentCurve, BaseMultiplier);
}

void UDualSenseProxy::SetFeedback(int32 ControllerId, int32 BeginForce,
                                  int32 MiddleForce, int32 EndForce, EControllerHand Hand)
{
	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	return DualSenseInstance->Feedback(BeginForce, MiddleForce, EndForce, Hand);
}

void UDualSenseProxy::Resistance(int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 Force,
                                 EControllerHand Hand)
{
	if (!UValidateHelpers::ValidateMaxPosition(StartPosition)) StartPosition = 0;
	if (!UValidateHelpers::ValidateMaxPosition(EndPosition)) EndPosition = 8;
	if (!UValidateHelpers::ValidateMaxPosition(Force)) Force = 8;

	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}
	DualSenseInstance->SectionResistance(StartPosition, EndPosition, Force, Hand);
}

void UDualSenseProxy::SetTriggerHapticFeedbackEffect(int32 ControllerId, int32 StartPosition, int32 BeginForce,
                                                     int32 MiddleForce, int32 EndForce, EControllerHand Hand,
                                                     bool KeepEffect)
{
	if (!UValidateHelpers::ValidateMaxPosition(StartPosition)) StartPosition = 0;
	if (!UValidateHelpers::ValidateMaxPosition(BeginForce)) BeginForce = 8;
	if (!UValidateHelpers::ValidateMaxPosition(MiddleForce)) MiddleForce = 8;
	if (!UValidateHelpers::ValidateMaxPosition(EndForce)) EndForce = 8;

	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->ConfigTriggerHapticFeedbackEffect(StartPosition, BeginForce, MiddleForce, EndForce, Hand,
	                                                     KeepEffect);
}

void UDualSenseProxy::ContinuousResistance(int32 ControllerId, int32 StartPosition, int32 Force, EControllerHand Hand)
{
	if (!UValidateHelpers::ValidateMaxPosition(StartPosition)) StartPosition = 0;
	if (!UValidateHelpers::ValidateMaxPosition(Force)) Force = 8;

	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}
	DualSenseInstance->ContinuousResistance(StartPosition, Force, Hand);
}

void UDualSenseProxy::Galloping(
	int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 BeginForce,
                                int32 EndForce, float Frequency, EControllerHand Hand)
{
	if (!UValidateHelpers::ValidateMaxPosition(StartPosition)) StartPosition = 0;
	if (!UValidateHelpers::ValidateMaxPosition(EndPosition)) EndPosition = 8;
	if (!UValidateHelpers::ValidateMaxPosition(BeginForce)) BeginForce = 0;
	if (!UValidateHelpers::ValidateMaxPosition(EndForce)) EndForce = 8;

	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->SetGallopingEffects(StartPosition, EndPosition, BeginForce, EndForce, Frequency, Hand);
}

void UDualSenseProxy::Machine(int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 FirstFoot,
                              int32 LasFoot, float Frequency, float Period, EControllerHand Hand)
{
	if (!UValidateHelpers::ValidateMaxPosition(StartPosition)) StartPosition = 0;
	if (!UValidateHelpers::ValidateMaxPosition(EndPosition)) EndPosition = 8;
	if (!UValidateHelpers::ValidateMaxPosition(FirstFoot)) FirstFoot = 1;
	if (!UValidateHelpers::ValidateMaxPosition(LasFoot)) LasFoot = 7;

	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->SetMachineEffects(StartPosition, EndPosition, FirstFoot, LasFoot, Frequency, Period, Hand);
}

void UDualSenseProxy::Weapon(int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 Force,
	EControllerHand Hand)
{
	if (!UValidateHelpers::ValidateMaxPosition(StartPosition)) StartPosition = 0;
	if (!UValidateHelpers::ValidateMaxPosition(EndPosition)) EndPosition = 8;
	if (!UValidateHelpers::ValidateMaxPosition(Force)) Force = 8;

	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->SetWeaponEffects(StartPosition, EndPosition, Force, Hand);
}

void UDualSenseProxy::Bow(int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 BeginForce, int32 EndForce,
                          EControllerHand Hand)
{
	if (!UValidateHelpers::ValidateMaxPosition(StartPosition)) StartPosition = 0;
	if (!UValidateHelpers::ValidateMaxPosition(EndPosition)) EndPosition = 8;
	if (!UValidateHelpers::ValidateMaxPosition(BeginForce)) BeginForce = 0;
	if (!UValidateHelpers::ValidateMaxPosition(EndForce)) EndForce = 8;

	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->SetBowEffects(StartPosition, EndPosition, BeginForce, EndForce, Hand);
}

void UDualSenseProxy::LedPlayerEffects(int32 ControllerId, ELedPlayerEnum Value, ELedBrightnessEnum Brightness)
{
	int32 BrightnessValue = static_cast<int32>(Brightness);
	int32 LedValue = static_cast<int32>(Value);

	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->SetLedPlayerEffects(LedValue, BrightnessValue);
}

void UDualSenseProxy::LedMicEffects(int32 ControllerId, ELedMicEnum Value)
{
	int32 LedNumber = 0;
	if (Value == ELedMicEnum::MicOn)
	{
		LedNumber = 1;
	}

	if (Value == ELedMicEnum::Pulse)
	{
		LedNumber = 2;
	}

	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->SetLedMicEffects(LedNumber);
}

void UDualSenseProxy::LedColorEffects(int32 ControllerId, FColor Color)
{
	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->UpdateColorOutput(Color);
}

void UDualSenseProxy::EnableTouch(int32 ControllerId, bool bEnableTouch)
{
	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->SetTouch(bEnableTouch);
}

void UDualSenseProxy::EnableAccelerometerValues(int32 ControllerId, bool bEnableAccelerometer)
{
	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->SetAcceleration(bEnableAccelerometer);
}

void UDualSenseProxy::EnableGyroscopeValues(int32 ControllerId, bool bEnableGyroscope)
{
	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->SetGyroscope(bEnableGyroscope);
}

void UDualSenseProxy::NoResistance(int32 ControllerId, EControllerHand Hand)
{
	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->NoResistance(Hand);
}

void UDualSenseProxy::StopTriggerEffect(const int32 ControllerId, EControllerHand HandStop)
{
	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->StopEffect(HandStop);
}

void UDualSenseProxy::StopAllTriggersEffects(const int32 ControllerId)
{
	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->StopAllEffects();
}

void UDualSenseProxy::ResetEffects(const int32 ControllerId)
{
	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->StopAll();
}
