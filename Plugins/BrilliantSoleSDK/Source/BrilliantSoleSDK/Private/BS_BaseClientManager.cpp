// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_BaseClientManager.h"
#include "Logging/StructuredLog.h"
#include "BS_Message.h"
#include "BS_Subsystem.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "BS_ByteParser.h"
#include "GenericPlatform/GenericPlatformTime.h"

DEFINE_LOG_CATEGORY(LogBS_BaseClientManager);

UBS_BaseClientManager::UBS_BaseClientManager()
{
    UE_LOGFMT(LogBS_BaseClientManager, Verbose, "Constructor: {0}", GetName());
    if (HasAnyFlags(RF_ClassDefaultObject))
    {
        UE_LOGFMT(LogBS_BaseClientManager, Verbose, "CDO - Skipping Constructor");
        return;
    }
}

void UBS_BaseClientManager::PostInitProperties()
{
    Super::PostInitProperties();
    UE_LOGFMT(LogBS_BaseClientManager, Verbose, "PostInitProperties {0}", GetName());
    if (HasAnyFlags(RF_ClassDefaultObject))
    {
        UE_LOGFMT(LogBS_BaseClientManager, Verbose, "CDO - Skipping");
        return;
    }

    GetBS_Subsystem();
    InitializeBP();
}

void UBS_BaseClientManager::GetBS_Subsystem()
{
    if (!HasAnyFlags(RF_ClassDefaultObject))
    {
        UWorld *World = GetWorld();
        if (World)
        {
            UE_LOGFMT(LogBS_BaseClientManager, Verbose, "World found");
            UGameInstance *GameInstance = World->GetGameInstance();
            if (GameInstance)
            {
                UE_LOGFMT(LogBS_BaseClientManager, Verbose, "GameInstance found");
                UBS_Subsystem *__BS_Subsystem = GameInstance->GetSubsystem<UBS_Subsystem>();
                if (__BS_Subsystem)
                {
                    UE_LOGFMT(LogBS_BaseClientManager, Verbose, "BS_Subsystem found");
                    _BS_Subsystem = __BS_Subsystem;
                }
                else
                {
                    UE_LOGFMT(LogBS_BaseClientManager, Error, "BS_Subsystem not found");
                }
            }
            else
            {
                UE_LOGFMT(LogBS_BaseClientManager, Error, "GameInstance not found");
            }
        }
        else
        {
            UE_LOGFMT(LogBS_BaseClientManager, Error, "World not found");
        }
    }
    else
    {
        UE_LOGFMT(LogBS_BaseClientManager, Verbose, "CDO constructor - skipping");
    }
}

// CONNECTION START
void UBS_BaseClientManager::SetConnectionStatus(EBS_ConnectionStatus NewConnectionStatus)
{
    if (ConnectionStatus == NewConnectionStatus)
    {
        UE_LOGFMT(LogBS_BaseClientManager, Verbose, "Redundant ConnectionStatus {0} - not updating", UEnum::GetValueAsString(ConnectionStatus));
        return;
    }
    ConnectionStatus = NewConnectionStatus;
    UE_LOGFMT(LogBS_BaseClientManager, Verbose, "ConnectionStatus Updated to {0}", UEnum::GetValueAsString(ConnectionStatus));
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