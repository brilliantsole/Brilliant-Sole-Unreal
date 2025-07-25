// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "BS_InformationMessageType.h"
#include "BS_SensorConfigurationMessageType.h"
#include "BS_SensorDataMessageType.h"
#include "BS_VibrationMessageType.h"
#include "BS_FileTransferMessageType.h"
#include "BS_TfliteMessageType.h"

#define EBS_TxRxMessages EBS_InformationMessages,         \
                         EBS_SensorConfigurationMessages, \
                         EBS_SensorDataMessages,          \
                         EBS_VibrationMessages,           \
                         EBS_FileTransferMessages,        \
                         EBS_TfliteMessages

enum class EBS_TxRxMessage : uint8
{
    EBS_TxRxMessages,
    COUNT
};

ENUM_RANGE_BY_COUNT(EBS_TxRxMessage, static_cast<int32>(EBS_TxRxMessage::COUNT))