// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_DeviceInformation.h"
#include "Logging/StructuredLog.h"
#include "BS_ByteParser.h"

DEFINE_LOG_CATEGORY(LogBS_DeviceInformation);

void FBS_DeviceInformation::UpdateDidGetAllInformation()
{
    bool bNewDidGetAllInformation = true;
    bNewDidGetAllInformation = bNewDidGetAllInformation && ManufacturerName != "";
    bNewDidGetAllInformation = bNewDidGetAllInformation && ModelNumber != "";
    bNewDidGetAllInformation = bNewDidGetAllInformation && SerialNumber != "";
    bNewDidGetAllInformation = bNewDidGetAllInformation && HardwareRevision != "";
    bNewDidGetAllInformation = bNewDidGetAllInformation && FirmwareRevision != "";
    bNewDidGetAllInformation = bNewDidGetAllInformation && SoftwareRevision != "";

    UE_LOGFMT(LogBS_DeviceInformation, Log, "bNewDidGetAllInformation: {0}", bNewDidGetAllInformation);

    if (bDidGetAllInformation == bNewDidGetAllInformation)
    {
        return;
    }

    bDidGetAllInformation = bNewDidGetAllInformation;
    UE_LOGFMT(LogBS_DeviceInformation, Log, "Updated bDidGetAllInformation to {0}", bDidGetAllInformation);

    if (bDidGetAllInformation)
    {
        UE_LOGFMT(LogBS_DeviceInformation, Log, "Got all Information");
    }
}

void FBS_DeviceInformation::Reset()
{
    {
        bDidGetAllInformation = false;

        ManufacturerName = "";
        ModelNumber = "";
        SerialNumber = "";
        HardwareRevision = "";
        FirmwareRevision = "";
        SoftwareRevision = "";
    }
}

void FBS_DeviceInformation::SetValue(const EBS_DeviceInformationType DeviceInformationType, const TArray<uint8> &Value)
{
    const FString String = BS_ByteParser::GetString(Value);

    UE_LOGFMT(LogBS_DeviceInformation, Log, "Setting {0} to \"{1}\"", UEnum::GetValueAsString(DeviceInformationType), String);

    switch (DeviceInformationType)
    {
    case EBS_DeviceInformationType::MANUFACTURER_NAME:
        ManufacturerName = String;
        break;
    case EBS_DeviceInformationType::MODEL_NUMBER:
        ModelNumber = String;
        break;
    case EBS_DeviceInformationType::SERIAL_NUMBER:
        SerialNumber = String;
        break;
    case EBS_DeviceInformationType::HARDWARE_REVISION:
        HardwareRevision = String;
        break;
    case EBS_DeviceInformationType::FIRMWARE_REVISION:
        FirmwareRevision = String;
        break;
    case EBS_DeviceInformationType::SOFTWARE_REVISION:
        SoftwareRevision = String;
        break;
    default:
        UE_LOGFMT(LogBS_DeviceInformation, Error, "Uncaught DeviceInformationType {0}", UEnum::GetValueAsString(DeviceInformationType));
        break;
    }

    UpdateDidGetAllInformation();
}