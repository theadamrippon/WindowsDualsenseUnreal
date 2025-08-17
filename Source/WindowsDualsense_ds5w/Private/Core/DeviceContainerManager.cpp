// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "Core/DeviceContainerManager.h"

#include "SonyGamepadProxy.h"
#include "Core/DeviceHIDManager.h"
#include "Core/DualSense/DualSenseLibrary.h"
#include "Core/DualShock/DualShockLibrary.h"
#include "Core/Interfaces/SonyGamepadTriggerInterface.h"

UDeviceContainerManager* UDeviceContainerManager::Instance;
TMap<int32, ISonyGamepadInterface*> UDeviceContainerManager::LibraryInstances;

UDeviceContainerManager* UDeviceContainerManager::Get()
{
	if (!Instance)
	{
		Instance = NewObject<UDeviceContainerManager>();
		Instance->AddToRoot();
	}
	return Instance;
}

ISonyGamepadInterface* UDeviceContainerManager::GetLibraryOrReconnect(int32 ControllerId)
{
	if (LibraryInstances.Contains(ControllerId))
	{
		if (LibraryInstances[ControllerId]->IsConnected())
		{
			return LibraryInstances[ControllerId];
		}

		RemoveLibraryInstance(ControllerId); // destruct instance to reconnect
	}

	if (!LibraryInstances.Contains(ControllerId))
	{
		ISonyGamepadInterface* DSLibrary = CreateLibraryInstance(ControllerId);
		if (!DSLibrary)
		{
			return nullptr;
		}
		LibraryInstances.Add(ControllerId, DSLibrary);
	}

	LibraryInstances[ControllerId]->Reconnect();
	return LibraryInstances[ControllerId];
}

ISonyGamepadInterface* UDeviceContainerManager::GetLibraryInstance(int32 ControllerId)
{
	if (!LibraryInstances.Contains(ControllerId))
	{
		return nullptr;
	}

	if (!LibraryInstances[ControllerId]->IsConnected())
	{
		return nullptr;
	}

	return LibraryInstances[ControllerId];
}

void UDeviceContainerManager::RemoveLibraryInstance(int32 ControllerId)
{
	if (LibraryInstances.Contains(ControllerId))
	{
		LibraryInstances[ControllerId]->ShutdownLibrary();
		LibraryInstances.Remove(ControllerId);
	}
}

void UDeviceContainerManager::RemoveAllLibraryInstance()
{
	for (const auto& LibraryInstance : LibraryInstances)
	{
		LibraryInstance.Value->ShutdownLibrary();
	}
	LibraryInstances.Reset();
}

void UDeviceContainerManager::CreateLibraryInstances()
{
	LibraryInstances.Reset();

	TArray<FDeviceContext> DetectedDevices;
	DetectedDevices.Reset();

	UDeviceHIDManager* HIDManager = NewObject<UDeviceHIDManager>();
	if (!HIDManager->FindDevices(DetectedDevices) || DetectedDevices.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("SonyGamepad: device not found. Creating default library instance."));
		return;
	}

	if (DetectedDevices.Num() > 8)
	{
		return;
	}

	for (int32 DeviceIndex = 0; DeviceIndex < DetectedDevices.Num(); DeviceIndex++)
	{
		FDeviceContext& Context = DetectedDevices[DeviceIndex];
		Context.Output = FOutputContext();
		Context.Handle = UDeviceHIDManager::CreateHandle(&Context);
		if (Context.IsConnected)
		{
			ISonyGamepadInterface* SonyGamepad = nullptr;
			if (Context.DeviceType == EDeviceType::DualSense || Context.DeviceType == EDeviceType::DualSenseEdge)
			{
				SonyGamepad = Cast<ISonyGamepadInterface>(NewObject<UDualSenseLibrary>(UDualSenseLibrary::StaticClass()));
			}
			
			if (Context.DeviceType == EDeviceType::DualShock4)
			{
				SonyGamepad = Cast<ISonyGamepadInterface>(NewObject<UDualShockLibrary>(UDualShockLibrary::StaticClass()));
			}
			
			if (!SonyGamepad)
			{
				UE_LOG(LogTemp, Warning, TEXT("SonyGamepad: not found device shutdown library... %d"), DeviceIndex);
				SonyGamepad->ShutdownLibrary();
				continue;
			}
			
			SonyGamepad->_getUObject()->AddToRoot();
			SonyGamepad->SetControllerId(DeviceIndex);
			SonyGamepad->InitializeLibrary(Context);
			
			LibraryInstances.Add(DeviceIndex, SonyGamepad);
		}
	}
}

int32 UDeviceContainerManager::GetAllocatedDevices()
{
	return LibraryInstances.Num();
}

ISonyGamepadInterface* UDeviceContainerManager::CreateLibraryInstance(int32 ControllerID)
{
	TArray<FDeviceContext> DetectedDevices;
	DetectedDevices.Reset();

	UDeviceHIDManager* HIDManager = NewObject<UDeviceHIDManager>();
	if (!HIDManager->FindDevices(DetectedDevices) || DetectedDevices.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("SonyGamepad: device not found. Creating default library instance."));
		return nullptr;
	}

	if (ControllerID >= DetectedDevices.Num())
	{
		return nullptr;
	}
	
	FDeviceContext& Context = DetectedDevices[ControllerID];
	if (Context.IsConnected)
	{
		ISonyGamepadInterface*  SonyGamepad = nullptr;
		Context.Output = FOutputContext();
		Context.Handle = UDeviceHIDManager::CreateHandle(&Context);
		if (Context.DeviceType == EDeviceType::DualSense || Context.DeviceType == EDeviceType::DualSenseEdge)
		{
			SonyGamepad = Cast<ISonyGamepadInterface>(NewObject<UDualSenseLibrary>(UDualSenseLibrary::StaticClass()));
		}

		if (Context.DeviceType == EDeviceType::DualShock4)
		{
			SonyGamepad = Cast<ISonyGamepadInterface>(NewObject<UDualShockLibrary>(UDualShockLibrary::StaticClass()));
		}
		
		if (!SonyGamepad)
		{
			return nullptr;
		}

		SonyGamepad->_getUObject()->AddToRoot();
		SonyGamepad->SetControllerId(ControllerID);
		SonyGamepad->InitializeLibrary(Context);
		return SonyGamepad;
	}
	return nullptr;
}