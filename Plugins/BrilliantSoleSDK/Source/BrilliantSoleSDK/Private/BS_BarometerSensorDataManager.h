// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BS_BaseSensorDataManager.h"
#include "BS_BarometerSensorDataManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_BarometerSensorDataManager, Warning, All);

DECLARE_DELEGATE_TwoParams(FBS_BarometerCallbackLocal, const float &, const uint64 &);

UCLASS()
class UBS_BarometerSensorDataManager : public UBS_BaseSensorDataManager
{
	GENERATED_BODY()

public:
	bool OnSensorDataMessage(EBS_SensorType SensorType, const TArrayView<const uint8> &Message, const uint64 &Timestamp, const float &Scalar) override;

private:
	void ParseBarometer(EBS_SensorType SensorType, const TArrayView<const uint8> &Message, const uint64 &Timestamp, const float &Scalar);

public:
	FBS_BarometerCallbackLocal OnBarometerUpdate;
};
