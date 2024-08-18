// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
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
class UBS_FileTransferManager : public UObject
{
	GENERATED_BODY()
};
