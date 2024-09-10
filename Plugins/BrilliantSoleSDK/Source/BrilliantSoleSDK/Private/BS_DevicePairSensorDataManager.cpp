// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_DevicePairSensorDataManager.h"
#include "BS_SensorType.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(LogBS_DevicePairSensorDataManager);

UBS_DevicePairSensorDataManager::UBS_DevicePairSensorDataManager()
{
    UE_LOGFMT(LogBS_DevicePairSensorDataManager, Log, "Constructor: {0}", GetName());
    if (HasAnyFlags(RF_ClassDefaultObject))
    {
        UE_LOGFMT(LogBS_DevicePairSensorDataManager, Log, "CDO - Skipping Constructor");
        return;
    }

    PressureSensorDataManager = CreateDefaultSubobject<UBS_DevicePairPressureSensorDataManager>(TEXT("PressureSensorDataManager"));
    MotionSensorDataManager = CreateDefaultSubobject<UBS_DevicePairMotionSensorDataManager>(TEXT("MotionSensorDataManager"));
    BarometerSensorDataManager = CreateDefaultSubobject<UBS_DevicePairBarometerSensorDataManager>(TEXT("BarometerSensorDataManager"));
}

void UBS_DevicePairSensorDataManager::Reset()
{
    PressureSensorDataManager->Reset();
}
