// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025


#include "DualSenseHIDManager.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include <windows.h>
#include <hidsdi.h>
#include <setupapi.h>
#include "Windows/HideWindowsPlatformTypes.h"

DualSenseHIDManager::DualSenseHIDManager()
{
}

DualSenseHIDManager::~DualSenseHIDManager()
{
}

bool DualSenseHIDManager::FindDevices(TArray<FHIDDeviceContext>& Devices)
{
	Devices.Empty();

	GUID HidGuid;
	HidD_GetHidGuid(&HidGuid);

	const HDEVINFO DeviceInfoSet = SetupDiGetClassDevs(&HidGuid, nullptr, nullptr,
	                                                   DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
	if (DeviceInfoSet == INVALID_HANDLE_VALUE)
	{
		UE_LOG(LogTemp, Error, TEXT("HIDManager: Falha ao obter informações dos dispositivos HID."));
		return false;
	}

	SP_DEVICE_INTERFACE_DATA DeviceInterfaceData = {};
	DeviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

	for (DWORD DeviceIndex = 0; SetupDiEnumDeviceInterfaces(DeviceInfoSet, nullptr, &HidGuid, DeviceIndex,
	                                                        &DeviceInterfaceData); DeviceIndex++)
	{
		DWORD RequiredSize = 0;

		SetupDiGetDeviceInterfaceDetail(DeviceInfoSet, &DeviceInterfaceData, nullptr, 0, &RequiredSize, nullptr);

		const auto DetailDataBuffer = static_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA>(malloc(RequiredSize));
		if (!DetailDataBuffer)
		{
			UE_LOG(LogTemp, Error, TEXT("HIDManager: Falha ao alocar memória para os detalhes do dispositivo."));
			continue;
		}
		DetailDataBuffer->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

		


		if (SetupDiGetDeviceInterfaceDetail(DeviceInfoSet, &DeviceInterfaceData, DetailDataBuffer, RequiredSize,
		                                    nullptr, nullptr))
		{
			const HANDLE TempDeviceHandle = CreateFileW(
				DetailDataBuffer->DevicePath,
				GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, NULL, nullptr
			);

			if (TempDeviceHandle != INVALID_HANDLE_VALUE)
			{
				HIDD_ATTRIBUTES Attributes = {};
				Attributes.Size = sizeof(HIDD_ATTRIBUTES);

				if (HidD_GetAttributes(TempDeviceHandle, &Attributes))
				{
					if (Attributes.VendorID == 0x054C && (Attributes.ProductID == 0x0CE6 || Attributes.ProductID == 0x0DF2))
					{
						FHIDDeviceContext Context = {};
						size_t PathSize = 260;
						WCHAR DeviceProductString[260]; // Variável separada para o nome do produto.
						if (!HidD_GetProductString(TempDeviceHandle, DeviceProductString, PathSize))
						{
							UE_LOG(LogTemp, Error, TEXT("HIDManager: Failed to obtain device path for the DualSense."));
							continue;
						}

						wcscpy_s(Context.Internal.DevicePath, 260, DetailDataBuffer->DevicePath);
						// UE_LOG(LogTemp, Log, TEXT("HIDManager: Detalhes do dispositivo encontrados: %s %s"), DetailDataBuffer->DevicePath, Context.Internal.DevicePath);

						Context.Internal.Connection = EHIDDeviceConnection::Usb;
						FString DevicePath(DetailDataBuffer->DevicePath);
						if (DevicePath.Contains(TEXT("{00001124-0000-1000-8000-00805f9b34fb}")) ||
							DevicePath.Contains(TEXT("bth")) ||
							DevicePath.Contains(TEXT("BTHENUM")))
						{
							UE_LOG(LogTemp, Log, TEXT("HIDManager: Connection Bluetooth: %s"), DetailDataBuffer->DevicePath);
							Context.Internal.Connection = EHIDDeviceConnection::Bluetooth;
						}

						Context.Internal.Connected = true;
						Context.Internal.DeviceHandle = TempDeviceHandle;
						Devices.Add(Context);
					}
				}
			}
			// CloseHandle(TempDeviceHandle);
		}
		free(DetailDataBuffer);
	}

	SetupDiDestroyDeviceInfoList(DeviceInfoSet);
	return Devices.Num() > 0;
}

bool DualSenseHIDManager::GetDeviceInputState(FHIDDeviceContext* DeviceContext, unsigned char* InputState)
{
	if (!DeviceContext->Internal.Connected)
	{
		UE_LOG(LogTemp, Error, TEXT("Dualsense: DeviceContext->Internal.Connected, false"));
		FreeContext(DeviceContext);
		return false;
	}
	
	if (DeviceContext->Internal.DeviceHandle == INVALID_HANDLE_VALUE)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid device handle before attempting to read"));
		FreeContext(DeviceContext);
		return false;
	}
	
	HidD_FlushQueue(DeviceContext->Internal.DeviceHandle);

	DWORD BytesRead = 0;
	if (!ReadFile(DeviceContext->Internal.DeviceHandle, DeviceContext->Internal.Buffer,
	              sizeof(DeviceContext->Internal.Buffer), &BytesRead, NULL))
	{
		const DWORD Error = GetLastError();
		UE_LOG(LogTemp, Error, TEXT("Erro read DualSense: size buffer %llu, Erro: %d"), sizeof(DeviceContext->Internal.Buffer), Error);

		DeviceContext->Internal.DeviceHandle = INVALID_HANDLE_VALUE;
		FreeContext(DeviceContext);
		return false;
	}


	if (DeviceContext->Internal.Buffer[0] == 0x31 && DeviceContext->Internal.Buffer[1] == 0x02)
	{
		memcpy(&InputState, &DeviceContext->Internal.Buffer[2], sizeof(DeviceContext->Internal.Buffer));
		return true;
	}

	memcpy(&InputState, &DeviceContext->Internal.Buffer[1], sizeof(DeviceContext->Internal.Buffer));
	return true;
}

void DualSenseHIDManager::FreeContext(FHIDDeviceContext* Context)
{
	Context->Internal.Connected = false;
	Context->Internal.Connection = EHIDDeviceConnection::Unknown;
	ZeroMemory(&Context->Internal.DevicePath, sizeof(Context->Internal.DevicePath));
	ZeroMemory(&Context->Internal.Buffer, sizeof(Context->Internal.Buffer));
	CloseHandle(Context->Internal.DeviceHandle);
}


void DualSenseHIDManager::OutputBuffering(FHIDDeviceContext* Context, const FDsHIDOutputBuffer& HidOut)
{
	if (!Context->Internal.Connected || Context->Internal.Connection == EHIDDeviceConnection::Unknown || Context->Internal.DeviceHandle == INVALID_HANDLE_VALUE)
	{
		FreeContext(Context);
		return;
	}
	
	Context->Internal.Buffer[0] = 0x02;
	if (Context->Internal.Connection == EHIDDeviceConnection::Bluetooth)
	{
		Context->Internal.Buffer[0] = 0x31;
		Context->Internal.Buffer[1] = 0x02;
	}
	
	unsigned char* Output = Context->Internal.Connection == EHIDDeviceConnection::Bluetooth ? &Context->Internal.Buffer[2] : &Context->Internal.Buffer[1];
	Output[0x00] = 0xff;
	Output[0x01] = 0xF7;
	Output[0x30] = 0x80;
	Output[0x31] = 0xFF;

	// Rrigger R
	unsigned char* TriggerR = &Output[0x0A];
	TriggerR[0x0] = HidOut.RightTrigger.Mode;

	// Trigger L
	unsigned char* TriggerL = &Output[0x15];
	TriggerL[0x0] = HidOut.LeftTrigger.Mode;

	Output[0x03] = HidOut.MotorsHid.Left;
	Output[0x02] = HidOut.MotorsHid.Right;
	Output[0x08] = HidOut.MicLed.Mode;

	Output[0x26] = 0x03;

	Output[0x29] = HidOut.LedPlayerHid.Player;
	Output[0x2A] = HidOut.LedPlayerHid.Brightness;
	Output[0x2B] = HidOut.LedPlayerHid.Led;


	Output[0x2C] = HidOut.ColorHid.R;
	Output[0x2D] = HidOut.ColorHid.G;
	Output[0x2E] = HidOut.ColorHid.B;

	if (HidOut.ResetLedEffects) // Reset temp effects
	{
		Output[0x2B] = 0x00;
		Output[0x29] = 0x01;
		Output[0x2A] = 0x00;

		Output[0x2C] = 0x00;
		Output[0x2D] = 0x00;
		Output[0x2E] = 0xFF;
	}

	// Trigger L
	if (HidOut.LeftTrigger.Mode == 0x01)
	{
		TriggerL[0x1] = HidOut.LeftTrigger.StartPosition;
		TriggerL[0x2] = HidOut.LeftTrigger.Strengths.Start;
	}

	if (HidOut.LeftTrigger.Mode == 0x02)
	{
		TriggerL[0x1] = HidOut.LeftTrigger.StartPosition;
		TriggerL[0x2] = HidOut.LeftTrigger.EndPosition;
		TriggerL[0x3] = HidOut.LeftTrigger.Strengths.Start;
	}

	if (HidOut.LeftTrigger.Mode == 0x21)
	{
		const uint64_t LeftTriggerStrengthZones = HidOut.LeftTrigger.Strengths.StrengthZones;
		TriggerL[0x1] = ((HidOut.LeftTrigger.Strengths.ActiveZones >> 0) & 0xff);
		TriggerL[0x2] = ((HidOut.LeftTrigger.Strengths.ActiveZones >> 8) & 0xff);
		TriggerL[0x3] = ((LeftTriggerStrengthZones >> 0) & 0xff);
		TriggerL[0x4] = ((LeftTriggerStrengthZones >> 8) & 0xff);
		TriggerL[0x5] = ((LeftTriggerStrengthZones >> 16) & 0xff);
		TriggerL[0x6] = ((LeftTriggerStrengthZones >> 24) & 0xff);

		if (sizeof(LeftTriggerStrengthZones) > 4)
		{
			TriggerL[0x7] = ((LeftTriggerStrengthZones >> 32) & 0xff);
			TriggerL[0x8] = ((LeftTriggerStrengthZones >> 40) & 0xff);
		}
	}

	if (HidOut.LeftTrigger.Mode == 0x22) // Bow
	{
		TriggerL[0x1] = ((HidOut.LeftTrigger.Strengths.ActiveZones >> 0) & 0xff);
		TriggerL[0x2] = ((HidOut.LeftTrigger.Strengths.ActiveZones >> 8) & 0xff);
		TriggerL[0x3] = ((HidOut.LeftTrigger.Strengths.StrengthZones >> 0) & 0xff);
		TriggerL[0x4] = ((HidOut.LeftTrigger.Strengths.StrengthZones >> 8) & 0xff);
	}

	if (HidOut.LeftTrigger.Mode == 0x23) // Gallopping
	{
		TriggerL[0x1] = ((HidOut.LeftTrigger.Strengths.ActiveZones >> 0) & 0xff);
		TriggerL[0x2] = ((HidOut.LeftTrigger.Strengths.ActiveZones >> 8) & 0xff);
		TriggerL[0x3] = ((HidOut.LeftTrigger.Strengths.TimeAndRatio >> 0) & 0xff);
		TriggerL[0x4] = HidOut.LeftTrigger.Frequency;
	}

	if (HidOut.LeftTrigger.Mode == 0x25) // Weapon
	{
		TriggerL[0x1] = HidOut.LeftTrigger.StartPosition;
		TriggerL[0x2] = HidOut.LeftTrigger.EndPosition;
		TriggerL[0x3] = HidOut.LeftTrigger.Strengths.Start;
		TriggerL[0x4] = 0x00;
		TriggerL[0x5] = 0x00;
		TriggerL[0x6] = 0x00;
		TriggerL[0x7] = 0x00;
		TriggerL[0x8] = 0x00;
		TriggerL[0x9] = 0x00;
	}

	if (HidOut.LeftTrigger.Mode == 0x26)
	{
		const uint64_t LeftTriggerStrengthZones = HidOut.LeftTrigger.Strengths.StrengthZones;
		TriggerL[0x1] = ((HidOut.LeftTrigger.Strengths.ActiveZones >> 0) & 0xff);
		TriggerL[0x2] = ((HidOut.LeftTrigger.Strengths.ActiveZones >> 8) & 0xff);
		TriggerL[0x3] = ((LeftTriggerStrengthZones >> 0) & 0xff);
		TriggerL[0x4] = ((LeftTriggerStrengthZones >> 8) & 0xff);
		TriggerL[0x5] = ((LeftTriggerStrengthZones >> 16) & 0xff);
		TriggerL[0x6] = ((LeftTriggerStrengthZones >> 24) & 0xff);

		if (sizeof(LeftTriggerStrengthZones) > 4)
		{
			TriggerL[0x7] = ((LeftTriggerStrengthZones >> 32) & 0xff);
			TriggerL[0x8] = ((LeftTriggerStrengthZones >> 40) & 0xff);
		}
		TriggerL[0x9] = HidOut.LeftTrigger.Frequency;
	}

	if (HidOut.LeftTrigger.Mode == 0x27) // Machine
	{
		TriggerL[0x1] = ((HidOut.LeftTrigger.Strengths.ActiveZones >> 0) & 0xff);
		TriggerL[0x2] = ((HidOut.LeftTrigger.Strengths.ActiveZones >> 8) & 0xff);
		TriggerL[0x3] = ((HidOut.LeftTrigger.Strengths.StrengthZones >> 0) & 0xff);
		TriggerL[0x4] = HidOut.LeftTrigger.Frequency;
		TriggerL[0x5] = HidOut.LeftTrigger.Strengths.Period;
	}


	// Trigger R
	if (HidOut.RightTrigger.Mode == 0x01)
	{
		TriggerR[0x1] = HidOut.RightTrigger.StartPosition;
		TriggerR[0x2] = HidOut.RightTrigger.Strengths.Start;
	}

	if (HidOut.RightTrigger.Mode == 0x02)
	{
		TriggerR[0x1] = HidOut.RightTrigger.StartPosition;
		TriggerR[0x2] = HidOut.RightTrigger.EndPosition;
		TriggerR[0x3] = HidOut.RightTrigger.Strengths.Start;
	}

	if (HidOut.RightTrigger.Mode == 0x21)
	{
		const uint64_t RightTriggerStrengthZones = HidOut.RightTrigger.Strengths.StrengthZones;
		TriggerR[0x1] = ((HidOut.RightTrigger.Strengths.ActiveZones >> 0) & 0xff);
		TriggerR[0x2] = ((HidOut.RightTrigger.Strengths.ActiveZones >> 8) & 0xff);
		TriggerR[0x3] = ((RightTriggerStrengthZones >> 0) & 0xff);
		TriggerR[0x4] = ((RightTriggerStrengthZones >> 8) & 0xff);
		TriggerR[0x5] = ((RightTriggerStrengthZones >> 16) & 0xff);
		TriggerR[0x6] = ((RightTriggerStrengthZones >> 24) & 0xff);
	}

	if (HidOut.RightTrigger.Mode == 0x22) // Bow
	{
		TriggerR[0x1] = ((HidOut.RightTrigger.Strengths.ActiveZones >> 0) & 0xff);
		TriggerR[0x2] = ((HidOut.RightTrigger.Strengths.ActiveZones >> 8) & 0xff);
		TriggerR[0x3] = ((HidOut.RightTrigger.Strengths.StrengthZones >> 0) & 0xff);
		TriggerR[0x4] = ((HidOut.RightTrigger.Strengths.StrengthZones >> 8) & 0xff);
	}

	if (HidOut.RightTrigger.Mode == 0x23) // Gallopping
	{
		TriggerR[0x1] = ((HidOut.RightTrigger.Strengths.ActiveZones >> 0) & 0xff);
		TriggerR[0x2] = ((HidOut.RightTrigger.Strengths.ActiveZones >> 8) & 0xff);
		TriggerR[0x3] = ((HidOut.RightTrigger.Strengths.TimeAndRatio >> 0) & 0xff);
		TriggerR[0x4] = HidOut.RightTrigger.Frequency;
	}

	if (HidOut.RightTrigger.Mode == 0x25) // Weapon
	{
		TriggerR[0x1] = HidOut.RightTrigger.StartPosition;
		TriggerR[0x2] = HidOut.RightTrigger.EndPosition;
		TriggerR[0x3] = HidOut.RightTrigger.Strengths.Start;
	}

	if (HidOut.RightTrigger.Mode == 0x26)
	{
		const uint64_t RightTriggerStrengthZones = HidOut.RightTrigger.Strengths.StrengthZones;

		TriggerR[0x1] = ((HidOut.RightTrigger.Strengths.ActiveZones >> 0) & 0xff);
		TriggerR[0x2] = ((HidOut.RightTrigger.Strengths.ActiveZones >> 8) & 0xff);
		TriggerR[0x3] = ((RightTriggerStrengthZones >> 0) & 0xff);
		TriggerR[0x4] = ((RightTriggerStrengthZones >> 8) & 0xff);
		TriggerR[0x5] = ((RightTriggerStrengthZones >> 16) & 0xff);
		TriggerR[0x6] = ((RightTriggerStrengthZones >> 24) & 0xff);
		if (sizeof(RightTriggerStrengthZones) > 4)
		{
			TriggerR[0x7] = ((RightTriggerStrengthZones >> 32) & 0xff);
			TriggerR[0x8] = ((RightTriggerStrengthZones >> 40) & 0xff);
		}
		TriggerR[0x9] = HidOut.RightTrigger.Frequency;
	}

	if (HidOut.RightTrigger.Mode == 0x27) // Machine
	{
		TriggerR[0x1] = ((HidOut.RightTrigger.Strengths.ActiveZones >> 0) & 0xff);
		TriggerR[0x2] = ((HidOut.RightTrigger.Strengths.ActiveZones >> 8) & 0xff);
		TriggerR[0x3] = ((HidOut.RightTrigger.Strengths.StrengthZones >> 0) & 0xff);
		TriggerR[0x4] = HidOut.RightTrigger.Frequency;
		TriggerR[0x5] = HidOut.RightTrigger.Strengths.Period;
	}
	
	if (HidOut.LeftTrigger.Mode == 0x05)
	{
		TriggerL[0x1] = 0x00;
		TriggerL[0x2] = 0x00;
		TriggerL[0x3] = 0x00;
		TriggerL[0x4] = 0x00;
		TriggerL[0x5] = 0x00;
		TriggerL[0x6] = 0x00;
		TriggerL[0x7] = 0x00;
		TriggerL[0x8] = 0x00;
		TriggerL[0x9] = 0x00;
	}

	if (HidOut.RightTrigger.Mode == 0x05) // Reset temp effects trigger
	{
		TriggerR[0x1] = 0x00;
		TriggerR[0x2] = 0x00;
		TriggerR[0x3] = 0x00;
		TriggerR[0x4] = 0x00;
		TriggerR[0x5] = 0x00;
		TriggerR[0x6] = 0x00;
		TriggerR[0x7] = 0x00;
		TriggerR[0x8] = 0x00;
		TriggerR[0x9] = 0x00;
	}

	/**
	 *
	 * Copyright (c) 2020 Ludwig Füchsl
	 * Code reference https://github.com/Ohjurot/DualSense-Windows/blob/main/VS19_Solution/DualSenseWindows/src/DualSenseWindows/DS_CRC32.cpp
	 */
	const UINT32 CrcChecksum = Compute(Context->Internal.Buffer, 74);
	Context->Internal.Buffer[0x4A] = static_cast<unsigned char>((CrcChecksum & 0x000000FF) >> 0UL);
	Context->Internal.Buffer[0x4B] = static_cast<unsigned char>((CrcChecksum & 0x0000FF00) >> 8UL);
	Context->Internal.Buffer[0x4C] = static_cast<unsigned char>((CrcChecksum & 0x00FF0000) >> 16UL);
	Context->Internal.Buffer[0x4D] = static_cast<unsigned char>((CrcChecksum & 0xFF000000) >> 24UL);

	DWORD BytesWritten = 0;
	if (!WriteFile(Context->Internal.DeviceHandle, Context->Internal.Buffer, sizeof(Context->Internal.Buffer),
	               &BytesWritten, nullptr))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to write output data to device. Error Code: %d"), GetLastError());
		FreeContext(Context);
	}
}


/**
 *
 * Copyright (c) 2020 Ludwig Füchsl
 * Code reference https://github.com/Ohjurot/DualSense-Windows/blob/main/VS19_Solution/DualSenseWindows/src/DualSenseWindows/DS_CRC32.cpp
 */
const UINT32 DualSenseHIDManager::HashTable[256] = {
	0xd202ef8d, 0xa505df1b, 0x3c0c8ea1, 0x4b0bbe37, 0xd56f2b94, 0xa2681b02, 0x3b614ab8, 0x4c667a2e,
	0xdcd967bf, 0xabde5729, 0x32d70693, 0x45d03605, 0xdbb4a3a6, 0xacb39330, 0x35bac28a, 0x42bdf21c,
	0xcfb5ffe9, 0xb8b2cf7f, 0x21bb9ec5, 0x56bcae53, 0xc8d83bf0, 0xbfdf0b66, 0x26d65adc, 0x51d16a4a,
	0xc16e77db, 0xb669474d, 0x2f6016f7, 0x58672661, 0xc603b3c2, 0xb1048354, 0x280dd2ee, 0x5f0ae278,
	0xe96ccf45, 0x9e6bffd3, 0x762ae69, 0x70659eff, 0xee010b5c, 0x99063bca, 0xf6a70, 0x77085ae6,
	0xe7b74777, 0x90b077e1, 0x9b9265b, 0x7ebe16cd, 0xe0da836e, 0x97ddb3f8, 0xed4e242, 0x79d3d2d4,
	0xf4dbdf21, 0x83dcefb7, 0x1ad5be0d, 0x6dd28e9b, 0xf3b61b38, 0x84b12bae, 0x1db87a14, 0x6abf4a82,
	0xfa005713, 0x8d076785, 0x140e363f, 0x630906a9, 0xfd6d930a, 0x8a6aa39c, 0x1363f226, 0x6464c2b0,
	0xa4deae1d, 0xd3d99e8b, 0x4ad0cf31, 0x3dd7ffa7, 0xa3b36a04, 0xd4b45a92, 0x4dbd0b28, 0x3aba3bbe,
	0xaa05262f, 0xdd0216b9, 0x440b4703, 0x330c7795, 0xad68e236, 0xda6fd2a0, 0x4366831a, 0x3461b38c,
	0xb969be79, 0xce6e8eef, 0x5767df55, 0x2060efc3, 0xbe047a60, 0xc9034af6, 0x500a1b4c, 0x270d2bda,
	0xb7b2364b, 0xc0b506dd, 0x59bc5767, 0x2ebb67f1, 0xb0dff252, 0xc7d8c2c4, 0x5ed1937e, 0x29d6a3e8,
	0x9fb08ed5, 0xe8b7be43, 0x71beeff9, 0x6b9df6f, 0x98dd4acc, 0xefda7a5a, 0x76d32be0, 0x1d41b76,
	0x916b06e7, 0xe66c3671, 0x7f6567cb, 0x862575d, 0x9606c2fe, 0xe101f268, 0x7808a3d2, 0xf0f9344,
	0x82079eb1, 0xf500ae27, 0x6c09ff9d, 0x1b0ecf0b, 0x856a5aa8, 0xf26d6a3e, 0x6b643b84, 0x1c630b12,
	0x8cdc1683, 0xfbdb2615, 0x62d277af, 0x15d54739, 0x8bb1d29a, 0xfcb6e20c, 0x65bfb3b6, 0x12b88320,
	0x3fba6cad, 0x48bd5c3b, 0xd1b40d81, 0xa6b33d17, 0x38d7a8b4, 0x4fd09822, 0xd6d9c998, 0xa1def90e,
	0x3161e49f, 0x4666d409, 0xdf6f85b3, 0xa868b525, 0x360c2086, 0x410b1010, 0xd80241aa, 0xaf05713c,
	0x220d7cc9, 0x550a4c5f, 0xcc031de5, 0xbb042d73, 0x2560b8d0, 0x52678846, 0xcb6ed9fc, 0xbc69e96a,
	0x2cd6f4fb, 0x5bd1c46d, 0xc2d895d7, 0xb5dfa541, 0x2bbb30e2, 0x5cbc0074, 0xc5b551ce, 0xb2b26158,
	0x4d44c65, 0x73d37cf3, 0xeada2d49, 0x9ddd1ddf, 0x3b9887c, 0x74beb8ea, 0xedb7e950, 0x9ab0d9c6,
	0xa0fc457, 0x7d08f4c1, 0xe401a57b, 0x930695ed, 0xd62004e, 0x7a6530d8, 0xe36c6162, 0x946b51f4,
	0x19635c01, 0x6e646c97, 0xf76d3d2d, 0x806a0dbb, 0x1e0e9818, 0x6909a88e, 0xf000f934, 0x8707c9a2,
	0x17b8d433, 0x60bfe4a5, 0xf9b6b51f, 0x8eb18589, 0x10d5102a, 0x67d220bc, 0xfedb7106, 0x89dc4190,
	0x49662d3d, 0x3e611dab, 0xa7684c11, 0xd06f7c87, 0x4e0be924, 0x390cd9b2, 0xa0058808, 0xd702b89e,
	0x47bda50f, 0x30ba9599, 0xa9b3c423, 0xdeb4f4b5, 0x40d06116, 0x37d75180, 0xaede003a, 0xd9d930ac,
	0x54d13d59, 0x23d60dcf, 0xbadf5c75, 0xcdd86ce3, 0x53bcf940, 0x24bbc9d6, 0xbdb2986c, 0xcab5a8fa,
	0x5a0ab56b, 0x2d0d85fd, 0xb404d447, 0xc303e4d1, 0x5d677172, 0x2a6041e4, 0xb369105e, 0xc46e20c8,
	0x72080df5, 0x50f3d63, 0x9c066cd9, 0xeb015c4f, 0x7565c9ec, 0x262f97a, 0x9b6ba8c0, 0xec6c9856,
	0x7cd385c7, 0xbd4b551, 0x92dde4eb, 0xe5dad47d, 0x7bbe41de, 0xcb97148, 0x95b020f2, 0xe2b71064,
	0x6fbf1d91, 0x18b82d07, 0x81b17cbd, 0xf6b64c2b, 0x68d2d988, 0x1fd5e91e, 0x86dcb8a4, 0xf1db8832,
	0x616495a3, 0x1663a535, 0x8f6af48f, 0xf86dc419, 0x660951ba, 0x110e612c, 0x88073096, 0xff000000
};

/**
 *
 * Copyright (c) 2020 Ludwig Füchsl
 * Code reference https://github.com/Ohjurot/DualSense-Windows/blob/main/VS19_Solution/DualSenseWindows/src/DualSenseWindows/DS_CRC32.cpp
 */
const UINT32 DualSenseHIDManager::CRCSeed = 0xeada2d49;

/**
 *
 * Copyright (c) 2020 Ludwig Füchsl
 * Code reference https://github.com/Ohjurot/DualSense-Windows/blob/main/VS19_Solution/DualSenseWindows/src/DualSenseWindows/DS_CRC32.cpp
 */
UINT32 DualSenseHIDManager::Compute(const unsigned char* Buffer, const size_t Len)
{
	UINT32 Result = CRCSeed;

	for (size_t i = 0; i < Len; i++)
	{
		Result = HashTable[static_cast<unsigned char>(Result) ^ static_cast<unsigned char>(Buffer[i])] ^ (Result >> 8);
	}
	return Result;
}
