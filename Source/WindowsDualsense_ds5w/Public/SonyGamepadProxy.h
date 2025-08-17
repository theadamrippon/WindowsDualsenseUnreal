// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Core/Enums/EDeviceCommons.h"
#include "Core/Enums/EDeviceConnection.h"
#include "SonyGamepadProxy.generated.h"


/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class WINDOWSDUALSENSE_DS5W_API USonyGamepadProxy : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Checks if the DualSense or DualShock device with the specified Controller ID is connected.
	 *
	 * @param ControllerId The ID of the controller to check for connectivity.
	 * @return True if the DualSense or DualShock  device is connected, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "SonyGamepad: Dualsense or DualShock Status")
	static bool DeviceIsConnected(int32 ControllerId);
	/**
	 * Retrieves the type of the connected device (e.g., DualSense, DualSense Edge, or DualShock 4)
	 * based on the specified Controller ID.
	 *
	 * @param ControllerId The ID of the connected controller whose type is to be determined.
	 * @return The type of the device as an EDeviceType enum value. Possible values include:
	 *         - DualSense
	 *         - DualSenseEdge
	 *         - DualShock4
	 */
	UFUNCTION(BlueprintCallable, Category = "SonyGamepad: Dualsense or DualShock Status")
	static EDeviceType GetDeviceType(int32 ControllerId);
	/**
	 * Retrieves the connection type of the DualSense or DualShock device with the specified Controller ID.
	 *
	 * @param ControllerId The ID of the controller whose connection type is being queried.
	 * @return The connection type of the device, which can be USB, Bluetooth, or Unrecognized.
	 */
	UFUNCTION(BlueprintCallable, Category = "SonyGamepad: Dualsense or DualShock Status")
	static EDeviceConnection GetConnectionType(int32 ControllerId);
	/**
	 * Attempts to reconnect a DualSense or DualShock controller based on the given controller ID.
	 * If the controller is successfully reconnected, the operation returns true;
	 * otherwise, it returns false.
	 *
	 * @param ControllerId The ID of the controller to reconnect.
	 * @return Returns true if the controller was successfully reconnected, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "SonyGamepad: Dualsense or DualShock Status")
	static bool DeviceReconnect(int32 ControllerId);
	/**
	 * Disconnects the DualSense or DualShock device associated with the given Controller ID.
	 * This method removes the library instance associated with the specified controller.
	 *
	 * @param ControllerId The ID of the DualSense or DualShock or DualShock controller to be disconnected.
	 * @return true if the disconnection was initiated successfully.
	 */
	UFUNCTION(BlueprintCallable, Category = "SonyGamepad: Dualsense or DualShock Status")
	static bool DeviceDisconnect(int32 ControllerId);

	/**
	 * Retrieves the battery level of the DualSense or DualShock controller for the specified controller ID.
	 *
	 * This method uses the DualSense or DualShock library instance associated with the provided controller ID
	 * to fetch the battery level. If the library instance is not found, the method returns 0.0f.
	 *
	 * @param ControllerId The ID of the DualSense or DualShock controller to query.
	 * @return The battery level of the controller as a float. Returns 0.0f if the library instance cannot be retrieved.
	 */
	UFUNCTION(BlueprintCallable, Category = "SonyGamepad: Dualsense or DualShock Status")
	static float LevelBatteryDevice(int32 ControllerId);

	/**
	 * Updates the LED color effects on a DualSense controller using the specified color.
	 *
	 * @param ControllerId The identifier of the controller whose LED color will be updated.
	 * @param Color The color to set on the controller's LED.
	 */
	UFUNCTION(BlueprintCallable, Category = "SonyGamepad: Dualsense or DualShock Led Effects")
	static void LedColorEffects(
		int32 ControllerId,
		FColor Color,
		UPARAM(DisplayName = "(DualShock 4) LED brightness transition time min: 0.0f max: 2.5f", meta = (ClampMin = "0.0", ClampMax = "2.5", UIMin = "0.0", UIMax = "2.5", ToolTip = "(DualShock) LED brightness transition time, in seconds."))
		const float BrightnessTime = 0.0f,
		UPARAM(DisplayName = "(DualShock 4) Toggle transition time min: 0.0f max: 2.5f",  meta = (ClampMin = "0.0", ClampMax = "2.5", UIMin = "0.0", UIMax = "2.5", ToolTip = "(DualShock) Toggle transition time, in seconds."))
		const float ToogleTime = 0.0f
	);

	/**
	 * Controls the LED and microphone visual effects on a DualSense controller.
	 *
	 * @param ControllerId The ID of the DualSense controller to be affected.
	 * @param Value The desired LED and microphone effect to apply, represented as an ELedMicEnum value.
	 */
	UFUNCTION(BlueprintCallable, Category = "SonyGamepad: Dualsense or DualShock Led Effects")
	static void LedMicEffects(int32 ControllerId, ELedMicEnum Value);

	/**
	 * Enables or disables the touch functionality on a specified DualSense controller.
	 *
	 * @param ControllerId The identifier of the controller for which the touch functionality should be enabled or disabled.
	 * @param bEnableTouch A boolean indicating whether to enable (true) or disable (false) the touch functionality.
	 */
	UFUNCTION(BlueprintCallable, Category = "SonyGamepad: Dualsense or DualShock Touch, Gyroscope and Accelerometer")
	static void EnableTouch(int32 ControllerId, bool bEnableTouch);

	/**
	 * Enables or disables accelerometer values for the specified controller.
	 *
	 * This method allows toggling the accelerometer functionality for a given
	 * controller ID. If the DualSense instance for the specified controller ID
	 * is not available, the function will return without performing any actions.
	 *
	 * @param ControllerId The ID of the controller for which the accelerometer values will be enabled or disabled.
	 * @param bEnableAccelerometer A boolean value that determines whether to enable or disable accelerometer values.
	 */
	UFUNCTION(BlueprintCallable, Category = "SonyGamepad: Dualsense or DualShock Touch, Gyroscope and Accelerometer")
	static void EnableAccelerometerValues(int32 ControllerId, bool bEnableAccelerometer);

	/**
	 * Enables or disables the gyroscope functionality for a specified DualSense controller.
	 *
	 * @param ControllerId The ID of the controller for which the gyroscope functionality is to be modified.
	 * @param bEnableGyroscope Set to true to enable the gyroscope, or false to disable it.
	 */
	UFUNCTION(BlueprintCallable, Category = "SonyGamepad: Dualsense or DualShock or Touch, Gyroscope and Accelerometer")
	static void EnableGyroscopeValues(int32 ControllerId, bool bEnableGyroscope);
};
