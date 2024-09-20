// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_BaseUDP_Manager.h"
#include "Logging/StructuredLog.h"
#include "BS_Message.h"
#include "BS_Subsystem.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "BS_ByteParser.h"
#include "GenericPlatform/GenericPlatformTime.h"

DEFINE_LOG_CATEGORY(LogBS_BaseUDP_Manager);

UBS_BaseUDP_Manager::UBS_BaseUDP_Manager()
{
    UE_LOGFMT(LogBS_BaseUDP_Manager, Verbose, "Constructor: {0}", GetName());
    if (HasAnyFlags(RF_ClassDefaultObject))
    {
        UE_LOGFMT(LogBS_BaseUDP_Manager, Verbose, "CDO - Skipping Constructor");
        return;
    }

    UDP_Manager = CreateDefaultSubobject<UUDPManager>(TEXT("UDPManager"));
}

void UBS_BaseUDP_Manager::PostInitProperties()
{
    Super::PostInitProperties();
    UE_LOGFMT(LogBS_BaseUDP_Manager, Verbose, "PostInitProperties {0}", GetName());
    if (HasAnyFlags(RF_ClassDefaultObject))
    {
        UE_LOGFMT(LogBS_BaseUDP_Manager, Verbose, "CDO - Skipping");
        return;
    }

    GetBS_Subsystem();
    InitializeBP();
}

void UBS_BaseUDP_Manager::GetBS_Subsystem()
{
    if (!HasAnyFlags(RF_ClassDefaultObject))
    {
        UWorld *World = GetWorld();
        if (World)
        {
            UE_LOGFMT(LogBS_BaseUDP_Manager, Verbose, "World found");
            UGameInstance *GameInstance = World->GetGameInstance();
            if (GameInstance)
            {
                UE_LOGFMT(LogBS_BaseUDP_Manager, Verbose, "GameInstance found");
                UBS_Subsystem *__BS_Subsystem = GameInstance->GetSubsystem<UBS_Subsystem>();
                if (__BS_Subsystem)
                {
                    UE_LOGFMT(LogBS_BaseUDP_Manager, Verbose, "BS_Subsystem found");
                    _BS_Subsystem = __BS_Subsystem;
                }
                else
                {
                    UE_LOGFMT(LogBS_BaseUDP_Manager, Error, "BS_Subsystem not found");
                }
            }
            else
            {
                UE_LOGFMT(LogBS_BaseUDP_Manager, Error, "GameInstance not found");
            }
        }
        else
        {
            UE_LOGFMT(LogBS_BaseUDP_Manager, Error, "World not found");
        }
    }
    else
    {
        UE_LOGFMT(LogBS_BaseUDP_Manager, Verbose, "CDO constructor - skipping");
    }
}

// IN LISTEN PORT START
int32 UBS_BaseUDP_Manager::SetInListenPort(int32 NewInListenPort)
{
    // UE_LOGFMT(LogBS_BaseUDP_Manager, Verbose, "Setting InListenPort to {0}...", NewInListenPort);
    if (InListenPort != NewInListenPort)
    {
        InListenPort = NewInListenPort;
        SetInListenPortUDP_Message.Data = BS_ByteParser::ToByteArray<uint16>(InListenPort);
        UE_LOGFMT(LogBS_BaseUDP_Manager, Verbose, "Updated InListenPort to {0}", InListenPort);
    }
    else
    {
        UE_LOGFMT(LogBS_BaseUDP_Manager, Verbose, "Redundant InListenPort assignment {0}", InListenPort);
    }
    return InListenPort;
}
// IN LISTEN PORT END

// PING START
void UBS_BaseUDP_Manager::Ping()
{
    UE_LOGFMT(LogBS_BaseUDP_Manager, Verbose, "Pinging Server...");
    SendUDP_Messages({GetPingMessage()}, true);
}

const FBS_UDP_Message &UBS_BaseUDP_Manager::GetPingMessage()
{
    return bDidSendSetInListenPortMessage ? PingUDP_Message : SetInListenPortUDP_Message;
};

const FBS_UDP_Message UBS_BaseUDP_Manager::PingUDP_Message = {EBS_UDP_MessageType::PING};
// PING END

// MESSAGE START
void UBS_BaseUDP_Manager::SendUDP_Messages(const TArray<FBS_UDP_Message> &UDP_Messages, bool bSendImmediately)
{
    UE_LOGFMT(LogBS_BaseUDP_Manager, Verbose, "Requesting to send {0} Messages...", UDP_Messages.Num());
    PendingUDP_Messages.Append(UDP_Messages);
    if (!bSendImmediately)
    {
        UE_LOGFMT(LogBS_BaseUDP_Manager, Verbose, "Not sending data immediately");
        return;
    }
    if (bIsSendingUDP_Data)
    {
        UE_LOGFMT(LogBS_BaseUDP_Manager, Verbose, "Already sending data - will wait until new data is sent");
        return;
    }
    SendPendingUDP_Messages();
}
void UBS_BaseUDP_Manager::SendPendingUDP_Messages()
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
            UE_LOGFMT(LogBS_BaseUDP_Manager, Verbose, "Appending message {0} ({1} bytes)", UEnum::GetValueAsString(PendingUDP_Message.Type), PendingUDP_MessageLength);
            PendingUDP_Message.AppendTo(UDP_Data);
            PendingUDP_Messages.RemoveAt(PendingUDP_MessageIndex);
        }
        else
        {
            UE_LOGFMT(LogBS_BaseUDP_Manager, Verbose, "Skipping message {0} ({1} bytes)", UEnum::GetValueAsString(PendingUDP_Message.Type), PendingUDP_MessageLength);
            PendingUDP_MessageIndex++;
        }
    }

    if (UDP_Data.IsEmpty())
    {
        UE_LOGFMT(LogBS_BaseUDP_Manager, Verbose, "UDP_Data is Empty - won't send any data");
        bIsSendingUDP_Data = false;
        return;
    }

    UE_LOGFMT(LogBS_BaseUDP_Manager, Verbose, "Sending {0} bytes", UDP_Data.Num());

    SendUDP_Data(UDP_Data);
}
// MESSAGE END

// PARSING START
void UBS_BaseUDP_Manager::OnUDP_Message(EBS_UDP_MessageType MessageType, const TArray<uint8> &Message)
{
    UE_LOGFMT(LogBS_BaseUDP_Manager, Verbose, "message {0} ({1} bytes)", UEnum::GetValueAsString(MessageType), Message.Num());

    switch (MessageType)
    {
    case EBS_UDP_MessageType::PING:
        break;
    case EBS_UDP_MessageType::PONG:
        break;
    case EBS_UDP_MessageType::SET_REMOTE_RECEIVE_PORT:
        OnSetRemoteReceivePortMessage(Message);
        break;
    case EBS_UDP_MessageType::SERVER_MESSAGE:
        break;
    default:
        UE_LOGFMT(LogBS_BaseUDP_Manager, Error, "Uncaught MessageType {0}", UEnum::GetValueAsString(MessageType));
        break;
    }
}

void UBS_BaseUDP_Manager::OnSetRemoteReceivePortMessage(const TArray<uint8> &Message)
{
    const uint16 ParsedRemoteReceivePort = BS_ByteParser::ParseAs<uint16>(Message);
    UE_LOGFMT(LogBS_BaseUDP_Manager, Verbose, "Parsed RemoteReceivePort: {0}", ParsedRemoteReceivePort);

    if (ParsedRemoteReceivePort != InListenPort)
    {
        UE_LOGFMT(LogBS_BaseUDP_Manager, Error, "Incorrect RemoteReceivePort (expected {0}, got {1})", InListenPort, ParsedRemoteReceivePort);
        return;
    }

    UE_LOGFMT(LogBS_BaseUDP_Manager, Verbose, "Successfully set RemoteReceivePort");

    bDidSendSetInListenPortMessage = true;
}
// PARSING END