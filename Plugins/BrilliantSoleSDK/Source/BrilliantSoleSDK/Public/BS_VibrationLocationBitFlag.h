// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EBS_VibrationLocationBitFlag : uint8
{
    NONE = 0 UMETA(Hidden),
    FRONT = 1 << 0 UMETA(DisplayName = "Front"),
    REAR = 1 << 1 UMETA(DisplayName = "Rear")
};
ENUM_CLASS_FLAGS(EBS_VibrationLocationBitFlag);
