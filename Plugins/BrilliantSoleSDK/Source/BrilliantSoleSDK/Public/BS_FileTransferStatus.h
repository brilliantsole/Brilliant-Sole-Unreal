// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBS_FileTransferStatus : uint8
{
    IDLE UMETA(DisplayName = "Idle"),
    SENDING UMETA(DisplayName = "Sending"),
    RECEIVING UMETA(DisplayName = "Receiving"),

    COUNT UMETA(Hidden)
};

ENUM_RANGE_BY_COUNT(EBS_FileTransferStatus, static_cast<int32>(EBS_FileTransferStatus::COUNT))