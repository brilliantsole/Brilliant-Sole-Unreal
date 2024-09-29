// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UDPManager.h"
#include "Logging/StructuredLog.h"
#include "BS_UDP_Message.h"
#include "BS_ConnectionStatus.h"
#include "BS_BaseClient.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "BS_BaseUDP_Client.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_BaseUDP_Client, Log, All);

#define MAX_UDP_MESSAGE_SIZE 65507

UCLASS(Abstract, BlueprintType, Blueprintable)
class UBS_BaseUDP_Client : public UBS_BaseClient
{
    GENERATED_BODY()

public:
    UBS_BaseUDP_Client();

private:
    void Reset();

    // UDP CLIENT START
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UDP Client")
    UUDPManager *UDP_Manager;
    // UDP CLIENT END

    // IN LISTEN PORT START
protected:
    UFUNCTION(BlueprintPure, Category = "BS UDP Client")
    int32 GetInListenPort() { return InListenPort; };

    UFUNCTION(BlueprintCallable, Category = "BS UDP Client")
    int32 SetInListenPort(int32 NewInListenPort);

private:
    int32 InListenPort;

    FBS_UDP_Message SetInListenPortUDP_Message = {EBS_UDP_MessageType::SET_REMOTE_RECEIVE_PORT};
    bool bDidSendSetInListenPortMessage;
    // IN LISTEN PORT END

    // PING START
public:
    UFUNCTION(BlueprintCallable, Category = "BS UDP Client")
    void StartPinging();
    UFUNCTION(BlueprintCallable, Category = "BS UDP Client")
    void StopPinging();

protected:
private:
    FTimerHandle PingTimerHandler;
    void Ping();

    static const FBS_UDP_Message PingMessage;
    // PING END

    // PONG START
private:
    FTimerHandle WaitForPongTimerHandler;
    void WaitForPong();
    void StopWaitingForPong();
    void ResetWaitForPong()
    {
        StopWaitingForPong();
        WaitForPong();
    }
    void PongTimeout();

    void Pong();
    static const FBS_UDP_Message PongMessage;
    // PONG END

    // MESSAGING START
public:
protected:
    void SendMessageData(const TArray<uint8> &Data, bool bSendImmediately) override;

private:
    void SendUDP_Data(const TArray<uint8> &Data)
    {
        UDP_Manager->EmitBytes(Data);
        bIsSendingUDP_Data = false;
    }

    void SendUDP_Messages(const TArray<FBS_UDP_Message> &UDP_Messages, bool bSendImmediately = true);
    void SendPendingUDP_Messages();

    TArray<FBS_UDP_Message> PendingUDP_Messages;
    TArray<uint8> UDP_Data;

    bool bIsSendingUDP_Data = false;
    // MESSAGING END

    // PARSING START
protected:
    UFUNCTION(BlueprintCallable, Category = "BS UDP Client")
    void OnUDP_Message(EBS_UDP_MessageType MessageType, const TArray<uint8> &Message);

private:
    void OnSetRemoteReceivePortMessage(const TArray<uint8> &Message);
    // PARSING END

    // CONNECTION START
public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "BS UDP Client")
    void Connect(const FString &IP_Address = "127.0.0.1", const int32 Port = 3000);
    virtual void Connect_Implementation(const FString &IP_Address = "127.0.0.1", const int32 Port = 3000);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "BS UDP Client")
    void Disconnect();
    virtual void Disconnect_Implementation();

    UFUNCTION(BlueprintCallable, Category = "BS UDP Client")
    void ToggleConnection(const FString &IP_Address = "127.0.0.1", const int32 Port = 3000);
    // CONNECTION END
};
