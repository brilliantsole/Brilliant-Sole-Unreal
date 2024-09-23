// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_Device.h"
#include "Logging/StructuredLog.h"
#include "BS_Subsystem.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

DEFINE_LOG_CATEGORY(LogBS_Device);

UBS_Device::UBS_Device()
{
    UE_LOGFMT(LogBS_Device, Verbose, "Constructor: {0}", GetName());
    if (HasAnyFlags(RF_ClassDefaultObject))
    {
        UE_LOGFMT(LogBS_Device, Verbose, "CDO - Skipping Constructor");
        return;
    }

    ReceivedTxMessages.Reserve(RequiredTxMessageTypes.Num());

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

    SensorDataManager->PressureSensorDataManager->OnPressureUpdate.BindUObject(this, &UBS_Device::OnPressureUpdate);

    SensorDataManager->MotionSensorDataManager->OnAccelerationUpdate.BindUObject(this, &UBS_Device::OnAccelerationUpdate);
    SensorDataManager->MotionSensorDataManager->OnGravityUpdate.BindUObject(this, &UBS_Device::OnGravityUpdate);
    SensorDataManager->MotionSensorDataManager->OnLinearAccelerationUpdate.BindUObject(this, &UBS_Device::OnLinearAccelerationUpdate);
    SensorDataManager->MotionSensorDataManager->OnGyroscopeUpdate.BindUObject(this, &UBS_Device::OnGyroscopeUpdate);
    SensorDataManager->MotionSensorDataManager->OnMagnetometerUpdate.BindUObject(this, &UBS_Device::OnMagnetometerUpdate);
    SensorDataManager->MotionSensorDataManager->OnGameRotationUpdate.BindUObject(this, &UBS_Device::OnGameRotationUpdate);
    SensorDataManager->MotionSensorDataManager->OnRotationUpdate.BindUObject(this, &UBS_Device::OnRotationUpdate);

    SensorDataManager->MotionSensorDataManager->OnOrientationUpdate.BindUObject(this, &UBS_Device::OnOrientationUpdate);
    SensorDataManager->MotionSensorDataManager->OnActivityUpdate.BindUObject(this, &UBS_Device::OnActivityUpdate);
    SensorDataManager->MotionSensorDataManager->OnStepCountUpdate.BindUObject(this, &UBS_Device::OnStepCountUpdate);
    SensorDataManager->MotionSensorDataManager->OnStepDetectionUpdate.BindUObject(this, &UBS_Device::OnStepDetectionUpdate);
    SensorDataManager->MotionSensorDataManager->OnDeviceOrientationUpdate.BindUObject(this, &UBS_Device::OnDeviceOrientationUpdate);

    SensorDataManager->BarometerSensorDataManager->OnBarometerUpdate.BindUObject(this, &UBS_Device::OnBarometerUpdate);

    SensorConfigurationManager = CreateDefaultSubobject<UBS_SensorConfigurationManager>(TEXT("SensorConfigurationManager"));
    SensorConfigurationManager->SendTxMessages.BindUObject(this, &UBS_Device::SendTxMessages);
    SensorConfigurationManager->OnSensorConfigurationUpdate.BindUObject(this, &UBS_Device::OnSensorConfigurationUpdate);

    VibrationManager = CreateDefaultSubobject<UBS_VibrationManager>(TEXT("VibrationManager"));
    VibrationManager->SendTxMessages.BindUObject(this, &UBS_Device::SendTxMessages);

    FileTransferManager = CreateDefaultSubobject<UBS_FileTransferManager>(TEXT("FileTransferManager"));
    FileTransferManager->SendTxMessages.BindUObject(this, &UBS_Device::SendTxMessages);

    FileTransferManager->OnFileTransferStatusUpdate.BindUObject(this, &UBS_Device::OnFileTransferStatusUpdate);
    FileTransferManager->OnMaxFileLengthUpdate.BindUObject(this, &UBS_Device::OnMaxFileLengthUpdate);
    FileTransferManager->OnFileReceived.BindUObject(this, &UBS_Device::OnFileReceivedUpdate);
    FileTransferManager->OnFileTransferProgress.BindUObject(this, &UBS_Device::OnFileTransferProgressUpdate);
    FileTransferManager->OnFileTransferComplete.BindUObject(this, &UBS_Device::OnFileTransferCompleteUpdate);

    TfliteManager = CreateDefaultSubobject<UBS_TfliteManager>(TEXT("TfliteManager"));
    TfliteManager->SendTxMessages.BindUObject(this, &UBS_Device::SendTxMessages);

    TfliteManager->OnNameUpdate.BindUObject(this, &UBS_Device::OnTfliteNameUpdate);
    TfliteManager->OnTaskUpdate.BindUObject(this, &UBS_Device::OnTfliteTaskUpdate);
    TfliteManager->OnSampleRateUpdate.BindUObject(this, &UBS_Device::OnTfliteSampleRateUpdate);
    TfliteManager->OnSensorTypesUpdate.BindUObject(this, &UBS_Device::OnTfliteSensorTypesUpdate);
    TfliteManager->OnIsReadyUpdate.BindUObject(this, &UBS_Device::OnTfliteIsReadyUpdate);
    TfliteManager->OnCaptureDelayUpdate.BindUObject(this, &UBS_Device::OnTfliteCaptureDelayUpdate);
    TfliteManager->OnThresholdUpdate.BindUObject(this, &UBS_Device::OnTfliteThresholdUpdate);
    TfliteManager->OnInferencingEnabledUpdate.BindUObject(this, &UBS_Device::OnTfliteInferencingEnabledUpdate);
    TfliteManager->OnInferenceUpdate.BindUObject(this, &UBS_Device::OnTfliteInferenceUpdate);
}

void UBS_Device::PostInitProperties()
{
    Super::PostInitProperties();
    UE_LOGFMT(LogBS_Device, Verbose, "PostInitProperties {0}", GetName());
    if (HasAnyFlags(RF_ClassDefaultObject))
    {
        UE_LOGFMT(LogBS_Device, Verbose, "CDO - Skipping");
        return;
    }

    GetBS_Subsystem();
    InitializeBP();
}

void UBS_Device::GetBS_Subsystem()
{
    if (!HasAnyFlags(RF_ClassDefaultObject))
    {
        UWorld *World = GetWorld();
        if (World)
        {
            UE_LOGFMT(LogBS_Device, Verbose, "World found");
            UGameInstance *GameInstance = World->GetGameInstance();
            if (GameInstance)
            {
                UE_LOGFMT(LogBS_Device, Verbose, "GameInstance found");
                UBS_Subsystem *__BS_Subsystem = GameInstance->GetSubsystem<UBS_Subsystem>();
                if (__BS_Subsystem)
                {
                    UE_LOGFMT(LogBS_Device, Verbose, "BS_Subsystem found");
                    _BS_Subsystem = __BS_Subsystem;
                }
                else
                {
                    UE_LOGFMT(LogBS_Device, Error, "BS_Subsystem not found");
                }
            }
            else
            {
                UE_LOGFMT(LogBS_Device, Error, "GameInstance not found");
            }
        }
        else
        {
            UE_LOGFMT(LogBS_Device, Error, "World not found");
        }
    }
    else
    {
        UE_LOGFMT(LogBS_Device, Verbose, "CDO constructor - skipping");
    }
}

void UBS_Device::Reset()
{
    BatteryLevel = 0;
    bDidGetBatteryLevel = false;

    ReceivedTxMessages.Reset();

    DeviceInformation.Reset();

    BatteryManager->Reset();
    InformationManager->Reset();
    SensorDataManager->Reset();
    SensorConfigurationManager->Reset();
    VibrationManager->Reset();
    FileTransferManager->Reset();
    TfliteManager->Reset();

    bIsSendingTxData = false;
}

// BATTERY LEVEL START
void UBS_Device::OnBatteryLevelUpdate(uint8 NewBatteryLevel)
{
    BatteryLevel = NewBatteryLevel;
    bDidGetBatteryLevel = true;
    UE_LOGFMT(LogBS_Device, Verbose, "BatteryLevel: {0}%", BatteryLevel);
    OnBatteryLevel.Broadcast(this, BatteryLevel);
}
// BATTERY LEVEL END

// CONNECTION START
void UBS_Device::OnConnectionManagerConnectionStatusUpdate(EBS_ConnectionStatus NewConnectionManagerConnectionStatus)
{
    UE_LOGFMT(LogBS_Device, Verbose, "ConnectionManager ConnectionStatus: {0}", UEnum::GetValueAsString(NewConnectionManagerConnectionStatus));
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
        UE_LOGFMT(LogBS_Device, Verbose, "Redundant ConnectionStatus {0} - not updating", UEnum::GetValueAsString(ConnectionStatus));
        return;
    }
    ConnectionStatus = NewConnectionStatus;
    UE_LOGFMT(LogBS_Device, Verbose, "ConnectionStatus Updated to {0}", UEnum::GetValueAsString(ConnectionStatus));
    OnConnectionStatusUpdate.Broadcast(this, ConnectionStatus);

    switch (ConnectionStatus)
    {
    case EBS_ConnectionStatus::CONNECTED:
    case EBS_ConnectionStatus::NOT_CONNECTED:
        OnIsConnectedUpdate.Broadcast(this, IsConnected());
        break;
    default:
        break;
    }
}

void UBS_Device::CheckIfFullyConnected()
{
    UE_LOGFMT(LogBS_Device, Verbose, "Checking if Fully Connected...");
    if (!bDidGetBatteryLevel)
    {
        UE_LOGFMT(LogBS_Device, Verbose, "Didn't get BatteryLevel, stopping now");
        return;
    }
    if (!DeviceInformation.GetDidGetAllInformation())
    {
        UE_LOGFMT(LogBS_Device, Verbose, "Didn't get Device Information, stopping now");
        return;
    }
    if (ConnectionStatus != EBS_ConnectionStatus::CONNECTING)
    {
        UE_LOGFMT(LogBS_Device, Verbose, "Not Connecting, stopping now");
        return;
    }
    if (InformationManager->GetCurrentTime() == 0)
    {
        UE_LOGFMT(LogBS_Device, Verbose, "CurrentTime is 0, stopping now");
        return;
    }

    bool HasReceivedAllRequiredTxMessages = true;
    for (EBS_TxRxMessage TxMessageType : RequiredTxMessageTypes)
    {
        if (!ReceivedTxMessages.Contains(TxMessageType))
        {
            UE_LOGFMT(LogBS_Device, Verbose, "Doesn't contain TxMessageType {0}", static_cast<uint8>(TxMessageType));
            HasReceivedAllRequiredTxMessages = false;
            break;
        }
    }
    UE_LOGFMT(LogBS_Device, Verbose, "HasReceivedAllRequiredTxMessages? {0}", HasReceivedAllRequiredTxMessages);
    if (!HasReceivedAllRequiredTxMessages)
    {
        return;
    }
    SetConnectionStatus(EBS_ConnectionStatus::CONNECTED);
}

// CONNECTION END

// PING START
const FBS_TxMessage UBS_Device::PingTxMessage = {EBS_TxRxMessage::GET_MTU};
const TArray<uint8> UBS_Device::PingTxData = UBS_Device::InitializePingTxData();
const TArray<uint8> UBS_Device::InitializePingTxData()
{
    TArray<uint8> _PingTxData;
    PingTxMessage.AppendTo(_PingTxData);
    return _PingTxData;
}
// PING STOP

// MESSAGING START

void UBS_Device::OnRxMessage(uint8 MessageTypeEnum, const TArray<uint8> &Message)
{
    if (MessageTypeEnum >= static_cast<uint8>(EBS_TxRxMessage::COUNT))
    {
        UE_LOGFMT(LogBS_Device, Error, "invalid TxRxMessage {0}", MessageTypeEnum);
        return;
    }
    EBS_TxRxMessage MessageType = static_cast<EBS_TxRxMessage>(MessageTypeEnum);

    UE_LOGFMT(LogBS_Device, Verbose, "message {0} ({1} bytes)", static_cast<uint8>(MessageType), Message.Num());

    if (BatteryManager->OnRxMessage(MessageType, Message))
    {
        UE_LOGFMT(LogBS_Device, Verbose, "Parsed BatteryManager Message");
    }
    else if (InformationManager->OnRxMessage(MessageType, Message))
    {
        UE_LOGFMT(LogBS_Device, Verbose, "Parsed InformationManager Message");
    }
    else if (SensorConfigurationManager->OnRxMessage(MessageType, Message))
    {
        UE_LOGFMT(LogBS_Device, Verbose, "Parsed SensorConfigurationManager Message");
    }
    else if (SensorDataManager->OnRxMessage(MessageType, Message))
    {
        UE_LOGFMT(LogBS_Device, Verbose, "Parsed SensorDataManager Message");
    }
    else if (FileTransferManager->OnRxMessage(MessageType, Message))
    {
        UE_LOGFMT(LogBS_Device, Verbose, "Parsed FileTransferManager Message");
    }
    else if (TfliteManager->OnRxMessage(MessageType, Message))
    {
        UE_LOGFMT(LogBS_Device, Verbose, "Parsed TfliteManager Message");
    }

    if (ConnectionStatus == EBS_ConnectionStatus::CONNECTING)
    {
        ReceivedTxMessages.Emplace(MessageType);
    }
}

void UBS_Device::OnRxMessages()
{
    UE_LOGFMT(LogBS_Device, Verbose, "Rx Messages received");
    SendPendingTxMessages();

    if (ConnectionStatus == EBS_ConnectionStatus::CONNECTING)
    {
        CheckIfFullyConnected();
    }
}

void UBS_Device::SendTxMessages(const TArray<FBS_TxMessage> &TxMessages, bool bSendImmediately)
{
    UE_LOGFMT(LogBS_Device, Verbose, "Requesting to send {0} Messages...", TxMessages.Num());
    PendingTxMessages.Append(TxMessages);
    if (!bSendImmediately)
    {
        UE_LOGFMT(LogBS_Device, Verbose, "Not sending data immediately");
        return;
    }
    if (bIsSendingTxData)
    {
        UE_LOGFMT(LogBS_Device, Verbose, "Already sending data - will wait until new data is sent");
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
            UE_LOGFMT(LogBS_Device, Verbose, "Appending message {0} ({1} bytes)", static_cast<uint8>(PendingTxMessage.Type), PendingTxMessageLength);
            PendingTxMessage.AppendTo(TxData);
            PendingTxMessages.RemoveAt(PendingTxMessageIndex);
        }
        else
        {
            UE_LOGFMT(LogBS_Device, Verbose, "Skipping message {0} ({1} bytes)", static_cast<uint8>(PendingTxMessage.Type), PendingTxMessageLength);
            PendingTxMessageIndex++;
        }
    }

    if (TxData.IsEmpty())
    {
        UE_LOGFMT(LogBS_Device, Verbose, "TxData is Empty - won't send any data");
        bIsSendingTxData = false;
        return;
    }

    UE_LOGFMT(LogBS_Device, Verbose, "Sending {0} bytes", TxData.Num());

    SendTxData(TxData);
}

void UBS_Device::OnSendTxData()
{
    UE_LOGFMT(LogBS_Device, Verbose, "sent tx data");
    bIsSendingTxData = false;

    BatteryManager->OnSendTxData();
    InformationManager->OnSendTxData();
    SensorDataManager->OnSendTxData();
    SensorConfigurationManager->OnSendTxData();
    VibrationManager->OnSendTxData();
    FileTransferManager->OnSendTxData();
    TfliteManager->OnSendTxData();

    SendPendingTxMessages();
}

const TArray<EBS_TxRxMessage> UBS_Device::RequiredTxMessageTypes = {
    EBS_TxRxMessage::IS_BATTERY_CHARGING,
    EBS_TxRxMessage::GET_BATTERY_CURRENT,

    EBS_TxRxMessage::GET_MTU,

    EBS_TxRxMessage::GET_ID,
    EBS_TxRxMessage::GET_NAME,
    EBS_TxRxMessage::GET_TYPE,
    EBS_TxRxMessage::GET_CURRENT_TIME,

    EBS_TxRxMessage::GET_PRESSURE_POSITIONS,
    EBS_TxRxMessage::GET_SENSOR_SCALARS,
    EBS_TxRxMessage::GET_SENSOR_CONFIGURATION,

    EBS_TxRxMessage::GET_MAX_FILE_LENGTH,
    EBS_TxRxMessage::GET_FILE_TRANSFER_TYPE,
    EBS_TxRxMessage::GET_FILE_LENGTH,
    EBS_TxRxMessage::GET_FILE_CHECKSUM,
    EBS_TxRxMessage::GET_FILE_TRANSFER_STATUS,

    EBS_TxRxMessage::GET_TFLITE_NAME,
    EBS_TxRxMessage::GET_TFLITE_TASK,
    EBS_TxRxMessage::GET_TFLITE_SAMPLE_RATE,
    EBS_TxRxMessage::GET_TFLITE_SENSOR_TYPES,
    EBS_TxRxMessage::IS_TFLITE_READY,
    EBS_TxRxMessage::GET_TFLITE_CAPTURE_DELAY,
    EBS_TxRxMessage::GET_TFLITE_THRESHOLD,
    EBS_TxRxMessage::GET_TFLITE_INFERENCING_ENABLED};

const TArray<FBS_TxMessage> UBS_Device::InitializeRequiredTxMessages()
{
    TArray<FBS_TxMessage> _RequiredTxMessages;
    for (EBS_TxRxMessage TxMessageType : UBS_Device::RequiredTxMessageTypes)
    {
        _RequiredTxMessages.Add({TxMessageType});
    }
    return _RequiredTxMessages;
}
const TArray<FBS_TxMessage> UBS_Device::RequiredTxMessages = UBS_Device::InitializeRequiredTxMessages();

// MESSAGING END