// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"
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
	TSharedPtr<FDualSenseInputDevice> DeviceInstance;
	static void RegisterCustomKeys();
	void PrintBufferAsHex(const unsigned char* Buffer, int BufferSize);
};
