// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_FileTransferManager.h"
#include "BS_ByteParser.h"
#include "BS_Message.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(LogBS_FileTransferManager);

bool UBS_FileTransferManager::OnRxMessage(uint8 MessageType, const TArray<uint8> &Message)
{
    switch (MessageType)
    {
    case BS_MessageGetMaxFileLength:
        // ParseMaxFileLength(Message);
        break;
    case BS_MessageGetFileTransferType:
    case BS_MessageSetFileTransferType:
        // ParseFileTransferType(Message);
        break;
    case BS_MessageGetFileLength:
    case BS_MessageSetFileLength:
        // ParseFileLength(Message);
        break;
    case BS_MessageGetFileChecksum:
    case BS_MessageSetFileChecksum:
        // ParseFileChecksum(Message);
        break;
    case BS_MessageGetFileTransferStatus:
        // ParseFileTransferStatus(Message);
        break;
    case BS_MessageGetFileTransferBlock:
    case BS_MessageSetFileTransferBlock:
        // ParseFileTransferBlock(Message);
        break;
    default:
        return false;
    }
    return true;
}

void UBS_FileTransferManager::Reset()
{
    // FILL
}