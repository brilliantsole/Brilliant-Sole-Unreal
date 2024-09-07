// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBS_BleCharacteristic : uint8
{
    NONE UMETA(Hidden),

    BATTERY_LEVEL UMETA(DisplayName = "Battery Level"),

    MANUFACTURER_NAME_STRING UMETA(DisplayName = "Manufacturer Name String"),
    MODEL_NUMBER_STRING UMETA(DisplayName = "Model Number String"),
    SERIAL_NUMBER_STRING UMETA(DisplayName = "Serial Number String"),
    HARDWARE_REVISION_STRING UMETA(DisplayName = "Hardware Revision String"),
    FIRMWARE_REVISION_STRING UMETA(DisplayName = "Firmware Revision String"),
    SOFTWARE_REVISION_STRING UMETA(DisplayName = "Software Revision String"),

    RX UMETA(DisplayName = "Rx"),
    TX UMETA(DisplayName = "Tx"),

    COUNT UMETA(Hidden)
};

ENUM_RANGE_BY_COUNT(EBS_BleCharacteristic, static_cast<int32>(EBS_BleCharacteristic::COUNT))