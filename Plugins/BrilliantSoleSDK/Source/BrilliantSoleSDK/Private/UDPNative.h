#pragma once

#include "CoreMinimal.h"
#include "UDPSettings.h"
#include "IPAddress.h"
#include "Common/UdpSocketBuilder.h"
#include "Common/UdpSocketReceiver.h"
#include "Common/UdpSocketSender.h"

class FUDPNative
{
public:
    TFunction<void(const TArray<uint8> &, const FString &, const int32 &)> OnReceivedBytes;
    TFunction<void(int32 Port)> OnReceiveOpened;
    TFunction<void(int32 Port)> OnReceiveClosed;
    TFunction<void(int32 SpecifiedPort, int32 BoundPort, FString BoundIP)> OnSendOpened;
    TFunction<void(int32 Port)> OnSendClosed;

    // Default settings, on open send/receive they will sync with what was last passed into them
    FUDPSettings Settings;

    FUDPNative();
    ~FUDPNative();

    // Send
    /**
     * Open socket for sending and return bound port
     */
    int32 OpenSendSocket(const FString &InIP = TEXT("127.0.0.1"), const int32 InPort = 3000);

    /**
     * Close current sending socket, returns true if successful
     */
    bool CloseSendSocket();

    /**
     * Emit given bytes to send socket. If Settings.bShouldAutoOpenSend is true it will auto-open socket.
     * Returns true if bytes emitted successfully
     */
    bool EmitBytes(const TArray<uint8> &Bytes);

    // Receive
    /**
     * Open current receiving socket, returns true if successful
     */
    bool OpenReceiveSocket(const FString &InIP = TEXT("0.0.0.0"), const int32 InListenPort = 3002);
    /**
     * Close current receiving socket, returns true if successful
     */
    bool CloseReceiveSocket();

    // Callback convenience
    void ClearSendCallbacks();
    void ClearReceiveCallbacks();

protected:
    FSocket *SenderSocket;
    FSocket *ReceiverSocket;
    FUdpSocketReceiver *UDPReceiver;
    FString SocketDescription;
    TSharedPtr<FInternetAddr> RemoteAdress;
    ISocketSubsystem *SocketSubsystem;
};