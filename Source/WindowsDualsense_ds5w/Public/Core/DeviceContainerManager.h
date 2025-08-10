// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/SonyGamepadInterface.h"
#include "UObject/Object.h"
#include "DeviceContainerManager.generated.h"

/**
 * A manager class that handles the creation, storage, and lifecycle management of device library
 * instances associated with Sony gamepad controllers. This class ensures proper initialization,
 * cleanup, and access control for the controller devices.
 */
UCLASS()
class WINDOWSDUALSENSE_DS5W_API UDeviceContainerManager : public UObject
{
	GENERATED_BODY()

	
public:
	/**
	 * Retrieves the static instance of the UDeviceContainerManager class. This method
	 * ensures that only a single instance of the manager class is created and provides
	 * global access to it for managing device library instances of Sony gamepad controllers.
	 *
	 * @return A pointer to the singleton instance of UDeviceContainerManager. Returns nullptr
	 *         if the instance has not been initialized.
	 */
	static UDeviceContainerManager* Get();
	/**
	 * Removes all existing library instances managed by the device container. This method
	 * is responsible for cleaning up and unloading all currently allocated Sony gamepad
	 * controllers' library resources, ensuring proper resource management and preventing
	 * potential memory leaks.
	 */
	static void RemoveAllLibraryInstance();
	/**
	 * Removes the library instance associated with the specified controller ID. This method
	 * is used to unload and clean up resources associated with the Sony gamepad controller
	 * corresponding to the given ID, ensuring proper resource management and preventing memory leaks.
	 *
	 * @param ControllerId The unique identifier of the controller whose library instance
	 *                     needs to be removed.
	 */
	static void RemoveLibraryInstance(int32 ControllerId);
	/**
	 * Retrieves the library instance associated with a specific Sony gamepad controller,
	 * identified by its unique controller ID. This method ensures that the appropriate
	 * library instance for the given controller is returned, enabling interaction with
	 * the corresponding input device.
	 *
	 * @param ControllerId The unique identifier of the Sony gamepad controller for which
	 *                     the library instance is to be retrieved.
	 * @return A pointer to the ISonyGamepadInterface instance corresponding to the specified
	 *         controller ID, or nullptr if no matching instance exists.
	 */
	static ISonyGamepadInterface* GetLibraryInstance(int32 ControllerId);
	/**
	 * Attempts to retrieve the library instance associated with the specified Sony gamepad
	 * controller ID. If no library instance is found, the method attempts to reconnect and
	 * reinitialize the library for the specified controller, ensuring continued functionality.
	 *
	 * @param ControllerId The unique identifier of the Sony gamepad controller for which
	 *                     the library instance is to be retrieved or reconnected.
	 * @return A pointer to the ISonyGamepadInterface instance associated with the specified
	 *         controller ID, or nullptr if the library could not be retrieved or reconnected.
	 */
	static ISonyGamepadInterface* GetLibraryOrReconnect(int32 ControllerId);
	/**
	 * Initializes and creates library instances for managing Sony gamepad controllers.
	 * This method is responsible for setting up all necessary resources and ensuring
	 * that the library instances are properly configured and ready for operation.
	 */
	static void CreateLibraryInstances();
	/**
	 * Retrieves the total number of currently allocated device library instances.
	 * This method provides a count of the devices being managed by the container,
	 * useful for monitoring and ensuring proper resource allocation.
	 *
	 * @return The number of allocated device library instances.
	 */
	static int32 GetAllocatedDevices();
	
private:
	/**
	 * A static instance of the UDeviceContainerManager, serving as the singleton instance
	 * for managing device library instances of Sony gamepad controllers. This variable
	 * ensures global access and consistent management of the controller devices, while
	 * enforcing a single unique instance of the manager class.
	 */
	static UDeviceContainerManager* Instance;
	/**
	 * A static map that stores instances of Sony gamepad interfaces, identified by a unique
	 * controller ID. This map is used to manage and provide access to the library instances
	 * of Sony gamepad controllers, enabling interaction and control over the corresponding
	 * input devices. Each entry in the map associates a unique integer ID with a pointer
	 * to the ISonyGamepadInterface implementation for that controller.
	 */
	static TMap<int32, ISonyGamepadInterface*> LibraryInstances;
	/**
	 * Creates a new library instance for managing the Sony gamepad controller specified
	 * by the given Controller ID. This method initializes and allocates the resources
	 * necessary for the specified controller, allowing it to be accessed and interacted with.
	 *
	 * @param ControllerID The unique identifier of the Sony gamepad controller for which
	 *                     the library instance should be created.
	 * @return A pointer to the newly created ISonyGamepadInterface instance associated with
	 *         the specified controller ID, or nullptr if the creation fails.
	 */
	static ISonyGamepadInterface* CreateLibraryInstance(int32 ControllerID);
};
