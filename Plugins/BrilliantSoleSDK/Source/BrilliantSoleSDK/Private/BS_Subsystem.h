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

	UClass *GetClass(const FString &ClassPath);

	// HELPERS START
private:
	UObject *CreateSingleton(UClass *SingletonClass, bool bInitialize);
	void CallFunction(UObject *Singleton, FString FunctionName);
	// HELPERS END

	// BS DEVICE BP START
public:
	UObject *CreateDevice() { return CreateSingleton(DeviceBP_Class, false); }

private:
	void GetDeviceBP_Class();

	UPROPERTY()
	TSubclassOf<UObject> DeviceBP_Class;
	// BS DEVICE BP END

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

	// UDP CLIENT START
public:
	UFUNCTION(BlueprintCallable, Category = "BS Subsystem")
	UObject *GetUDP_Client();

private:
	void GetUDP_ClientClass();

	UPROPERTY()
	TSubclassOf<UObject> UDP_ClientClass;

	UPROPERTY()
	UObject *UDP_ClientSingleton;
	// UDP CLIENT END
};
