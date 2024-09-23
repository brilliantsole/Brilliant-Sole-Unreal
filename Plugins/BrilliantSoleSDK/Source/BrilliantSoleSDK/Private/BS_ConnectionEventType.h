// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "BS_ConnectionStatus.h"

#define EBS_ConnectionEvents EBS_ConnectionStatuses, \
                             CONNECTION_STATUS,      \
                             IS_CONNECTED

enum class EBS_ConnectionEvent : uint8
{
    EBS_ConnectionEvents,
    COUNT
};

ENUM_RANGE_BY_COUNT(EBS_ConnectionEvent, static_cast<int32>(EBS_ConnectionEvent::COUNT))