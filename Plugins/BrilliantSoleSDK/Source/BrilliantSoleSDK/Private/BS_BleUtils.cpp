// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_BleUtils.h"
#include "Logging/StructuredLog.h"

#define BS_BLE_UUID_PREFIX "ea6da725-"
#define BS_BLE_UUID_SUFFIX "-4f9b-893d-c3913e33b39f"
#define BS_BLE_GENERATE_UUID(val) (BS_BLE_UUID_PREFIX val BS_BLE_UUID_SUFFIX)

#define BLE_UUID_PREFIX "0000"
#define BLE_UUID_SUFFIX "-0000-1000-8000-00805f9b34fb"
#define BLE_GENERATE_UUID(val) (BLE_UUID_PREFIX val BLE_UUID_SUFFIX)

DEFINE_LOG_CATEGORY(LogBS_BleUtils);

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
const FString UBS_BleUtils::BatteryLevelCharacteristicUUID = BLE_GENERATE_UUID("2a19");

const FString UBS_BleUtils::RxCharacteristicUUID = BS_BLE_GENERATE_UUID("1000");
const FString UBS_BleUtils::TxCharacteristicUUID = BS_BLE_GENERATE_UUID("1001");

const FString UBS_BleUtils::ManufacturerNameStringCharacteristicUUID = BLE_GENERATE_UUID("2a29");
const FString UBS_BleUtils::ModelNumberStringCharacteristicUUID = BLE_GENERATE_UUID("2a24");
const FString UBS_BleUtils::SerialNumberStringCharacteristicUUID = BLE_GENERATE_UUID("2a25");
const FString UBS_BleUtils::HardwareRevisionStringCharacteristicUUID = BLE_GENERATE_UUID("2a27");
const FString UBS_BleUtils::FirmwareRevisionCharacteristicUUID = BLE_GENERATE_UUID("2a26");
const FString UBS_BleUtils::SoftwareRevisionCharacteristicUUID = BLE_GENERATE_UUID("2a28");

const EBS_BleCharacteristic UBS_BleUtils::GetBleCharacteristic(const FString &CharacteristicUUID)
{
    if (CharacteristicUUID == BatteryLevelCharacteristicUUID)
    {
        return EBS_BleCharacteristic::BATTERY_LEVEL;
    }
    else if (CharacteristicUUID == RxCharacteristicUUID)
    {
        return EBS_BleCharacteristic::RX;
    }
    else if (CharacteristicUUID == TxCharacteristicUUID)
    {
        return EBS_BleCharacteristic::TX;
    }
    else if (CharacteristicUUID == ManufacturerNameStringCharacteristicUUID)
    {
        return EBS_BleCharacteristic::MANUFACTURER_NAME;
    }
    else if (CharacteristicUUID == ModelNumberStringCharacteristicUUID)
    {
        return EBS_BleCharacteristic::MODEL_NUMBER;
    }
    else if (CharacteristicUUID == SerialNumberStringCharacteristicUUID)
    {
        return EBS_BleCharacteristic::SERIAL_NUMBER;
    }
    else if (CharacteristicUUID == HardwareRevisionStringCharacteristicUUID)
    {
        return EBS_BleCharacteristic::HARDWARE_REVISION;
    }
    else if (CharacteristicUUID == FirmwareRevisionCharacteristicUUID)
    {
        return EBS_BleCharacteristic::FIRMWARE_REVISION;
    }
    else if (CharacteristicUUID == SoftwareRevisionCharacteristicUUID)
    {
        return EBS_BleCharacteristic::SOFTWARE_REVISION;
    }
    else
    {
        UE_LOGFMT(LogBS_BleUtils, Error, "Uncaught CharacteristicUUID {0}", CharacteristicUUID);
        return EBS_BleCharacteristic::NONE;
    }
}

const EBS_DeviceInformationType UBS_BleUtils::GetDeviceInformationType(const EBS_BleCharacteristic DeviceInformationCharacteristic)
{
    switch (DeviceInformationCharacteristic)
    {
    case EBS_BleCharacteristic::MANUFACTURER_NAME:
        return EBS_DeviceInformationType::MANUFACTURER_NAME;
    case EBS_BleCharacteristic::MODEL_NUMBER:
        return EBS_DeviceInformationType::MODEL_NUMBER;
    case EBS_BleCharacteristic::SERIAL_NUMBER:
        return EBS_DeviceInformationType::SERIAL_NUMBER;
    case EBS_BleCharacteristic::HARDWARE_REVISION:
        return EBS_DeviceInformationType::HARDWARE_REVISION;
    case EBS_BleCharacteristic::FIRMWARE_REVISION:
        return EBS_DeviceInformationType::FIRMWARE_REVISION;
    case EBS_BleCharacteristic::SOFTWARE_REVISION:
        return EBS_DeviceInformationType::SOFTWARE_REVISION;
    default:
        UE_LOGFMT(LogBS_BleUtils, Error, "Uncaught DeviceInformationCharacteristic {0}", UEnum::GetValueAsString(DeviceInformationCharacteristic));
        return EBS_DeviceInformationType::NONE;
    }
}

const EBS_BleCharacteristic UBS_BleUtils::GetDeviceInformationCharacteristic(const EBS_DeviceInformationType DeviceInformationType)
{
    switch (DeviceInformationType)
    {
    case EBS_DeviceInformationType::MANUFACTURER_NAME:
        return EBS_BleCharacteristic::MANUFACTURER_NAME;
    case EBS_DeviceInformationType::MODEL_NUMBER:
        return EBS_BleCharacteristic::MODEL_NUMBER;
    case EBS_DeviceInformationType::SERIAL_NUMBER:
        return EBS_BleCharacteristic::SERIAL_NUMBER;
    case EBS_DeviceInformationType::HARDWARE_REVISION:
        return EBS_BleCharacteristic::HARDWARE_REVISION;
    case EBS_DeviceInformationType::FIRMWARE_REVISION:
        return EBS_BleCharacteristic::FIRMWARE_REVISION;
    case EBS_DeviceInformationType::SOFTWARE_REVISION:
        return EBS_BleCharacteristic::SOFTWARE_REVISION;
    default:
        UE_LOGFMT(LogBS_BleUtils, Error, "Uncaught DeviceInformationType {0}", UEnum::GetValueAsString(DeviceInformationType));
        return EBS_BleCharacteristic::NONE;
    }
}

const FString &UBS_BleUtils::GetDeviceInformationCharacteristicUUID(const EBS_DeviceInformationType DeviceInformationType)
{
    switch (DeviceInformationType)
    {
    case EBS_DeviceInformationType::MANUFACTURER_NAME:
        return ManufacturerNameStringCharacteristicUUID;
    case EBS_DeviceInformationType::MODEL_NUMBER:
        return ModelNumberStringCharacteristicUUID;
    case EBS_DeviceInformationType::SERIAL_NUMBER:
        return SerialNumberStringCharacteristicUUID;
    case EBS_DeviceInformationType::HARDWARE_REVISION:
        return HardwareRevisionStringCharacteristicUUID;
    case EBS_DeviceInformationType::FIRMWARE_REVISION:
        return FirmwareRevisionCharacteristicUUID;
    case EBS_DeviceInformationType::SOFTWARE_REVISION:
        return SoftwareRevisionCharacteristicUUID;
    default:
        UE_LOGFMT(LogBS_BleUtils, Error, "Uncaught DeviceInformationType {0}", UEnum::GetValueAsString(DeviceInformationType));
        return ManufacturerNameStringCharacteristicUUID;
    }
}