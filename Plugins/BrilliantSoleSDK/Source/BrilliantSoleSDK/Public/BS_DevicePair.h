// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BS_InsoleSide.h"
#include "BS_Device.h"
#include "BS_DevicePairSensorDataManager.h"
#include "BS_DevicePair.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_DevicePair, Log, All);

UCLASS(Abstract, BlueprintType)
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

    // IS SHARED START
public:
    void SetIsShared(bool bNewIsShared);

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = "BS Device Pair")
    void OnIsSharedUpdate(bool IsShared);

private:
    bool bIsShared;
    // IS SHARED END

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
    UPROPERTY()
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

    // PRESSURE START
private:
    void OnDevicePressure(UBS_Device *Device, const FBS_PressureData &PressureData, const float &Timestamp);
    // PRESSURE END
};