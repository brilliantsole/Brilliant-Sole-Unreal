// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#define EBS_VibrationMessages TRIGGER_VIBRATION

enum class EBS_VibrationMessage : uint8
{
    EBS_VibrationMessages,
    COUNT
};

ENUM_RANGE_BY_COUNT(EBS_VibrationMessage, static_cast<int32>(EBS_VibrationMessage::COUNT))