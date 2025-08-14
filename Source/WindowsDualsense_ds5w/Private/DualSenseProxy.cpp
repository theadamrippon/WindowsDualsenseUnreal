// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "DualSenseProxy.h"

#include "Core/DeviceContainerManager.h"
#include "Core/DualSense/DualSenseLibrary.h"
#include "Core/Interfaces/SonyGamepadInterface.h"
#include "Core/Interfaces/SonyGamepadTriggerInterface.h"
#include "Helpers/ValidateHelpers.h"
#include "Runtime/ApplicationCore/Public/GenericPlatform/IInputInterface.h"


void UDualSenseProxy::DeviceSettings(int32 ControllerId, FDualSenseFeatureReport Settings)
{
	ISonyGamepadInterface* Gamepad = UDeviceContainerManager::Get()->GetLibraryInstance(ControllerId);
	if (!Gamepad)
	{
		return;
	}

	UDualSenseLibrary* DualSenseInstance = Cast<UDualSenseLibrary>(Gamepad);
	if (!DualSenseInstance)
	{
		return;
	}
	DualSenseInstance->Settings(Settings);
}

void UDualSenseProxy::LedPlayerEffects(int32 ControllerId, ELedPlayerEnum Value, ELedBrightnessEnum Brightness)
{
	ISonyGamepadInterface* Gamepad = Cast<ISonyGamepadInterface>(UDeviceContainerManager::Get()->GetLibraryInstance(ControllerId));
	if (!IsValid(Gamepad->_getUObject()))
	{
		return;
	}

	Gamepad->SetPlayerLed(Value, Brightness);
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
	ISonyGamepadTriggerInterface* Gamepad = Cast<ISonyGamepadTriggerInterface>(UDeviceContainerManager::Get()->GetLibraryInstance(ControllerId));
	if (!IsValid(Gamepad->_getUObject()))
	{
		return;
	}

	const float VibrationLeft = FMath::Clamp(AverageEnvelopeValue * EnvelopeToVibrationMultiplier * NumWaveInstances,0.0f, 1.0f);
	const float VibrationRight = FMath::Clamp(MaxEnvelopeValue * PeakToVibrationMultiplier * NumWaveInstances, 0.0f,1.0f);

	FForceFeedbackValues FeedbackValues;
	FeedbackValues.LeftLarge = VibrationLeft;
	FeedbackValues.RightLarge = VibrationRight;
	Gamepad->SetVibrationAudioBased(FeedbackValues, Threshold, ExponentCurve, BaseMultiplier);
}

void UDualSenseProxy::SetFeedback(int32 ControllerId, int32 BeginStrength,
                                  int32 MiddleStrength, int32 EndStrength, EControllerHand Hand)
{
	ISonyGamepadTriggerInterface* Gamepad = Cast<ISonyGamepadTriggerInterface>(UDeviceContainerManager::Get()->GetLibraryInstance(ControllerId));
	if (!IsValid(Gamepad->_getUObject()))
	{
		return;
	}

	return Gamepad->SetResistance(BeginStrength, MiddleStrength, EndStrength, Hand);
}

void UDualSenseProxy::Resistance(int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 Strength, EControllerHand Hand)
{
	if (!UValidateHelpers::ValidateMaxPosition(StartPosition)) StartPosition = 0;
	if (!UValidateHelpers::ValidateMaxPosition(EndPosition)) EndPosition = 8;
	if (!UValidateHelpers::ValidateMaxPosition(Strength)) Strength = 8;

	ISonyGamepadTriggerInterface* Gamepad = Cast<ISonyGamepadTriggerInterface>(UDeviceContainerManager::Get()->GetLibraryInstance(ControllerId));
	if (!IsValid(Gamepad->_getUObject()))
	{
		return;
	}
	
	Gamepad->SetResistance(StartPosition, EndPosition, Strength, Hand);
}

void UDualSenseProxy::AutomaticGun(int32 ControllerId, int32 BeginStrength, int32 MiddleStrength, int32 EndStrength, EControllerHand Hand, bool KeepEffect)
{
	if (!UValidateHelpers::ValidateMaxPosition(BeginStrength)) BeginStrength = 8;
	if (!UValidateHelpers::ValidateMaxPosition(MiddleStrength)) MiddleStrength = 8;
	if (!UValidateHelpers::ValidateMaxPosition(EndStrength)) EndStrength = 8;

	ISonyGamepadTriggerInterface* Gamepad = Cast<ISonyGamepadTriggerInterface>(UDeviceContainerManager::Get()->GetLibraryInstance(ControllerId));
	if (!IsValid(Gamepad->_getUObject()))
	{
		return;
	}
	
	Gamepad->SetAutomaticGun(BeginStrength, MiddleStrength, EndStrength, Hand, KeepEffect);
}

void UDualSenseProxy::ContinuousResistance(int32 ControllerId, int32 StartPosition, int32 Strength, EControllerHand Hand)
{
	if (!UValidateHelpers::ValidateMaxPosition(StartPosition)) StartPosition = 0;
	if (!UValidateHelpers::ValidateMaxPosition(Strength)) Strength = 8;

	ISonyGamepadTriggerInterface* Gamepad = Cast<ISonyGamepadTriggerInterface>(UDeviceContainerManager::Get()->GetLibraryInstance(ControllerId));
	if (!IsValid(Gamepad->_getUObject()))
	{
		return;
	}
	
	Gamepad->SetContinuousResistance(StartPosition, Strength, Hand);
}

void UDualSenseProxy::Galloping(
	int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 FirstFoot,
                                int32 SecondFoot, float Frequency, EControllerHand Hand)
{
	if (!UValidateHelpers::ValidateMaxPosition(StartPosition)) StartPosition = 0;
	if (!UValidateHelpers::ValidateMaxPosition(EndPosition)) EndPosition = 8;
	if (!UValidateHelpers::ValidateMaxPosition(FirstFoot)) FirstFoot = 2;
	if (!UValidateHelpers::ValidateMaxPosition(SecondFoot)) SecondFoot = 7;

	ISonyGamepadTriggerInterface* Gamepad = Cast<ISonyGamepadTriggerInterface>(UDeviceContainerManager::Get()->GetLibraryInstance(ControllerId));
	if (!IsValid(Gamepad->_getUObject()))
	{
		return;
	}
	
	Gamepad->SetGalloping(StartPosition, EndPosition, FirstFoot, SecondFoot, Frequency, Hand);
}

void UDualSenseProxy::Machine(int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 FirstFoot,
                              int32 LasFoot, float Frequency, float Period, EControllerHand Hand)
{
	if (!UValidateHelpers::ValidateMaxPosition(StartPosition)) StartPosition = 0;
	if (!UValidateHelpers::ValidateMaxPosition(EndPosition)) EndPosition = 8;
	if (!UValidateHelpers::ValidateMaxPosition(FirstFoot)) FirstFoot = 1;
	if (!UValidateHelpers::ValidateMaxPosition(LasFoot)) LasFoot = 7;

	ISonyGamepadTriggerInterface* Gamepad = Cast<ISonyGamepadTriggerInterface>(UDeviceContainerManager::Get()->GetLibraryInstance(ControllerId));
	if (!IsValid(Gamepad->_getUObject()))
	{
		return;
	}

	Gamepad->SetMachine(StartPosition, EndPosition, FirstFoot, LasFoot, Frequency, Period, Hand);
}

void UDualSenseProxy::Weapon(int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 Strength,
	EControllerHand Hand)
{
	if (!UValidateHelpers::ValidateMaxPosition(StartPosition)) StartPosition = 0;
	if (!UValidateHelpers::ValidateMaxPosition(EndPosition)) EndPosition = 8;
	if (!UValidateHelpers::ValidateMaxPosition(Strength)) Strength = 8;

	ISonyGamepadTriggerInterface* Gamepad = Cast<ISonyGamepadTriggerInterface>(UDeviceContainerManager::Get()->GetLibraryInstance(ControllerId));
	if (!IsValid(Gamepad->_getUObject()))
	{
		return;
	}

	Gamepad->SetWeapon(StartPosition, EndPosition, Strength, Hand);
}

void UDualSenseProxy::Bow(int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 BeginStrength, int32 EndStrength,
                          EControllerHand Hand)
{
	if (!UValidateHelpers::ValidateMaxPosition(StartPosition)) StartPosition = 0;
	if (!UValidateHelpers::ValidateMaxPosition(EndPosition)) EndPosition = 8;
	if (!UValidateHelpers::ValidateMaxPosition(BeginStrength)) BeginStrength = 0;
	if (!UValidateHelpers::ValidateMaxPosition(EndStrength)) EndStrength = 8;

	ISonyGamepadTriggerInterface* Gamepad = Cast<ISonyGamepadTriggerInterface>(UDeviceContainerManager::Get()->GetLibraryInstance(ControllerId));
	if (!IsValid(Gamepad->_getUObject()))
	{
		return;
	}

	Gamepad->SetBow(StartPosition, EndPosition, BeginStrength, EndStrength, Hand);
}

void UDualSenseProxy::NoResistance(int32 ControllerId, EControllerHand Hand)
{
	ISonyGamepadTriggerInterface* Gamepad = Cast<ISonyGamepadTriggerInterface>(UDeviceContainerManager::Get()->GetLibraryInstance(ControllerId));
	if (!IsValid(Gamepad->_getUObject()))
	{
		return;
	}

	Gamepad->StopTrigger(Hand);
}

void UDualSenseProxy::StopTriggerEffect(const int32 ControllerId, EControllerHand HandStop)
{
	ISonyGamepadTriggerInterface* Gamepad = Cast<ISonyGamepadTriggerInterface>(UDeviceContainerManager::Get()->GetLibraryInstance(ControllerId));
	if (!IsValid(Gamepad->_getUObject()))
	{
		return;
	}

	Gamepad->StopTrigger(HandStop);
}

void UDualSenseProxy::StopAllTriggersEffects(const int32 ControllerId)
{
	ISonyGamepadTriggerInterface* Gamepad = Cast<ISonyGamepadTriggerInterface>(UDeviceContainerManager::Get()->GetLibraryInstance(ControllerId));
	if (!IsValid(Gamepad->_getUObject()))
	{
		return;
	}

	Gamepad->StopTrigger(EControllerHand::AnyHand);
}

void UDualSenseProxy::ResetEffects(const int32 ControllerId)
{
	ISonyGamepadInterface* Gamepad = Cast<ISonyGamepadInterface>(UDeviceContainerManager::Get()->GetLibraryInstance(ControllerId));
	if (!IsValid(Gamepad->_getUObject()))
	{
		return;
	}

	Gamepad->StopAll();
}
