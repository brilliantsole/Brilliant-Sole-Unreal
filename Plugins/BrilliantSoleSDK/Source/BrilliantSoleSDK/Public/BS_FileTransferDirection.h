// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBS_FileTransferDirection : uint8
{
    SENDING UMETA(DisplayName = "Sending"),
    RECEIVING UMETA(DisplayName = "Receiving"),

    COUNT UMETA(Hidden)
};