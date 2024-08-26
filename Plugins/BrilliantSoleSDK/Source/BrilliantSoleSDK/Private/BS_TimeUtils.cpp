// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_TimeUtils.h"
#include "BS_ByteParser.h"
#include "Misc/DateTime.h"
#include <limits>
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

uint64 TimeUtils::ParseTimestamp(const TArray<uint8> &Message)
{
    uint16 RawTimestamp = ByteParser::ParseAs<uint16>(Message, 0);
    UE_LOGFMT(LogBS_TimeUtils, Log, "RawTimestamp:{0}", RawTimestamp);

    uint64 Milliseconds = GetMilliseconds();
    UE_LOGFMT(LogBS_TimeUtils, Log, "Milliseconds: {0}ms", Milliseconds);
    Milliseconds -= Milliseconds % UINT16_MAX;
    UE_LOGFMT(LogBS_TimeUtils, Log, "Milliseconds without lower 2 bytes: {0}ms", Milliseconds);
    Milliseconds += RawTimestamp;
    UE_LOGFMT(LogBS_TimeUtils, Log, "Milliseconds with RawTimestamp: {0}ms", Milliseconds);

    return Milliseconds;
}