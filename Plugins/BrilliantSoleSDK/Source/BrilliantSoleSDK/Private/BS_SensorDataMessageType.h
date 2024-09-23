// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BS_SensorType.h"

#define EBS_SensorDataMessages GET_PRESSURE_POSITIONS, \
                               GET_SENSOR_SCALARS,     \
                               SENSOR_DATA

enum class EBS_SensorDataMessage : uint8
{
    EBS_SensorDataMessages,
    COUNT
};

ENUM_RANGE_BY_COUNT(EBS_SensorDataMessage, static_cast<int32>(EBS_SensorDataMessage::COUNT))

#define EBS_SensorDataEvents EBS_SensorDataMessages, \
                             EBS_SensorTypes

enum class EBS_SensorDataEvent : uint8
{
    EBS_SensorDataEvents,
    COUNT
};

ENUM_RANGE_BY_COUNT(EBS_SensorDataEvent, static_cast<int32>(EBS_SensorDataEvent::COUNT))