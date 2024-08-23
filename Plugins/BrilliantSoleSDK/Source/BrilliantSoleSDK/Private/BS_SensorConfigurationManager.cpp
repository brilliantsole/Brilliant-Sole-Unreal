// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_SensorConfigurationManager.h"
#include "Logging/StructuredLog.h"
#include "BS_Message.h"

DEFINE_LOG_CATEGORY(LogBS_SensorConfigurationManager);

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
    SensorConfiguration.Reset();
}

void UBS_SensorConfigurationManager::ParseSensorConfiguration(const TArray<uint8> &Message)
{
    SensorConfiguration.Parse(Message);
    OnSensorConfigurationUpdate.ExecuteIfBound(SensorConfiguration);
}

void UBS_SensorConfigurationManager::SetSensorConfiguration(const FBS_SensorConfiguration &NewSensorConfiguration)
{
    const TArray<uint8> TxMessage = NewSensorConfiguration.ToArray();
    SendTxMessages.ExecuteIfBound({{BS_MessageSetSensorConfiguration, TxMessage}}, true);
}

void UBS_SensorConfigurationManager::ClearSensorConfiguration()
{
    SetSensorConfiguration(ZeroSensorConfiguration);
}

const FBS_SensorConfiguration UBS_SensorConfigurationManager::InitializeZeroSensorConfiguration()
{
    FBS_SensorConfiguration NewConfiguration;
    for (uint8 SensorTypeIndex = 0; SensorTypeIndex < static_cast<uint8>(EBS_SensorType::COUNT); SensorTypeIndex++)
    {
        const EBS_SensorType SensorType = static_cast<EBS_SensorType>(SensorTypeIndex);
        NewConfiguration.SensorRates.Add(SensorType, EBS_SensorRate::Value0);
    }
    return NewConfiguration;
}
const FBS_SensorConfiguration UBS_SensorConfigurationManager::ZeroSensorConfiguration = UBS_SensorConfigurationManager::InitializeZeroSensorConfiguration();