// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_InformationManager.h"
#include "Logging/StructuredLog.h"
#include "BS_ByteParser.h"
#include "BS_Message.h"
#include "Misc/DateTime.h"

DEFINE_LOG_CATEGORY(LogBS_InformationManager);

bool UBS_InformationManager::OnRxMessage(uint8 MessageType, const TArray<uint8> &Message)
{
    switch (MessageType)
    {
    case BS_MessageGetMTU:
        ParseMTU(Message);
        break;
    case BS_MessageGetId:
        ParseId(Message);
        break;
    case BS_MessageGetName:
    case BS_MessageSetName:
        ParseName(Message);
        break;
    case BS_MessageGetType:
    case BS_MessageSetType:
        ParseType(Message);
        break;
    case BS_MessageGetCurrentTime:
    case BS_MessageSetCurrentTime:
        ParseCurrentTime(Message);
        break;
    default:
        return false;
        break;
    }

    return true;
}

void UBS_InformationManager::Reset()
{
    MTU = 0;
    Id = "";
    Name = "";
    Type = EBS_DeviceType::LEFT_INSOLE;
}

void UBS_InformationManager::ParseMTU(const TArray<uint8> &Message)
{
    MTU = ByteParser::GetUint16(Message);
    UE_LOGFMT(LogBS_InformationManager, Log, "Parsed MTU: {0}", MTU);
    OnMTU_Update.ExecuteIfBound(MTU);
}

void UBS_InformationManager::ParseId(const TArray<uint8> &Message)
{
    Id = ByteParser::GetString(Message);
    UE_LOGFMT(LogBS_InformationManager, Log, "Parsed Id: {0} ({1} bytes)", Id, Message.Num());
    OnIdUpdate.ExecuteIfBound(Id);
}

void UBS_InformationManager::ParseName(const TArray<uint8> &Message)
{
    Name = ByteParser::GetString(Message);
    UE_LOGFMT(LogBS_InformationManager, Log, "Parsed Name: {0}", Name);
    OnNameUpdate.ExecuteIfBound(Name);
}

void UBS_InformationManager::ParseType(const TArray<uint8> &Message)
{
    Type = (EBS_DeviceType)Message[0];
    UE_LOGFMT(LogBS_InformationManager, Log, "Parsed Type: {0}", UEnum::GetValueAsString(Type));
    OnTypeUpdate.ExecuteIfBound(Type);
}

void UBS_InformationManager::ParseCurrentTime(const TArray<uint8> &Message)
{
    CurrentTime = ByteParser::GetUint64(Message);
    UE_LOGFMT(LogBS_InformationManager, Log, "Parsed CurrentTime: {0}", CurrentTime);
    if (CurrentTime == 0)
    {
        FDateTime Now = FDateTime::UtcNow();
        FDateTime UnixEpoch(1970, 1, 1);
        FTimespan Timespan = Now - UnixEpoch;
        uint64 Milliseconds = static_cast<uint64>(Timespan.GetTotalMilliseconds());
        UE_LOGFMT(LogBS_InformationManager, Log, "Updating CurrentTime to {0}", Milliseconds);
        const TArray<uint8> TxMessage = ByteParser::Uint64AsArray(Milliseconds);
        SendTxMessages.ExecuteIfBound({{BS_MessageSetCurrentTime, TxMessage}}, true);
    }
    else
    {
        OnCurrentTimeUpdate.ExecuteIfBound(CurrentTime);
    }
}