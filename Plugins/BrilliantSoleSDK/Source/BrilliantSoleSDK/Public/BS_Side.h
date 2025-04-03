// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBS_Side : uint8
{
    LEFT UMETA(DisplayName = "Left"),
    RIGHT UMETA(DisplayName = "Right"),
    COUNT UMETA(Hidden)
};

ENUM_RANGE_BY_COUNT(EBS_Side, static_cast<int32>(EBS_Side::COUNT))