// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBS_Activity : uint8
{
    STILL UMETA(DisplayName = "Still"),
    WALKING UMETA(DisplayName = "Walking"),
    RUNNING UMETA(DisplayName = "Running"),
    BICYCLE UMETA(DisplayName = "Bicycle"),
    VEHICLE UMETA(DisplayName = "Vehicle"),
    TILTING UMETA(DisplayName = "Tilting"),
    COUNT UMETA(Hidden)
};

ENUM_RANGE_BY_COUNT(EBS_Activity, static_cast<int32>(EBS_Activity::COUNT))