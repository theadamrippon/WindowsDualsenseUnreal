// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"
#include "EDeviceCommons.generated.h"


// Function Buttons
#define BTN_FN1          0x10
#define BTN_FN2          0x20
#define BTN_PADDLE_LEFT  0x40
#define BTN_PADDLE_RIGHT 0x80

// DPad
#define BTN_DPAD_UP 0x8
#define BTN_DPAD_DOWN 0x02
#define BTN_DPAD_LEFT 0x01
#define BTN_DPAD_RIGHT 0x04

// Face buttons
#define BTN_CROSS 0x20
#define BTN_SQUARE 0x10
#define BTN_CIRCLE 0x40
#define BTN_TRIANGLE 0x80

#define BTN_LEFT_SHOLDER 0x01
#define BTN_RIGHT_SHOLDER 0x02
#define BTN_LEFT_TRIGGER 0x04
#define BTN_RIGHT_TRIGGER 0x08
#define BTN_LEFT_STICK 0x40
#define BTN_RIGHT_STICK 0x80
#define BTN_START 0x20
#define BTN_SELECT 0x10

#define BTN_PLAYSTATION_LOGO 0x01
#define BTN_PAD_BUTTON 0x02
#define BTN_MIC_BUTTON 0x04

#define PLAYER_LED_LEFT 0x01
#define PLAYER_LED_MIDDLE_LEFT 0x02
#define PLAYER_LED_MIDDLE 0x04
#define PLAYER_LED_MIDDLE_RIGHT 0x08
#define PLAYER_LED_RIGHT 0x10

UENUM(BlueprintType)
enum class ELedMicEnum : uint8
{
	MicOff = 0x0 UMETA(DisplayName = "Mic Off"),
	MicOn = 0x1 UMETA(DisplayName = "Mic On"),
	Pulse = 0x2 UMETA(DisplayName = "Pulse")
};

UENUM(BlueprintType)
enum class ELedPlayerEnum : uint8
{
	Off = 0x0 UMETA(DisplayName = "Led Player Off"),
	One = PLAYER_LED_MIDDLE UMETA(DisplayName = "Player One"),
	Two = PLAYER_LED_MIDDLE_RIGHT | PLAYER_LED_MIDDLE_LEFT UMETA(DisplayName = "Player Two"),
	Three = PLAYER_LED_RIGHT | PLAYER_LED_MIDDLE | PLAYER_LED_LEFT UMETA(DisplayName = "Player Three"),
	Four = PLAYER_LED_RIGHT | PLAYER_LED_MIDDLE_RIGHT | PLAYER_LED_MIDDLE_LEFT | PLAYER_LED_LEFT UMETA(
		DisplayName = "Player Four"),
	All = 31 UMETA(DisplayName = "All")
};

UENUM(BlueprintType)
enum class ELedBrightnessEnum : uint8
{
	Low = 0x02 UMETA(DisplayName = "Brightness Low"),
	Medium = 0x01 UMETA(DisplayName = "Brightness Medium"),
	High = 0x00 UMETA(DisplayName = "Brightness High")
};

UENUM(BlueprintType)
enum class EDualSenseAudioFeatureReport : uint8
{
	On = 0x00 UMETA(DisplayName = "Audio On"),
	Off = 0x10 UMETA(DisplayName = "Audio Off")
};

UENUM(BlueprintType)
enum class EDualSenseDeviceFeatureReport : uint8
{
	Off = 0,
	DefaultRumble = 0xFF UMETA(DisplayName = "Default Rumble"),
	HapticSoftRumble = 0xFC UMETA(DisplayName = "Haptic Soft Rumble")
};

UENUM(BlueprintType)
enum class EDualSenseTriggerSoftnessLevel : uint8
{
	Off = 0,
	Soft = 6 UMETA(DisplayName = "Soft"),
	Rigid = 2 UMETA(DisplayName = "Rigid"),
	Medium = 4 UMETA(DisplayName = "Medium"),
	VerySoft = 8 UMETA(DisplayName = "Very Soft"),
	VeryRigid = 1 UMETA(DisplayName = "Very Rigid")
};

USTRUCT(BlueprintType)
struct FDeviceSettings
{
	GENERATED_BODY()
	
};

USTRUCT(BlueprintType)
struct FDualSenseFeatureReport : public FDeviceSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DualSense Settings")
	EDualSenseAudioFeatureReport MicStatus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DualSense Settings")
	EDualSenseAudioFeatureReport AudioHeadset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DualSense Settings")
	EDualSenseAudioFeatureReport AudioSpeaker;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DualSense Settings",
		meta=(DisplayName = "Soft haptic feedback, adivanced vibrate"))
	EDualSenseDeviceFeatureReport VibrationMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DualSense Settings",
		meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
	int32 MicVolume;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DualSense Settings",
		meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
	int32 AudioVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DualSense Settings",
		meta = (ClampMin = "0", ClampMax = "15", UIMin = "0", UIMax = "15"))
	int32 SoftRumbleReduce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DualSense Settings",
		meta = (ClampMin = "0", ClampMax = "15", UIMin = "0", UIMax = "15"))
	EDualSenseTriggerSoftnessLevel TriggerSoftnessLevel;

	FDualSenseFeatureReport();
};


inline FDualSenseFeatureReport::FDualSenseFeatureReport():
	MicStatus(EDualSenseAudioFeatureReport::Off)
	, AudioHeadset(EDualSenseAudioFeatureReport::Off)
	, AudioSpeaker(EDualSenseAudioFeatureReport::On)
	, VibrationMode(EDualSenseDeviceFeatureReport::DefaultRumble)
	, MicVolume(0)
	, AudioVolume(0)
	, SoftRumbleReduce(0)
	, TriggerSoftnessLevel(EDualSenseTriggerSoftnessLevel::Medium)
{
}
