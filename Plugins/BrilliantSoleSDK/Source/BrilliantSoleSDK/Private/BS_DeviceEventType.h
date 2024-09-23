// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "BS_ConnectionEventType.h"
#include "BS_BatteryLevelMessageType.h"
#include "BS_DeviceInformationType.h"
#include "BS_MetaConnectionMessageType.h"
#include "BS_TxRxMessageType.h"
#include "BS_SMP_MessageType.h"

#define EBS_DeviceEvents CONNECTION_MESSAGE,              \
                         EBS_ConnectionEvents,            \
                         EBS_MetaConnectionMessages,      \
                         EBS_BatteryLevelMessages,        \
                         EBS_InformationMessages,         \
                         EBS_DeviceInformationEvents,     \
                         EBS_SensorConfigurationMessages, \
                         EBS_SensorDataEvents,            \
                         EBS_FileTransferEvents,          \
                         EBS_TfliteMessages,              \
                         EBS_SMP_Messages

enum class EBS_DeviceEvent : uint8
{
    EBS_DeviceEvents,
    COUNT
};

ENUM_RANGE_BY_COUNT(EBS_DeviceEvent, static_cast<int32>(EBS_DeviceEvent::COUNT))