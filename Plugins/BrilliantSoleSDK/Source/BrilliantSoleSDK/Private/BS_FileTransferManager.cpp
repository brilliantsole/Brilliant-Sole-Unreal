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
        ParseMaxFileLength(Message);
        break;
    case BS_MessageGetFileTransferType:
    case BS_MessageSetFileTransferType:
        ParseFileType(Message);
        break;
    case BS_MessageGetFileLength:
    case BS_MessageSetFileLength:
        ParseFileLength(Message);
        break;
    case BS_MessageGetFileChecksum:
    case BS_MessageSetFileChecksum:
        ParseFileChecksum(Message);
        break;
    case BS_MessageGetFileTransferStatus:
        ParseFileTransferStatus(Message);
        break;
    case BS_MessageGetFileTransferBlock:
        ParseFileTransferBlock(Message);
        break;
    case BS_MessageSetFileTransferBlock:
        break;
    default:
        return false;
    }
    return true;
}

void UBS_FileTransferManager::Reset()
{
    MTU = 0;
    MaxFileLength = 0;
    FileType = EBS_FileType::TFLITE;
    FileLength = 0;
    FileChecksum = 0;
    FileTransferStatus = EBS_FileTransferStatus::IDLE;
    FileToSend.Empty();
    FileBlockToSend.Reset();
    BytesTransferred = 0;
    bWaitingToSendMoreData = false;
}

void UBS_FileTransferManager::OnSendTxData()
{
    if (bWaitingToSendMoreData && FileTransferStatus == EBS_FileTransferStatus::SENDING)
    {
        UE_LOGFMT(LogBS_FileTransferManager, Log, "Sending more File Blocks...");
        SendFileBlock(false);
    }
}

// MAX FILE LENGTH START
void UBS_FileTransferManager::ParseMaxFileLength(const TArray<uint8> &Message)
{
    UE_LOGFMT(LogBS_FileTransferManager, Log, "Parsing MaxFileLength...");
    MaxFileLength = BS_ByteParser::ParseAs<uint32>(Message, 0, true);
    UE_LOGFMT(LogBS_FileTransferManager, Log, "Parsed MaxFileLength: {0}", MaxFileLength);
    OnMaxFileLengthUpdate.ExecuteIfBound(MaxFileLength);
}
// MAX FILE LENGTH END

// FILE TRANSFER TYPE START
void UBS_FileTransferManager::ParseFileType(const TArray<uint8> &Message)
{
    FileType = static_cast<EBS_FileType>(Message[0]);
    UE_LOGFMT(LogBS_FileTransferManager, Log, "Parsed FileType: {0}", UEnum::GetValueAsString(FileType));
    OnFileTypeUpdate.ExecuteIfBound(FileType);
}

void UBS_FileTransferManager::SetFileType(const EBS_FileType NewFileType, bool bSendImmediately)
{
    if (NewFileType == FileType)
    {
        UE_LOGFMT(LogBS_FileTransferManager, Log, "Redundant FileType - not setting");
        return;
    }
    UE_LOGFMT(LogBS_FileTransferManager, Log, "Setting FileType to {0}...", UEnum::GetValueAsString(NewFileType));

    const TArray<uint8> TxMessage = {static_cast<uint8>(NewFileType)};
    SendTxMessages.ExecuteIfBound({{BS_MessageSetFileTransferType, TxMessage}}, bSendImmediately);
}
// FILE TRANSFER TYPE END

// FILE LENGTH START
void UBS_FileTransferManager::ParseFileLength(const TArray<uint8> &Message)
{
    UE_LOGFMT(LogBS_FileTransferManager, Log, "Parsing FileLength...");
    FileLength = BS_ByteParser::ParseAs<uint32>(Message, 0, true);
    UE_LOGFMT(LogBS_FileTransferManager, Log, "Parsed FileLength: {0}", FileLength);
    OnFileLengthUpdate.ExecuteIfBound(FileLength);
}

void UBS_FileTransferManager::SetFileLength(const uint32 NewFileLength, bool bSendImmediately)
{
    if (NewFileLength == FileLength)
    {
        UE_LOGFMT(LogBS_FileTransferManager, Log, "Redundant FileLength - not setting");
        return;
    }
    UE_LOGFMT(LogBS_FileTransferManager, Log, "Updating FileLength to {0}", NewFileLength);
    const TArray<uint8> TxMessage = BS_ByteParser::ToByteArray(NewFileLength);
    SendTxMessages.ExecuteIfBound({{BS_MessageSetFileLength, TxMessage}}, bSendImmediately);
}
// FILE LENGTH END

// FILE CHECKSUM START
void UBS_FileTransferManager::ParseFileChecksum(const TArray<uint8> &Message)
{
    UE_LOGFMT(LogBS_FileTransferManager, Log, "Parsing FileChecksum...");
    FileChecksum = BS_ByteParser::ParseAs<uint32>(Message, 0, true);
    UE_LOGFMT(LogBS_FileTransferManager, Log, "Parsed FileChecksum: {0}", FileChecksum);
    OnFileChecksumUpdate.ExecuteIfBound(FileChecksum);
}

void UBS_FileTransferManager::SetFileChecksum(const uint32 NewFileChecksum, bool bSendImmediately)
{
    if (NewFileChecksum == FileChecksum)
    {
        UE_LOGFMT(LogBS_FileTransferManager, Log, "Redundant FileChecksum - not setting");
        return;
    }
    UE_LOGFMT(LogBS_FileTransferManager, Log, "Updating FileChecksum to {0}", NewFileChecksum);
    const TArray<uint8> TxMessage = BS_ByteParser::ToByteArray(NewFileChecksum);
    SendTxMessages.ExecuteIfBound({{BS_MessageSetFileChecksum, TxMessage}}, bSendImmediately);
}
// FILE CHECKSUM END

// FILE TRANSFER COMMAND START
void UBS_FileTransferManager::SetFileTransferCommand(const EBS_FileTransferCommand NewFileTransferCommand, bool bSendImmediately)
{
    UE_LOGFMT(LogBS_FileTransferManager, Log, "Setting FileTransferCommand to {0}...", UEnum::GetValueAsString(NewFileTransferCommand));

    const TArray<uint8> TxMessage = {static_cast<uint8>(NewFileTransferCommand)};
    SendTxMessages.ExecuteIfBound({{BS_MessageSetFileTransferCommand, TxMessage}}, bSendImmediately);
}
// FILE TRANSFER COMMAND END

// FILE TRANSFER STATUS START
void UBS_FileTransferManager::ParseFileTransferStatus(const TArray<uint8> &Message)
{
    FileTransferStatus = static_cast<EBS_FileTransferStatus>(Message[0]);
    UE_LOGFMT(LogBS_FileTransferManager, Log, "Parsed FileTransferStatus: {0}", UEnum::GetValueAsString(FileTransferStatus));
    OnFileTransferStatusUpdate.ExecuteIfBound(FileTransferStatus);

    BytesTransferred = 0;

    if (FileTransferStatus == EBS_FileTransferStatus::SENDING)
    {
        UE_LOGFMT(LogBS_FileTransferManager, Log, "Starting to send file...");
        SendFileBlock(true);
    }
}
// FILE TRANSFER STATUS END

// FILE BLOCK START

uint32 UBS_FileTransferManager::GetCRC32(const TArray<uint8> &Data)
{
    const uint32 Checksum = FCrc::MemCrc32(Data.GetData(), Data.Num());
    UE_LOGFMT(LogBS_FileTransferManager, Log, "Checksum: {0}", Checksum);
    return Checksum;
}

void UBS_FileTransferManager::SendFile(const EBS_FileType NewFileType, const TArray<uint8> &File)
{
    if (FileTransferStatus != EBS_FileTransferStatus::IDLE)
    {
        UE_LOGFMT(LogBS_FileTransferManager, Warning, "Can only send files when Idle");
        return;
    }

    UE_LOGFMT(LogBS_FileTransferManager, Log, "Requestig to send File with {0} bytes...", File.Num());

    FileToSend = File;

    SetFileType(NewFileType, false);
    SetFileLength(FileToSend.Num(), false);
    SetFileChecksum(GetCRC32(FileToSend), false);
    SetFileTransferCommand(EBS_FileTransferCommand::SEND);
}
void UBS_FileTransferManager::ReceiveFile(const EBS_FileType NewFileType)
{
    if (FileTransferStatus != EBS_FileTransferStatus::IDLE)
    {
        UE_LOGFMT(LogBS_FileTransferManager, Warning, "Can only receive files when Idle");
        return;
    }
    SetFileType(NewFileType, false);
    SetFileTransferCommand(EBS_FileTransferCommand::RECEIVE);
}
void UBS_FileTransferManager::CancelFileTransfer()
{
    if (FileTransferStatus == EBS_FileTransferStatus::IDLE)
    {
        UE_LOGFMT(LogBS_FileTransferManager, Log, "Already Idle");
        return;
    }
    SetFileTransferCommand(EBS_FileTransferCommand::CANCEL);
}

void UBS_FileTransferManager::SendFileBlock(bool bSendImmediately)
{
    if (FileTransferStatus != EBS_FileTransferStatus::SENDING)
    {
        UE_LOGFMT(LogBS_FileTransferManager, Error, "Cannot send file block when not sending");
        return;
    }
    if (FileToSend.Num() == 0)
    {
        UE_LOGFMT(LogBS_FileTransferManager, Error, "FileToSend is empty");
        return;
    }

    const uint16 FileToSendLength = FileToSend.Num();

    const uint16 RemainingBytes = FileToSendLength - BytesTransferred;
    UE_LOGFMT(LogBS_FileTransferManager, Log, "RemainingBytes: {0}", RemainingBytes);

    const float FileTransferProgress = static_cast<float>(BytesTransferred) / static_cast<float>(FileToSendLength);
    UE_LOGFMT(LogBS_FileTransferManager, Log, "FileTransferProgress: {0}%", FileTransferProgress * 100.0f);
    OnFileTransferProgress.ExecuteIfBound(FileType, EBS_FileTransferDirection::SENDING, FileTransferProgress);

    if (RemainingBytes == 0)
    {
        UE_LOGFMT(LogBS_FileTransferManager, Log, "File Transfer Complete");
        OnFileTransferComplete.ExecuteIfBound(FileType, EBS_FileTransferDirection::SENDING);
        bWaitingToSendMoreData = false;
        return;
    }

    bWaitingToSendMoreData = true;

    const uint16 MaxMessageLength = MTU - 3 - 3;
    const uint16 FileBlockLength = FMath::Min(RemainingBytes, MaxMessageLength);
    UE_LOGFMT(LogBS_FileTransferManager, Log, "MaxMessageLength: {0}, FileBlockLength: {1}", MaxMessageLength, FileBlockLength);

    FileBlockToSend.SetNumUninitialized(FileBlockLength);
    uint8 *FileToSendPtr = FileToSend.GetData() + BytesTransferred;
    uint8 *FileBlockToSendPtr = FileBlockToSend.GetData();
    FMemory::Memcpy(FileBlockToSendPtr, FileToSendPtr, FileBlockLength * sizeof(uint8));

    BytesTransferred += FileBlockLength;
    UE_LOGFMT(LogBS_FileTransferManager, Log, "BytesTransferred: {0}", BytesTransferred);

    SendTxMessages.ExecuteIfBound({{BS_MessageSetFileTransferBlock, FileBlockToSend}}, bSendImmediately);
}
void UBS_FileTransferManager::ParseFileTransferBlock(const TArray<uint8> &Message)
{
    if (FileTransferStatus != EBS_FileTransferStatus::RECEIVING)
    {
        UE_LOGFMT(LogBS_FileTransferManager, Warning, "Cannot parse file block when not receiving");
        return;
    }

    UE_LOGFMT(LogBS_FileTransferManager, Log, "Received File Block of {0} bytes", Message.Num());

    // FILL
}
// FILE BLOCK END