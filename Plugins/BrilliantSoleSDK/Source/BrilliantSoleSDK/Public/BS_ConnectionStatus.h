// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBS_ConnectionStatus : uint8
{
    NOT_CONNECTED UMETA(DisplayName = "Not Connected"),
    CONNECTING UMETA(DisplayName = "Connecting"),
    CONNECTED UMETA(DisplayName = "Connected"),
    DISCONNECTING UMETA(DisplayName = "Disconnecting"),
    COUNT UMETA(Hidden)
};

ENUM_RANGE_BY_COUNT(EBS_ConnectionStatus, static_cast<int32>(EBS_ConnectionStatus::COUNT))