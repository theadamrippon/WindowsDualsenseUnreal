// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Core/Enums/EDeviceCommons.h"
#include "Core/Structs/FDeviceContext.h"
#include "SonyGamepadInterface.generated.h"


UINTERFACE()
class USonyGamepadInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WINDOWSDUALSENSE_DS5W_API ISonyGamepadInterface
{
	GENERATED_BODY()

public:
	virtual void Reconnect() = 0;
	virtual bool IsConnected() = 0;

	virtual void Settings(FDeviceSettings& Settings) = 0;
	virtual bool InitializeLibrary(const FDeviceContext& Context) = 0;
	virtual void ShutdownLibrary() = 0;
	
	virtual void SendOut() = 0;
};
