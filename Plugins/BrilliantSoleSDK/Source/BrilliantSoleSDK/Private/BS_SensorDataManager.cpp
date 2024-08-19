// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_SensorDataManager.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(LogBS_SensorDataManager);

bool UBS_SensorDataManager::OnRxMessage(uint8 MessageType, const TArray<uint8> &Message)
{
    return false;
}