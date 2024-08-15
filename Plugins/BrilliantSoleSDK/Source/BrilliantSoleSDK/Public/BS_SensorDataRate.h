// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBS_SensorDataRate : uint8
{
    Value0 = 0 UMETA(DisplayName = "0"),
    Value10 = 1 UMETA(DisplayName = "10"),
    Value20 = 2 UMETA(DisplayName = "20"),
    Value40 = 4 UMETA(DisplayName = "40"),
    Value60 = 6 UMETA(DisplayName = "60"),
    Value80 = 8 UMETA(DisplayName = "80"),
    Value100 = 10 UMETA(DisplayName = "100")
};