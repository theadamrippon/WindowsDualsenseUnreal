// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"
#include "DualSenseLibrary.h"
#include "UObject/Object.h"
#include "FDualSenseLibraryManager.generated.h"

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
		RemoveAllLibraryInstance();
	}
	
	static UFDualSenseLibraryManager* Get()
	{
		return NewObject<UFDualSenseLibraryManager>();
	}

	UDualSenseLibrary* GetLibraryInstance(int32 ControllerId)
	{
		if (!LibraryInstances.Contains(ControllerId))
		{
			LibraryInstances.Add(ControllerId, CreateLibraryInstance(ControllerId));
		}

		return LibraryInstances[ControllerId].Get();
	}

	void RemoveLibraryInstance(int32 ControllerId)
	{
		if (LibraryInstances.Contains(ControllerId))
		{
			LibraryInstances.Remove(ControllerId);
		}
	}

	static void RemoveAllLibraryInstance()
	{
		LibraryInstances.Reset();
	}


	static void CreateLibraryInstances()
	{
		DS5W::DeviceEnumInfo* Infos = new DS5W::_DeviceEnumInfo[4];
		unsigned int Count = 0;

		if (DS5W_OK != DS5W::enumDevices(Infos, 4, &Count))
		{
			UE_LOG(LogTemp, Error, TEXT("Error enumDevices"));
			delete[] Infos;
			return;
		}

		if (Count == 0)
		{
			UE_LOG(LogTemp, Log, TEXT("Error Count %d"), Count);
			delete[] Infos;
			return;
		}

		for (unsigned int i = 0; i < Count; i++)
		{
			UE_LOG(LogTemp, Log, TEXT("Devices Count %d"), i);
			
			DS5W::DeviceContext* Context = new DS5W::DeviceContext(); // Criado dinamicamente
			if (DS5W_SUCCESS(DS5W::initDeviceContext(&Infos[i], Context)))
			{
				UE_LOG(LogTemp, Log, TEXT("Devices initDeviceContext %d"), i);
				UDualSenseLibrary* DualSense = NewObject<UDualSenseLibrary>();

				if (DualSense->InitializeLibrary(*Context))
				{
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

private:
	static TMap<int, TObjectPtr<UDualSenseLibrary>> LibraryInstances;

	static TObjectPtr<UDualSenseLibrary> CreateLibraryInstance(int32 ControllerID)
	{
		DS5W::DeviceEnumInfo* Infos = new DS5W::DeviceEnumInfo[4];
		unsigned int Count = 0;
		
		if (DS5W_OK != DS5W::enumDevices(Infos, 8, &Count))
		{
			delete[] Infos;
			return nullptr;
		}

		if (Count == 0)
		{
			delete[] Infos;
			return nullptr;
		}

		if (ControllerID < 0 || static_cast<unsigned int>(ControllerID) >= Count)
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
		if (!DualSense->InitializeLibrary(Context))
		{
			return nullptr;
		}

		return DualSense;
	}
};
