// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBS_DeviceOrientation : uint8
{
    PORTRAIT_UPRIGHT UMETA(DisplayName = "Portrait Upright"),
    LANDSCAPE_LEFT UMETA(DisplayName = "Landscape Left"),
    PORTRAIT_UPSIDE_DOWN UMETA(DisplayName = "Portrait Upside Down"),
    LANDSCAPE_RIGHT UMETA(DisplayName = "Landscape Right"),
    UNKNWON UMETA(DisplayName = "Unknown"),
    COUNT UMETA(Hidden)
};

ENUM_RANGE_BY_COUNT(EBS_DeviceOrientation, static_cast<int32>(EBS_DeviceOrientation::COUNT))