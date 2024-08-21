// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_InformationManager.h"
#include "Logging/StructuredLog.h"
#include "BS_ByteParser.h"
#include "BS_Message.h"

DEFINE_LOG_CATEGORY(LogBS_InformationManager);

bool UBS_InformationManager::OnRxMessage(uint8 MessageType, const TArray<uint8> &Message)
{
    switch (MessageType)
    {
    case BS_MessageGetMTU:
        ParseMTU(Message);
        return true;
        break;
    case BS_MessageGetId:
        return true;
        break;
    case BS_MessageGetName:
    case BS_MessageSetName:
        return true;
        break;
    case BS_MessageGetType:
    case BS_MessageSetType:
        return true;
        break;
    case BS_MessageGetCurrentTime:
    case BS_MessageSetCurrentTime:
        return true;
        break;
    default:
        return false;
        break;
    }
}

void UBS_InformationManager::Reset()
{
    MTU = 0;
}

void UBS_InformationManager::ParseMTU(const TArray<uint8> &Message)
{
    MTU = ByteParser::GetUint16(Message);
    UE_LOGFMT(LogBS_InformationManager, Log, "Parsed MTU: {0}", MTU);
}