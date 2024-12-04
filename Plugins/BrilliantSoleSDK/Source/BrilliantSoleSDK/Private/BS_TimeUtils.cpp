// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_TimeUtils.h"
#include "BS_ByteParser.h"
#include "Misc/DateTime.h"
#include <limits>
#include "Math/UnrealMathUtility.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(LogBS_TimeUtils);

uint64 TimeUtils::GetMilliseconds()
{
    FDateTime Now = FDateTime::UtcNow();
    FDateTime UnixEpoch(1970, 1, 1);
    FTimespan Timespan = Now - UnixEpoch;
    uint64 Milliseconds = static_cast<uint64>(Timespan.GetTotalMilliseconds());
    return Milliseconds;
}

uint64 TimeUtils::ParseTimestamp(const TArrayView<const uint8> &Message)
{
    const uint64 CurrentTimestamp = GetMilliseconds();
    UE_LOGFMT(LogBS_TimeUtils, Verbose, "CurrentTimestamp: {0}ms", CurrentTimestamp);

    uint16 RawTimestamp = BS_ByteParser::ParseAs<uint16>(Message, 0, true);
    UE_LOGFMT(LogBS_TimeUtils, Verbose, "RawTimestamp: {0}ms", RawTimestamp);

    uint64 Timestamp = CurrentTimestamp - (CurrentTimestamp % (UINT16_MAX + 1));
    Timestamp += RawTimestamp;

    UE_LOGFMT(LogBS_TimeUtils, Verbose, "Temp Timestamp: {0}ms", Timestamp);

    const uint64 TimestampDifference = FMath::Abs(CurrentTimestamp > Timestamp ? CurrentTimestamp - Timestamp : Timestamp - CurrentTimestamp);
    UE_LOGFMT(LogBS_TimeUtils, Verbose, "TimestampDifference: {0}ms", TimestampDifference);

    if (TimestampDifference > TimestampThreshold)
    {
        UE_LOGFMT(LogBS_TimeUtils, Verbose, "Correcting Timestamp Overflow");
        Timestamp += UINT16_MAX * FMath::Sign(CurrentTimestamp - Timestamp);
    }

    UE_LOGFMT(LogBS_TimeUtils, Verbose, "Timestamp: {0}ms", Timestamp);
    return Timestamp;
}

const uint64 TimeUtils::TimestampThreshold = 60000;