// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once
#include "CoreMinimal.h"


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

enum class EHIDDeviceConnection : uint8
{
	Usb,
	Bluetooth,
	Unknown
};

typedef struct FHIDDeviceContext
{
	struct
	{
		void* DeviceHandle;
		wchar_t DevicePath[260];
		unsigned char Buffer[547];

		bool Connected;
		EHIDDeviceConnection Connection;
	} Internal;

} FHIDDeviceContext;


struct FOutputBuffer
{
	typedef struct FLedPlayer
	{
		unsigned char Brightness = 0x00;
		unsigned char Player = 0x02;
		unsigned char Led = PLAYER_LED_MIDDLE;
	} FLedPlayer;
	FLedPlayer LedPlayerHid;

	typedef struct FMicLed
	{
		unsigned char Mode = 0x00;
	} FMicLed;
	FMicLed MicLed;

	typedef struct FLedColor
	{
		unsigned char R = 0x00;
		unsigned char G = 0x00;
		unsigned char B = 0xff;
		unsigned char A = 0xff;
	} FLedColor;
	FLedColor ColorHid;

	typedef struct FMotors
	{
		unsigned char Left = 0x00;
		unsigned char Right = 0x00;
	} FMotors;
	FMotors MotorsHid;

	typedef struct FLeftTriggerEffects
	{
		unsigned char Mode = 0x05;
		unsigned char StartPosition = 0x00;
		unsigned char EndPosition = 0x00;
		unsigned char Frequency = 0x00;
		unsigned char TimeRatio = 0x00;
		unsigned char KeepEffect = 0x00;

		typedef struct FStrengths
		{
			uint8_t  Period = 0;
			uint32_t ActiveZones = 0;
			uint32_t TimeAndRatio = 0;
			uint64_t StrengthZones = 0;
			unsigned char Start = 0x00;
			unsigned char Middle = 0x00;
			unsigned char End = 0x00;
		} FStrengths;
		FStrengths Strengths;
		
	} FLeftTriggerEffects;
	FLeftTriggerEffects LeftTrigger;

	typedef struct FRightTriggerEffects
	{
		unsigned char Mode = 0x05;
		unsigned char StartPosition = 0x00;
		unsigned char EndPosition = 0x00;
		unsigned char Frequency = 0x00;
		unsigned char TimeRatio = 0x00;
		unsigned char KeepEffect = 0x00;

		typedef struct FStrengths
		{
			uint8_t  Period = 0;
			uint32_t ActiveZones = 0;
			uint32_t TimeAndRatio = 0;
			uint64_t StrengthZones = 0;
			unsigned char Start = 0x00;
			unsigned char Middle = 0x00;
			unsigned char End = 0x00;
		} FStrengths;
		FStrengths Strengths;
		
	} FRightTriggerEffects;
	FRightTriggerEffects RightTrigger;
	
	bool ResetEffects = false;
	bool ResetLedEffects = false;
	bool ResetEffectsLeftTrigger = false;
	bool ResetEffectsRightTrigger = false;
};


/**
 * 
 */
class WINDOWSDUALSENSE_DS5W_API DualSenseHIDManager
{
	const static UINT32 HashTable[256];
	const static UINT32 CRCSeed;
public:
	DualSenseHIDManager();
	~DualSenseHIDManager();

	
	bool FindDevices(TArray<FHIDDeviceContext>& Devices);
	static bool ReconnectDevice(FHIDDeviceContext* DeviceContext);
	static void FreeContext(FHIDDeviceContext* Context);
	static void OutputBuffering(FHIDDeviceContext* Context, const FOutputBuffer& HidOut);
	static bool GetDeviceInputState(FHIDDeviceContext* DeviceContext, unsigned char* InputState);
	
	static UINT32 Compute(const unsigned char* Buffer, size_t Len);
};
