// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#define EBS_BatteryLevelMessages BATTERY_LEVEL

enum class EBS_BatteryLevelMessage : uint8
{
    EBS_BatteryLevelMessages,
    COUNT
};

ENUM_RANGE_BY_COUNT(EBS_BatteryLevelMessage, static_cast<int32>(EBS_BatteryLevelMessage::COUNT))