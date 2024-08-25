// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBS_SensorRate : uint8
{
    Value0 = 0 UMETA(DisplayName = "0ms"),
    Value10 = 1 UMETA(DisplayName = "10ms"),
    Value20 = 2 UMETA(DisplayName = "20ms"),
    Value40 = 4 UMETA(DisplayName = "40ms"),
    Value60 = 6 UMETA(DisplayName = "60ms"),
    Value80 = 8 UMETA(DisplayName = "80ms"),
    Value100 = 10 UMETA(DisplayName = "100ms"),
    COUNT UMETA(Hidden)
};