// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_SensorDataManager.h"
#include "BS_TxRxMessageType.h"
#include "BS_ByteParser.h"
#include "BS_TimeUtils.h"
#include "BS_SensorType.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(LogBS_SensorDataManager);

UBS_SensorDataManager::UBS_SensorDataManager()
{
    UE_LOGFMT(LogBS_SensorDataManager, Verbose, "Constructor: {0}", GetName());
    if (HasAnyFlags(RF_ClassDefaultObject))
    {
        UE_LOGFMT(LogBS_SensorDataManager, Verbose, "CDO - Skipping Constructor");
        return;
    }

    SensorScalars.Reserve(static_cast<uint8>(EBS_SensorType::COUNT));

    PressureSensorDataManager = CreateDefaultSubobject<UBS_PressureSensorDataManager>(TEXT("PressureSensorDataManager"));
    MotionSensorDataManager = CreateDefaultSubobject<UBS_MotionSensorDataManager>(TEXT("MotionSensorDataManager"));
    BarometerSensorDataManager = CreateDefaultSubobject<UBS_BarometerSensorDataManager>(TEXT("BarometerSensorDataManager"));
}

void UBS_SensorDataManager::Reset()
{
    PressureSensorDataManager->Reset();
}

bool UBS_SensorDataManager::OnRxMessage(EBS_TxRxMessage MessageType, const TArrayView<const uint8> &Message)
{
    switch (MessageType)
    {
    case EBS_TxRxMessage::GET_PRESSURE_POSITIONS:
        PressureSensorDataManager->ParsePressurePositions(Message);
        break;
    case EBS_TxRxMessage::GET_SENSOR_SCALARS:
        ParseSensorScalars(Message);
        break;
    case EBS_TxRxMessage::SENSOR_DATA:
        ParseSensorData(Message);
        break;
    default:
        return false;
    }

    return true;
}

void UBS_SensorDataManager::ParseSensorScalars(const TArrayView<const uint8> &Message)
{
    SensorScalars.Reset();

    const uint8 MessageLength = Message.Num();
    for (uint8 Offset = 0; Offset < MessageLength; Offset += 5)
    {
        const uint8 SensorTypeEnum = Message[Offset];
        if (SensorTypeEnum >= static_cast<uint8>(EBS_SensorType::COUNT))
        {
            UE_LOGFMT(LogBS_SensorDataManager, Error, "invalid SensorTypeEnum {0}", SensorTypeEnum);
            continue;
        }
        const EBS_SensorType SensorType = static_cast<EBS_SensorType>(SensorTypeEnum);
        float SensorScalar = BS_ByteParser::ParseAs<float>(Message, Offset + 1, true);
        UE_LOGFMT(LogBS_SensorDataManager, Verbose, "SensorType: {0}, SensorScalar: {1}", UEnum::GetValueAsString(SensorType), SensorScalar);

        SensorScalars.Emplace(SensorType, SensorScalar);
    }

    UE_LOGFMT(LogBS_SensorDataManager, Verbose, "SensorScalars updated");
}

void UBS_SensorDataManager::ParseSensorData(const TArrayView<const uint8> &Message)
{
    uint16 Offset = 0;
    const uint16 MessageLength = Message.Num();
    UE_LOGFMT(LogBS_SensorDataManager, Verbose, "MessageLength: {0}", MessageLength);

    uint64 Timestamp = TimeUtils::ParseTimestamp(Message);
    Offset += 2;
    UE_LOGFMT(LogBS_SensorDataManager, Verbose, "Timestamp: {0}ms", Timestamp);

    while (Offset < MessageLength)
    {
        const uint8 SensorTypeEnum = Message[Offset++];
        if (SensorTypeEnum >= static_cast<uint8>(EBS_SensorType::COUNT))
        {
            UE_LOGFMT(LogBS_SensorDataManager, Error, "Invalid SensorTypeEnum {0}", SensorTypeEnum);
            return;
        }
        const EBS_SensorType SensorType = static_cast<EBS_SensorType>(SensorTypeEnum);
        UE_LOGFMT(LogBS_SensorDataManager, Verbose, "SensorType: {0}", UEnum::GetValueAsString(SensorType));

        float Scalar = SensorScalars.Contains(SensorType) ? SensorScalars[SensorType] : 1.0f;
        UE_LOGFMT(LogBS_SensorDataManager, Verbose, "Scalar: {0}", Scalar);

        const uint8 SensorDataMessageLength = Message[Offset++];

        UE_LOGFMT(LogBS_SensorDataManager, Verbose, "SensorDataMessageLength: {0}", SensorDataMessageLength);

        const TArrayView<const uint8> SensorDataMessage = Message.Slice(Offset, SensorDataMessageLength);

        if (PressureSensorDataManager->OnSensorDataMessage(SensorType, SensorDataMessage, Timestamp, Scalar))
        {
            UE_LOGFMT(LogBS_SensorDataManager, Verbose, "Parsed PressureSensorDataManager Message");
        }
        else if (MotionSensorDataManager->OnSensorDataMessage(SensorType, SensorDataMessage, Timestamp, Scalar))
        {
            UE_LOGFMT(LogBS_SensorDataManager, Verbose, "Parsed MotionSensorDataManager Message");
        }
        else if (BarometerSensorDataManager->OnSensorDataMessage(SensorType, SensorDataMessage, Timestamp, Scalar))
        {
            UE_LOGFMT(LogBS_SensorDataManager, Verbose, "Parsed BarometerSensorDataManager Message");
        }
        else
        {
            UE_LOGFMT(LogBS_SensorDataManager, Error, "Unable to Parse {0} Message", UEnum::GetValueAsString(SensorType));
        }

        Offset += SensorDataMessageLength;
    }
}