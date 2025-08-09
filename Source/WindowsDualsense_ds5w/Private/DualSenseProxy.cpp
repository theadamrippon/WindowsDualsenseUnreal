// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025


#include "DualSenseProxy.h"
#include "../Public/Core/DualSense/DualSenseLibrary.h"
#include "Core/DualSense/DualSenseLibraryManager.h"
#include "Helpers/ValidateHelpers.h"
#include "Runtime/ApplicationCore/Public/GenericPlatform/IInputInterface.h"


void UDualSenseProxy::DeviceSettings(int32 ControllerId, FDualSenseFeatureReport Settings)
{
	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}
	
	DualSenseInstance->Settings(Settings);
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

	return DualSenseInstance->GetBattery();
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

void UDualSenseProxy::SetFeedback(int32 ControllerId, int32 BeginStrength,
                                  int32 MiddleStrength, int32 EndStrength, EControllerHand Hand)
{
	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	return DualSenseInstance->SetResistance(BeginStrength, MiddleStrength, EndStrength, Hand);
}

void UDualSenseProxy::Resistance(int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 Strength,
                                 EControllerHand Hand)
{
	if (!UValidateHelpers::ValidateMaxPosition(StartPosition)) StartPosition = 0;
	if (!UValidateHelpers::ValidateMaxPosition(EndPosition)) EndPosition = 8;
	if (!UValidateHelpers::ValidateMaxPosition(Strength)) Strength = 8;

	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}
	DualSenseInstance->SetResistance(StartPosition, EndPosition, Strength, Hand);
}

void UDualSenseProxy::AutomaticGun(int32 ControllerId, int32 BeginStrength,
                                                     int32 MiddleStrength, int32 EndStrength, EControllerHand Hand,
                                                     bool KeepEffect)
{
	if (!UValidateHelpers::ValidateMaxPosition(BeginStrength)) BeginStrength = 8;
	if (!UValidateHelpers::ValidateMaxPosition(MiddleStrength)) MiddleStrength = 8;
	if (!UValidateHelpers::ValidateMaxPosition(EndStrength)) EndStrength = 8;

	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->SetAutomaticGun(BeginStrength, MiddleStrength, EndStrength, Hand, KeepEffect);
}

void UDualSenseProxy::ContinuousResistance(int32 ControllerId, int32 StartPosition, int32 Strength, EControllerHand Hand)
{
	if (!UValidateHelpers::ValidateMaxPosition(StartPosition)) StartPosition = 0;
	if (!UValidateHelpers::ValidateMaxPosition(Strength)) Strength = 8;

	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}
	DualSenseInstance->SetContinuousResistance(StartPosition, Strength, Hand);
}

void UDualSenseProxy::Galloping(
	int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 BeginStrength,
                                int32 EndStrength, float Frequency, EControllerHand Hand)
{
	if (!UValidateHelpers::ValidateMaxPosition(StartPosition)) StartPosition = 0;
	if (!UValidateHelpers::ValidateMaxPosition(EndPosition)) EndPosition = 8;
	if (!UValidateHelpers::ValidateMaxPosition(BeginStrength)) BeginStrength = 0;
	if (!UValidateHelpers::ValidateMaxPosition(EndStrength)) EndStrength = 8;

	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->SetGalloping(StartPosition, EndPosition, BeginStrength, EndStrength, Frequency, Hand);
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

	DualSenseInstance->SetMachine(StartPosition, EndPosition, FirstFoot, LasFoot, Frequency, Period, Hand);
}

void UDualSenseProxy::Weapon(int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 Strength,
	EControllerHand Hand)
{
	if (!UValidateHelpers::ValidateMaxPosition(StartPosition)) StartPosition = 0;
	if (!UValidateHelpers::ValidateMaxPosition(EndPosition)) EndPosition = 8;
	if (!UValidateHelpers::ValidateMaxPosition(Strength)) Strength = 8;

	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->SetWeapon(StartPosition, EndPosition, Strength, Hand);
}

void UDualSenseProxy::Bow(int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 BeginStrength, int32 EndStrength,
                          EControllerHand Hand)
{
	if (!UValidateHelpers::ValidateMaxPosition(StartPosition)) StartPosition = 0;
	if (!UValidateHelpers::ValidateMaxPosition(EndPosition)) EndPosition = 8;
	if (!UValidateHelpers::ValidateMaxPosition(BeginStrength)) BeginStrength = 0;
	if (!UValidateHelpers::ValidateMaxPosition(EndStrength)) EndStrength = 8;

	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->SetBow(StartPosition, EndPosition, BeginStrength, EndStrength, Hand);
}

void UDualSenseProxy::LedPlayerEffects(int32 ControllerId, ELedPlayerEnum Value, ELedBrightnessEnum Brightness)
{
	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->SetPlayerLed(Value, Brightness);
}

void UDualSenseProxy::LedMicEffects(int32 ControllerId, ELedMicEnum Value)
{
	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->SetMicrophoneLed(Value);
}

void UDualSenseProxy::LedColorEffects(int32 ControllerId, FColor Color)
{
	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->SetLightbar(Color);
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

	DualSenseInstance->StopTrigger(Hand);
}

void UDualSenseProxy::StopTriggerEffect(const int32 ControllerId, EControllerHand HandStop)
{
	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->StopTrigger(HandStop);
}

void UDualSenseProxy::StopAllTriggersEffects(const int32 ControllerId)
{
	UDualSenseLibrary* DualSenseInstance = UDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->StopTrigger(EControllerHand::AnyHand);
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
