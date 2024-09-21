// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Logging/StructuredLog.h"
#include "BS_ServerMessageType.h"
#include "BS_ConnectionStatus.h"
#include "BS_ServerMessage.h"
#include "BS_BaseClient.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_BaseClient, Verbose, All);

UCLASS(Abstract, BlueprintType, Blueprintable)
class UBS_BaseClient : public UObject
{
    GENERATED_BODY()

public:
    UBS_BaseClient();
    void PostInitProperties();

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
    bool GetIsConnected() const { return ConnectionStatus == EBS_ConnectionStatus::CONNECTED; }

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBS_ClientConnectionStatusUpdateCallback, UBS_BaseClient *, Client, EBS_ConnectionStatus, ConnectionStatus);
    UPROPERTY(BlueprintAssignable, Category = "BS Client")
    FBS_ClientConnectionStatusUpdateCallback OnConnectionStatusUpdate;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBS_ClientIsConnectedUpdateCallback, UBS_BaseClient *, Client, bool, IsConnected);
    UPROPERTY(BlueprintAssignable, Category = "BS Client")
    FBS_ClientIsConnectedUpdateCallback OnIsConnectedUpdate;

    UFUNCTION(BlueprintPure, Category = "BS Client")
    bool IsConnected() const { return ConnectionStatus == EBS_ConnectionStatus::CONNECTED; }

protected:
    UFUNCTION(BlueprintCallable, Category = "BS Client")
    void SetConnectionStatus(EBS_ConnectionStatus NewConnectionStatus);

private:
    EBS_ConnectionStatus ConnectionStatus = EBS_ConnectionStatus::NOT_CONNECTED;

    // CONNECTION END

    // MESSAGE START
protected:
    void OnData(const TArray<uint8> &Data);
    void OnMessage(EBS_ServerMessageType MessageType, const TArray<uint8> &Message);

    virtual void SendMessageData(const TArray<uint8> &Data, bool bSendImmediately = true) {}

private:
    void SendMessages(const TArray<FBS_ServerMessage> &ServerMessages, bool bSendImmediately = true);

private:
    // MESSAGE END
};
