// Fill out your copyright notice in the Description page of Project Settings.


#include "SonyGamepadProxy.h"

#include "Core/DeviceContainerManager.h"

bool USonyGamepadProxy::DeviceIsConnected(int32 ControllerId)
{
	if (const ISonyGamepadInterface* Gamepad = UDeviceContainerManager::Get()->GetLibraryOrReconnect(ControllerId); !Gamepad)
	{
		return false;
	}

	return true;
}

bool USonyGamepadProxy::DeviceReconnect(int32 ControllerId)
{
	if (const ISonyGamepadInterface* Gamepad = UDeviceContainerManager::Get()->GetLibraryOrReconnect(ControllerId); !Gamepad)
	{
		return false;
	}

	return true;
}

bool USonyGamepadProxy::DeviceDisconnect(int32 ControllerId)
{
	UDeviceContainerManager::Get()->RemoveLibraryInstance(ControllerId);
	return true;
}

float USonyGamepadProxy::LevelBatteryDevice(int32 ControllerId)
{
	ISonyGamepadInterface* Gamepad = UDeviceContainerManager::Get()->GetLibraryInstance(ControllerId);
	if (!Gamepad)
	{
		return 0.0f;
	}

	return Gamepad->GetBattery();
}

void USonyGamepadProxy::LedColorEffects(int32 ControllerId, FColor Color, float BrightnessTime, float ToogleTime)
{
	ISonyGamepadInterface* Gamepad = Cast<ISonyGamepadInterface>(UDeviceContainerManager::Get()->GetLibraryInstance(ControllerId));
	if (!Gamepad)
	{
		return;
	}

	Gamepad->Get()->SetLightbar(Color, BrightnessTime, ToogleTime);
}

void USonyGamepadProxy::LedMicEffects(int32 ControllerId, ELedMicEnum Value)
{
	ISonyGamepadInterface* Gamepad = Cast<ISonyGamepadInterface>(UDeviceContainerManager::Get()->GetLibraryInstance(ControllerId));
	if (!IsValid(Gamepad->_getUObject()))
	{
		return;
	}

	Gamepad->SetMicrophoneLed(Value);
}

void USonyGamepadProxy::EnableTouch(int32 ControllerId, bool bEnableTouch)
{
	ISonyGamepadInterface* Gamepad = Cast<ISonyGamepadInterface>(UDeviceContainerManager::Get()->GetLibraryInstance(ControllerId));
	if (!IsValid(Gamepad->_getUObject()))
	{
		return;
	}

	Gamepad->SetTouch(bEnableTouch);
}

void USonyGamepadProxy::EnableAccelerometerValues(int32 ControllerId, bool bEnableAccelerometer)
{
	ISonyGamepadInterface* Gamepad = Cast<ISonyGamepadInterface>(UDeviceContainerManager::Get()->GetLibraryInstance(ControllerId));
	if (!IsValid(Gamepad->_getUObject()))
	{
		return;
	}

	Gamepad->SetAcceleration(bEnableAccelerometer);
}

void USonyGamepadProxy::EnableGyroscopeValues(int32 ControllerId, bool bEnableGyroscope)
{
	ISonyGamepadInterface* Gamepad = Cast<ISonyGamepadInterface>(UDeviceContainerManager::Get()->GetLibraryInstance(ControllerId));
	if (!IsValid(Gamepad->_getUObject()))
	{
		return;
	}

	Gamepad->SetGyroscope(bEnableGyroscope);
}
