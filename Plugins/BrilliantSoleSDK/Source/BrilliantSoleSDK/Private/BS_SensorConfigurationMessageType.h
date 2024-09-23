// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#define EBS_SensorConfigurationMessages GET_SENSOR_CONFIGURATION, \
										SET_SENSOR_CONFIGURATION

enum class EBS_SensorConfigurationMessage : uint8
{
	EBS_SensorConfigurationMessages,
	COUNT
};

ENUM_RANGE_BY_COUNT(EBS_SensorConfigurationMessage, static_cast<int32>(EBS_SensorConfigurationMessage::COUNT))