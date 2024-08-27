// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BS_BaseSensorDataManager.h"
#include "BS_PressureSensorDataManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_PressureSensorDataManager, Log, All);

#define EBS_PressurePositionsMessage BS_MessageGetPressurePositions

UCLASS()
class UBS_PressureSensorDataManager : public UBS_BaseSensorDataManager
{
	GENERATED_BODY()

public:
	void ParsePressurePositions(const TArray<uint8> &Message);
	const uint8 GetNumberOfPressurePositions() const { return PressurePositions.Num(); }

private:
	UPROPERTY()
	TArray<FVector2D> PressurePositions;

	static const float PressurePositionScalar;

public:
	bool OnSensorDataMessage(EBS_SensorType SensorType, const TArray<uint8> &Message, const uint64 &Timestamp, const float &Scalar) override;
};
