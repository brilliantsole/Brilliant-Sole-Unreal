// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBS_ServerMessageType : uint8
{
    PING UMETA(DisplayName = "Ping"),
    PONG UMETA(DisplayName = "Pong"),
    IS_SCANNING_AVAILABLE UMETA(DisplayName = "Is Scanning Available"),
    IS_SCANNING UMETA(DisplayName = "Is Scanning"),
    START_SCAN UMETA(DisplayName = "Start Scan"),
    STOP_SCAN UMETA(DisplayName = "Stop Scan"),
    DISCOVERED_DEVICE UMETA(DisplayName = "Discovered Device"),
    DISCOVERED_DEVICES UMETA(DisplayName = "Discovered Devices"),
    EXPIRED_DISCOVERED_DEVICE UMETA(DisplayName = "Expired Discovered Device"),
    CONNECT_TO_DEVICE UMETA(DisplayName = "Connect to Device"),
    DISCONNECT_FROM_DEVICE UMETA(DisplayName = "Disconnect from Device"),
    CONNECTED_DEVICES UMETA(DisplayName = "Connected Devices"),
    DEVICE_MESSAGE UMETA(DisplayName = "Device Message"),
    COUNT UMETA(Hidden)
};

ENUM_RANGE_BY_COUNT(EBS_ServerMessageType, static_cast<int32>(EBS_ServerMessageType::COUNT))