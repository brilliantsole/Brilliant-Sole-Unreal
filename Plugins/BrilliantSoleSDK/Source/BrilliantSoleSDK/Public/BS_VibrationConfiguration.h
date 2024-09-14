// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BS_VibrationType.h"
#include "BS_VibrationLocationBitFlag.h"
#include "BS_VibrationWaveformSegment.h"
#include "BS_VibrationWaveformEffectSegment.h"
#include "BS_VibrationConfiguration.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_VibrationConfiguration, Verbose, All);

USTRUCT(BlueprintType, meta = (ShowOnlyInnerProperties, FullyExpand = "true"))
struct FBS_VibrationConfiguration
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BS Vibration Configuration")
    EBS_VibrationType VibrationType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BS Vibration Configuration", meta = (DisplayName = "Vibration Locations", Bitmask, BitmaskEnum = "/Script/BrilliantSoleSDK.EBS_VibrationLocationBitFlag"))
    uint8 VibrationLocationsBitmask = 0;

    // WAVEFORM EFFECT START
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BS Vibration Waveform Effect Configuration", meta = (EditCondition = "VibrationType==EBS_VibrationType::WAVEFORM_EFFECT", EditConditionHides, ToolTip = "Max 8 Segments"))
    TArray<FBS_VibrationWaveformEffectSegment> WaveformEffectSequence;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BS Vibration Waveform Effect Configuration", meta = (EditCondition = "VibrationType==EBS_VibrationType::WAVEFORM_EFFECT", DisplayName = "Loop Count", EditConditionHides, ClampMin = "0", ClampMax = "6", UIMin = "0", UIMax = "6"))
    uint8 WaveformEffectSequenceLoopCount;

private:
    static const uint8 MaxNumberOfWaveformEffectSegments = 8;
    static const uint8 MaxWaveformEffectSequenceLoopCount = 6;

    const TArray<uint8> WaveformEffectSequenceToArray() const;

    static constexpr FBS_VibrationWaveformEffectSegment NoneWaveformEffectSegment = {EBS_VibrationWaveformEffectSegmentType::Effect, EBS_VibrationWaveformEffect::NONE, 0, 0};
    // WAVEFORM EFFECT END

    // WAVEFORM START
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BS Vibration Waveform Configuration", meta = (EditCondition = "VibrationType==EBS_VibrationType::WAVEFORM", EditConditionHides, ToolTip = "Max 20 Segments"))
    TArray<FBS_VibrationWaveformSegment> WaveformSequence;

private:
    static const uint8 MaxNumberOfWaveformSegments = 20;

    const TArray<uint8> WaveformSequenceToArray() const;
    // WAVEFORM END

public:
    const TArray<uint8> ToArray() const;

private:
    const TArray<uint8> GetVibrationData() const;
};