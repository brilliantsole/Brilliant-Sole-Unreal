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

void UBS_DevicePairPressureSensorDataManager::OnDevicePressureData(EBS_InsoleSide InsoleSide, const FBS_PressureData &DevicePressureData, const float &Timestamp)
{
    UE_LOGFMT(LogBS_DevicePairPressureSensorDataManager, Log, "Received Pressure Data from {0}", UEnum::GetValueAsString(InsoleSide));
    DevicesPressureData.Emplace(InsoleSide, DevicePressureData);

    if (!HasAllData())
    {
        UE_LOGFMT(LogBS_DevicePairPressureSensorDataManager, Log, "Doesn't have data from all devices - skipping");
        return;
    }

    FBS_DevicePairPressureData PressureData;
    for (const TPair<EBS_InsoleSide, FBS_PressureData> &Pair : DevicesPressureData)
    {
        const FBS_PressureData &_DevicePressureData = Pair.Value;
        PressureData.RawSum += _DevicePressureData.ScaledSum;
        PressureData.NormalizedSum += _DevicePressureData.NormalizedSum;
    }

    UE_LOGFMT(LogBS_DevicePairPressureSensorDataManager, Log, "RawSum: {0}, NormalizedSum: {1}", PressureData.RawSum, PressureData.NormalizedSum);

    if (PressureData.NormalizedSum > 0)
    {
        for (const TPair<EBS_InsoleSide, FBS_PressureData> &Pair : DevicesPressureData)
        {
            const EBS_InsoleSide _InsoleSide = Pair.Key;
            const FBS_PressureData &_DevicePressureData = Pair.Value;
            const float NormalizedSumWeight = _DevicePressureData.NormalizedSum / PressureData.NormalizedSum;
            if (NormalizedSumWeight > 0)
            {
                PressureData.CenterOfPressure.Y = _DevicePressureData.NormalizedCenterOfPressure.Y * NormalizedSumWeight;
                if (_InsoleSide == EBS_InsoleSide::RIGHT)
                {
                    PressureData.CenterOfPressure.X = NormalizedSumWeight;
                }
            }
        }
        PressureData.NormalizedCenterOfPressure = CenterOfPressureRange.UpdateAndGetNormalization(PressureData.CenterOfPressure);

        UE_LOGFMT(LogBS_DevicePairPressureSensorDataManager, Log, "CenterOfPressure: {0}, NormalizedCenterOfPressure: {1}", PressureData.CenterOfPressure.ToString(), PressureData.NormalizedCenterOfPressure.ToString());
    }

    OnPressureUpdate.ExecuteIfBound(PressureData, Timestamp);
}