# Windows Dualsense for Unreal Engine version 5.2 ~ 5.5

> **Update Note:**
> **Important Update for WindowsDualsense_ds5w Plugin**

>This update replaces the previous implementation, which relied on a third-party DLL located in the `ThirdParty` directory, with a direct integration using the native Windows HIDAPI.

>If you have cloned the project and still use the older version with the third-party DLL, please update to this latest version to benefit from the native HIDAPI integration. This eliminates the dependency on external DLLs and provides a more streamlined setup.
For convenience, you can also download the latest version as a ZIP file from the repository or documentation links provided.

### **Plugin with full support for the DualSense PS5 controller in Unreal Engine versions  5.2 ~ 5.5, for Windows platforms. No configuration needed**

### The controller's customization commands, such as vibration, haptic feedback, and LEDs, can be implemented directly via C++ or Blueprints. Below, we provide examples of both implementations.

[See the example video](https://1drv.ms/v/c/6c07d40187e87b76/EYPKCwWTTuZGqLC7pVkyGgEBONfwM-6fQKzt-RzBpQsKKg?e=lUp8kh)

### Supports settings of triggers, haptic feedback triggers, unreal native force feedback blueprint, vibrations, leds, battery level, gyroscope, accelerometer etc..

# Usage via Blueprints

![Unreal Editor](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/Images/Metodos.gif)

![Unreal Editor](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/Images/Usage.gif)

# Basic example to check connection, reconnect device, and apply LEDs to the DualSense.

![Unreal Editor](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/Images/ExampleBasic.png)

# Haptic trigger feedback

![Unreal Editor](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/Images/haptic1.png)


# Multiple players with multiple controllers

```
// MyGameModeBase.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PLUGINTESTE_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AMyGameModeBase();
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameMode")
	TSubclassOf<APawn> Player1PawnClass;

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void HandleConnectedControllers(APlayerController* PlayerController);
};
```
```
// MyGameModeBase.cpp


#include "MyGameModeBase.h"

AMyGameModeBase::AMyGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> Player(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C"));
	if (Player.Class != nullptr)
	{
		DefaultPawnClass = Player.Class;
	}
	PlayerControllerClass = APlayerController::StaticClass();
}

void AMyGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void AMyGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	UE_LOG(LogTemp, Warning, TEXT("Player local %d login..."), NewPlayer->GetLocalPlayer()->GetControllerId())

	const int32 PlayerId = NewPlayer->GetLocalPlayer()->GetControllerId();
	if (constexpr int32 MaxPlayer = 2; NewPlayer->IsLocalController() &&  (PlayerId + 1) < MaxPlayer)
	{
		HandleConnectedControllers(NewPlayer);
	}

	if (NewPlayer->IsLocalController() && NewPlayer->GetLocalPlayer()->GetControllerId() > 0)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			FTimerDelegate::CreateLambda([=]()
			{
				FCoreDelegates::OnUserLoginChangedEvent.Broadcast(true, NewPlayer->GetLocalPlayer()->GetControllerId(), NewPlayer->GetLocalPlayer()->GetControllerId());
			}),
			0.2f,
			false
		);
	}
}

void AMyGameModeBase::HandleConnectedControllers(APlayerController* PlayerController)
{
	UE_LOG(LogTemp, Warning, TEXT("Registering new player..."));
	
	if (!GetWorld() || !GetGameInstance() || !PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("GameInstance or World are not available."));
		return;
	}

	FString Error;
	UGameInstance* GameInstance = GetGameInstance();
	if (const ULocalPlayer* NewLocalPlayer = GameInstance->CreateLocalPlayer(PlayerController->GetLocalPlayer()->GetControllerId() + 1, Error, true))
	{
		UE_LOG(LogTemp, Warning, TEXT("New player created for ControllerId: %d"), NewLocalPlayer->GetControllerId());
	}
}
```

# Example of using DualSense effects via C++

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

    // Level battery Full load max 100.0f
    float levelBattery = UDualSenseProxy::LevelBatteryDevice(ControllerId);

    // Leds configs
    UDualSenseProxy::LedMicEffects(ControllerId, ELedMicEnum::MicOn);
    UDualSenseProxy::LedPlayerEffects(ControllerId, ELedPlayerEnum::One, ELedBrightnessEnum::Medium);
    UDualSenseProxy::LedColorEffects(ControllerId, FColor(255, 255, 255));

    // Vibrations example 
    PlayDynamicForceFeedback(0.5f, 3.f, true, true, true, true);
	
    // Stop triggers effects
    UDualSenseProxy::StopAllTriggersEffects(ControllerId);
    UDualSenseProxy::StopTriggerEffect(ControllerId, EControllerHand::Left);
    UDualSenseProxy::StopTriggerEffect(ControllerId, EControllerHand::Right);

    // Normalize triggers
    UDualSenseProxy::EffectNoResitance(ControllerId, EControllerHand::Left);
    UDualSenseProxy::EffectNoResitance(ControllerId, EControllerHand::Right);

    // Start position max value 8 | Force max value 9
    UDualSenseProxy::EffectContinuousResitance(ControllerId, 5, 8, EControllerHand::Left); 
    UDualSenseProxy::EffectContinuousResitance(ControllerId, 1, 4, EControllerHand::Right);

    // Start and end positions max value 8
    UDualSenseProxy::EffectSectionResitance(ControllerId, 1, 8, EControllerHand::Left); 
    UDualSenseProxy::EffectContinuousResitance(ControllerId, 5, 8, EControllerHand::Right);

    // Example Haptics Effects...

    // Start position max value 8 | Forces max value 9 
    UDualSenseProxy::SetTriggerHapticFeedbackEffect(ControllerId, 8, 0, 0, 6, EControllerHand::Left, true);
    UDualSenseProxy::SetTriggerHapticFeedbackEffect(ControllerId, 8, 0, 0, 7, EControllerHand::Right, true);

    // SetHapticsByValue is a method of PlayerController.
    SetHapticsByValue(0.1f, 1.0f, EControllerHand::Left);
    SetHapticsByValue(1.0f, 1.0f, EControllerHand::Right);
}

```

Now enable the plugin in the Unreal Editor, connect your DualSense device, and restart the editor.

![Unreal Editor](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/Images/Install.gif)

### Native Gamepad Mapping in Unreal

![Unreal Editor](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/Images/Buttons.gif)

### The keys for the Gyroscope and Accelerometer are as follows:

![Unreal Editor](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/Images/GyroscopeBTN.gif)

You don’t need to map the keys. By default, the plugin already maps the standard Gamepad layout.

> **NOTE:** Enable Gyroscope, Accelerometer, and Touch only after mapping the actions to avoid interfering with button press recognition, as Gyroscope, Accelerometer, and Touch constantly emit values.


![Unreal Editor](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/Images/Mapp.gif)


> **NOTE:** It may be necessary to enable haptic feedback in the project settings, under **Edit > Project Settings > Input**.

![Unreal Editor](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/Images/GamepadEnable.gif)


### The plugin is compatible with Unreal's native Blueprints Force Feedback

# Manual Installation

Download plugin UE_Version

[UE 5.2 download plugin WindowsDualsense_ds5w_5.2.zip](WindowsDualsense_ds5w_5.2.zip)

[UE 5.3 download plugin WindowsDualsense_ds5w_5.3.zip](WindowsDualsense_ds5w_5.3.zip)

[UE 5.4 download plugin WindowsDualsense_ds5w_5.4.zip](WindowsDualsense_ds5w_5.4.zip)

[UE 5.5 download plugin WindowsDualsense_ds5w_5.5.zip](WindowsDualsense_ds5w_5.5.zip)

# Installing the Plugin in the Project's Directory

## Steps:

1. **Move to the Plugins Folder**  
   Move the cloned repository to your Unreal Engine project’s `Plugins` directory. If the folder does not exist, create it manually:
    - Navigate to your Unreal project’s root directory.
    - Create a folder named `Plugins`, if it doesn’t already exist.
    - Move the cloned repository folder into this newly created `Plugins` folder.

2. **Verify Plugin Installation in Unreal Engine**
    - Open your Unreal Engine project.
    - Go to the "Plugins" section under the `Edit` menu (`Edit > Plugins`).
    - Look for the new plugin in the list and enable it if it is not enabled by default.
    - Restart the Unreal Engine editor.

Move the `WindowsDualsenseUnreal` folder into:

## Contributions

Thanks to the [DualSense on Windows API](https://github.com/Ohjurot/DualSense-Windows) library, which facilitated the development of this plugin.
