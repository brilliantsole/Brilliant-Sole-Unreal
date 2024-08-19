// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_SensorConfigurationManager.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(LogBS_SensorConfigurationManager);

bool UBS_SensorConfigurationManager::OnRxMessage(uint8 MessageType, const TArray<uint8> &Message)
{
    return false;
}