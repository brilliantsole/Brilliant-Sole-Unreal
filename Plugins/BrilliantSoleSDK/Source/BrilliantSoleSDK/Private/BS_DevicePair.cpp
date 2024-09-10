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

// IS SHARED START
void UBS_DevicePair::SetIsShared(bool bNewIsShared)
{
    if (bIsShared == bNewIsShared)
    {
        UE_LOGFMT(LogBS_DevicePair, Log, "redundant bIsShared assignment - skipping");
        return;
    }

    bIsShared = bNewIsShared;
    UE_LOGFMT(LogBS_DevicePair, Log, "Updated bIsShared to {0}", bIsShared);
    OnIsSharedUpdate(bIsShared);
}
// IS SHARED END

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

    UE_LOGFMT(LogBS_DevicePair, Log, "Removing Device \"{0}\"", Device->GetName());
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
    UE_LOGFMT(LogBS_DevicePair, Log, "AddDeviceListeners to \"{0}\"", Device->GetName());
    Device->OnPressure.AddDynamic(this, &UBS_DevicePair::OnDevicePressure);
}
void UBS_DevicePair::RemoveDeviceListeners(UBS_Device *Device)
{
    if (!VerifyDevice(Device))
    {
        return;
    }
    UE_LOGFMT(LogBS_DevicePair, Log, "RemoveDeviceListeners from \"{0}\"", Device->GetName());
    Device->OnPressure.RemoveDynamic(this, &UBS_DevicePair::OnDevicePressure);
}
// DEVICE LISTENERS END

// PRESSURE START
void UBS_DevicePair::OnDevicePressure(UBS_Device *Device, const FBS_PressureData &PressureData, const float &Timestamp)
{
    // FILL
}
// PRESSURE END