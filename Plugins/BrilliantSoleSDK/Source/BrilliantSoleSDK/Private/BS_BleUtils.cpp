// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserves

#include "BS_BleUtils.h"

#define BS_BLE_UUID_PREFIX "ea6da725-"
#define BS_BLE_UUID_SUFFIX "-4f9b-893d-c3913e33b39f"
#define BS_BLE_GENERATE_UUID(val) (BS_BLE_UUID_PREFIX val BS_BLE_UUID_SUFFIX)

// SERVICES
const FString UBS_BleUtils::MainServiceUUID = BS_BLE_GENERATE_UUID("0000");
const TArray<FString> UBS_BleUtils::ServiceUUIDs = UBS_BleUtils::InitializeServiceUUIDs();
const TArray<FString> UBS_BleUtils::InitializeServiceUUIDs()
{
    return {MainServiceUUID};
}

// CHARACTERISTICS
const FString UBS_BleUtils::RxCharacteristicUUID = BS_BLE_GENERATE_UUID("1000");
const FString UBS_BleUtils::TxCharacteristicUUID = BS_BLE_GENERATE_UUID("1001");