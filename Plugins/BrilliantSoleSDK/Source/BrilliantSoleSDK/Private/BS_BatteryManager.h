// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BS_BaseManager.h"
#include "BS_BatteryManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_BatteryManager, Verbose, All);

DECLARE_DELEGATE_OneParam(FBS_IsBatteryChargingCallbackLocal, bool);
DECLARE_DELEGATE_OneParam(FBS_BatteryCurrentCallbackLocal, float);

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
	FBS_IsBatteryChargingCallbackLocal OnIsBatteryChargingUpdate;

	float GetBatteryCurrent() const { return BatteryCurrent; }
	FBS_BatteryCurrentCallbackLocal OnBatteryCurrentUpdate;

private:
	UPROPERTY()
	bool bIsBatteryCharging = false;
	void ParseIsBatteryCharging(const TArray<uint8> &Message);

	UPROPERTY()
	float BatteryCurrent = 0.0f;
	void ParseBatteryCurrent(const TArray<uint8> &Message);
};
