// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BS_ConnectionStatus.h"
#include "BS_TxMessage.h"
#include "BS_DeviceInformationManager.h"
#include "BS_BatteryManager.h"
#include "BS_InformationManager.h"
#include "BS_SensorDataManager.h"
#include "BS_VibrationManager.h"
#include "BS_FileTransferManager.h"
#include "BS_TfliteManager.h"
#include "BS_Device.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_Device, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FConnectionStatusUpdateCallback, EBS_ConnectionStatus, ConnectionStatus);

UCLASS()
class UBS_Device : public UObject
{
	GENERATED_BODY()

public:
	UBS_Device();

	UFUNCTION(BlueprintPure, Category = "BS Device")
	EBS_ConnectionStatus GetConnectionStatus() const { return ConnectionStatus; }

	UPROPERTY(BlueprintAssignable, Category = "BS Device")
	FConnectionStatusUpdateCallback OnConnectionStatusUpdate;

protected:
	// MANAGERS

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

	// CONNECTION

	UFUNCTION(BlueprintCallable, Category = "BS ConnectionManager")
	void OnConnectionManagerConnectionStatusUpdate(EBS_ConnectionStatus NewConnectionManagerConnectionStatus);

	// MESSAGING

	UFUNCTION(BlueprintCallable, Category = "BS ConnectionManager")
	void OnRxMessage(const uint8 MessageType, const TArray<uint8> &Message);

	UFUNCTION(BlueprintImplementableEvent, Category = "BS ConnectionManager")
	void SendTxData(const TArray<uint8> &Data);

	UFUNCTION(BlueprintCallable, Category = "BS ConnectionManager")
	void OnSendTxData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BS ConnectionManager")
	bool bIsSendingTxData = false;

private:
	EBS_ConnectionStatus ConnectionStatus = EBS_ConnectionStatus::NOT_CONNECTED;
	void SetConnectionStatus(EBS_ConnectionStatus NewConnectionStatus);

	void SendTxMessages(const TArray<FBS_TxMessage> &TxMessages, bool bSendImmediately = true);
	void SendPendingTxMessages();

	TArray<FBS_TxMessage> PendingTxMessages;
	TArray<uint8> TxData;

	void Reset();
};
