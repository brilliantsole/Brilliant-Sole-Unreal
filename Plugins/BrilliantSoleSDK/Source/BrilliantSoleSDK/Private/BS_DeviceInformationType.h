// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#define EBS_DeviceInformationMessages MANUFACTURER_NAME, \
                                      MODEL_NUMBER,      \
                                      SOFTWARE_REVISION, \
                                      HARDWARE_REVISION, \
                                      FIRMWARE_REVISION, \
                                      PNP_ID,            \
                                      SERIAL_NUMBER

UENUM(BlueprintType)
enum class EBS_DeviceInformation : uint8
{
    MANUFACTURER_NAME UMETA(DisplayName = "Manufacturer Name"),
    MODEL_NUMBER UMETA(DisplayName = "Model Number"),
    SOFTWARE_REVISION UMETA(DisplayName = "Software Revision"),
    HARDWARE_REVISION UMETA(DisplayName = "Hardware Revision"),
    FIRMWARE_REVISION UMETA(DisplayName = "Firmware Revision"),
    SERIAL_NUMBER UMETA(DisplayName = "Serial Number"),
    COUNT UMETA(Hidden)
};

ENUM_RANGE_BY_COUNT(EBS_DeviceInformation, static_cast<int32>(EBS_DeviceInformation::COUNT))

#define EBS_DeviceInformationEvents EBS_DeviceInformationMessages, \
                                    DEVICE_INFORMATION

enum class EBS_DeviceInformationEvent : uint8
{
    EBS_DeviceInformationEvents,
    COUNT
};

ENUM_RANGE_BY_COUNT(EBS_DeviceInformationEvent, static_cast<int32>(EBS_DeviceInformationEvent::COUNT))