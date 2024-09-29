// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Logging/StructuredLog.h"
#include "BS_TxRxMessageType.h"
#include "BS_DeviceEventType.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_TypeUtils, Log, All);

class BS_TypeUtils
{
public:
	static bool ConvertDeviceEventTypeToTxRxMessageType(EBS_DeviceEvent DeviceEventType, EBS_TxRxMessage &TxRxMessageType);
};
