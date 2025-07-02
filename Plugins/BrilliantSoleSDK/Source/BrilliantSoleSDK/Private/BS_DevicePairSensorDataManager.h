// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BS_DevicePairBarometerSensorDataManager.h"
#include "BS_DevicePairPressureSensorDataManager.h"
#include "BS_DevicePairMotionSensorDataManager.h"
#include "BS_SensorType.h"
#include "BS_BaseManager.h"
#include "BS_DevicePairSensorDataManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_DevicePairSensorDataManager, Warning, All);

UCLASS()
class UBS_DevicePairSensorDataManager : public UBS_BaseManager
{
	GENERATED_BODY()

public:
	UBS_DevicePairSensorDataManager();
	void Reset() override;

public:
	UPROPERTY()
	UBS_DevicePairPressureSensorDataManager *PressureSensorDataManager;
	UPROPERTY()
	UBS_DevicePairMotionSensorDataManager *MotionSensorDataManager;
	UPROPERTY()
	UBS_DevicePairBarometerSensorDataManager *BarometerSensorDataManager;
};
