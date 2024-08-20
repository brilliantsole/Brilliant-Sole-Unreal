// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_InformationManager.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(LogBS_InformationManager);

bool UBS_InformationManager::OnRxMessage(uint8 MessageType, const TArray<uint8> &Message)
{
    return false;
}

void UBS_InformationManager::Reset()
{
    MTU = 0;
}