// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_DevicePairPressureSensorDataManager.h"
#include "Logging/StructuredLog.h"
#include "Math/UnrealMathUtility.h"

DEFINE_LOG_CATEGORY(LogBS_DevicePairPressureSensorDataManager);

bool UBS_DevicePairPressureSensorDataManager::HasAllData() const
{
    bool bHasAllData = true;
    for (EBS_InsoleSide InsoleSide : TEnumRange<EBS_InsoleSide>())
    {
        bHasAllData = bHasAllData || DevicesPressureData.Contains(InsoleSide);
        if (!bHasAllData)
        {
            break;
        }
    }
    return bHasAllData;
}

void UBS_DevicePairPressureSensorDataManager::OnDevicePressureData(EBS_InsoleSide InsoleSide, const FBS_PressureData &DevicePressureData, const int64 &Timestamp)
{
    UE_LOGFMT(LogBS_DevicePairPressureSensorDataManager, Verbose, "Received Pressure Data from {0}", UEnum::GetValueAsString(InsoleSide));
    DevicesPressureData.Emplace(InsoleSide, DevicePressureData);

    if (!HasAllData())
    {
        UE_LOGFMT(LogBS_DevicePairPressureSensorDataManager, Verbose, "Doesn't have data from all devices - skipping");
        return;
    }

    FBS_DevicePairPressureData PressureData;
    for (const TPair<EBS_InsoleSide, FBS_PressureData> &Pair : DevicesPressureData)
    {
        const FBS_PressureData &_DevicePressureData = Pair.Value;
        PressureData.ScaledSum += _DevicePressureData.ScaledSum;
    }
    PressureData.NormalizedSum = NormalizedSumRange.UpdateAndGetNormalization(PressureData.ScaledSum, false);
    UE_LOGFMT(LogBS_DevicePairPressureSensorDataManager, Verbose, "ScaledSum: {0}, NormalizedSum: {1}", PressureData.ScaledSum, PressureData.NormalizedSum);

    if (PressureData.NormalizedSum > 0)
    {
        for (const TPair<EBS_InsoleSide, FBS_PressureData> &Pair : DevicesPressureData)
        {
            const EBS_InsoleSide _InsoleSide = Pair.Key;
            const FBS_PressureData &_DevicePressureData = Pair.Value;

            if (true)
            {
                const uint8 NumberOfPressureSensors = _DevicePressureData.Sensors.Num();

                if (_InsoleSide == EBS_InsoleSide::RIGHT)
                {
                    PressureData.RightSensors.Reset(NumberOfPressureSensors);
                }
                else
                {
                    PressureData.LeftSensors.Reset(NumberOfPressureSensors);
                }

                for (uint8 Index = 0; Index < NumberOfPressureSensors; Index++)
                {
                    auto PressureSensorData = _DevicePressureData.Sensors[Index];
                    PressureSensorData.WeightedValue = PressureSensorData.ScaledValue / PressureData.ScaledSum;
                    PressureSensorData.Position.X *= 0.5;
                    if (_InsoleSide == EBS_InsoleSide::RIGHT)
                    {
                        PressureSensorData.Position.X += 0.5;
                    }
                    PressureData.CenterOfPressure.X += PressureSensorData.Position.X * PressureSensorData.WeightedValue;
                    PressureData.CenterOfPressure.Y += PressureSensorData.Position.Y * PressureSensorData.WeightedValue;

                    if (_InsoleSide == EBS_InsoleSide::RIGHT)
                    {
                        PressureData.RightSensors.Add(PressureSensorData);
                    }
                    else
                    {
                        PressureData.LeftSensors.Add(PressureSensorData);
                    }
                }
            }
            else
            {
                const float NormalizedSumWeight = _DevicePressureData.NormalizedSum / PressureData.NormalizedSum;
                if (NormalizedSumWeight > 0)
                {
                    PressureData.CenterOfPressure.Y += _DevicePressureData.NormalizedCenterOfPressure.Y * NormalizedSumWeight;
                    if (_InsoleSide == EBS_InsoleSide::RIGHT)
                    {
                        PressureData.CenterOfPressure.X = NormalizedSumWeight;
                    }
                }
            }
        }
        PressureData.NormalizedCenterOfPressure = CenterOfPressureRange.UpdateAndGetNormalization(PressureData.CenterOfPressure);

        UE_LOGFMT(LogBS_DevicePairPressureSensorDataManager, Verbose, "CenterOfPressure: {0}, NormalizedCenterOfPressure: {1}", PressureData.CenterOfPressure.ToString(), PressureData.NormalizedCenterOfPressure.ToString());
    }

    OnPressureUpdate.ExecuteIfBound(PressureData, Timestamp);
}