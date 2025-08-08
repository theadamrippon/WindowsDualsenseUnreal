// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"
#include "FOutput.generated.h"

USTRUCT()
struct FAudioConfig
{
	GENERATED_BODY()

	uint8_t Mode = 0x5;
	uint8_t HeadsetVolume = 0x7C;
	uint8_t SpeakerVolume = 0x7C;
	uint8_t MicVolume = 0x7C;
	uint8_t MicStatus = 0x0;
};

USTRUCT()
struct FFeatureConfig
{
	GENERATED_BODY()

	uint8_t FeatureMode	  = 0xF7;
	uint8_t VibrationMode = 0xFC;
	uint8_t SoftRumbleReduce = 0x0F;
	uint8_t TriggerSoftnessLevel = 0x4;
};

USTRUCT()
struct FLightbar
{
	GENERATED_BODY()

	unsigned char R = 0x0;
	unsigned char G = 0x0;
	unsigned char B = 0x0;
	unsigned char A = 0x0;
};

USTRUCT()
struct FPlayerLed
{
	GENERATED_BODY()
	
	unsigned char Led = 0x4;
	unsigned char Brightness = 0x0;
};

USTRUCT()
struct FRumbles
{
	GENERATED_BODY()
	
	unsigned char Left = 0x0;
	unsigned char Right = 0x0;
};

USTRUCT()
struct FStrengths
{
	GENERATED_BODY()
	
	uint8_t  Period = 0;
	uint32_t ActiveZones = 0;
	uint32_t TimeAndRatio = 0;
	uint64_t StrengthZones = 0;
};

USTRUCT()
struct FHapticTriggers
{
	GENERATED_BODY()

	unsigned char Mode = 0x0;
	unsigned char Frequency = 0x0;
	unsigned char Amplitude = 0x0;
	unsigned char KeepEffect = 0x0;
	FStrengths Strengths;
};

USTRUCT()
struct FMicLight
{
	GENERATED_BODY()
	
	unsigned char Mode = 0x0;
};

USTRUCT()
struct FOutput
{
	GENERATED_BODY()
	
	FLightbar Lightbar;
	FMicLight MicLight;
	FPlayerLed PlayerLed;
	
	FRumbles Rumbles;
	FAudioConfig Audio;
	FFeatureConfig Feature;
	FHapticTriggers LeftTrigger;
	FHapticTriggers RightTrigger;
};
