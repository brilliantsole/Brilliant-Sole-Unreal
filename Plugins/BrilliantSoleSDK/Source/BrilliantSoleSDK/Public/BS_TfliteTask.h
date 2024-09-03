// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBS_TfliteTask : uint8
{
    CLASSIFICATION UMETA(DisplayName = "Classification"),
    REGRESSION UMETA(DisplayName = "Regression"),

    COUNT UMETA(Hidden)
};

ENUM_RANGE_BY_COUNT(EBS_TfliteTask, static_cast<int32>(EBS_TfliteTask::COUNT))