// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class WINDOWSDUALSENSE_DS5W_API FValidationUtils
{
public:
	FValidationUtils();
	~FValidationUtils();

	static bool ValidateMaxForce(int32 Force)
	{
		return Force <= 8 && Force >= 0;
	}

	static bool ValidateMaxPosition(int32 Position)
	{
		return Position <= 9 && Position >= 0;
	}

	static bool ValidateMaxFrequency(float Frequency)
	{
		return Frequency <= 1.0 && Frequency >= 0.0;
	}
};
