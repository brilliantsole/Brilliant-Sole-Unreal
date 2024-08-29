// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BS_BaseSensorDataManager.h"
#include "BS_Pressure.h"
#include "BS_PressureSensorDataManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_PressureSensorDataManager, Log, All);

DECLARE_DELEGATE_TwoParams(FBS_PressureCallbackLocal, const FBS_Pressure &, const uint64 &);

#define EBS_PressurePositionsMessage BS_MessageGetPressurePositions

UCLASS()
class UBS_PressureSensorDataManager : public UBS_BaseSensorDataManager
{
	GENERATED_BODY()

public:
	bool OnSensorDataMessage(EBS_SensorType SensorType, const TArray<uint8> &Message, const uint64 &Timestamp, const float &Scalar) override;

private:
	void ParsePressure(EBS_SensorType SensorType, const TArray<uint8> &Message, const uint64 &Timestamp, const float &Scalar);

public:
	FBS_PressureCallbackLocal OnPressureUpdate;

	// PRESSURE POSITIONS START
public:
	void ParsePressurePositions(const TArray<uint8> &Message);
	const uint8 GetNumberOfPressurePositions() const { return PressurePositions.Num(); }

private:
	UPROPERTY()
	TArray<FVector2D> PressurePositions;
	static const float PressurePositionScalar;
	// PRESSURE POSITIONS END
};
