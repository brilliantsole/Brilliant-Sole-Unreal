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

    UE_LOGFMT(LogBS_MotionSensorDataManager, Log, "X: {0}, Y: {1}, Z: {2}", X, Y, Z);

    FVector Vector(-Z, Y, X);
    Vector *= Scalar;

    UE_LOGFMT(LogBS_MotionSensorDataManager, Log, "{0}: {1}", UEnum::GetValueAsString(SensorType), Vector.ToString());

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

    UE_LOGFMT(LogBS_MotionSensorDataManager, Log, "Yaw: {0}, Pitch: {1}, Roll: {2}", Yaw, Pitch, Roll);

    FRotator Rotator(-Pitch, Yaw, -Roll);
    Rotator *= Scalar;

    UE_LOGFMT(LogBS_MotionSensorDataManager, Log, "{0}: {1}", UEnum::GetValueAsString(SensorType), Rotator.ToString());

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

    UE_LOGFMT(LogBS_MotionSensorDataManager, Log, "X: {0}, Y: {1}, Z: {2}, W: {3}", X, Y, Z, W);

    FQuat Quaternion(-Z, X, Y, -W);
    Quaternion *= Scalar;

    // PermuteQuaternion(Quaternion);

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
            UE_LOGFMT(LogBS_MotionSensorDataManager, Log, "Found ActivityType {0}", UEnum::GetValueAsString(ActivityType));
            Activity.Add(ActivityType);
        }
    }
    OnActivityUpdate.ExecuteIfBound(Activity, Timestamp);
}

void UBS_MotionSensorDataManager::ParseStepCount(EBS_SensorType SensorType, const TArray<uint8> &Message, const uint64 &Timestamp)
{
    const uint32 StepCount = BS_ByteParser::ParseAs<uint32>(Message);
    UE_LOGFMT(LogBS_MotionSensorDataManager, Log, "StepCount: {0}", StepCount);
    OnStepCountUpdate.ExecuteIfBound(StepCount, Timestamp);
}

void UBS_MotionSensorDataManager::ParseStepDetection(EBS_SensorType SensorType, const TArray<uint8> &Message, const uint64 &Timestamp)
{
    UE_LOGFMT(LogBS_MotionSensorDataManager, Log, "Step Detected");
    OnStepDetectionUpdate.ExecuteIfBound(Timestamp);
}

void UBS_MotionSensorDataManager::ParseDeviceOrientation(EBS_SensorType SensorType, const TArray<uint8> &Message, const uint64 &Timestamp)
{
    const EBS_DeviceOrientation DeviceOrientation = static_cast<EBS_DeviceOrientation>(Message[0]);
    UE_LOGFMT(LogBS_MotionSensorDataManager, Log, "Device Orientation: {0}", UEnum::GetValueAsString(DeviceOrientation));
    OnDeviceOrientationUpdate.ExecuteIfBound(DeviceOrientation, Timestamp);
}

// // Helper function to compute all permutations of an array.
// void UBS_MotionSensorDataManager::GeneratePermutations(TArray<uint8> &Input, int32 StartIndex, TArray<TArray<uint8>> &OutPermutations)
// {
//     if (StartIndex == Input.Num() - 1)
//     {
//         OutPermutations.Add(Input);
//     }
//     else
//     {
//         for (int32 i = StartIndex; i < Input.Num(); i++)
//         {
//             Input.Swap(StartIndex, i);
//             GeneratePermutations(Input, StartIndex + 1, OutPermutations);
//             Input.Swap(StartIndex, i); // Backtrack
//         }
//     }
// }

// // Helper function to generate all signed combinations for a given array.
// void UBS_MotionSensorDataManager::GenerateSignedCombinations(const TArray<uint8> &Input, TArray<TArray<int8>> &OutSignedCombinations)
// {
//     int32 NumElements = Input.Num();
//     int32 NumCombinations = 1 << NumElements; // 2^NumElements combinations

//     for (int32 i = 0; i < NumCombinations; i++)
//     {
//         TArray<int8> SignedCombination;
//         for (int32 j = 0; j < NumElements; j++)
//         {
//             // If the j-th bit in i is set, make the element negative.
//             int8 Element = (i & (1 << j)) ? -Input[j] : Input[j];
//             SignedCombination.Add(Element);
//         }
//         OutSignedCombinations.Add(SignedCombination);
//     }
// }

// TArray<TArray<int8>> UBS_MotionSensorDataManager::GenerateSignedPermutations(const TArray<uint8> &Input)
// {
//     TArray<TArray<uint8>> Permutations;
//     TArray<uint8> InputCopy = Input;
//     GeneratePermutations(InputCopy, 0, Permutations);

//     TArray<TArray<int8>> SignedPermutations;

//     for (const TArray<uint8> &Permutation : Permutations)
//     {
//         TArray<TArray<int8>> SignedCombinations;
//         GenerateSignedCombinations(Permutation, SignedCombinations);
//         SignedPermutations.Append(SignedCombinations);
//     }

//     return SignedPermutations;
// }

// const TArray<TArray<int8>> UBS_MotionSensorDataManager::InitializeQuaternionPermutations()
// {
//     TArray<TArray<int8>> _QuaternionPermutations;
//     const TArray<uint8> InputArray = {1, 2, 3, 4};

//     // Start the recursive permutation generation
//     _QuaternionPermutations = GenerateSignedPermutations(InputArray);

//     return _QuaternionPermutations;
// }

// void UBS_MotionSensorDataManager::PermuteQuaternion(FQuat &Quat)
// {
//     if (QuaternionPermutationIndex < 0)
//     {
//         QuaternionPermutationIndex = 0;
//     }
//     else if (QuaternionPermutationIndex >= QuaternionPermutations.Num())
//     {
//         QuaternionPermutationIndex %= QuaternionPermutations.Num();
//     }

//     auto QuaternionPermutation = QuaternionPermutations[QuaternionPermutationIndex];

//     UE_LOGFMT(LogBS_MotionSensorDataManager, Log, "QuaternionPermutation #{0}/{1}: {2}", QuaternionPermutationIndex, QuaternionPermutations.Num(), QuaternionPermutation);

//     TempArray1[0] = Quat.W;
//     TempArray1[1] = Quat.X;
//     TempArray1[2] = Quat.Y;
//     TempArray1[3] = Quat.Z;

//     for (uint8 i = 0; i < QuaternionPermutation.Num(); i++)
//     {
//         auto j = QuaternionPermutation[i];
//         auto Value = TempArray1[i];
//         TempArray2[i] = TempArray1[FMath::Abs(j) - 1];
//         if (j < 0)
//         {
//             TempArray2[i] *= -1;
//         }
//     }

//     SetQuat(Quat, TempArray2[0], TempArray2[1], TempArray2[2], TempArray2[3]);
// }

// const TArray<TArray<int8>> UBS_MotionSensorDataManager::QuaternionPermutations = UBS_MotionSensorDataManager::InitializeQuaternionPermutations();

// void UBS_MotionSensorDataManager::SetQuat(FQuat &Quat, double W, double X, double Y, double Z)
// {
//     Quat.W = W;
//     Quat.X = X;
//     Quat.Y = Y;
//     Quat.Z = Z;
// }
// void UBS_MotionSensorDataManager::SetRotator(FRotator &Rotator, const FVector &Vector)
// {
//     Rotator.Pitch = Vector.X;
//     Rotator.Roll = Vector.Y;
//     Rotator.Yaw = Vector.Z;
// }