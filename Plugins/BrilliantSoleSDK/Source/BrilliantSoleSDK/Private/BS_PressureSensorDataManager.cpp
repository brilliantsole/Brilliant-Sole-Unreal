// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_PressureSensorDataManager.h"
#include "Logging/StructuredLog.h"
#include "BS_ByteParser.h"
#include "Math/UnrealMathUtility.h"

DEFINE_LOG_CATEGORY(LogBS_PressureSensorDataManager);

const float UBS_PressureSensorDataManager::PressurePositionScalar = FMath::Pow(2.0f, 8.0f);

bool UBS_PressureSensorDataManager::OnSensorDataMessage(EBS_SensorType SensorType, const TArray<uint8> &Message, const uint64 &Timestamp, const float &Scalar)
{
    switch (SensorType)
    {
    case EBS_SensorType::PRESSURE:
        ParsePressure(SensorType, Message, Timestamp, Scalar);
        break;
    default:
        return false;
    }
    return true;
}

void UBS_PressureSensorDataManager::ParsePressure(EBS_SensorType SensorType, const TArray<uint8> &Message, const uint64 &Timestamp, const float &Scalar)
{
    FBS_PressureData PressureData;
    PressureData.Parse(Message, Scalar, PressurePositions);
    OnPressureUpdate.ExecuteIfBound(PressureData, Timestamp);
}

void UBS_PressureSensorDataManager::ParsePressurePositions(const TArray<uint8> &Message)
{
    PressurePositions.Reset();

    const uint8 MessageLength = Message.Num();
    for (uint8 Offset = 0; Offset < MessageLength; Offset += 2)
    {
        float X = static_cast<float>(Message[Offset]) / PressurePositionScalar;
        float Y = static_cast<float>(Message[Offset + 1]) / PressurePositionScalar;
        UE_LOGFMT(LogBS_PressureSensorDataManager, Log, "PressurePosition #{0}: {1}, {2}", PressurePositions.Num(), X, Y);

        PressurePositions.Add({X, Y});
    }

    UE_LOGFMT(LogBS_PressureSensorDataManager, Log, "PressurePositions updated");
}