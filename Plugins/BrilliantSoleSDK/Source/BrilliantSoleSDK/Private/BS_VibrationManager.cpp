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
        TxMessage.Append(VibrationConfiguration.ToArray());
    }

    SendTxMessages.ExecuteIfBound({{BS_MessageTriggerVibration, TxMessage}}, true);
}