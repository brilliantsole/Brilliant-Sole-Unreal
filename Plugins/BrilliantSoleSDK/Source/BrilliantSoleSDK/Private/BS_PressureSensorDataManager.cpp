// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_PressureSensorDataManager.h"
#include "Logging/StructuredLog.h"
#include "Math/UnrealMathUtility.h"

DEFINE_LOG_CATEGORY(LogBS_PressureSensorDataManager);

const float UBS_PressureSensorDataManager::PressurePositionScalar = FMath::Pow(2.0f, 8.0f);

void UBS_PressureSensorDataManager::ParsePressurePositions(const TArray<uint8> &Message)
{
    PressurePositions.Reset();

    const uint8 MessageLength = Message.Num();
    for (uint8 Index = 0; Index < MessageLength; Index += 2)
    {
        float X = static_cast<float>(Message[Index]) / PressurePositionScalar;
        float Y = static_cast<float>(Message[Index + 1]) / PressurePositionScalar;
        UE_LOGFMT(LogBS_PressureSensorDataManager, Log, "#{0}: {1}, {2}", PressurePositions.Num(), X, Y);

        PressurePositions.Add({X, Y});
    }

    UE_LOGFMT(LogBS_PressureSensorDataManager, Log, "PressurePositions updated");
}