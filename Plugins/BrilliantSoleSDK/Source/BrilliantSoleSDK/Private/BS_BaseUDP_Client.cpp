// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_BaseUDP_Client.h"
#include "Logging/StructuredLog.h"
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

void UBS_BaseUDP_Client::Reset()
{
    Super::Reset();
    bDidSendSetInListenPortMessage = false;
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
    if (UWorld *World = GetWorld())
    {
        UE_LOGFMT(LogBS_BaseUDP_Client, Verbose, "StartPinging");
        World->GetTimerManager().SetTimer(PingTimerHandler, this, &UBS_BaseUDP_Client::Ping, 2.0f, true);
    }
}
void UBS_BaseUDP_Client::StopPinging()
{
    if (UWorld *World = GetWorld())
    {
        UE_LOGFMT(LogBS_BaseUDP_Client, Verbose, "StopPinging");
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
void UBS_BaseUDP_Client::WaitForPong()
{
    if (UWorld *World = GetWorld())
    {
        UE_LOGFMT(LogBS_BaseUDP_Client, Verbose, "Waiting for Pong...");
        World->GetTimerManager().SetTimer(WaitForPongTimerHandler, this, &UBS_BaseUDP_Client::PongTimeout, 3.0f, false);
    }
}
void UBS_BaseUDP_Client::StopWaitingForPong()
{
    if (UWorld *World = GetWorld())
    {
        UE_LOGFMT(LogBS_BaseUDP_Client, Verbose, "StopWaitingForPong");
        World->GetTimerManager().ClearTimer(WaitForPongTimerHandler);
    }
}

void UBS_BaseUDP_Client::PongTimeout()
{
    UE_LOGFMT(LogBS_BaseUDP_Client, Verbose, "PongTimeout");
    Disconnect();
}

void UBS_BaseUDP_Client::Pong()
{
    UE_LOGFMT(LogBS_BaseUDP_Client, Verbose, "Ponging Server...");
    SendUDP_Messages({PongMessage}, true);
}
const FBS_UDP_Message UBS_BaseUDP_Client::PongMessage = {EBS_UDP_MessageType::PONG};

// PONG END

// MESSAGING START
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
// MESSAGING END

// PARSING START
void UBS_BaseUDP_Client::ParseUDP_Data(const TArray<uint8> &Data)
{
    const auto DataLength = Data.Num();
    uint16 Offset = 0;

    UE_LOGFMT(LogBS_BaseUDP_Client, Verbose, "Parsing {0} bytes...", DataLength);

    while (Offset < DataLength)
    {
        UE_LOGFMT(LogBS_BaseUDP_Client, Verbose, "Parsing Message at {0}...", Offset);

        const uint8 MessageTypeEnum = Data[Offset++];
        if (MessageTypeEnum >= static_cast<uint8>(EBS_UDP_MessageType::COUNT))
        {
            UE_LOGFMT(LogBS_BaseUDP_Client, Error, "invalid MessageTypeEnum {0}", MessageTypeEnum);
            continue;
        }
        const EBS_UDP_MessageType MessageType = static_cast<EBS_UDP_MessageType>(MessageTypeEnum);

        const uint16 MessageDataLength = BS_ByteParser::ParseAs<uint16>(Data, Offset, true);
        Offset += 2;

        UE_LOGFMT(LogBS_BaseUDP_Client, Verbose, "Message {0} ({1} bytes)", UEnum::GetValueAsString(MessageType), MessageDataLength);

        const TArrayView<const uint8> MessageData((uint8 *)(Data.GetData() + Offset), MessageDataLength);
        OnUDP_Message(MessageType, MessageData);

        Offset += MessageDataLength;
        UE_LOGFMT(LogBS_BaseUDP_Client, Verbose, "New Offset: {0}/{1}", Offset, DataLength);
    }
}

void UBS_BaseUDP_Client::OnUDP_Message(EBS_UDP_MessageType MessageType, const TArrayView<const uint8> &Message)
{
    UE_LOGFMT(LogBS_BaseUDP_Client, Verbose, "message {0} ({1} bytes)", UEnum::GetValueAsString(MessageType), Message.Num());
    StopWaitingForPong();

    switch (MessageType)
    {
    case EBS_UDP_MessageType::PING:
        Pong();
        break;
    case EBS_UDP_MessageType::PONG:
        // StopWaitingForPong();
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

    if (GetIsConnected())
    {
        WaitForPong();
    }
}

void UBS_BaseUDP_Client::OnSetRemoteReceivePortMessage(const TArrayView<const uint8> &Message)
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
    UE_LOGFMT(LogBS_BaseUDP_Client, Verbose, "Connecting to {0}:{1}", IP_Address, Port);
    SetConnectionStatus(EBS_ConnectionStatus::CONNECTING);
}
void UBS_BaseUDP_Client::Disconnect_Implementation()
{
    UE_LOGFMT(LogBS_BaseUDP_Client, Verbose, "Disconnect");

    SetConnectionStatus(EBS_ConnectionStatus::DISCONNECTING);
    Reset();
    StopPinging();
    StopWaitingForPong();
}

void UBS_BaseUDP_Client::ToggleConnection(const FString &IP_Address, const int32 Port)
{
    const EBS_ConnectionStatus ConnectionStatus = GetConnectionStatus();
    UE_LOGFMT(LogBS_BaseUDP_Client, Verbose, "Toggling Connection based on {0}...", UEnum::GetValueAsString(ConnectionStatus));
    switch (ConnectionStatus)
    {
    case EBS_ConnectionStatus::CONNECTED:
    case EBS_ConnectionStatus::CONNECTING:
        Disconnect();
        break;
    default:
        Connect(IP_Address, Port);
        break;
    }
}
// CONNECTION END