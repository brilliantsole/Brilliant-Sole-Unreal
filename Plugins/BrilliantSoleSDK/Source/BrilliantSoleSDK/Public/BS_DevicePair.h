// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BS_InsoleSide.h"
#include "BS_Device.h"
#include "BS_DevicePairSensorDataManager.h"
#include "BS_DevicePair.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_DevicePair, Log, All);

UCLASS(Abstract, BlueprintType, Blueprintable)
class UBS_DevicePair : public UObject
{
    GENERATED_BODY()

public:
    UBS_DevicePair();
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
    void RemoveDeviceBySide(const EBS_InsoleSide InsoleSide);
    UFUNCTION(BlueprintCallable, Category = "BS Device Pair")
    void RemoveDevice(UBS_Device *Device);
    UFUNCTION(BlueprintCallable, Category = "BS Device Pair")
    void RemoveDevices();

    UFUNCTION(BlueprintPure, Category = "BS Device Pair")
    bool HasAllDevices() const { return bHasAllDevices; }

protected:
    UPROPERTY(BlueprintReadOnly)
    TMap<EBS_InsoleSide, UBS_Device *> Devices;

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
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FBS_DevicePairDeviceConnectionStatusCallback, UBS_DevicePair *, DevicePair, EBS_InsoleSide, Side, UBS_Device *, Device, EBS_ConnectionStatus, ConnectionStatus);
    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair")
    FBS_DevicePairDeviceConnectionStatusCallback OnDeviceConnectionStatusUpdate;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FBS_DevicePairDeviceIsConnectedCallback, UBS_DevicePair *, DevicePair, EBS_InsoleSide, Side, UBS_Device *, Device, bool, IsConnected);
    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair")
    FBS_DevicePairDeviceIsConnectedCallback OnDeviceIsConnectedUpdate;

private:
    UFUNCTION()
    void _OnDeviceConnectionStatusUpdate(UBS_Device *Device, EBS_ConnectionStatus ConnectionStatus) { OnDeviceConnectionStatusUpdate.Broadcast(this, Device->InsoleSide(), Device, ConnectionStatus); }

    UFUNCTION()
    void _OnDeviceIsConnectedUpdate(UBS_Device *Device, bool bIsConnected)
    {
        OnDeviceIsConnectedUpdate.Broadcast(this, Device->InsoleSide(), Device, bIsConnected);
        UpdateIsFullyConnected();
    }
    // DEVICE CONNECTION LISTENERS END

    // SENSOR CONFIGURATION START
public:
    UFUNCTION(BlueprintCallable, Category = "BS Device Pair Sensor Configuration")
    void SetSensorConfiguration(const UBS_SensorConfiguration *NewSensorConfiguration);

    UFUNCTION(BlueprintCallable, Category = "BS Device Pair Sensor Configuration")
    void ClearSensorConfiguration();

    UFUNCTION(BlueprintCallable, Category = "BS Device Pair Sensor Configuration")
    void SetSensorRate(EBS_SensorType SensorType, EBS_SensorRate SensorRate, bool &bDidUpdateSensorRate);

    UFUNCTION(BlueprintCallable, Category = "BS Device Pair Sensor Configuration")
    void SetSensorRates(const TMap<EBS_SensorType, EBS_SensorRate> &SensorRates);

    UFUNCTION(BlueprintCallable, Category = "BS Device Pair Sensor Configuration")
    void ClearSensorRate(EBS_SensorType SensorType);

    UFUNCTION(BlueprintCallable, Category = "BS Device Pair Sensor Configuration")
    void ToggleSensorRate(EBS_SensorType SensorType, EBS_SensorRate SensorRate, EBS_SensorRate &UpdatedSensorRate);

    // SENSOR CONFIGURATION END

    // DEVICE MOTION DATA LISTENERS START
public:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FBS_DevicePairDeviceVectorCallback, UBS_DevicePair *, DevicePair, EBS_InsoleSide, Side, UBS_Device *, Device, const FVector &, Vector, const float &, Timestamp);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FBS_DevicePairDeviceRotatorCallback, UBS_DevicePair *, DevicePair, EBS_InsoleSide, Side, UBS_Device *, Device, const FRotator &, Rotator, const float &, Timestamp);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FBS_DevicePairDeviceQuaternionCallback, UBS_DevicePair *, DevicePair, EBS_InsoleSide, Side, UBS_Device *, Device, const FQuat &, Quaternion, const float &, Timestamp);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FBS_DevicePairDeviceTimestampCallback, UBS_DevicePair *, DevicePair, EBS_InsoleSide, Side, UBS_Device *, Device, const float &, Timestamp);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FBS_DevicePairDeviceActivityCallback, UBS_DevicePair *, DevicePair, EBS_InsoleSide, Side, UBS_Device *, Device, const TSet<EBS_Activity> &, Activity, const float &, Timestamp);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FBS_DevicePairDeviceStepCountCallback, UBS_DevicePair *, DevicePair, EBS_InsoleSide, Side, UBS_Device *, Device, const uint32 &, StepCount, const float &, Timestamp);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FBS_DevicePairDeviceDeviceOrientationCallback, UBS_DevicePair *, DevicePair, EBS_InsoleSide, Side, UBS_Device *, Device, const EBS_DeviceOrientation &, DeviceOrientation, const float &, Timestamp);

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
    void OnDeviceAccelerationUpdate(UBS_Device *Device, const FVector &Vector, const float &Timestamp) { OnDeviceAcceleration.Broadcast(this, Device->InsoleSide(), Device, Vector, Timestamp); }
    UFUNCTION()
    void OnDeviceGravityUpdate(UBS_Device *Device, const FVector &Vector, const float &Timestamp) { OnDeviceGravity.Broadcast(this, Device->InsoleSide(), Device, Vector, Timestamp); }
    UFUNCTION()
    void OnDeviceLinearAccelerationUpdate(UBS_Device *Device, const FVector &Vector, const float &Timestamp) { OnDeviceLinearAcceleration.Broadcast(this, Device->InsoleSide(), Device, Vector, Timestamp); }
    UFUNCTION()
    void OnDeviceGyroscopeUpdate(UBS_Device *Device, const FVector &Vector, const float &Timestamp) { OnDeviceGyroscope.Broadcast(this, Device->InsoleSide(), Device, Vector, Timestamp); }
    UFUNCTION()
    void OnDeviceMagnetometerUpdate(UBS_Device *Device, const FVector &Vector, const float &Timestamp) { OnDeviceMagnetometer.Broadcast(this, Device->InsoleSide(), Device, Vector, Timestamp); }
    UFUNCTION()
    void OnDeviceGameRotationUpdate(UBS_Device *Device, const FQuat &Quaternion, const float &Timestamp) { OnDeviceGameRotation.Broadcast(this, Device->InsoleSide(), Device, Quaternion, Timestamp); }
    UFUNCTION()
    void OnDeviceRotationUpdate(UBS_Device *Device, const FQuat &Quaternion, const float &Timestamp) { OnDeviceRotation.Broadcast(this, Device->InsoleSide(), Device, Quaternion, Timestamp); }

    UFUNCTION()
    void OnDeviceOrientationUpdate(UBS_Device *Device, const FRotator &Rotator, const float &Timestamp) { OnDeviceOrientation.Broadcast(this, Device->InsoleSide(), Device, Rotator, Timestamp); }
    UFUNCTION()
    void OnDeviceActivityUpdate(UBS_Device *Device, const TSet<EBS_Activity> &Activity, const float &Timestamp) { OnDeviceActivity.Broadcast(this, Device->InsoleSide(), Device, Activity, Timestamp); }
    UFUNCTION()
    void OnDeviceStepCountUpdate(UBS_Device *Device, const uint32 &StepCount, const float &Timestamp) { OnDeviceStepCount.Broadcast(this, Device->InsoleSide(), Device, StepCount, Timestamp); }
    UFUNCTION()
    void OnDeviceStepDetectionUpdate(UBS_Device *Device, const float &Timestamp) { OnDeviceStepDetection.Broadcast(this, Device->InsoleSide(), Device, Timestamp); }
    UFUNCTION()
    void OnDeviceDeviceOrientationUpdate(UBS_Device *Device, const EBS_DeviceOrientation &DeviceOrientation, const float &Timestamp) { OnDeviceDeviceOrientation.Broadcast(this, Device->InsoleSide(), Device, DeviceOrientation, Timestamp); }
    // DEVICE MOTION DATA LISTENERS END

    // DEVICE BAROMETER LISTENERS START
public:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FBS_DevicePairDeviceBarometerCallback, UBS_DevicePair *, DevicePair, EBS_InsoleSide, Side, UBS_Device *, Device, const float &, Barometer, const float &, Timestamp);

    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair Barometer Data")
    FBS_DevicePairDeviceBarometerCallback OnDeviceBarometer;

private:
    UFUNCTION()
    void OnDeviceBarometerUpdate(UBS_Device *Device, const float &Barometer, const float &Timestamp) { OnDeviceBarometer.Broadcast(this, Device->InsoleSide(), Device, Barometer, Timestamp); }
    // DEVICE BAROMETER LISTENERS END

    // PRESSURE START
public:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FBS_DevicePairDevicePressureCallback, UBS_DevicePair *, DevicePair, EBS_InsoleSide, Side, UBS_Device *, Device, const FBS_PressureData &, PressureData, const float &, Timestamp);
    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair Pressure")
    FBS_DevicePairDevicePressureCallback OnDevicePressure;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FBS_DevicePairPressureCallback, UBS_DevicePair *, DevicePair, const FBS_DevicePairPressureData &, PressureData, const float &, Timestamp);
    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair Pressure")
    FBS_DevicePairPressureCallback OnPressure;

private:
    UFUNCTION()
    void OnDevicePressureUpdate(UBS_Device *Device, const FBS_PressureData &PressureData, const float &Timestamp);

    UFUNCTION()
    void OnPressureUpdate(const FBS_DevicePairPressureData &PressureData, const float &Timestamp) { OnPressure.Broadcast(this, PressureData, Timestamp); }
    // PRESSURE END

    // VIBRATION START
public:
    UFUNCTION(BlueprintCallable, Category = "BS Device Pair Vibration")
    void TriggerVibration(const TArray<FBS_VibrationConfiguration> &VibrationConfigurations);
    // VIBRATION END

    // TFLITE START
public:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FBS_DevicePairDeviceTfliteInferenceCallback, UBS_DevicePair *, DevicePair, EBS_InsoleSide, Side, UBS_Device *, Device, const TArray<float> &, Inference, const float &, Timestamp);
    UPROPERTY(BlueprintAssignable, Category = "BS Device Pair Tflite")
    FBS_DevicePairDeviceTfliteInferenceCallback OnDeviceTfliteInference;

    UFUNCTION(BlueprintCallable, Category = "BS Device Pair Tflite")
    void SetTfliteInferencingEnabled(const bool NewInferencingEnabled);

private:
    UFUNCTION()
    void OnDeviceTfliteInferenceUpdate(UBS_Device *Device, const TArray<float> &Inference, const float &Timestamp)
    {
        OnDeviceTfliteInference.Broadcast(this, Device->InsoleSide(), Device, Inference, Timestamp);
    }
    // TFLITE END
};