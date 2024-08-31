// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_VibrationConfiguration.h"
#include "Logging/StructuredLog.h"
#include "BS_ByteParser.h"

DEFINE_LOG_CATEGORY(LogBS_VibrationConfiguration);

const TArray<uint8> FBS_VibrationConfiguration::ToArray() const
{
    TArray<uint8> Array;
    Array.Add(VibrationLocations);
    Array.Add(static_cast<uint8>(VibrationType));
    Array.Append(GetVibrationData());
    return Array;
}

const TArray<uint8> FBS_VibrationConfiguration::GetVibrationData() const
{
    // FILL
    return {};
}

const TArray<uint8> FBS_VibrationConfiguration::WaveformEffectSequenceToArray() const
{
    // FILL
    return {};
}

const TArray<uint8> FBS_VibrationConfiguration::WaveformSequenceToArray() const
{
    // FILL
    return {};
}