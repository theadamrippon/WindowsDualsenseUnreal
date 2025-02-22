# Windows Dualsense for Unreal Engine 5.x

### Plugin to integrate Dualsense PS5 devices (controllers) into Unreal Engine 5.x for Windows platforms.

### No necessary configuration to run the control.

## Supports settings of triggers, force feedback, vibrations, leds, battery level, microphone etc..

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
