// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025


#include "DualSenseProxy.h"
#include "DualSenseLibrary.h"
#include "FDualSenseLibraryManager.h"
#include "FValidationUtils.h"
#include "Runtime/ApplicationCore/Public/GenericPlatform/IInputInterface.h"
#include "Runtime/ApplicationCore/Public/GenericPlatform/GenericApplicationMessageHandler.h"


bool UDualSenseProxy::DeviceIsConnected(int32 ControllerId)
{
	UDualSenseLibrary* DualSenseInstance = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return false;
	}
	
	return DualSenseInstance->IsConnected();
}

bool UDualSenseProxy::DeviceReconnect(int32 ControllerId)
{
	UDualSenseLibrary* DualSenseInstance = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return false;
	}
	
	if (DualSenseInstance->IsConnected())
	{
		return true;
	}

	return DualSenseInstance->Reconnect();
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

void UDualSenseProxy::SetTriggerHapticFeedbackEffect(int32 ControllerId, int32 StartPosition, int32 BeginForce,
                                                     int32 MiddleForce, int32 EndForce, EControllerHand Hand,
                                                     bool KeepEffect)
{
	if (
		!FValidationUtils::ValidateMaxForce(BeginForce) &&
		!FValidationUtils::ValidateMaxForce(MiddleForce) &&
		!FValidationUtils::ValidateMaxForce(EndForce) &&
		!FValidationUtils::ValidateMaxPosition(StartPosition)
	)
	{
		return;
	}

	UDualSenseLibrary* DualSenseInstance = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}
	
	DualSenseInstance->ConfigTriggerHapticFeedbackEffect(StartPosition, BeginForce, MiddleForce, EndForce, Hand, KeepEffect);
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

void UDualSenseProxy::EffectContinuousResitance(int32 ControllerId, int32 StartPosition, int32 Force,
                                                EControllerHand HandContinous)
{
	if (
		!FValidationUtils::ValidateMaxForce(Force) &&
		!FValidationUtils::ValidateMaxPosition(StartPosition)
	)
	{
		return;
	}

	UDualSenseLibrary* DualSenseInstance = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}
	
	DualSenseInstance->ContinuousResitance(StartPosition, Force, HandContinous);
}

void UDualSenseProxy::EffectSectionResitance(int32 ControllerId, int32 StartPosition, int32 EndPosition,
                                             EControllerHand Hand)
{
	if (
		!FValidationUtils::ValidateMaxPosition(StartPosition) &&
		!FValidationUtils::ValidateMaxPosition(EndPosition)
	)
	{
		return;
	}

	UDualSenseLibrary* DualSenseInstance = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DualSenseInstance)
	{
		return;
	}
	
	DualSenseInstance->SectionResitance(StartPosition, EndPosition, Hand);
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
