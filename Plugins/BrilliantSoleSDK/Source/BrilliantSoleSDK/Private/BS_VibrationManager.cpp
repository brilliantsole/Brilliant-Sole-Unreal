// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_VibrationManager.h"
#include "Logging/StructuredLog.h"
#include "BS_TxRxMessageType.h"

DEFINE_LOG_CATEGORY(LogBS_VibrationManager);

bool UBS_VibrationManager::OnRxMessage(EBS_TxRxMessage MessageType, const TArrayView<const uint8> &Message)
{
    switch (MessageType)
    {
    case EBS_TxRxMessage::GET_VIBRATION_LOCATIONS:
        ParseVibrationLocations(Message);
        break;
    default:
        return false;
    }
    return true;
}

void UBS_VibrationManager::Reset()
{
    VibrationLocations.Reset();
}

// VIBRATION LOCATIONS START
void UBS_VibrationManager::ParseVibrationLocations(const TArrayView<const uint8> &Message)
{
    UE_LOGFMT(LogBS_VibrationManager, Verbose, "Parsing Vibration Locations...");

    TArray<EBS_VibrationLocation> NewVibrationLocations;
    for (const uint8 VibrationLocationEnum : Message)
    {
        if (VibrationLocationEnum >= static_cast<uint8>(EBS_VibrationLocation::COUNT))
        {
            UE_LOGFMT(LogBS_VibrationManager, Error, "invalid VibrationLocationEnum {0}", VibrationLocationEnum);
            continue;
        }
        const EBS_VibrationLocation VibrationLocation = static_cast<EBS_VibrationLocation>(VibrationLocationEnum);

        UE_LOGFMT(LogBS_VibrationManager, Verbose, "Adding VibrationLocation {0}", UEnum::GetValueAsString(VibrationLocation));
        NewVibrationLocations.Add(VibrationLocation);
    }

    VibrationLocations = NewVibrationLocations;
    UE_LOGFMT(LogBS_VibrationManager, Verbose, "Parsed {0} VibrationLocations", VibrationLocations.Num());

    OnVibrationLocationsUpdate.ExecuteIfBound(VibrationLocations);
}
// VIBRATION LOCATIONS END

void UBS_VibrationManager::TriggerVibration(const TArray<FBS_VibrationConfiguration> &VibrationConfigurations)
{
    UE_LOGFMT(LogBS_VibrationManager, Verbose, "Triggering Vibration...");
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

    SendTxMessages.ExecuteIfBound({{EBS_TxRxMessage::TRIGGER_VIBRATION, TxMessage}}, true);
}