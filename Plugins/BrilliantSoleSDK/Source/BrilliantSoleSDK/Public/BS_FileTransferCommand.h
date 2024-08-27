// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBS_FileTransferCommand : uint8
{
    SEND UMETA(DisplayName = "Send"),
    RECEIVE UMETA(DisplayName = "Receive"),
    CANCEL UMETA(DisplayName = "Cancel"),

    COUNT UMETA(Hidden)
};

ENUM_RANGE_BY_COUNT(EBS_FileTransferCommand, static_cast<int32>(EBS_FileTransferCommand::COUNT))