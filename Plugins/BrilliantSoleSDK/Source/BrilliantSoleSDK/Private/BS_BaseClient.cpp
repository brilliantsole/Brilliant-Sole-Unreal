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
#include "BS_ClientConnectionManager.h"

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
}

void UBS_BaseClient::Reset()
{
    bIsScanningAvailable = false;
    bIsScanning = false;

    for (const TPair<FString, UBS_Device *> &Pair : Devices)
    {
        UBS_Device *Device = Pair.Value;

        UBS_ClientConnectionManager *ConnectionManager = Cast<UBS_ClientConnectionManager>(Device->GetConnectionManager());
        if (!ConnectionManager)
        {
            UE_LOGFMT(LogBS_BaseClient, Error, "Failed to cast ConnectionManager to ClientConnectionManager");
            return;
        }
        // Device->SetConnectionStatus(EBS_ConnectionStatus::DISCONNECTING);
        ConnectionManager->SetIsConnected(false);
        if (!Device->IsAvailable())
        {
            // Device->Remove();
        }
    }

    DiscoveredDevices.Reset();
    // Devices.Reset();

    ReceivedMessages.Reset();
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
            // SendRequiredMessages();
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

void UBS_BaseClient::CheckIfFullyConnected()
{
    if (ConnectionStatus != EBS_ConnectionStatus::CONNECTING)
    {
        return;
    }
    UE_LOGFMT(LogBS_BaseClient, Verbose, "Checking if fully connected");

    if (!ReceivedMessages.Contains(EBS_ServerMessage::IS_SCANNING_AVAILABLE))
    {
        UE_LOGFMT(LogBS_BaseClient, Verbose, "Not Fully Connected - didn't receive {0}", UEnum::GetValueAsString(EBS_ServerMessage::IS_SCANNING_AVAILABLE));
        return;
    }

    if (GetIsScanningAvailable())
    {
        if (!ReceivedMessages.Contains(EBS_ServerMessage::IS_SCANNING))
        {
            UE_LOGFMT(LogBS_BaseClient, Verbose, "Not Fully Connected - didn't receive {0}", UEnum::GetValueAsString(EBS_ServerMessage::IS_SCANNING));
            return;
        }
    }

    UE_LOGFMT(LogBS_BaseClient, Verbose, "Fully Connected");
    SetConnectionStatus(EBS_ConnectionStatus::CONNECTED);
}
// CONNECTION END

// MESSAGING START
void UBS_BaseClient::OnMessage(EBS_ServerMessage MessageType, const TArrayView<const uint8> &Message)
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
        ParseConnectedDevices(Message);
        break;
    case EBS_ServerMessage::DEVICE_MESSAGE:
        ParseDeviceMessage(Message);
        break;
    default:
        UE_LOGFMT(LogBS_BaseClient, Error, "Uncaught MessageType {0}", UEnum::GetValueAsString(MessageType));
        break;
    }

    if (ConnectionStatus == EBS_ConnectionStatus::CONNECTING)
    {
        ReceivedMessages.Emplace(MessageType);
    }
}

void UBS_BaseClient::OnData(const TArrayView<const uint8> &Data)
{
    UE_LOGFMT(LogBS_BaseClient, Verbose, "Parsing {0} Bytes...", Data.Num());
    UBS_ParseUtils::ParseServerData(Data, BoundOnMessage);
    CheckIfFullyConnected();
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
void UBS_BaseClient::ParseIsScanningAvailable(const TArrayView<const uint8> &Message)
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
void UBS_BaseClient::ParseIsScanning(const TArrayView<const uint8> &Message)
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
void UBS_BaseClient::ParseDiscoveredDevice(const TArrayView<const uint8> &Message)
{
    UE_LOGFMT(LogBS_BaseClient, Verbose, "Parsing Discovered Device ({0} bytes)...", Message.Num());

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
void UBS_BaseClient::ParseExpiredDiscoveredDevice(const TArrayView<const uint8> &Message)
{
    UE_LOGFMT(LogBS_BaseClient, Verbose, "Parsing Expired Discovered Device ({0} bytes)...", Message.Num());
    const FString BluetoothId = BS_ByteParser::GetString(Message, true);
    UE_LOGFMT(LogBS_BaseClient, Verbose, "Expired Discovered Device for BluetoothId: {0}", BluetoothId);
    if (!DiscoveredDevices.Contains(BluetoothId))
    {
        UE_LOGFMT(LogBS_BaseClient, Error, "No DiscoveredDevice found with BluetoothId {0}", BluetoothId);
        return;
    }

    const FBS_DiscoveredDevice ExpiredDiscoveredDevice = DiscoveredDevices[BluetoothId];
    UE_LOGFMT(LogBS_BaseClient, Verbose, "Expiring Discovered Device with name \"{0}\" and BluetoothId \"{1}\"", ExpiredDiscoveredDevice.Name, ExpiredDiscoveredDevice.BluetoothId);
    DiscoveredDevices.Remove(BluetoothId);
    OnExpiredDiscoveredDevice.Broadcast(this, ExpiredDiscoveredDevice);
}
// DISCOVERED DEVICES END

// DEVICE CONNECTION START
UBS_Device *UBS_BaseClient::ConnectToDevice(const FBS_DiscoveredDevice &DiscoveredDevice)
{
    UBS_Device *Device = GetDeviceByDiscoveredDevice(DiscoveredDevice);
    if (Device)
    {
        Device->Connect();
        return Device;
    }
    Device = CreateDevice(DiscoveredDevice.BluetoothId);
    if (!Device)
    {
        UE_LOGFMT(LogBS_BaseClient, Error, "Unable to create device");
        return nullptr;
    }
    Device->Connect();
    return Device;
}
UBS_Device *UBS_BaseClient::CreateDevice(const FString &BluetoothId)
{
    UBS_Device *Device = GetDeviceByBluetoothId(BluetoothId);
    if (Device)
    {
        UE_LOGFMT(LogBS_BaseClient, Verbose, "Aready Created device for BluetoothId {0}", BluetoothId);
        return Device;
    }
    Device = Cast<UBS_Device>(_BS_Subsystem->CreateDevice());
    if (!Device)
    {
        UE_LOGFMT(LogBS_BaseClient, Verbose, "Failed to create Device");
        return nullptr;
    }
    UBS_ClientConnectionManager *ConnectionManager = NewObject<UBS_ClientConnectionManager>(this);
    ConnectionManager->AssignClient(this);
    ConnectionManager->AssignBluetoothId(BluetoothId);
    Device->AssignConnectionManager(ConnectionManager);
    AddDevice(BluetoothId, Device);
    return Device;
}

UBS_Device *UBS_BaseClient::DisconnectFromDevice(const FBS_DiscoveredDevice &DiscoveredDevice)
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
    return GetDeviceByBluetoothId(DiscoveredDevice.BluetoothId);
}
UBS_Device *UBS_BaseClient::GetDeviceByBluetoothId(const FString &BluetoothId)
{
    UBS_Device *FoundDevice = nullptr;
    for (const TPair<FString, UBS_Device *> &Pair : Devices)
    {
        if (Pair.Key == BluetoothId)
        {
            FoundDevice = Pair.Value;
            break;
        }
    }
    return FoundDevice;
}
void UBS_BaseClient::ParseConnectedDevices(const TArrayView<const uint8> &Message)
{
    UE_LOGFMT(LogBS_BaseClient, Verbose, "Parsing Connected Devices ({0} bytes)...", Message.Num());
    const FString ConnectedDeviceBluetoothIdsString = BS_ByteParser::GetString(Message, true);
    UE_LOGFMT(LogBS_BaseClient, Verbose, "ConnectedDeviceBluetoothIdsString: {0}", ConnectedDeviceBluetoothIdsString);

    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ConnectedDeviceBluetoothIdsString);
    TSharedPtr<FJsonObject> JsonObject;

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        const TArray<TSharedPtr<FJsonValue>> *ConnectedDeviceBluetoothIds;
        if (JsonObject->TryGetArrayField(TEXT("connectedDevices"), ConnectedDeviceBluetoothIds))
        {
            for (const TSharedPtr<FJsonValue> &Value : *ConnectedDeviceBluetoothIds)
            {
                FString ConnectedDeviceBluetoothId = Value->AsString();
                UE_LOGFMT(LogBS_BaseClient, Verbose, "ConnectedDeviceBluetoothId: {0}", ConnectedDeviceBluetoothId);

                UBS_Device *Device = CreateDevice(ConnectedDeviceBluetoothId);
                UBS_ClientConnectionManager *ConnectionManager = Cast<UBS_ClientConnectionManager>(Device->GetConnectionManager());
                if (!ConnectionManager)
                {
                    UE_LOGFMT(LogBS_BaseClient, Error, "Failed to cast ConnectionManager to ClientConnectionManager");
                    return;
                }

                Device->SetConnectionStatus(EBS_ConnectionStatus::CONNECTING);
                ConnectionManager->SetIsConnected(true);
            }
        }
        else
        {
            UE_LOGFMT(LogBS_BaseClient, Error, "Failed to find 'connectedDevices' array in JSON");
        }
    }
    else
    {
        UE_LOGFMT(LogBS_BaseClient, Error, "Unable to parse ConnectedDeviceBluetoothIdsString as JSON object");
    }
}
// DEVICES END

// DEVICE MESSAGE START
void UBS_BaseClient::SendConnectToDeviceMessage(const FBS_DiscoveredDevice &DiscoveredDevice, bool bSendImmediately)
{
    UE_LOGFMT(LogBS_BaseClient, Verbose, "Requesting connection to {0}...", DiscoveredDevice.BluetoothId);
    const TArray<uint8> Message = BS_ByteParser::StringToArray(DiscoveredDevice.BluetoothId, true);
    SendMessages({{EBS_ServerMessage::CONNECT_TO_DEVICE, Message}});
}
void UBS_BaseClient::SendDisconnectFromDeviceMessage(const FBS_DiscoveredDevice &DiscoveredDevice, bool bSendImmediately)
{
    UE_LOGFMT(LogBS_BaseClient, Verbose, "Requesting disconnection from {0}...", DiscoveredDevice.BluetoothId);
    const TArray<uint8> Message = BS_ByteParser::StringToArray(DiscoveredDevice.BluetoothId, true);
    SendMessages({{EBS_ServerMessage::DISCONNECT_FROM_DEVICE, Message}});
}
void UBS_BaseClient::SendDeviceMessages(const FBS_DiscoveredDevice &DiscoveredDevice, const TArray<FBS_ConnectionMessage> &Messages, bool bSendImmediately)
{
    UE_LOGFMT(LogBS_BaseClient, Verbose, "Sending {0} messages to {1}...", Messages.Num(), DiscoveredDevice.BluetoothId);
    TArray<uint8> MessageData;
    MessageData.Append(BS_ByteParser::StringToArray(DiscoveredDevice.BluetoothId, true));

    for (const FBS_ConnectionMessage &Message : Messages)
    {
        UE_LOGFMT(LogBS_BaseClient, Verbose, "Appending {0} ({1} bytes) to message", static_cast<uint8>(Message.Type), Message.DataNum());
        Message.AppendTo(MessageData);
    }

    SendMessages({{EBS_ServerMessage::DEVICE_MESSAGE, MessageData}});
}

void UBS_BaseClient::ParseDeviceMessage(const TArrayView<const uint8> &Message)
{
    const uint16 MessageLength = Message.Num();

    UE_LOGFMT(LogBS_BaseClient, Verbose, "Parsing Device Message ({0} bytes)...", MessageLength);
    uint8 Offset = 0;
    const FString BluetoothId = BS_ByteParser::GetString(Message, true);
    Offset += 1 + BluetoothId.Len();

    UE_LOGFMT(LogBS_BaseClient, Verbose, "Received Device Message for BluetoothId: {0}", BluetoothId);
    if (!Devices.Contains(BluetoothId))
    {
        UE_LOGFMT(LogBS_BaseClient, Error, "No Device found with BluetoothId {0}", BluetoothId);
        return;
    }

    UBS_Device *Device = Devices[BluetoothId];

    const uint16 MessageDataLength = MessageLength - Offset;
    const TArrayView<const uint8> MessageData((uint8 *)(Message.GetData() + Offset), MessageDataLength);

    UE_LOGFMT(LogBS_BaseClient, Verbose, "Parsing {0} Bytes for Device...", MessageDataLength);
    UBS_ClientConnectionManager *ConnectionManager = Cast<UBS_ClientConnectionManager>(Device->GetConnectionManager());
    if (!ConnectionManager)
    {
        UE_LOGFMT(LogBS_BaseClient, Error, "Failed to cast ConnectionManager to ClientConnectionManager");
        return;
    }

    UBS_ParseUtils::ParseDeviceEventData(Device, static_cast<TArray<uint8>>(MessageData), ConnectionManager->BoundOnDeviceEvent);
}

// DEVICE MESSAGE END