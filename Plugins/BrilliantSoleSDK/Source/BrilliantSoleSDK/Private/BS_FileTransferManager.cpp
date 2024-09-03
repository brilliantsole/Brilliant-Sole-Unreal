// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_FileTransferManager.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(LogBS_FileTransferManager);

bool UBS_FileTransferManager::OnRxMessage(uint8 MessageType, const TArray<uint8> &Message)
{
    switch (MessageType)
    {
        // FILL
    default:
        return false;
    }
    return true;
}