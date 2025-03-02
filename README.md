# Windows Dualsense for Unreal Engine 5.x

### Plugin to integrate Dualsense PS5 devices (controllers) into Unreal Engine 5.x for Windows platforms.

### No necessary configuration to run the control.

## Supports settings of triggers, force feedback, vibrations, leds, battery level, microphone etc..

```
// Usage methods available via C++ or Blueprints. 
// "It is necessary to change the value to false in PlayerController if HapticFeedback is to be used. SetDisableHaptics(true) is set by default."
SetDisableHaptics(false);

// Reset buffer all values 
UDualSenseProxy::ResetEffects(0);

// Stop triggers effects
UDualSenseProxy::StopAllTriggersEffects(0);
UDualSenseProxy::StopTriggerEffect(0, EControllerHand::Left);
UDualSenseProxy::StopTriggerEffect(0, EControllerHand::Right);

// Normalize triggers
UDualSenseProxy::EffectNoResitance(0, EControllerHand::Left);
UDualSenseProxy::EffectNoResitance(0, EControllerHand::Right);

// Start position max value 9 | Force max value 8
UDualSenseProxy::EffectContinuousResitance(0, 5, 8, EControllerHand::Left); 
UDualSenseProxy::EffectContinuousResitance(0, 1, 4, EControllerHand::Right);

// Start and end positions max value 9
UDualSenseProxy::EffectSectionResitance(0, 1, 9, EControllerHand::Left); 
UDualSenseProxy::EffectContinuousResitance(0, 5, 9, EControllerHand::Right);

// Example Haptics Effects...


// Start position max value 9 | Forces max value 8 
UDualSenseProxy::SetTriggerHapticFeedbackEffect(0, 9, 0, 0, 5, EControllerHand::Left, true);
UDualSenseProxy::SetTriggerHapticFeedbackEffect(0, 9, 0, 0, 6, EControllerHand::Right, true);

// SetHapticsByValue is a method of PlayerController.
SetHapticsByValue(0.1f, 1.0f, EControllerHand::Left);
SetHapticsByValue(1.0f, 1.0f, EControllerHand::Right);

```

# Installation
Download the compiled plugin **Windows x64**
[Download plugin](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/WindowsDualsense_ds5w.zip)

Extract the file to

```C:\Program Files\Epic Games\UE_[version]\Engine\Plugins```

Now enable the plugin in the Unreal Editor, connect your DualSense device, and restart the editor.

![Unreal Editor](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/Images/PluginsEngine.png)


After restarting the engine, the following message will appear in the bottom right corner:

![Unreal Editor](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/Images/IsConnection.png)

You don’t need to map the keys. By default, the plugin already maps the standard Gamepad layout.
You can test it using Enhancement Input.

![Unreal Editor](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/Images/Mapp.gif)

### The plugin is compatible with Unreal's native Blueprints Force Feedback, for example:

Config

![Unreal Editor](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/Images/Feedback.png)

Result

![Unreal Editor](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/Images/DS_Led.jpg)


## Contributions

Thanks to the [DualSense on Windows API](https://github.com/Ohjurot/DualSense-Windows) library, which facilitated the development of this plugin.
