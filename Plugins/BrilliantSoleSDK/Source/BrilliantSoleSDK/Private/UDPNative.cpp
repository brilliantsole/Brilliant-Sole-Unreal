
#include "UDPNative.h"
#include "Async/Async.h"
#include "SocketSubsystem.h"
#include "Kismet/KismetSystemLibrary.h"

FUDPNative::FUDPNative()
{
    SenderSocket = nullptr;
    ReceiverSocket = nullptr;

    ClearReceiveCallbacks();
    ClearSendCallbacks();
}

FUDPNative::~FUDPNative()
{
    if (Settings.bIsReceiveOpen)
    {
        CloseReceiveSocket();
        ClearReceiveCallbacks();
    }
    if (Settings.bIsSendOpen)
    {
        CloseSendSocket();
        ClearSendCallbacks();
    }
}

int32 FUDPNative::OpenSendSocket(const FString &InIP /*= TEXT("127.0.0.1")*/, const int32 InPort /*= 3000*/)
{
    Settings.SendIP = InIP;
    Settings.SendPort = InPort;

    RemoteAdress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

    bool bIsValid;
    RemoteAdress->SetIp(*Settings.SendIP, bIsValid);
    RemoteAdress->SetPort(Settings.SendPort);

    if (!bIsValid)
    {
        UE_LOG(LogTemp, Error, TEXT("UDP address is invalid <%s:%d>"), *Settings.SendIP, Settings.SendPort);
        return 0;
    }

    SenderSocket = FUdpSocketBuilder(*Settings.SendSocketName).AsReusable().WithBroadcast();

    // Set Send Buffer Size
    SenderSocket->SetSendBufferSize(Settings.BufferSize, Settings.BufferSize);
    SenderSocket->SetReceiveBufferSize(Settings.BufferSize, Settings.BufferSize);

    bool bDidConnect = SenderSocket->Connect(*RemoteAdress);
    Settings.bIsSendOpen = true;
    Settings.SendBoundPort = SenderSocket->GetPortNo();
    TSharedRef<FInternetAddr> SendBoundAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
    SenderSocket->GetAddress(*SendBoundAddress);
    Settings.SendBoundIP = SendBoundAddress->ToString(false);

    if (OnSendOpened)
    {
        OnSendOpened(Settings.SendPort, Settings.SendBoundPort, Settings.SendBoundIP);
    }

    return Settings.SendBoundPort;
}

bool FUDPNative::CloseSendSocket()
{
    bool bDidCloseCorrectly = true;
    Settings.bIsSendOpen = false;

    if (SenderSocket)
    {
        bDidCloseCorrectly = SenderSocket->Close();
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(SenderSocket);
        SenderSocket = nullptr;

        if (OnSendClosed)
        {
            OnSendClosed(Settings.SendPort);
        }
    }

    return bDidCloseCorrectly;
}

bool FUDPNative::EmitBytes(const TArray<uint8> &Bytes)
{
    bool bDidSendCorrectly = true;

    if (SenderSocket && SenderSocket->GetConnectionState() == SCS_Connected)
    {
        int32 BytesSent = 0;
        bDidSendCorrectly = SenderSocket->Send(Bytes.GetData(), Bytes.Num(), BytesSent);
    }
    else if (Settings.bShouldAutoOpenSend)
    {
        bool bDidOpen = OpenSendSocket(Settings.SendIP, Settings.SendPort) != 0;
        return bDidOpen && EmitBytes(Bytes);
    }

    return bDidSendCorrectly;
}

bool FUDPNative::OpenReceiveSocket(const FString &InListenIP /*= TEXT("0.0.0.0")*/, const int32 InListenPort /*= 3002*/)
{
    // Sync and overwrite settings
    if (Settings.bShouldOpenReceiveToBoundSendPort)
    {
        if (Settings.SendBoundPort == 0)
        {
            UE_LOG(LogTemp, Error, TEXT("FUDPNative::OpenReceiveSocket Can't bind to SendBoundPort if send socket hasn't been opened before this call."));
            return false;
        }
        Settings.ReceiveIP = Settings.SendBoundIP;
        Settings.ReceivePort = Settings.SendBoundPort;
    }
    else
    {
        Settings.ReceiveIP = InListenIP;
        Settings.ReceivePort = InListenPort;
    }

    bool bDidOpenCorrectly = true;

    if (Settings.bIsReceiveOpen)
    {
        bDidOpenCorrectly = CloseReceiveSocket();
    }

    FIPv4Address Addr;
    FIPv4Address::Parse(Settings.ReceiveIP, Addr);

    // Create Socket
    FIPv4Endpoint Endpoint(Addr, Settings.ReceivePort);

    ReceiverSocket = FUdpSocketBuilder(*Settings.ReceiveSocketName)
                         .AsNonBlocking()
                         .AsReusable()
                         .BoundToEndpoint(Endpoint)
                         .WithReceiveBufferSize(Settings.BufferSize);

    FTimespan ThreadWaitTime = FTimespan::FromMilliseconds(100);
    FString ThreadName = FString::Printf(TEXT("UDP RECEIVER-FUDPNative"));
    UDPReceiver = new FUdpSocketReceiver(ReceiverSocket, ThreadWaitTime, *ThreadName);

    UDPReceiver->OnDataReceived().BindLambda([this](const FArrayReaderPtr &DataPtr, const FIPv4Endpoint &Endpoint)
                                             {
		if (!OnReceivedBytes)
		{
			return;
		}

		TArray<uint8> Data;
		Data.AddUninitialized(DataPtr->TotalSize());
		DataPtr->Serialize(Data.GetData(), DataPtr->TotalSize());

		FString SenderIp = Endpoint.Address.ToString();
		int32 SenderPort = Endpoint.Port;

		if (Settings.bReceiveDataOnGameThread)
		{
			//Copy data to receiving thread via lambda capture
			AsyncTask(ENamedThreads::GameThread, [this, Data, SenderIp, SenderPort]()
			{
				//double check we're still bound on this thread
				if (OnReceivedBytes)
				{
					OnReceivedBytes(Data, SenderIp, SenderPort);
				}
			});
		}
		else
		{
			OnReceivedBytes(Data, SenderIp, SenderPort);
		} });

    Settings.bIsReceiveOpen = true;

    if (OnReceiveOpened)
    {
        OnReceiveOpened(Settings.ReceivePort);
    }

    UDPReceiver->Start();

    return bDidOpenCorrectly;
}

bool FUDPNative::CloseReceiveSocket()
{
    bool bDidCloseCorrectly = true;
    Settings.bIsReceiveOpen = false;

    if (ReceiverSocket)
    {
        UDPReceiver->Stop();
        delete UDPReceiver;
        UDPReceiver = nullptr;

        bDidCloseCorrectly = ReceiverSocket->Close();
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ReceiverSocket);
        ReceiverSocket = nullptr;

        if (OnReceiveClosed)
        {
            OnReceiveClosed(Settings.ReceivePort);
        }
    }

    return bDidCloseCorrectly;
}

void FUDPNative::ClearSendCallbacks()
{
    OnSendOpened = nullptr;
    OnSendClosed = nullptr;
}

void FUDPNative::ClearReceiveCallbacks()
{
    OnReceivedBytes = nullptr;
    OnReceiveOpened = nullptr;
    OnReceiveClosed = nullptr;
}