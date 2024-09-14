// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Logging/LogMacros.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "BS_Subsystem.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogBS_Subsystem, Verbose, All);

UCLASS()
class BRILLIANTSOLESDK_API UBS_Subsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UBS_Subsystem();

	virtual void Initialize(FSubsystemCollectionBase &Collection) override;
	virtual void Deinitialize() override;

	// HELPERS START
private:
	UClass *GetClass(const FString &ClassPath);
	UObject *CreateSingleton(UClass *SingletonClass, bool bInitialize);
	void CallFunction(UObject *Singleton, FString FunctionName);
	// HELPERS END

	// BLE MANAGER START
public:
	UFUNCTION(BlueprintCallable, Category = "BS Subsystem")
	UObject *GetBleManager();

private:
	void GetBleManagerClass();

	UPROPERTY()
	TSubclassOf<UObject> BleManagerClass;

	UPROPERTY()
	UObject *BleManagerSingleton;
	// BLE MANAGER END

	// DEVICE MANAGER START
public:
	UFUNCTION(BlueprintCallable, Category = "BS Subsystem")
	UObject *GetDeviceManager();

private:
	void GetDeviceManagerClass();

	UPROPERTY()
	TSubclassOf<UObject> DeviceManagerClass;

	UPROPERTY()
	UObject *DeviceManagerSingleton;
	// DEVICE MANAGER END
};
