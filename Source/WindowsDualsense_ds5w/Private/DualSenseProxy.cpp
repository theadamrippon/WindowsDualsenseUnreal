// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025


#include "DualSenseProxy.h"
#include "DualSenseLibrary.h"
#include "FDualSenseLibraryManager.h"
#include "FValidationUtils.h"
#include "Runtime/ApplicationCore/Public/GenericPlatform/IInputInterface.h"


bool UDualSenseProxy::DeviceIsConnected(int32 ControllerId)
{
	UDualSenseLibrary* DualSenseInstance = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return false;
	}

	return true;
}

bool UDualSenseProxy::DeviceReconnect(int32 ControllerId)
{
	UDualSenseLibrary* DualSenseInstance = UFDualSenseLibraryManager::Get()->GetLibraryOrRecconect(ControllerId);
	if (!DualSenseInstance)
	{
		return false;
	}

	return true;
}

void UDualSenseProxy::SetVibrationFromAudio(
	const int32 ControllerId,
	float AverageEnvelopeValue,
	float MaxEnvelopeValue,
	int32 NumWaveInstances,
	float EnvelopeToVibrationMultiplier = 1.2f,
	float PeakToVibrationMultiplier = 0.8f
)
{
	UDualSenseLibrary* DualSenseInstance = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	float InstancesFactor = FMath::Clamp(static_cast<float>(NumWaveInstances), 1.0f, 4.0f);
	float VibrationLeft = FMath::Clamp(AverageEnvelopeValue * EnvelopeToVibrationMultiplier * InstancesFactor, 0.0f, 1.0f);
	float VibrationRight = FMath::Clamp(MaxEnvelopeValue * PeakToVibrationMultiplier * InstancesFactor, 0.0f, 1.0f);

	FForceFeedbackValues FeedbackValues;
	FeedbackValues.LeftLarge = VibrationLeft;
	FeedbackValues.RightLarge = VibrationRight;
	DualSenseInstance->SetVibrationAudioBased(FeedbackValues);
}

bool UDualSenseProxy::DeviceDisconnect(int32 ControllerId)
{
	UFDualSenseLibraryManager::Get()->RemoveLibraryInstance(ControllerId);
	return true;
}

float UDualSenseProxy::LevelBatteryDevice(int32 ControllerId)
{
	UDualSenseLibrary* DualSenseInstance = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);

	if (!DualSenseInstance)
	{
		return 0.0f;
	}

	return DualSenseInstance->GetLevelBattery();
}

int32 UDualSenseProxy::GetTriggerLeftForceFeedback(int32 ControllerId)
{
	UDualSenseLibrary* DualSenseInstance = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return 0;
	}

	EControllerHand HandToUse = EControllerHand::Left;
	return DualSenseInstance->GetTrirggersFeedback(HandToUse);
}


int32 UDualSenseProxy::GetTriggerRightForceFeedback(int32 ControllerId)
{
	UDualSenseLibrary* DualSenseInstance = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return 0;
	}

	EControllerHand HandToUse = EControllerHand::Right;
	return DualSenseInstance->GetTrirggersFeedback(HandToUse);
}

void UDualSenseProxy::SetFeedback(int32 ControllerId, int32 BeginForce,
                                  int32 MiddleForce, int32 EndForce, EControllerHand Hand)
{
	UDualSenseLibrary* DualSenseInstance = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	return DualSenseInstance->Feedback(BeginForce, MiddleForce, EndForce, Hand);
}

void UDualSenseProxy::SetTriggerHapticFeedbackEffect(int32 ControllerId, int32 StartPosition, int32 BeginForce,
                                                     int32 MiddleForce, int32 EndForce, EControllerHand Hand,
                                                     bool KeepEffect)
{
	if (!FValidationUtils::ValidateMaxPosition(StartPosition)) StartPosition = 0;
	if (!FValidationUtils::ValidateMaxPosition(BeginForce)) BeginForce = 8;
	if (!FValidationUtils::ValidateMaxPosition(MiddleForce)) MiddleForce = 8;
	if (!FValidationUtils::ValidateMaxPosition(EndForce)) EndForce = 8;

	UDualSenseLibrary* DualSenseInstance = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->ConfigTriggerHapticFeedbackEffect(StartPosition, BeginForce, MiddleForce, EndForce, Hand,
	                                                     KeepEffect);
}

void UDualSenseProxy::EffectNoResitance(int32 ControllerId, EControllerHand HandResistence)
{
	UDualSenseLibrary* DualSenseInstance = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->NoResitance(HandResistence);
}

void UDualSenseProxy::EffectSectionResitance(int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 Force,
                                             EControllerHand Hand)
{
	if (!FValidationUtils::ValidateMaxPosition(StartPosition)) StartPosition = 0;
	if (!FValidationUtils::ValidateMaxPosition(EndPosition)) EndPosition = 8;
	if (!FValidationUtils::ValidateMaxPosition(Force)) Force = 8;

	UDualSenseLibrary* DualSenseInstance = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}
	DualSenseInstance->SectionResitance(StartPosition, EndPosition, Force, Hand);
}

void UDualSenseProxy::EffectWeapon(int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 Force,
                                   EControllerHand Hand)
{
	if (!FValidationUtils::ValidateMaxPosition(StartPosition)) StartPosition = 0;
	if (!FValidationUtils::ValidateMaxPosition(EndPosition)) EndPosition = 8;
	if (!FValidationUtils::ValidateMaxPosition(Force)) Force = 8;

	UDualSenseLibrary* DualSenseInstance = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->SetWeaponEffects(StartPosition, EndPosition, Force, Hand);
}

void UDualSenseProxy::EffectGalloping(int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 BeginForce,
                                      int32 EndForce, float Frequency, EControllerHand Hand)
{
	if (!FValidationUtils::ValidateMaxPosition(StartPosition)) StartPosition = 0;
	if (!FValidationUtils::ValidateMaxPosition(EndPosition)) EndPosition = 8;
	if (!FValidationUtils::ValidateMaxPosition(BeginForce)) BeginForce = 0;
	if (!FValidationUtils::ValidateMaxPosition(EndForce)) EndForce = 8;

	UDualSenseLibrary* DualSenseInstance = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->SetGallopingEffects(StartPosition, EndPosition, BeginForce, EndForce, Frequency, Hand);
}

void UDualSenseProxy::EffectMachine(int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 FirstFoot,
                                    int32 LasFoot, float Frequency, float Period, EControllerHand Hand)
{
	if (!FValidationUtils::ValidateMaxPosition(StartPosition)) StartPosition = 0;
	if (!FValidationUtils::ValidateMaxPosition(EndPosition)) EndPosition = 8;
	if (!FValidationUtils::ValidateMaxPosition(FirstFoot)) FirstFoot = 0;
	if (!FValidationUtils::ValidateMaxPosition(LasFoot)) LasFoot = 8;

	UDualSenseLibrary* DualSenseInstance = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->SetMachineEffects(StartPosition, EndPosition, FirstFoot, LasFoot, Frequency, Period, Hand);
}

void UDualSenseProxy::EffectBow(int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 BegingForce,
                                int32 EndForce, EControllerHand Hand)
{
	if (!FValidationUtils::ValidateMaxPosition(StartPosition)) StartPosition = 0;
	if (!FValidationUtils::ValidateMaxPosition(EndPosition)) EndPosition = 8;
	if (!FValidationUtils::ValidateMaxPosition(BegingForce)) BegingForce = 0;
	if (!FValidationUtils::ValidateMaxPosition(EndForce)) EndForce = 8;

	UDualSenseLibrary* DualSenseInstance = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->SetBowEffects(StartPosition, EndPosition, BegingForce, EndForce, Hand);
}

void UDualSenseProxy::EffectContinuousResitance(int32 ControllerId, int32 StartPosition, int32 Force,
                                                EControllerHand ContinuousHand)
{
	if (!FValidationUtils::ValidateMaxPosition(StartPosition)) StartPosition = 0;
	if (!FValidationUtils::ValidateMaxPosition(Force)) Force = 8;

	UDualSenseLibrary* DualSenseInstance = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}
	DualSenseInstance->ContinuousResitance(StartPosition, Force, ContinuousHand);
}

void UDualSenseProxy::StopTriggerEffect(const int32 ControllerId, EControllerHand HandStop)
{
	UDualSenseLibrary* DualSenseInstance = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->StopEffect(HandStop);
}

void UDualSenseProxy::StopAllTriggersEffects(const int32 ControllerId)
{
	UDualSenseLibrary* DualSenseInstance = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->StopAllEffects();
}

void UDualSenseProxy::ResetEffects(const int32 ControllerId)
{
	UDualSenseLibrary* DualSenseInstance = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->StopAll();
}


void UDualSenseProxy::LedPlayerEffects(int32 ControllerId, ELedPlayerEnum Value, ELedBrightnessEnum Brightness)
{
	int32 BrightnessValue = static_cast<int32>(Brightness);
	int32 LedValue = static_cast<int32>(Value);

	UDualSenseLibrary* DualSenseInstance = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
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

	UDualSenseLibrary* DualSenseInstance = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->SetLedMicEffects(LedNumber);
}

void UDualSenseProxy::LedColorEffects(int32 ControllerId, FColor Color)
{
	UDualSenseLibrary* DualSenseInstance = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->UpdateColorOutput(Color);
}

void UDualSenseProxy::EnableAccelerometerValues(int32 ControllerId, bool bEnableAccelerometer)
{
	UDualSenseLibrary* DualSenseInstance = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->SetAcceleration(bEnableAccelerometer);
}

void UDualSenseProxy::EnableGyroscopeValues(int32 ControllerId, bool bEnableGyroscope)
{
	UDualSenseLibrary* DualSenseInstance = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->SetGyroscope(bEnableGyroscope);
}

void UDualSenseProxy::EnableTouch1(int32 ControllerId, bool bEnableTouch)
{
	UDualSenseLibrary* DualSenseInstance = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->SetTouch1(bEnableTouch);
}

void UDualSenseProxy::EnableTouch2(int32 ControllerId, bool bEnableTouch)
{
	UDualSenseLibrary* DualSenseInstance = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}

	DualSenseInstance->SetTouch2(bEnableTouch);
}
