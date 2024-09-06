// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBS_FileType : uint8
{
    TFLITE UMETA(DisplayName = "Tflite"),
    COUNT UMETA(Hidden)
};

ENUM_RANGE_BY_COUNT(EBS_FileType, static_cast<int32>(EBS_FileType::COUNT))