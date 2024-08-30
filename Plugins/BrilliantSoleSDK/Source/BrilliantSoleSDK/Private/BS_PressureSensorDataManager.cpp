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

void UBS_PressureSensorDataManager::Reset()
{
    CenterOfPressureRange.Reset();
    for (FBS_Range PressureSensorRange : PressureSensorRanges)
    {
        PressureSensorRange.Reset();
    }
}

void UBS_PressureSensorDataManager::ParsePressure(EBS_SensorType SensorType, const TArray<uint8> &Message, const uint64 &Timestamp, const float &Scalar)
{
    FBS_PressureData PressureData;

    const uint8 NumberOfPressureSensors = GetNumberOfPressureSensors();
    uint8 MessageLength = Message.Num();

    if (NumberOfPressureSensors * 2 != MessageLength)
    {
        UE_LOGFMT(LogBS_PressureSensorDataManager, Error, "Invalid Message Length - expected {0}, got {1}", NumberOfPressureSensors * 2, MessageLength);
        return;
    }

    PressureData.Sensors.SetNum(NumberOfPressureSensors);

    for (uint8 Index = 0; Index < NumberOfPressureSensors; Index++)
    {
        const FVector2D &Position = PressurePositions[Index];

        const uint16 RawValue = BS_ByteParser::ParseAs<uint16>(Message, Index * 2);
        UE_LOGFMT(LogBS_PressureSensorDataManager, Log, "RawPressureValue #{0}: {1}", Index, RawValue);

        const float ScaledValue = static_cast<float>(RawValue) * Scalar;
        UE_LOGFMT(LogBS_PressureSensorDataManager, Log, "ScaledValue #{0}: {1}", Index, ScaledValue);

        // FILL - ranges and stuff
        const float NormalizedValue = 0.0f;
        const float WeightedValue = 0.0f;

        const FBS_PressureSensorData PressureSensorData = {Position, RawValue, ScaledValue, NormalizedValue, WeightedValue};
        UE_LOGFMT(LogBS_PressureSensorDataManager, Log, "PressureSensorData #{0}", Index, PressureSensorData.ToString());
        PressureData.Sensors.Emplace(PressureSensorData);
    }

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

        PressurePositions.Emplace(X, Y);
    }

    UE_LOGFMT(LogBS_PressureSensorDataManager, Log, "PressurePositions updated");

    const uint8 NumberOfPressureSensors = GetNumberOfPressureSensors();
    PressureSensorRanges.SetNum(NumberOfPressureSensors);
}