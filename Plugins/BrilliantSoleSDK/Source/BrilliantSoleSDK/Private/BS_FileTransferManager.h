// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BS_BaseManager.h"
#include "BS_FileTransferCommand.h"
#include "BS_FileTransferStatus.h"
#include "BS_FileTransferDirection.h"
#include "BS_FileType.h"
#include "Misc/Crc.h"
#include "BS_FileTransferManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_FileTransferManager, Log, All);

DECLARE_DELEGATE_OneParam(FBS_FileTransferFileLengthCallbackLocal, uint32);

#define EBS_FileTransferMessage BS_MessageGetMaxFileLength,       \
								BS_MessageGetFileTransferType,    \
								BS_MessageSetFileTransferType,    \
								BS_MessageGetFileLength,          \
								BS_MessageSetFileLength,          \
								BS_MessageGetFileChecksum,        \
								BS_MessageSetFileChecksum,        \
								BS_MessageSetFileTransferCommand, \
								BS_MessageGetFileTransferStatus,  \
								BS_MessageGetFileTransferBlock,   \
								BS_MessageSetFileTransferBlock

UCLASS()
class UBS_FileTransferManager : public UBS_BaseManager
{
	GENERATED_BODY()

public:
	bool OnRxMessage(uint8 MessageType, const TArray<uint8> &Message) override;
	void Reset() override;
	void OnSendTxData() override;

	// MAX FILE LENGTH START
public:
	uint32 GetMaxFileLength() const { return MaxFileLength; }
	FBS_FileTransferFileLengthCallbackLocal OnMaxFileLengthUpdate;

private:
	UPROPERTY()
	uint32 MaxFileLength = 0;
	void ParseMaxFileLength(const TArray<uint8> &Message);
	// MAX FILE LENGTH END

	// FILE TRANSFER TYPE START
public:
	const EBS_FileType GetFileType() const { return FileType; }
	DECLARE_DELEGATE_OneParam(FBS_FileTypeCallbackLocal, EBS_FileType);
	FBS_FileTypeCallbackLocal OnFileTypeUpdate;

	void SetFileType(const EBS_FileType NewFileType, bool bSendImmediately = true);

private:
	UPROPERTY()
	EBS_FileType FileType = EBS_FileType::TFLITE;
	void ParseFileType(const TArray<uint8> &Message);
	// FILE TRANSFER TYPE END

	// FILE LENGTH START
public:
	const uint32 GetFileLength() const { return FileLength; }
	FBS_FileTransferFileLengthCallbackLocal OnFileLengthUpdate;

	void SetFileLength(const uint32 NewFileLength, bool bSendImmediately = true);

private:
	UPROPERTY()
	uint32 FileLength = 0;
	void ParseFileLength(const TArray<uint8> &Message);
	// FILE LENGTH END

	// FiLE CHECKSUM START
public:
	const uint32 GetFileChecksum() const { return FileChecksum; }
	DECLARE_DELEGATE_OneParam(FBS_FileTransferChecksumCallbackLocal, uint32);
	FBS_FileTransferChecksumCallbackLocal OnFileChecksumUpdate;

	void SetFileChecksum(const uint32 NewFileChecksum, bool bSendImmediately = true);

private:
	UPROPERTY()
	uint32 FileChecksum = 0;
	void ParseFileChecksum(const TArray<uint8> &Message);
	// FiLE CHECKSUM END

	// FILE TRANSFER COMMAND START
public:
	void SetFileTransferCommand(const EBS_FileTransferCommand FileTransferCommand, bool bSendImmediately = true);
	// FILE TRANSFER COMMAND END

	// FILE TRANSFER STATUS START
public:
	const EBS_FileTransferStatus GetFileTransferStatus() const { return FileTransferStatus; }
	DECLARE_DELEGATE_OneParam(FBS_FileTransferStatusCallbackLocal, EBS_FileTransferStatus);
	FBS_FileTransferStatusCallbackLocal OnFileTransferStatusUpdate;

	DECLARE_DELEGATE_ThreeParams(FBS_FileTransferProgressLocal, EBS_FileType, EBS_FileTransferDirection, float);
	FBS_FileTransferProgressLocal OnFileTransferProgress;

	DECLARE_DELEGATE_TwoParams(FBS_FileReceivedLocal, EBS_FileType, const TArray<uint8> &Message);
	FBS_FileReceivedLocal OnFileReceived;

	DECLARE_DELEGATE_TwoParams(FBS_FileTransferCompleteLocal, EBS_FileType, EBS_FileTransferDirection);
	FBS_FileTransferCompleteLocal OnFileTransferComplete;

private:
	UPROPERTY()
	EBS_FileTransferStatus FileTransferStatus = EBS_FileTransferStatus::IDLE;
	void ParseFileTransferStatus(const TArray<uint8> &Message);
	// FILE TRANSFER STATUS END

	// FILE BLOCK START
public:
	UPROPERTY()
	uint16 MTU = 0;

	void SendFile(const EBS_FileType NewFileType, const TArray<uint8> &File);
	void ReceiveFile(const EBS_FileType NewFileType);
	void CancelFileTransfer();

private:
	uint32 GetCRC32(const TArray<uint8> &Data);

	UPROPERTY()
	TArray<uint8> FileToSend;
	UPROPERTY()
	TArray<uint8> FileBlockToSend;
	UPROPERTY()
	uint32 BytesTransferred = 0;

	UPROPERTY()
	bool bWaitingToSendMoreData = false;

	void SendFileBlock(bool bSendImmediately);

	void ParseFileTransferBlock(const TArray<uint8> &Message);
	// FILE BLOCK END
};
