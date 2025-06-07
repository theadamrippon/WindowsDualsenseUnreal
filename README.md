# Windows Dualsense for Unreal Engine version 5.2 ~ 5.6

### **Plugin with full support for the DualSense PS5 controller in Unreal Engine versions  5.2 ~ 5.6, for Windows platforms. No configuration needed**

### The controller's customization commands, such as vibration, haptic feedback, and LEDs, can be implemented directly via C++ or Blueprints. Below, we provide examples of both implementations.

### Supports settings of triggers, haptic feedback triggers, unreal native force feedback blueprint, vibrations, leds, battery level, gyroscope, accelerometer etc..

### [Links for installation](#Install-on-FAB-official-page-plugin)

# Usage via Blueprints
### [See the example video](https://1drv.ms/v/c/6c07d40187e87b76/EYPKCwWTTuZGqLC7pVkyGgEBONfwM-6fQKzt-RzBpQsKKg?e=lUp8kh)

### Basic example to check connection, reconnect device, and apply LEDs to the DualSense.

![Unreal Editor](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/Images/ExampleBasic.png)

### Haptic trigger feedback

![Unreal Editor](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/master/Images/haptic1.png)

# Example of using DualSense effects via C++

### New effects
```
   int32 ControllerId = 0; 
   
   UDualSenseProxy::EffectWeapon(int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 Force, EControllerHand Hand);
   
   UDualSenseProxy::EffectGalloping(ControllerId, 5, 8, 5, 7, 0.01f, EControllerHand::Left);
   UDualSenseProxy::EffectGalloping(ControllerId, 0, 5, 0, 4, 0.005f, EControllerHand::Right);
   
   UDualSenseProxy::EffectMachine(ControllerId, 1, 5, 1, 5, 0.5f, 3.f, EControllerHand::Left);
   UDualSenseProxy::EffectMachine(ControllerId, 5, 8, 5, 8, 0.4f, 1.0f, EControllerHand::Right);
   
   UDualSenseProxy::EffectBow(ControllerId, 0, 5, 5, 8, EControllerHand::Left);
   UDualSenseProxy::EffectBow(ControllerId, 5, 8, 8, 8, EControllerHand::Right);
```
### Example haptics effects
```
   int32 ControllerId = 0; 
   
    // Stop triggers effects
    UDualSenseProxy::StopAllTriggersEffects(ControllerId);
    UDualSenseProxy::StopTriggerEffect(ControllerId, EControllerHand::Left);
    UDualSenseProxy::StopTriggerEffect(ControllerId, EControllerHand::Right);

    // Normalize triggers
    UDualSenseProxy::EffectNoResitance(ControllerId, EControllerHand::Left);
    UDualSenseProxy::EffectNoResitance(ControllerId, EControllerHand::Right);

    // Start position max value 8 | Forces max value 8
    UDualSenseProxy::EffectContinuousResitance(ControllerId, 5, 8, EControllerHand::Left); 
    UDualSenseProxy::EffectContinuousResitance(ControllerId, 1, 4, EControllerHand::Right);

    // Start position max value 8 | Forces max value 8
    UDualSenseProxy::EffectSectionResitance(ControllerId, 1, 8, EControllerHand::Left); 
    UDualSenseProxy::EffectContinuousResitance(ControllerId, 5, 8, EControllerHand::Right);

    

    // Start position max value 8 | Forces max value 8
    UDualSenseProxy::SetTriggerHapticFeedbackEffect(ControllerId, 8, 0, 0, 6, EControllerHand::Left, true);
    UDualSenseProxy::SetTriggerHapticFeedbackEffect(ControllerId, 8, 0, 0, 7, EControllerHand::Right, true);

    // SetHapticsByValue is a method of PlayerController.
    SetHapticsByValue(0.1f, 1.0f, EControllerHand::Left);
    SetHapticsByValue(1.0f, 1.0f, EControllerHand::Right);
```

### Players and led effects
```
   #include "DualSenseProxy.h"
   
   void APlayerController::BeginPlay()
   {
       Super::BeginPlay();
       
       int32 ControllerId = 0; 
       
       // Reset buffer all values 
       UDualSenseProxy::ResetEffects(ControllerId);
       
       // Gyroscope and Accelerometer are set to false by default.
       UDualSenseProxy::EnableAccelerometerValues(ControllerId, false);
       UDualSenseProxy::EnableGyroscopeValues(ControllerId, false);
   
       // Touch pad values default false
       UDualSenseProxy::EnableTouch1(ControllerId, false);
       UDualSenseProxy::EnableTouch2(ControllerId, false);
   
       // Level battery Full load max 100.0f
       float levelBattery = UDualSenseProxy::LevelBatteryDevice(ControllerId);
   
       // Leds configs
       UDualSenseProxy::LedMicEffects(ControllerId, ELedMicEnum::MicOn);
       UDualSenseProxy::LedPlayerEffects(ControllerId, ELedPlayerEnum::One, ELedBrightnessEnum::Medium);
       UDualSenseProxy::LedColorEffects(ControllerId, FColor(255, 255, 255));
   }

```
### Vibrations

##### The plugin is compatible with Unreal's native Blueprints Force Feedback
``` 
    // Vibrations example 
    PlayDynamicForceFeedback(0.5f, 3.f, true, true, true, true);
```
## Multiple players with multiple controllers

MyGameModeBase.h
```
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
MyGameModeBase.cpp
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

# Install on FAB official page plugin

To install this plugin directly via FAB (Official Source), follow the steps below:

1. **Access the plugin's official page on FAB**  
   Go to the official plugin page [by clicking here](?).

3. **Install the Plugin**
   - On the plugin page, click the **Install** or **Add to Project** button (depending on the FAB interface).
   - Choose your Unreal Engine project where the plugin will be used, or simply download it for manual setup.

4. **Configure in Unreal Engine**
   - Open your project in Unreal Engine.
   - Go to the **Plugins** tab under `Edit > Plugins`.
   - Search for `Windows DualSense Plugin` in the list and enable it if necessary.
   - Restart the project to apply the changes.


# Manual Installation

[See the example video](https://drive.google.com/file/d/1i4afhW-tG-FkWqjeI6sDr8aoo5KcnPZI/view?usp=sharing)

Download plugin UE_Version

[UE 5.2 download plugin WindowsDualsense1.1.zip](https://drive.google.com/file/d/1dyDkXjNqnt_gkvGkfMSEGJ9oN0prqQn-/view?usp=drive_link)

[UE 5.3 download plugin WindowsDualsense1.1.zip](https://drive.google.com/file/d/141AI1vbBWFOu-UsogfBX7eG4OhurEGJ3/view?usp=drive_link)

[UE 5.4 download plugin WindowsDualsense1.1.zip](https://drive.google.com/file/d/1seGUIslOO3Zm1ZuvA6g9YtbFg_iYk_ub/view?usp=drive_link)

[UE 5.5 download plugin WindowsDualsense1.1.zip](https://drive.google.com/file/d/1NJ1snCv0D3Ng_4arYvHg0LpgyJbYErgI/view?usp=drive_link)

[UE 5.6 download plugin WindowsDualsense1.1.zip](https://drive.google.com/file/d/17uUbXM9R8YARK8vEkdSZ2LFRoa8KZGwu/view?usp=drive_link)

## Installing the Plugin in the Project's Directory

### Steps:

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

## Contributions
Thanks to,


[Nondebug Dualsense](https://github.com/nondebug/dualsense/blob/main/report-descriptor-bluetooth.txt) instructions, which facilitated the development of this plugin.

[DualSense on Windows API](https://github.com/Ohjurot/DualSense-Windows) library, which facilitated the development of this plugin.

[Nielk1 on GIST](https://gist.github.com/Nielk1/6d54cc2c00d2201ccb8c2720ad7538db) code, which facilitated the development of this plugin.

[DualSenseAPI](https://github.com/BadMagic100/DualSenseAPI/tree/master) library, which facilitated the development of this plugin.

[flok pydualsense](https://github.com/flok/pydualsense) library, which facilitated the development of this plugin.

***

> **UPDATED NOTE (2025-06-04)**
> 
>This update replaces the previous implementation, which relied on a third-party DLL located in the `ThirdParty` directory, with a direct integration using the native Windows HIDAPI.
>
>If you have cloned the project and still use the older version with the third-party DLL, please update to this latest version to benefit from the native HIDAPI integration.
>
> This eliminates the dependency on external DLLs and provides a more streamlined setup.
For convenience, you can also download the latest version as a ZIP file from the repository or documentation links provided.
