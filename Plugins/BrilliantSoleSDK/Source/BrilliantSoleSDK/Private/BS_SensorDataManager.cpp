// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_SensorDataManager.h"
#include "BS_Message.h"
#include "BS_ByteParser.h"
#include "BS_TimeUtils.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(LogBS_SensorDataManager);

UBS_SensorDataManager::UBS_SensorDataManager()
{
    SensorScalars.Reserve(static_cast<uint8>(EBS_SensorType::COUNT));
    PressureSensorDataManager = CreateDefaultSubobject<UBS_PressureSensorDataManager>(TEXT("PressureSensorDataManager"));
    MotionSensorDataManager = CreateDefaultSubobject<UBS_MotionSensorDataManager>(TEXT("MotionSensorDataManager"));
}

bool UBS_SensorDataManager::OnRxMessage(uint8 MessageType, const TArray<uint8> &Message)
{
    switch (MessageType)
    {
    case BS_MessageGetPressurePositions:
        PressureSensorDataManager->ParsePressurePositions(Message);
        break;
    case BS_MessageGetSensorScalars:
        ParseSensorScalars(Message);
        break;
    case BS_MessageSensorData:
        ParseSensorData(Message);
        break;
    default:
        return false;
    }

    return true;
}

void UBS_SensorDataManager::ParseSensorScalars(const TArray<uint8> &Message)
{
    SensorScalars.Reset();

    const uint8 MessageLength = Message.Num();
    for (uint8 Index = 0; Index < MessageLength; Index += 5)
    {
        const uint8 SensorTypeEnum = Message[Index];
        if (SensorTypeEnum >= static_cast<uint8>(EBS_SensorType::COUNT))
        {
            UE_LOGFMT(LogBS_SensorDataManager, Error, "invalid SensorTypeEnum {0}", SensorTypeEnum);
            continue;
        }
        const EBS_SensorType SensorType = static_cast<EBS_SensorType>(SensorTypeEnum);
        float SensorScalar = ByteParser::ParseAs<float>(Message, Index + 1);
        UE_LOGFMT(LogBS_SensorDataManager, Log, "SensorType: {0}, SensorScalar: {1}", UEnum::GetValueAsString(SensorType), SensorScalar);

        SensorScalars.Emplace(SensorType, SensorScalar);
    }

    UE_LOGFMT(LogBS_SensorDataManager, Log, "SensorScalars updated");
}

void UBS_SensorDataManager::ParseSensorData(const TArray<uint8> &Message)
{
    const uint8 MessageLength = Message.Num();
    uint8 Index = 0;

    uint64 Timestamp = TimeUtils::ParseTimestamp(Message);
    Index += 2;

    UE_LOGFMT(LogBS_SensorDataManager, Log, "Timestamp: {0}ms", Timestamp);
}