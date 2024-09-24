// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Logging/StructuredLog.h"
#include "BS_ServerMessageType.h"
#include "BS_ServerMessage.h"
#include "BS_ConnectionStatus.h"
#include "BS_ServerMessage.h"
#include "BS_ParseUtils.h"
#include "BS_DiscoveredDevice.h"
#include "BS_Device.h"
#include "BS_ConnectionMessage.h"
#include "BS_BaseClient.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_BaseClient, Verbose, All);

UCLASS(Abstract, BlueprintType, Blueprintable)
class UBS_BaseClient : public UObject
{
    GENERATED_BODY()

public:
    UBS_BaseClient();
    void PostInitProperties();

private:
    void Reset();

public:
    UFUNCTION(BlueprintImplementableEvent, Category = "BS Subsystem")
    void InitializeBP();

    // BS SUBSYSTEM START
protected:
    UFUNCTION(BlueprintPure, Category = "BS Subsystem")
    const UBS_Subsystem *BS_Subsystem() const { return _BS_Subsystem; }

protected:
    void GetBS_Subsystem();
    UBS_Subsystem *_BS_Subsystem;
    // BS SUBSYSTEM END

    // CONNECTION START
public:
    UFUNCTION(BlueprintPure, Category = "BS Client")
    EBS_ConnectionStatus GetConnectionStatus() const { return ConnectionStatus; }

    UFUNCTION(BlueprintPure, Category = "BS Client")
    const bool GetIsConnected() const { return ConnectionStatus == EBS_ConnectionStatus::CONNECTED; }

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBS_ClientConnectionStatusUpdateCallback, UBS_BaseClient *, Client, EBS_ConnectionStatus, ConnectionStatus);
    UPROPERTY(BlueprintAssignable, Category = "BS Client")
    FBS_ClientConnectionStatusUpdateCallback OnConnectionStatusUpdate;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBS_ClientIsConnectedUpdateCallback, UBS_BaseClient *, Client, bool, IsConnected);
    UPROPERTY(BlueprintAssignable, Category = "BS Client")
    FBS_ClientIsConnectedUpdateCallback OnIsConnectedUpdate;

    UFUNCTION(BlueprintPure, Category = "BS Client")
    const bool IsConnected() const { return ConnectionStatus == EBS_ConnectionStatus::CONNECTED; }

protected:
    UFUNCTION(BlueprintCallable, Category = "BS Client")
    void SetConnectionStatus(EBS_ConnectionStatus NewConnectionStatus);

private:
    EBS_ConnectionStatus ConnectionStatus = EBS_ConnectionStatus::NOT_CONNECTED;

    // CONNECTION END

    // MESSAGING START
protected:
    void OnData(const TArray<uint8> &Data);

    virtual void SendMessageData(const TArray<uint8> &Data, bool bSendImmediately = true) {}

private:
    void OnMessage(EBS_ServerMessage MessageType, const TArray<uint8> &Message);
    FBS_ServerMessageCallback BoundOnMessage;

    void SendMessages(const TArray<FBS_ServerMessage> &ServerMessages, bool bSendImmediately = true);

    void SendRequiredMessages() { SendMessages(UBS_BaseClient::RequiredMessages); }

    static const TArray<EBS_ServerMessage> RequiredMessageTypes;
    static const TArray<FBS_ServerMessage> RequiredMessages;
    static const TArray<FBS_ServerMessage> InitializeRequiredMessages();

private:
    // MESSAGING END

    // SCANNING START
public:
    UFUNCTION(BlueprintPure, Category = "BS Client")
    const bool GetIsScanningAvailable() const { return bIsScanningAvailable; }

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBS_ClientIsScanningAvailableUpdateCallback, UBS_BaseClient *, Client, bool, IsScanningAvailable);
    UPROPERTY(BlueprintAssignable, Category = "BS Client")
    FBS_ClientIsScanningAvailableUpdateCallback OnIsScanningAvailableUpdate;

    UFUNCTION(BlueprintPure, Category = "BS Client")
    const bool GetIsScanning() const { return bIsScanning; }

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBS_ClientIsScanningUpdateCallback, UBS_BaseClient *, Client, bool, IsScanning);
    UPROPERTY(BlueprintAssignable, Category = "BS Client")
    FBS_ClientIsScanningUpdateCallback OnIsScanningUpdate;

    UFUNCTION(BlueprintCallable, Category = "BS Client")
    void StartScan();

    UFUNCTION(BlueprintCallable, Category = "BS Client")
    void StopScan();

    UFUNCTION(BlueprintCallable, Category = "BS Client")
    void ToggleScan();

private:
    bool bIsScanningAvailable = false;
    bool bIsScanning = false;

    void ParseIsScanningAvailable(const TArray<uint8> &Message);
    void ParseIsScanning(const TArray<uint8> &Message);
    // SCANNING END

    // DISCOVERED DEVICES START
protected:
    UFUNCTION(BlueprintPure, Category = "BS Client")
    const TMap<FString, FBS_DiscoveredDevice> &GetDiscoveredDevices() const { return DiscoveredDevices; }

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBS_ClientDiscoveredDeviceCallback, UBS_BaseClient *, Client, FBS_DiscoveredDevice, DiscoveredDevice);
    UPROPERTY(BlueprintAssignable, Category = "BS Client")
    FBS_ClientDiscoveredDeviceCallback OnDiscoveredDevice;

private:
    void ParseDiscoveredDevice(const TArray<uint8> &Message);
    void ParseExpiredDiscoveredDevice(const TArray<uint8> &Message);

    UPROPERTY()
    TMap<FString, FBS_DiscoveredDevice> DiscoveredDevices;
    // DISCOVERED DEVICES END

    // DEVICE CONNECTION START
public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "BS Client")
    UBS_Device *ConnectToDevice(const FBS_DiscoveredDevice &DiscoveredDevice);
    virtual UBS_Device *ConnectToDevice_Implementation(const FBS_DiscoveredDevice &DiscoveredDevice);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "BS Client")
    UBS_Device *DisconnectFromDevice(const FBS_DiscoveredDevice &DiscoveredDevice);
    virtual UBS_Device *DisconnectFromDevice_Implementation(const FBS_DiscoveredDevice &DiscoveredDevice);

    UFUNCTION(BlueprintCallable, Category = "BS Client")
    UBS_Device *ToggleDeviceConnection(const FBS_DiscoveredDevice &DiscoveredDevice);

    // DEVICE CONNECTION END

    // DEVICES START
protected:
    UFUNCTION(BlueprintPure, Category = "BS Client")
    const TMap<FString, UBS_Device *> &GetDevices() const { return Devices; }

    UFUNCTION(BlueprintCallable, Category = "BS Client")
    void AddDevice(FString BluetoothId, UBS_Device *Device)
    {
        UE_LOGFMT(LogTemp, Log, "Adding Device for BluetoothId {0}", BluetoothId);
        Devices.Add(BluetoothId, Device);
    }

    UFUNCTION(BlueprintCallable, Category = "BS Client")
    UBS_Device *GetDeviceByDiscoveredDevice(const FBS_DiscoveredDevice &DiscoveredDevice);

    UPROPERTY()
    TMap<FString, UBS_Device *> Devices;

private:
    void ParseDevices(const TArray<uint8> &Message);

    // DEVICES DEVICES END

    // DEVICE MESSAGE START
public:
    void SendConnectToDeviceMessage(const FBS_DiscoveredDevice &DiscoveredDevice, bool bSendImmediately = true);
    void SendDisconnectFromDeviceMessage(const FBS_DiscoveredDevice &DiscoveredDevice, bool bSendImmediately = true);
    void SendDeviceMessages(const FBS_DiscoveredDevice &DiscoveredDevice, const TArray<FBS_ConnectionMessage> &Messages, bool bSendImmediately = true);

private:
    void ParseDeviceMessage(const TArray<uint8> &Message);

    // DEVICE MESSAGE END
};
