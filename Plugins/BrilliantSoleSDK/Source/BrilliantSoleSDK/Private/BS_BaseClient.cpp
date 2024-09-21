// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_BaseClient.h"
#include "Logging/StructuredLog.h"
#include "BS_Message.h"
#include "BS_Subsystem.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "BS_ByteParser.h"
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
        break;
    default:
        break;
    }
}
// CONNECTION END

// MESSAGE START
void UBS_BaseClient::OnMessage(EBS_ServerMessageType MessageType, const TArray<uint8> &Message)
{
    UE_LOGFMT(LogBS_BaseClient, Verbose, "message {0} ({1} bytes)", UEnum::GetValueAsString(MessageType), Message.Num());

    switch (MessageType)
    {
    default:
        UE_LOGFMT(LogBS_BaseClient, Error, "Uncaught MessageType {0}", UEnum::GetValueAsString(MessageType));
        break;
    }
}

void UBS_BaseClient::OnData(const TArray<uint8> &Data)
{
    // FILL
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
// MESSAGE END