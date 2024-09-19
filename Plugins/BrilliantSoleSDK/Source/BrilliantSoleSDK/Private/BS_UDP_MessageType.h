// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBS_UDP_MessageType : uint8
{
    PING UMETA(DisplayName = "Ping"),
    SET_REMOTE_RECEIVE_PORT UMETA(DisplayName = "Set Remote Receive Port"),
    SERVER_MESSAGE UMETA(DisplayName = "Server Message"),
    COUNT UMETA(Hidden)
};

ENUM_RANGE_BY_COUNT(EBS_UDP_MessageType, static_cast<int32>(EBS_UDP_MessageType::COUNT))