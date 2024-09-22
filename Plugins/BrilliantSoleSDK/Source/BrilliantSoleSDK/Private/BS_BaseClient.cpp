// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_BaseClient.h"
#include "Logging/StructuredLog.h"
#include "BS_Message.h"
#include "BS_Subsystem.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "BS_ByteParser.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "BS_DeviceType.h"
#include "GenericPlatform/GenericPlatformTime.h"

DEFINE_LOG_CATEGORY(LogBS_BaseClient);

UBS_BaseClient::UBS_BaseClient()
{
    UE_LOGFMT(LogBS_BaseClient, Verbose, "Constructor: {0}", GetName());
    if (HasAnyFlags(RF_ClassDefaultObject))
    {
        UE_LOGFMT(LogBS_BaseClient, Verbose, "CDO - Skipping Constructor");
        return;
    }

    BoundOnMessage.BindUObject(this, &UBS_BaseClient::OnMessage);
}

void UBS_BaseClient::Reset()
{
    bIsScanningAvailable = false;
    bIsScanning = false;

    DiscoveredDevices.Reset();
    // FILL
}

void UBS_BaseClient::PostInitProperties()
{
    Super::PostInitProperties();
    UE_LOGFMT(LogBS_BaseClient, Verbose, "PostInitProperties {0}", GetName());
    if (HasAnyFlags(RF_ClassDefaultObject))
    {
        UE_LOGFMT(LogBS_BaseClient, Verbose, "CDO - Skipping");
        return;
    }

    GetBS_Subsystem();
    InitializeBP();
}

void UBS_BaseClient::GetBS_Subsystem()
{
    if (!HasAnyFlags(RF_ClassDefaultObject))
    {
        UWorld *World = GetWorld();
        if (World)
        {
            UE_LOGFMT(LogBS_BaseClient, Verbose, "World found");
            UGameInstance *GameInstance = World->GetGameInstance();
            if (GameInstance)
            {
                UE_LOGFMT(LogBS_BaseClient, Verbose, "GameInstance found");
                UBS_Subsystem *__BS_Subsystem = GameInstance->GetSubsystem<UBS_Subsystem>();
                if (__BS_Subsystem)
                {
                    UE_LOGFMT(LogBS_BaseClient, Verbose, "BS_Subsystem found");
                    _BS_Subsystem = __BS_Subsystem;
                }
                else
                {
                    UE_LOGFMT(LogBS_BaseClient, Error, "BS_Subsystem not found");
                }
            }
            else
            {
                UE_LOGFMT(LogBS_BaseClient, Error, "GameInstance not found");
            }
        }
        else
        {
            UE_LOGFMT(LogBS_BaseClient, Error, "World not found");
        }
    }
    else
    {
        UE_LOGFMT(LogBS_BaseClient, Verbose, "CDO constructor - skipping");
    }
}

// CONNECTION START
void UBS_BaseClient::SetConnectionStatus(EBS_ConnectionStatus NewConnectionStatus)
{
    if (ConnectionStatus == NewConnectionStatus)
    {
        UE_LOGFMT(LogBS_BaseClient, Verbose, "Redundant ConnectionStatus {0} - not updating", UEnum::GetValueAsString(ConnectionStatus));
        return;
    }
    ConnectionStatus = NewConnectionStatus;
    UE_LOGFMT(LogBS_BaseClient, Verbose, "ConnectionStatus Updated to {0}", UEnum::GetValueAsString(ConnectionStatus));
    OnConnectionStatusUpdate.Broadcast(this, ConnectionStatus);

    switch (ConnectionStatus)
    {
    case EBS_ConnectionStatus::CONNECTED:
    case EBS_ConnectionStatus::NOT_CONNECTED:
        OnIsConnectedUpdate.Broadcast(this, GetIsConnected());
        if (ConnectionStatus == EBS_ConnectionStatus::CONNECTED)
        {
            SendRequiredMessages();
        }
        else
        {
            Reset();
        }
        break;
    default:
        break;
    }
}
// CONNECTION END

// MESSAGING START
void UBS_BaseClient::OnMessage(EBS_ServerMessageType MessageType, const TArray<uint8> &Message)
{
    UE_LOGFMT(LogBS_BaseClient, Verbose, "message {0} ({1} bytes)", UEnum::GetValueAsString(MessageType), Message.Num());

    switch (MessageType)
    {
    case EBS_ServerMessageType::IS_SCANNING_AVAILABLE:
        ParseIsScanningAvailable(Message);
        break;
    case EBS_ServerMessageType::IS_SCANNING:
        ParseIsScanning(Message);
        break;
    case EBS_ServerMessageType::DISCOVERED_DEVICE:
        ParseDiscoveredDevice(Message);
        break;
    case EBS_ServerMessageType::EXPIRED_DISCOVERED_DEVICE:
        ParseExpiredDiscoveredDevice(Message);
        break;
    case EBS_ServerMessageType::CONNECTED_DEVICES:
        ParseConnectedDevices(Message);
        break;
    case EBS_ServerMessageType::DEVICE_MESSAGE:
        ParseDeviceMessage(Message);
        break;
    default:
        UE_LOGFMT(LogBS_BaseClient, Error, "Uncaught MessageType {0}", UEnum::GetValueAsString(MessageType));
        break;
    }
}

void UBS_BaseClient::OnData(const TArray<uint8> &Data)
{
    UE_LOGFMT(LogBS_BaseClient, Log, "Parsing {0} Bytes...", Data.Num());
    UBS_ParseUtils::ParseServerData(Data, BoundOnMessage);
}

void UBS_BaseClient::SendMessages(const TArray<FBS_ServerMessage> &ServerMessages, bool bSendImmediately)
{
    TArray<uint8> MessageData;

    for (const FBS_ServerMessage &ServerMessage : ServerMessages)
    {
        ServerMessage.AppendTo(MessageData);
    }

    if (MessageData.IsEmpty())
    {
        UE_LOGFMT(LogBS_BaseClient, Verbose, "MessageData is Empty - won't send any data");
        return;
    }

    UE_LOGFMT(LogBS_BaseClient, Verbose, "Sending {0} bytes", MessageData.Num());

    SendMessageData(MessageData, bSendImmediately);
}

const TArray<EBS_ServerMessageType> UBS_BaseClient::RequiredMessageTypes = {EBS_ServerMessageType::IS_SCANNING_AVAILABLE, EBS_ServerMessageType::DISCOVERED_DEVICES, EBS_ServerMessageType::CONNECTED_DEVICES};

const TArray<FBS_ServerMessage> UBS_BaseClient::InitializeRequiredMessages()
{
    TArray<FBS_ServerMessage> _RequiredMessages;
    for (const EBS_ServerMessageType MessageType : UBS_BaseClient::RequiredMessageTypes)
    {
        _RequiredMessages.Add({MessageType});
    }
    return _RequiredMessages;
}
const TArray<FBS_ServerMessage> UBS_BaseClient::RequiredMessages = UBS_BaseClient::InitializeRequiredMessages();
// MESSAGING END

// SCANNING START
void UBS_BaseClient::ParseIsScanningAvailable(const TArray<uint8> &Message)
{
    bool bNewIsScanningAvailable = static_cast<bool>(Message[0]);
    UE_LOGFMT(LogBS_BaseClient, Verbose, "bNewIsScanningAvailable: {0}", bNewIsScanningAvailable);

    if (bNewIsScanningAvailable == bIsScanningAvailable)
    {
        UE_LOGFMT(LogBS_BaseClient, Verbose, "Redundant bIsScanningAvailable Assignment");
        return;
    }

    bIsScanningAvailable = bNewIsScanningAvailable;
    UE_LOGFMT(LogBS_BaseClient, Verbose, "Updated bIsScanningAvailable to {0}", bIsScanningAvailable);
    OnIsScanningAvailableUpdate.Broadcast(this, bIsScanningAvailable);

    if (bIsScanningAvailable)
    {
        UE_LOGFMT(LogBS_BaseClient, Verbose, "Checking if IsScanning...");
        SendMessages({{EBS_ServerMessageType::IS_SCANNING}});
    }
}
void UBS_BaseClient::ParseIsScanning(const TArray<uint8> &Message)
{
    bool bNewIsScanning = static_cast<bool>(Message[0]);
    UE_LOGFMT(LogBS_BaseClient, Verbose, "bNewIsScanning: {0}", bNewIsScanning);

    if (bNewIsScanning == bIsScanning)
    {
        UE_LOGFMT(LogBS_BaseClient, Verbose, "Redundant bIsScanning Assignment");
        return;
    }

    bIsScanning = bNewIsScanning;
    UE_LOGFMT(LogBS_BaseClient, Verbose, "Updated bIsScanning to {0}", bIsScanning);
    OnIsScanningUpdate.Broadcast(this, bIsScanning);
}

void UBS_BaseClient::StartScan()
{
    if (!bIsScanningAvailable)
    {
        UE_LOGFMT(LogBS_BaseClient, Verbose, "Scanning is not available");
        return;
    }
    if (bIsScanning)
    {
        UE_LOGFMT(LogBS_BaseClient, Verbose, "Already scanning");
        return;
    }
    DiscoveredDevices.Reset();
    SendMessages({{EBS_ServerMessageType::START_SCAN}});
}
void UBS_BaseClient::StopScan()
{
    if (!bIsScanning)
    {
        UE_LOGFMT(LogBS_BaseClient, Verbose, "Already not scanning");
        return;
    }
    SendMessages({{EBS_ServerMessageType::STOP_SCAN}});
}

void UBS_BaseClient::ToggleScan()
{
    if (bIsScanning)
    {
        StopScan();
    }
    else
    {
        StartScan();
    }
}
// SCANNING END

// DISCOVERED DEVICES START
void UBS_BaseClient::ParseDiscoveredDevice(const TArray<uint8> &Message)
{
    UE_LOGFMT(LogBS_BaseClient, Log, "Parsing Discovered Device ({0} bytes)...", Message.Num());

    FBS_DiscoveredDevice DiscoveredDevice;
    if (DiscoveredDevice.Parse(Message))
    {
        DiscoveredDevices.Emplace(DiscoveredDevice.BluetoothId, DiscoveredDevice);
        OnDiscoveredDevice.Broadcast(this, DiscoveredDevice);
    }
    else
    {
        UE_LOGFMT(LogBS_BaseClient, Error, "Failed to parse Discovered Device");
    }
}
void UBS_BaseClient::ParseExpiredDiscoveredDevice(const TArray<uint8> &Message)
{
    UE_LOGFMT(LogBS_BaseClient, Log, "Parsing Expired Discovered Device ({0} bytes)...", Message.Num());
    // FILL
}
// DISCOVERED DEVICES END

// CONNECTED DEVICES START
void UBS_BaseClient::ParseConnectedDevices(const TArray<uint8> &Message)
{
    UE_LOGFMT(LogBS_BaseClient, Log, "Parsing Connected Devices ({0} bytes)...", Message.Num());
    // FILL
}
// CONNECTED DEVICES END

// DEVICE MESSAGE START
void UBS_BaseClient::ParseDeviceMessage(const TArray<uint8> &Message)
{
    UE_LOGFMT(LogBS_BaseClient, Log, "Parsing Device Message ({0} bytes)...", Message.Num());
    // FILL
}

// DEVICE MESSAGE END