// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BS_InsoleSide.h"
#include "BS_PressureData.h"
#include "BS_DevicePairPressureData.generated.h"

USTRUCT(BlueprintType, meta = (ShowOnlyInnerProperties, FullyExpand = "true"))
struct FBS_DevicePairPressureData
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, Category = "BS Device Pair Pressure Data")
    TArray<FBS_PressureSensorData> LeftSensors;

    UPROPERTY(BlueprintReadOnly, Category = "BS Device Pair Pressure Data")
    TArray<FBS_PressureSensorData> RightSensors;

    UPROPERTY(BlueprintReadOnly, Category = "BS Device Pair Pressure Data")
    float ScaledSum;

    UPROPERTY(BlueprintReadOnly, Category = "BS Device Pair Pressure Data")
    float NormalizedSum;

    UPROPERTY(BlueprintReadOnly, Category = "BS Device Pair Pressure Data")
    FVector2D CenterOfPressure;

    UPROPERTY(BlueprintReadOnly, Category = "BS Device Pair Pressure Data")
    FVector2D NormalizedCenterOfPressure;
};