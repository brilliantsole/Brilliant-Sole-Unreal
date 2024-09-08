// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBS_DeviceInformationType : uint8
{
    NONE UMETA(DisplayName = "None"),

    MANUFACTURER_NAME UMETA(DisplayName = "Manufacturer Name"),
    MODEL_NUMBER UMETA(DisplayName = "Model Number"),
    SERIAL_NUMBER UMETA(DisplayName = "Serial Number"),
    HARDWARE_REVISION UMETA(DisplayName = "Hardware Revision"),
    FIRMWARE_REVISION UMETA(DisplayName = "Firmware Revision"),
    SOFTWARE_REVISION UMETA(DisplayName = "Software Revision"),

    COUNT UMETA(Hidden)
};

ENUM_RANGE_BY_COUNT(EBS_DeviceInformationType, static_cast<int32>(EBS_DeviceInformationType::COUNT))