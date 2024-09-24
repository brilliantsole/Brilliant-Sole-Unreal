// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_BaseClient.h"
#include "Logging/StructuredLog.h"
#include "BS_TxRxMessageType.h"
#include "BS_Subsystem.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "BS_ByteParser.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "BS_DeviceType.h"

DEFINE_LOG_CATEGORY(LogBS_BaseClient);

UBS_BaseClient::UBS_BaseClient()
{
    UE_LOGFMT(LogBS_BaseClient, Verbose, "Constructor: {0}", GetName());
    if (HasAnyFlags(RF_ClassDefaultObject))
    {
        UE_LOGFMT(LogBS_BaseClient, Verbose, "CDO - Skipping Constructor");
        return;
    }

    BoundOnMessage.BindUObject(this, &UBS_BaseClient::OnMessage);
    BoundOnDeviceEvent.BindUObject(this, &UBS_BaseClient::OnDeviceEvent);
}

void UBS_BaseClient::Reset()
{
    bIsScanningAvailable = false;
    bIsScanning = false;

    DiscoveredDevices.Reset();
    Devices.Reset();
}

void UBS_BaseClient::PostInitProperties()
{
    Super::PostInitProperties();
    UE_LOGFMT(LogBS_BaseClient, Verbose, "PostInitProperties {0}", GetName());
    if (HasAnyFlags(RF_ClassDefaultObject))
    {
        UE_LOGFMT(LogBS_BaseClient, Verbose, "CDO - Skipping");
        return;
    }

    GetBS_Subsystem();
    InitializeBP();
}

void UBS_BaseClient::GetBS_Subsystem()
{
    if (!HasAnyFlags(RF_ClassDefaultObject))
    {
        UWorld *World = GetWorld();
        if (World)
        {
            UE_LOGFMT(LogBS_BaseClient, Verbose, "World found");
            UGameInstance *GameInstance = World->GetGameInstance();
            if (GameInstance)
            {
                UE_LOGFMT(LogBS_BaseClient, Verbose, "GameInstance found");
                UBS_Subsystem *__BS_Subsystem = GameInstance->GetSubsystem<UBS_Subsystem>();
                if (__BS_Subsystem)
                {
                    UE_LOGFMT(LogBS_BaseClient, Verbose, "BS_Subsystem found");
                    _BS_Subsystem = __BS_Subsystem;
                }
                else
                {
                    UE_LOGFMT(LogBS_BaseClient, Error, "BS_Subsystem not found");
                }
            }
            else
            {
                UE_LOGFMT(LogBS_BaseClient, Error, "GameInstance not found");
            }
        }
        else
        {
            UE_LOGFMT(LogBS_BaseClient, Error, "World not found");
        }
    }
    else
    {
        UE_LOGFMT(LogBS_BaseClient, Verbose, "CDO constructor - skipping");
    }
}

// CONNECTION START
void UBS_BaseClient::SetConnectionStatus(EBS_ConnectionStatus NewConnectionStatus)
{
    if (ConnectionStatus == NewConnectionStatus)
    {
        UE_LOGFMT(LogBS_BaseClient, Verbose, "Redundant ConnectionStatus {0} - not updating", UEnum::GetValueAsString(ConnectionStatus));
        return;
    }
    ConnectionStatus = NewConnectionStatus;
    UE_LOGFMT(LogBS_BaseClient, Verbose, "ConnectionStatus Updated to {0}", UEnum::GetValueAsString(ConnectionStatus));
    OnConnectionStatusUpdate.Broadcast(this, ConnectionStatus);

    switch (ConnectionStatus)
    {
    case EBS_ConnectionStatus::CONNECTED:
    case EBS_ConnectionStatus::NOT_CONNECTED:
        OnIsConnectedUpdate.Broadcast(this, GetIsConnected());
        if (ConnectionStatus == EBS_ConnectionStatus::CONNECTED)
        {
            SendRequiredMessages();
        }
        else
        {
            Reset();
        }
        break;
    default:
        break;
    }
}
// CONNECTION END

// MESSAGING START
void UBS_BaseClient::OnMessage(EBS_ServerMessage MessageType, const TArray<uint8> &Message)
{
    UE_LOGFMT(LogBS_BaseClient, Verbose, "message {0} ({1} bytes)", UEnum::GetValueAsString(MessageType), Message.Num());

    switch (MessageType)
    {
    case EBS_ServerMessage::IS_SCANNING_AVAILABLE:
        ParseIsScanningAvailable(Message);
        break;
    case EBS_ServerMessage::IS_SCANNING:
        ParseIsScanning(Message);
        break;
    case EBS_ServerMessage::DISCOVERED_DEVICE:
        ParseDiscoveredDevice(Message);
        break;
    case EBS_ServerMessage::EXPIRED_DISCOVERED_DEVICE:
        ParseExpiredDiscoveredDevice(Message);
        break;
    case EBS_ServerMessage::CONNECTED_DEVICES:
        ParseDevices(Message);
        break;
    case EBS_ServerMessage::DEVICE_MESSAGE:
        ParseDeviceMessage(Message);
        break;
    default:
        UE_LOGFMT(LogBS_BaseClient, Error, "Uncaught MessageType {0}", UEnum::GetValueAsString(MessageType));
        break;
    }
}

void UBS_BaseClient::OnData(const TArray<uint8> &Data)
{
    UE_LOGFMT(LogBS_BaseClient, Log, "Parsing {0} Bytes...", Data.Num());
    UBS_ParseUtils::ParseServerData(Data, BoundOnMessage);
}

void UBS_BaseClient::SendMessages(const TArray<FBS_ServerMessage> &ServerMessages, bool bSendImmediately)
{
    TArray<uint8> MessageData;

    for (const FBS_ServerMessage &ServerMessage : ServerMessages)
    {
        ServerMessage.AppendTo(MessageData);
    }

    if (MessageData.IsEmpty())
    {
        UE_LOGFMT(LogBS_BaseClient, Verbose, "MessageData is Empty - won't send any data");
        return;
    }

    UE_LOGFMT(LogBS_BaseClient, Verbose, "Sending {0} bytes", MessageData.Num());

    SendMessageData(MessageData, bSendImmediately);
}

const TArray<EBS_ServerMessage> UBS_BaseClient::RequiredMessageTypes = {EBS_ServerMessage::IS_SCANNING_AVAILABLE, EBS_ServerMessage::DISCOVERED_DEVICES, EBS_ServerMessage::CONNECTED_DEVICES};

const TArray<FBS_ServerMessage> UBS_BaseClient::InitializeRequiredMessages()
{
    TArray<FBS_ServerMessage> _RequiredMessages;
    for (const EBS_ServerMessage MessageType : UBS_BaseClient::RequiredMessageTypes)
    {
        _RequiredMessages.Add({MessageType});
    }
    return _RequiredMessages;
}
const TArray<FBS_ServerMessage> UBS_BaseClient::RequiredMessages = UBS_BaseClient::InitializeRequiredMessages();
// MESSAGING END

// SCANNING START
void UBS_BaseClient::ParseIsScanningAvailable(const TArray<uint8> &Message)
{
    bool bNewIsScanningAvailable = static_cast<bool>(Message[0]);
    UE_LOGFMT(LogBS_BaseClient, Verbose, "bNewIsScanningAvailable: {0}", bNewIsScanningAvailable);

    if (bNewIsScanningAvailable == bIsScanningAvailable)
    {
        UE_LOGFMT(LogBS_BaseClient, Verbose, "Redundant bIsScanningAvailable Assignment");
        return;
    }

    bIsScanningAvailable = bNewIsScanningAvailable;
    UE_LOGFMT(LogBS_BaseClient, Verbose, "Updated bIsScanningAvailable to {0}", bIsScanningAvailable);
    OnIsScanningAvailableUpdate.Broadcast(this, bIsScanningAvailable);

    if (bIsScanningAvailable)
    {
        UE_LOGFMT(LogBS_BaseClient, Verbose, "Checking if IsScanning...");
        SendMessages({{EBS_ServerMessage::IS_SCANNING}});
    }
}
void UBS_BaseClient::ParseIsScanning(const TArray<uint8> &Message)
{
    bool bNewIsScanning = static_cast<bool>(Message[0]);
    UE_LOGFMT(LogBS_BaseClient, Verbose, "bNewIsScanning: {0}", bNewIsScanning);

    if (bNewIsScanning == bIsScanning)
    {
        UE_LOGFMT(LogBS_BaseClient, Verbose, "Redundant bIsScanning Assignment");
        return;
    }

    bIsScanning = bNewIsScanning;
    UE_LOGFMT(LogBS_BaseClient, Verbose, "Updated bIsScanning to {0}", bIsScanning);
    OnIsScanningUpdate.Broadcast(this, bIsScanning);
}

void UBS_BaseClient::StartScan()
{
    if (!bIsScanningAvailable)
    {
        UE_LOGFMT(LogBS_BaseClient, Verbose, "Scanning is not available");
        return;
    }
    if (bIsScanning)
    {
        UE_LOGFMT(LogBS_BaseClient, Verbose, "Already scanning");
        return;
    }
    DiscoveredDevices.Reset();
    SendMessages({{EBS_ServerMessage::START_SCAN}});
}
void UBS_BaseClient::StopScan()
{
    if (!bIsScanning)
    {
        UE_LOGFMT(LogBS_BaseClient, Verbose, "Already not scanning");
        return;
    }
    SendMessages({{EBS_ServerMessage::STOP_SCAN}});
}

void UBS_BaseClient::ToggleScan()
{
    if (bIsScanning)
    {
        StopScan();
    }
    else
    {
        StartScan();
    }
}
// SCANNING END

// DISCOVERED DEVICES START
void UBS_BaseClient::ParseDiscoveredDevice(const TArray<uint8> &Message)
{
    UE_LOGFMT(LogBS_BaseClient, Log, "Parsing Discovered Device ({0} bytes)...", Message.Num());

    FBS_DiscoveredDevice DiscoveredDevice;
    if (DiscoveredDevice.Parse(Message))
    {
        DiscoveredDevices.Emplace(DiscoveredDevice.BluetoothId, DiscoveredDevice);
        OnDiscoveredDevice.Broadcast(this, DiscoveredDevice);
    }
    else
    {
        UE_LOGFMT(LogBS_BaseClient, Error, "Failed to parse Discovered Device");
    }
}
void UBS_BaseClient::ParseExpiredDiscoveredDevice(const TArray<uint8> &Message)
{
    UE_LOGFMT(LogBS_BaseClient, Log, "Parsing Expired Discovered Device ({0} bytes)...", Message.Num());
    // FILL
}
// DISCOVERED DEVICES END

// DEVICE CONNECTION START
UBS_Device *UBS_BaseClient::ConnectToDevice_Implementation(const FBS_DiscoveredDevice &DiscoveredDevice)
{
    UBS_Device *Device = GetDeviceByDiscoveredDevice(DiscoveredDevice);
    if (Device)
    {
        Device->Connect();
        return Device;
    }
    return nullptr;
}

UBS_Device *UBS_BaseClient::DisconnectFromDevice_Implementation(const FBS_DiscoveredDevice &DiscoveredDevice)
{
    UBS_Device *Device = GetDeviceByDiscoveredDevice(DiscoveredDevice);
    if (Device)
    {
        Device->Disconnect();
        return Device;
    }
    return nullptr;
}

UBS_Device *UBS_BaseClient::ToggleDeviceConnection(const FBS_DiscoveredDevice &DiscoveredDevice)
{
    UBS_Device *Device = GetDeviceByDiscoveredDevice(DiscoveredDevice);
    if (Device)
    {
        Device->ToggleConnection();
        return Device;
    }
    return nullptr;
}
// DEVICE CONNECTION END

// DEVICES START

UBS_Device *UBS_BaseClient::GetDeviceByDiscoveredDevice(const FBS_DiscoveredDevice &DiscoveredDevice)
{
    UBS_Device *FoundDevice = nullptr;
    for (const TPair<FString, UBS_Device *> &Pair : Devices)
    {
        if (Pair.Key == DiscoveredDevice.BluetoothId)
        {
            FoundDevice = Pair.Value;
            break;
        }
    }
    return FoundDevice;
}
void UBS_BaseClient::ParseDevices(const TArray<uint8> &Message)
{
    UE_LOGFMT(LogBS_BaseClient, Log, "Parsing Connected Devices ({0} bytes)...", Message.Num());
    // FILL
}
// DEVICES END

// DEVICE MESSAGE START
void UBS_BaseClient::SendConnectToDeviceMessage(const FBS_DiscoveredDevice &DiscoveredDevice, bool bSendImmediately)
{
    UE_LOGFMT(LogBS_BaseClient, Log, "Requesting connection to {0}...", DiscoveredDevice.BluetoothId);
    const TArray<uint8> TxMessage = BS_ByteParser::StringToArray(DiscoveredDevice.BluetoothId, true);
    SendMessages({{EBS_ServerMessage::CONNECT_TO_DEVICE, TxMessage}});
}
void UBS_BaseClient::SendDisconnectFromDeviceMessage(const FBS_DiscoveredDevice &DiscoveredDevice, bool bSendImmediately)
{
    UE_LOGFMT(LogBS_BaseClient, Log, "Requesting disconnection from {0}...", DiscoveredDevice.BluetoothId);
    const TArray<uint8> TxMessage = BS_ByteParser::StringToArray(DiscoveredDevice.BluetoothId, true);
    SendMessages({{EBS_ServerMessage::DISCONNECT_FROM_DEVICE, TxMessage}});
}
void UBS_BaseClient::SendDeviceMessage(const FBS_DiscoveredDevice &DiscoveredDevice, const TArray<uint8> &Message, bool bSendImmediately)
{
    UE_LOGFMT(LogBS_BaseClient, Log, "Sending {0} bytes to {1}...", Message.Num(), DiscoveredDevice.BluetoothId);
    // FILL
    // SendMessages({{EBS_ServerMessage::DEVICE_MESSAGE, Message}});
}

void UBS_BaseClient::ParseDeviceMessage(const TArray<uint8> &Message)
{
    const uint16 MessageLength = Message.Num();

    UE_LOGFMT(LogBS_BaseClient, Log, "Parsing Device Message ({0} bytes)...", MessageLength);
    uint8 Offset = 0;
    const FString BluetoothId = BS_ByteParser::GetString(Message, true);
    Offset += 1 + BluetoothId.Len();

    UE_LOGFMT(LogBS_BaseClient, Log, "Received Device Message for BluetoothId: {0}", BluetoothId);
    if (!Devices.Contains(BluetoothId))
    {
        UE_LOGFMT(LogBS_BaseClient, Error, "No Device found with BluetoothId {0}", BluetoothId);
        return;
    }

    UBS_Device *Device = Devices[BluetoothId];

    const uint16 MessageDataLength = MessageLength - Offset;
    const TArrayView<uint8> MessageData((uint8 *)(Message.GetData() + Offset), MessageDataLength);

    UE_LOGFMT(LogBS_BaseClient, Log, "Parsing {0} Bytes for Device...", MessageDataLength);
    UBS_ParseUtils::ParseDeviceEventData(Device, static_cast<TArray<uint8>>(MessageData), BoundOnDeviceEvent);
}

void UBS_BaseClient::OnDeviceEvent(UBS_Device *Device, EBS_DeviceEvent DeviceEventType, const TArray<uint8> &Message)
{
    UE_LOGFMT(LogBS_BaseClient, Log, "Message {0} ({1} Bytes) for \"{2}\"", static_cast<uint8>(DeviceEventType), Message.Num(), Device->Name());
    switch (DeviceEventType)
    {
    case EBS_DeviceEvent::IS_CONNECTED:
        UE_LOGFMT(LogBS_BaseClient, Log, "Received IsConnected Message");
        // FILL - set connectionStasus
        break;
    case EBS_DeviceEvent::RX:
        UE_LOGFMT(LogBS_BaseClient, Log, "Received Rx Message");
        // FILL - parse RX message
        break;
    default:
        UE_LOGFMT(LogBS_BaseClient, Log, "Miscellaneous message {0}", static_cast<uint8>(DeviceEventType));
        // FILL - onMessageReceived
        break;
    }
}

// DEVICE MESSAGE END