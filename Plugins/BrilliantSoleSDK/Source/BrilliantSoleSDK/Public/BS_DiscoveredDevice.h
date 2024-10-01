// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BS_DeviceType.h"
#include "Logging/StructuredLog.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "BS_ByteParser.h"
#include "BS_DiscoveredDevice.generated.h"

USTRUCT(BlueprintType, meta = (ShowOnlyInnerProperties, FullyExpand = "true"))
struct FBS_DiscoveredDevice
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BS Discovered Device")
    FString BluetoothId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BS Discovered Device")
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BS Discovered Device")
    EBS_DeviceType DeviceType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BS Discovered Device")
    int32 RSSI;

    bool Parse(const TArrayView<const uint8> &Message)
    {
        const FString DiscoveredDeviceString = BS_ByteParser::GetString(Message, true);
        UE_LOGFMT(LogTemp, Verbose, "DiscoveredDeviceString: {0}", DiscoveredDeviceString);

        TSharedPtr<FJsonObject> DiscoveredDeviceJson;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(DiscoveredDeviceString);

        if (FJsonSerializer::Deserialize(Reader, DiscoveredDeviceJson) && DiscoveredDeviceJson.IsValid())
        {
            Name = DiscoveredDeviceJson->GetStringField("name");
            BluetoothId = DiscoveredDeviceJson->GetStringField("bluetoothId");
            RSSI = DiscoveredDeviceJson->GetIntegerField("rssi");
            const FString DeviceTypeString = DiscoveredDeviceJson->GetStringField("deviceType");
            if (DeviceTypeString == "leftInsole")
            {
                DeviceType = EBS_DeviceType::LEFT_INSOLE;
            }
            else if (DeviceTypeString == "rightInsole")
            {
                DeviceType = EBS_DeviceType::RIGHT_INSOLE;
            }
            else
            {
                UE_LOGFMT(LogTemp, Error, "uncaught  DeviceTypeString {0}", DeviceTypeString);
                return false;
            }

            UE_LOGFMT(LogTemp, Verbose, "Name: {0}, BluetoothId: {1}, RSSI: {2}, DeviceType: {3}", Name, BluetoothId, RSSI, UEnum::GetValueAsString(DeviceType));
        }
        else
        {
            UE_LOGFMT(LogTemp, Error, "Unable to convert DiscoveredDeviceString to json");
            return false;
        }
        return true;
    }
};