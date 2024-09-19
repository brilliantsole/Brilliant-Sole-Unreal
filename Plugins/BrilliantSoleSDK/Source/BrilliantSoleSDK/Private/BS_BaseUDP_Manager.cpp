// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_BaseUDP_Manager.h"
#include "Logging/StructuredLog.h"
#include "BS_Message.h"
#include "BS_Subsystem.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
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

    SetInListenPortMessage.SetNum(3);
    SetInListenPortMessage[0] = static_cast<uint8>(EBS_UDP_MessageType::SET_REMOTE_RECEIVE_PORT);

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
    if (InListenPort != NewInListenPort)
    {
        InListenPort = NewInListenPort;
        SetInListenPortMessage[1] = static_cast<uint8>(InListenPort & 0xFF);
        SetInListenPortMessage[2] = static_cast<uint8>((InListenPort >> 8) & 0xFF);
    }
    return InListenPort;
}
// IN LISTEN PORT END

// PING START
TArray<uint8> UBS_BaseUDP_Manager::PingMessage = {static_cast<uint8>(EBS_UDP_MessageType::PING)};

const TArray<uint8> &UBS_BaseUDP_Manager::GetPingMessage()
{
    return bDidSendSetInListenPortMessage ? PingMessage : SetInListenPortMessage;
};

void UBS_BaseUDP_Manager::Ping()
{
    UE_LOGFMT(LogBS_BaseUDP_Manager, Verbose, "Pinging Server...");
    SendData(GetPingMessage());
}

// PING END