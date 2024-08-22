// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBS_InsoleSide : uint8
{
    LEFT UMETA(DisplayName = "Left"),
    RIGHT UMETA(DisplayName = "Right"),
    COUNT UMETA(Hidden)
};