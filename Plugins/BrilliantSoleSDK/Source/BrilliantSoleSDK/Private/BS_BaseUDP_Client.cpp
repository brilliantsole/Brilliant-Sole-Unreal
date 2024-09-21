// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_BaseUDP_Client.h"
#include "Logging/StructuredLog.h"
#include "BS_Message.h"
#include "BS_Subsystem.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "BS_ByteParser.h"
#include "GenericPlatform/GenericPlatformTime.h"

DEFINE_LOG_CATEGORY(LogBS_BaseUDP_Client);

UBS_BaseUDP_Client::UBS_BaseUDP_Client()
    : UBS_BaseClient()
{
    UE_LOGFMT(LogBS_BaseUDP_Client, Verbose, "Constructor: {0}", GetName());
    if (HasAnyFlags(RF_ClassDefaultObject))
    {
        UE_LOGFMT(LogBS_BaseUDP_Client, Verbose, "CDO - Skipping Constructor");
        return;
    }

    UDP_Manager = CreateDefaultSubobject<UUDPManager>(TEXT("UDPManager"));
}

// IN LISTEN PORT START
int32 UBS_BaseUDP_Client::SetInListenPort(int32 NewInListenPort)
{
    // UE_LOGFMT(LogBS_BaseUDP_Client, Verbose, "Setting InListenPort to {0}...", NewInListenPort);
    if (InListenPort != NewInListenPort)
    {
        InListenPort = NewInListenPort;
        SetInListenPortUDP_Message.Data = BS_ByteParser::ToByteArray<uint16>(InListenPort);
        UE_LOGFMT(LogBS_BaseUDP_Client, Verbose, "Updated InListenPort to {0}", InListenPort);
    }
    else
    {
        UE_LOGFMT(LogBS_BaseUDP_Client, Verbose, "Redundant InListenPort assignment {0}", InListenPort);
    }
    return InListenPort;
}
// IN LISTEN PORT END

// PING START
void UBS_BaseUDP_Client::StartPinging()
{
    UWorld *World = GetWorld();
    if (World)
    {
        World->GetTimerManager().SetTimer(PingTimerHandler, this, &UBS_BaseUDP_Client::Ping, 2.0f, true);
    }
}
void UBS_BaseUDP_Client::StopPinging()
{
    UWorld *World = GetWorld();
    if (World)
    {
        World->GetTimerManager().ClearTimer(PingTimerHandler);
    }
}

void UBS_BaseUDP_Client::Ping()
{
    UE_LOGFMT(LogBS_BaseUDP_Client, Verbose, "Pinging...");
    const FBS_UDP_Message &_PingMessage = bDidSendSetInListenPortMessage ? PingMessage : SetInListenPortUDP_Message;
    SendUDP_Messages({_PingMessage}, true);
};

const FBS_UDP_Message UBS_BaseUDP_Client::PingMessage = {EBS_UDP_MessageType::PING};
// PING END

// PONG START
void UBS_BaseUDP_Client::Pong()
{
    UE_LOGFMT(LogBS_BaseUDP_Client, Verbose, "Ponging Server...");
    SendUDP_Messages({PongMessage}, true);
}
const FBS_UDP_Message UBS_BaseUDP_Client::PongMessage = {EBS_UDP_MessageType::PONG};

// PONG END

// MESSAGE START
void UBS_BaseUDP_Client::SendMessageData(const TArray<uint8> &Data, bool bSendImmediately)
{
    SendUDP_Messages({{EBS_UDP_MessageType::SERVER_MESSAGE, Data}}, bSendImmediately);
}
void UBS_BaseUDP_Client::SendUDP_Messages(const TArray<FBS_UDP_Message> &UDP_Messages, bool bSendImmediately)
{
    UE_LOGFMT(LogBS_BaseUDP_Client, Verbose, "Requesting to send {0} Messages...", UDP_Messages.Num());
    PendingUDP_Messages.Append(UDP_Messages);
    if (!bSendImmediately)
    {
        UE_LOGFMT(LogBS_BaseUDP_Client, Verbose, "Not sending data immediately");
        return;
    }
    if (bIsSendingUDP_Data)
    {
        UE_LOGFMT(LogBS_BaseUDP_Client, Verbose, "Already sending data - will wait until new data is sent");
        return;
    }
    SendPendingUDP_Messages();
}
void UBS_BaseUDP_Client::SendPendingUDP_Messages()
{
    if (PendingUDP_Messages.IsEmpty())
    {
        return;
    }
    bIsSendingUDP_Data = true;

    UDP_Data.Reset();

    const uint32 MaxMessageLength = MAX_UDP_MESSAGE_SIZE;

    uint8 PendingUDP_MessageIndex = 0;
    while (PendingUDP_MessageIndex < PendingUDP_Messages.Num())
    {
        const FBS_UDP_Message &PendingUDP_Message = PendingUDP_Messages[PendingUDP_MessageIndex];

        uint16 PendingUDP_MessageLength = PendingUDP_Message.Num();

        bool ShouldAppendUDP_Message = MaxMessageLength == 0 || UDP_Data.Num() + PendingUDP_MessageLength <= MaxMessageLength;
        if (ShouldAppendUDP_Message)
        {
            UE_LOGFMT(LogBS_BaseUDP_Client, Verbose, "Appending message {0} ({1} bytes)", UEnum::GetValueAsString(PendingUDP_Message.Type), PendingUDP_MessageLength);
            PendingUDP_Message.AppendTo(UDP_Data);
            PendingUDP_Messages.RemoveAt(PendingUDP_MessageIndex);
        }
        else
        {
            UE_LOGFMT(LogBS_BaseUDP_Client, Verbose, "Skipping message {0} ({1} bytes)", UEnum::GetValueAsString(PendingUDP_Message.Type), PendingUDP_MessageLength);
            PendingUDP_MessageIndex++;
        }
    }

    if (UDP_Data.IsEmpty())
    {
        UE_LOGFMT(LogBS_BaseUDP_Client, Verbose, "UDP_Data is Empty - won't send any data");
        bIsSendingUDP_Data = false;
        return;
    }

    UE_LOGFMT(LogBS_BaseUDP_Client, Verbose, "Sending {0} bytes", UDP_Data.Num());

    SendUDP_Data(UDP_Data);
}
// MESSAGE END

// PARSING START
void UBS_BaseUDP_Client::OnUDP_Message(EBS_UDP_MessageType MessageType, const TArray<uint8> &Message)
{
    UE_LOGFMT(LogBS_BaseUDP_Client, Verbose, "message {0} ({1} bytes)", UEnum::GetValueAsString(MessageType), Message.Num());

    switch (MessageType)
    {
    case EBS_UDP_MessageType::PING:
        Pong();
        break;
    case EBS_UDP_MessageType::PONG:
        break;
    case EBS_UDP_MessageType::SET_REMOTE_RECEIVE_PORT:
        OnSetRemoteReceivePortMessage(Message);
        break;
    case EBS_UDP_MessageType::SERVER_MESSAGE:
        OnData(Message);
        break;
    default:
        UE_LOGFMT(LogBS_BaseUDP_Client, Error, "Uncaught MessageType {0}", UEnum::GetValueAsString(MessageType));
        break;
    }
}

void UBS_BaseUDP_Client::OnSetRemoteReceivePortMessage(const TArray<uint8> &Message)
{
    const uint16 ParsedRemoteReceivePort = BS_ByteParser::ParseAs<uint16>(Message);
    UE_LOGFMT(LogBS_BaseUDP_Client, Verbose, "Parsed RemoteReceivePort: {0}", ParsedRemoteReceivePort);

    if (ParsedRemoteReceivePort != InListenPort)
    {
        UE_LOGFMT(LogBS_BaseUDP_Client, Error, "Incorrect RemoteReceivePort (expected {0}, got {1})", InListenPort, ParsedRemoteReceivePort);
        return;
    }

    UE_LOGFMT(LogBS_BaseUDP_Client, Verbose, "Successfully set RemoteReceivePort");

    bDidSendSetInListenPortMessage = true;

    SetConnectionStatus(EBS_ConnectionStatus::CONNECTED);
}
// PARSING END

// CONNECTION START
void UBS_BaseUDP_Client::Connect_Implementation(const FString &IP_Address, const int32 Port)
{
    SetConnectionStatus(EBS_ConnectionStatus::CONNECTING);
}
void UBS_BaseUDP_Client::Disconnect_Implementation()
{
    SetConnectionStatus(EBS_ConnectionStatus::DISCONNECTING);
}
// CONNECTION END