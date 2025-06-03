// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"
#include "DualSenseHIDManager.h"
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
		
		UE_LOG(LogTemp, Log, TEXT("DualSense: CreateLibraryInstances"));
		TArray<FHIDDeviceContext> DetectedDevices;
		DetectedDevices.Reset();

		// Enumera dispositivos usando o HIDManager
		if (DualSenseHIDManager HIDManager; !HIDManager.FindDevices(DetectedDevices) || DetectedDevices.Num() == 0)
		{
			FHIDDeviceContext Context;
			Context.Internal.Connected = false;
			if (UDualSenseLibrary* DualSense = NewObject<UDualSenseLibrary>())
			{
				DualSense->AddToRoot();
				DualSense->InitializeLibrary(Context);
				LibraryInstances.Add(0, DualSense);
			}
			UE_LOG(LogTemp, Warning, TEXT("DualSense: Nenhum dispositivo encontrado."));
			return;
		}

		// Itera sobre os dispositivos detectados para inicializar as bibliotecas
		for (int32 DeviceIndex = 0; DeviceIndex < DetectedDevices.Num(); DeviceIndex++)
		{
			FHIDDeviceContext& Context = DetectedDevices[DeviceIndex];
			UE_LOG(LogTemp, Warning, TEXT("DualSense: Inicializando biblioteca para dispositivo status %d"), Context.Internal.Connected);
			if (Context.Internal.Connected)
			{
				UE_LOG(LogTemp, Warning, TEXT("DualSense: IsConnected %d"), Context.Internal.Connected);
				UDualSenseLibrary* DualSense = NewObject<UDualSenseLibrary>();
				if (DualSense)
				{
					DualSense->AddToRoot();
					DualSense->InitializeLibrary(Context);

					if (!DualSense->IsConnected())
					{
						UE_LOG(LogTemp, Warning, TEXT("DualSense: Dispositivo não conectado. Finalizando biblioteca... %d"), DeviceIndex);
						DualSense->ShutdownLibrary();
						continue;
					}

					// Adiciona a instância à lista de bibliotecas
					LibraryInstances.Add(DeviceIndex, DualSense);
					UE_LOG(LogTemp, Log, TEXT("DualSense: Biblioteca inicializada para dispositivo %d"), DeviceIndex);
				}
			}
		}
	}

	static int32 GetAllocatedDevices()
	{
		return LibraryInstances.Num();
	}

	bool bIsBlockCreateInstance = false;
private:
	static UFDualSenseLibraryManager* Instance;
	static TMap<int32, UDualSenseLibrary*> LibraryInstances;
	
	// static UDualSenseLibrary* CreateLibraryInstance(int32 ControllerID)
	// {
	// 	DS5W::DeviceEnumInfo* Infos = new DS5W::DeviceEnumInfo[MAX_DEVICES];
	// 	
	// 	unsigned int Count = 0;
	// 	if (DS5W_OK != DS5W::enumDevices(Infos, MAX_DEVICES, &Count))
	// 	{
	// 		delete[] Infos;
	// 		return nullptr;
	// 	}
	//
	// 	if (Count == 0)
	// 	{
	// 		delete[] Infos;
	// 		return nullptr;
	// 	}
	//
	// 	if (static_cast<unsigned int>(ControllerID) >= Count)
	// 	{
	// 		delete[] Infos;
	// 		return nullptr;
	// 	}
	//
	// 	DS5W::DeviceContext Context;
	// 	if (!DS5W_SUCCESS(DS5W::initDeviceContext(&Infos[ControllerID], &Context)))
	// 	{
	// 		delete[] Infos;
	// 		return nullptr;
	// 	}
	//
	// 	delete[] Infos;
	//
	// 	UDualSenseLibrary* DualSense = NewObject<UDualSenseLibrary>();
	// 	if (!DualSense)
	// 	{
	// 		return nullptr;
	// 	}
	//
	// 	DualSense->AddToRoot();
	// 	// DualSense->InitializeLibrary(Context);
	//
	// 	if (!DualSense->IsConnected())
	// 	{
	// 		UE_LOG(LogTemp, Log, TEXT("DualSense: Device not connected. Shutdown library..."));
	// 		DualSense->ShutdownLibrary();
	// 		return nullptr;
	// 	}
	//
	// 	UE_LOG(LogTemp, Log, TEXT("DualSense: New device connected..."));
	// 	SetIsBlockCreateInstance(false);
	// 	return DualSense;
	// }
};
