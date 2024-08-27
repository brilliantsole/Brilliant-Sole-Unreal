// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BS_BarometerSensorDataManager.h"
#include "BS_PressureSensorDataManager.h"
#include "BS_MotionSensorDataManager.h"
#include "BS_SensorType.h"
#include "BS_BaseManager.h"
#include "BS_SensorDataManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_SensorDataManager, Log, All);

#define EBS_SensorScalarsMessage BS_MessageGetSensorScalars
#define EBS_SensorDataMessge BS_MessageSensorData

UCLASS()
class UBS_SensorDataManager : public UBS_BaseManager
{
	GENERATED_BODY()

public:
	UBS_SensorDataManager();

public:
	bool OnRxMessage(uint8 MessageType, const TArray<uint8> &Message) override;

private:
	UPROPERTY()
	TMap<EBS_SensorType, float> SensorScalars;
	void ParseSensorScalars(const TArray<uint8> &Message);

	void ParseSensorData(const TArray<uint8> &Message);

	UPROPERTY()
	UBS_PressureSensorDataManager *PressureSensorDataManager;
	UPROPERTY()
	UBS_MotionSensorDataManager *MotionSensorDataManager;
	UPROPERTY()
	UBS_BarometerSensorDataManager *BarometerSensorDataManager;

	// MOTION CALLBACKS START
public:
	FVectorCallbackLocal OnAccelerationUpdate;
	FVectorCallbackLocal OnGravityUpdate;
	FVectorCallbackLocal OnLinearAccelerationUpdate;
	FVectorCallbackLocal OnGyroscopeUpdate;
	FVectorCallbackLocal OnMagnetometerUpdate;
	FQuaternionCallbackLocal OnGameRotationUpdate;
	FQuaternionCallbackLocal OnRotationUpdate;

	FRotatorCallbackLocal OnOrientationUpdate;
	FActivityCallbackLocal OnActivityUpdate;
	FStepCountCallbackLocal OnStepCountUpdate;
	FTimestampCallbackLocal OnStepDetectionUpdate;
	FDeviceOrientationCallbackLocal OnDeviceOrientationUpdate;
	// MOTION CALLBACKS END
};
