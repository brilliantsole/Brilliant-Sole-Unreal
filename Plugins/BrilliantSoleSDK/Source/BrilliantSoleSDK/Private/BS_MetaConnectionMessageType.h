// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#define EBS_MetaConnectionMessages RX, \
                                   TX

enum class EBS_MetaConnectionMessage : uint8
{
    EBS_MetaConnectionMessages,
    COUNT
};

ENUM_RANGE_BY_COUNT(EBS_MetaConnectionMessage, static_cast<int32>(EBS_MetaConnectionMessage::COUNT))