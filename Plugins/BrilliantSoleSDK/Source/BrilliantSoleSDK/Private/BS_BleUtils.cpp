// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_BleUtils.h"

#define BS_BLE_UUID_PREFIX "ea6da725-"
#define BS_BLE_UUID_SUFFIX "-4f9b-893d-c3913e33b39f"
#define BS_BLE_GENERATE_UUID(val) (BS_BLE_UUID_PREFIX val BS_BLE_UUID_SUFFIX)

#define BLE_UUID_PREFIX "0000"
#define BLE_UUID_SUFFIX "-0000-1000-8000-00805f9b34fb"
#define BLE_GENERATE_UUID(val) (BLE_UUID_PREFIX val BLE_UUID_SUFFIX)

// SERVICES
const FString UBS_BleUtils::MainServiceUUID = BS_BLE_GENERATE_UUID("0000");
const FString UBS_BleUtils::BatteryServiceUUID = BLE_GENERATE_UUID("180f");
const FString UBS_BleUtils::DeviceInformationServiceUUID = BLE_GENERATE_UUID("180a");

const TArray<FString> UBS_BleUtils::ServiceUUIDs = UBS_BleUtils::InitializeServiceUUIDs();
const TArray<FString> UBS_BleUtils::InitializeServiceUUIDs()
{
    return {MainServiceUUID};
}

// CHARACTERISTICS
const FString UBS_BleUtils::RxCharacteristicUUID = BS_BLE_GENERATE_UUID("1000");
const FString UBS_BleUtils::TxCharacteristicUUID = BS_BLE_GENERATE_UUID("1001");

const FString UBS_BleUtils::BatteryLevelCharacteristicUUID = BLE_GENERATE_UUID("2a19");

const FString UBS_BleUtils::ManufacturerNameStringCharacteristicUUID = BLE_GENERATE_UUID("2a29");
const FString UBS_BleUtils::ModelNumberStringCharacteristicUUID = BLE_GENERATE_UUID("2a24");
const FString UBS_BleUtils::SerialNumberStringCharacteristicUUID = BLE_GENERATE_UUID("2a25");
const FString UBS_BleUtils::HardwareRevisionStringCharacteristicUUID = BLE_GENERATE_UUID("2a27");
const FString UBS_BleUtils::FirmwareRevisionCharacteristicUUID = BLE_GENERATE_UUID("2a26");
const FString UBS_BleUtils::SoftwareRevisionCharacteristicUUID = BLE_GENERATE_UUID("2a28");
