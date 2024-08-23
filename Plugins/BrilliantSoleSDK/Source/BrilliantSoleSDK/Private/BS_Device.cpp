// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_Device.h"
#include "Logging/StructuredLog.h"
#include "BS_Message.h"

DEFINE_LOG_CATEGORY(LogBS_Device);

UBS_Device::UBS_Device()
{
    ReceivedTxMessages.Reserve(RequiredTxMessageTypes.Num());

    DeviceInformationManager = CreateDefaultSubobject<UBS_DeviceInformationManager>(TEXT("DeviceInformationManager"));

    BatteryManager = CreateDefaultSubobject<UBS_BatteryManager>(TEXT("BatteryManager"));
    BatteryManager->SendTxMessages.BindUObject(this, &UBS_Device::SendTxMessages);
    BatteryManager->OnIsBatteryChargingUpdate.BindUObject(this, &UBS_Device::OnIsBatteryChargingUpdate);
    BatteryManager->OnBatteryCurrentUpdate.BindUObject(this, &UBS_Device::OnBatteryCurrentUpdate);

    InformationManager = CreateDefaultSubobject<UBS_InformationManager>(TEXT("InformationManager"));
    InformationManager->SendTxMessages.BindUObject(this, &UBS_Device::SendTxMessages);
    InformationManager->OnMTU_Update.BindUObject(this, &UBS_Device::OnMTU_Update);
    InformationManager->OnIdUpdate.BindUObject(this, &UBS_Device::OnIdUpdate);
    InformationManager->OnNameUpdate.BindUObject(this, &UBS_Device::OnNameUpdate);
    InformationManager->OnTypeUpdate.BindUObject(this, &UBS_Device::OnTypeUpdate);
    InformationManager->OnCurrentTimeUpdate.BindUObject(this, &UBS_Device::OnCurrentTimeUpdate);

    SensorDataManager = CreateDefaultSubobject<UBS_SensorDataManager>(TEXT("SensorDataManager"));
    SensorDataManager->SendTxMessages.BindUObject(this, &UBS_Device::SendTxMessages);

    SensorConfigurationManager = CreateDefaultSubobject<UBS_SensorConfigurationManager>(TEXT("SensorConfigurationManager"));
    SensorConfigurationManager->SendTxMessages.BindUObject(this, &UBS_Device::SendTxMessages);
    SensorConfigurationManager->OnSensorConfigurationUpdate.BindUObject(this, &UBS_Device::OnSensorConfigurationUpdate);

    VibrationManager = CreateDefaultSubobject<UBS_VibrationManager>(TEXT("VibrationManager"));
    VibrationManager->SendTxMessages.BindUObject(this, &UBS_Device::SendTxMessages);
    FileTransferManager = CreateDefaultSubobject<UBS_FileTransferManager>(TEXT("FileTransferManager"));
    FileTransferManager->SendTxMessages.BindUObject(this, &UBS_Device::SendTxMessages);
    TfliteManager = CreateDefaultSubobject<UBS_TfliteManager>(TEXT("TfliteManager"));
    TfliteManager->SendTxMessages.BindUObject(this, &UBS_Device::SendTxMessages);
}

void UBS_Device::Reset()
{
    ReceivedTxMessages.Reset();

    DeviceInformationManager->Reset();
    BatteryManager->Reset();
    InformationManager->Reset();
    SensorDataManager->Reset();
    SensorConfigurationManager->Reset();
    VibrationManager->Reset();
    FileTransferManager->Reset();
    TfliteManager->Reset();
}

// CONNECTION START
void UBS_Device::OnConnectionManagerConnectionStatusUpdate(EBS_ConnectionStatus NewConnectionManagerConnectionStatus)
{
    UE_LOGFMT(LogBS_Device, Log, "ConnectionManager ConnectionStatus: {0}", UEnum::GetValueAsString(NewConnectionManagerConnectionStatus));
    switch (NewConnectionManagerConnectionStatus)
    {
    case EBS_ConnectionStatus::CONNECTED:
        SendTxMessages(UBS_Device::RequiredTxMessages);
        break;
    case EBS_ConnectionStatus::NOT_CONNECTED:
        Reset();
        break;
    }

    if (NewConnectionManagerConnectionStatus != EBS_ConnectionStatus::CONNECTED)
    {
        SetConnectionStatus(NewConnectionManagerConnectionStatus);
    }
}

void UBS_Device::SetConnectionStatus(EBS_ConnectionStatus NewConnectionStatus)
{
    if (ConnectionStatus == NewConnectionStatus)
    {
        UE_LOGFMT(LogBS_Device, Log, "Redundant ConnectionStatus {0} - not updating", UEnum::GetValueAsString(ConnectionStatus));
        return;
    }
    ConnectionStatus = NewConnectionStatus;
    UE_LOGFMT(LogBS_Device, Log, "ConnectionStatus Updated to {0}", UEnum::GetValueAsString(ConnectionStatus));
    OnConnectionStatusUpdate.Broadcast(ConnectionStatus);
}

void UBS_Device::CheckIfFullyConnected()
{
    UE_LOGFMT(LogBS_Device, Log, "Checking if Fully Connected...");
    if (ConnectionStatus != EBS_ConnectionStatus::CONNECTING)
    {
        UE_LOGFMT(LogBS_Device, Log, "Not Connecting, stopping now");
        return;
    }
    if (InformationManager->GetCurrentTime() == 0)
    {
        UE_LOGFMT(LogBS_Device, Log, "CurrentTime is 0, stopping now");
        return;
    }

    bool HasReceivedAllRequiredTxMessages = true;
    for (uint8 TxMessageType : RequiredTxMessageTypes)
    {
        if (!ReceivedTxMessages.Contains(TxMessageType))
        {
            UE_LOGFMT(LogBS_Device, Log, "Doesn't contain TxMessageType #{0}", TxMessageType);
            HasReceivedAllRequiredTxMessages = false;
            break;
        }
    }
    UE_LOGFMT(LogBS_Device, Log, "HasReceivedAllRequiredTxMessages? {0}", HasReceivedAllRequiredTxMessages);
    if (!HasReceivedAllRequiredTxMessages)
    {
        return;
    }
    SetConnectionStatus(EBS_ConnectionStatus::CONNECTED);
}

// CONNECTION END

// MESSAGING START
void UBS_Device::OnRxMessage(uint8 MessageType, const TArray<uint8> &Message)
{
    UE_LOGFMT(LogBS_Device, Log, "message #{0} ({1} bytes)", MessageType, Message.Num());

    if (BatteryManager->OnRxMessage(MessageType, Message))
    {
        UE_LOGFMT(LogBS_Device, Log, "Parsed BatteryManager Message");
    }
    else if (InformationManager->OnRxMessage(MessageType, Message))
    {
        UE_LOGFMT(LogBS_Device, Log, "Parsed InformationManager Message");
    }
    else if (SensorConfigurationManager->OnRxMessage(MessageType, Message))
    {
        UE_LOGFMT(LogBS_Device, Log, "Parsed SensorConfigurationManager Message");
    }
    else if (SensorDataManager->OnRxMessage(MessageType, Message))
    {
        UE_LOGFMT(LogBS_Device, Log, "Parsed SensorDataManager Message");
    }
    else if (FileTransferManager->OnRxMessage(MessageType, Message))
    {
        UE_LOGFMT(LogBS_Device, Log, "Parsed FileTransferManager Message");
    }
    else if (TfliteManager->OnRxMessage(MessageType, Message))
    {
        UE_LOGFMT(LogBS_Device, Log, "Parsed TfliteManager Message");
    }

    if (ConnectionStatus == EBS_ConnectionStatus::CONNECTING)
    {
        ReceivedTxMessages.Emplace(MessageType);
        CheckIfFullyConnected();
    }
}

void UBS_Device::SendTxMessages(const TArray<FBS_TxMessage> &TxMessages, bool bSendImmediately)
{
    PendingTxMessages.Append(TxMessages);
    if (!bSendImmediately || bIsSendingTxData)
    {
        return;
    }
    SendPendingTxMessages();
}
void UBS_Device::SendPendingTxMessages()
{
    if (PendingTxMessages.IsEmpty())
    {
        return;
    }
    bIsSendingTxData = true;

    TxData.Reset();

    const auto MaxMessageLength = InformationManager->GetMaxTxMessageLength();

    uint8 PendingTxMessageIndex = 0;
    while (PendingTxMessageIndex < PendingTxMessages.Num())
    {
        const FBS_TxMessage &PendingTxMessage = PendingTxMessages[PendingTxMessageIndex];

        uint16 PendingTxMessageLength = PendingTxMessage.Num();

        bool ShouldAppendTxMessage = MaxMessageLength == 0 || TxData.Num() + PendingTxMessageLength <= MaxMessageLength;
        if (ShouldAppendTxMessage)
        {
            UE_LOGFMT(LogBS_Device, Log, "Appending message #{0} ({1} bytes)", PendingTxMessage.Type, PendingTxMessageLength);
            PendingTxMessage.AppendTo(TxData);
            PendingTxMessages.RemoveAt(PendingTxMessageIndex);
        }
        else
        {
            UE_LOGFMT(LogBS_Device, Log, "Skipping message #{0} ({1} bytes)", PendingTxMessage.Type, PendingTxMessageLength);
            PendingTxMessageIndex++;
        }
    }

    if (TxData.IsEmpty())
    {
        UE_LOGFMT(LogBS_Device, Log, "TxData is Empty - won't send any data");
        bIsSendingTxData = false;
        return;
    }

    UE_LOGFMT(LogBS_Device, Log, "Sending {0} bytes", TxData.Num());

    SendTxData(TxData);
    TxData.Reset();
}

void UBS_Device::OnSendTxData()
{
    UE_LOGFMT(LogBS_Device, Log, "sent tx data");
    bIsSendingTxData = false;
    SendPendingTxMessages();
}

const TArray<uint8> UBS_Device::RequiredTxMessageTypes = {
    BS_MessageIsBatteryCharging,
    BS_MessageGetBatteryCurrent,

    BS_MessageGetId,
    BS_MessageGetName,
    BS_MessageGetType,
    BS_MessageGetCurrentTime,

    BS_MessageGetSensorConfiguration,
    BS_MessageGetSensorScalars,
    BS_MessageGetPressurePositions,

    BS_MessageGetMaxFileLength,
    BS_MessageGetFileTransferType,
    BS_MessageGetFileLength,
    BS_MessageGetFileChecksum,
    BS_MessageGetFileTransferStatus,

    BS_MessageTfliteGetName,
    BS_MessageTfliteGetTask,
    BS_MessageTfliteGetSampleRate,
    BS_MessageTfliteGetSensorTypes,
    BS_MessageTfliteGetIsReady,
    BS_MessageTfliteGetCaptureDelay,
    BS_MessageTfliteGetThreshold,
    BS_MessageTfliteGetInferencingEnabled};

const TArray<FBS_TxMessage> UBS_Device::InitializeRequiredTxMessages()
{
    TArray<FBS_TxMessage> _RequiredTxMessages;
    for (uint8 TxMessageType : UBS_Device::RequiredTxMessageTypes)
    {
        _RequiredTxMessages.Add({TxMessageType});
    }
    return _RequiredTxMessages;
}
const TArray<FBS_TxMessage> UBS_Device::RequiredTxMessages = UBS_Device::InitializeRequiredTxMessages();

// MESSAGING END