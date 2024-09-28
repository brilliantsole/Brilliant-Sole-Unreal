// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_SensorConfigurationManager.h"
#include "Logging/StructuredLog.h"
#include "BS_TxRxMessageType.h"

DEFINE_LOG_CATEGORY(LogBS_SensorConfigurationManager);

UBS_SensorConfigurationManager::UBS_SensorConfigurationManager()
{
    SensorConfiguration = CreateDefaultSubobject<UBS_SensorConfiguration>(TEXT("SensorConfiguration"));
    TempSensorConfiguration = CreateDefaultSubobject<UBS_SensorConfiguration>(TEXT("TempSensorConfiguration"));
    TempSensorConfiguration2 = CreateDefaultSubobject<UBS_SensorConfiguration>(TEXT("TempSensorConfiguration2"));
}

bool UBS_SensorConfigurationManager::OnRxMessage(EBS_TxRxMessage MessageType, const TArray<uint8> &Message)
{
    switch (MessageType)
    {
    case EBS_TxRxMessage::GET_SENSOR_CONFIGURATION:
    case EBS_TxRxMessage::SET_SENSOR_CONFIGURATION:
        ParseSensorConfiguration(Message);
        break;
    default:
        return false;
    }

    return true;
}

void UBS_SensorConfigurationManager::Reset()
{
    SensorConfiguration->Reset();
}

void UBS_SensorConfigurationManager::ParseSensorConfiguration(const TArray<uint8> &Message)
{
    UE_LOGFMT(LogBS_SensorConfigurationManager, Verbose, "Parsing SensorConfiguration...");
    SensorConfiguration->Parse(Message);
    OnSensorConfigurationUpdate.ExecuteIfBound(SensorConfiguration);
}

void UBS_SensorConfigurationManager::SetSensorConfiguration(const UBS_SensorConfiguration *NewSensorConfiguration, bool bClearRest)
{
    if (SensorConfiguration->IsEqualTo(NewSensorConfiguration))
    {
        UE_LOGFMT(LogBS_SensorConfigurationManager, Verbose, "SensorConfigurations are equal - not updating");
        return;
    }
    TempSensorConfiguration2->Copy(NewSensorConfiguration);
    if (bClearRest)
    {
        const TArray<EBS_SensorType> &SensorTypes = SensorConfiguration->GetSensorTypes();
        const TMap<EBS_SensorType, EBS_SensorRate> &SensorRates = SensorConfiguration->GetSensorRates();
        const TMap<EBS_SensorType, EBS_SensorRate> &NewSensorRates = TempSensorConfiguration2->GetSensorRates();
        for (const TPair<EBS_SensorType, EBS_SensorRate> &Pair : SensorRates)
        {
            if (!NewSensorRates.Contains(Pair.Key))
            {
                TempSensorConfiguration2->ClearSensorRate(Pair.Key);
            }
        }
    }
    UE_LOGFMT(LogBS_SensorConfigurationManager, Verbose, "Setting SensorConfiguration...");
    const TArray<uint8> TxMessage = TempSensorConfiguration2->ToArray();
    SendTxMessages.ExecuteIfBound({{EBS_TxRxMessage::SET_SENSOR_CONFIGURATION, TxMessage}}, true);
}

void UBS_SensorConfigurationManager::ClearSensorConfiguration()
{
    TempSensorConfiguration->Copy(SensorConfiguration);
    TempSensorConfiguration->Clear();
    SetSensorConfiguration(TempSensorConfiguration);
}

void UBS_SensorConfigurationManager::SetSensorRate(EBS_SensorType SensorType, EBS_SensorRate SensorRate, bool &bDidUpdateSensorRate)
{
    TempSensorConfiguration->Reset();
    TempSensorConfiguration->SetSensorRate(SensorType, SensorRate, bDidUpdateSensorRate);
    SetSensorConfiguration(TempSensorConfiguration);
}
void UBS_SensorConfigurationManager::SetSensorRates(const TMap<EBS_SensorType, EBS_SensorRate> &NewSensorRates)
{
    TempSensorConfiguration->Reset();
    TempSensorConfiguration->SetSensorRates(NewSensorRates);
    SetSensorConfiguration(TempSensorConfiguration);
}

void UBS_SensorConfigurationManager::ClearSensorRate(EBS_SensorType SensorType)
{
    TempSensorConfiguration->Reset();
    TempSensorConfiguration->SetSensorRate(SensorType, EBS_SensorRate::Value0);
    SetSensorConfiguration(TempSensorConfiguration);
}
void UBS_SensorConfigurationManager::ToggleSensorRate(EBS_SensorType SensorType, EBS_SensorRate SensorRate, EBS_SensorRate &UpdatedSensorRate)
{
    UE_LOGFMT(LogBS_SensorConfigurationManager, Verbose, "Toggling Sensor Rate {0} to {1}", UEnum::GetValueAsString(SensorType), UEnum::GetValueAsString(SensorRate));
    TempSensorConfiguration->Reset();
    TempSensorConfiguration->SetSensorRate(SensorType, SensorConfiguration->GetSensorRate(SensorType));
    TempSensorConfiguration->ToggleSensorRate(SensorType, SensorRate, UpdatedSensorRate);
    UE_LOGFMT(LogBS_SensorConfigurationManager, Verbose, "UpdatedSensorRate {0}", UEnum::GetValueAsString(UpdatedSensorRate));
    SetSensorConfiguration(TempSensorConfiguration);
}