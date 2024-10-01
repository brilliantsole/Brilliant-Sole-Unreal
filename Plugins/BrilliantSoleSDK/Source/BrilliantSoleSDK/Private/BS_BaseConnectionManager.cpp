// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_BaseConnectionManager.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(LogBS_BaseConnectionManager);

UBS_BaseConnectionManager::UBS_BaseConnectionManager()
{
    UE_LOGFMT(LogBS_BaseConnectionManager, Verbose, "Constructor: {0}", GetName());
    if (HasAnyFlags(RF_ClassDefaultObject))
    {
        UE_LOGFMT(LogBS_BaseConnectionManager, Verbose, "CDO - Skipping Constructor");
        return;
    }
}

// CONNECTION START
void UBS_BaseConnectionManager::Connect_Implementation(bool &bContinue)
{
    switch (ConnectionStatus)
    {
    case EBS_ConnectionStatus::CONNECTED:
        UE_LOGFMT(LogBS_BaseConnectionManager, Verbose, "Already connected");
        bContinue = false;
        break;
    default:
        UE_LOGFMT(LogBS_BaseConnectionManager, Verbose, "Connecting...");
        SetConnectionStatus(EBS_ConnectionStatus::CONNECTING);
        bContinue = true;
        break;
    }
}

void UBS_BaseConnectionManager::Disconnect_Implementation(bool &bContinue)
{
    switch (ConnectionStatus)
    {
    case EBS_ConnectionStatus::NOT_CONNECTED:
        UE_LOGFMT(LogBS_BaseConnectionManager, Verbose, "Already not connected");
        bContinue = false;
        break;
    default:
        UE_LOGFMT(LogBS_BaseConnectionManager, Verbose, "Disconnecting...");
        SetConnectionStatus(EBS_ConnectionStatus::DISCONNECTING);
        bContinue = true;
        break;
    }
}

void UBS_BaseConnectionManager::ToggleConnection()
{
    switch (ConnectionStatus)
    {
    case EBS_ConnectionStatus::NOT_CONNECTED:
        Connect();
        break;
    default:
        Disconnect();
        break;
    }
}
// CONNECTION END

// CONNECTION STATUS START
void UBS_BaseConnectionManager::UpdateConnectionStatus()
{
    if (GetIsConnected())
    {
        SetConnectionStatus(EBS_ConnectionStatus::CONNECTED);
    }
    else
    {
        SetConnectionStatus(EBS_ConnectionStatus::NOT_CONNECTED);
    }
}
void UBS_BaseConnectionManager::SetConnectionStatus(EBS_ConnectionStatus NewConnectionStatus)
{
    if (NewConnectionStatus == ConnectionStatus)
    {
        UE_LOGFMT(LogBS_BaseConnectionManager, Verbose, "Redundant ConnectionStatus Assignment");
        return;
    }

    ConnectionStatus = NewConnectionStatus;
    UE_LOGFMT(LogBS_BaseConnectionManager, Verbose, "Updated ConnectionStatus to {0}", UEnum::GetValueAsString(ConnectionStatus));

    OnConnectionUpdate.Broadcast(this, ConnectionStatus);

    switch (ConnectionStatus)
    {
    case EBS_ConnectionStatus::NOT_CONNECTED:
        OnNotConnected.Broadcast(this);
        break;
    case EBS_ConnectionStatus::CONNECTING:
        OnConnecting.Broadcast(this);
        break;
    case EBS_ConnectionStatus::CONNECTED:
        OnConnected.Broadcast(this);
        break;
    case EBS_ConnectionStatus::DISCONNECTING:
        OnDisconnecting.Broadcast(this);
        break;
    }
}
// CONNECTION STATUS START

// RX DATA END
void UBS_BaseConnectionManager::ParseRxData(const TArray<uint8> &Data)
{
    const TArrayView<const uint8> &DataView = Data;
    ParseRxData(DataView);
}

void UBS_BaseConnectionManager::ParseRxData(const TArrayView<const uint8> &Data)
{
    UE_LOGFMT(LogBS_BaseConnectionManager, Verbose, "Parsing Rx Data ({0} Bytes)", Data.Num());

    const auto DataLength = Data.Num();
    uint16 Offset = 0;

    UE_LOGFMT(LogBS_BaseConnectionManager, Verbose, "Parsing {0} bytes...", DataLength);

    while (Offset < DataLength)
    {
        UE_LOGFMT(LogBS_BaseConnectionManager, Verbose, "Parsing Message at {0}...", Offset);

        const uint8 MessageTypeEnum = Data[Offset++];
        if (MessageTypeEnum >= static_cast<uint8>(EBS_TxRxMessage::COUNT))
        {
            UE_LOGFMT(LogBS_BaseConnectionManager, Error, "invalid EBS_TxRxMessageType {0}", MessageTypeEnum);
            break;
        }
        const EBS_TxRxMessage MessageType = static_cast<EBS_TxRxMessage>(MessageTypeEnum);

        const uint16 MessageDataLength = BS_ByteParser::ParseAs<uint16>(Data, Offset, true);
        Offset += 2;

        UE_LOGFMT(LogBS_BaseConnectionManager, Verbose, "Message {0} ({1} bytes)", static_cast<uint8>(MessageType), MessageDataLength);

        const TArrayView<const uint8> MessageData((uint8 *)(Data.GetData() + Offset), MessageDataLength);
        OnRxMessage.Broadcast(this, MessageType, MessageData);

        Offset += MessageDataLength;
        UE_LOGFMT(LogBS_BaseConnectionManager, Verbose, "New Offset: {0}/{1}", Offset, DataLength);
    }

    OnRxMessages.Broadcast(this);
}
// RX DATA START

// MESSAGING START
void UBS_BaseConnectionManager::SendTxData_Implementation(const TArray<uint8> &Data)
{
    UE_LOGFMT(LogBS_BaseConnectionManager, Verbose, "Sending {0} Bytes...", Data.Num());
}

void UBS_BaseConnectionManager::SendPingTxData()
{
    UE_LOGFMT(LogBS_BaseConnectionManager, Verbose, "Sending Ping Tx Data");
    SendTxData(PingTxData);
}
// MESSAGING END

// PING START
const FBS_TxMessage UBS_BaseConnectionManager::PingTxMessage = {EBS_TxRxMessage::GET_MTU};
const TArray<uint8> UBS_BaseConnectionManager::PingTxData = UBS_BaseConnectionManager::InitializePingTxData();
const TArray<uint8> UBS_BaseConnectionManager::InitializePingTxData()
{
    TArray<uint8> _PingTxData;
    PingTxMessage.AppendTo(_PingTxData);
    return _PingTxData;
}
// PING STOP