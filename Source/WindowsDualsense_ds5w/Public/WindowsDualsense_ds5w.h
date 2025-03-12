// Copyright (c) 2025 Rafael Valotor/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"
#include "DualSenseLibrary.h"
#include "FDualSenseInputDevice.h"
#include "IInputDeviceModule.h"
#include "InputCoreTypes.h"



class FWindowsDualsense_ds5wModule : public IInputDeviceModule
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual TSharedPtr<IInputDevice> CreateInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InCustomMessageHandler) override;
	void RegisterDevice(int32 ControllerId);
	
private:
	void* DS5WdllHandle = nullptr;
	TSharedPtr<FDualSenseInputDevice> DeviceInstances;
	UDualSenseLibrary* DualSenseLibraryInstance = nullptr;
	

	static void RegisterCustomKeys();
};
