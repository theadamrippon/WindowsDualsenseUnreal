// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"
#include "DualSenseHIDManager.h"
#include "DualSenseLibrary.h"
#include "UObject/Object.h"
#include "Misc/CoreDelegates.h"
#include "DualSenseLibraryManager.generated.h"

#define MAX_DEVICES 16


/**
 * 
 */
UCLASS()
class WINDOWSDUALSENSE_DS5W_API UDualSenseLibraryManager : public UObject
{
	GENERATED_BODY()

public:
	virtual ~UDualSenseLibraryManager() override;

	static UDualSenseLibraryManager* Get();
	static UDualSenseLibrary* GetLibraryInstance(int32 ControllerId);
	static UDualSenseLibrary* GetLibraryOrReconnect(int32 ControllerId);
	static int32 GetAllocatedDevices();
	static void CreateLibraryInstances();
	static void RemoveAllLibraryInstance();
	static void RemoveLibraryInstance(int32 ControllerId);

	static FGenericPlatformInputDeviceMapper PlatformInputDeviceMapper;

private:
	static UDualSenseLibraryManager* Instance;
	static TMap<int32, UDualSenseLibrary*> LibraryInstances;
	static UDualSenseLibrary* CreateLibraryInstance(int32 ControllerID);
};
