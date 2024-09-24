// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_ClientConnectionManager.h"
#include "Logging/StructuredLog.h"
#include "BS_DeviceInformationType.h"
#include "BS_TypeUtils.h"

DEFINE_LOG_CATEGORY(LogBS_ClientConnectionManager);

UBS_ClientConnectionManager::UBS_ClientConnectionManager()
{
    UE_LOGFMT(LogBS_ClientConnectionManager, Verbose, "Constructor: {0}", GetName());
    if (HasAnyFlags(RF_ClassDefaultObject))
    {
        UE_LOGFMT(LogBS_ClientConnectionManager, Verbose, "CDO - Skipping Constructor");
        return;
    }

    BoundOnDeviceEvent.BindUObject(this, &UBS_ClientConnectionManager::OnDeviceEvent);
}

// CLIENT START
void UBS_ClientConnectionManager::AssignClient(UBS_BaseClient *_Client)
{
    UE_LOGFMT(LogBS_ClientConnectionManager, Verbose, "Assigning Client...");
    Client = _Client;
}
// CLIENT END

// DISCOVERED DEVICE START
void UBS_ClientConnectionManager::AssignDiscoveredDevice(const FBS_DiscoveredDevice &NewDiscoveredDevice)
{
    UE_LOGFMT(LogBS_ClientConnectionManager, Verbose, "Assigning DiscoveredDevice...");
    DiscoveredDevice = NewDiscoveredDevice;
}
// DISCOVERED DEVICE END

// CONNECTION START
void UBS_ClientConnectionManager::SetIsConnected(bool bNewIsConnected)
{
    if (bNewIsConnected == bIsConnected)
    {
        UE_LOGFMT(LogBS_ClientConnectionManager, Verbose, "Redundant bIsConnected Assignment");
        return;
    }
    bIsConnected = bNewIsConnected;
    UE_LOGFMT(LogBS_ClientConnectionManager, Verbose, "Updated bIsConnected to {0}", bIsConnected);

    SetConnectionStatus(bIsConnected ? EBS_ConnectionStatus::CONNECTED : EBS_ConnectionStatus::NOT_CONNECTED);

    if (bIsConnected)
    {
        RequestDeviceInformation();
    }
}
// CONNECTION END

// MESSAGING START
void UBS_ClientConnectionManager::Connect_Implementation(bool &bContinue)
{
    Super::Connect_Implementation(bContinue);
    if (!bContinue)
    {
        UE_LOGFMT(LogBS_ClientConnectionManager, Verbose, "won't connect");
        return;
    }

    UE_LOGFMT(LogBS_ClientConnectionManager, Verbose, "Connecting...");

    if (!Client)
    {
        UE_LOGFMT(LogBS_ClientConnectionManager, Error, "Client not defined");
        return;
    }

    Client->SendConnectToDeviceMessage(DiscoveredDevice, true);
}

void UBS_ClientConnectionManager::Disconnect_Implementation(bool &bContinue)
{
    Super::Disconnect_Implementation(bContinue);
    if (!bContinue)
    {
        UE_LOGFMT(LogBS_ClientConnectionManager, Verbose, "won't disconnect");
        return;
    }

    UE_LOGFMT(LogBS_ClientConnectionManager, Verbose, "Disconnecting...");

    if (!Client)
    {
        UE_LOGFMT(LogBS_ClientConnectionManager, Error, "Client not defined");
        return;
    }
    Client->SendDisconnectFromDeviceMessage(DiscoveredDevice, true);
}

void UBS_ClientConnectionManager::SendTxData_Implementation(const TArray<uint8> &Data)
{
    Super::SendTxData_Implementation(Data);
    if (!Client)
    {
        UE_LOGFMT(LogBS_ClientConnectionManager, Error, "Client not defined");
        return;
    }

    Client->SendDeviceMessages(DiscoveredDevice, {{EBS_ConnectionMessage::TX, Data}});
    OnSendTxMessage.Broadcast(this);
}

void UBS_ClientConnectionManager::OnDeviceEvent(UBS_Device *Device, EBS_DeviceEvent DeviceEventType, const TArray<uint8> &Message)
{
    UE_LOGFMT(LogBS_ClientConnectionManager, Log, "Message {0} ({1} Bytes) for \"{2}\"", static_cast<uint8>(DeviceEventType), Message.Num(), Device->Name());
    switch (DeviceEventType)
    {
    case EBS_DeviceEvent::IS_CONNECTED:
    {
        UE_LOGFMT(LogBS_ClientConnectionManager, Log, "Received IsConnected Message");
        bool bNewIsConnected = static_cast<bool>(Message[0]);
        SetIsConnected(bNewIsConnected);
    }
    break;
    case EBS_DeviceEvent::RX:
        UE_LOGFMT(LogBS_ClientConnectionManager, Log, "Received Rx Message");
        ParseRxData(Message);
        break;

    case EBS_DeviceEvent::BATTERY_LEVEL:
    {
        uint8 BatteryLevel = Message[0];
        OnBatteryLevel.Broadcast(this, BatteryLevel);
    }
    break;

    case EBS_DeviceEvent::MANUFACTURER_NAME:
        OnDeviceInformationValue.Broadcast(this, EBS_DeviceInformation::MANUFACTURER_NAME, Message);
        break;
    case EBS_DeviceEvent::MODEL_NUMBER:
        OnDeviceInformationValue.Broadcast(this, EBS_DeviceInformation::MODEL_NUMBER, Message);
        break;
    case EBS_DeviceEvent::SOFTWARE_REVISION:
        OnDeviceInformationValue.Broadcast(this, EBS_DeviceInformation::SOFTWARE_REVISION, Message);
        break;
    case EBS_DeviceEvent::HARDWARE_REVISION:
        OnDeviceInformationValue.Broadcast(this, EBS_DeviceInformation::HARDWARE_REVISION, Message);
        break;
    case EBS_DeviceEvent::FIRMWARE_REVISION:
        OnDeviceInformationValue.Broadcast(this, EBS_DeviceInformation::FIRMWARE_REVISION, Message);
        break;
    case EBS_DeviceEvent::SERIAL_NUMBER:
        OnDeviceInformationValue.Broadcast(this, EBS_DeviceInformation::SERIAL_NUMBER, Message);
        break;

    default:
        UE_LOGFMT(LogBS_ClientConnectionManager, Log, "miscellaneous message {0}", static_cast<uint8>(DeviceEventType));

        EBS_TxRxMessage TxRxMessage = static_cast<EBS_TxRxMessage>(0);
        if (BS_TypeUtils::ConvertDeviceEventTypeToTxRxMessageType(DeviceEventType, TxRxMessage))
        {
            UE_LOGFMT(LogBS_ClientConnectionManager, Log, "TxRxMessage message {0}", static_cast<uint8>(DeviceEventType));
            OnRxMessage.Broadcast(this, TxRxMessage, static_cast<TArray<uint8>>(Message));
            OnRxMessages.Broadcast(this);
        }
        break;
    }
}
// MESSAGING END

// DEVICE INFORMATION START
const TArray<EBS_ConnectionMessage> UBS_ClientConnectionManager::RequiredDeviceInformationMessageTypes = {
    EBS_ConnectionMessage::BATTERY_LEVEL,

    EBS_ConnectionMessage::MANUFACTURER_NAME,
    EBS_ConnectionMessage::MODEL_NUMBER,
    EBS_ConnectionMessage::SERIAL_NUMBER,
    EBS_ConnectionMessage::SOFTWARE_REVISION,
    EBS_ConnectionMessage::HARDWARE_REVISION,
    EBS_ConnectionMessage::FIRMWARE_REVISION};

const TArray<FBS_ConnectionMessage> UBS_ClientConnectionManager::InitializeRequiredDeviceInformationMessages()
{
    TArray<FBS_ConnectionMessage> _RequiredDeviceInformationMessageTypes;
    for (EBS_ConnectionMessage ConnectionMessageType : UBS_ClientConnectionManager::RequiredDeviceInformationMessageTypes)
    {
        _RequiredDeviceInformationMessageTypes.Add({ConnectionMessageType});
    }
    return _RequiredDeviceInformationMessageTypes;
}
const TArray<FBS_ConnectionMessage> UBS_ClientConnectionManager::RequiredDeviceInformationMessages = UBS_ClientConnectionManager::InitializeRequiredDeviceInformationMessages();

void UBS_ClientConnectionManager::RequestDeviceInformation()
{
    if (!Client)
    {
        UE_LOGFMT(LogBS_ClientConnectionManager, Error, "Client not defined");
        return;
    }
    UE_LOGFMT(LogBS_ClientConnectionManager, Verbose, "Requesting Device information...");
    Client->SendDeviceMessages(DiscoveredDevice, RequiredDeviceInformationMessages);
}
// DEVICE INFORMATION END