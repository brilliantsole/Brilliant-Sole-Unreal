// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#define EBS_SMP_Messages SMP

enum class EBS_SMP_Message : uint8
{
    EBS_SMP_Messages,
    COUNT
};

ENUM_RANGE_BY_COUNT(EBS_SMP_Message, static_cast<int32>(EBS_SMP_Message::COUNT))

#define EBS_SMP_Events EBS_SMP_Messages,         \
                       FIRMWARE_IMAGES,          \
                       FIRMWARE_UPLOAD_PROGRESS, \
                       FIRMWARE_STATUS,          \
                       FIRMWARE_UPLOAD_COMPLETE

enum class EBS_SMP_Event : uint8
{
    EBS_SMP_Events,
    COUNT
};

ENUM_RANGE_BY_COUNT(EBS_SMP_Event, static_cast<int32>(EBS_SMP_Event::COUNT))
