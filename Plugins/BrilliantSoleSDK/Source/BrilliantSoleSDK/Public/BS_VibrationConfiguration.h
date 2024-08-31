// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BS_VibrationType.h"
#include "BS_VibrationLocationBitFlag.h"
#include "BS_VibrationWaveformSegment.h"
#include "BS_VibrationWaveformEffectSegment.h"
#include "BS_VibrationConfiguration.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_VibrationConfiguration, Log, All);

USTRUCT(BlueprintType, meta = (ShowOnlyInnerProperties, FullyExpand = "true"))
struct FBS_VibrationConfiguration
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BS Vibration Configuration")
    EBS_VibrationType VibrationType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BS Vibration Configuration", meta = (Bitmask, BitmaskEnum = "/Script/BrilliantSoleSDK.EBS_VibrationLocationBitFlag"))
    uint8 VibrationLocations = 0;

    // WAVEFORM EFFECT START
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BS Vibration Waveform Effect Configuration", meta = (EditCondition = "VibrationType==EBS_VibrationType::WAVEFORM_EFFECT", EditConditionHides, ToolTip = "Max 8 Segments"))
    TArray<FBS_VibrationWaveformEffectSegment> WaveformEffectSegments;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BS Vibration Waveform Effect Configuration", meta = (EditCondition = "VibrationType==EBS_VibrationType::WAVEFORM_EFFECT", DisplayName = "Loop Count", EditConditionHides, ClampMin = "0", ClampMax = "6", UIMin = "0", UIMax = "6"))
    uint8 WaveformEffectLoopCount;

private:
    static const uint8 VibrationWaveformEffectConfigurationMaxSegments = 8;
    static const uint8 VibrationWaveformEffectConfigurationMaxLoopCount = 6;

    const TArray<uint8> WaveformEffectSegmentsToArray() const;
    // WAVEFORM EFFECT END

    // WAVEFORM START
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BS Vibration Waveform Configuration", meta = (EditCondition = "VibrationType==EBS_VibrationType::WAVEFORM", EditConditionHides, ToolTip = "Max 20 Segments"))
    TArray<FBS_VibrationWaveformSegment> WaveformSegments;

private:
    static const uint8 VibrationWaveformConfigurationMaxWaveformSegments = 20;

    const TArray<uint8> WaveformSegmentsToArray() const;
    // WAVEFORM END

public:
    const TArray<uint8> ToArray() const;

private:
    const TArray<uint8> GetVibrationData() const;
};