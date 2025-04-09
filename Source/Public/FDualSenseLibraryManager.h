// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"
#include "DualSenseLibrary.h"
#include "UObject/Object.h"
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

	void static SetIsBlockCreateInstance(const bool IsBlock)
	{
		Instance->bIsBlockCreateInstance = IsBlock;
	}
	
	static UDualSenseLibrary* GetLibraryInstance(int32 ControllerId)
	{
		if (!LibraryInstances.Contains(ControllerId))
		{
			if (GetIsBlockCreateInstance())
			{
				return nullptr;
			}
			
			SetIsBlockCreateInstance(true);
			if (ControllerId == -1)
			{
				ControllerId = 0;
				if (LibraryInstances.Num() >= 1)
				{
					ControllerId = LibraryInstances.Num() - 1;	
				}
			}

			if (LibraryInstances.Num() == 0)
			{
				ControllerId = 0;
			}

			UDualSenseLibrary* DsNew = CreateLibraryInstance(ControllerId);
			if (!DsNew)
			{
				SetIsBlockCreateInstance(false);
				return nullptr;
			}
			
			LibraryInstances.Add(ControllerId, DsNew);
			UE_LOG(LogTemp, Log, TEXT("DualSense: Creating new instance for controller %d"), ControllerId);
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
		DS5W::DeviceEnumInfo* Infos = new DS5W::_DeviceEnumInfo[MAX_DEVICES];
		unsigned int Count = 0;

		if (DS5W_OK != DS5W::enumDevices(Infos, MAX_DEVICES, &Count))
		{
			delete[] Infos;
			return;
		}

		if (Count == 0)
		{
			delete[] Infos;
			return;
		}

		for (unsigned int i = 0; i < Count; i++)
		{
			DS5W::DeviceContext Context;
			if (DS5W_SUCCESS(DS5W::initDeviceContext(&Infos[i], &Context)))
			{
				UE_LOG(LogTemp, Log, TEXT("DualSense: Devices init context %d"), i);
				if (UDualSenseLibrary* DualSense = NewObject<UDualSenseLibrary>())
				{
					DualSense->AddToRoot();
					DualSense->InitializeLibrary(Context);

					if (!DualSense->IsConnected())
					{
						UE_LOG(LogTemp, Log, TEXT("DualSense: Device not connected. Shutdown library... %d"), i);
						DualSense->ShutdownLibrary();
						continue;
					}
					
					LibraryInstances.Add(i,DualSense);
				}
			}
		}

		delete[] Infos;
	}

	static int32 GetAllocatedDevices()
	{
		return LibraryInstances.Num();
	}

	bool bIsBlockCreateInstance = false;
private:
	static UFDualSenseLibraryManager* Instance;
	static TMap<int32, UDualSenseLibrary*> LibraryInstances;
	
	static UDualSenseLibrary* CreateLibraryInstance(int32 ControllerID)
	{
		DS5W::DeviceEnumInfo* Infos = new DS5W::DeviceEnumInfo[MAX_DEVICES];
		
		unsigned int Count = 0;
		if (DS5W_OK != DS5W::enumDevices(Infos, MAX_DEVICES, &Count))
		{
			delete[] Infos;
			return nullptr;
		}

		if (Count == 0)
		{
			delete[] Infos;
			return nullptr;
		}

		if (static_cast<unsigned int>(ControllerID) >= Count)
		{
			delete[] Infos;
			return nullptr;
		}

		DS5W::DeviceContext Context;
		if (!DS5W_SUCCESS(DS5W::initDeviceContext(&Infos[ControllerID], &Context)))
		{
			delete[] Infos;
			return nullptr;
		}

		delete[] Infos;

		UDualSenseLibrary* DualSense = NewObject<UDualSenseLibrary>();
		if (!DualSense)
		{
			return nullptr;
		}

		DualSense->AddToRoot();
		DualSense->InitializeLibrary(Context);

		if (!DualSense->IsConnected())
		{
			UE_LOG(LogTemp, Log, TEXT("DualSense: Device not connected. Shutdown library..."));
			DualSense->ShutdownLibrary();
			return nullptr;
		}

		UE_LOG(LogTemp, Log, TEXT("DualSense: New device connected..."));
		SetIsBlockCreateInstance(false);
		return DualSense;
	}
};
