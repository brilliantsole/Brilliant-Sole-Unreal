// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BS_SensorType.h"
#include "BS_BaseSensorDataManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_BaseSensorDataManager, Log, All);

UCLASS(Abstract)
class UBS_BaseSensorDataManager : public UObject
{
	GENERATED_BODY()

public:
	virtual bool OnSensorDataMessage(EBS_SensorType SensorType, const TArrayView<const uint8> &Message, const uint64 &Timestamp, const float &Scalar) { return false; }
};
