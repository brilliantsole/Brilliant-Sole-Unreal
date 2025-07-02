// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BS_Range.h"
#include "BS_CenterOfPressureRange.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_CenterOfPressureRange, Warning, All);

USTRUCT()
struct FBS_CenterOfPressureRange
{
    GENERATED_BODY()

private:
    FBS_Range X;
    FBS_Range Y;

public:
    void Reset()
    {
        X.Reset();
        Y.Reset();
    }

    void Update(FVector2D Value)
    {
        X.Update(Value.X);
        Y.Update(Value.Y);
        UE_LOGFMT(LogTemp, Verbose, "Updated CenterOfPressureRange to {0}", *ToString());
    }

    FVector2D GetNormalization(FVector2D Value)
    {
        return {X.GetNormalization(Value.X, false), Y.GetNormalization(Value.Y, false)};
    }

    FVector2D UpdateAndGetNormalization(FVector2D Value)
    {
        Update(Value);
        return GetNormalization(Value);
    }

    FString ToString() const
    {
        return FString::Printf(TEXT("X: %s, Y: %s"),
                               *X.ToString(), *Y.ToString());
    }
};