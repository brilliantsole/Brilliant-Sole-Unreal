// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BS_PressureSensorDataManager.h"
#include "BS_MotionSensorDataManager.h"
#include "BS_SensorDataManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_SensorDataManager, Log, All);

#define EBS_SensorScalarsMessage BS_MessageGetSensorScalars
#define EBS_SensorDataMessge BS_MessageSensorData

UCLASS()
class UBS_SensorDataManager : public UObject
{
	GENERATED_BODY()
};
