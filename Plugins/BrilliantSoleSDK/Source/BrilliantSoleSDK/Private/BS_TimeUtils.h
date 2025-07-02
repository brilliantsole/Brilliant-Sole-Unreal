// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Logging/StructuredLog.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_TimeUtils, Warning, All);

class TimeUtils
{
public:
    static uint64 GetMilliseconds();
    static uint64 ParseTimestamp(const TArrayView<const uint8> &Message);

private:
    static const uint64 TimestampThreshold;
};