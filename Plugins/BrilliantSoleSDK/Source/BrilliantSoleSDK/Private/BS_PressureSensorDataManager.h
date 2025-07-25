// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BS_BaseSensorDataManager.h"
#include "BS_PressureData.h"
#include "BS_Range.h"
#include "BS_CenterOfPressureRange.h"
#include "BS_PressureSensorDataManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_PressureSensorDataManager, Warning, All);

DECLARE_DELEGATE_TwoParams(FBS_PressureDataCallbackLocal, const FBS_PressureData &, const uint64 &);

UCLASS()
class UBS_PressureSensorDataManager : public UBS_BaseSensorDataManager
{
	GENERATED_BODY()

public:
	bool OnSensorDataMessage(EBS_SensorType SensorType, const TArrayView<const uint8> &Message, const uint64 &Timestamp, const float &Scalar) override;

private:
	void ParsePressure(EBS_SensorType SensorType, const TArrayView<const uint8> &Message, const uint64 &Timestamp, const float &Scalar);

public:
	FBS_PressureDataCallbackLocal OnPressureUpdate;

private:
	UPROPERTY()
	TArray<FBS_Range> PressureSensorRanges;
	UPROPERTY()
	FBS_CenterOfPressureRange CenterOfPressureRange;
	UPROPERTY()
	FBS_Range NormalizedSumRange;

public:
	void Reset();

	// PRESSURE POSITIONS START
public:
	void ParsePressurePositions(const TArrayView<const uint8> &Message);
	const uint8 GetNumberOfPressureSensors() const { return NumberOfPressureSensors; }

private:
	UPROPERTY()
	TArray<FVector2D> PressurePositions;
	static const float PressurePositionScalar;

	UPROPERTY()
	uint8 NumberOfPressureSensors;
	// PRESSURE POSITIONS END
};
