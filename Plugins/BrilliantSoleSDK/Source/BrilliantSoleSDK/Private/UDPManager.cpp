
#include "UDPManager.h"
#include "Async/Async.h"
#include "SocketSubsystem.h"
#include "Kismet/KismetSystemLibrary.h"

UUDPManager::UUDPManager()
{
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		return;
	}

	Native = MakeShareable(new FUDPNative);

	LinkupCallbacks();
}

void UUDPManager::PostInitProperties()
{
	Super::PostInitProperties();

	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		return;
	}

	// Sync all settings to native. These are duplicated for dev convenience in bp
	Native->Settings = Settings;

	if (Settings.bShouldAutoOpenSend)
	{
		OpenSendSocket(Settings.SendIP, Settings.SendPort);
	}

	if (Settings.bShouldAutoOpenReceive)
	{
		OpenReceiveSocket(Settings.ReceiveIP, Settings.ReceivePort);
	}
}

void UUDPManager::LinkupCallbacks()
{
	Native->OnSendOpened = [this](int32 SpecifiedPort, int32 BoundPort, FString BoundIP)
	{
		Settings.bIsSendOpen = true;
		Settings.SendBoundPort = BoundPort; // ensure sync on opened bound port
		Settings.SendBoundIP = BoundIP;

		Settings.SendIP = Native->Settings.SendIP;
		Settings.SendPort = Native->Settings.SendPort;

		OnSendSocketOpened.Broadcast(Settings.SendPort, Settings.SendBoundPort, Settings.SendBoundIP);
	};
	Native->OnSendClosed = [this](int32 Port)
	{
		Settings.bIsSendOpen = false;
		OnSendSocketClosed.Broadcast(Port);
	};
	Native->OnReceiveOpened = [this](int32 Port)
	{
		Settings.ReceiveIP = Native->Settings.ReceiveIP;
		Settings.ReceivePort = Native->Settings.ReceivePort;

		Settings.bIsReceiveOpen = true;
		OnReceiveSocketOpened.Broadcast(Port);
	};
	Native->OnReceiveClosed = [this](int32 Port)
	{
		Settings.bIsReceiveOpen = false;
		OnReceiveSocketClosed.Broadcast(Port);
	};
	Native->OnReceivedBytes = [this](const TArray<uint8> &Data, const FString &Endpoint, const int32 &Port)
	{
		OnReceivedBytes.Broadcast(Data, Endpoint, Port);
	};
}

bool UUDPManager::CloseReceiveSocket()
{
	return Native->CloseReceiveSocket();
}

int32 UUDPManager::OpenSendSocket(const FString &InIP /*= TEXT("127.0.0.1")*/, const int32 InPort /*= 3000*/)
{
	// Sync side effect sampled settings
	Native->Settings.SendSocketName = Settings.SendSocketName;
	Native->Settings.BufferSize = Settings.BufferSize;

	return Native->OpenSendSocket(InIP, InPort);
}

bool UUDPManager::CloseSendSocket()
{
	Settings.SendBoundPort = 0;
	Settings.SendBoundIP = FString(TEXT("0.0.0.0"));
	return Native->CloseSendSocket();
}

bool UUDPManager::OpenReceiveSocket(const FString &InListenIp /*= TEXT("0.0.0.0")*/, const int32 InListenPort /*= 3002*/)
{
	// Sync side effect sampled settings
	Native->Settings.bShouldAutoOpenReceive = Settings.bShouldAutoOpenReceive;
	Native->Settings.ReceiveSocketName = Settings.ReceiveSocketName;
	Native->Settings.BufferSize = Settings.BufferSize;

	return Native->OpenReceiveSocket(InListenIp, InListenPort);
}

bool UUDPManager::EmitBytes(const TArray<uint8> &Bytes)
{
	return Native->EmitBytes(Bytes);
}

void UUDPManager::Destroy()
{
	CloseSendSocket();
	CloseReceiveSocket();

	Native->ClearSendCallbacks();
	Native->ClearReceiveCallbacks();
}