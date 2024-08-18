// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "BS_BatteryManager.h"
#include "BS_MTU_Manager.h"
#include "BS_InformationManager.h"
#include "BS_SensorConfigurationManager.h"
#include "BS_SensorDataManager.h"
#include "BS_VibrationManager.h"
#include "BS_TfliteManager.h"
#include "BS_FileTransferManager.h"

enum EBS_Message
{
    EBS_BatteryMessage,
    EBS_MTU_Message,
    EBS_DeviceIdMessage,
    EBS_DeviceNameMessage,
    EBS_DeviceTypeMessage,
    EBS_CurrentTimeMessage,
    EBS_SensorConfigurationMessage,
    EBS_PressurePositionsMessage,
    EBS_SensorScalarsMessage,
    EBS_SensorDataMessge,
    EBS_VibrationMessage,
    EBS_TfliteMessage,
    EBS_FileTransferMessage,
    BS_MessageCount
};