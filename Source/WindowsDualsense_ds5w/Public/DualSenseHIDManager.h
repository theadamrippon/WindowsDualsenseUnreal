// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include <concrt.h>

#include "CoreMinimal.h"
#include "Core/Structs/FDeviceContext.h"
#include "DualSenseHIDManager.generated.h"

UCLASS()
class WINDOWSDUALSENSE_DS5W_API UDualSenseHIDManager : public UObject
{
	GENERATED_BODY()
	
	const static UINT32 HashTable[256];
	const static UINT32 CRCSeed;

public:
	UDualSenseHIDManager();

	static void FreeContext(FDeviceContext* Context);
	static bool FindDevices(TArray<FDeviceContext>& Devices);
	static HANDLE CreateHandle(FDeviceContext* DeviceContext);
	static void OutputBuffering(FDeviceContext* DeviceContext);
	static bool GetDeviceInputState(FDeviceContext* DeviceContext);
	static void SetTriggerEffects(unsigned char* Trigger, FHapticTriggers& Effect);
	static UINT32 Compute(const unsigned char* Buffer, size_t Len);
};
