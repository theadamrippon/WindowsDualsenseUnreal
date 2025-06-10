// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"
#include "DualSenseHIDManager.h"
#include "DualSenseLibrary.h"
#include "UObject/Object.h"
#include "Misc/CoreDelegates.h"
#include "FDualSenseLibraryManager.generated.h"

#define MAX_DEVICES 16


/**
 * 
 */
UCLASS()
class WINDOWSDUALSENSE_DS5W_API UFDualSenseLibraryManager : public UObject
{
	GENERATED_BODY()

public:
	
	virtual ~UFDualSenseLibraryManager() override
	{
		UE_LOG(LogTemp, Log, TEXT("DualSense: Destruct UFDualSenseLibraryManager"));
	}
	
	static UFDualSenseLibraryManager* Get()
	{
		if (!Instance)
		{
			UE_LOG(LogTemp, Log, TEXT("DualSense: Creating new instance UFDualSenseLibraryManager"));
			Instance = NewObject<UFDualSenseLibraryManager>();
		}
		return Instance;
	}

	static bool GetIsBlockCreateInstance()
	{
		return Instance->bIsBlockCreateInstance;
	}

	static FGenericPlatformInputDeviceMapper PlatformInputDeviceMapper;

	void static SetIsBlockCreateInstance(const bool IsBlock)
	{
		Instance->bIsBlockCreateInstance = IsBlock;
	}

	static bool GetIsBroadcasting()
	{
		return Instance->bIsBroadcasting;
	}

	void static SetIsBroadcasting(const bool IsBlock)
	{
		Instance->bIsBroadcasting = IsBlock;
	}

	
	static UDualSenseLibrary* GetLibraryInstance(int32 ControllerId)
	{
		if (!LibraryInstances.Contains(ControllerId))
		{
			if (!GetIsBlockCreateInstance() && !GetIsBroadcasting())
			{
				UDualSenseLibrary* DSLibrary = CreateLibraryInstance(ControllerId);
				if (DSLibrary)
				{
					PlatformInputDeviceMapper.Get().GetOnInputDeviceConnectionChange().Broadcast(EInputDeviceConnectionState::Connected, FPlatformUserId::CreateFromInternalId(ControllerId), FInputDeviceId::CreateFromInternalId(ControllerId));
					SetIsBroadcasting(false);
					SetIsBlockCreateInstance(false);
					return DSLibrary;
				}
				SetIsBroadcasting(false);
				SetIsBlockCreateInstance(false);
			}
			return nullptr;
		}
		
		return LibraryInstances[ControllerId];
	}

	static void RemoveLibraryInstance(int32 ControllerId)
	{
		if (LibraryInstances.Contains(ControllerId))
		{
			UE_LOG(LogTemp, Log, TEXT("DualSense: ShutdownLibrary UDualSenseLibrary ControllerId, %d"), ControllerId);
			LibraryInstances[ControllerId]->ShutdownLibrary();
			LibraryInstances.Remove(ControllerId);
		}
	}

	static void RemoveAllLibraryInstance()
	{
		for (const auto& LibraryInstance : LibraryInstances)
		{
			LibraryInstance.Value->ShutdownLibrary();
		}
		
		LibraryInstances.Reset();
	}


	static void CreateLibraryInstances()
	{
		LibraryInstances.Reset();
		
		TArray<FHIDDeviceContext> DetectedDevices;
		DetectedDevices.Reset();

		if (DualSenseHIDManager HIDManager; !HIDManager.FindDevices(DetectedDevices) || DetectedDevices.Num() == 0)
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
			
			UE_LOG(LogTemp, Log, TEXT("DualSense: init device isConnected %d"), Context.Internal.Connected);
			if (Context.Internal.Connected)
			{
				UDualSenseLibrary* DualSense = NewObject<UDualSenseLibrary>();
				if (DualSense)
				{
					DualSense->AddToRoot();
					DualSense->ControllerID = DeviceIndex;
					DualSense->InitializeLibrary(Context);

					if (!DualSense->IsConnected())
					{
						UE_LOG(LogTemp, Warning, TEXT("DualSense: not found device shutdown library... %d"), DeviceIndex);
						PlatformInputDeviceMapper.Get().GetOnInputDeviceConnectionChange().Broadcast(EInputDeviceConnectionState::Disconnected, FPlatformUserId::CreateFromInternalId(DeviceIndex), FInputDeviceId::CreateFromInternalId(DeviceIndex));
						DualSense->ShutdownLibrary();
						continue;
					}

					LibraryInstances.Add(DeviceIndex, DualSense);
					UE_LOG(LogTemp, Log, TEXT("DualSense: Library initialized deviceId %d"), DeviceIndex);
				}
			}
		}
		SetIsBroadcasting(false);
		SetIsBlockCreateInstance(false);
	}

	static int32 GetAllocatedDevices()
	{
		return LibraryInstances.Num();
	}
	 
	bool bIsBlockCreateInstance = true;
	bool bIsBroadcasting = true;
private:
	static UFDualSenseLibraryManager* Instance;
	static TMap<int32, UDualSenseLibrary*> LibraryInstances;

	bool IsNewInstance = false;
	static UDualSenseLibrary* CreateLibraryInstance(int32 ControllerID)
	{
		if (GetAllocatedDevices() == ControllerID && ControllerID <= 0)
		{
			Get()->RemoveAllLibraryInstance();
		}

		if (GetIsBlockCreateInstance() || GetIsBroadcasting())
		{
			return nullptr;
		}

		TArray<FHIDDeviceContext> DetectedDevices;
		DetectedDevices.Reset();
		
		if (DualSenseHIDManager HIDManager; !HIDManager.FindDevices(DetectedDevices) || DetectedDevices.Num() == 0)
		{
			LibraryInstances.Reset();
			PlatformInputDeviceMapper.Get().GetOnInputDeviceConnectionChange().Broadcast(EInputDeviceConnectionState::Disconnected, FPlatformUserId::CreateFromInternalId(ControllerID), FInputDeviceId::CreateFromInternalId(ControllerID));
			UE_LOG(LogTemp, Error, TEXT("DualSense: device not found. Creating default library instance."));
			return nullptr;
		}

			
		FHIDDeviceContext& Context = DetectedDevices[ControllerID];
		UE_LOG(LogTemp, Log, TEXT("DualSense: init device isConnected %d"), Context.Internal.Connected);
			
		if (Context.Internal.Connected)
		{
			UDualSenseLibrary* DualSense = NewObject<UDualSenseLibrary>();
			if (!DualSense)
			{
				return nullptr;
			}

			DualSense->AddToRoot();
			DualSense->InitializeLibrary(Context);

			if (!DualSense->IsConnected())
			{
				UE_LOG(LogTemp, Warning, TEXT("DualSense: not found device shutdown library... %d"), ControllerID);
				DualSense->ShutdownLibrary();
				PlatformInputDeviceMapper.Get().GetOnInputDeviceConnectionChange().Broadcast(EInputDeviceConnectionState::Disconnected, FPlatformUserId::CreateFromInternalId(ControllerID), FInputDeviceId::CreateFromInternalId(ControllerID));
				return nullptr;
			}

			if (!LibraryInstances.Contains(ControllerID))
			{
				LibraryInstances.Add(ControllerID, DualSense);
			}
			
			return LibraryInstances[ControllerID];
		}
		return nullptr; 
	}
};
