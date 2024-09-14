// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_BarometerSensorDataManager.h"
#include "Logging/StructuredLog.h"
#include "BS_ByteParser.h"

DEFINE_LOG_CATEGORY(LogBS_BarometerSensorDataManager);

bool UBS_BarometerSensorDataManager::OnSensorDataMessage(EBS_SensorType SensorType, const TArray<uint8> &Message, const uint64 &Timestamp, const float &Scalar)
{
    switch (SensorType)
    {
    case EBS_SensorType::BAROMETER:
        ParseBarometer(SensorType, Message, Timestamp, Scalar);
        break;
    default:
        return false;
    }
    return true;
}

void UBS_BarometerSensorDataManager::ParseBarometer(EBS_SensorType SensorType, const TArray<uint8> &Message, const uint64 &Timestamp, const float &Scalar)
{
    const float Barometer = BS_ByteParser::ParseAs<float>(Message);
    UE_LOGFMT(LogBS_BarometerSensorDataManager, Verbose, "Barometer: {0}", Barometer);
    OnBarometerUpdate.ExecuteIfBound(Barometer, Timestamp);
}