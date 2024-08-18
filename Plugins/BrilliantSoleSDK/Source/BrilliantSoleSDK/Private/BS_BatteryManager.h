// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BS_BatteryManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_BatteryManager, Log, All);

#define EBS_BatteryMessage BS_MessageBatteryIsCharging, \
						   BS_MessageGetBatteryCurrent

UCLASS()
class UBS_BatteryManager : public UObject
{
	GENERATED_BODY()
};
