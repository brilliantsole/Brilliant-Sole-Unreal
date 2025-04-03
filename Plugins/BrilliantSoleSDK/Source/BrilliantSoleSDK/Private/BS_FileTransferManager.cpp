// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_FileTransferManager.h"
#include "BS_ByteParser.h"
#include "BS_TxRxMessageType.h"
#include "BS_TxMessage.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(LogBS_FileTransferManager);

bool UBS_FileTransferManager::OnRxMessage(EBS_TxRxMessage MessageType, const TArrayView<const uint8> &Message)
{
    switch (MessageType)
    {
    case EBS_TxRxMessage::GET_MAX_FILE_LENGTH:
        ParseMaxFileLength(Message);
        break;
    case EBS_TxRxMessage::GET_FILE_TRANSFER_TYPE:
    case EBS_TxRxMessage::SET_FILE_TRANSFER_TYPE:
        ParseFileType(Message);
        break;
    case EBS_TxRxMessage::GET_FILE_LENGTH:
    case EBS_TxRxMessage::SET_FILE_LENGTH:
        ParseFileLength(Message);
        break;
    case EBS_TxRxMessage::GET_FILE_CHECKSUM:
    case EBS_TxRxMessage::SET_FILE_CHECKSUM:
        ParseFileChecksum(Message);
        break;
    case EBS_TxRxMessage::GET_FILE_TRANSFER_STATUS:
        ParseFileTransferStatus(Message);
        break;
    case EBS_TxRxMessage::GET_FILE_TRANSFER_BLOCK:
        ParseFileTransferBlock(Message);
        break;
    case EBS_TxRxMessage::SET_FILE_TRANSFER_BLOCK:
        break;
    case EBS_TxRxMessage::FILE_BYTES_TRANSFERRED:
        ParseFileBytesTransferred(Message);
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
    FileToReceive.Empty();
    FileBlockToSend.Reset();
    BytesTransferred = 0;
    bWaitingToSendMoreData = false;
}

// MAX FILE LENGTH START
void UBS_FileTransferManager::ParseMaxFileLength(const TArrayView<const uint8> &Message)
{
    UE_LOGFMT(LogBS_FileTransferManager, Verbose, "Parsing MaxFileLength...");
    MaxFileLength = BS_ByteParser::ParseAs<uint32>(Message, 0, true);
    UE_LOGFMT(LogBS_FileTransferManager, Verbose, "Parsed MaxFileLength: {0}", MaxFileLength);
    OnMaxFileLengthUpdate.ExecuteIfBound(MaxFileLength);
}
// MAX FILE LENGTH END

// FILE TRANSFER TYPE START
void UBS_FileTransferManager::ParseFileType(const TArrayView<const uint8> &Message)
{
    FileType = static_cast<EBS_FileType>(Message[0]);
    UE_LOGFMT(LogBS_FileTransferManager, Verbose, "Parsed FileType: {0}", UEnum::GetValueAsString(FileType));
    OnFileTypeUpdate.ExecuteIfBound(FileType);
}

void UBS_FileTransferManager::SetFileType(const EBS_FileType NewFileType, bool bSendImmediately)
{
    if (NewFileType == FileType)
    {
        UE_LOGFMT(LogBS_FileTransferManager, Verbose, "Redundant FileType - not setting");
        return;
    }
    UE_LOGFMT(LogBS_FileTransferManager, Verbose, "Setting FileType to {0}...", UEnum::GetValueAsString(NewFileType));

    const TArray<uint8> TxMessage = {static_cast<uint8>(NewFileType)};
    SendTxMessages.ExecuteIfBound({{EBS_TxRxMessage::SET_FILE_TRANSFER_TYPE, TxMessage}}, bSendImmediately);
}
// FILE TRANSFER TYPE END

// FILE LENGTH START
void UBS_FileTransferManager::ParseFileLength(const TArrayView<const uint8> &Message)
{
    UE_LOGFMT(LogBS_FileTransferManager, Verbose, "Parsing FileLength...");
    FileLength = BS_ByteParser::ParseAs<uint32>(Message, 0, true);
    UE_LOGFMT(LogBS_FileTransferManager, Verbose, "Parsed FileLength: {0}", FileLength);
    OnFileLengthUpdate.ExecuteIfBound(FileLength);

    if (FileTransferStatus == EBS_FileTransferStatus::RECEIVING)
    {
        FileToReceive.Reset(FileLength);
    }
}

void UBS_FileTransferManager::SetFileLength(const uint32 NewFileLength, bool bSendImmediately)
{
    if (NewFileLength == FileLength)
    {
        UE_LOGFMT(LogBS_FileTransferManager, Verbose, "Redundant FileLength - not setting");
        return;
    }
    UE_LOGFMT(LogBS_FileTransferManager, Verbose, "Updating FileLength to {0}", NewFileLength);
    const TArray<uint8> TxMessage = BS_ByteParser::ToByteArray(NewFileLength, true);
    SendTxMessages.ExecuteIfBound({{EBS_TxRxMessage::SET_FILE_LENGTH, TxMessage}}, bSendImmediately);
}
// FILE LENGTH END

// FILE CHECKSUM START
void UBS_FileTransferManager::ParseFileChecksum(const TArrayView<const uint8> &Message)
{
    UE_LOGFMT(LogBS_FileTransferManager, Verbose, "Parsing FileChecksum...");
    FileChecksum = BS_ByteParser::ParseAs<uint32>(Message, 0, true);
    UE_LOGFMT(LogBS_FileTransferManager, Verbose, "Parsed FileChecksum: {0}", FileChecksum);
    OnFileChecksumUpdate.ExecuteIfBound(FileChecksum);
}

void UBS_FileTransferManager::SetFileChecksum(const uint32 NewFileChecksum, bool bSendImmediately)
{
    if (NewFileChecksum == FileChecksum)
    {
        UE_LOGFMT(LogBS_FileTransferManager, Verbose, "Redundant FileChecksum - not setting");
        return;
    }
    UE_LOGFMT(LogBS_FileTransferManager, Verbose, "Updating FileChecksum to {0}", NewFileChecksum);
    const TArray<uint8> TxMessage = BS_ByteParser::ToByteArray(NewFileChecksum, true);
    SendTxMessages.ExecuteIfBound({{EBS_TxRxMessage::SET_FILE_CHECKSUM, TxMessage}}, bSendImmediately);
}
// FILE CHECKSUM END

// FILE TRANSFER COMMAND START
void UBS_FileTransferManager::SetFileTransferCommand(const EBS_FileTransferCommand NewFileTransferCommand, bool bSendImmediately)
{
    UE_LOGFMT(LogBS_FileTransferManager, Verbose, "Setting FileTransferCommand to {0}...", UEnum::GetValueAsString(NewFileTransferCommand));

    const TArray<uint8> TxMessage = {static_cast<uint8>(NewFileTransferCommand)};
    SendTxMessages.ExecuteIfBound({{EBS_TxRxMessage::SET_FILE_TRANSFER_COMMAND, TxMessage}}, bSendImmediately);
}
// FILE TRANSFER COMMAND END

// FILE TRANSFER STATUS START
void UBS_FileTransferManager::ParseFileTransferStatus(const TArrayView<const uint8> &Message)
{
    FileTransferStatus = static_cast<EBS_FileTransferStatus>(Message[0]);
    UE_LOGFMT(LogBS_FileTransferManager, Verbose, "Parsed FileTransferStatus: {0}", UEnum::GetValueAsString(FileTransferStatus));
    OnFileTransferStatusUpdate.ExecuteIfBound(FileTransferStatus);

    BytesTransferred = 0;

    if (FileTransferStatus == EBS_FileTransferStatus::SENDING)
    {
        UE_LOGFMT(LogBS_FileTransferManager, Verbose, "Starting to send file...");
        SendFileBlock(true);
    }
}
// FILE TRANSFER STATUS END

// FILE BLOCK START

uint32 UBS_FileTransferManager::GetCRC32(const TArray<uint8> &Data)
{
    const uint32 Checksum = FCrc::MemCrc32(Data.GetData(), Data.Num());
    UE_LOGFMT(LogBS_FileTransferManager, Verbose, "Checksum: {0}", Checksum);
    return Checksum;
}

void UBS_FileTransferManager::SendFile(const EBS_FileType NewFileType, const TArray<uint8> &File)
{
    if (FileTransferStatus != EBS_FileTransferStatus::IDLE)
    {
        UE_LOGFMT(LogBS_FileTransferManager, Warning, "Can only send files when Idle");
        return;
    }

    UE_LOGFMT(LogBS_FileTransferManager, Verbose, "Requesting to send File with {0} bytes...", File.Num());

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
        UE_LOGFMT(LogBS_FileTransferManager, Verbose, "Already Idle");
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

    const uint32 FileToSendLength = FileToSend.Num();

    const uint32 RemainingBytes = FileToSendLength - BytesTransferred;
    UE_LOGFMT(LogBS_FileTransferManager, Verbose, "RemainingBytes: {0}", RemainingBytes);

    const float FileTransferProgress = static_cast<float>(BytesTransferred) / static_cast<float>(FileToSendLength);
    UE_LOGFMT(LogBS_FileTransferManager, Verbose, "FileTransferProgress: {0}%", FileTransferProgress * 100.0f);
    OnFileTransferProgress.ExecuteIfBound(FileType, EBS_FileTransferDirection::SENDING, FileTransferProgress);

    if (RemainingBytes == 0)
    {
        UE_LOGFMT(LogBS_FileTransferManager, Verbose, "File Transfer Complete");
        OnFileTransferComplete.ExecuteIfBound(FileType, EBS_FileTransferDirection::SENDING);
        bWaitingToSendMoreData = false;
        return;
    }

    bWaitingToSendMoreData = true;

    const uint32 MaxMessageLength = MTU - 3 - 3;
    const uint32 FileBlockLength = FMath::Min(RemainingBytes, MaxMessageLength);
    UE_LOGFMT(LogBS_FileTransferManager, Verbose, "MaxMessageLength: {0}, FileBlockLength: {1}", MaxMessageLength, FileBlockLength);

    FileBlockToSend.SetNumUninitialized(FileBlockLength);
    uint8 *FileToSendPtr = FileToSend.GetData() + BytesTransferred;
    uint8 *FileBlockToSendPtr = FileBlockToSend.GetData();
    FMemory::Memcpy(FileBlockToSendPtr, FileToSendPtr, FileBlockLength * sizeof(uint8));

    BytesTransferred += FileBlockLength;
    UE_LOGFMT(LogBS_FileTransferManager, Verbose, "BytesTransferred: {0}", BytesTransferred);

    SendTxMessages.ExecuteIfBound({{EBS_TxRxMessage::SET_FILE_TRANSFER_BLOCK, FileBlockToSend}}, bSendImmediately);
}
void UBS_FileTransferManager::ParseFileTransferBlock(const TArrayView<const uint8> &Message)
{
    if (FileTransferStatus != EBS_FileTransferStatus::RECEIVING)
    {
        UE_LOGFMT(LogBS_FileTransferManager, Warning, "Cannot parse file block when not receiving");
        return;
    }

    const uint16 FileBlockLength = Message.Num();
    UE_LOGFMT(LogBS_FileTransferManager, Verbose, "Received File Block of {0} bytes", FileBlockLength);

    uint32 CurrentFileLength = FileToReceive.Num();
    uint32 NewFileLength = CurrentFileLength + FileBlockLength;
    UE_LOGFMT(LogBS_FileTransferManager, Verbose, "Updating FileToReceive from {0} to {1} bytes...", CurrentFileLength, NewFileLength);
    if (NewFileLength > FileLength)
    {
        UE_LOGFMT(LogBS_FileTransferManager, Verbose, "New File length {0} is larger than expected File Length {1}", NewFileLength, FileLength);
        CancelFileTransfer();
        return;
    }
    FileToReceive.Append(Message);
    CurrentFileLength = FileToReceive.Num();
    UE_LOGFMT(LogBS_FileTransferManager, Verbose, "FileToReceive length: {0}/{1} bytes", CurrentFileLength, FileLength);

    if (CurrentFileLength == FileLength)
    {
        UE_LOGFMT(LogBS_FileTransferManager, Verbose, "Finished Receiving File");
        const uint32 ReceivedFileChecksum = GetCRC32(FileToReceive);
        if (ReceivedFileChecksum != FileChecksum)
        {
            UE_LOGFMT(LogBS_FileTransferManager, Error, "File Checksums don't match - expected {0}, got {1}", FileChecksum, ReceivedFileChecksum);
            return;
        }
        UE_LOGFMT(LogBS_FileTransferManager, Verbose, "File Checksums match, {0}", FileChecksum);
        OnFileTransferComplete.ExecuteIfBound(FileType, EBS_FileTransferDirection::RECEIVING);
        OnFileReceived.ExecuteIfBound(FileType, FileToReceive);
    }
    else
    {
        const TArray<uint8> TxMessage = BS_ByteParser::ToByteArray(CurrentFileLength, true);
        SendTxMessages.ExecuteIfBound({{EBS_TxRxMessage::FILE_BYTES_TRANSFERRED, TxMessage}}, false);
    }
}
void UBS_FileTransferManager::ParseFileBytesTransferred(const TArrayView<const uint8> &Message)
{
    if (FileTransferStatus != EBS_FileTransferStatus::SENDING)
    {
        UE_LOGFMT(LogBS_FileTransferManager, Error, "Currently not sending file");
        return;
    }
    if (!bWaitingToSendMoreData)
    {
        UE_LOGFMT(LogBS_FileTransferManager, Error, "Not waiting to send more data");
        return;
    }

    uint32 CurrentBytesTransferred = BS_ByteParser::ParseAs<uint32>(Message, 0, true);
    UE_LOGFMT(LogBS_FileTransferManager, Verbose, "CurrentBytesTransferred: {0}", CurrentBytesTransferred);

    if (BytesTransferred != CurrentBytesTransferred)
    {
        UE_LOGFMT(LogBS_FileTransferManager, Error, "BytesTransferred not equal - got {0}, expected {1}", CurrentBytesTransferred, BytesTransferred);
        CancelFileTransfer();
        return;
    }

    UE_LOGFMT(LogBS_FileTransferManager, Verbose, "Sending more File Blocks...");
    SendFileBlock(true);
}

// FILE BLOCK END