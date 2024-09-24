// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#define EBS_FileTransferMessages GET_MAX_FILE_LENGTH,       \
								 GET_FILE_TRANSFER_TYPE,    \
								 SET_FILE_TRANSFER_TYPE,    \
								 GET_FILE_LENGTH,           \
								 SET_FILE_LENGTH,           \
								 GET_FILE_CHECKSUM,         \
								 SET_FILE_CHECKSUM,         \
								 SET_FILE_TRANSFER_COMMAND, \
								 GET_FILE_TRANSFER_STATUS,  \
								 GET_FILE_TRANSFER_BLOCK,   \
								 SET_FILE_TRANSFER_BLOCK,   \
								 FILE_BYTES_TRANSFERRED

enum class EBS_FileTransferMessage : uint8
{
	EBS_FileTransferMessages,
	COUNT
};

ENUM_RANGE_BY_COUNT(EBS_FileTransferMessage, static_cast<int32>(EBS_FileTransferMessage::COUNT))

#define EBS_FileTransferEvents EBS_FileTransferMessages, \
							   FILE_TRANSFER_PROGRESS,   \
							   FILE_TRANSFER_COMPLETE,   \
							   FILE_RECEIVED

enum class EBS_FileTransferEvent : uint8
{
	EBS_FileTransferEvents,
	COUNT
};

ENUM_RANGE_BY_COUNT(EBS_FileTransferEvent, static_cast<int32>(EBS_FileTransferEvent::COUNT))