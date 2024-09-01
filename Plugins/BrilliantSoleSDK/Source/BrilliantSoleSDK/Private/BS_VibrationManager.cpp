// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_VibrationManager.h"
#include "Logging/StructuredLog.h"
#include "BS_Message.h"

DEFINE_LOG_CATEGORY(LogBS_VibrationManager);

void UBS_VibrationManager::TriggerVibration(const TArray<FBS_VibrationConfiguration> &VibrationConfigurations)
{
    UE_LOGFMT(LogBS_VibrationManager, Log, "Triggering Vibration...");
    TArray<uint8> TxMessage;
    for (const FBS_VibrationConfiguration &VibrationConfiguration : VibrationConfigurations)
    {
        const TArray<uint8> VibrationConfigurationArray = VibrationConfiguration.ToArray();
        const uint8 VibrationConfigurationArrayLength = VibrationConfigurationArray.Num();
        if (VibrationConfigurationArrayLength == 0)
        {
            UE_LOGFMT(LogBS_VibrationManager, Warning, "Empty VibrationConfigurationArray - Skipping");
            continue;
        }
        TxMessage.Append(VibrationConfigurationArray);
    }

    const uint8 TxMessageLength = TxMessage.Num();
    if (TxMessageLength == 0)
    {
        UE_LOGFMT(LogBS_VibrationManager, Warning, "Empty TxMessageLength - Skipping");
        return;
    }

    SendTxMessages.ExecuteIfBound({{BS_MessageTriggerVibration, TxMessage}}, true);
}