// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBS_FileType : uint8
{
    TFLITE UMETA(DisplayName = "Tflite"),

    COUNT UMETA(Hidden)
};