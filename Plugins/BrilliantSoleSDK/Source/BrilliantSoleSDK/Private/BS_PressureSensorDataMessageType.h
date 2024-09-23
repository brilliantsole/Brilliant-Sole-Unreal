// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#define EBS_PressurePositionsMessages GET_PRESSURE_POSITIONS

enum class EBS_PressurePositionsMessage : uint8
{
    EBS_PressurePositionsMessages,
    COUNT
};

ENUM_RANGE_BY_COUNT(EBS_PressurePositionsMessage, static_cast<int32>(EBS_PressurePositionsMessage::COUNT))