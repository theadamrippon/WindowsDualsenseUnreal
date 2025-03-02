// Fill out your copyright notice in the Description page of Project Settings.


#include "DualSenseProxy.h"
#include "DualSenseLibrary.h"
#include "Runtime/ApplicationCore/Public/GenericPlatform/IInputInterface.h"
#include "Runtime/ApplicationCore/Public/GenericPlatform/GenericApplicationMessageHandler.h"

bool UDualSenseProxy::DeviceIsConnected(const int32 ControllerId)
{
	return UDualSenseLibrary::IsConnected(ControllerId);
}

bool UDualSenseProxy::DeviceReconnect(const int32 ControllerId)
{
	if (UDualSenseLibrary::IsConnected(ControllerId))
	{
		return true;
	}
	
	return UDualSenseLibrary::Reconnect(ControllerId);
}

uint8 UDualSenseProxy::GetTriggerLeftForceFeedback(const int32 ControllerId)
{
	if (!UDualSenseLibrary::IsConnected(ControllerId))
	{
		return 0;
	}
	
	return UDualSenseLibrary::GetTrirggersFeedback(ControllerId, EControllerHand::Left);
}

uint8 UDualSenseProxy::GetTriggerRightForceFeedback(const int32 ControllerId)
{
	if (!UDualSenseLibrary::IsConnected(ControllerId))
	{
		return 0;
	}
	
	return UDualSenseLibrary::GetTrirggersFeedback(ControllerId, EControllerHand::Right);
}

void UDualSenseProxy::SetTriggerHapticFeedbackEffect(const int32 ControllerId, const uint8 StartPosition, const uint8 BeginForce,
	const uint8 MiddleForce, const uint8 EndForce, const EControllerHand& Hand, const bool KeepEffect)
{
	if (!UDualSenseLibrary::IsConnected(ControllerId))
	{
		return;
	}
	
	UDualSenseLibrary::ConfigTriggerHapticFeedbackEffect(ControllerId, StartPosition, BeginForce, MiddleForce, EndForce, Hand, KeepEffect);
}

void UDualSenseProxy::EffectNoResitance(const int32 ControllerId, const EControllerHand& Hand)
{
	if (!UDualSenseLibrary::IsConnected(ControllerId))
	{
		return;
	}
	
	UDualSenseLibrary::NoResitance(ControllerId, Hand);
}

void UDualSenseProxy::EffectContinuousResitance(const int32 ControllerId, const uint8 StartPosition, const uint8 Force,
	const EControllerHand& Hand)
{
	if (!UDualSenseLibrary::IsConnected(ControllerId))
	{
		return;
	}
	
	UDualSenseLibrary::ContinuousResitance(ControllerId, StartPosition, Force, Hand);
}

void UDualSenseProxy::EffectSectionResitance(const int32 ControllerId, const uint8 StartPosition, const uint8 EndPosition,
	const EControllerHand& Hand)
{
	if (!UDualSenseLibrary::IsConnected(ControllerId))
	{
		return;
	}

	UE_LOG(LogTemp, Error, TEXT("UDualSenseProxy::EffectSectionResitance - ControllerId: %d, position: %d, %d"), ControllerId, StartPosition, EndPosition);
	
	UDualSenseLibrary::SectionResitance(ControllerId, StartPosition, EndPosition, Hand);
}

void UDualSenseProxy::StopTriggerEffect(const int32 ControllerId, const EControllerHand& Hand)
{
	if (!UDualSenseLibrary::IsConnected(ControllerId))
	{
		return;
	}
	
	UDualSenseLibrary::StopEffect(ControllerId, Hand);
}

void UDualSenseProxy::StopAllTriggersEffects(const int32 ControllerId)
{
	if (!UDualSenseLibrary::IsConnected(ControllerId))
	{
		return;
	}
	
	UDualSenseLibrary::StopAllEffects(ControllerId);
}

void UDualSenseProxy::ResetEffects(const int32 ControllerId)
{
	if (!UDualSenseLibrary::IsConnected(ControllerId))
	{
		return;
	}
	
	UDualSenseLibrary::StopAll(ControllerId);
}
