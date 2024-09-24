// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_BaseConnectionManager.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(LogBS_BaseConnectionManager);

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