// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_SensorConfigurationManager.h"
#include "Logging/StructuredLog.h"
#include "BS_Message.h"

DEFINE_LOG_CATEGORY(LogBS_SensorConfigurationManager);

UBS_SensorConfigurationManager::UBS_SensorConfigurationManager()
{
    SensorConfiguration = NewObject<UBS_SensorConfiguration>();
    TempSensorConfiguration = NewObject<UBS_SensorConfiguration>();
}

bool UBS_SensorConfigurationManager::OnRxMessage(uint8 MessageType, const TArray<uint8> &Message)
{
    switch (MessageType)
    {
    case BS_MessageGetSensorConfiguration:
    case BS_MessageSetSensorConfiguration:
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
    SensorConfiguration->Parse(Message);
    OnSensorConfigurationUpdate.ExecuteIfBound(SensorConfiguration);
}

void UBS_SensorConfigurationManager::SetSensorConfiguration(const UBS_SensorConfiguration *NewSensorConfiguration)
{
    if (SensorConfiguration->IsEqualTo(NewSensorConfiguration))
    {
        UE_LOGFMT(LogBS_SensorConfigurationManager, Log, "SensorConfigurations are equal - not updating");
        return;
    }
    const TArray<uint8> TxMessage = NewSensorConfiguration->ToArray();
    SendTxMessages.ExecuteIfBound({{BS_MessageSetSensorConfiguration, TxMessage}}, true);
}

void UBS_SensorConfigurationManager::ClearSensorConfiguration()
{
    TempSensorConfiguration->Copy(SensorConfiguration);
    TempSensorConfiguration->Clear();
    SetSensorConfiguration(TempSensorConfiguration);
}

void UBS_SensorConfigurationManager::SetSensorRate(EBS_SensorType SensorType, EBS_SensorRate SensorRate, bool &bDidUpdateSensorRate)
{
    TempSensorConfiguration->Copy(SensorConfiguration);
    TempSensorConfiguration->SetSensorRate(SensorType, SensorRate, bDidUpdateSensorRate);
    SetSensorConfiguration(TempSensorConfiguration);
}
void UBS_SensorConfigurationManager::SetSensorRates(const TMap<EBS_SensorType, EBS_SensorRate> &NewSensorRates)
{
    TempSensorConfiguration->Copy(SensorConfiguration);
    TempSensorConfiguration->SetSensorRates(NewSensorRates);
    SetSensorConfiguration(TempSensorConfiguration);
}

void UBS_SensorConfigurationManager::ClearSensorRate(EBS_SensorType SensorType)
{
    TempSensorConfiguration->Copy(SensorConfiguration);
    TempSensorConfiguration->ClearSensorRate(SensorType);
    SetSensorConfiguration(TempSensorConfiguration);
}
void UBS_SensorConfigurationManager::ToggleSensorRate(EBS_SensorType SensorType, EBS_SensorRate SensorRate, EBS_SensorRate &UpdatedSensorRate)
{
    TempSensorConfiguration->Copy(SensorConfiguration);
    TempSensorConfiguration->ToggleSensorRate(SensorType, SensorRate, UpdatedSensorRate);
    SetSensorConfiguration(TempSensorConfiguration);
}