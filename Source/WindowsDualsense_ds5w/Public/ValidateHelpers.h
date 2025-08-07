// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ValidateHelpers.generated.h"
/**
 * 
 */
UCLASS()
class WINDOWSDUALSENSE_DS5W_API UValidateHelpers : public UObject
{
	GENERATED_BODY()
public:
	static bool ValidateMaxForce(const int32 Strength)
	{
		return Strength <= 8 && Strength >= 0;
	}

	static bool ValidateMaxPosition(const int32 Position)
	{
		return Position <= 8 && Position >= 0;
	}

	static bool ValidateMaxFrequency(const float Frequency)
	{
		return Frequency <= 1.0 && Frequency >= 0.0;
	}
};
