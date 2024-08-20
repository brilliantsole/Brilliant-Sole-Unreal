// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BS_DeviceInformationManager.h"
#include "BS_BatteryManager.h"
#include "BS_InformationManager.h"
#include "BS_SensorDataManager.h"
#include "BS_VibrationManager.h"
#include "BS_FileTransferManager.h"
#include "BS_TfliteManager.h"
#include "BS_Device.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_Device, Log, All);

UCLASS()
class UBS_Device : public UObject
{
	GENERATED_BODY()

public:
	UBS_Device();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BS Device Information")
	UBS_DeviceInformationManager *DeviceInformationManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BS Battery")
	UBS_BatteryManager *BatteryManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BS Information")
	UBS_InformationManager *InformationManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BS Sensor Data")
	UBS_SensorDataManager *SensorDataManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BS Vibration")
	UBS_VibrationManager *VibrationManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BS File Transfer")
	UBS_FileTransferManager *FileTransferManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BS Tflite")
	UBS_TfliteManager *TfliteManager;

	UFUNCTION(BlueprintCallable, Category = "BS ConnectionManager")
	void OnRxMessage(const uint8 MessageType, const TArray<uint8> &Message);

	UFUNCTION(BlueprintImplementableEvent, Category = "BS ConnectionManager")
	void SendTxMessage(const TArray<uint8> &Message, bool SendImmediately);
	void SendTxMessage(const TArray<uint8> &Message);
};
