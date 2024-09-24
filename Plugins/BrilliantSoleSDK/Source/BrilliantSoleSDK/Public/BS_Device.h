// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Logging/StructuredLog.h"
#include "BS_ConnectionStatus.h"
#include "BS_DeviceInformation.h"
#include "BS_Subsystem.h"
#include "BS_TxMessage.h"
#include "BS_TxRxMessageType.h"
#include "BS_BatteryManager.h"
#include "BS_InformationManager.h"
#include "BS_SensorDataManager.h"
#include "BS_SensorConfigurationManager.h"
#include "BS_VibrationManager.h"
#include "BS_FileTransferManager.h"
#include "BS_TfliteManager.h"
#include "BS_InsoleSide.h"
#include "BS_MathUtils.h"
#include "BS_BaseConnectionManager.h"
#include "BS_Device.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_Device, Verbose, All);

UCLASS(Abstract, BlueprintType, Blueprintable)
class UBS_Device : public UObject
{
	GENERATED_BODY()

public:
	UBS_Device();
	void PostInitProperties();

private:
	void Reset();

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

	// BATTERY LEVEL START
public:
	UFUNCTION(BlueprintPure, Category = "BS Battery Level")
	uint8 GetBatteryLevel() const { return BatteryLevel; }

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBS_BatteryLevelUpdateCallback, UBS_Device *, Device, uint8, BatteryLevel);
	UPROPERTY(BlueprintAssignable, Category = "BS Battery Level")
	FBS_BatteryLevelUpdateCallback OnBatteryLevel;

protected:
	UFUNCTION(BlueprintCallable, Category = "BS Battery Level")
	void OnBatteryLevelUpdate(UBS_BaseConnectionManager *_ConnectionManager, uint8 NewBatteryLevel);

private:
	bool bDidGetBatteryLevel = false;
	uint8 BatteryLevel = 0;
	// BATTERY LEVEL END

	// DEVICE INFORMATION START
public:
	UFUNCTION(BlueprintPure, Category = "BS Device Information")
	FBS_DeviceInformation GetDeviceInformation() const { return DeviceInformation; }

protected:
	UFUNCTION(BlueprintCallable, Category = "BS Device Information")
	void OnDeviceInformationValue(UBS_BaseConnectionManager *_ConnectionManager, const EBS_DeviceInformation DeviceInformationType, const TArray<uint8> &Value) { DeviceInformation.SetValue(DeviceInformationType, Value); };

private:
	FBS_DeviceInformation DeviceInformation;
	// DEVICE INFORMATION END

	// CONNECTION START
public:
	UFUNCTION(BlueprintCallable, Category = "BS Device")
	void Connect() { ConnectionManager->Connect(); }

	UFUNCTION(BlueprintCallable, Category = "BS Device")
	void Disconnect() { ConnectionManager->Disconnect(); }

	UFUNCTION(BlueprintCallable, Category = "BS Device")
	void ToggleConnection() { ConnectionManager->ToggleConnection(); }

	UFUNCTION(BlueprintPure, Category = "BS Device")
	EBS_ConnectionStatus GetConnectionStatus() const { return ConnectionStatus; }

	UFUNCTION(BlueprintPure, Category = "BS Device")
	bool IsConnected() const { return ConnectionStatus == EBS_ConnectionStatus::CONNECTED; }

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBS_ConnectionStatusUpdateCallback, UBS_Device *, Device, EBS_ConnectionStatus, ConnectionStatus);
	UPROPERTY(BlueprintAssignable, Category = "BS Device")
	FBS_ConnectionStatusUpdateCallback OnConnectionStatusUpdate;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBS_IsConnectedUpdateCallback, UBS_Device *, Device, bool, IsConnected);
	UPROPERTY(BlueprintAssignable, Category = "BS Device")
	FBS_IsConnectedUpdateCallback OnIsConnectedUpdate;

protected:
	UFUNCTION(BlueprintCallable, Category = "BS Connection Manager")
	void OnConnectionManagerStatusUpdate(UBS_BaseConnectionManager *_ConnectionManager, EBS_ConnectionStatus NewConnectionManagerConnectionStatus);

private:
	EBS_ConnectionStatus ConnectionStatus = EBS_ConnectionStatus::NOT_CONNECTED;
	void SetConnectionStatus(EBS_ConnectionStatus NewConnectionStatus);

	void CheckIfFullyConnected();
	TSet<EBS_TxRxMessage> ReceivedTxMessages;
	// CONNECTION END

	// CONNECTION MANAGER START
public:
	UFUNCTION(BlueprintPure, Category = "BS Connection Manager")
	UBS_BaseConnectionManager *GetConnectionManager() const { return ConnectionManager; }

	UFUNCTION(BlueprintCallable, Category = "BS Connection Manager")
	void AssignConnectionManager(UBS_BaseConnectionManager *NewConnectionManager);

	UFUNCTION(BlueprintCallable, Category = "BS Connection Manager")
	void RemoveConnectionManager();

private:
	UBS_BaseConnectionManager *ConnectionManager = nullptr;
	// CONNECTION MANAGER END

	// MESSAGING START
public:
protected:
	UFUNCTION(BlueprintCallable, Category = "BS Connection Manager")
	void OnRxMessage(UBS_BaseConnectionManager *_ConnectionManager, uint8 MessageTypeEnum, const TArray<uint8> &Message);

	UFUNCTION(BlueprintCallable, Category = "BS Connection Manager")
	void OnRxMessages(UBS_BaseConnectionManager *_ConnectionManager);

	void SendTxData(const TArray<uint8> &Data) { ConnectionManager->SendTxData(Data); }

	UFUNCTION(BlueprintCallable, Category = "BS Connection Manager")
	void OnSendTxData(UBS_BaseConnectionManager *_ConnectionManager);

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BS Connection Manager")
	bool bIsSendingTxData = false;

private:
	void SendTxMessages(const TArray<FBS_TxMessage> &TxMessages, bool bSendImmediately = true);
	void SendPendingTxMessages();

	void SendRequiredTxMessages() { SendTxMessages(UBS_Device::RequiredTxMessages); }

	TArray<FBS_TxMessage> PendingTxMessages;
	TArray<uint8> TxData;

	static const TArray<EBS_TxRxMessage> RequiredTxMessageTypes;
	static const TArray<FBS_TxMessage> RequiredTxMessages;
	static const TArray<FBS_TxMessage> InitializeRequiredTxMessages();

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

	// BATTERY START
public:
	UFUNCTION(BlueprintPure, Category = "BS Battery")
	uint8 IsBatteryCharging() const { return BatteryManager->GetIsBatteryCharging(); }

	UFUNCTION(BlueprintPure, Category = "BS Battery")
	float BatteryCurrent() const { return BatteryManager->GetBatteryCurrent(); }

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBS_IsBatteryChargingCallback, UBS_Device *, Device, bool, IsBatteryCharging);
	UPROPERTY(BlueprintAssignable, Category = "BS Battery")
	FBS_IsBatteryChargingCallback OnIsBatteryCharging;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBS_BatteryCurrentCallback, UBS_Device *, Device, float, BatteryCurrent);
	UPROPERTY(BlueprintAssignable, Category = "BS Battery")
	FBS_BatteryCurrentCallback OnBatteryCurrent;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "BS Battery")
	UBS_BatteryManager *BatteryManager;

private:
	void OnBatteryCurrentUpdate(float BatteryCurrent) { OnBatteryCurrent.Broadcast(this, BatteryCurrent); }
	void OnIsBatteryChargingUpdate(bool bIsBatteryCharging) { OnIsBatteryCharging.Broadcast(this, bIsBatteryCharging); }
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

	UFUNCTION(BlueprintPure, Category = "BS Information")
	bool IsInsole() const
	{
		switch (Type())
		{
		case EBS_DeviceType::LEFT_INSOLE:
		case EBS_DeviceType::RIGHT_INSOLE:
			return true;
		}
		return false;
	}

	UFUNCTION(BlueprintPure, Category = "BS Information")
	EBS_InsoleSide InsoleSide() const
	{
		switch (Type())
		{
		case EBS_DeviceType::LEFT_INSOLE:
			return EBS_InsoleSide::LEFT;
		default:
			return EBS_InsoleSide::RIGHT;
		}
	}

	UFUNCTION(BlueprintCallable, Category = "BS Information")
	void SetType(const EBS_DeviceType NewType)
	{
		InformationManager->SetType(NewType);
	}

	UFUNCTION(BlueprintPure, Category = "BS Information")
	float CurrentTime() const { return InformationManager->GetCurrentTime(); }

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBS_MTU_Callback, UBS_Device *, Device, uint16, MTU);
	UPROPERTY(BlueprintAssignable, Category = "BS Information")
	FBS_MTU_Callback OnMTU;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBS_IdCallback, UBS_Device *, Device, const FString &, Id);
	UPROPERTY(BlueprintAssignable, Category = "BS Information")
	FBS_IdCallback OnId;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBS_NameCallback, UBS_Device *, Device, const FString &, Name);
	UPROPERTY(BlueprintAssignable, Category = "BS Information")
	FBS_NameCallback OnName;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBS_TypeCallback, UBS_Device *, Device, EBS_DeviceType, Type);
	UPROPERTY(BlueprintAssignable, Category = "BS Information")
	FBS_TypeCallback OnType;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBS_CurrentTimeCallback, UBS_Device *, Device, float, CurrentTime);
	UPROPERTY(BlueprintAssignable, Category = "BS Information")
	FBS_CurrentTimeCallback OnCurrentTime;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "BS Information")
	UBS_InformationManager *InformationManager;

private:
	void OnMTU_Update(uint16 MTU)
	{
		FileTransferManager->MTU = MTU;
		OnMTU.Broadcast(this, MTU);
	}
	void OnIdUpdate(const FString &Id) { OnId.Broadcast(this, Id); }
	void OnNameUpdate(const FString &Name) { OnName.Broadcast(this, Name); }
	void OnTypeUpdate(EBS_DeviceType Type) { OnType.Broadcast(this, Type); }
	void OnCurrentTimeUpdate(uint64 CurrentTime) { OnCurrentTime.Broadcast(this, CurrentTime); }
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
	const EBS_SensorRate GetSensorRate(EBS_SensorType SensorType, bool &bContainsSensorType) const { return SensorConfigurationManager->GetSensorRate(SensorType, bContainsSensorType); }

	UFUNCTION(BlueprintCallable, Category = "BS Sensor Configuration")
	const bool IsSensorRateNonZero(EBS_SensorType SensorType) const { return SensorConfigurationManager->IsSensorRateNonZero(SensorType); }

	UFUNCTION(BlueprintCallable, Category = "BS Sensor Configuration")
	const TMap<EBS_SensorType, EBS_SensorRate> &GetSensorRates() const { return SensorConfigurationManager->GetSensorRates(); }

	UFUNCTION(BlueprintCallable, Category = "BS Sensor Configuration")
	void SetSensorRate(EBS_SensorType SensorType, EBS_SensorRate SensorRate, bool &bDidUpdateSensorRate) { return SensorConfigurationManager->SetSensorRate(SensorType, SensorRate, bDidUpdateSensorRate); }

	UFUNCTION(BlueprintCallable, Category = "BS Sensor Configuration")
	void SetSensorRates(const TMap<EBS_SensorType, EBS_SensorRate> &SensorRates) { return SensorConfigurationManager->SetSensorRates(SensorRates); }

	UFUNCTION(BlueprintCallable, Category = "BS Sensor Configuration")
	void ClearSensorRate(EBS_SensorType SensorType) { return SensorConfigurationManager->ClearSensorRate(SensorType); }

	UFUNCTION(BlueprintCallable, Category = "BS Sensor Configuration")
	void ToggleSensorRate(EBS_SensorType SensorType, EBS_SensorRate SensorRate, EBS_SensorRate &UpdatedSensorRate) { return SensorConfigurationManager->ToggleSensorRate(SensorType, SensorRate, UpdatedSensorRate); }

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBS_SensorConfigurationCallback, UBS_Device *, Device, const UBS_SensorConfiguration *, SensorConfiguration);
	UPROPERTY(BlueprintAssignable, Category = "BS Sensor Configuration")
	FBS_SensorConfigurationCallback OnSensorConfiguration;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "BS Sensor Configuration")
	UBS_SensorConfigurationManager *SensorConfigurationManager;

private:
	void OnSensorConfigurationUpdate(const UBS_SensorConfiguration *SensorConfiguration) { OnSensorConfiguration.Broadcast(this, SensorConfiguration); }
	// SENSOR CONFIGURATION END

	// SENSOR DATA START
protected:
	UPROPERTY(BlueprintReadOnly, Category = "BS Sensor Data")
	UBS_SensorDataManager *SensorDataManager;
	// SENSOR DATA END

	// PRESSURE DATA START
public:
	UFUNCTION(BlueprintPure, Category = "BS Pressure Data")
	const uint8 NumberOfPressureSensors() const { return SensorDataManager->PressureSensorDataManager->GetNumberOfPressureSensors(); }

	UFUNCTION(BlueprintCallable, Category = "BS Pressure Data")
	void ResetPressure() { SensorDataManager->PressureSensorDataManager->Reset(); }

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FBS_PressureCallback, UBS_Device *, Device, const FBS_PressureData &, PressureData, const int64 &, Timestamp);
	UPROPERTY(BlueprintAssignable, Category = "BS Pressure Data")
	FBS_PressureCallback OnPressure;

private:
	void OnPressureUpdate(const FBS_PressureData &PressureData, const uint64 &Timestamp) { OnPressure.Broadcast(this, PressureData, Timestamp); }
	// PRESSURE DATA END

	// MOTION DATA START
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FBS_VectorCallback, UBS_Device *, Device, const FVector &, Vector, const int64 &, Timestamp);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FBS_RotatorCallback, UBS_Device *, Device, const FRotator &, Rotator, const int64 &, Timestamp);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FBS_QuaternionCallback, UBS_Device *, Device, const FQuat &, Quaternion, const int64 &, Timestamp);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBS_TimestampCallback, UBS_Device *, Device, const int64 &, Timestamp);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FBS_ActivityCallback, UBS_Device *, Device, const TSet<EBS_Activity> &, Activity, const int64 &, Timestamp);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FBS_StepCountCallback, UBS_Device *, Device, const uint32 &, StepCount, const int64 &, Timestamp);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FBS_DeviceOrientationCallback, UBS_Device *, Device, const EBS_DeviceOrientation &, DeviceOrientation, const int64 &, Timestamp);

	UPROPERTY(BlueprintAssignable, Category = "BS Motion Data")
	FBS_VectorCallback OnAcceleration;
	UPROPERTY(BlueprintAssignable, Category = "BS Motion Data")
	FBS_VectorCallback OnGravity;
	UPROPERTY(BlueprintAssignable, Category = "BS Motion Data")
	FBS_VectorCallback OnLinearAcceleration;
	UPROPERTY(BlueprintAssignable, Category = "BS Motion Data")
	FBS_VectorCallback OnGyroscope;
	UPROPERTY(BlueprintAssignable, Category = "BS Motion Data")
	FBS_VectorCallback OnMagnetometer;
	UPROPERTY(BlueprintAssignable, Category = "BS Motion Data")
	FBS_QuaternionCallback OnGameRotation;
	UPROPERTY(BlueprintAssignable, Category = "BS Motion Data")
	FBS_QuaternionCallback OnRotation;

	UPROPERTY(BlueprintAssignable, Category = "BS Motion Data")
	FBS_RotatorCallback OnOrientation;
	UPROPERTY(BlueprintAssignable, Category = "BS Motion Data")
	FBS_ActivityCallback OnActivity;
	UPROPERTY(BlueprintAssignable, Category = "BS Motion Data")
	FBS_StepCountCallback OnStepCount;
	UPROPERTY(BlueprintAssignable, Category = "BS Motion Data")
	FBS_TimestampCallback OnStepDetection;
	UPROPERTY(BlueprintAssignable, Category = "BS Motion Data")
	FBS_DeviceOrientationCallback OnDeviceOrientation;

private:
	void OnAccelerationUpdate(const FVector &Vector, const uint64 &Timestamp) { OnAcceleration.Broadcast(this, Vector, Timestamp); }
	void OnGravityUpdate(const FVector &Vector, const uint64 &Timestamp) { OnGravity.Broadcast(this, Vector, Timestamp); }
	void OnLinearAccelerationUpdate(const FVector &Vector, const uint64 &Timestamp) { OnLinearAcceleration.Broadcast(this, Vector, Timestamp); }
	void OnGyroscopeUpdate(const FVector &Vector, const uint64 &Timestamp) { OnGyroscope.Broadcast(this, Vector, Timestamp); }
	void OnMagnetometerUpdate(const FVector &Vector, const uint64 &Timestamp) { OnMagnetometer.Broadcast(this, Vector, Timestamp); }
	void OnGameRotationUpdate(const FQuat &Quaternion, const uint64 &Timestamp) { OnGameRotation.Broadcast(this, Quaternion, Timestamp); }
	void OnRotationUpdate(const FQuat &Quaternion, const uint64 &Timestamp) { OnRotation.Broadcast(this, Quaternion, Timestamp); }

	void OnOrientationUpdate(const FRotator &Rotator, const uint64 &Timestamp) { OnOrientation.Broadcast(this, Rotator, Timestamp); }
	void OnActivityUpdate(const TSet<EBS_Activity> &Activity, const uint64 &Timestamp) { OnActivity.Broadcast(this, Activity, Timestamp); }
	void OnStepCountUpdate(const uint32 &StepCount, const uint64 &Timestamp) { OnStepCount.Broadcast(this, StepCount, Timestamp); }
	void OnStepDetectionUpdate(const uint64 &Timestamp) { OnStepDetection.Broadcast(this, Timestamp); }
	void OnDeviceOrientationUpdate(const EBS_DeviceOrientation &DeviceOrientation, const uint64 &Timestamp) { OnDeviceOrientation.Broadcast(this, DeviceOrientation, Timestamp); }
	// MOTION DATA END

	// BAROMETER DATA START
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FBS_BarometerCallback, UBS_Device *, Device, const float &, Barometer, const int64 &, Timestamp);

	UPROPERTY(BlueprintAssignable, Category = "BS Barometer Data")
	FBS_BarometerCallback OnBarometer;

private:
	void OnBarometerUpdate(const float &Barometer, const uint64 &Timestamp) { OnBarometer.Broadcast(this, Barometer, Timestamp); }

	// BAROMETER DATA END

	// VIBRATION START
public:
	UFUNCTION(BlueprintCallable, Category = "BS Vibration")
	void TriggerVibration(const TArray<FBS_VibrationConfiguration> &VibrationConfigurations) { VibrationManager->TriggerVibration(VibrationConfigurations); }

protected:
	UPROPERTY(BlueprintReadOnly, Category = "BS Vibration")
	UBS_VibrationManager *VibrationManager;

private:
	// VIBRATION END

	// FILE TRANSFER START
public:
	UFUNCTION(BlueprintPure, Category = "BS File Transfer")
	const int32 GetMaxFileLength() const { return FileTransferManager->GetMaxFileLength(); }

	UFUNCTION(BlueprintPure, Category = "BS File Transfer")
	const EBS_FileTransferStatus GetFileTransferStatus() const { return FileTransferManager->GetFileTransferStatus(); }

	UFUNCTION(BlueprintCallable, Category = "BS File Transfer")
	void CancelFileTransfer() { FileTransferManager->CancelFileTransfer(); }

	UFUNCTION(BlueprintCallable, Category = "BS File Transfer")
	void SendFile(const EBS_FileType FileType, const TArray<uint8> &File) { FileTransferManager->SendFile(FileType, File); }

	UFUNCTION(BlueprintCallable, Category = "BS File Transfer")
	void ReceiveFile(const EBS_FileType FileType) { FileTransferManager->ReceiveFile(FileType); }

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBS_FileTransferStatusCallback, UBS_Device *, Device, EBS_FileTransferStatus, FileTransferStatus);
	UPROPERTY(BlueprintAssignable, Category = "BS File Transfer")
	FBS_FileTransferStatusCallback OnFileTransferStatus;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBS_FileTransferMaxFileLengthCallback, UBS_Device *, Device, int32, MaxFileLength);
	UPROPERTY(BlueprintAssignable, Category = "BS File Transfer")
	FBS_FileTransferMaxFileLengthCallback OnMaxFileLength;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FBS_FileTransferFileReceivedCallback, UBS_Device *, Device, EBS_FileType, FileType, const TArray<uint8> &, File);
	UPROPERTY(BlueprintAssignable, Category = "BS File Transfer")
	FBS_FileTransferFileReceivedCallback OnFileReceived;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FBS_FileTransferProgressCallback, UBS_Device *, Device, EBS_FileType, FileType, EBS_FileTransferDirection, FileTransferDirection, float, Progress);
	UPROPERTY(BlueprintAssignable, Category = "BS File Transfer")
	FBS_FileTransferProgressCallback OnFileTransferProgress;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FBS_FileTransferCompleteCallback, UBS_Device *, Device, EBS_FileType, FileType, EBS_FileTransferDirection, FileTransferDirection);
	UPROPERTY(BlueprintAssignable, Category = "BS File Transfer")
	FBS_FileTransferCompleteCallback OnFileTransferComplete;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "BS File Transfer")
	UBS_FileTransferManager *FileTransferManager;

private:
	void OnFileTransferStatusUpdate(const EBS_FileTransferStatus FileTransferStatus) { OnFileTransferStatus.Broadcast(this, FileTransferStatus); }
	void OnMaxFileLengthUpdate(const uint32 MaxFileLength) { OnMaxFileLength.Broadcast(this, MaxFileLength); }
	void OnFileReceivedUpdate(const EBS_FileType FileType, const TArray<uint8> &File) { OnFileReceived.Broadcast(this, FileType, File); }
	void OnFileTransferProgressUpdate(const EBS_FileType FileType, EBS_FileTransferDirection FileTransferDirection, float Progress) { OnFileTransferProgress.Broadcast(this, FileType, FileTransferDirection, Progress); }
	void OnFileTransferCompleteUpdate(const EBS_FileType FileType, EBS_FileTransferDirection FileTransferDirection) { OnFileTransferComplete.Broadcast(this, FileType, FileTransferDirection); }

	// FILE TRANSFER END

	// TFLITE START
public:
	UFUNCTION(BlueprintPure, Category = "BS Tflite")
	const FString &GetTfliteName() const { return TfliteManager->GetName(); }

	UFUNCTION(BlueprintCallable, Category = "BS Tflite")
	void SetTfliteName(const FString &NewName, bool bSendImmediately = true) { return TfliteManager->SetName(NewName, bSendImmediately); }

	UFUNCTION(BlueprintPure, Category = "BS Tflite")
	const EBS_TfliteTask GetTfliteTask() const { return TfliteManager->GetTask(); }

	UFUNCTION(BlueprintCallable, Category = "BS Tflite")
	void SetTfliteTask(const EBS_TfliteTask NewTask, bool bSendImmediately = true) { return TfliteManager->SetTask(NewTask, bSendImmediately); }

	UFUNCTION(BlueprintPure, Category = "BS Tflite")
	const EBS_SensorRate GetTfliteSampleRate() const { return TfliteManager->GetSampleRate(); }

	UFUNCTION(BlueprintCallable, Category = "BS Tflite")
	void SetTfliteSampleRate(const EBS_SensorRate NewSampleRate, bool bSendImmediately = true) { return TfliteManager->SetSampleRate(NewSampleRate, bSendImmediately); }

	UFUNCTION(BlueprintPure, Category = "BS Tflite")
	const TArray<EBS_SensorType> &GetTfliteSensorTypes() const { return TfliteManager->GetSensorTypes(); }

	UFUNCTION(BlueprintCallable, Category = "BS Tflite")
	void SetTfliteSensorTypes(const TArray<EBS_SensorType> &NewSensorTypes, bool bSendImmediately = true) { return TfliteManager->SetSensorTypes(NewSensorTypes, bSendImmediately); }

	UFUNCTION(BlueprintPure, Category = "BS Tflite")
	const bool GetTfliteIsReady() const { return TfliteManager->GetIsReady(); }

	UFUNCTION(BlueprintPure, Category = "BS Tflite")
	const int32 GetTfliteCaptureDelay() const { return TfliteManager->GetCaptureDelay(); }

	UFUNCTION(BlueprintCallable, Category = "BS Tflite")
	void SetTfliteCaptureDelay(const int32 NewCaptureDelay, bool bSendImmediately = true) { return TfliteManager->SetCaptureDelay(BS_MathUtils::ClampToRange<uint16>(NewCaptureDelay), bSendImmediately); }

	UFUNCTION(BlueprintPure, Category = "BS Tflite")
	const float GetTfliteThreshold() const { return TfliteManager->GetThreshold(); }

	UFUNCTION(BlueprintCallable, Category = "BS Tflite")
	void SetTfliteThreshold(const float NewThreshold, bool bSendImmediately = true) { return TfliteManager->SetThreshold(NewThreshold, bSendImmediately); }

	UFUNCTION(BlueprintPure, Category = "BS Tflite")
	const bool GetTfliteInferencingEnabled() const { return TfliteManager->GetInferencingEnabled(); }

	UFUNCTION(BlueprintCallable, Category = "BS Tflite")
	void SetTfliteInferencingEnabled(const bool NewInferencingEnabled) { return TfliteManager->SetInferencingEnabled(NewInferencingEnabled); }

	UFUNCTION(BlueprintCallable, Category = "BS Tflite")
	void ToggleTfliteInferencingEnabled() { return TfliteManager->ToggleInferencingEnabled(); }

	UFUNCTION(BlueprintCallable, Category = "BS Tflite")
	void SendTfliteModel(const FBS_TfliteConfiguration &TfliteConfiguration, const TArray<uint8> &File)
	{
		TfliteManager->SetConfiguration(TfliteConfiguration, false);
		FileTransferManager->SendFile(EBS_FileType::TFLITE, File);
	}

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBS_TfliteNameCallback, UBS_Device *, Device, const FString &, Name);
	UPROPERTY(BlueprintAssignable, Category = "BS Tflite")
	FBS_TfliteNameCallback OnTfliteName;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBS_TfliteTaskCallback, UBS_Device *, Device, EBS_TfliteTask, Task);
	UPROPERTY(BlueprintAssignable, Category = "BS Tflite")
	FBS_TfliteTaskCallback OnTfliteTask;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBS_TfliteSampleRateCallback, UBS_Device *, Device, EBS_SensorRate, SampleRate);
	UPROPERTY(BlueprintAssignable, Category = "BS Tflite")
	FBS_TfliteSampleRateCallback OnTfliteSampleRate;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBS_TfliteSensorTypesCallback, UBS_Device *, Device, const TArray<EBS_SensorType> &, SensorTypes);
	UPROPERTY(BlueprintAssignable, Category = "BS Tflite")
	FBS_TfliteSensorTypesCallback OnTfliteSensorTypes;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBS_TfliteIsReadyCallback, UBS_Device *, Device, bool, IsReady);
	UPROPERTY(BlueprintAssignable, Category = "BS Tflite")
	FBS_TfliteIsReadyCallback OnTfliteIsReady;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBS_TfliteCaptureDelayCallback, UBS_Device *, Device, uint16, CaptureDelay);
	UPROPERTY(BlueprintAssignable, Category = "BS Tflite")
	FBS_TfliteCaptureDelayCallback OnTfliteCaptureDelay;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBS_TfliteThresholdCallback, UBS_Device *, Device, float, Threshold);
	UPROPERTY(BlueprintAssignable, Category = "BS Tflite")
	FBS_TfliteThresholdCallback OnTfliteThreshold;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBS_TfliteInferencingEnabledCallback, UBS_Device *, Device, bool, InferencingEnabled);
	UPROPERTY(BlueprintAssignable, Category = "BS Tflite")
	FBS_TfliteInferencingEnabledCallback OnTfliteInferencingEnabled;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FBS_TfliteInferenceCallback, UBS_Device *, Device, const TArray<float> &, Inference, const int64 &, Timestamp);
	UPROPERTY(BlueprintAssignable, Category = "BS Tflite")
	FBS_TfliteInferenceCallback OnTfliteInference;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "BS Tflite")
	UBS_TfliteManager *TfliteManager;

private:
	void OnTfliteNameUpdate(const FString &Name) { OnTfliteName.Broadcast(this, Name); }
	void OnTfliteTaskUpdate(EBS_TfliteTask Task) { OnTfliteTask.Broadcast(this, Task); }
	void OnTfliteSampleRateUpdate(EBS_SensorRate SampleRate) { OnTfliteSampleRate.Broadcast(this, SampleRate); }
	void OnTfliteSensorTypesUpdate(const TArray<EBS_SensorType> &SensorTypes) { OnTfliteSensorTypes.Broadcast(this, SensorTypes); }
	void OnTfliteIsReadyUpdate(bool IsReady) { OnTfliteIsReady.Broadcast(this, IsReady); }
	void OnTfliteCaptureDelayUpdate(uint16 CaptureDelay) { OnTfliteCaptureDelay.Broadcast(this, CaptureDelay); }
	void OnTfliteThresholdUpdate(float Threshold) { OnTfliteThreshold.Broadcast(this, Threshold); }
	void OnTfliteInferencingEnabledUpdate(bool InferencingEnabled) { OnTfliteInferencingEnabled.Broadcast(this, InferencingEnabled); }
	void OnTfliteInferenceUpdate(const TArray<float> &Inference, const uint64 &Timestamp) { OnTfliteInference.Broadcast(this, Inference, Timestamp); }
	// TFLITE END
};
