// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BS_DevicePairPressureData.generated.h"

USTRUCT(BlueprintType, meta = (ShowOnlyInnerProperties, FullyExpand = "true"))
struct FBS_DevicePairPressureData
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, Category = "BS Device Pair Pressure Data")
    float RawSum;

    UPROPERTY(BlueprintReadOnly, Category = "BS Device Pair Pressure Data")
    float NormalizedSum;

    UPROPERTY(BlueprintReadOnly, Category = "BS Device Pair Pressure Data")
    FVector2D CenterOfPressure;

    UPROPERTY(BlueprintReadOnly, Category = "BS Device Pair Pressure Data")
    FVector2D NormalizedCenterOfPressure;
};