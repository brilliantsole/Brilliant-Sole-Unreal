// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBS_VibrationLocation : uint8
{
    FRONT UMETA(DisplayName = "Front"),
    REAR UMETA(DisplayName = "Rear"),
    COUNT UMETA(Hidden)
};