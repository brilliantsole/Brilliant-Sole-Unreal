// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BS_PressureSensorData.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_PressureSensorData, Log, All);

USTRUCT(BlueprintType)
struct FBS_PressureSensorData
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, Category = "BS Pressure Sensor Data")
    FVector2D Position;

    UPROPERTY(BlueprintReadOnly, Category = "BS Pressure Sensor Data")
    int32 RawValue;

    UPROPERTY(BlueprintReadOnly, Category = "BS Pressure Sensor Data")
    float ScaledValue;

    UPROPERTY(BlueprintReadOnly, Category = "BS Pressure Sensor Data")
    float NormalizedValue;

    UPROPERTY(BlueprintReadOnly, Category = "BS Pressure Sensor Data")
    float WeightedValue;

public:
    FString ToString() const
    {
        return FString::Printf(TEXT("Position: (%f, %f), RawValue: %d, ScaledValue: %f, NormalizedValue: %f, WeightedValue: %f"),
                               Position.X, Position.Y, RawValue, ScaledValue, NormalizedValue, WeightedValue);
    }
};