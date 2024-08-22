// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BS_ConnectionStatus.h"
#include "BS_TxMessage.h"
#include "BS_DeviceInformationManager.h"
#include "BS_BatteryManager.h"
#include "BS_InformationManager.h"
#include "BS_SensorDataManager.h"
#include "BS_SensorConfigurationManager.h"
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

private:
	void Reset();

	// CONNECTION START
public:
	UFUNCTION(BlueprintPure, Category = "BS Device")
	EBS_ConnectionStatus GetConnectionStatus() const { return ConnectionStatus; }

	UPROPERTY(BlueprintAssignable, Category = "BS Device")
	FConnectionStatusUpdateCallback OnConnectionStatusUpdate;

protected:
	UFUNCTION(BlueprintCallable, Category = "BS ConnectionManager")
	void OnConnectionManagerConnectionStatusUpdate(EBS_ConnectionStatus NewConnectionManagerConnectionStatus);

private:
	EBS_ConnectionStatus ConnectionStatus = EBS_ConnectionStatus::NOT_CONNECTED;
	void SetConnectionStatus(EBS_ConnectionStatus NewConnectionStatus);
	// CONNECTION END

	// MESSAGING START
protected:
	UFUNCTION(BlueprintCallable, Category = "BS ConnectionManager")
	void OnRxMessage(const uint8 MessageType, const TArray<uint8> &Message);

	UFUNCTION(BlueprintImplementableEvent, Category = "BS ConnectionManager")
	void SendTxData(const TArray<uint8> &Data);

	UFUNCTION(BlueprintCallable, Category = "BS ConnectionManager")
	void OnSendTxData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BS ConnectionManager")
	bool bIsSendingTxData = false;

private:
	void SendTxMessages(const TArray<FBS_TxMessage> &TxMessages, bool bSendImmediately = true);
	void SendPendingTxMessages();

	TArray<FBS_TxMessage> PendingTxMessages;
	TArray<uint8> TxData;

	static TArray<FBS_TxMessage> RequiredTxMessages;
	// MESSAGING END

	// DEVICE INFORMATION START
public:
protected:
	UPROPERTY(BlueprintReadOnly, Category = "BS Device Information")
	UBS_DeviceInformationManager *DeviceInformationManager;

private:
	// DEVICE INFORMATION END

	// BATTERY START
public:
	UFUNCTION(BlueprintPure, Category = "BS Battery")
	uint8 IsBatteryCharging() const { return BatteryManager->GetIsBatteryCharging(); }

	UFUNCTION(BlueprintPure, Category = "BS Battery")
	float BatteryCurrent() const { return BatteryManager->GetBatteryCurrent(); }

protected:
	UPROPERTY(BlueprintReadOnly, Category = "BS Battery")
	UBS_BatteryManager *BatteryManager;

	UPROPERTY(BlueprintAssignable, Category = "BS Battery")
	FIsBatteryChargingCallback OnIsBatteryCharging;

	UPROPERTY(BlueprintAssignable, Category = "BS Battery")
	FBatteryCurrentCallback OnBatteryCurrent;

private:
	void OnBatteryCurrentUpdate(float BatteryCurrent) { OnBatteryCurrent.Broadcast(BatteryCurrent); }
	void OnIsBatteryChargingUpdate(bool bIsBatteryCharging) { OnIsBatteryCharging.Broadcast(bIsBatteryCharging); }
	// BATTERY END

	// INFORMATION START
public:
	UFUNCTION(BlueprintPure, Category = "BS Information")
	int32 MTU() const { return InformationManager->GetMTU(); }

	UFUNCTION(BlueprintPure, Category = "BS Information")
	FString Id() const { return InformationManager->GetId(); }

	UFUNCTION(BlueprintPure, Category = "BS Information")
	FString Name() const { return InformationManager->GetName(); }

	UFUNCTION(BlueprintPure, Category = "BS Information")
	EBS_DeviceType Type() const { return InformationManager->GetType(); }

	UFUNCTION(BlueprintPure, Category = "BS Information")
	float CurrentTime() const { return InformationManager->GetCurrentTime(); }

protected:
	UPROPERTY(BlueprintReadOnly, Category = "BS Information")
	UBS_InformationManager *InformationManager;

	UPROPERTY(BlueprintAssignable, Category = "BS Information")
	FMTU_Callback OnMTU;

	UPROPERTY(BlueprintAssignable, Category = "BS Information")
	FIdCallback OnId;

	UPROPERTY(BlueprintAssignable, Category = "BS Information")
	FNameCallback OnName;

	UPROPERTY(BlueprintAssignable, Category = "BS Information")
	FTypeCallback OnType;

	UPROPERTY(BlueprintAssignable, Category = "BS Information")
	FCurrentTimeCallback OnCurrentTime;

private:
	void OnMTU_Update(uint16 MTU) { OnMTU.Broadcast(MTU); }
	void OnIdUpdate(FString &Id) { OnId.Broadcast(Id); }
	void OnNameUpdate(FString &Name) { OnName.Broadcast(Name); }
	void OnTypeUpdate(EBS_DeviceType Type) { OnType.Broadcast(Type); }
	void OnCurrentTimeUpdate(uint64 CurrentTime) { OnCurrentTime.Broadcast(CurrentTime); }
	// INFORMATION END

	// SENSOR CONFIGURATION START
public:
protected:
	UPROPERTY(BlueprintReadOnly, Category = "BS Sensor Configuration")
	UBS_SensorConfigurationManager *SensorConfigurationManager;

private:
	// SENSOR CONFIGURATION END

	// SENSOR DATA START
public:
protected:
	UPROPERTY(BlueprintReadOnly, Category = "BS Sensor Data")
	UBS_SensorDataManager *SensorDataManager;

private:
	// SENSOR DATA END

	// VIBRATION START
public:
protected:
	UPROPERTY(BlueprintReadOnly, Category = "BS Vibration")
	UBS_VibrationManager *VibrationManager;

private:
	// VIBRATION END

	// FILE TRANSFER START
public:
protected:
	UPROPERTY(BlueprintReadOnly, Category = "BS File Transfer")
	UBS_FileTransferManager *FileTransferManager;

private:
	// FILE TRANSFER END

	// TFLITE START
public:
protected:
	UPROPERTY(BlueprintReadOnly, Category = "BS Tflite")
	UBS_TfliteManager *TfliteManager;

private:
	// TFLITE END
};
