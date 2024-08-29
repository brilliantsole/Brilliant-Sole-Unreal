// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BS_BaseSensorDataManager.h"
#include "BS_DeviceOrientation.h"
#include "BS_Activity.h"
#include "BS_MotionSensorDataManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_MotionSensorDataManager, Log, All);

DECLARE_DELEGATE_TwoParams(FBS_VectorCallbackLocal, const FVector &, const uint64 &);
DECLARE_DELEGATE_TwoParams(FBS_RotatorCallbackLocal, const FRotator &, const uint64 &);
DECLARE_DELEGATE_TwoParams(FBS_QuaternionCallbackLocal, const FQuat &, const uint64 &);
DECLARE_DELEGATE_OneParam(FBS_TimestampCallbackLocal, const uint64 &);
DECLARE_DELEGATE_TwoParams(FBS_ActivityCallbackLocal, const TSet<EBS_Activity> &, const uint64 &);
DECLARE_DELEGATE_TwoParams(FBS_StepCountCallbackLocal, const uint32 &, const uint64 &);
DECLARE_DELEGATE_TwoParams(FBS_DeviceOrientationCallbackLocal, const EBS_DeviceOrientation &, const uint64 &);

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
	FBS_VectorCallbackLocal OnAccelerationUpdate;
	FBS_VectorCallbackLocal OnGravityUpdate;
	FBS_VectorCallbackLocal OnLinearAccelerationUpdate;
	FBS_VectorCallbackLocal OnGyroscopeUpdate;
	FBS_VectorCallbackLocal OnMagnetometerUpdate;
	FBS_QuaternionCallbackLocal OnGameRotationUpdate;
	FBS_QuaternionCallbackLocal OnRotationUpdate;

	FBS_RotatorCallbackLocal OnOrientationUpdate;
	FBS_ActivityCallbackLocal OnActivityUpdate;
	FBS_StepCountCallbackLocal OnStepCountUpdate;
	FBS_TimestampCallbackLocal OnStepDetectionUpdate;
	FBS_DeviceOrientationCallbackLocal OnDeviceOrientationUpdate;
};