// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_ClientConnectionManager.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(LogBS_ClientConnectionManager);

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
    // FILL
}
// MESSAGING END