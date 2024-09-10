// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_DevicePair.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(LogBS_DevicePair);

UBS_DevicePair::UBS_DevicePair()
{
    UE_LOGFMT(LogBS_DevicePair, Log, "Constructor: {0}", GetName());
    if (HasAnyFlags(RF_ClassDefaultObject))
    {
        UE_LOGFMT(LogBS_DevicePair, Log, "CDO - Skipping Constructor");
        return;
    }

    SensorDataManager = CreateDefaultSubobject<UBS_DevicePairSensorDataManager>(TEXT("SensorDataManager"));
}

void UBS_DevicePair::PostInitProperties()
{
    Super::PostInitProperties();
    UE_LOGFMT(LogBS_DevicePair, Log, "PostInitProperties {0}", GetName());
    if (HasAnyFlags(RF_ClassDefaultObject))
    {
        UE_LOGFMT(LogBS_DevicePair, Log, "CDO - Skipping");
        return;
    }

    GetBS_Subsystem();
    InitializeBP();
}

void UBS_DevicePair::GetBS_Subsystem()
{
    if (!HasAnyFlags(RF_ClassDefaultObject))
    {
        UWorld *World = GetWorld();
        if (World)
        {
            UE_LOGFMT(LogBS_DevicePair, Log, "World found");
            UGameInstance *GameInstance = World->GetGameInstance();
            if (GameInstance)
            {
                UE_LOGFMT(LogBS_DevicePair, Log, "GameInstance found");
                UBS_Subsystem *__BS_Subsystem = GameInstance->GetSubsystem<UBS_Subsystem>();
                if (__BS_Subsystem)
                {
                    UE_LOGFMT(LogBS_DevicePair, Log, "BS_Subsystem found");
                    _BS_Subsystem = __BS_Subsystem;
                }
                else
                {
                    UE_LOGFMT(LogBS_DevicePair, Error, "BS_Subsystem not found");
                }
            }
            else
            {
                UE_LOGFMT(LogBS_DevicePair, Error, "GameInstance not found");
            }
        }
        else
        {
            UE_LOGFMT(LogBS_DevicePair, Error, "World not found");
        }
    }
    else
    {
        UE_LOGFMT(LogBS_DevicePair, Log, "CDO constructor - skipping");
    }
}

void UBS_DevicePair::Reset()
{
    SensorDataManager->Reset();
}

// IS SINGLETON START
void UBS_DevicePair::SetIsSingleton(bool bNewIsSingleton)
{
    if (bIsSingleton == bNewIsSingleton)
    {
        UE_LOGFMT(LogBS_DevicePair, Log, "redundant bIsSingleton assignment - skipping");
        return;
    }

    bIsSingleton = bNewIsSingleton;
    UE_LOGFMT(LogBS_DevicePair, Log, "Updated bIsSingleton to {0}", bIsSingleton);
    OnIsSingletonUpdate(bIsSingleton);
}
// IS SINGLETON END

// DEVICES START
bool UBS_DevicePair::VerifyDevice(const UBS_Device *Device)
{
    if (Device == nullptr)
    {
        UE_LOGFMT(LogBS_DevicePair, Error, "Device is nullptr");
        return false;
    }
    if (!Device->IsInsole())
    {
        UE_LOGFMT(LogBS_DevicePair, Log, "Device is not an Insole");
        return false;
    }

    return true;
}
void UBS_DevicePair::AddDevice(UBS_Device *Device)
{
    if (!VerifyDevice(Device))
    {
        return;
    }

    const EBS_InsoleSide InsoleSide = Device->InsoleSide();
    if (Devices.Contains(InsoleSide) && Devices[InsoleSide] == Device)
    {
        UE_LOGFMT(LogBS_DevicePair, Log, "Redundant Device Assignment");
        return;
    }
    if (Devices.Contains(InsoleSide))
    {
        UE_LOGFMT(LogBS_DevicePair, Log, "Removing existing {0} Device...", UEnum::GetValueAsString(InsoleSide));
        RemoveDeviceBySide(InsoleSide);
    }

    UE_LOGFMT(LogBS_DevicePair, Log, "Adding {0} Device \"{1}\"", UEnum::GetValueAsString(InsoleSide), Device->GetName());
    Devices.Emplace(InsoleSide, Device);
    AddDeviceListeners(Device);
    UpdateHasAllDevices();
}
void UBS_DevicePair::RemoveDeviceBySide(const EBS_InsoleSide InsoleSide)
{
    if (!Devices.Contains(InsoleSide))
    {
        UE_LOGFMT(LogBS_DevicePair, Log, "{0} Device not assigned", UEnum::GetValueAsString(InsoleSide));
        return;
    }
    RemoveDevice(Devices[InsoleSide]);
}
void UBS_DevicePair::RemoveDevice(UBS_Device *Device)
{
    if (!VerifyDevice(Device))
    {
        return;
    }

    const EBS_InsoleSide InsoleSide = Device->InsoleSide();
    if (!Devices.Contains(InsoleSide) || Devices[InsoleSide] != Device)
    {
        UE_LOGFMT(LogBS_DevicePair, Log, "Device is not assigned");
        return;
    }

    UE_LOGFMT(LogBS_DevicePair, Log, "Removing {0} Device \"{1}\"", UEnum::GetValueAsString(InsoleSide), Device->GetName());
    RemoveDeviceListeners(Device);
    Devices.Remove(InsoleSide);
    UpdateHasAllDevices();
}
void UBS_DevicePair::RemoveDevices()
{
    for (const EBS_InsoleSide InsoleSide : TEnumRange<EBS_InsoleSide>())
    {
        RemoveDeviceBySide(InsoleSide);
    }
}

void UBS_DevicePair::UpdateHasAllDevices()
{
    bool bNewHasAllDevices = true;
    for (const EBS_InsoleSide InsoleSide : TEnumRange<EBS_InsoleSide>())
    {
        bNewHasAllDevices = bNewHasAllDevices || Devices.Contains(InsoleSide);
        if (!bNewHasAllDevices)
        {
            break;
        }
    }
    if (bHasAllDevices == bNewHasAllDevices)
    {
        return;
    }
    bHasAllDevices = bNewHasAllDevices;
}
// DEVICES START

// IS CONNECTED START
void UBS_DevicePair::UpdateIsFullyConnected()
{
    bool bNewIsFullyConnected = true;

    if (bHasAllDevices)
    {
        for (const EBS_InsoleSide InsoleSide : TEnumRange<EBS_InsoleSide>())
        {
            bNewIsFullyConnected = bNewIsFullyConnected || Devices[InsoleSide]->IsConnected();
            if (!bNewIsFullyConnected)
            {
                break;
            }
        }
    }
    else
    {
        bNewIsFullyConnected = false;
    }

    if (bIsFullyConnected == bNewIsFullyConnected)
    {
        return;
    }

    bIsFullyConnected = bNewIsFullyConnected;
    UE_LOGFMT(LogBS_DevicePair, Log, "Updating bIsFullyConnected to {0}", bIsFullyConnected);

    OnIsFullyConnectedUpdated.Broadcast(this, bIsFullyConnected);
}
// IS CONNECTED END

// DEVICE LISTENERS START
void UBS_DevicePair::AddDeviceListeners(UBS_Device *Device)
{
    if (!VerifyDevice(Device))
    {
        return;
    }
    UE_LOGFMT(LogBS_DevicePair, Log, "AddDeviceListeners to \"{0}\"", Device->Name());

    Device->OnConnectionStatusUpdate.AddDynamic(this, &UBS_DevicePair::_OnDeviceConnectionStatusUpdate);
    Device->OnIsConnectedUpdate.AddDynamic(this, &UBS_DevicePair::_OnDeviceIsConnectedUpdate);

    Device->OnAcceleration.AddDynamic(this, &UBS_DevicePair::OnDeviceAccelerationUpdate);
    Device->OnGravity.AddDynamic(this, &UBS_DevicePair::OnDeviceGravityUpdate);
    Device->OnLinearAcceleration.AddDynamic(this, &UBS_DevicePair::OnDeviceLinearAccelerationUpdate);
    Device->OnGyroscope.AddDynamic(this, &UBS_DevicePair::OnDeviceGyroscopeUpdate);
    Device->OnMagnetometer.AddDynamic(this, &UBS_DevicePair::OnDeviceMagnetometerUpdate);
    Device->OnGameRotation.AddDynamic(this, &UBS_DevicePair::OnDeviceGameRotationUpdate);
    Device->OnRotation.AddDynamic(this, &UBS_DevicePair::OnDeviceRotationUpdate);

    Device->OnOrientation.AddDynamic(this, &UBS_DevicePair::OnDeviceOrientationUpdate);
    Device->OnActivity.AddDynamic(this, &UBS_DevicePair::OnDeviceActivityUpdate);
    Device->OnStepCount.AddDynamic(this, &UBS_DevicePair::OnDeviceStepCountUpdate);
    Device->OnStepDetection.AddDynamic(this, &UBS_DevicePair::OnDeviceStepDetectionUpdate);
    Device->OnDeviceOrientation.AddDynamic(this, &UBS_DevicePair::OnDeviceDeviceOrientationUpdate);

    Device->OnBarometer.AddDynamic(this, &UBS_DevicePair::OnDeviceBarometerUpdate);

    Device->OnPressure.AddDynamic(this, &UBS_DevicePair::OnDevicePressureUpdate);
}
void UBS_DevicePair::RemoveDeviceListeners(UBS_Device *Device)
{
    if (!VerifyDevice(Device))
    {
        return;
    }
    UE_LOGFMT(LogBS_DevicePair, Log, "RemoveDeviceListeners from \"{0}\"", Device->Name());

    Device->OnConnectionStatusUpdate.RemoveDynamic(this, &UBS_DevicePair::_OnDeviceConnectionStatusUpdate);
    Device->OnIsConnectedUpdate.RemoveDynamic(this, &UBS_DevicePair::_OnDeviceIsConnectedUpdate);

    Device->OnAcceleration.AddDynamic(this, &UBS_DevicePair::OnDeviceAccelerationUpdate);
    Device->OnGravity.AddDynamic(this, &UBS_DevicePair::OnDeviceGravityUpdate);
    Device->OnLinearAcceleration.AddDynamic(this, &UBS_DevicePair::OnDeviceLinearAccelerationUpdate);
    Device->OnGyroscope.AddDynamic(this, &UBS_DevicePair::OnDeviceGyroscopeUpdate);
    Device->OnMagnetometer.AddDynamic(this, &UBS_DevicePair::OnDeviceMagnetometerUpdate);
    Device->OnGameRotation.AddDynamic(this, &UBS_DevicePair::OnDeviceGameRotationUpdate);
    Device->OnRotation.AddDynamic(this, &UBS_DevicePair::OnDeviceRotationUpdate);

    Device->OnOrientation.AddDynamic(this, &UBS_DevicePair::OnDeviceOrientationUpdate);
    Device->OnActivity.AddDynamic(this, &UBS_DevicePair::OnDeviceActivityUpdate);
    Device->OnStepCount.AddDynamic(this, &UBS_DevicePair::OnDeviceStepCountUpdate);
    Device->OnStepDetection.AddDynamic(this, &UBS_DevicePair::OnDeviceStepDetectionUpdate);
    Device->OnDeviceOrientation.AddDynamic(this, &UBS_DevicePair::OnDeviceDeviceOrientationUpdate);

    Device->OnBarometer.RemoveDynamic(this, &UBS_DevicePair::OnDeviceBarometerUpdate);

    Device->OnPressure.RemoveDynamic(this, &UBS_DevicePair::OnDevicePressureUpdate);
}
// DEVICE LISTENERS END

// SENSOR CONFIGURATION START
void UBS_DevicePair::SetSensorConfiguration(const UBS_SensorConfiguration *NewSensorConfiguration)
{
    for (const TPair<EBS_InsoleSide, UBS_Device *> &Pair : Devices)
    {
        Pair.Value->SetSensorConfiguration(NewSensorConfiguration);
    }
}
void UBS_DevicePair::ClearSensorConfiguration()
{
    for (const TPair<EBS_InsoleSide, UBS_Device *> &Pair : Devices)
    {
        Pair.Value->ClearSensorConfiguration();
    }
}
void UBS_DevicePair::SetSensorRate(EBS_SensorType SensorType, EBS_SensorRate SensorRate, bool &bDidUpdateSensorRate)
{
    for (const TPair<EBS_InsoleSide, UBS_Device *> &Pair : Devices)
    {
        Pair.Value->SetSensorRate(SensorType, SensorRate, bDidUpdateSensorRate);
    }
}
void UBS_DevicePair::SetSensorRates(const TMap<EBS_SensorType, EBS_SensorRate> &SensorRates)
{
    for (const TPair<EBS_InsoleSide, UBS_Device *> &Pair : Devices)
    {
        Pair.Value->SetSensorRates(SensorRates);
    }
}
void UBS_DevicePair::ClearSensorRate(EBS_SensorType SensorType)
{
    for (const TPair<EBS_InsoleSide, UBS_Device *> &Pair : Devices)
    {
        Pair.Value->ClearSensorRate(SensorType);
    }
}
void UBS_DevicePair::ToggleSensorRate(EBS_SensorType SensorType, EBS_SensorRate SensorRate, EBS_SensorRate &UpdatedSensorRate)
{
    for (const TPair<EBS_InsoleSide, UBS_Device *> &Pair : Devices)
    {
        Pair.Value->ToggleSensorRate(SensorType, SensorRate, UpdatedSensorRate);
    }
}

// SENSOR CONFIGURATION END

// PRESSURE START

// PRESSURE END