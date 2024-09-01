// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_VibrationConfiguration.h"
#include "Logging/StructuredLog.h"
#include "Math/UnrealMathUtility.h"

DEFINE_LOG_CATEGORY(LogBS_VibrationConfiguration);

const TArray<uint8> FBS_VibrationConfiguration::ToArray() const
{
    TArray<uint8> Array;

    Array.Add(VibrationLocationsBitmask);

    Array.Add(static_cast<uint8>(VibrationType));

    const TArray<uint8> VibrationData = GetVibrationData();
    const uint8 VibrationDataLength = VibrationData.Num();
    if (VibrationDataLength == 0)
    {
        UE_LOGFMT(LogBS_VibrationConfiguration, Warning, "No Vibration Data - skipping");
        return {};
    }
    Array.Add(static_cast<uint8>(VibrationDataLength));
    Array.Append(VibrationData);

    return Array;
}

const TArray<uint8> FBS_VibrationConfiguration::GetVibrationData() const
{
    switch (VibrationType)
    {
    case EBS_VibrationType::WAVEFORM_EFFECT:
        return WaveformEffectSequenceToArray();
    case EBS_VibrationType::WAVEFORM:
        return WaveformSequenceToArray();
    default:
        UE_LOGFMT(LogBS_VibrationConfiguration, Error, "Uncaught VibrationType {0}", UEnum::GetValueAsString(VibrationType));
        return {};
    }
}

const TArray<uint8> FBS_VibrationConfiguration::WaveformEffectSequenceToArray() const
{
    const uint8 NumberOfSegments = FMath::Min(static_cast<uint8>(WaveformEffectSequence.Num()), MaxNumberOfWaveformEffectSegments);
    if (NumberOfSegments == 0)
    {
        UE_LOGFMT(LogBS_VibrationConfiguration, Warning, "No WaveformEffect Segments - skipping");
        return {};
    }

    TArray<uint8> Array;

    bool HasAtLeast1WaveformEffectWithANonzeroLoopCount = false;
    for (const FBS_VibrationWaveformEffectSegment &Segment : WaveformEffectSequence)
    {
        if (Segment.LoopCount > 0)
        {
            HasAtLeast1WaveformEffectWithANonzeroLoopCount = true;
            break;
        }
    }

    const bool IncludeAllWaveformEffectSegments = HasAtLeast1WaveformEffectWithANonzeroLoopCount || WaveformEffectSequenceLoopCount != 0;

    for (uint8 Index = 0; Index < NumberOfSegments || (IncludeAllWaveformEffectSegments && Index < MaxNumberOfWaveformEffectSegments); Index++)
    {
        const FBS_VibrationWaveformEffectSegment &Segment = WaveformEffectSequence.IsValidIndex(Index) ? WaveformEffectSequence[Index] : NoneWaveformEffectSegment;
        switch (Segment.Type)
        {
        case EBS_VibrationWaveformEffectSegmentType::Effect:
            UE_LOGFMT(LogBS_VibrationConfiguration, Log, "Effect #{0}: {1}", Index, UEnum::GetValueAsString(Segment.Effect));
            Array.Add(static_cast<uint8>(Segment.Effect));
            break;
        case EBS_VibrationWaveformEffectSegmentType::Delay:
            UE_LOGFMT(LogBS_VibrationConfiguration, Log, "Delay #{0}: {1}ms", Index, Segment.Delay);
            Array.Add(static_cast<uint8>((1 << 7) | (Segment.Delay / 10)));
            break;
        default:
            UE_LOGFMT(LogBS_VibrationConfiguration, Error, "Uncaught WaveformEffect Segment Type {0}", UEnum::GetValueAsString(Segment.Type));
            break;
        }
    }

    const uint8 IncludeAllWaveformEffectSegmentLoopCounts = WaveformEffectSequenceLoopCount != 0;

    for (uint8 Index = 0; Index < NumberOfSegments || (IncludeAllWaveformEffectSegments && Index < MaxNumberOfWaveformEffectSegments); Index++)
    {
        const uint8 SegmentLoopCount = WaveformEffectSequence.IsValidIndex(Index) ? WaveformEffectSequence[Index].LoopCount : 0;
        if (Index == 0 || Index == 4)
        {
            Array.Add(0);
        }
        const uint8 BitOffset = 2 * (Index % 4);
        Array[Array.Num() - 1] |= SegmentLoopCount << BitOffset;
        if (Index == 3 || Index == 7)
        {
        }
    }

    if (WaveformEffectSequenceLoopCount != 0)
    {
        Array.Add(WaveformEffectSequenceLoopCount);
    }

    return Array;
}

const TArray<uint8> FBS_VibrationConfiguration::WaveformSequenceToArray() const
{
    const uint8 NumberOfSegments = FMath::Min(static_cast<uint8>(WaveformSequence.Num()), MaxNumberOfWaveformSegments);
    if (NumberOfSegments == 0)
    {
        UE_LOGFMT(LogBS_VibrationConfiguration, Warning, "No Waveform Segments - skipping");
        return {};
    }

    TArray<uint8> Array;
    for (uint Index = 0; Index < NumberOfSegments; Index++)
    {
        const FBS_VibrationWaveformSegment &Segment = WaveformSequence[Index];

        Array.Add(FMath::FloorToInt((float)Segment.Amplitude * 127.0f));
        Array.Add(FMath::FloorToInt((float)Segment.Duration / 10.0f));
    }
    return Array;
}