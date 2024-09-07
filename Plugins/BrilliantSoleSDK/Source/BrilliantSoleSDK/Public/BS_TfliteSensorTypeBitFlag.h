// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EBS_TfliteSensorTypeBitFlag : uint8
{
    NONE = 0 UMETA(Hidden),
    PRESSURE = 1 << 0 UMETA(DisplayName = "Pressure"),
    LINEAR_ACCELERATION = 1 << 1 UMETA(DisplayName = "Linear Acceleration"),
    GYROSCOPE = 1 << 2 UMETA(DisplayName = "Gyroscope"),
    MAGNETOMETER = 1 << 3 UMETA(DisplayName = "Magnetometer"),
    COUNT UMETA(Hidden)
};
ENUM_CLASS_FLAGS(EBS_TfliteSensorTypeBitFlag);

ENUM_RANGE_BY_COUNT(EBS_TfliteSensorTypeBitFlag, static_cast<int32>(EBS_TfliteSensorTypeBitFlag::COUNT))