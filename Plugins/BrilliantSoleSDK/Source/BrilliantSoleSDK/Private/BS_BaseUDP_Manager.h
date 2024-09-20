// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UDPManager.h"
#include "Logging/StructuredLog.h"
#include "BS_UDP_Message.h"
#include "BS_ConnectionStatus.h"
#include "BS_BaseClientManager.h"
#include "BS_BaseUDP_Manager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_BaseUDP_Manager, Verbose, All);

#define MAX_UDP_MESSAGE_SIZE 65507

UCLASS(Abstract, BlueprintType, Blueprintable)
class UBS_BaseUDP_Manager : public UBS_BaseClientManager
{
    GENERATED_BODY()

public:
    UBS_BaseUDP_Manager();

    // UDP MANAGER START
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UDP Manager")
    UUDPManager *UDP_Manager;
    // UDP MANAGER END

    // IN LISTEN PORT START
protected:
    UFUNCTION(BlueprintPure, Category = "BS UDP Manager")
    int32 GetInListenPort() { return InListenPort; };

    UFUNCTION(BlueprintCallable, Category = "BS UDP Manager")
    int32 SetInListenPort(int32 NewInListenPort);

private:
    int32 InListenPort;

    FBS_UDP_Message SetInListenPortUDP_Message = {EBS_UDP_MessageType::SET_REMOTE_RECEIVE_PORT};
    bool bDidSendSetInListenPortMessage;
    // IN LISTEN PORT END

    // PING START
protected:
    UFUNCTION(BlueprintCallable, Category = "BS UDP Manager")
    void Ping();

private:
    static const FBS_UDP_Message PingUDP_Message;
    const FBS_UDP_Message &GetPingMessage();
    // PING END

    // PONG START
    void OnPongMessage(const TArray<uint8> &Message);

    // PONG END

    // MESSAGING START
public:
protected:
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
    UFUNCTION(BlueprintCallable, Category = "BS UDP Manager")
    void OnUDP_Message(EBS_UDP_MessageType MessageType, const TArray<uint8> &Message);

private:
    void OnSetRemoteReceivePortMessage(const TArray<uint8> &Message);
    // PARSING END

    // CONNECTION START
public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "BS UDP Manager")
    void Connect(const FString &IP_Address = "127.0.0.1", const int32 Port = 3000);
    virtual void Connect_Implementation(const FString &IP_Address = "127.0.0.1", const int32 Port = 3000);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "BS UDP Manager")
    void Disconnect();
    virtual void Disconnect_Implementation();
    // CONNECTION END
};
