// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025


#include "DualSenseLibraryManager.h"

UDualSenseLibraryManager* UDualSenseLibraryManager::Instance;
TMap<int32, UDualSenseLibrary*> UDualSenseLibraryManager::LibraryInstances;

UDualSenseLibraryManager::~UDualSenseLibraryManager()
{
	UE_LOG(LogTemp, Log, TEXT("DualSense: Destruct UFDualSenseLibraryManager"));
}

UDualSenseLibraryManager* UDualSenseLibraryManager::Get()
{
	if (!Instance)
	{
		Instance = NewObject<UDualSenseLibraryManager>();
		Instance->AddToRoot();
	}
	return Instance;
}

UDualSenseLibrary* UDualSenseLibraryManager::GetLibraryOrReconnect(int32 ControllerId)
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
		UDualSenseLibrary* DSLibrary = CreateLibraryInstance(ControllerId);
		if (!DSLibrary)
		{
			return nullptr;
		}
		LibraryInstances.Add(ControllerId, DSLibrary);
	}

	if (!LibraryInstances[ControllerId]->Reconnect())
	{
		return nullptr;
	}

	return LibraryInstances[ControllerId];
}

UDualSenseLibrary* UDualSenseLibraryManager::GetLibraryInstance(int32 ControllerId)
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

void UDualSenseLibraryManager::RemoveLibraryInstance(int32 ControllerId)
{
	if (LibraryInstances.Contains(ControllerId))
	{
		LibraryInstances[ControllerId]->ShutdownLibrary();
		LibraryInstances.Remove(ControllerId);
	}
}

void UDualSenseLibraryManager::RemoveAllLibraryInstance()
{
	for (const auto& LibraryInstance : LibraryInstances)
	{
		LibraryInstance.Value->ShutdownLibrary();
	}
	LibraryInstances.Reset();
}

void UDualSenseLibraryManager::CreateLibraryInstances()
{
	LibraryInstances.Reset();

	TArray<FHIDDeviceContext> DetectedDevices;
	DetectedDevices.Reset();

	UDualSenseHIDManager* HIDManager = NewObject<UDualSenseHIDManager>();
	if (!HIDManager->FindDevices(DetectedDevices) || DetectedDevices.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("DualSense: device not found. Creating default library instance."));
		return;
	}

	if (DetectedDevices.Num() > MAX_DEVICES)
	{
		return;
	}

	for (int32 DeviceIndex = 0; DeviceIndex < DetectedDevices.Num(); DeviceIndex++)
	{
		FHIDDeviceContext& Context = DetectedDevices[DeviceIndex];
		Context.Internal.Output = FHIDOutput();
		UE_LOG(LogTemp, Log, TEXT("DualSense: Library path deviceId %d, %s"), DeviceIndex, Context.Internal.DevicePath);
		UE_LOG(LogTemp, Log, TEXT("DualSense: Library initialized deviceId %d"), DeviceIndex);
		if (Context.Internal.Connected)
		{
			Context.Internal.DeviceHandle = UDualSenseHIDManager::CreateHandle(&Context);
			UDualSenseLibrary* DualSense  = NewObject<UDualSenseLibrary>();
			if (!DualSense)
			{
				UE_LOG(LogTemp, Warning, TEXT("DualSense: not found device shutdown library... %d"), DeviceIndex);
				DualSense->ShutdownLibrary();
				continue;
			}

			DualSense->AddToRoot();
			DualSense->ControllerID = DeviceIndex;
			DualSense->InitializeLibrary(Context);

			LibraryInstances.Add(DeviceIndex, DualSense);
			UE_LOG(LogTemp, Log, TEXT("DualSense: Library path deviceId %d, %s"), DeviceIndex, Context.Internal.DevicePath);
			UE_LOG(LogTemp, Log, TEXT("DualSense: Library initialized deviceId %d"), DeviceIndex);
		}
	}
}

int32 UDualSenseLibraryManager::GetAllocatedDevices()
{
	return LibraryInstances.Num();
}

UDualSenseLibrary* UDualSenseLibraryManager::CreateLibraryInstance(int32 ControllerID)
{
	TArray<FHIDDeviceContext> DetectedDevices;
	DetectedDevices.Reset();

	UDualSenseHIDManager* HIDManager = NewObject<UDualSenseHIDManager>();
	if (!HIDManager->FindDevices(DetectedDevices) || DetectedDevices.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("DualSense: device not found. Creating default library instance."));
		return nullptr;
	}

	if (ControllerID >= DetectedDevices.Num())
	{
		return nullptr;
	}
	
	FHIDDeviceContext& Context = DetectedDevices[ControllerID];
	if (Context.Internal.Connected)
	{
		Context.Internal.DeviceHandle = UDualSenseHIDManager::CreateHandle(&Context);
		UDualSenseLibrary* DualSense = NewObject<UDualSenseLibrary>();
		if (!DualSense)
		{
			return nullptr;
		}

		DualSense->AddToRoot();
		DualSense->ControllerID = ControllerID;
		DualSense->InitializeLibrary(Context);
		return DualSense;
	}
	return nullptr;
}
