// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

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
    STEP_COUNTER UMETA(DisplayName = "Step Counter"),
    STEP_DETECTOR UMETA(DisplayName = "Step Detector"),
    DEVICE_ORIENTATION UMETA(DisplayName = "Device Orientation"),

    BAROMETER UMETA(DisplayName = "Barometer"),

    COUNT UMETA(Hidden)
};