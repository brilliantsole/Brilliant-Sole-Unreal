// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BS_InsoleSide.h"
#include "BS_PressureData.h"
#include "BS_DevicePairPressureData.h"
#include "BS_CenterOfPressureRange.h"
#include "BS_DevicePairPressureSensorDataManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_DevicePairPressureSensorDataManager, Log, All);

DECLARE_DELEGATE_TwoParams(FBS_DevicePairPressureDataCallbackLocal, const FBS_DevicePairPressureData &, const int64 &);

UCLASS()
class UBS_DevicePairPressureSensorDataManager : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TMap<EBS_InsoleSide, FBS_PressureData> DevicesPressureData;
	bool HasAllData() const;

public:
	FBS_DevicePairPressureDataCallbackLocal OnPressureUpdate;

public:
	void OnDevicePressureData(EBS_InsoleSide InsoleSide, const FBS_PressureData &DevicePressureData, const int64 &Timestamp);

private:
	UPROPERTY()
	FBS_CenterOfPressureRange CenterOfPressureRange;
	UPROPERTY()
	FBS_Range NormalizedSumRange;

public:
	void Reset()
	{
		CenterOfPressureRange.Reset();
		NormalizedSumRange.Reset();
	}
};
