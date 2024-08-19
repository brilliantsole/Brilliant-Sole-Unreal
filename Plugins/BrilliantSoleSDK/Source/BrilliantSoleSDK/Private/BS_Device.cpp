// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_Device.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(LogBS_Device);

UBS_Device::UBS_Device()
{
    DeviceInformationManager = CreateDefaultSubobject<UBS_DeviceInformationManager>(TEXT("DeviceInformationManager"));
    DeviceInformationManager->SendTxMessage.BindUObject(this, &UBS_Device::SendTxMessage);
    InformationManager = CreateDefaultSubobject<UBS_InformationManager>(TEXT("InformationManager"));
    InformationManager->SendTxMessage.BindUObject(this, &UBS_Device::SendTxMessage);
    SensorDataManager = CreateDefaultSubobject<UBS_SensorDataManager>(TEXT("SensorDataManager"));
    SensorDataManager->SendTxMessage.BindUObject(this, &UBS_Device::SendTxMessage);
    VibrationManager = CreateDefaultSubobject<UBS_VibrationManager>(TEXT("VibrationManager"));
    VibrationManager->SendTxMessage.BindUObject(this, &UBS_Device::SendTxMessage);
    FileTransferManager = CreateDefaultSubobject<UBS_FileTransferManager>(TEXT("FileTransferManager"));
    FileTransferManager->SendTxMessage.BindUObject(this, &UBS_Device::SendTxMessage);
    TfliteManager = CreateDefaultSubobject<UBS_TfliteManager>(TEXT("TfliteManager"));
    TfliteManager->SendTxMessage.BindUObject(this, &UBS_Device::SendTxMessage);
}

void UBS_Device::OnRxMessage(uint8 MessageType, const TArray<uint8> &Message)
{
    UE_LOG(LogBS_Device, Log, TEXT("message #%u (%u bytes)"), MessageType, Message.Num());

    // FILL
}

void UBS_Device::SendTxMessage(const TArray<uint8> &Message)
{
    SendTxMessage(Message, true);
}