// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_BatteryManager.h"
#include "Logging/StructuredLog.h"
#include "BS_Message.h"
#include "BS_ByteParser.h"

DEFINE_LOG_CATEGORY(LogBS_BatteryManager);

bool UBS_BatteryManager::OnRxMessage(uint8 MessageType, const TArray<uint8> &Message)
{
    switch (MessageType)
    {
    case BS_MessageIsBatteryCharging:
        ParseIsBatteryCharging(Message);
        break;
    case BS_MessageGetBatteryCurrent:
        ParseBatteryCurrent(Message);
        break;
    default:
        return false;
    }

    return true;
}

void UBS_BatteryManager::ParseIsBatteryCharging(const TArray<uint8> &Message)
{
    bIsBatteryCharging = Message[0];
    UE_LOGFMT(LogBS_BatteryManager, Verbose, "Is Battery Charging: {0}", bIsBatteryCharging);
    OnIsBatteryChargingUpdate.ExecuteIfBound(bIsBatteryCharging);
}
void UBS_BatteryManager::ParseBatteryCurrent(const TArray<uint8> &Message)
{
    BatteryCurrent = BS_ByteParser::ParseAs<float>(Message, 0, true);
    UE_LOGFMT(LogBS_BatteryManager, Verbose, "Battery Current: {0}mAh", BatteryCurrent);
    OnBatteryCurrentUpdate.ExecuteIfBound(BatteryCurrent);
}

void UBS_BatteryManager::Reset()
{
    bIsBatteryCharging = false;
    BatteryCurrent = 0.0f;
}