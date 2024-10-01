// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_PressureSensorDataManager.h"
#include "Logging/StructuredLog.h"
#include "BS_ByteParser.h"
#include "Math/UnrealMathUtility.h"

DEFINE_LOG_CATEGORY(LogBS_PressureSensorDataManager);

const float UBS_PressureSensorDataManager::PressurePositionScalar = FMath::Pow(2.0f, 8.0f);

bool UBS_PressureSensorDataManager::OnSensorDataMessage(EBS_SensorType SensorType, const TArrayView<const uint8> &Message, const uint64 &Timestamp, const float &Scalar)
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

void UBS_PressureSensorDataManager::ParsePressure(EBS_SensorType SensorType, const TArrayView<const uint8> &Message, const uint64 &Timestamp, const float &Scalar)
{
    FBS_PressureData PressureData;

    uint8 MessageLength = Message.Num();

    if (NumberOfPressureSensors * 2 != MessageLength)
    {
        UE_LOGFMT(LogBS_PressureSensorDataManager, Error, "Invalid Message Length - expected {0}, got {1}", NumberOfPressureSensors * 2, MessageLength);
        return;
    }

    PressureData.Sensors.Reset(NumberOfPressureSensors);

    for (uint8 Index = 0; Index < NumberOfPressureSensors; Index++)
    {
        const FVector2D &Position = PressurePositions[Index];

        const uint16 RawValue = BS_ByteParser::ParseAs<uint16>(Message, Index * 2);
        UE_LOGFMT(LogBS_PressureSensorDataManager, Verbose, "RawPressureValue #{0}: {1}", Index, RawValue);

        const float ScaledValue = static_cast<float>(RawValue) * Scalar;
        UE_LOGFMT(LogBS_PressureSensorDataManager, Verbose, "ScaledValue #{0}: {1}", Index, ScaledValue);

        FBS_Range &Range = PressureSensorRanges[Index];
        const float NormalizedValue = Range.UpdateAndGetNormalization(ScaledValue, true);

        const float WeightedValue = 0.0f;

        const FBS_PressureSensorData PressureSensorData = {Position, RawValue, ScaledValue, NormalizedValue, WeightedValue};
        UE_LOGFMT(LogBS_PressureSensorDataManager, Verbose, "PressureSensorData #{0}: {1}", Index, PressureSensorData.ToString());
        PressureData.Sensors.Add(PressureSensorData);

        PressureData.ScaledSum += ScaledValue;
        PressureData.NormalizedSum += NormalizedValue / static_cast<float>(NumberOfPressureSensors);

        UE_LOGFMT(LogBS_PressureSensorDataManager, Verbose, "ScaledSum after adding ScaledValue #{0}: {1}", Index, PressureData.ScaledSum);
    }

    UE_LOGFMT(LogBS_PressureSensorDataManager, Verbose, "ScaledSum: {0}, NormalizedSum: {1}", PressureData.ScaledSum, PressureData.NormalizedSum);

    if (PressureData.ScaledSum > 0)
    {
        for (FBS_PressureSensorData PressureSensorData : PressureData.Sensors)
        {
            PressureSensorData.WeightedValue = PressureSensorData.ScaledValue / PressureData.ScaledSum;
            PressureData.CenterOfPressure += PressureSensorData.Position * PressureSensorData.WeightedValue;
        }
        UE_LOGFMT(LogBS_PressureSensorDataManager, Verbose, "CenterOfPressure: {0}", PressureData.CenterOfPressure.ToString());

        PressureData.NormalizedCenterOfPressure = CenterOfPressureRange.UpdateAndGetNormalization(PressureData.CenterOfPressure);
        UE_LOGFMT(LogBS_PressureSensorDataManager, Verbose, "NormalizedCenterOfPressure: {0}", PressureData.NormalizedCenterOfPressure.ToString());
    }
    else
    {
        UE_LOGFMT(LogBS_PressureSensorDataManager, Verbose, "ScaledSum is 0 - skipping CenterOfPressure calculation");
    }

    OnPressureUpdate.ExecuteIfBound(PressureData, Timestamp);
}

void UBS_PressureSensorDataManager::ParsePressurePositions(const TArrayView<const uint8> &Message)
{
    PressurePositions.Reset();

    const uint8 MessageLength = Message.Num();
    for (uint8 Offset = 0; Offset < MessageLength; Offset += 2)
    {
        float X = static_cast<float>(Message[Offset]) / PressurePositionScalar;
        float Y = static_cast<float>(Message[Offset + 1]) / PressurePositionScalar;
        UE_LOGFMT(LogBS_PressureSensorDataManager, Verbose, "PressurePosition #{0}: {1}, {2}", PressurePositions.Num(), X, Y);

        PressurePositions.Emplace(X, Y);
    }

    UE_LOGFMT(LogBS_PressureSensorDataManager, Verbose, "PressurePositions updated");

    NumberOfPressureSensors = PressurePositions.Num();
    UE_LOGFMT(LogBS_PressureSensorDataManager, Verbose, "NumberOfPressureSensors: {0}", NumberOfPressureSensors);
    PressureSensorRanges.SetNum(NumberOfPressureSensors);
}