// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"
#include "EDeviceCommons.generated.h"

#define BTN_FN1          0x10
#define BTN_FN2          0x20
#define BTN_PADDLE_LEFT  0x40
#define BTN_PADDLE_RIGHT 0x80

#define BTN_DPAD_UP 0x8
#define BTN_DPAD_DOWN 0x02
#define BTN_DPAD_LEFT 0x01
#define BTN_DPAD_RIGHT 0x04

#define BTN_CROSS 0x20
#define BTN_SQUARE 0x10
#define BTN_CIRCLE 0x40
#define BTN_TRIANGLE 0x80

#define BTN_START 0x20
#define BTN_SELECT 0x10

#define BTN_LEFT_STICK 0x40
#define BTN_RIGHT_STICK 0x80

#define BTN_LEFT_SHOLDER 0x01
#define BTN_RIGHT_SHOLDER 0x02

#define BTN_LEFT_TRIGGER 0x04
#define BTN_RIGHT_TRIGGER 0x08

#define BTN_PAD_BUTTON 0x02
#define BTN_MIC_BUTTON 0x04
#define BTN_PLAYSTATION_LOGO 0x01

#define PLAYER_LED_LEFT 0x01
#define PLAYER_LED_RIGHT 0x10
#define PLAYER_LED_MIDDLE 0x04

#define PLAYER_LED_MIDDLE_LEFT 0x02
#define PLAYER_LED_MIDDLE_RIGHT 0x08

/**
 * @brief Enum class representing various LED microphone states.
 *
 * This enum class is used to define the states for LED microphone controls on a compatible device.
 *
 * Enum values:
 * - MicOff: Represents the "Mic Off" state.
 * - MicOn: Represents the "Mic On" state.
 * - Pulse: Represents the "Pulse" state.
 */
UENUM(BlueprintType)
enum class ELedMicEnum : uint8
{
	MicOff = 0x0 UMETA(DisplayName = "Mic Off"),
	MicOn = 0x1 UMETA(DisplayName = "Mic On"),
	Pulse = 0x2 UMETA(DisplayName = "Pulse")
};

/**
 * @enum ELedPlayerEnum
 * Enumeration for controlling DualSense LED light player effects.
 */
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

/**
 * @brief Enum defining brightness levels for LED effects on the DualSense controller.
 *
 * This enumeration specifies different brightness levels that can be assigned
 * to LED effects. It is designed to work with Blueprint environments.
 */
UENUM(BlueprintType)
enum class ELedBrightnessEnum : uint8
{
	Low = 0x02 UMETA(DisplayName = "Brightness Low"),
	Medium = 0x01 UMETA(DisplayName = "Brightness Medium"),
	High = 0x00 UMETA(DisplayName = "Brightness High")
};

/**
 * @brief Enum class representing audio feature states for a DualSense controller.
 *
 * This enum class is utilized to manage the audio-related features of a DualSense controller,
 * such as enabling or disabling audio functionality.
 *
 * Enum values:
 * - On: Indicates that the audio is enabled.
 * - Off: Indicates that the audio is disabled.
 */
UENUM(BlueprintType)
enum class EDualSenseAudioFeatureReport : uint8
{
	On = 0x00 UMETA(DisplayName = "Audio On"),
	Off = 0x10 UMETA(DisplayName = "Audio Off")
};

/**
 * @brief Enum class representing feature report states for a DualSense device.
 *
 * This enum class is used to define different modes or states of operational features
 * for DualSense devices, such as vibration or haptic feedback behaviors.
 *
 * Enum values:
 * - Off: Represents the "Off" state, indicating no active feature.
 * - DefaultRumble: Represents the default rumble behavior of the device.
 * - HapticSoftRumble: Represents a softer haptic feedback mode, often used for advanced vibration control.
 */
UENUM(BlueprintType)
enum class EDualSenseDeviceFeatureReport : uint8
{
	Off = 0,
	DefaultRumble = 0xFF UMETA(DisplayName = "Default Rumble"),
	HapticSoftRumble = 0xFC UMETA(DisplayName = "Haptic Soft Rumble")
};

/**
 * @brief Enum class defining the softness levels for DualSense controller triggers.
 *
 * This enum specifies different levels of trigger softness for DualSense controllers, which can
 * be used to adjust the tactile feedback of the triggers based on the desired gaming experience.
 *
 * Enum values:
 * - Off: Disables trigger softness entirely.
 * - VeryRigid: Represents a very rigid trigger feedback.
 * - Rigid: Represents rigid trigger feedback.
 * - Medium: Represents a medium level of softness for the trigger.
 * - Soft: Represents a soft trigger feedback.
 * - VerySoft: Represents a very soft trigger feedback.
 */
UENUM(BlueprintType)
enum class EDualSenseTriggerSoftnessLevel : uint8
{
	Off = 0,
	VeryRigid = 1 UMETA(DisplayName = "Very Rigid"),
	Rigid = 2 UMETA(DisplayName = "Rigid"),
	Medium = 4 UMETA(DisplayName = "Medium"),
	Soft = 6 UMETA(DisplayName = "Soft"),
	VerySoft = 8 UMETA(DisplayName = "Very Soft")
};

/**
 * FDeviceSettings is a base structure intended to represent the general configuration settings
 * for device-related features. It is designed as a BlueprintType to allow usage within Unreal Engine
 * Blueprints, providing greater flexibility for developers to expose and utilize its properties in a visual scripting environment.
 *
 * This struct serves as a foundational building block and can be extended by other structures
 * to include specific device settings for custom hardware or software interaction.
 *
 * Derived classes may override or expand upon the settings defined in FDeviceSettings to handle
 * specific device configuration requirements or features.
 *
 * Examples of utilization may include configuration management, runtime device customization,
 * or initializing required parameters for hardware integration.
 *
 * Commonly used in interfaces or libraries where device-specific configurations are abstracted
 * and reused across various systems, promoting consistency and modularity.
 */
USTRUCT(BlueprintType)
struct FDeviceSettings
{
	GENERATED_BODY()
	
};

/**
 * Struct representing the feature report settings of a DualSense controller.
 * This struct is used to configure various audio and haptic feedback features
 * for a connected DualSense controller.
 */
USTRUCT(BlueprintType)
struct FDualSenseFeatureReport : public FDeviceSettings
{
	GENERATED_BODY()

	/**
	 * Represents the microphone status of the DualSense controller audio settings.
	 * This variable utilizes the EDualSenseAudioFeatureReport enum to toggle the microphone feature.
	 * The microphone can be toggled On or Off, influencing the device's audio configuration.
	 *
	 * Category: DualSense Settings
	 * Blueprint Read/Write: Enabled
	 * Possible Values:
	 *   - EDualSenseAudioFeatureReport::On: Enables the microphone.
	 *   - EDualSenseAudioFeatureReport::Off: Disables the microphone.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DualSense Settings")
	EDualSenseAudioFeatureReport MicStatus;
	/**
	 * Specifies the operational state of the audio headset on a DualSense controller.
	 * The variable can be configured to enable or disable the headset audio functionality.
	 *
	 * Category:
	 * - DualSense Settings
	 *
	 * Access:
	 * - Editable anywhere in the editor.
	 * - Readable and writable in Blueprints.
	 *
	 * Enum:
	 * - EDualSenseAudioFeatureReport::On: Enables headset audio.
	 * - EDualSenseAudioFeatureReport::Off: Disables headset audio.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DualSense Settings")
	EDualSenseAudioFeatureReport AudioHeadset;
	/**
	 * Represents the audio speaker settings for a DualSense device.
	 *
	 * This variable allows enabling or disabling the speaker functionality
	 * on the DualSense device. It uses the EDualSenseAudioFeatureReport
	 * enum to specify the state of the speaker, either "On" or "Off".
	 *
	 * Category: DualSense Settings
	 * Access: Readable and writable in Blueprints
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DualSense Settings")
	EDualSenseAudioFeatureReport AudioSpeaker;
	/**
	 * Specifies the vibration mode for the DualSense device. Controls the type of haptic feedback
	 * or vibration effect to be applied on the controller using the available feature report.
	 *
	 * The possible values are:
	 * - EDualSenseDeviceFeatureReport::Off: Disables vibration feedback.
	 * - EDualSenseDeviceFeatureReport::DefaultRumble: Enables default rumble feedback.
	 * - EDualSenseDeviceFeatureReport::HapticSoftRumble: Enables soft haptic rumble feedback.
	 *
	 * This property can be edited within the editor and accessed in Blueprints, allowing advanced customization
	 * of the DualSense controller's haptic behavior to provide a tailored user experience.
	 *
	 * Category: DualSense Settings
	 *
	 * Display Name: Soft haptic feedback, advanced vibrate
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DualSense Settings",
		meta=(DisplayName = "Soft haptic feedback, adivanced vibrate"))
	EDualSenseDeviceFeatureReport VibrationMode;

	/**
	 * The microphone volume level for the DualSense device.
	 *
	 * This variable determines the intensity of the microphone input, allowing users
	 * to adjust the microphone sensitivity or volume output. The value is clamped between 0 and 100.
	 *
	 * - Category: DualSense Settings
	 * - Range: 0 (minimum volume) to 100 (maximum volume)
	 * - Metadata:
	 *   - ClampMin: 0
	 *   - ClampMax: 100
	 *   - UIMin: 0
	 *   - UIMax: 100
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DualSense Settings",
		meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
	int32 MicVolume;
	/**
	 * Represents the audio volume setting for a DualSense device.
	 * This value determines the volume level applied to the audio output, including the headset and speaker.
	 *
	 * - The valid range for this property is from 0 to 100, where:
	 *   - 0 represents no audio output (muted).
	 *   - 100 represents the maximum audio output volume.
	 * - Values outside this range are clamped automatically.
	 *
	 * This property can be edited in the editor or accessed in Blueprints. It appears under the "DualSense Settings" category in the editor.
	 *
	 * Metadata:
	 * - ClampMin: 0 (Minimum allowable value for automatic clamping)
	 * - ClampMax: 100 (Maximum allowable value for automatic clamping)
	 * - UIMin: 0 (Minimum value shown in UI sliders)
	 * - UIMax: 100 (Maximum value shown in UI sliders)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DualSense Settings",
		meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
	int32 AudioVolume;

	/**
	 * Determines the reduction intensity of the soft rumble effect for a DualSense device.
	 *
	 * The value represents the level of adjustment applied to the soft haptic feedback vibration,
	 * ranging from 0 (no reduction) to 15 (maximum reduction).
	 *
	 * The value is clamped and displayed in the UI with a minimum of 0 and a maximum of 15.
	 *
	 * Category:
	 * - DualSense Settings
	 *
	 * Metadata:
	 * - ClampMin: 0
	 * - ClampMax: 15
	 * - UIMin: 0
	 * - UIMax: 15
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DualSense Settings",
		meta = (ClampMin = "0", ClampMax = "15", UIMin = "0", UIMax = "15"))
	int32 SoftRumbleReduce;
	/**
	 * Specifies the softness level of the adaptive triggers on a DualSense controller.
	 *
	 * This property allows fine-tuning of the trigger pressure resistance, ranging from "Off" to "Very Soft".
	 * Users can adjust the value in both the editor and at runtime via Blueprints.
	 *
	 * Valid range:
	 * - Minimum: 0 (Off)
	 * - Maximum: 15 (Very Soft)
	 *
	 * The property is clamped between 0 and 15, and the UI provides a slider ranging between these values
	 * for easier configuration.
	 *
	 * Category: DualSense Settings
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DualSense Settings",
		meta = (ClampMin = "0", ClampMax = "15", UIMin = "0", UIMax = "15"))
	EDualSenseTriggerSoftnessLevel TriggerSoftnessLevel;

	/**
	 * Default constructor for the FDualSenseFeatureReport struct.
	 * Initializes various audio and vibration settings for the DualSense controller.
	 *
	 * The constructor sets default states for microphone, audio outputs, vibration, and trigger softness levels.
	 *
	 * @return A default-initialized instance of FDualSenseFeatureReport.
	 */
	FDualSenseFeatureReport():
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
};
