// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BS_BaseSensorDataManager.h"
#include "BS_DeviceOrientation.h"
#include "BS_Activity.h"
#include "BS_MotionSensorDataManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_MotionSensorDataManager, Log, All);

DECLARE_DELEGATE_TwoParams(FVectorCallbackLocal, const FVector &, const uint64 &);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FVectorCallback, const FVector &, Vector, const uint64 &, Timestamp);

DECLARE_DELEGATE_TwoParams(FRotatorCallbackLocal, const FRotator &, const uint64 &);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRotatorCallback, const FRotator &, Rotator, const uint64 &, Timestamp);

DECLARE_DELEGATE_TwoParams(FQuaternionCallbackLocal, const FQuat &, const uint64 &);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FQuaternionCallback, const FQuat &, Quaternion, const uint64 &, Timestamp);

DECLARE_DELEGATE_OneParam(FTimestampCallbackLocal, const uint64 &);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTimestampCallback, const uint64 &, Timestamp);

DECLARE_DELEGATE_TwoParams(FActivityCallbackLocal, const TSet<EBS_Activity> &, const uint64 &);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActivityCallback, const TSet<EBS_Activity> &, Activity, const uint64 &, Timestamp);

DECLARE_DELEGATE_TwoParams(FStepCountCallbackLocal, const uint32 &, const uint64 &);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStepCountCallback, const uint32 &, StepCount, const uint64 &, Timestamp);

DECLARE_DELEGATE_TwoParams(FDeviceOrientationCallbackLocal, const EBS_DeviceOrientation &, const uint64 &);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDeviceOrientationCallback, const EBS_DeviceOrientation &, DeviceOrientation, const uint64 &, Timestamp);

UCLASS()
class UBS_MotionSensorDataManager : public UBS_BaseSensorDataManager
{
	GENERATED_BODY()

public:
	bool OnSensorDataMessage(EBS_SensorType SensorType, const TArray<uint8> &Message, const uint64 &Timestamp, const float &Scalar) override;

private:
	void ParseVector(EBS_SensorType SensorType, const TArray<uint8> &Message, const uint64 &Timestamp, const float &Scalar);
	void ParseRotator(EBS_SensorType SensorType, const TArray<uint8> &Message, const uint64 &Timestamp, const float &Scalar);
	void ParseQuaternion(EBS_SensorType SensorType, const TArray<uint8> &Message, const uint64 &Timestamp, const float &Scalar);
	void ParseOrientation(EBS_SensorType SensorType, const TArray<uint8> &Message, const uint64 &Timestamp, const float &Scalar);
	void ParseActivity(EBS_SensorType SensorType, const TArray<uint8> &Message, const uint64 &Timestamp);
	void ParseStepCount(EBS_SensorType SensorType, const TArray<uint8> &Message, const uint64 &Timestamp);
	void ParseStepDetection(EBS_SensorType SensorType, const TArray<uint8> &Message, const uint64 &Timestamp);
	void ParseDeviceOrientation(EBS_SensorType SensorType, const TArray<uint8> &Message, const uint64 &Timestamp);

public:
	FVectorCallbackLocal OnAccelerationCallback;
	FVectorCallbackLocal OnGravityCallback;
	FVectorCallbackLocal OnLinearAccelerationCallback;
	FVectorCallbackLocal OnGyroscopeCallback;
	FVectorCallbackLocal OnMagnetometerCallback;
	FQuaternionCallbackLocal OnGameRotationCallback;
	FQuaternionCallbackLocal OnRotationCallback;

	FRotatorCallbackLocal OnOrientationCallback;
	FActivityCallbackLocal OnActivityCallback;
	FStepCountCallbackLocal OnStepCountCallback;
	FTimestampCallbackLocal OnStepDetectionCallback;
	FDeviceOrientationCallbackLocal OnDeviceOrientationCallback;
};