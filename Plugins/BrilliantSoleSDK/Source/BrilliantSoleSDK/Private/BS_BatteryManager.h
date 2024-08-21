// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BS_BaseManager.h"
#include "BS_BatteryManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_BatteryManager, Log, All);

DECLARE_DYNAMIC_DELEGATE_OneParam(FIsBatteryChargingCallback, bool, IsBatteryCharging);
DECLARE_DYNAMIC_DELEGATE_OneParam(FBatteryCurrentCallback, float, BatteryCurrent);

#define EBS_BatteryMessage BS_MessageIsBatteryCharging, \
						   BS_MessageGetBatteryCurrent

UCLASS()
class UBS_BatteryManager : public UBS_BaseManager
{
	GENERATED_BODY()

public:
	bool OnRxMessage(uint8 MessageType, const TArray<uint8> &Message) override;

	UFUNCTION(BlueprintPure, Category = "BS Battery Manager")
	bool GetIsBatterCharging() const { return bIsBatteryCharging; }

	UPROPERTY(BlueprintReadWrite, Category = "BS Battery Manager")
	FIsBatteryChargingCallback OnIsBatteryChargingUpdate;

	UFUNCTION(BlueprintPure, Category = "BS Battery Manager")
	float GetBatterCurrent() const { return BatteryCurrent; }

	UPROPERTY(BlueprintReadWrite, Category = "BS Battery Manager")
	FBatteryCurrentCallback OnBatteryCurrentUpdate;

private:
	bool bIsBatteryCharging = false;
	void ParseIsBatteryCharging(const TArray<uint8> &Message);

	float BatteryCurrent = 0.0f;
	void ParseBatteryCurrent(const TArray<uint8> &Message);
};
