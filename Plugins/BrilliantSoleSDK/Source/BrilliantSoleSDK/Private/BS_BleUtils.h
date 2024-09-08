// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserves

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BS_BleCharacteristic.h"
#include "BS_DeviceInformationType.h"
#include "BS_BleUtils.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_BleUtils, Log, All);

UCLASS()
class BRILLIANTSOLESDK_API UBS_BleUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// SERVICES
	static const FString MainServiceUUID;
	UFUNCTION(BlueprintPure, Category = "BS Ble Utils")
	static const FString &GetMainServiceUUID()
	{
		return MainServiceUUID;
	}

	static const FString BatteryServiceUUID;
	UFUNCTION(BlueprintPure, Category = "BS Ble Utils")
	static const FString &GetBatteryServiceUUID()
	{
		return BatteryServiceUUID;
	}

	static const FString DeviceInformationServiceUUID;
	UFUNCTION(BlueprintPure, Category = "BS Ble Utils")
	static const FString &GetDeviceInformationServiceUUID()
	{
		return DeviceInformationServiceUUID;
	}

	static const TArray<FString> ServiceUUIDs;
	UFUNCTION(BlueprintPure, Category = "BS Ble Utils")
	static const TArray<FString> GetServiceUUIDs()
	{
		return ServiceUUIDs;
	}
	static const TArray<FString> InitializeServiceUUIDs();

	// CHARACTERISTICS
	static const FString RxCharacteristicUUID;
	UFUNCTION(BlueprintPure, Category = "BS Ble Utils")
	static const FString &GetRxCharacteristicUUID()
	{
		return RxCharacteristicUUID;
	}
	static const FString TxCharacteristicUUID;
	UFUNCTION(BlueprintPure, Category = "BS Ble Utils")
	static const FString &GetTxCharacteristicUUID()
	{
		return TxCharacteristicUUID;
	}

	static const FString BatteryLevelCharacteristicUUID;
	UFUNCTION(BlueprintPure, Category = "BS Ble Utils")
	static const FString &GetBatteryLevelCharacteristicUUID()
	{
		return BatteryLevelCharacteristicUUID;
	}

	static const FString ManufacturerNameStringCharacteristicUUID;
	UFUNCTION(BlueprintPure, Category = "BS Ble Utils")
	static const FString &GetManufacturerNameStringCharacteristicUUID()
	{
		return ManufacturerNameStringCharacteristicUUID;
	}
	static const FString ModelNumberStringCharacteristicUUID;
	UFUNCTION(BlueprintPure, Category = "BS Ble Utils")
	static const FString &GetModelNumberStringCharacteristicUUID()
	{
		return ModelNumberStringCharacteristicUUID;
	}
	static const FString SerialNumberStringCharacteristicUUID;
	UFUNCTION(BlueprintPure, Category = "BS Ble Utils")
	static const FString &GetSerialNumberStringCharacteristicUUID()
	{
		return SerialNumberStringCharacteristicUUID;
	}
	static const FString HardwareRevisionStringCharacteristicUUID;
	UFUNCTION(BlueprintPure, Category = "BS Ble Utils")
	static const FString &GetHardwareRevisionStringCharacteristicUUID()
	{
		return HardwareRevisionStringCharacteristicUUID;
	}
	static const FString FirmwareRevisionCharacteristicUUID;
	UFUNCTION(BlueprintPure, Category = "BS Ble Utils")
	static const FString &GetFirmwareRevisionCharacteristicUUID()
	{
		return FirmwareRevisionCharacteristicUUID;
	}
	static const FString SoftwareRevisionCharacteristicUUID;
	UFUNCTION(BlueprintPure, Category = "BS Ble Utils")
	static const FString &GetSoftwareRevisionCharacteristicUUID()
	{
		return SoftwareRevisionCharacteristicUUID;
	}

	UFUNCTION(BlueprintPure, Category = "BS Ble Utils")
	static const EBS_BleCharacteristic GetBleCharacteristic(const FString &CharacteristicUUID);

	UFUNCTION(BlueprintPure, Category = "BS Ble Utils")
	static const EBS_DeviceInformationType GetDeviceInformationType(const EBS_BleCharacteristic DeviceInformationCharacteristic);

	UFUNCTION(BlueprintPure, Category = "BS Ble Utils")
	static const EBS_BleCharacteristic GetDeviceInformationCharacteristic(const EBS_DeviceInformationType DeviceInformationType);

	UFUNCTION(BlueprintPure, Category = "BS Ble Utils")
	static const FString &GetDeviceInformationCharacteristicUUID(const EBS_DeviceInformationType DeviceInformationType);
};