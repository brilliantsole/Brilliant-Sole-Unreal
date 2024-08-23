// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_SensorConfiguration.h"
#include "Logging/StructuredLog.h"
#include "BS_ByteParser.h"

DEFINE_LOG_CATEGORY(LogBS_SensorConfiguration);

FString FBS_SensorConfiguration::ToString() const
{
    FString String;

    for (const TPair<EBS_SensorType, EBS_SensorRate> &Pair : SensorRates)
    {
        String += FString::Printf(TEXT("Key: %s, Value: %s\n"), *UEnum::GetValueAsString(Pair.Key), *UEnum::GetValueAsString(Pair.Value));
    }

    return String;
}

EBS_SensorRate FBS_SensorConfiguration::GetClosestSensorRate(uint16 RawSensorRate)
{
    EBS_SensorRate SensorRate = EBS_SensorRate::Value0;
    if (RawSensorRate > 0)
    {
        const UEnum *EBS_SensorRatePtr = StaticEnum<EBS_SensorRate>();
        for (uint8 SensorRateIndex = 0; SensorRateIndex < EBS_SensorRatePtr->NumEnums(); SensorRateIndex++)
        {
            if (!EBS_SensorRatePtr->IsValidEnumValue(SensorRateIndex))
            {
                continue;
            }
            const EBS_SensorRate _SensorRate = static_cast<EBS_SensorRate>(EBS_SensorRatePtr->GetValueByIndex(SensorRateIndex));
            const uint16 _RawSensorRate = GetRawSensorRate(_SensorRate);
            if (RawSensorRate >= _RawSensorRate)
            {
                SensorRate = _SensorRate;
            }
        }
    }
    return SensorRate;
}

void FBS_SensorConfiguration::Parse(const TArray<uint8> &Message)
{
    SensorRates.Empty();

    const uint8 MessageLength = Message.Num();
    for (uint8 Index = 0; Index < MessageLength; Index += 3)
    {
        const uint8 SensorTypeEnum = Message[Index];
        if (SensorTypeEnum >= static_cast<uint8>(EBS_SensorType::COUNT))
        {
            UE_LOGFMT(LogBS_SensorConfiguration, Error, "invalid SensorTypeEnum {0}", SensorTypeEnum);
            continue;
        }
        const EBS_SensorType SensorType = static_cast<EBS_SensorType>(SensorTypeEnum);
        uint16 RawSensorRate = ByteParser::ParseAs<uint16>(Message, Index + 1);
        UE_LOGFMT(LogBS_SensorConfiguration, Log, "RawSensorRate: {0}ms", RawSensorRate);

        EBS_SensorRate SensorRate = GetClosestSensorRate(RawSensorRate);
        RawSensorRate = GetRawSensorRate(SensorRate);

        UE_LOGFMT(LogBS_SensorConfiguration, Log, "{0}: {1} ({2}ms)", UEnum::GetValueAsString(SensorType), UEnum::GetValueAsString(SensorRate), RawSensorRate);
        SensorRates.Emplace(SensorType, SensorRate);
    }

    UE_LOGFMT(LogBS_SensorConfiguration, Log, "Updated SensorConfiguration:\n{0}", ToString());
}

const TArray<uint8> FBS_SensorConfiguration::ToArray() const
{
    TArray<uint8> ByteArray;
    for (const TPair<EBS_SensorType, EBS_SensorRate> &Pair : SensorRates)
    {
        const uint8 SensorTypeEnum = static_cast<uint8>(Pair.Key);
        ByteArray.Add(SensorTypeEnum);
        const uint16 RawSensorRate = GetRawSensorRate(Pair.Value);
        ByteArray.Append(ByteParser::ToByteArray(RawSensorRate));
    }
    return ByteArray;
}