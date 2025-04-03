// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBS_DevicePairType : uint8
{
    INSOLES UMETA(DisplayName = "Insoles"),
    GLOVES UMETA(DisplayName = "Gloves"),

    COUNT UMETA(Hidden)
};

ENUM_RANGE_BY_COUNT(EBS_DevicePairType, static_cast<int32>(EBS_DevicePairType::COUNT))