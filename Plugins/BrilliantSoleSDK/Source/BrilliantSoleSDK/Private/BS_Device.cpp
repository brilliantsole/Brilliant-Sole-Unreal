// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_Device.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(LogBS_Device);

UBS_Device::UBS_Device()
{
    DeviceInformationManager = CreateDefaultSubobject<UBS_DeviceInformationManager>(TEXT("DeviceInformationManager"));
    InformationManager = CreateDefaultSubobject<UBS_InformationManager>(TEXT("InformationManager"));
    SensorDataManager = CreateDefaultSubobject<UBS_SensorDataManager>(TEXT("SensorDataManager"));
    VibrationManager = CreateDefaultSubobject<UBS_VibrationManager>(TEXT("VibrationManager"));
    FileTransferManager = CreateDefaultSubobject<UBS_FileTransferManager>(TEXT("FileTransferManager"));
    TfliteManager = CreateDefaultSubobject<UBS_TfliteManager>(TEXT("TfliteManager"));
}