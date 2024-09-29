// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Logging/StructuredLog.h"
#include "UObject/NoExportTypes.h"
#include "BS_ConnectionStatus.h"
#include "BS_DeviceInformationType.h"
#include "BS_TxMessage.h"
#include "BS_TxRxMessageType.h"
#include "BS_BaseConnectionManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_BaseConnectionManager, Log, All);

UCLASS(Abstract, BlueprintType, Blueprintable)
class UBS_BaseConnectionManager : public UObject
{
	GENERATED_BODY()

public:
	UBS_BaseConnectionManager();

	// CONNECTION START
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "BS Connection Manager")
	void Connect(bool &bContinue);
	virtual void Connect_Implementation(bool &bContinue);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "BS Connection Manager")
	void Disconnect(bool &bContinue);
	virtual void Disconnect_Implementation(bool &bContinue);

	UFUNCTION(BlueprintCallable, Category = "BS Connection Manager")
	void ToggleConnection();

	void Connect()
	{
		bool bContinue = false;
		Connect(bContinue);
	}
	void Disconnect()
	{
		bool bContinue = false;
		Disconnect(bContinue);
	}
	// CONNECTION END

	// CONNECTION STATUS START
public:
	UFUNCTION(BlueprintPure, Category = "BS Connection Manager")
	const EBS_ConnectionStatus GetConnectionStatus() const { return ConnectionStatus; }

	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "BS Connection Manager")
	const bool GetIsConnected() const;
	virtual const bool GetIsConnected_Implementation() const { return ConnectionStatus == EBS_ConnectionStatus::CONNECTED; }

	UFUNCTION(BlueprintCallable, Category = "BS Connection Manager")
	void UpdateConnectionStatus();

protected:
	UFUNCTION(BlueprintCallable, Category = "BS Connection Manager")
	void SetConnectionStatus(EBS_ConnectionStatus NewConnectionStatus);

private:
	EBS_ConnectionStatus ConnectionStatus = EBS_ConnectionStatus::NOT_CONNECTED;
	// CONNECTION STATUS END

	// RX DATA START
protected:
	UFUNCTION(BlueprintCallable, Category = "BS Connection Manager")
	void ParseRxData(const TArray<uint8> &Data);
	// RX DATA END

	// MESSAGING START
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "BS Connection Manager")
	void SendTxData(const TArray<uint8> &Data);
	virtual void SendTxData_Implementation(const TArray<uint8> &Data);

	UFUNCTION(BlueprintCallable, Category = "BS Connection Manager")
	void SendPingTxData();
	// MESSAGING END

	// PING START
public:
	UFUNCTION(BlueprintPure, Category = "BS Connection Manager")
	static const TArray<uint8> &GetPingTxData() { return PingTxData; }

private:
	static const FBS_TxMessage PingTxMessage;
	static const TArray<uint8> InitializePingTxData();
	static const TArray<uint8> PingTxData;
	// PING END

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBS_ConnectionManagerCallback, UBS_BaseConnectionManager *, ConnectionManager);

	// CONNECTION EVENT DISPTACHERS START
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBS_ConnectionManagerStatusCallback, UBS_BaseConnectionManager *, ConnectionManager, EBS_ConnectionStatus, ConnectionStatus);

	UPROPERTY(BlueprintCallable, Category = "BS Connection Manager")
	FBS_ConnectionManagerStatusCallback OnConnectionUpdate;

	UPROPERTY(BlueprintCallable, Category = "BS Connection Manager")
	FBS_ConnectionManagerCallback OnConnecting;

	UPROPERTY(BlueprintCallable, Category = "BS Connection Manager")
	FBS_ConnectionManagerCallback OnConnected;

	UPROPERTY(BlueprintCallable, Category = "BS Connection Manager")
	FBS_ConnectionManagerCallback OnDisconnecting;

	UPROPERTY(BlueprintCallable, Category = "BS Connection Manager")
	FBS_ConnectionManagerCallback OnNotConnected;
	// CONNECTION EVENT DISPTACHERS END

	// MESSAGING EVENT DISPTACHERS START
public:
	DECLARE_MULTICAST_DELEGATE_ThreeParams(FBS_ConnectionManagerRxDataCallback, UBS_BaseConnectionManager *, EBS_TxRxMessage, const TArray<uint8> &);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBS_ConnectionManagerBatteryLevelCallback, UBS_BaseConnectionManager *, ConnectionManager, uint8, BatteryLevel);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FBS_ConnectionManagerDeviceInformationValueCallback, UBS_BaseConnectionManager *, ConnectionManager, EBS_DeviceInformation, Type, const TArray<uint8> &, Value);

	FBS_ConnectionManagerRxDataCallback OnRxMessage;

	UPROPERTY(BlueprintCallable, Category = "BS Connection Manager")
	FBS_ConnectionManagerBatteryLevelCallback OnBatteryLevel;

	UPROPERTY(BlueprintCallable, Category = "BS Connection Manager")
	FBS_ConnectionManagerCallback OnSendTxMessage;

	UPROPERTY(BlueprintCallable, Category = "BS Connection Manager")
	FBS_ConnectionManagerCallback OnRxMessages;

	UPROPERTY(BlueprintCallable, Category = "BS Connection Manager")
	FBS_ConnectionManagerDeviceInformationValueCallback OnDeviceInformationValue;
	// MESSAGING EVENT DISPTACHERS END
};
