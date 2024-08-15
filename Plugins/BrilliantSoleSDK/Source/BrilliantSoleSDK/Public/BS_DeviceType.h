// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBS_DeviceType : uint8
{
    LEFT_INSOLE UMETA(DisplayName = "Left Insole"),
    RIGHT_INSOLE UMETA(DisplayName = "Right Insole"),
    COUNT UMETA(Hidden)
};