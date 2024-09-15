// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_MotionSensorDataManager.h"
#include "Logging/StructuredLog.h"
#include "BS_ByteParser.h"

DEFINE_LOG_CATEGORY(LogBS_MotionSensorDataManager);

bool UBS_MotionSensorDataManager::OnSensorDataMessage(EBS_SensorType SensorType, const TArray<uint8> &Message, const uint64 &Timestamp, const float &Scalar)
{
    switch (SensorType)
    {
    case EBS_SensorType::ACCELERATION:
    case EBS_SensorType::GRAVITY:
    case EBS_SensorType::LINEAR_ACCELERATION:
    case EBS_SensorType::GYROSCOPE:
    case EBS_SensorType::MAGNETOMETER:
        ParseVector(SensorType, Message, Timestamp, Scalar);
        break;
    case EBS_SensorType::GAME_ROTATION:
    case EBS_SensorType::ROTATION:
        ParseQuaternion(SensorType, Message, Timestamp, Scalar);
        break;

    case EBS_SensorType::ORIENTATION:
        ParseRotator(SensorType, Message, Timestamp, Scalar);
        break;
    case EBS_SensorType::ACTIVITY:
        ParseActivity(SensorType, Message, Timestamp);
        break;
    case EBS_SensorType::STEP_COUNT:
        ParseStepCount(SensorType, Message, Timestamp);
        break;
    case EBS_SensorType::STEP_DETECTION:
        ParseStepDetection(SensorType, Message, Timestamp);
        break;
    case EBS_SensorType::DEVICE_ORIENTATION:
        ParseDeviceOrientation(SensorType, Message, Timestamp);
        break;
    default:
        return false;
    }
    return true;
}

void UBS_MotionSensorDataManager::ParseVector(EBS_SensorType SensorType, const TArray<uint8> &Message, const uint64 &Timestamp, const float &Scalar)
{
    const int16 X = BS_ByteParser::ParseAs<int16>(Message, 0);
    const int16 Y = BS_ByteParser::ParseAs<int16>(Message, 2);
    const int16 Z = BS_ByteParser::ParseAs<int16>(Message, 4);

    UE_LOGFMT(LogBS_MotionSensorDataManager, Verbose, "X: {0}, Y: {1}, Z: {2}", X, Y, Z);

    FVector Vector(-Z, X, Y);
    Vector *= Scalar;

    UE_LOGFMT(LogBS_MotionSensorDataManager, Verbose, "{0}: {1}", UEnum::GetValueAsString(SensorType), Vector.ToString());

    switch (SensorType)
    {
    case EBS_SensorType::ACCELERATION:
        OnAccelerationUpdate.ExecuteIfBound(Vector, Timestamp);
        break;
    case EBS_SensorType::GRAVITY:
        OnGravityUpdate.ExecuteIfBound(Vector, Timestamp);
        break;
    case EBS_SensorType::LINEAR_ACCELERATION:
        OnLinearAccelerationUpdate.ExecuteIfBound(Vector, Timestamp);
        break;
    case EBS_SensorType::GYROSCOPE:
        OnGyroscopeUpdate.ExecuteIfBound(Vector, Timestamp);
        break;
    case EBS_SensorType::MAGNETOMETER:
        OnMagnetometerUpdate.ExecuteIfBound(Vector, Timestamp);
        break;
    default:
        UE_LOGFMT(LogBS_MotionSensorDataManager, Error, "Uncaught Vector SensorType {0}", UEnum::GetValueAsString(SensorType));
        break;
    }
}

void UBS_MotionSensorDataManager::ParseRotator(EBS_SensorType SensorType, const TArray<uint8> &Message, const uint64 &Timestamp, const float &Scalar)
{
    const int16 Yaw = BS_ByteParser::ParseAs<int16>(Message, 0);
    const int16 Pitch = BS_ByteParser::ParseAs<int16>(Message, 2);
    const int16 Roll = BS_ByteParser::ParseAs<int16>(Message, 4);

    UE_LOGFMT(LogBS_MotionSensorDataManager, Verbose, "Yaw: {0}, Pitch: {1}, Roll: {2}", Yaw, Pitch, Roll);

    FRotator Rotator(-Pitch, Yaw, -Roll);
    Rotator *= Scalar;

    UE_LOGFMT(LogBS_MotionSensorDataManager, Verbose, "{0}: {1}", UEnum::GetValueAsString(SensorType), Rotator.ToString());

    switch (SensorType)
    {
    case EBS_SensorType::ORIENTATION:
        OnOrientationUpdate.ExecuteIfBound(Rotator, Timestamp);
        break;
    default:
        UE_LOGFMT(LogBS_MotionSensorDataManager, Error, "Uncaught Orientation SensorType {0}", UEnum::GetValueAsString(SensorType));
        break;
    }
}

void UBS_MotionSensorDataManager::ParseQuaternion(EBS_SensorType SensorType, const TArray<uint8> &Message, const uint64 &Timestamp, const float &Scalar)
{
    const int16 X = BS_ByteParser::ParseAs<int16>(Message, 0);
    const int16 Y = BS_ByteParser::ParseAs<int16>(Message, 2);
    const int16 Z = BS_ByteParser::ParseAs<int16>(Message, 4);
    const int16 W = BS_ByteParser::ParseAs<int16>(Message, 6);

    UE_LOGFMT(LogBS_MotionSensorDataManager, Verbose, "X: {0}, Y: {1}, Z: {2}, W: {3}", X, Y, Z, W);

    FQuat Quaternion(-Z, X, Y, -W);
    Quaternion *= Scalar;

    UE_LOGFMT(LogBS_MotionSensorDataManager, Verbose, "Quaternion: {0}", Quaternion.ToString());

    switch (SensorType)
    {
    case EBS_SensorType::GAME_ROTATION:
        OnGameRotationUpdate.ExecuteIfBound(Quaternion, Timestamp);
        break;
    case EBS_SensorType::ROTATION:
        OnRotationUpdate.ExecuteIfBound(Quaternion, Timestamp);
        break;
    default:
        UE_LOGFMT(LogBS_MotionSensorDataManager, Error, "Uncaught Quaternion SensorType {0}", UEnum::GetValueAsString(SensorType));
        break;
    }
}

void UBS_MotionSensorDataManager::ParseActivity(EBS_SensorType SensorType, const TArray<uint8> &Message, const uint64 &Timestamp)
{
    const uint8 ActivityBitfield = Message[0];

    TSet<EBS_Activity> Activity;
    for (auto ActivityType : TEnumRange<EBS_Activity>())
    {
        const uint8 RawActivityType = static_cast<uint8>(ActivityType);
        if (ActivityBitfield & (1 << RawActivityType))
        {
            UE_LOGFMT(LogBS_MotionSensorDataManager, Verbose, "Found ActivityType {0}", UEnum::GetValueAsString(ActivityType));
            Activity.Add(ActivityType);
        }
    }
    OnActivityUpdate.ExecuteIfBound(Activity, Timestamp);
}

void UBS_MotionSensorDataManager::ParseStepCount(EBS_SensorType SensorType, const TArray<uint8> &Message, const uint64 &Timestamp)
{
    const uint32 StepCount = BS_ByteParser::ParseAs<uint32>(Message);
    UE_LOGFMT(LogBS_MotionSensorDataManager, Verbose, "StepCount: {0}", StepCount);
    OnStepCountUpdate.ExecuteIfBound(StepCount, Timestamp);
}

void UBS_MotionSensorDataManager::ParseStepDetection(EBS_SensorType SensorType, const TArray<uint8> &Message, const uint64 &Timestamp)
{
    UE_LOGFMT(LogBS_MotionSensorDataManager, Verbose, "Step Detected");
    OnStepDetectionUpdate.ExecuteIfBound(Timestamp);
}

void UBS_MotionSensorDataManager::ParseDeviceOrientation(EBS_SensorType SensorType, const TArray<uint8> &Message, const uint64 &Timestamp)
{
    const EBS_DeviceOrientation DeviceOrientation = static_cast<EBS_DeviceOrientation>(Message[0]);
    UE_LOGFMT(LogBS_MotionSensorDataManager, Verbose, "Device Orientation: {0}", UEnum::GetValueAsString(DeviceOrientation));
    OnDeviceOrientationUpdate.ExecuteIfBound(DeviceOrientation, Timestamp);
}