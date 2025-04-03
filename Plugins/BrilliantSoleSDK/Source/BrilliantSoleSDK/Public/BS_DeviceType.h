// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBS_DeviceType : uint8
{
    LEFT_INSOLE UMETA(DisplayName = "Left Insole"),
    RIGHT_INSOLE UMETA(DisplayName = "Right Insole"),
    LEFT_GLOVE UMETA(DisplayName = "Left Glove"),
    RIGHT_GLOVE UMETA(DisplayName = "Right Glove"),
    GLASSES UMETA(DisplayName = "Glasses"),
    GENERIC UMETA(DisplayName = "Generic"),

    COUNT UMETA(Hidden)
};

ENUM_RANGE_BY_COUNT(EBS_DeviceType, static_cast<int32>(EBS_DeviceType::COUNT))