// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BS_VibrationWaveformEffect.h"
#include "BS_VibrationWaveformEffectSegment.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(BS_VibrationWaveformEffectSegment, Warning, All);

UENUM()
enum class EBS_VibrationWaveformEffectSegmentType
{
    Effect,
    Delay
};

USTRUCT(BlueprintType, meta = (ShowOnlyInnerProperties, FullyExpand = "true"))
struct FBS_VibrationWaveformEffectSegment
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BS Vibration Waveform Effect Segment")
    EBS_VibrationWaveformEffectSegmentType Type = EBS_VibrationWaveformEffectSegmentType::Effect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BS Vibration Waveform Effect Segment", meta = (EditCondition = "Type==EBS_VibrationWaveformEffectSegmentType::Effect", EditConditionHides))
    EBS_VibrationWaveformEffect Effect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BS Vibration Waveform Effect Segment", meta = (DisplayName = "Delay (ms)", EditCondition = "Type==EBS_VibrationWaveformEffectSegmentType::Delay", EditConditionHides, ToolTip = "0 - 1270ms", ClampMin = "0", ClampMax = "1270", UIMin = "0", UIMax = "1270", Delta = 10, Multiple = 10))
    int32 Delay;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BS Vibration Waveform Effect Segment", meta = (ClampMin = "0", ClampMax = "3", UIMin = "0", UIMax = "3"))
    uint8 LoopCount;

private:
    static const uint16 MaxDelay = 1270;
    static const uint8 MaxLoopCount = 3;
};