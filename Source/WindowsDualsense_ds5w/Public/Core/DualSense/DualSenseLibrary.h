// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Core/Interfaces/SonyGamepadInterface.h"
#include "Core/Interfaces/SonyGamepadTriggerInterface.h"
#include "InputCoreTypes.h"
#include "Misc/CoreDelegates.h"
#include "Runtime/ApplicationCore/Public/GenericPlatform/IInputInterface.h"
#include "Runtime/ApplicationCore/Public/GenericPlatform/GenericApplicationMessageHandler.h"
#include "Core/Enums/EDeviceCommons.h"
#include "Core/Structs/FDeviceContext.h"
#include "Core/Structs/FDeviceSettings.h"
#include "Core/Structs/FDualSenseFeatureReport.h"
#include "DualSenseLibrary.generated.h"

/**
 * @class FTouchPoint1
 * @brief Represents a touch point in a touch-based input system.
 *
 * The FTouchPoint1 class is designed to encapsulate the properties and behavior
 * of a single touch point detected on a touch-sensitive surface. It provides
 * information such as the position, pressure, and state of the touch point,
 * which can be used to build touch interactions within an application.
 *
 * This class is typically used in systems that handle multi-touch input,
 * providing the data necessary to track individual touch points and their
 * specific attributes.
 *
 * @details The class may store information about the position of the touch point
 * in screen coordinates, the pressure or force of the touch, and the current state
 * of the touch event (e.g., touch began, moved, or ended). It may be used in
 * conjunction with other similar touch point objects for multi-touch gesture recognition
 * or input processing.
 */
struct FTouchPoint1
{
	/**
	 * @brief Represents a generic variable `X`.
	 *
	 * This variable is used as a placeholder or for storing a specific value
	 * during the program's execution. The exact nature and purpose of `X`
	 * depend on the context in which it is defined.
	 *
	 * @note Ensure that the value assigned to `X` is valid and adheres
	 * to the expected data type or constraints for its proper usage.
	 */
	unsigned char X;
	/**
	 * @brief Computes the factorial of a given non-negative integer.
	 *
	 * This function calculates the factorial of a number using recursion.
	 * The factorial of a number `n` is the product of all positive integers less than
	 * or equal to `n`. Factorial is defined as:
	 * - 0! = 1
	 * - n! = n * (n-1)!, where n > 0
	 *
	 * @param n The non-negative integer for which the factorial is to be computed.
	 * @return The factorial of the input number. If the input is 0, returns 1.
	 * @throw std::invalid_argument If the input is a negative number.
	 */
	unsigned char Y;
	/**
	 * @brief Represents a downward movement in a grid or coordinate system.
	 *
	 * This class encapsulates the functionality related to movement in the downward direction.
	 * It could be used in contexts such as 2D games, simulations, or navigation systems
	 * where positioning within a coordinate grid needs to be adjusted downward.
	 */
	unsigned char Down;
	/**
	 * @class Id
	 * @brief Represents a unique identifier.
	 *
	 * This class is designed to handle and manipulate unique identifiers
	 * for various use cases. It provides functionality to generate,
	 * validate, and compare unique ID values.
	 *
	 * @details
	 * The Id class can be used in scenarios where unique identification
	 * is required, such as database keys, UUID generation, and other
	 * similar functionalities. It ensures that each instance represents
	 * a distinct identifier.
	 */
	unsigned char Id;
};

/**
 * @class FTouchPoint2
 * @brief Represents a point of contact on a touch-sensitive surface with additional properties.
 *
 * The FTouchPoint2 class is used to encapsulate the state and properties of a touch point
 * as it interacts with a touch-sensitive interface. Each instance of this class provides
 * detailed information about the current state of the touch point, including its position,
 * pressure, and movement. This is typically used in multi-touch systems to track and manage
 * touch inputs across a surface.
 *
 * The FTouchPoint2 class is particularly useful in applications like gesture recognition,
 * gaming, and graphical applications that require an understanding of user interactions
 * on touch input devices.
 */
struct FTouchPoint2
{
	/**
	 * @brief Represents the variable X, which is used as a placeholder or for a specific purpose in the context of the program.
	 *
	 * This variable may hold data or serve as a control mechanism
	 * depending on the logic implemented in the application. The detailed
	 * behavior, intent, and value assignment of X should align with the program's requirements.
	 *
	 * @note Ensure proper initialization and context-specific usage of X to avoid
	 * unintended behaviors.
	 */
	unsigned char X;
	/**
	 * @brief Represents a variable 'Y' with unspecified type and purpose.
	 *
	 * This variable is declared as 'Y' and may be assigned or used in various contexts
	 * depending on the implementation or application where it is defined. The specific
	 * type, purpose, and usage of 'Y' is undefined and should be interpreted based on
	 * its associated logic or framework.
	 */
	unsigned char Y;
	/**
	 * @brief Represents a direction or movement towards a lower position or level.
	 *
	 * The variable Down is typically used to indicate a downward movement,
	 * orientation, or state in various contexts such as navigation, positioning,
	 * or directional control in a program.
	 */
	unsigned char Down;
	/**
	 * @brief Represents a unique identifier.
	 *
	 * This variable is used to store a unique value that can be assigned
	 * to distinguish between different entities or objects in a system.
	 * It is typically used for identification purposes.
	 */
	unsigned char Id;
};

/**
 * @class FAccelerometer
 * @brief Represents an accelerometer component that tracks and provides data related to acceleration.
 *
 * The FAccelerometer class is responsible for handling acceleration readings
 * and providing utility functions to process and retrieve those readings in a
 * meaningful way. This class can be used to monitor motion in three-dimensional
 * space and calculate relevant metrics based on the gathered data.
 *
 * @details
 * The accelerometer typically measures acceleration in three axes: X, Y, and Z,
 * and provides functionality to retrieve raw acceleration values, calculate the
 * magnitude of the acceleration vector, and filter out noise or apply smoothing
 * when needed. This class can be utilized for physical simulation, motion
 * detection, or other use cases where acceleration tracking is required.
 */
struct FAccelerometer
{
	/**
	 *
	 */
	unsigned char X;
	/**
	 * @brief Represents the variable Y used within the application.
	 *
	 * This variable serves as a placeholder for a specific purpose
	 * depending on the context of the application. The exact use of
	 * Y should be defined in the code implementation and may represent
	 * a variety of data types or values depending on its usage.
	 *
	 * The role of Y can vary, such as acting as an input parameter,
	 * intermediate storage, or output result. Proper initialization
	 * and handling of this variable are essential to ensure the correctness
	 * and reliability of the program's operations.
	 *
	 * Ensure that the value of Y is properly documented and updated
	 * throughout the codebase to avoid confusion or misuse.
	 */
	unsigned char Y;
	/**
	 * @brief Represents a variable named Z.
	 *
	 * This variable is designed to hold a specific value or data type as required
	 * by the implementation. Its intended use and functionality should be defined
	 * within the corresponding program or module context. The type and purpose of
	 * this variable should align with the logic and requirements of its usage.
	 *
	 * Ensure that the value assigned to Z adheres to the appropriate constraints
	 * or expectations in the application to maintain correctness and stability.
	 */
	unsigned char Z;
};

/**
 * @class FGyro
 * @brief The FGyro class represents a gyroscopic sensor that provides data on angular velocity and orientation in three-dimensional space.
 *
 * This class models the behavior and properties of a gyroscope, commonly used in robotics, mobile devices, and game controllers for motion tracking
 * and orientation detection. The class offers functionalities to retrieve, reset, and process gyroscopic readings.
 *
 * Detailed methods and attributes allow users to interact with the gyroscope hardware or simulation to acquire real-time data for use in various applications.
 * The gyroscope data includes angular velocity along the X, Y, and Z axes, as well as possible methods for calibration.
 */
struct FGyro
{
	/**
	 * @brief Represents a variable used for general purposes within the application.
	 *
	 * This variable may store values of various types based on its application context,
	 * and its purpose is defined by the implementation details of the code where it is used.
	 *
	 * @note Ensure to initialize and manage the value of this variable correctly to avoid
	 * unexpected behavior or runtime errors.
	 */
	unsigned char X;
	/**
	 * @brief Represents a generic variable or entity identified as 'Y'.
	 *
	 * @details The purpose and type of 'Y' should be defined by its context in
	 *          the codebase. It could serve as a placeholder or functional part
	 *          of a process and requires further clarification or initialization
	 *          during implementation.
	 *
	 * @note Ensure 'Y' is properly initialized and used consistently to avoid
	 *       unexpected behavior. Misuse of 'Y' may lead to runtime errors or
	 *       logical inconsistencies.
	 */
	unsigned char Y;
	/**
	 * @brief Represents a variable or entity denoted as Z.
	 *
	 * This variable may serve as a placeholder or represent a specific value,
	 * object, or function in the given context. Ensure to initialize or define Z
	 * appropriately before using it, based on its intended purpose.
	 *
	 * @note The specific use case and type of Z must be determined by its context
	 *       within the scope of the application or module.
	 */
	unsigned char Z;
};

/**
 * @class UDualSenseLibrary
 * @brief Utility class for interfacing with the PlayStation DualSense controller.
 *
 * This class provides a collection of static functions and utilities specific to
 * interacting with the PlayStation DualSense controller within a project. It facilitates
 * operations such as managing haptic feedback, adaptive triggers, and other special
 * features offered by the controller.
 *
 * The UDualSenseLibrary is typically used to control and respond to events from
 * a connected DualSense device, enabling advanced functionalities for an enhanced
 * user experience.
 *
 * Features supported may include:
 * - Triggering haptic feedback.
 * - Configuring adaptive trigger effects.
 * - Managing light bars and other controller-specific outputs.
 * - Retrieving connected-state and other metadata.
 *
 * The library is designed for developers seeking to leverage unique features of
 * the DualSense controller programmatically within an application.
 */
UCLASS()
class WINDOWSDUALSENSE_DS5W_API UDualSenseLibrary : public UObject, public ISonyGamepadInterface, public ISonyGamepadTriggerInterface
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief Configures device settings for a connected device.
	 *
	 * This method allows customization and application of configuration
	 * settings for a specific device by modifying its settings object.
	 * It is typically overridden to provide device-specific configuration
	 * logic.
	 *
	 * @param Settings A reference to a FDeviceSettings object that holds
	 * the device's configuration options to be adjusted or updated.
	 */
	virtual void Settings(const FSettings<FFeatureReport>& Settings) override;
	virtual void Settings(const FDualSenseFeatureReport& Settings);

	/**
	 * @brief Initializes the DualSense library with the specified device context.
	 *
	 * This method sets up the library with the provided device context, configuring
	 * the necessary states for the DualSense controller, such as enabling or disabling
	 * certain features. It ensures the library is prepared to handle input from a connected
	 * DualSense device.
	 *
	 * @param Context A reference to an FDeviceContext structure that contains information
	 * about the current device, such as the connection type and configuration.
	 *
	 * @return Returns true if the library was successfully initialized.
	 */
	virtual bool InitializeLibrary(const FDeviceContext& Context) override;
	/**
	 * @brief Shuts down the DualSense library and releases related resources.
	 *
	 * This method is responsible for halting the operation of the DualSense library,
	 * including resetting button states, clearing device contexts, and releasing
	 * system handles. It ensures that all resources utilized by the library are
	 * properly deallocated to prevent potential memory leaks or resource conflicts.
	 *
	 * @details The method resets the internal button states, zeroes any output data
	 * associated with the HID device context, closes the associated device handle,
	 * and frees any allocated HID device contexts via the DualSense HID Manager.
	 * Once completed, a log message is generated to confirm the successful shutdown.
	 */
	virtual void ShutdownLibrary() override;
	/**
	 * @brief Reconnects a DualSense controller to the input system.
	 *
	 * The Reconnect method signals the input system to re-establish a connection
	 * with a DualSense controller, ensuring it is recognized as an active input device.
	 * This is done by broadcasting a connection change event reflecting the controller's
	 * connected state.
	 *
	 * @details This method triggers a connection event through the input device mapper,
	 * associating the controller with the appropriate user and input device identifiers.
	 * It is useful in scenarios where the controller's connection state needs to be reset
	 * or when manually managing input device connections in the application.
	 */
	virtual void Reconnect() override;
	/**
	 * @brief Checks if a DualSense device is currently connected.
	 *
	 * This method determines whether a DualSense controller is actively connected
	 * to the system. It queries the underlying device context to assess the connection
	 * status and provides a boolean value indicating the result.
	 *
	 * @return True if a DualSense controller is connected; otherwise, false.
	 */
	virtual bool IsConnected() override;
	/**
	 * @brief Sends output data to the connected DualSense controller.
	 *
	 * This method is responsible for transmitting data to the DualSense
	 * controller through the HID device context. It ensures that the
	 * output buffering mechanism processes the data correctly before
	 * managing communication with the controller.
	 *
	 * @details The method first verifies the connection status of the HID
	 * device context. If the device is not connected, the operation is aborted.
	 * Otherwise, it delegates the responsibility of handling the output
	 * buffering to the appropriate manager, ensuring proper data flow to the device.
	 */
	virtual void SendOut() override;
	/**
	 * @brief Handles button input events for a DualSense controller.
	 *
	 * This function checks the current state of a button on a DualSense controller and triggers
	 * appropriate input events (button press or release) based on the change in its state.
	 *
	 * @param InMessageHandler The message handler responsible for dispatching input events.
	 * @param UserId The platform user ID associated with the controller.
	 * @param InputDeviceId The unique identifier for the DualSense input device.
	 * @param ButtonName The name of the button being checked.
	 * @param IsButtonPressed A boolean indicating the current pressed state of the button (true if pressed, false otherwise).
	 */
	virtual void CheckButtonInput(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
								  const FPlatformUserId UserId, const FInputDeviceId InputDeviceId,
								  const FName ButtonName, const bool IsButtonPressed);
	/**
	 * @brief Updates the input state for a DualSense device.
	 *
	 * This method processes input data received from a DualSense controller and updates
	 * the application's state via the provided message handler. It ensures that the input
	 * is correctly mapped and associated with the specified platform user and input device.
	 *
	 * @param InMessageHandler A shared reference to the application's message handler that processes input events.
	 * @param UserId The identifier for the platform user associated with the input device.
	 * @param InputDeviceId The unique identifier of the input device to be updated.
	 * @return A boolean value indicating whether the input update was successful.
	 */
	virtual bool UpdateInput(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
	                         const FPlatformUserId UserId, const FInputDeviceId InputDeviceId) override;

	/**
	 * Retrieves the current battery level of the DualSense controller.
	 *
	 * @return The battery level as a floating-point value, representing the charge percentage or level.
	 */
	virtual float GetBattery() const override
	{
		return LevelBattery;
	}
	/**
	 * @brief Sets the controller ID for the instance.
	 *
	 * This method is used to assign a specific identifier to the controller, which
	 * can be utilized to differentiate between multiple controllers or devices in use.
	 * The method overrides an existing function in the parent class, ensuring consistent
	 * behavior across derived implementations.
	 *
	 * @param Id The unique identifier to assign to the controller.
	 */
	virtual void SetControllerId(int32 Id) override
	{
		ControllerID = Id;
	}
	/**
	 * @brief Provides access to the implementation of the Sony gamepad interface.
	 *
	 * This method retrieves the current instance as an implementation of the
	 * ISonyGamepadInterface, ensuring that the object adheres to the interface's
	 * expected functionality. It is typically used to expose the interface implementation
	 * for interaction with other components or systems that rely on the ISonyGamepadInterface.
	 *
	 * @return A pointer to the current instance as an ISonyGamepadInterface.
	 */
	virtual ISonyGamepadInterface* Get() override
	{
		return this;
	}
	/**
	 * @brief Sets the haptic feedback values for the specified hand of the DualSense controller.
	 *
	 * This function configures the haptic feedback characteristics (e.g., frequency) for either
	 * the left or right hand side of the controller, or for both hands when applicable.
	 *
	 * It allows developers to adjust the haptic feedback strength and feel dynamically
	 * during gameplay to enhance the user's immersive experience.
	 *
	 * @param Hand Specifies the hand for which the haptic feedback should be applied.
	 *             Possible values include:
	 *             - EControllerHand::Left for the left hand.
	 *             - EControllerHand::Right for the right hand.
	 *             - EControllerHand::AnyHand for both hands.
	 * @param Values Pointer to the structure containing haptic feedback parameters to be applied,
	 *               including properties such as frequency of the vibration.
	 */
	virtual void SetHapticFeedback(int32 Hand, const FHapticFeedbackValues* Values);
	/**
	 * @brief Configures and applies an automatic gun effect to the specified controller hand using DualSense triggers.
	 *
	 * This function is designed to simulate an automatic gun effect by dynamically adjusting the trigger resistance
	 * and strength profiles of the DualSense controller based on input parameters. It provides a customizable
	 * experience by setting specific strengths for the beginning, middle, and end of the trigger zone, as well as
	 * determining whether the effect should persist.
	 *
	 * @param BeginStrength The initial resistance strength when the trigger is pressed.
	 * @param MiddleStrength The resistance strength at the middle range of the trigger press.
	 * @param EndStrength The resistance strength at the end range of the trigger press.
	 * @param Hand Specifies the target controller hand (e.g., left, right, or both).
	 * @param KeepEffect If true, maintains a predefined effect regardless of the end strength; otherwise, uses the provided strength values.
	 *
	 * @details The function calculates resistance strengths for ten distinct zones of the trigger and applies
	 * them to either the left or right trigger, or both, based on the specified hand. It adjusts the outputs
	 * of the DualSense controller's internal hardware to create the desired effect. The trigger's feedback is
	 * configured in terms of positional amplitudes and strength zones, allowing precise control over the
	 * haptic experience. This method is particularly useful for implementing haptic feedback in shooting mechanics.
	 */
	virtual void SetAutomaticGun(int32 BeginStrength, int32 MiddleStrength, int32 EndStrength,
	                             const EControllerHand& Hand, bool KeepEffect);

	/**
	 * Configures the adaptive trigger on a DualSense controller to apply continuous resistance.
	 *
	 * @param StartPosition The starting position for the resistance in the adaptive trigger (range: 0-8).
	 * @param Strength The intensity of the resistance in the adaptive trigger (range: 0-9).
	 * @param Hand Specifies which controller hand (left, right, or both) the resistance should be applied to.
	 */
	void SetContinuousResistance(int32 StartPosition, int32 Strength, const EControllerHand& Hand);
	/**
	 * @brief Sets the resistance parameters for the DualSense controller's adaptive triggers.
	 *
	 * This method configures the adaptive triggers with the specified resistance values for distinct
	 * positions (beginning, middle, and end strength zones) and applies the settings to the specified
	 * controller hand(s).
	 *
	 * @param BeginStrength The strength value for the beginning zone of the trigger.
	 * @param MiddleStrength The strength value for the middle zone of the trigger.
	 * @param EndStrength The strength value for the end zone of the trigger.
	 * @param Hand The controller hand (left, right, or both) to which the resistance will be applied.
	 */
	void SetResistance(int32 BeginStrength, int32 MiddleStrength, int32 EndStrength, const EControllerHand& Hand);
	/**
	 * Sets the weapon effect on the adaptive triggers of the DualSense controller. This method configures
	 * the trigger mode and strength for the specified trigger zones.
	 *
	 * @param StartPosition The starting position of the trigger zone. Must be within the valid range [0, 8].
	 * @param EndPosition The ending position of the trigger zone. Must be within the valid range [0, 8].
	 * @param Strength The strength of the trigger resistance. Must be within the valid range [0, 8].
	 * @param Hand Specifies which controller hand (left, right, or both) the effect should be applied to.
	 */
	void SetWeapon(int32 StartPosition, int32 EndPosition, int32 Strength, const EControllerHand& Hand);
	/**
	 * Configures the bow effects for the DualSense adaptive triggers based on specified parameters.
	 *
	 * @param StartPosition The starting position of the trigger effect range (0-8).
	 * @param EndPosition The ending position of the trigger effect range (0-8).
	 * @param BegingStrength The strength of the trigger at the starting position (1-8).
	 * @param EndStrength The strength of the trigger at the ending position (1-8).
	 * @param Hand The controller hand (Left, Right, or AnyHand) to apply the effect to.
	 */
	void SetBow(int32 StartPosition, int32 EndPosition, int32 BegingStrength, int32 EndStrength,
	                   const EControllerHand& Hand);
	/**
	 * Sets machine effects for the DualSense controller's adaptive triggers.
	 *
	 * @param StartPosition The starting position of the effect, where the haptic feedback begins.
	 * @param EndPosition The ending position of the effect, where the haptic feedback stops.
	 * @param AmplitudeBegin The amplitude value at the start of the effect, determining the vibration strength.
	 * @param AmplitudeEnd The amplitude value at the end of the effect, determining the vibration strength.
	 * @param Frequency The frequency of the vibration, controlling the oscillation speed.
	 * @param Period The time period of the vibration effect, influencing its duration.
	 * @param Hand The hand (Left, Right, or AnyHand) to apply the effect to.
	 */
	void SetMachine(int32 StartPosition, int32 EndPosition, int32 AmplitudeBegin, int32 AmplitudeEnd,
	                       float Frequency, float Period, const EControllerHand& Hand);
	/**
	 * @brief Configures the galloping effect for a controller's trigger.
	 *
	 * This method sets up a galloping haptic effect on the specified controller hand.
	 * It allows customization of the effect parameters such as the start and end positions,
	 * the intensity at specific points (FirstFoot and SecondFoot), and the frequency of the effect.
	 *
	 * @param StartPosition The starting position of the effect zone on the trigger (range-dependent on controller configuration).
	 * @param EndPosition The ending position of the effect zone on the trigger (range-dependent on controller configuration).
	 * @param FirstFoot Intensity of the first foot in the galloping effect, represented as an integer.
	 * @param SecondFoot Intensity of the second foot in the galloping effect, represented as an integer.
	 * @param Frequency The frequency of the galloping effect, typically a value between 0.0 and 1.0 representing normalized intensity.
	 * @param Hand Specifies which controller hand the effect applies to (Left, Right, or AnyHand).
	 */
	void SetGalloping(int32 StartPosition, int32 EndPosition, int32 FirstFoot, int32 SecondFoot, float Frequency,
	                  const EControllerHand& Hand);
	/**
	 * Sets the LED player indicator effects based on the desired player LED pattern and brightness intensity.
	 *
	 * @param Led The LED pattern to be displayed on the device, represented by the ELedPlayerEnum enumeration.
	 * @param Brightness The brightness intensity for the LED, represented by the ELedBrightnessEnum enumeration.
	 */
	void SetPlayerLed(ELedPlayerEnum Led, ELedBrightnessEnum Brightness);
	/**
	 * Sets the microphone LED effects on the DualSense controller.
	 *
	 * @param Led The desired LED microphone effect to be applied. It is defined by the ELedMicEnum enumeration,
	 *               which includes options such as MicOff, MicOn, and Pulse.
	 */
	void SetMicrophoneLed(ELedMicEnum Led);
	/**
	 * @brief Sets the lightbar color and behavior on the device.
	 *
	 * This method allows customizing the lightbar by specifying its color,
	 * brightness transition duration, and toggle interval. It is used
	 * to control the visual feedback on devices that have a lightbar feature.
	 *
	 * @param Color The desired color of the lightbar, represented as an FColor.
	 * @param BrithnessTime The duration for transitioning the brightness, in seconds.
	 * @param ToggleTime The duration for toggling the lightbar on and off, in seconds.
	 */
	virtual void SetLightbar(FColor Color, float BrithnessTime = 0.0f, float ToggleTime = 0.0f) override;
	/**
	 * @brief Configures trigger properties for a DualSense controller.
	 *
	 * This method adjusts the input trigger resistance or behavior based on the
	 * properties provided in the `Values` parameter. It specifically processes
	 * configurations for left and right triggers, setting their resistance zones,
	 * activation strength, and associated levels.
	 *
	 * @param Values A pointer to an `FInputDeviceProperty` structure that defines
	 *        the desired properties to configure the triggers. It may include
	 *        attributes such as the start and end positions of resistance zones,
	 *        strength levels, and the specific triggers to be affected (e.g., left,
	 *        right, or both triggers).
	 */
	virtual void SetTriggers(const FInputDeviceProperty* Values) override;
	/**
	 * @brief Updates the vibration feedback for a DualSense controller using force feedback values.
	 *
	 * This method takes in force feedback values and applies the corresponding vibration settings
	 * to the DualSense controller's left and right motors. The vibration levels for each motor are
	 * computed based on the provided force feedback values and are sent to the device to update
	 * its output state.
	 *
	 * @param Vibration The force feedback values representing the intensity of vibrations for the
	 * left and right motors. These values determine how strong the vibrations will be for the
	 * respective motors.
	 */
	virtual void SetVibration(const FForceFeedbackValues& Vibration) override;
	/**
	 * @brief Configures controller vibration intensity based on audio feedback values.
	 *
	 * This method calculates vibration intensity for a DualSense controller, using input
	 * force feedback values and specified parameters such as threshold, exponent curve,
	 * and base multiplier. The vibrations are applied separately to the left and right
	 * motors of the controller.
	 *
	 * @param Vibration A structure containing the left and right force feedback values,
	 *                  which represent the input intensities for computing vibration levels.
	 * @param Threshold The minimum input intensity required for vibration to be triggered.
	 *                  Inputs below this value will produce no output. Defaults to 0.015f.
	 * @param ExponentCurve The exponential factor applied to calculate the curve of the
	 *                      vibration intensity. This modifies how the input intensity
	 *                      maps to the output intensity. Defaults to 2.f.
	 * @param BaseMultiplier A scaling factor applied to the computed intensity, allowing
	 *                       for fine-tuned control over the overall vibration strength.
	 *                       Defaults to 1.5f.
	 */
	void SetVibrationAudioBased(const FForceFeedbackValues& Vibration, float Threshold, float ExponentCurve,
	                            float BaseMultiplier);

	/**
	 * Sets the connection state of a phone.
	 *
	 * @param HasConnected Indicates whether the phone is connected (true) or not (false).
	 */
	void SetHasPhoneConnected(bool HasConnected);
	/**
	 * Sets the battery level of the DualSense device.
	 *
	 * @param Level The current battery level as a float value.
	 * @param FullyCharged Indicates if the battery is fully charged.
	 * @param Charging Indicates if the device is currently charging.
	 */
	void SetLevelBattery(float Level, bool FullyCharged, bool Charging);
	/**
	 * @class UDualSenseLibrary
	 * @brief Provides functionality for interacting with the DualSense controller's advanced features.
	 *
	 * The UDualSenseLibrary class contains methods to enable and manage specific hardware features
	 * of the DualSense controller, such as accelerometer and gyroscope functionality.
	 */
	void SetAcceleration(bool bIsAccelerometer);
	/**
	 * @brief Sets the state of the gyroscope functionality.
	 *
	 * This method enables or disables the gyroscope functionality in the system based on the passed parameter.
	 *
	 * @param bIsGyroscope A boolean value indicating whether to enable or disable the gyroscope.
	 *                     Pass true to enable the gyroscope or false to disable it.
	 */
	void SetGyroscope(bool bIsGyroscope);
	/**
	 * Sets the touch functionality state for the DualSense controller.
	 *
	 * @param bIsTouch Boolean flag indicating whether the touch functionality should be enabled (true) or disabled (false).
	 */
	void SetTouch(bool bIsTouch);
	
	/**
	 * Stops any ongoing adaptive trigger effects on the specified controller hand.
	 *
	 * @param Hand The hand for which to stop the adaptive trigger effect.
	 *             Acceptable values are EControllerHand::Left, EControllerHand::Right,
	 *             or EControllerHand::AnyHand.
	 */
	void StopTrigger(const EControllerHand& Hand);
	/**
	 * @brief Stops all ongoing input and feedback operations on the DualSense controller.
	 *
	 * The StopAll function halts all active feedback features such as LED lights,
	 * trigger feedback, and other customizable components on the DualSense controller.
	 * It resets the state of the controller's visual and haptic features.
	 *
	 * @details This function modifies the controller's state by turning off the brightness
	 * of the player LED, deactivating lightbar and trigger modes, and updating the lightbar
	 * colors based on the assigned ControllerID. Each ControllerID is associated with a
	 * specific lightbar color to represent different controller states.
	 *
	 * The function ensures consistent controller deactivation for any connected players
	 * by resetting features to their default state and immediately sending the state update
	 * to the hardware. This can be used in scenarios where the controller must be brought
	 * into a neutral state, such as when pausing gameplay or shutting down the system.
	 */
	void StopAll();
	/**
	 * @brief Retrieves the connection type of the device.
	 *
	 * This method provides information about the connection type of the device,
	 * such as whether it is connected via USB, Bluetooth, or another supported
	 * connection type. It overrides the base class implementation to return
	 * the specific connection type associated with the HID device context.
	 *
	 * @return The connection type of the device as an enumerated value of type EDeviceConnection.
	 */
	virtual EDeviceConnection GetConnectionType() override
	{
		return HIDDeviceContexts.ConnectionType;	
	}
	/**
	 * @brief Retrieves the type of device associated with the current context.
	 *
	 * This method provides the specific type of device being used, as represented
	 * by the associated device type enumeration. The retrieved value can be utilized
	 * to determine the characteristics or capabilities of the device in use.
	 *
	 * @return The device type as an enumeration value of EDeviceType.
	 */
	virtual EDeviceType GetDeviceType() override
	{
		return HIDDeviceContexts.DeviceType;	
	}
	/**
	 * Represents the unique identifier assigned to a specific DualSense controller.
	 *
	 * This variable is used to differentiate connected DualSense controllers, enabling the system
	 * to identify which controller instance corresponds to ongoing operations or events. The
	 * `ControllerID` is integral to updating input, haptic feedback, LED settings, and managing
	 * individual device states in multi-controller environments.
	 */
	int32 ControllerID;
	/**
	 * A map representing the states of various buttons on a controller.
	 *
	 * Each key in the map is a button name (FName), and its associated value is a boolean
	 * indicating whether the button is currently pressed (`true`) or not pressed (`false`).
	 *
	 * This variable is primarily used for tracking button input states and ensuring
	 * accurate representation of input events, such as detecting when a button's state
	 * changes from pressed to released or vice versa.
	 *
	 * The map is updated dynamically during controller runtime, including in functions
	 * like CheckButtonInput, which ensures real-time synchronization of input states.
	 * It is reset during library shutdown to clear all stored button states.
	 */
	TMap<const FName, bool> ButtonStates;
	
protected:
	/**
	 * @brief The PlatformInputDeviceMapper is responsible for mapping platform-specific
	 *        input device data to a standardized format used within the application.
	 *
	 * This variable is part of the input handling system and plays a crucial role in
	 * ensuring compatibility between different platform input devices and the application's
	 * input handling logic. It abstracts the differences in input device representations
	 * across operating systems or platforms, providing a unified interface for accessing
	 * input data.
	 *
	 * Typical use cases include mapping device identifiers, translating input codes, and
	 * adapting platform-specific input behavior to match application requirements.
	 *
	 * The PlatformInputDeviceMapper handles tasks such as:
	 * - Mapping raw input device identifiers to application-specific identifiers.
	 * - Normalizing input event data across diverse platforms and devices.
	 * - Facilitating cross-platform input device compatibility.
	 */
	static FGenericPlatformInputDeviceMapper PlatformInputDeviceMapper;
	
private:
	/**
	 * @brief A variable that indicates whether touch functionality is enabled or disabled.
	 *
	 * This variable is used to toggle the touch input capability of the system or application.
	 * When set to true, touch input is enabled, allowing the system to respond to touch events.
	 * When set to false, touch input is disabled, and touch interactions are ignored.
	 */
	bool EnableTouch;
	/**
	 * Indicates whether a phone is connected to the system.
	 *
	 * This variable is used to store the connection status of a phone.
	 * It is updated by the SetHasPhoneConnected method.
	 * - `true`: A phone is connected.
	 * - `false`: No phone is connected.
	 */
	bool HasPhoneConnected;
	/**
	 * @brief Represents the current level of the battery.
	 *
	 * This variable is used to store the battery percentage or charge level,
	 * typically as an integer or floating-point value. It can be used to
	 * monitor battery status in various applications and scenarios.
	 *
	 * Note: The scale and representation (e.g., percentage, voltage) should
	 * be specified in the implementation or context where the variable is used.
	 */
	float LevelBattery;
	/**
	 * @brief Represents the feedback intensity for the left trigger of a DualSense controller.
	 *
	 * This variable is used to store the current level of feedback applied to the left trigger (L2 button).
	 * The value typically ranges from 0.0 to 1.0, where 0.0 signifies no feedback and 1.0 represents maximum feedback intensity.
	 */
	float LeftTriggerFeedback;
	/**
	 * @brief Represents the feedback intensity or state for the right trigger in a controller.
	 *
	 * This variable is used to specify or retrieve the vibration/feedback level
	 * applied to the right trigger, which can enhance user experience during gameplay
	 * or interactive scenarios by providing tactile feedback.
	 *
	 * The range and interpretation of the value may depend on the specific implementation
	 * or hardware being used.
	 */
	float RightTriggerFeedback;
	/**
	 * @variable EnableAccelerometerAndGyroscope
	 * @brief Flags the activation of accelerometer and gyroscope sensors in the system.
	 *
	 * This variable determines whether the accelerometer and gyroscope functionalities
	 * are enabled for the system. When set to true, data from these sensors will be collected
	 * and utilized, typically for motion detection or orientation tracking.
	 *
	 * @details This flag is often used in systems that require motion input for functionality,
	 * such as gaming controllers, virtual reality devices, or motion-sensing applications.
	 * Disabling this may reduce resource usage but will disable motion-based features.
	 */
	bool EnableAccelerometerAndGyroscope;
	/**
	 * @brief Represents the context of a Human Interface Device (HID) used by DualSense controllers.
	 *
	 * This variable holds the FDeviceContext structure, which encapsulates the necessary state and
	 * configuration details for managing the connection, communication, and interaction with a
	 * specific DualSense gamepad device. It is primarily used within the UDualSenseLibrary class for
	 * initialization, input handling, and managing device-specific settings.
	 */
	FDeviceContext HIDDeviceContexts;
};
