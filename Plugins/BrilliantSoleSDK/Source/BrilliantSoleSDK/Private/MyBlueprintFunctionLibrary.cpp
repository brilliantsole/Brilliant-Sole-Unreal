// Copyright 2024 Brilliant Sole. All Rights Reserves

#include "MyBlueprintFunctionLibrary.h"

#define BS_BLE_UUID_PREFIX "ea6da725-"
#define BS_BLE_UUID_SUFFIX "4f9b-893d-c3913e33b39f"
#define BS_BLE_GENERATE_UUID(val) (BS_BLE_UUID_PREFIX val BS_BLE_UUID_SUFFIX)

// SERVICES
const FString UMyBlueprintFunctionLibrary::MainServiceUUID = BS_BLE_GENERATE_UUID("0000");
const TArray<FString> UMyBlueprintFunctionLibrary::ServiceUUIDs = UMyBlueprintFunctionLibrary::InitializeServiceUUIDs();
const TArray<FString> UMyBlueprintFunctionLibrary::InitializeServiceUUIDs()
{
    return {MainServiceUUID};
}

// CHARACTERISTICS
const FString UMyBlueprintFunctionLibrary::RxCharacteristicUUID = BS_BLE_GENERATE_UUID("1000");
const FString UMyBlueprintFunctionLibrary::TxCharacteristicUUID = BS_BLE_GENERATE_UUID("1001");