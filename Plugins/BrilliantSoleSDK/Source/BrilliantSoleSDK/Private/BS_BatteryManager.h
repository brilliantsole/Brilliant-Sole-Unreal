// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BS_BaseManager.h"
#include "BS_BatteryManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_BatteryManager, Log, All);

#define EBS_BatteryMessage BS_MessageBatteryIsCharging, \
						   BS_MessageGetBatteryCurrent

UCLASS()
class UBS_BatteryManager : public UBS_BaseManager
{
	GENERATED_BODY()

public:
	bool OnRxMessage(uint8 MessageType, const TArray<uint8> &Message) override;
};
