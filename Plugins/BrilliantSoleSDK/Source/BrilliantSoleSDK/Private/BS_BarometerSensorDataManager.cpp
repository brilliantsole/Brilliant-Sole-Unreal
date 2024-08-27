// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_BarometerSensorDataManager.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(LogBS_BarometerSensorDataManager);

bool UBS_BarometerSensorDataManager::OnSensorDataMessage(EBS_SensorType SensorType, const TArray<uint8> &Message, const uint64 &Timestamp, const float &Scalar)
{
    switch (SensorType)
    {
    case EBS_SensorType::BAROMETER:
        // FILL
        break;
    default:
        return false;
    }
    return true;
}