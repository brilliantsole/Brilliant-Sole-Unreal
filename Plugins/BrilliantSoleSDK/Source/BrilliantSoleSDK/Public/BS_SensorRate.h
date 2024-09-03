// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBS_SensorRate : uint8
{
    Value0 = 0 UMETA(DisplayName = "0ms"),
    Value5 = 5 UMETA(DisplayName = "5ms"),
    Value10 = 10 UMETA(DisplayName = "10ms"),
    Value20 = 20 UMETA(DisplayName = "20ms"),
    Value40 = 40 UMETA(DisplayName = "40ms"),
    Value60 = 60 UMETA(DisplayName = "60ms"),
    Value80 = 80 UMETA(DisplayName = "80ms"),
    Value100 = 100 UMETA(DisplayName = "100ms"),
    COUNT UMETA(Hidden)
};

ENUM_RANGE_BY_COUNT(EBS_SensorRate, static_cast<int32>(EBS_SensorRate::COUNT))