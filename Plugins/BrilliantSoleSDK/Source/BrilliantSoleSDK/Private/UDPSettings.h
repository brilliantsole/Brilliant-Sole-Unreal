#pragma once

#include "CoreMinimal.h"
#include "UDPSettings.generated.h"

// UDP Connection Settings
USTRUCT(BlueprintType)
struct FUDPSettings
{
	GENERATED_USTRUCT_BODY()

	/** Default sending socket IP string in form e.g. 127.0.0.1. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UDP Connection Properties")
	FString SendIP;

	/** Default connection port e.g. 3001*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UDP Connection Properties")
	int32 SendPort;

	/** Port to which send is bound to on this client (this should change on each open) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UDP Connection Properties")
	int32 SendBoundPort;

	/** IP to which send is bound to on this client (this could change on open) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UDP Connection Properties")
	FString SendBoundIP;

	/** Default receiving socket IP string in form e.g. 0.0.0.0 for all connections, may need 127.0.0.1 for some cases. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UDP Connection Properties")
	FString ReceiveIP;

	/** Default connection port e.g. 3002*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UDP Connection Properties")
	int32 ReceivePort;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UDP Connection Properties")
	FString SendSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UDP Connection Properties")
	FString ReceiveSocketName;

	/** in bytes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UDP Connection Properties")
	int32 BufferSize;

	/** If true will auto-connect on begin play to IP/port specified for sending udp messages, plus when emit is called */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UDP Connection Properties")
	bool bShouldAutoOpenSend;

	/** If true will auto-listen on begin play to port specified for receiving udp messages. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UDP Connection Properties")
	bool bShouldAutoOpenReceive;

	/** This will open it to the bound send port at specified send ip and ignore passed in settings for open receive. Default False*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UDP Connection Properties")
	bool bShouldOpenReceiveToBoundSendPort;

	/** Whether we should process our data on the gamethread or the udp thread. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UDP Connection Properties")
	bool bReceiveDataOnGameThread;

	UPROPERTY(BlueprintReadOnly, Category = "UDP Connection Properties")
	bool bIsReceiveOpen;

	UPROPERTY(BlueprintReadOnly, Category = "UDP Connection Properties")
	bool bIsSendOpen;

	FUDPSettings();
};