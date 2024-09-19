// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UDPManager.h"
#include "Logging/StructuredLog.h"
#include "BS_UDPMessageType.h"
#include "BS_BaseUDP_Manager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_BaseUDP_Manager, Verbose, All);

UCLASS(Abstract, BlueprintType, Blueprintable)
class UBS_BaseUDP_Manager : public UObject
{
    GENERATED_BODY()

public:
    UBS_BaseUDP_Manager();
    void PostInitProperties();

public:
    UFUNCTION(BlueprintImplementableEvent, Category = "BS Subsystem")
    void InitializeBP();

    // BS SUBSYSTEM START
protected:
    UFUNCTION(BlueprintPure, Category = "BS Subsystem")
    const UBS_Subsystem *BS_Subsystem() const { return _BS_Subsystem; }

private:
    void GetBS_Subsystem();
    UBS_Subsystem *_BS_Subsystem;
    // BS SUBSYSTEM END

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

    TArray<uint8> SetInListenPortMessage;
    // IN LISTEN PORT END

    // PING START
protected:
    static TArray<uint8> PingMessage;

    UFUNCTION(BlueprintCallable, Category = "BS UDP Manager")
    void Ping();

private:
    const TArray<uint8> &GetPingMessage();
    bool bDidSendSetInListenPortMessage;
    // PING END

    // MESSAGING START
public:
protected:
private:
    void SendData(const TArray<uint8> &Data) { UDP_Manager->EmitBytes(Data); }
    // MESSAGING END
};
