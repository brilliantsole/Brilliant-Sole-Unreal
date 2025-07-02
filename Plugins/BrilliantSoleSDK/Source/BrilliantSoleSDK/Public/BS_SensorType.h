// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#define EBS_SensorTypes PRESSURE,            \
                        ACCELERATION,        \
                        GRAVITY,             \
                        LINEAR_ACCELERATION, \
                        GYROSCOPE,           \
                        MAGNETOMETER,        \
                        GAME_ROTATION,       \
                        ROTATION,            \
                        ORIENTATION,         \
                        ACTIVITY,            \
                        STEP_COUNT,          \
                        STEP_DETECTION,      \
                        DEVICE_ORIENTATION,  \
                        TAP_DETECTOR,        \
                        BAROMETER,           \
                        CAMERA,              \
                        MICROPHONE

UENUM(BlueprintType)
enum class EBS_SensorType : uint8
{
    PRESSURE UMETA(DisplayName = "Pressure"),

    ACCELERATION UMETA(DisplayName = "Acceleration"),
    GRAVITY UMETA(DisplayName = "Gravity"),
    LINEAR_ACCELERATION UMETA(DisplayName = "Linear Acceleration"),
    GYROSCOPE UMETA(DisplayName = "Gyroscope"),
    MAGNETOMETER UMETA(DisplayName = "Magnetometer"),
    GAME_ROTATION UMETA(DisplayName = "Game Rotation"),
    ROTATION UMETA(DisplayName = "Rotation"),

    ORIENTATION UMETA(DisplayName = "Orientation"),
    ACTIVITY UMETA(DisplayName = "Activity"),
    STEP_COUNT UMETA(DisplayName = "Step Count"),
    STEP_DETECTION UMETA(DisplayName = "Step Detection"),
    DEVICE_ORIENTATION UMETA(DisplayName = "Device Orientation"),
    TAP_DETECTOR UMETA(DisplayName = "Tap Detector"),

    BAROMETER UMETA(DisplayName = "Barometer"),

    CAMERA UMETA(DisplayName = "Camera"),
    MICROPHONE UMETA(DisplayName = "Camera"),

    COUNT UMETA(Hidden)
};

ENUM_RANGE_BY_COUNT(EBS_SensorType, static_cast<int32>(EBS_SensorType::COUNT))