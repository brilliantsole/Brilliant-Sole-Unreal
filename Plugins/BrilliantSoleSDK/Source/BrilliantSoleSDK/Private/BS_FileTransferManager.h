// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BS_BaseManager.h"
#include "BS_FileTransferManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_FileTransferManager, Log, All);

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
};
