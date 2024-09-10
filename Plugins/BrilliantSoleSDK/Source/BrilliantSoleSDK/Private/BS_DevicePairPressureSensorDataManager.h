// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BS_DevicePairPressureData.h"
#include "BS_CenterOfPressureRange.h"
#include "BS_DevicePairPressureSensorDataManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_DevicePairPressureSensorDataManager, Log, All);

DECLARE_DELEGATE_TwoParams(FBS_DevicePairPressureDataCallbackLocal, const FBS_DevicePairPressureData &, const uint64 &);

UCLASS()
class UBS_DevicePairPressureSensorDataManager : public UObject
{
	GENERATED_BODY()

public:
	FBS_DevicePairPressureDataCallbackLocal OnPressureUpdate;

private:
	FBS_CenterOfPressureRange CenterOfPressureRange;

public:
	void Reset() { CenterOfPressureRange.Reset(); }
};
