// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BS_PressureSensorData.h"
#include "BS_PressureData.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_PressureData, Log, All);

USTRUCT(BlueprintType, meta = (ShowOnlyInnerProperties, FullyExpand = "true"))
struct FBS_PressureData
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, Category = "BS Pressure Data")
    TArray<FBS_PressureSensorData> Sensors;

    UPROPERTY(BlueprintReadOnly, Category = "BS Pressure Data")
    float ScaledSum;

    UPROPERTY(BlueprintReadOnly, Category = "BS Pressure Data")
    float NormalizedSum;

    UPROPERTY(BlueprintReadOnly, Category = "BS Pressure Data")
    FVector2D CenterOfPressure;

    UPROPERTY(BlueprintReadOnly, Category = "BS Pressure Data")
    FVector2D NormalizedCenterOfPressure;
};