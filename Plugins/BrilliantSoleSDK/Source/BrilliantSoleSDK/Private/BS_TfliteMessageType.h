// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#define EBS_TfliteMessages GET_TFLITE_NAME,                \
						   SET_TFLITE_NAME,                \
						   GET_TFLITE_TASK,                \
						   SET_TFLITE_TASK,                \
						   GET_TFLITE_SAMPLE_RATE,         \
						   SET_TFLITE_SAMPLE_RATE,         \
						   GET_TFLITE_SENSOR_TYPES,        \
						   SET_TFLITE_SENSOR_TYPES,        \
						   IS_TFLITE_READY,                \
						   GET_TFLITE_CAPTURE_DELAY,       \
						   SET_TFLITE_CAPTURE_DELAY,       \
						   GET_TFLITE_THRESHOLD,           \
						   SET_TFLITE_THRESHOLD,           \
						   GET_TFLITE_INFERENCING_ENABLED, \
						   SET_TFLITE_INFERENCING_ENABLED, \
						   TFLITE_INFERENCE

enum class EBS_TfliteMessage : uint8
{
	EBS_TfliteMessages,
	COUNT
};

ENUM_RANGE_BY_COUNT(EBS_TfliteMessage, static_cast<int32>(EBS_TfliteMessage::COUNT))
