// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BS_BaseManager.h"
#include "BS_BatteryManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_BatteryManager, Log, All);

DECLARE_DELEGATE_OneParam(FIsBatteryChargingCallbackLocal, bool);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FIsBatteryChargingCallback, bool, IsBatteryCharging);

DECLARE_DELEGATE_OneParam(FBatteryCurrentCallbackLocal, float);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBatteryCurrentCallback, float, BatteryCurrent);

#define EBS_BatteryMessage BS_MessageIsBatteryCharging, \
						   BS_MessageGetBatteryCurrent

UCLASS()
class UBS_BatteryManager : public UBS_BaseManager
{
	GENERATED_BODY()

public:
	void Reset() override;
	bool OnRxMessage(uint8 MessageType, const TArray<uint8> &Message) override;

	bool GetIsBatteryCharging() const { return bIsBatteryCharging; }
	FIsBatteryChargingCallbackLocal OnIsBatteryChargingUpdate;

	float GetBatteryCurrent() const { return BatteryCurrent; }
	FBatteryCurrentCallbackLocal OnBatteryCurrentUpdate;

private:
	bool bIsBatteryCharging = false;
	void ParseIsBatteryCharging(const TArray<uint8> &Message);

	float BatteryCurrent = 0.0f;
	void ParseBatteryCurrent(const TArray<uint8> &Message);
};
