# Windows Dualsense for Unreal Engine 5.x

### Plugin to integrate Dualsense PS5 devices (controllers) into Unreal Engine 5.x for Windows platforms.

### No necessary configuration to run the control.

## Supports settings of triggers, haptic feedback triggers, unreal native force feedback blueprint, vibrations, leds, battery level, gyroscope, accelerometer etc..

### Usage methods available via C++.

```
#include "DualSenseProxy.h"

void APlayerController::BeginPlay()
{
    Super::BeginPlay();
	
    int32 ControllerId = 0; 
    
    // It is necessary to change the value to false in PlayerController if HapticFeedback is to be used. SetDisableHaptics(true) is set by default.
    SetDisableHaptics(false);

    // Reset buffer all values 
    UDualSenseProxy::ResetEffects(ControllerId);
    
    // Gyroscope and Accelerometer are set to false by default. Calibration needs to be implemented
    UDualSenseProxy::EnableAccelerometerValues(0, false);
    UDualSenseProxy::EnableGyroscopeValues(0, false);

    // Touch pad values default false, values max 1.0f
    UDualSenseProxy::EnableTouch1(0, false);
    UDualSenseProxy::EnableTouch2(0, false);

    // Level battery Full load max 1.0f 
    float levelBattery = UDualSenseProxy::LevelBatteryDevice(0);

    // Leds configs
    UDualSenseProxy::LedMicEffects(0, ELedMicEnum::MicOn);
    UDualSenseProxy::LedPlayerEffects(0, ELedPlayerEnum::One, ELedBrightnessEnum::Medium);
    UDualSenseProxy::LedColorEffects(0, FColor(255, 255, 255));

    // Stop triggers effects
    UDualSenseProxy::StopAllTriggersEffects(0);
    UDualSenseProxy::StopTriggerEffect(ControllerId, EControllerHand::Left);
    UDualSenseProxy::StopTriggerEffect(ControllerId, EControllerHand::Right);

    // Normalize triggers
    UDualSenseProxy::EffectNoResitance(ControllerId, EControllerHand::Left);
    UDualSenseProxy::EffectNoResitance(ControllerId, EControllerHand::Right);

    // Start position max value 9 | Force max value 8
    UDualSenseProxy::EffectContinuousResitance(ControllerId, 5, 8, EControllerHand::Left); 
    UDualSenseProxy::EffectContinuousResitance(ControllerId, 1, 4, EControllerHand::Right);

    // Start and end positions max value 9
    UDualSenseProxy::EffectSectionResitance(ControllerId, 1, 9, EControllerHand::Left); 
    UDualSenseProxy::EffectContinuousResitance(ControllerId, 5, 9, EControllerHand::Right);

    // Example Haptics Effects...

    // Start position max value 9 | Forces max value 8 
    UDualSenseProxy::SetTriggerHapticFeedbackEffect(ControllerId, 9, 0, 0, 5, EControllerHand::Left, true);
    UDualSenseProxy::SetTriggerHapticFeedbackEffect(ControllerId, 9, 0, 0, 6, EControllerHand::Right, true);

    // SetHapticsByValue is a method of PlayerController.
    SetHapticsByValue(0.1f, 1.0f, EControllerHand::Left);
    SetHapticsByValue(1.0f, 1.0f, EControllerHand::Right);
}

```

### Usage via Blueprints

![Unreal Editor](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/Images/Metodos.gif)

![Unreal Editor](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/Images/Usage.gif)

### Basic example to check connection, reconnect device, and apply LEDs to the DualSense.

![Unreal Editor](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/Images/ExampleBasic.png)

### Haptic trigger feedback

![Unreal Editor](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/Images/haptic1.png)

# Installation
Download the compiled plugin **Windows x64**
[Download plugin](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/WindowsDualsense_ds5w.zip)

Extract the file to

```C:\Program Files\Epic Games\UE_[version]\Engine\Plugins```

Now enable the plugin in the Unreal Editor, connect your DualSense device, and restart the editor.

![Unreal Editor](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/Images/Install.gif)


After restarting the engine, the following message will appear in the bottom right corner:

![Unreal Editor](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/Images/IsConnection.png)

### Native Gamepad Mapping in Unreal

![Unreal Editor](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/Images/Buttons.gif)

### The keys for the Gyroscope and Accelerometer are as follows:

![Unreal Editor](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/Images/GyroscopeBTN.gif)

You don’t need to map the keys. By default, the plugin already maps the standard Gamepad layout.

> **NOTE:** Enable Gyroscope, Accelerometer, and Touch only after mapping the actions to avoid interfering with button press recognition, as Gyroscope, Accelerometer, and Touch constantly emit values.


![Unreal Editor](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/Images/Mapp.gif)

### The plugin is compatible with Unreal's native Blueprints Force Feedback, for example:

Config

![Unreal Editor](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/Images/Feedback.png)

Result

![Unreal Editor](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/Images/DS_Led.jpg)


## Contributions

Thanks to the [DualSense on Windows API](https://github.com/Ohjurot/DualSense-Windows) library, which facilitated the development of this plugin.
