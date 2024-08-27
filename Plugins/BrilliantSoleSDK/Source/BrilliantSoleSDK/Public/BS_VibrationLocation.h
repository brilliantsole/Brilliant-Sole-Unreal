// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBS_VibrationLocation : uint8
{
    FRONT UMETA(DisplayName = "Front"),
    REAR UMETA(DisplayName = "Rear"),
    COUNT UMETA(Hidden)
};

ENUM_RANGE_BY_COUNT(EBS_VibrationLocation, static_cast<int32>(EBS_VibrationLocation::COUNT))