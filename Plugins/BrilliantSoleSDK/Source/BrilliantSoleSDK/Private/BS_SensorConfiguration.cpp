// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_SensorConfiguration.h"
#include "Logging/StructuredLog.h"
#include "BS_ByteParser.h"

DEFINE_LOG_CATEGORY(LogBS_SensorConfiguration);

void UBS_SensorConfiguration::Copy(const UBS_SensorConfiguration *Other)
{
    UE_LOGFMT(LogBS_SensorConfiguration, Log, "Copying SensorConfiguration...");
    SensorRates = Other->GetSensorRates();
}

bool UBS_SensorConfiguration::IsEqualTo(const UBS_SensorConfiguration *Other)
{

    const TMap<EBS_SensorType, EBS_SensorRate> &OtherSensorRates = Other->GetSensorRates();
    if (SensorRates.Num() != OtherSensorRates.Num())
    {
        return false;
    }

    for (const TPair<EBS_SensorType, EBS_SensorRate> &Pair : SensorRates)
    {
        const EBS_SensorType SensorType = Pair.Key;
        if (!OtherSensorRates.Contains(SensorType))
        {
            return false;
        }

        if (SensorRates[SensorType] != OtherSensorRates[SensorType])
        {
            return false;
        }
    }

    return true;
}

void UBS_SensorConfiguration::Clear()
{

    for (const TPair<EBS_SensorType, EBS_SensorRate> &Pair : SensorRates)
    {
        ClearSensorRate(Pair.Key);
    }
}

EBS_SensorRate UBS_SensorConfiguration::GetSensorRate(EBS_SensorType SensorType, bool &bContainsSensorType) const
{

    bContainsSensorType = SensorRates.Contains(SensorType);
    return bContainsSensorType ? SensorRates[SensorType] : EBS_SensorRate::Value0;
}
EBS_SensorRate UBS_SensorConfiguration::GetSensorRate(EBS_SensorType SensorType) const
{

    bool bContainsSensorType;
    return GetSensorRate(SensorType, bContainsSensorType);
}

bool UBS_SensorConfiguration::IsSensorRateNonZero(EBS_SensorType SensorType) const
{

    bool bContainsSensorType;
    EBS_SensorRate SensorRate = GetSensorRate(SensorType, bContainsSensorType);
    return SensorRate != EBS_SensorRate::Value0;
}

void UBS_SensorConfiguration::SetSensorRate(EBS_SensorType SensorType, EBS_SensorRate SensorRate, bool &bDidUpdateSensorRate)
{
    if (SensorRates.Contains(SensorType))
    {
        if (SensorRates[SensorType] == SensorRate)
        {
            bDidUpdateSensorRate = false;
        }
        else
        {
            SensorRates[SensorType] = SensorRate;
            bDidUpdateSensorRate = true;
        }
    }
    else
    {
        SensorRates.Add(SensorType, SensorRate);
        bDidUpdateSensorRate = true;
    }
}
void UBS_SensorConfiguration::SetSensorRate(EBS_SensorType SensorType, EBS_SensorRate SensorRate)
{

    bool bDidUpdateSensorRate;
    return SetSensorRate(SensorType, SensorRate, bDidUpdateSensorRate);
}

void UBS_SensorConfiguration::SetSensorRates(const TMap<EBS_SensorType, EBS_SensorRate> &NewSensorRates)
{

    for (const TPair<EBS_SensorType, EBS_SensorRate> &Pair : SensorRates)
    {
        if (SensorRates.Contains(Pair.Key))
        {
            SensorRates[Pair.Key] = Pair.Value;
        }
    }
}

void UBS_SensorConfiguration::ClearSensorRate(EBS_SensorType SensorType)
{

    if (!SensorRates.Contains(SensorType))
    {
        return;
    }
    SensorRates.Add(SensorType, EBS_SensorRate::Value0);
}

void UBS_SensorConfiguration::ToggleSensorRate(EBS_SensorType SensorType, EBS_SensorRate SensorRate, EBS_SensorRate &UpdatedSensorRate)
{

    UE_LOGFMT(LogBS_SensorConfiguration, Log, "Toggling Sensor Rate {0} to {1}", UEnum::GetValueAsString(SensorType), UEnum::GetValueAsString(SensorRate));
    if (SensorRates.Contains(SensorType))
    {
        if (SensorRates[SensorType] == EBS_SensorRate::Value0)
        {
            SensorRates.Add(SensorType, SensorRate);
        }
        else
        {
            SensorRates.Add(SensorType, EBS_SensorRate::Value0);
        }
    }
    else
    {
        SensorRates.Add(SensorType, SensorRate);
    }

    UpdatedSensorRate = SensorRates[SensorType];
}
void UBS_SensorConfiguration::ToggleSensorRate(EBS_SensorType SensorType, EBS_SensorRate SensorRate)
{
    EBS_SensorRate UpdatedSensorRate;
    return ToggleSensorRate(SensorType, SensorRate, UpdatedSensorRate);
}

void UBS_SensorConfiguration::UpdateSensorTypes()
{

    if (!bSensorTypesNeedsUpdate)
    {
        return;
    }
    SensorTypes.Reset();
    for (const TPair<EBS_SensorType, EBS_SensorRate> &Pair : SensorRates)
    {
        SensorTypes.Add(Pair.Key);
    }
    bSensorTypesNeedsUpdate = true;
}

FString UBS_SensorConfiguration::ToString() const
{

    FString String;

    for (const TPair<EBS_SensorType, EBS_SensorRate> &Pair : SensorRates)
    {
        String += FString::Printf(TEXT("Key: %s, Value: %s\n"), *UEnum::GetValueAsString(Pair.Key), *UEnum::GetValueAsString(Pair.Value));
    }

    return String;
}

EBS_SensorRate UBS_SensorConfiguration::GetClosestSensorRate(uint16 RawSensorRate)
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

void UBS_SensorConfiguration::Parse(const TArray<uint8> &Message)
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

    bSensorTypesNeedsUpdate = true;

    UE_LOGFMT(LogBS_SensorConfiguration, Log, "Updated SensorConfiguration:\n{0}", ToString());
}

const TArray<uint8> UBS_SensorConfiguration::ToArray() const
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