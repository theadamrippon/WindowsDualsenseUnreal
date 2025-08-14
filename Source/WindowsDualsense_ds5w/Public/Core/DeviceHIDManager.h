// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include <concrt.h>

#include "CoreMinimal.h"
#include "Core/Structs/FDeviceContext.h"
#include "DeviceHIDManager.generated.h"

/**
 * @class UDeviceHIDManager
 *
 * Manages HID (Human Interface Device) interactions for DualSense devices on Windows.
 * Provides functionality to discover devices, manage device contexts, handle input and output operations,
 * compute hash values, and configure haptic feedback.
 */
UCLASS()
class WINDOWSDUALSENSE_DS5W_API UDeviceHIDManager : public UObject
{
	GENERATED_BODY()

	const static UINT32 CRCSeed;
	const static UINT32 HashTable[256];
public:
	/**
	 * Default constructor for the UDualSenseHIDManager class.
	 *
	 * Initializes an instance of UDualSenseHIDManager to manage DualSense device interactions.
	 *
	 * @return A newly constructed UDualSenseHIDManager object.
	 */
	UDeviceHIDManager(){};
	/**
	 * Frees and resets the memory and resources associated with the given device context.
	 * This includes clearing buffers, resetting connection parameters, and closing the device handle.
	 *
	 * @param Context A pointer to the device context to be freed and reset.
	 *                The context must represent a device currently managed by the DualSense HID Manager.
	 */
	static void FreeContext(FDeviceContext* Context);
	/**
	 * Scans and discovers connected DualSense HID devices, populating the provided array with their contexts.
	 * This function retrieves information about all currently available HID devices compatible with DualSense controllers.
	 *
	 * @param Devices An array to be populated with discovered FDeviceContext objects representing the connected devices.
	 *                The array will contain one entry per successfully identified device.
	 * @return True if at least one DualSense HID device was successfully discovered and added to the Devices array, false otherwise.
	 */
	static bool FindDevices(TArray<FDeviceContext>& Devices);
	/**
	 * Opens and creates a handle to a DualSense device based on the provided device context.
	 * The handle is used for further communication with the device for input/output operations.
	 * If the handle creation fails, the associated device context is freed and an error is logged.
	 *
	 * @param DeviceContext A pointer to the FDeviceContext structure representing the DualSense device
	 *                      for which the handle is to be created. This context must be properly initialized
	 *                      and contain valid device path information.
	 *
	 * @return A HANDLE to the DualSense device if successful. Returns INVALID_HANDLE_VALUE if the operation fails.
	 */
	static HANDLE CreateHandle(FDeviceContext* DeviceContext);
	/**
	 * Outputs current DualSense device states to the HID (Human Interface Device).
	 *
	 * This method prepares and transmits an output report representing the current state
	 * of the DualSense device, including feedback effects such as vibration, adaptive triggers,
	 * lightbar colors, microphone light settings, and player LED configurations.
	 *
	 * Based on the device context, it determines the appropriate output buffer layout,
	 * handles Bluetooth-specific padding, and adjusts output feature structures.
	 * The method writes the prepared buffer to the device via HID protocols using the
	 * appropriate connection type (wired or Bluetooth).
	 *
	 * @param DeviceContext A pointer to the FDeviceContext structure, containing information
	 *                      about the device configuration, state, and connection details.
	 */
	static void OutputDualSense(FDeviceContext* DeviceContext);
	/**
	 * Sends output instructions to a DualShock-compatible device through the provided device context.
	 *
	 * Ensures the device is in a valid state before performing operations by checking the handle validity
	 * and connection status. Logs errors if the device handle is invalid or the device is not connected.
	 *
	 * @param DeviceContext A reference to the device context containing connection and handle details for the target device.
	 */
	static void OutputDualShock(FDeviceContext* DeviceContext);
	/**
	 * Attempts to retrieve the current input state from the specified DualSense device context.
	 * This function reads input data from the device handle into the device's buffer, ensuring the device is connected
	 * and its handle is valid. If the device is disconnected or any operation fails, the context is freed and reset.
	 *
	 * @param DeviceContext A pointer to the FDeviceContext representing the target DualSense device.
	 *                      The context must be properly initialized and linked to a valid device.
	 *                      It holds the connection type, buffer, and handle to the device.
	 * @return True if the input state was successfully retrieved and the device is functional, false otherwise.
	 */
	static bool GetDeviceInputState(FDeviceContext* DeviceContext);
	/**
	 * Configures the trigger effects for a DualSense device based on the specified effect parameters.
	 * Updates the trigger data buffer to represent various haptic feedback modes.
	 *
	 * @param Trigger A pointer to the memory buffer that represents the trigger effect configuration.
	 *                The buffer will be modified to match the specified haptic feedback configuration.
	 * @param Effect A reference to the FHapticTriggers structure containing the haptic effect parameters,
	 *               including mode, strength, frequency, and other relevant settings.
	 */
	static void SetTriggerEffects(unsigned char* Trigger, FHapticTriggers& Effect);
	/**
	 * Computes the CRC32 hash for the given buffer using a predefined hash table and seed value.
	 * The function iterates through each byte of the input buffer to calculate the resulting hash.
	 *
	 * @param Buffer A pointer to the input buffer containing the data for which the CRC32 hash is to be computed.
	 * @param Len The length of the input buffer in bytes.
	 * @return The computed CRC32 hash value.
	 */
	static UINT32 Compute(const unsigned char* Buffer, size_t Len);
	static UINT32 crc_32(unsigned char* buf, int length);
};
