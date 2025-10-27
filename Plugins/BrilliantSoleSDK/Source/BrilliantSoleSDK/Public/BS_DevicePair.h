// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BS_Side.h"
#include "BS_Device.h"
#include "BS_DevicePairType.h"
#include "BS_DevicePairSensorDataManager.h"
#include "BS_DevicePair.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_DevicePair, Warning, All);

UCLASS(Abstract, BlueprintType, Blueprintable)
class UBS_DevicePair : public UObject
{
    GENERATED_BODY()

public:
    UBS_DevicePair();
    void PostInitProperties();

private:
    void Reset();

    // TYPE START
public:
    UPROPERTY(BlueprintReadOnly, Category = "BS Device Pair")
    EBS_DevicePairType Type;

    UFUNCTION(BlueprintCallable, Category = "BS Device Pair")
    void SetType(EBS_DevicePairType newType);
    // TYPE END

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

    // IS SINGLETON START
public:
    UFUNCTION(BlueprintCallable, Category = "BS Device Pair")
    void SetIsSingleton(bool bNewIsSingleton);

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = "BS Device Pair")
    void OnIsSingletonUpdate(bool IsSingleton);

private:
    bool bIsSingleton;
    // IS SINGLETON END

    // DEVICES START
public:
    UFUNCTION(BlueprintCallable, Category = "BS Device Pair")
    void AddDevice(UBS_Device *Device);
    UFUNCTION(BlueprintCallable, Category = "BS Device Pair")
    void RemoveDeviceBySide(const EBS_Side Side);
    UFUNCTION(BlueprintCallable, Category = "BS Device Pair")
    void RemoveDevice(UBS_Device *Device);
    UFUNCTION(BlueprintCallable, Category = "BS Device Pair")
    void RemoveDevices();

    UFUNCTION(BlueprintPure, Category = "BS Device Pair")
    bool HasAllDevices() const { return bHasAllDevices; }

protected:
    UPROPERTY(BlueprintReadOnly)
    TMap<EBS_Side, UBS_Device *> Devices;

    bool VerifyDevice(const UBS_Device *Device);

private:
    bool bHasAllDevices;
    void UpdateHasAllDevices();
    // DEVICES END

    // IS CONNECTED START
public:
    UFUNCTION(BlueprintPure, Category = "BS Device Pair")
    bool GetIsFullyConnected() { return bIsFullyConnected; };

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBS_DevicePairIsFullyConnectedCallback, UBS_DevicePair *, DevicePair, bool, IsFullyConnected);
    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair")
    FBS_DevicePairIsFullyConnectedCallback OnIsFullyConnectedUpdated;

private:
    bool bIsFullyConnected;
    void UpdateIsFullyConnected();
    // IS CONNECTED END

private:
    UPROPERTY()
    UBS_DevicePairSensorDataManager *SensorDataManager;

    // DEVICE LISTENERS START
private:
    void AddDeviceListeners(UBS_Device *Device);
    void RemoveDeviceListeners(UBS_Device *Device);
    // DEVICE LISTENERS END

    // DEVICE CONNECTION LISTENERS START
public:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FBS_DevicePairDeviceConnectionStatusCallback, UBS_DevicePair *, DevicePair, EBS_Side, Side, UBS_Device *, Device, EBS_ConnectionStatus, ConnectionStatus);
    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair")
    FBS_DevicePairDeviceConnectionStatusCallback OnDeviceConnectionStatusUpdate;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FBS_DevicePairDeviceIsConnectedCallback, UBS_DevicePair *, DevicePair, EBS_Side, Side, UBS_Device *, Device, bool, IsConnected);
    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair")
    FBS_DevicePairDeviceIsConnectedCallback OnDeviceIsConnectedUpdate;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FBS_DevicePairDeviceGenericConnectionCallback, UBS_DevicePair *, DevicePair, EBS_Side, Side, UBS_Device *, Device);
    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair")
    FBS_DevicePairDeviceGenericConnectionCallback OnDeviceConnected;
    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair")
    FBS_DevicePairDeviceGenericConnectionCallback OnDeviceNotConnected;
    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair")
    FBS_DevicePairDeviceGenericConnectionCallback OnDeviceConnecting;
    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair")
    FBS_DevicePairDeviceGenericConnectionCallback OnDeviceDisconnecting;

private:
    UFUNCTION()
    void _OnDeviceConnectionStatusUpdate(UBS_Device *Device, EBS_ConnectionStatus ConnectionStatus)
    {
        OnDeviceConnectionStatusUpdate.Broadcast(this, Device->Side(), Device, ConnectionStatus);

        switch (ConnectionStatus)
        {
        case EBS_ConnectionStatus::CONNECTED:
            OnDeviceConnected.Broadcast(this, Device->Side(), Device);
            break;
        case EBS_ConnectionStatus::NOT_CONNECTED:
            OnDeviceNotConnected.Broadcast(this, Device->Side(), Device);
            break;
        case EBS_ConnectionStatus::CONNECTING:
            OnDeviceConnecting.Broadcast(this, Device->Side(), Device);
            break;
        case EBS_ConnectionStatus::DISCONNECTING:
            OnDeviceDisconnecting.Broadcast(this, Device->Side(), Device);
            break;
        }
    }

    UFUNCTION()
    void _OnDeviceIsConnectedUpdate(UBS_Device *Device, bool bIsConnected)
    {
        OnDeviceIsConnectedUpdate.Broadcast(this, Device->Side(), Device, bIsConnected);
        UpdateIsFullyConnected();
    }
    // DEVICE CONNECTION LISTENERS END

    // SENSOR CONFIGURATION START
public:
    UFUNCTION(BlueprintCallable, Category = "BS Device Pair Sensor Configuration")
    void SetSensorConfiguration(const UBS_SensorConfiguration *NewSensorConfiguration, bool bClearRest = false);

    UFUNCTION(BlueprintCallable, Category = "BS Device Pair Sensor Configuration")
    void ClearSensorConfiguration();

    UFUNCTION(BlueprintCallable, Category = "BS Device Pair Sensor Configuration")
    void SetSensorRate(EBS_SensorType SensorType, EBS_SensorRate SensorRate, bool &bDidUpdateSensorRate);

    UFUNCTION(BlueprintCallable, Category = "BS Device Pair Sensor Configuration")
    void SetSensorRates(const TMap<EBS_SensorType, EBS_SensorRate> &SensorRates);

    UFUNCTION(BlueprintCallable, Category = "BS Device Pair Sensor Configuration")
    void ClearSensorRate(EBS_SensorType SensorType);

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FBS_DevicePairDeviceSensorConfigurationCallback, UBS_DevicePair *, DevicePair, EBS_Side, Side, UBS_Device *, Device, const UBS_SensorConfiguration *, SensorConfiguration);
    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair Sensor Configuration")
    FBS_DevicePairDeviceSensorConfigurationCallback OnDeviceSensorConfiguration;

private:
    UFUNCTION()
    void OnDeviceSensorConfigurationUpdate(UBS_Device *Device, const UBS_SensorConfiguration *SensorConfiguration) { OnDeviceSensorConfiguration.Broadcast(this, Device->Side(), Device, SensorConfiguration); }

    // SENSOR CONFIGURATION END

    // DEVICE MOTION DATA LISTENERS START
public:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FBS_DevicePairDeviceVectorCallback, UBS_DevicePair *, DevicePair, EBS_Side, Side, UBS_Device *, Device, const FVector &, Vector, const int64 &, Timestamp);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FBS_DevicePairDeviceRotatorCallback, UBS_DevicePair *, DevicePair, EBS_Side, Side, UBS_Device *, Device, const FRotator &, Rotator, const int64 &, Timestamp);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FBS_DevicePairDeviceQuaternionCallback, UBS_DevicePair *, DevicePair, EBS_Side, Side, UBS_Device *, Device, const FQuat &, Quaternion, const int64 &, Timestamp);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FBS_DevicePairDeviceTimestampCallback, UBS_DevicePair *, DevicePair, EBS_Side, Side, UBS_Device *, Device, const int64 &, Timestamp);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FBS_DevicePairDeviceActivityCallback, UBS_DevicePair *, DevicePair, EBS_Side, Side, UBS_Device *, Device, const TSet<EBS_Activity> &, Activity, const int64 &, Timestamp);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FBS_DevicePairDeviceStepCountCallback, UBS_DevicePair *, DevicePair, EBS_Side, Side, UBS_Device *, Device, const uint32 &, StepCount, const int64 &, Timestamp);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FBS_DevicePairDeviceDeviceOrientationCallback, UBS_DevicePair *, DevicePair, EBS_Side, Side, UBS_Device *, Device, const EBS_DeviceOrientation &, DeviceOrientation, const int64 &, Timestamp);

    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair Motion Data")
    FBS_DevicePairDeviceVectorCallback OnDeviceAcceleration;
    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair Motion Data")
    FBS_DevicePairDeviceVectorCallback OnDeviceGravity;
    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair Motion Data")
    FBS_DevicePairDeviceVectorCallback OnDeviceLinearAcceleration;
    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair Motion Data")
    FBS_DevicePairDeviceVectorCallback OnDeviceGyroscope;
    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair Motion Data")
    FBS_DevicePairDeviceVectorCallback OnDeviceMagnetometer;
    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair Motion Data")
    FBS_DevicePairDeviceQuaternionCallback OnDeviceGameRotation;
    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair Motion Data")
    FBS_DevicePairDeviceQuaternionCallback OnDeviceRotation;

    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair Motion Data")
    FBS_DevicePairDeviceRotatorCallback OnDeviceOrientation;
    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair Motion Data")
    FBS_DevicePairDeviceActivityCallback OnDeviceActivity;
    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair Motion Data")
    FBS_DevicePairDeviceStepCountCallback OnDeviceStepCount;
    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair Motion Data")
    FBS_DevicePairDeviceTimestampCallback OnDeviceStepDetection;
    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair Motion Data")
    FBS_DevicePairDeviceDeviceOrientationCallback OnDeviceDeviceOrientation;

private:
    UFUNCTION()
    void OnDeviceAccelerationUpdate(UBS_Device *Device, const FVector &Vector, const int64 &Timestamp) { OnDeviceAcceleration.Broadcast(this, Device->Side(), Device, Vector, Timestamp); }
    UFUNCTION()
    void OnDeviceGravityUpdate(UBS_Device *Device, const FVector &Vector, const int64 &Timestamp) { OnDeviceGravity.Broadcast(this, Device->Side(), Device, Vector, Timestamp); }
    UFUNCTION()
    void OnDeviceLinearAccelerationUpdate(UBS_Device *Device, const FVector &Vector, const int64 &Timestamp) { OnDeviceLinearAcceleration.Broadcast(this, Device->Side(), Device, Vector, Timestamp); }
    UFUNCTION()
    void OnDeviceGyroscopeUpdate(UBS_Device *Device, const FVector &Vector, const int64 &Timestamp) { OnDeviceGyroscope.Broadcast(this, Device->Side(), Device, Vector, Timestamp); }
    UFUNCTION()
    void OnDeviceMagnetometerUpdate(UBS_Device *Device, const FVector &Vector, const int64 &Timestamp) { OnDeviceMagnetometer.Broadcast(this, Device->Side(), Device, Vector, Timestamp); }
    UFUNCTION()
    void OnDeviceGameRotationUpdate(UBS_Device *Device, const FQuat &Quaternion, const int64 &Timestamp) { OnDeviceGameRotation.Broadcast(this, Device->Side(), Device, Quaternion, Timestamp); }
    UFUNCTION()
    void OnDeviceRotationUpdate(UBS_Device *Device, const FQuat &Quaternion, const int64 &Timestamp) { OnDeviceRotation.Broadcast(this, Device->Side(), Device, Quaternion, Timestamp); }

    UFUNCTION()
    void OnDeviceOrientationUpdate(UBS_Device *Device, const FRotator &Rotator, const int64 &Timestamp) { OnDeviceOrientation.Broadcast(this, Device->Side(), Device, Rotator, Timestamp); }
    UFUNCTION()
    void OnDeviceActivityUpdate(UBS_Device *Device, const TSet<EBS_Activity> &Activity, const int64 &Timestamp) { OnDeviceActivity.Broadcast(this, Device->Side(), Device, Activity, Timestamp); }
    UFUNCTION()
    void OnDeviceStepCountUpdate(UBS_Device *Device, const uint32 &StepCount, const int64 &Timestamp) { OnDeviceStepCount.Broadcast(this, Device->Side(), Device, StepCount, Timestamp); }
    UFUNCTION()
    void OnDeviceStepDetectionUpdate(UBS_Device *Device, const int64 &Timestamp) { OnDeviceStepDetection.Broadcast(this, Device->Side(), Device, Timestamp); }
    UFUNCTION()
    void OnDeviceDeviceOrientationUpdate(UBS_Device *Device, const EBS_DeviceOrientation &DeviceOrientation, const int64 &Timestamp) { OnDeviceDeviceOrientation.Broadcast(this, Device->Side(), Device, DeviceOrientation, Timestamp); }
    // DEVICE MOTION DATA LISTENERS END

    // DEVICE BAROMETER LISTENERS START
public:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FBS_DevicePairDeviceBarometerCallback, UBS_DevicePair *, DevicePair, EBS_Side, Side, UBS_Device *, Device, const float &, Barometer, const int64 &, Timestamp);

    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair Barometer Data")
    FBS_DevicePairDeviceBarometerCallback OnDeviceBarometer;

private:
    UFUNCTION()
    void OnDeviceBarometerUpdate(UBS_Device *Device, const float &Barometer, const int64 &Timestamp) { OnDeviceBarometer.Broadcast(this, Device->Side(), Device, Barometer, Timestamp); }
    // DEVICE BAROMETER LISTENERS END

    // PRESSURE START
public:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FBS_DevicePairDevicePressureCallback, UBS_DevicePair *, DevicePair, EBS_Side, Side, UBS_Device *, Device, const FBS_PressureData &, PressureData, const int64 &, Timestamp);
    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair Pressure")
    FBS_DevicePairDevicePressureCallback OnDevicePressure;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FBS_DevicePairPressureCallback, UBS_DevicePair *, DevicePair, const FBS_DevicePairPressureData &, PressureData, const int64 &, Timestamp);
    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair Pressure")
    FBS_DevicePairPressureCallback OnPressure;

private:
    UFUNCTION()
    void OnDevicePressureUpdate(UBS_Device *Device, const FBS_PressureData &PressureData, const int64 &Timestamp);

    UFUNCTION()
    void OnPressureUpdate(const FBS_DevicePairPressureData &PressureData, const int64 &Timestamp) { OnPressure.Broadcast(this, PressureData, Timestamp); }
    // PRESSURE END

    // VIBRATION START
public:
    UFUNCTION(BlueprintCallable, Category = "BS Device Pair Vibration")
    void TriggerVibration(const TArray<FBS_VibrationConfiguration> &VibrationConfigurations);
    // VIBRATION END

    // FILE TRANSFER START
public:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FBS_DevicePairDeviceFileTransferStatusCallback, UBS_DevicePair *, DevicePair, EBS_Side, Side, UBS_Device *, Device, EBS_FileTransferStatus, FileTransferStatus);
    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair File Transfer")
    FBS_DevicePairDeviceFileTransferStatusCallback OnDeviceFileTransferStatus;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FBS_DevicePairDeviceFileTransferFileReceivedCallback, UBS_DevicePair *, DevicePair, EBS_Side, Side, UBS_Device *, Device, EBS_FileType, FileType, const TArray<uint8> &, File);
    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair File Transfer")
    FBS_DevicePairDeviceFileTransferFileReceivedCallback OnDeviceFileReceived;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FBS_DevicePairDeviceFileTransferProgressCallback, UBS_DevicePair *, DevicePair, EBS_Side, Side, UBS_Device *, Device, EBS_FileType, FileType, EBS_FileTransferDirection, FileTransferDirection, float, Progress);
    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair File Transfer")
    FBS_DevicePairDeviceFileTransferProgressCallback OnDeviceFileTransferProgress;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FBS_DevicePairDeviceFileTransferCompleteCallback, UBS_DevicePair *, DevicePair, EBS_Side, Side, UBS_Device *, Device, EBS_FileType, FileType, EBS_FileTransferDirection, FileTransferDirection);
    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair File Transfer")
    FBS_DevicePairDeviceFileTransferCompleteCallback OnDeviceFileTransferComplete;

private:
    UFUNCTION()
    void OnDeviceFileTransferStatusUpdate(UBS_Device *Device, const EBS_FileTransferStatus FileTransferStatus)
    {
        OnDeviceFileTransferStatus.Broadcast(this, Device->Side(), Device, FileTransferStatus);
    }

    UFUNCTION()
    void OnDeviceFileReceivedUpdate(UBS_Device *Device, const EBS_FileType FileType, const TArray<uint8> &File)
    {
        OnDeviceFileReceived.Broadcast(this, Device->Side(), Device, FileType, File);
    }

    UFUNCTION()
    void OnDeviceFileTransferProgressUpdate(UBS_Device *Device, const EBS_FileType FileType, EBS_FileTransferDirection FileTransferDirection, float Progress)
    {
        OnDeviceFileTransferProgress.Broadcast(this, Device->Side(), Device, FileType, FileTransferDirection, Progress);
    }

    UFUNCTION()
    void OnDeviceFileTransferCompleteUpdate(UBS_Device *Device, const EBS_FileType FileType, EBS_FileTransferDirection FileTransferDirection)
    {
        OnDeviceFileTransferComplete.Broadcast(this, Device->Side(), Device, FileType, FileTransferDirection);
    }
    // FILE TRANSFER END

    // TFLITE START
public:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FBS_DeviceTfliteIsReadyCallback, UBS_DevicePair *, DevicePair, EBS_Side, Side, UBS_Device *, Device, bool, IsReady);
    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair Tflite")
    FBS_DeviceTfliteIsReadyCallback OnDeviceTfliteIsReady;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FBS_DeviceTfliteInferencingEnabledCallback, UBS_DevicePair *, DevicePair, EBS_Side, Side, UBS_Device *, Device, bool, InferencingEnabled);
    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair Tflite")
    FBS_DeviceTfliteInferencingEnabledCallback OnDeviceTfliteInferencingEnabled;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FBS_DevicePairDeviceTfliteInferenceCallback, UBS_DevicePair *, DevicePair, EBS_Side, Side, UBS_Device *, Device, const TArray<float> &, Inference, const int64 &, Timestamp);
    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair Tflite")
    FBS_DevicePairDeviceTfliteInferenceCallback OnDeviceTfliteInference;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FBS_DevicePairDeviceTfliteClassificationCallback, UBS_DevicePair *, DevicePair, EBS_Side, Side, UBS_Device *, Device, const uint32 &, ClassificationIndex, const float &, ClassificationValue, const int64 &, Timestamp);
    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair Tflite")
    FBS_DevicePairDeviceTfliteClassificationCallback OnDeviceTfliteClassification;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_SevenParams(FBS_DevicePairDeviceTfliteNamedClassificationCallback, UBS_DevicePair *, DevicePair, EBS_Side, Side, UBS_Device *, Device, const FString &, ClassificationName, const uint32 &, ClassificationIndex, const float &, ClassificationValue, const int64 &, Timestamp);
    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair Tflite")
    FBS_DevicePairDeviceTfliteNamedClassificationCallback OnDeviceTfliteNamedClassification;

    UFUNCTION(BlueprintCallable, Category = "BS Device Pair Tflite")
    void SetTfliteInferencingEnabled(const bool NewInferencingEnabled);

    UFUNCTION(BlueprintCallable, Category = "BS Device Pair Tflite")
    void SendTfliteModel(const FBS_TfliteConfiguration &TfliteConfiguration, const TArray<uint8> &File);

private:
    UFUNCTION()
    void OnDeviceTfliteIsReadyUpdate(UBS_Device *Device, bool IsReady)
    {
        OnDeviceTfliteIsReady.Broadcast(this, Device->Side(), Device, IsReady);
    }

    UFUNCTION()
    void OnDeviceTfliteInferencingEnabledUpdate(UBS_Device *Device, bool InferencingEnabled)
    {
        OnDeviceTfliteInferencingEnabled.Broadcast(this, Device->Side(), Device, InferencingEnabled);
    }

    UFUNCTION()
    void OnDeviceTfliteInferenceUpdate(UBS_Device *Device, const TArray<float> &Inference, const int64 &Timestamp)
    {
        OnDeviceTfliteInference.Broadcast(this, Device->Side(), Device, Inference, Timestamp);
    }

    UFUNCTION()
    void OnDeviceTfliteClassificationUpdate(UBS_Device *Device, const uint32 &ClassificationIndex, const float &ClassificationValue, const int64 &Timestamp)
    {
        OnDeviceTfliteClassification.Broadcast(this, Device->Side(), Device, ClassificationIndex, ClassificationValue, Timestamp);
    }

    UFUNCTION()
    void OnDeviceTfliteNamedClassificationUpdate(UBS_Device *Device, const FString &ClassificationName, const uint32 &ClassificationIndex, const float &ClassificationValue, const int64 &Timestamp)
    {
        OnDeviceTfliteNamedClassification.Broadcast(this, Device->Side(), Device, ClassificationName, ClassificationIndex, ClassificationValue, Timestamp);
    }
    // TFLITE END
};