// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_BaseVibrationConfiguration.h"
#include "Logging/StructuredLog.h"
#include "BS_ByteParser.h"

DEFINE_LOG_CATEGORY(LogBS_BaseVibrationConfiguration);

const TArray<uint8> UBS_BaseVibrationConfiguration::ToArray() const
{
    TArray<uint8> Array;
    Array.Add(GetVibrationLocationsBitmask());
    Array.Add(static_cast<uint8>(GetVibrationType()));
    Array.Append(GetVibrationData());
    return Array;
}

const uint8 UBS_BaseVibrationConfiguration::GetVibrationLocationsBitmask() const
{
    uint8 VibrationLocationsBitmask = 0;
    for (const EBS_VibrationLocation VibrationLocation : VibrationLocations)
    {
        VibrationLocationsBitmask |= 1 << static_cast<uint8>(VibrationLocation);
    }
    UE_LOGFMT(LogBS_BaseVibrationConfiguration, Log, "VibrationLocationsBitmask: {0}", VibrationLocationsBitmask);
    return VibrationLocationsBitmask;
}