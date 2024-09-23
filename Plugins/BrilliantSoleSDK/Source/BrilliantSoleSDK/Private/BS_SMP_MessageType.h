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