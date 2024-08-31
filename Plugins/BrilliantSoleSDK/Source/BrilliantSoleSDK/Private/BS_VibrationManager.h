// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BS_BaseManager.h"
#include "BS_VibrationConfiguration.h"
#include "BS_VibrationManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_VibrationManager, Log, All);

#define EBS_VibrationMessage BS_MessageTriggerVibration

UCLASS()
class UBS_VibrationManager : public UBS_BaseManager
{
	GENERATED_BODY()

public:
	void TriggerVibration(const TArray<FBS_VibrationConfiguration> &VibrationConfigurations);
};
