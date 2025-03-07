# Windows Dualsense for Unreal Engine version 5.3, 5.4 and 5.5

### **Plugin with full support for the DualSense PS5 controller in Unreal Engine versions 5.3, 5.4, and 5.5 for Windows platforms. No configuration needed**

### No necessary configuration to run the control.

## Supports settings of triggers, haptic feedback triggers, unreal native force feedback blueprint, vibrations, leds, battery level, gyroscope, accelerometer etc..

### Usage methods available via C++ or Blueprint.

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
    UDualSenseProxy::EnableAccelerometerValues(ControllerId, false);
    UDualSenseProxy::EnableGyroscopeValues(ControllerId, false);

    // Touch pad values default false, values max 1.0f
    UDualSenseProxy::EnableTouch1(ControllerId, false);
    UDualSenseProxy::EnableTouch2(ControllerId, false);

    // Level battery Full load max 1.0f 
    float levelBattery = UDualSenseProxy::LevelBatteryDevice(ControllerId);

    // Leds configs
    UDualSenseProxy::LedMicEffects(ControllerId, ELedMicEnum::MicOn);
    UDualSenseProxy::LedPlayerEffects(ControllerId, ELedPlayerEnum::One, ELedBrightnessEnum::Medium);
    UDualSenseProxy::LedColorEffects(ControllerId, FColor(255, 255, 255));

    // Stop triggers effects
    UDualSenseProxy::StopAllTriggersEffects(ControllerId);
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

[UE 5.3 download plugin WindowsDualsense_ds5w1.0.0.zip](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/WindowsDualsense_ds5w_5.3/WindowsDualsense_ds5w1.0.0.zip)

[UE 5.4 download plugin WindowsDualsense_ds5w1.0.0.zip](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/WindowsDualsense_ds5w_5.4/WindowsDualsense_ds5w1.0.0.zip)

[UE 5.5 download plugin WindowsDualsense_ds5w1.0.0.zip](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/WindowsDualsense_ds5w_5.5/WindowsDualsense_ds5w1.0.0.zip)


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


> **NOTE:** It may be necessary to enable haptic feedback in the project settings, under **Edit > Project Settings > Input**.

![Unreal Editor](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/Images/GamepadEnable.gif)


### The plugin is compatible with Unreal's native Blueprints Force Feedback, for example:

Config

![Unreal Editor](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/Images/Feedback.png)

Result

![Unreal Editor](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/Images/DS_Led.jpg)


## Contributions

Thanks to the [DualSense on Windows API](https://github.com/Ohjurot/DualSense-Windows) library, which facilitated the development of this plugin.
