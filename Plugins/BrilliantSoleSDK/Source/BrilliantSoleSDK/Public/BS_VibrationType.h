// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBS_VibrationType : uint8
{
    WAVEFORM_EFFECT UMETA(DisplayName = "Waveform Effect"),
    WAVEFORM UMETA(DisplayName = "Waveform"),
    COUNT UMETA(Hidden)
};

ENUM_RANGE_BY_COUNT(EBS_VibrationType, static_cast<int32>(EBS_VibrationType::COUNT))