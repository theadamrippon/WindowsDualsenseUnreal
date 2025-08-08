// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"
#include "FOutput.h"
#include "Core/Enums/EDeviceConnection.h"
#include "FDeviceContext.generated.h"

USTRUCT()
struct FDeviceContext
{
	GENERATED_BODY()

	void* Handle;
	wchar_t Path[260];
	unsigned char Buffer[78];
	bool IsConnected;
	
	FOutput Output;
	EDeviceConnection ConnectionType;
};
