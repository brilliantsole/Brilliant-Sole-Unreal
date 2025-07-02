// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BS_VibrationWaveformSegment.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(BS_VibrationWaveformSegment, Warning, All);

USTRUCT(BlueprintType, meta = (ShowOnlyInnerProperties, FullyExpand = "true"))
struct FBS_VibrationWaveformSegment
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BS Vibration Waveform Segment", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
    float Amplitude;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BS Vibration Waveform Segment", meta = (DisplayName = "Duration (ms)", ToolTip = "0 - 2550ms", ClampMin = "0", ClampMax = "2550", UIMin = "0", UIMax = "2550", Delta = 10, Multiple = 10))
    int32 Duration;

private:
    static const uint16 MaxDuration = 2550;
};