// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "BS_BatteryLevelMessageType.h"
#include "BS_DeviceInformationType.h"
#include "BS_MetaConnectionMessageType.h"
#include "BS_TxRxMessageType.h"
#include "BS_SMP_MessageType.h"

#define EBS_ConnectionMessages EBS_BatteryLevelMessages,      \
                               EBS_DeviceInformationMessages, \
                               EBS_MetaConnectionMessages,    \
                               EBS_TxRxMessages,              \
                               EBS_SMP_Messages

enum class EBS_ConnectionMessage : uint8
{
    EBS_ConnectionMessages,
    COUNT
};

ENUM_RANGE_BY_COUNT(EBS_ConnectionMessage, static_cast<int32>(EBS_ConnectionMessage::COUNT))