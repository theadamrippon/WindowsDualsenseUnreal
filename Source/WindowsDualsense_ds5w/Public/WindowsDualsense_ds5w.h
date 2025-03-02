// Copyright Epic Games, Inc. All Rights Reserved.

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

private:
	void* DS5WdllHandle = nullptr;
	TSharedPtr<FDualSenseInputDevice> DeviceInstances;
	UDualSenseLibrary* DualSenseLibraryInstance = nullptr;

	static void RegisterCustomKeys();
};
