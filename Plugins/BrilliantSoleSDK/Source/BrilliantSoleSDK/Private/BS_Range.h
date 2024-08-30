// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Logging/StructuredLog.h"
#include "BS_Range.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_Range, Log, All);

USTRUCT()
struct FBS_Range
{
    GENERATED_BODY()

private:
    float Min;
    float Max;
    float Span;

public:
    FBS_Range()
        : Min(FLT_MAX),
          Max(-FLT_MAX)
    {
    }

    void Reset()
    {
        Min = FLT_MAX;
        Max = -FLT_MAX;
        Span = 0;
    }

    void Update(float Value)
    {
        Min = FMath::Min(Min, Value);
        Max = FMath::Max(Max, Value);
        Span = Max - Min;
        UE_LOGFMT(LogBS_Range, Log, "Updated Range to {0}", ToString());
    }

    float GetNormalization(float Value)
    {
        if (Span == 0)
        {
            return 0.0f;
        }
        float Interpolation = (Value - Min) / Span;
        return Interpolation * Span;
    }

    float UpdateAndGetNormalization(float Value)
    {
        Update(Value);
        return GetNormalization(Value);
    }

    FString ToString() const
    {
        return FString::Printf(TEXT("Min: %f, Max: %f, Span: %f"),
                               Min, Max, Span);
    }
};