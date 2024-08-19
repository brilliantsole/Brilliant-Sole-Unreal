// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_DeviceInformationManager.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(LogBS_DeviceInformationManager);

bool UBS_DeviceInformationManager::OnRxMessage(uint8 MessageType, const TArray<uint8> &Message)
{
    return false;
}