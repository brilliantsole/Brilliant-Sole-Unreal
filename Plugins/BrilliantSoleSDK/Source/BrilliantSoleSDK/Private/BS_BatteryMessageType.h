// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#define EBS_BatteryMessages IS_BATTERY_CHARGING, \
							GET_BATTERY_CURRENT

enum class EBS_BatteryMessage : uint8
{
	EBS_BatteryMessages,
	COUNT
};

ENUM_RANGE_BY_COUNT(EBS_BatteryMessage, static_cast<int32>(EBS_BatteryMessage::COUNT))