// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#define EBS_InformationMessages IS_BATTERY_CHARGING, \
								GET_BATTERY_CURRENT, \
								GET_MTU,             \
								GET_ID,              \
								GET_NAME,            \
								SET_NAME,            \
								GET_TYPE,            \
								SET_TYPE,            \
								GET_CURRENT_TIME,    \
								SET_CURRENT_TIME

enum class EBS_InformationMessage : uint8
{
	EBS_InformationMessages,
	COUNT
};

ENUM_RANGE_BY_COUNT(EBS_InformationMessage, static_cast<int32>(EBS_InformationMessage::COUNT))