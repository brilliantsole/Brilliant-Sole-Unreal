
#include "UDPSettings.h"

FUDPSettings::FUDPSettings()
{
	bShouldAutoOpenSend = false;
	bShouldAutoOpenReceive = false;
	bShouldOpenReceiveToBoundSendPort = false;
	bReceiveDataOnGameThread = true;
	SendIP = FString(TEXT("127.0.0.1"));
	SendPort = 3001;
	SendBoundPort = 0; // invalid if 0
	SendBoundIP = FString(TEXT("0.0.0.0"));
	ReceiveIP = FString(TEXT("0.0.0.0"));
	ReceivePort = 3002;
	SendSocketName = FString(TEXT("ue4-dgram-send"));
	ReceiveSocketName = FString(TEXT("ue4-dgram-receive"));

	bIsReceiveOpen = false;
	bIsSendOpen = false;

	BufferSize = 2 * 1024 * 1024; // default roughly 2mb
}
