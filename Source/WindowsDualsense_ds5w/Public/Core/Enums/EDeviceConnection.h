// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"
#include "EDeviceConnection.generated.h"

UENUM()
enum EDeviceType: uint8
{
	Default UMETA(DisplayName = "Default"),
	Edge UMETA(DisplayName = "Edge"),
	DualShock UMETA(DisplayName = "DualShock")
};

UENUM()
enum EDeviceConnection: uint8
{
	Usb UMETA(DisplayName = "USB"),
	Bluetooth UMETA(DisplayName = "Bluetooth"),
	Unknown
};
