// Copyright 2024 Brilliant Sole. All Rights Reserves

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyBlueprintFunctionLibrary.generated.h"

/**
 *
 */
UCLASS()
class BRILLIANTSOLESDK_API UMyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// SERVICES
	static const FString MainServiceUUID;
	UFUNCTION(BlueprintCallable, Category = "Brilliant Sole Ble Utils")
	static const FString &GetMainServiceUUID()
	{
		return MainServiceUUID;
	}

	static const TArray<FString> ServiceUUIDs;
	UFUNCTION(BlueprintPure, Category = "Brilliant Sole Ble Utils")
	static const TArray<FString> GetServiceUUIDs()
	{
		return ServiceUUIDs;
	}
	static const TArray<FString> InitializeServiceUUIDs();

	// CHARACTERISTICS
	static const FString RxCharacteristicUUID;
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Brilliant Sole Ble Utils")
	static const FString &GetRxCharacteristicUUID()
	{
		return RxCharacteristicUUID;
	}
	static const FString TxCharacteristicUUID;
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Brilliant Sole Ble Utils")
	static const FString &GetTxCharacteristicUUID()
	{
		return TxCharacteristicUUID;
	}
};
