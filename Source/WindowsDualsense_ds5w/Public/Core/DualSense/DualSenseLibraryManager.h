// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"
#include "DualSenseLibrary.h"
#include "UObject/Object.h"
#include "Misc/CoreDelegates.h"
#include "DualSenseLibraryManager.generated.h"

#define MAX_DEVICES 16


/**
 * Represents a library manager responsible for handling DualSense controller functionalities.
 */
UCLASS()
class WINDOWSDUALSENSE_DS5W_API UDualSenseLibraryManager : public UObject
{
	GENERATED_BODY()

	/**
	 * Destructor for the UDualSenseLibraryManager class.
	 *
	 * Handles necessary cleanup operations when an instance of UDualSenseLibraryManager is destroyed.
	 * Logs a message to indicate the instance has been destructed.
	 */
public:
	virtual ~UDualSenseLibraryManager() override;

	/**
	 * Retrieves the singleton instance of UDualSenseLibraryManager, creating it if it does not already exist.
	 *
	 * @return A pointer to the UDualSenseLibraryManager singleton instance.
	 */
	static UDualSenseLibraryManager* Get();
	/**
	 * Retrieves the library instance associated with the specified controller ID.
	 *
	 * @param ControllerId The unique identifier of the controller whose library instance is to be retrieved.
	 * @return A pointer to the library instance if the controller is connected and exists in the library; otherwise, returns nullptr.
	 */
	static UDualSenseLibrary* GetLibraryInstance(int32 ControllerId);
	/**
	 * Retrieves the DualSense library instance associated with the given controller ID.
	 * If the library instance is not connected, it will attempt to reconnect.
	 * If no instance exists for the specified controller ID, a new instance will be created.
	 *
	 * @param ControllerId The unique ID of the controller for which the library instance should be retrieved or reconnected.
	 * @return A pointer to the DualSense library instance for the given controller ID, or nullptr if the instance could not be created.
	 */
	static UDualSenseLibrary* GetLibraryOrReconnect(int32 ControllerId);
	/**
	 * Retrieves the total number of allocated DualSense library instances.
	 *
	 * @return The number of currently allocated DualSense library instances.
	 */
	static int32 GetAllocatedDevices();
	/**
	 * Creates and initializes library instances for each connected DualSense device.
	 *
	 * This method scans for connected DualSense devices using the HID manager,
	 * initializes library instances for each device, and stores them in the
	 * `LibraryInstances` map. If no devices are found or the number of devices exceeds
	 * the maximum allowed limit, the method logs appropriate messages and terminates
	 * the process early. For each detected device, the method creates a handle,
	 * initializes the library, and maps the instance to the device's ID.
	 *
	 * Key Behaviors:
	 * - Resets the `LibraryInstances` map to clear any existing data.
	 * - Searches for connected devices using the HID manager and retrieves details
	 *   into a list of `FDeviceContext` objects.
	 * - Skips processing if no devices are found or if the number of devices exceeds
	 *   the defined limit.
	 * - For each connected device:
	 *   - Creates a new handle via `UDualSenseHIDManager::CreateHandle`.
	 *   - Initializes a `UDualSenseLibrary` instance and sets it up for the device.
	 *   - Adds the instance to the map with the device's ID as the key.
	 *   - Logs the status of initialization for debugging purposes.
	 * - Handles scenarios such as failed library creation or device disconnection
	 *   with appropriate error logging and continuation of the loop for remaining devices.
	 *
	 * Warnings:
	 * - If a device fails to initialize properly, its entry is skipped, and the process
	 *   continues with the next device.
	 * - Logs are generated for debugging and error-handling purposes.
	 */
	static void CreateLibraryInstances();
	/**
	 * Removes all instances of the DualSense library managed by the system.
	 *
	 * This function shuts down all existing library instances by calling
	 * their individual `ShutdownLibrary` method and then clears the internal map
	 * that maintains references to these instances.
	 *
	 * Calling this method is useful when you want to ensure that all library
	 * instances are properly cleaned up, for example, when shutting down or
	 * reinitializing the system.
	 */
	static void RemoveAllLibraryInstance();
	/**
	 * Removes the library instance associated with the specified controller ID.
	 * If a library instance exists for the given controller, it shuts down the library
	 * and removes its entry from the LibraryInstances map.
	 *
	 * @param ControllerId The ID of the controller whose library instance should be removed.
	 */
	static void RemoveLibraryInstance(int32 ControllerId);

	/**
	 * A static instance of FGenericPlatformInputDeviceMapper used to handle mapping of input devices
	 * across different platforms. This variable helps manage the relationship between controllers
	 * and their respective input configurations, ensuring seamless integration and functionality.
	 */
	static FGenericPlatformInputDeviceMapper PlatformInputDeviceMapper;

	/**
	 * A static instance of the UDualSenseLibraryManager class, serving as a singleton for managing
	 * the lifecycle and access of DualSense library instances. This property ensures a single
	 * point of access to the library manager across the application.
	 */
private:
	static UDualSenseLibraryManager* Instance;
	/**
	 * A static map that holds instances of UDualSenseLibrary objects associated with their respective integer keys.
	 * It is used to manage and retrieve library instances for DualSense functionality.
	 */
	static TMap<int32, UDualSenseLibrary*> LibraryInstances;
	/**
	 * Creates and initializes a library instance for a DualSense controller
	 * based on the given ControllerID. If the specified controller is not found
	 * or fails to initialize, this method will return nullptr.
	 *
	 * @param ControllerID The index of the DualSense controller to create the library instance for.
	 * @return A pointer to the initialized UDualSenseLibrary instance if successful, or nullptr if the controller
	 *         could not be found or the library could not be initialized.
	 */
	static UDualSenseLibrary* CreateLibraryInstance(int32 ControllerID);
};
