// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BS_ConnectionStatus.h"
#include "BS_Subsystem.h"
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

UCLASS(Abstract)
class UBS_Device : public UObject
{
	GENERATED_BODY()

public:
	UBS_Device();

private:
	void Reset();
	void LogMemoryUsage();

	// BS SUBSYSTEM START
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "BS Subsystem")
	void InitializeBP();

	UFUNCTION(BlueprintPure, Category = "BS Subsystem")
	const UBS_Subsystem *BS_Subsystem() const { return _BS_Subsystem; }

private:
	void GetBS_Subsystem();
	UBS_Subsystem *_BS_Subsystem;
	// BS SUBSYSTEM END

	// CONNECTION START
public:
	UFUNCTION(BlueprintPure, Category = "BS Device")
	EBS_ConnectionStatus GetConnectionStatus() const { return ConnectionStatus; }

	UFUNCTION(BlueprintPure, Category = "BS Device")
	bool IsConnected() const { return ConnectionStatus == EBS_ConnectionStatus::CONNECTED; }

	UPROPERTY(BlueprintAssignable, Category = "BS Device")
	FConnectionStatusUpdateCallback OnConnectionStatusUpdate;

protected:
	UFUNCTION(BlueprintCallable, Category = "BS ConnectionManager")
	void OnConnectionManagerConnectionStatusUpdate(EBS_ConnectionStatus NewConnectionManagerConnectionStatus);

private:
	EBS_ConnectionStatus ConnectionStatus = EBS_ConnectionStatus::NOT_CONNECTED;
	void SetConnectionStatus(EBS_ConnectionStatus NewConnectionStatus);

	void CheckIfFullyConnected();
	TSet<uint8> ReceivedTxMessages;
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

	static const TArray<uint8> RequiredTxMessageTypes;
	static const TArray<FBS_TxMessage> RequiredTxMessages;
	static const TArray<FBS_TxMessage> InitializeRequiredTxMessages();
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

public:
	UPROPERTY(BlueprintAssignable, Category = "BS Battery")
	FIsBatteryChargingCallback OnIsBatteryCharging;

	UPROPERTY(BlueprintAssignable, Category = "BS Battery")
	FBatteryCurrentCallback OnBatteryCurrent;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "BS Battery")
	UBS_BatteryManager *BatteryManager;

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

	UFUNCTION(BlueprintCallable, Category = "BS Information")
	void SetName(const FString &NewName) { InformationManager->SetName(NewName); }

	UFUNCTION(BlueprintPure, Category = "BS Information")
	EBS_DeviceType Type() const { return InformationManager->GetType(); }

	UFUNCTION(BlueprintCallable, Category = "BS Information")
	void SetType(const EBS_DeviceType NewType) { InformationManager->SetType(NewType); }

	UFUNCTION(BlueprintPure, Category = "BS Information")
	float CurrentTime() const { return InformationManager->GetCurrentTime(); }

public:
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

protected:
	UPROPERTY(BlueprintReadOnly, Category = "BS Information")
	UBS_InformationManager *InformationManager;

private:
	void OnMTU_Update(uint16 MTU) { OnMTU.Broadcast(MTU); }
	void OnIdUpdate(FString &Id) { OnId.Broadcast(Id); }
	void OnNameUpdate(FString &Name) { OnName.Broadcast(Name); }
	void OnTypeUpdate(EBS_DeviceType Type) { OnType.Broadcast(Type); }
	void OnCurrentTimeUpdate(uint64 CurrentTime) { OnCurrentTime.Broadcast(CurrentTime); }
	// INFORMATION END

	// SENSOR CONFIGURATION START
public:
	UFUNCTION(BlueprintPure, Category = "BS Sensor Configuration")
	const UBS_SensorConfiguration *SensorConfiguration() const { return SensorConfigurationManager->GetSensorConfiguration(); }

	UFUNCTION(BlueprintCallable, Category = "BS Sensor Configuration")
	void SetSensorConfiguration(const UBS_SensorConfiguration *NewSensorConfiguration) { SensorConfigurationManager->SetSensorConfiguration(NewSensorConfiguration); }

	UFUNCTION(BlueprintCallable, Category = "BS Sensor Configuration")
	void ClearSensorConfiguration() { SensorConfigurationManager->ClearSensorConfiguration(); }

	UFUNCTION(BlueprintCallable, Category = "BS Sensor Configuration")
	EBS_SensorRate GetSensorRate(EBS_SensorType SensorType, bool &bContainsSensorType) const { return SensorConfigurationManager->GetSensorRate(SensorType, bContainsSensorType); }

	UFUNCTION(BlueprintCallable, Category = "BS Sensor Configuration")
	bool IsSensorRateNonZero(EBS_SensorType SensorType) const { return SensorConfigurationManager->IsSensorRateNonZero(SensorType); }

	UFUNCTION(BlueprintCallable, Category = "BS Sensor Configuration")
	const TMap<EBS_SensorType, EBS_SensorRate> &GetSensorRates() const
	{
		return SensorConfigurationManager->GetSensorRates();
	}

	UFUNCTION(BlueprintCallable, Category = "BS Sensor Configuration")
	void SetSensorRate(EBS_SensorType SensorType, EBS_SensorRate SensorRate, bool &bDidUpdateSensorRate) { return SensorConfigurationManager->SetSensorRate(SensorType, SensorRate, bDidUpdateSensorRate); }

	UFUNCTION(BlueprintCallable, Category = "BS Sensor Configuration")
	void SetSensorRates(const TMap<EBS_SensorType, EBS_SensorRate> &SensorRates) { return SensorConfigurationManager->SetSensorRates(SensorRates); }

	UFUNCTION(BlueprintCallable, Category = "BS Sensor Configuration")
	void ClearSensorRate(EBS_SensorType SensorType) { return SensorConfigurationManager->ClearSensorRate(SensorType); }

	UFUNCTION(BlueprintCallable, Category = "BS Sensor Configuration")
	void ToggleSensorRate(EBS_SensorType SensorType, EBS_SensorRate SensorRate, EBS_SensorRate &UpdatedSensorRate) { return SensorConfigurationManager->ToggleSensorRate(SensorType, SensorRate, UpdatedSensorRate); }

protected:
	UPROPERTY(BlueprintReadOnly, Category = "BS Sensor Configuration")
	UBS_SensorConfigurationManager *SensorConfigurationManager;

	UPROPERTY(BlueprintAssignable, Category = "BS Sensor Configuration")
	FSensorConfigurationCallback OnSensorConfiguration;

private:
	void OnSensorConfigurationUpdate(const UBS_SensorConfiguration *SensorConfiguration) { OnSensorConfiguration.Broadcast(SensorConfiguration); }
	// SENSOR CONFIGURATION END

	// SENSOR DATA START
protected:
	UPROPERTY(BlueprintReadOnly, Category = "BS Sensor Data")
	UBS_SensorDataManager *SensorDataManager;
	// SENSOR DATA END

	// MOTION DATA START
public:
	UPROPERTY(BlueprintAssignable, Category = "BS Motion Data")
	FVectorCallback OnAcceleration;
	UPROPERTY(BlueprintAssignable, Category = "BS Motion Data")
	FVectorCallback OnGravity;
	UPROPERTY(BlueprintAssignable, Category = "BS Motion Data")
	FVectorCallback OnLinearAcceleration;
	UPROPERTY(BlueprintAssignable, Category = "BS Motion Data")
	FVectorCallback OnGyroscope;
	UPROPERTY(BlueprintAssignable, Category = "BS Motion Data")
	FVectorCallback OnMagnetometer;
	UPROPERTY(BlueprintAssignable, Category = "BS Motion Data")
	FQuaternionCallback OnGameRotation;
	UPROPERTY(BlueprintAssignable, Category = "BS Motion Data")
	FQuaternionCallback OnRotation;

	UPROPERTY(BlueprintAssignable, Category = "BS Motion Data")
	FRotatorCallback OnOrientation;
	UPROPERTY(BlueprintAssignable, Category = "BS Motion Data")
	FActivityCallback OnActivity;
	UPROPERTY(BlueprintAssignable, Category = "BS Motion Data")
	FStepCountCallback OnStepCount;
	UPROPERTY(BlueprintAssignable, Category = "BS Motion Data")
	FTimestampCallback OnStepDetection;
	UPROPERTY(BlueprintAssignable, Category = "BS Motion Data")
	FDeviceOrientationCallback OnDeviceOrientation;

private:
	void OnAccelerationUpdate(const FVector &Vector, const uint64 &Timestamp) { OnAcceleration.Broadcast(Vector, Timestamp); }
	void OnGravityUpdate(const FVector &Vector, const uint64 &Timestamp) { OnGravity.Broadcast(Vector, Timestamp); }
	void OnLinearAccelerationUpdate(const FVector &Vector, const uint64 &Timestamp) { OnLinearAcceleration.Broadcast(Vector, Timestamp); }
	void OnGyroscopeUpdate(const FVector &Vector, const uint64 &Timestamp) { OnGyroscope.Broadcast(Vector, Timestamp); }
	void OnMagnetometerUpdate(const FVector &Vector, const uint64 &Timestamp) { OnMagnetometer.Broadcast(Vector, Timestamp); }
	void OnGameRotationUpdate(const FQuat &Quaternion, const uint64 &Timestamp) { OnGameRotation.Broadcast(Quaternion, Timestamp); }
	void OnRotationUpdate(const FQuat &Quaternion, const uint64 &Timestamp)
	{
		UE_LOG(LogBS_Device, Log, TEXT("OnRotationUpdate %u"), OnRotation.IsBound());
		OnRotation.Broadcast(Quaternion, Timestamp);
	}

	void OnOrientationUpdate(const FRotator &Rotator, const uint64 &Timestamp) { OnOrientation.Broadcast(Rotator, Timestamp); }
	void OnActivityUpdate(const TSet<EBS_Activity> &Activity, const uint64 &Timestamp) { OnActivity.Broadcast(Activity, Timestamp); }
	void OnStepCountUpdate(const uint32 &StepCount, const uint64 &Timestamp) { OnStepCount.Broadcast(StepCount, Timestamp); }
	void OnStepDetectionUpdate(const uint64 &Timestamp) { OnStepDetection.Broadcast(Timestamp); }
	void OnDeviceOrientationUpdate(const EBS_DeviceOrientation &DeviceOrientation, const uint64 &Timestamp) { OnDeviceOrientation.Broadcast(DeviceOrientation, Timestamp); }
	// MOTION DATA END

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
