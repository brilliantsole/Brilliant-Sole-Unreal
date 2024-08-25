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
    const TArray<uint8> TxMessage = NewSensorConfiguration->ToArray();
    SendTxMessages.ExecuteIfBound({{BS_MessageSetSensorConfiguration, TxMessage}}, true);
}

void UBS_SensorConfigurationManager::ClearSensorConfiguration()
{
    TempSensorConfiguration->Copy(SensorConfiguration);
    TempSensorConfiguration->Clear();
    SetSensorConfiguration(TempSensorConfiguration);
}