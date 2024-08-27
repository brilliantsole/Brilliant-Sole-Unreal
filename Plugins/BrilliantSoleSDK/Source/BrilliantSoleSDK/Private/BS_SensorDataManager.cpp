// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_SensorDataManager.h"
#include "BS_Message.h"
#include "BS_ByteParser.h"
#include "BS_TimeUtils.h"
#include "BS_SensorType.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(LogBS_SensorDataManager);

UBS_SensorDataManager::UBS_SensorDataManager()
{
    SensorScalars.Reserve(static_cast<uint8>(EBS_SensorType::COUNT));

    PressureSensorDataManager = CreateDefaultSubobject<UBS_PressureSensorDataManager>(TEXT("PressureSensorDataManager"));

    MotionSensorDataManager = CreateDefaultSubobject<UBS_MotionSensorDataManager>(TEXT("MotionSensorDataManager"));

    OnAccelerationUpdate = MotionSensorDataManager->OnAccelerationUpdate;
    OnGravityUpdate = MotionSensorDataManager->OnGravityUpdate;
    OnLinearAccelerationUpdate = MotionSensorDataManager->OnLinearAccelerationUpdate;
    OnGyroscopeUpdate = MotionSensorDataManager->OnGyroscopeUpdate;
    OnMagnetometerUpdate = MotionSensorDataManager->OnMagnetometerUpdate;
    OnRotationUpdate = MotionSensorDataManager->OnRotationUpdate;
    OnGameRotationUpdate = MotionSensorDataManager->OnGameRotationUpdate;

    OnOrientationUpdate = MotionSensorDataManager->OnOrientationUpdate;
    OnActivityUpdate = MotionSensorDataManager->OnActivityUpdate;
    OnStepCountUpdate = MotionSensorDataManager->OnStepCountUpdate;
    OnStepDetectionUpdate = MotionSensorDataManager->OnStepDetectionUpdate;
    OnDeviceOrientationUpdate = MotionSensorDataManager->OnDeviceOrientationUpdate;

    BarometerSensorDataManager = CreateDefaultSubobject<UBS_BarometerSensorDataManager>(TEXT("BarometerSensorDataManager"));
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
        UE_LOGFMT(LogBS_SensorDataManager, Log, "SensorType: {0}, SensorScalar: {1}", UEnum::GetValueAsString(SensorType), SensorScalar);

        SensorScalars.Emplace(SensorType, SensorScalar);
    }

    UE_LOGFMT(LogBS_SensorDataManager, Log, "SensorScalars updated");
}

void UBS_SensorDataManager::ParseSensorData(const TArray<uint8> &Message)
{
    uint8 Offset = 0;

    uint64 Timestamp = TimeUtils::ParseTimestamp(Message);
    Offset += 2;

    UE_LOGFMT(LogBS_SensorDataManager, Log, "Timestamp: {0}ms", Timestamp);

    const uint8 SensorTypeEnum = Message[Offset++];
    if (SensorTypeEnum >= static_cast<uint8>(EBS_SensorType::COUNT))
    {
        UE_LOGFMT(LogBS_SensorDataManager, Error, "Invalid SensorTypeEnum {0}", SensorTypeEnum);
        return;
    }
    const EBS_SensorType SensorType = static_cast<EBS_SensorType>(SensorTypeEnum);
    UE_LOGFMT(LogBS_SensorDataManager, Log, "SensorType: {0}", UEnum::GetValueAsString(SensorType));

    float Scalar = SensorScalars.Contains(SensorType) ? SensorScalars[SensorType] : 1.0f;
    UE_LOGFMT(LogBS_SensorDataManager, Log, "Scalar: {0}", Scalar);

    const uint16 SensorDataMessageLength = BS_ByteParser::ParseAs<uint16>(Message, Offset, true);
    Offset += 2;

    const TArrayView<uint8> SensorDataMessage((uint8 *)(Message.GetData() + Offset), SensorDataMessageLength);

    if (PressureSensorDataManager->OnSensorDataMessage(SensorType, static_cast<TArray<uint8>>(SensorDataMessage), Timestamp, Scalar))
    {
        UE_LOGFMT(LogBS_SensorDataManager, Log, "Parsed PressureSensorDataManager Message");
    }
    else if (MotionSensorDataManager->OnSensorDataMessage(SensorType, static_cast<TArray<uint8>>(SensorDataMessage), Timestamp, Scalar))
    {
        UE_LOGFMT(LogBS_SensorDataManager, Log, "Parsed MotionSensorDataManager Message");
    }
    else if (BarometerSensorDataManager->OnSensorDataMessage(SensorType, static_cast<TArray<uint8>>(SensorDataMessage), Timestamp, Scalar))
    {
        UE_LOGFMT(LogBS_SensorDataManager, Log, "Parsed BarometerSensorDataManager Message");
    }
    else
    {
        UE_LOGFMT(LogBS_SensorDataManager, Error, "Unable to Parse {0} Message", UEnum::GetValueAsString(SensorType));
    }
}