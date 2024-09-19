#pragma once

#include "CoreMinimal.h"
#include "UDPSettings.h"
#include "UDPNative.h"
#include "UDPManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUDPSocketStateSignature, int32, Port);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FUDPSocketSendStateSignature, int32, SpecifiedPort, int32, BoundPort, const FString &, BoundIP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FUDPMessageSignature, const TArray<uint8> &, Bytes, const FString &, IPAddress, const int32 &, Port);

UCLASS(BlueprintType, Blueprintable)
class UUDPManager : public UObject
{
	GENERATED_BODY()

public:
	UUDPManager();
	void PostInitProperties();

public:
	// Async events

	/** On message received on receive socket from Ip address */
	UPROPERTY(BlueprintAssignable, Category = "UDP Events")
	FUDPMessageSignature OnReceivedBytes;

	/** Callback when we start listening on the udp receive socket*/
	UPROPERTY(BlueprintAssignable, Category = "UDP Events")
	FUDPSocketStateSignature OnReceiveSocketOpened;

	/** Called after receiving socket has been closed. */
	UPROPERTY(BlueprintAssignable, Category = "UDP Events")
	FUDPSocketStateSignature OnReceiveSocketClosed;

	/** Called when the send socket is ready to use; optionally open your receive socket to bound send port here */
	UPROPERTY(BlueprintAssignable, Category = "UDP Events")
	FUDPSocketSendStateSignature OnSendSocketOpened;

	/** Called when the send socket has been closed */
	UPROPERTY(BlueprintAssignable, Category = "UDP Events")
	FUDPSocketStateSignature OnSendSocketClosed;

	/** Defining UDP sending and receiving Ips, ports, and other defaults*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UDP Connection Properties")
	FUDPSettings Settings;

	/**
	 * Connect to a udp endpoint, optional method if auto-connect is set to true.
	 * Emit function will then work as long the network is reachable. By default
	 * it will attempt this setup for this socket on BeginPlay.
	 *
	 * @param InIP the ip4 you wish to connect to
	 * @param InPort the udp port you wish to connect to
	 */
	UFUNCTION(BlueprintCallable, Category = "UDP Functions")
	int32 OpenSendSocket(const FString &InIP = TEXT("127.0.0.1"), const int32 InPort = 3000);

	/**
	 * Close the sending socket. This is usually automatically done on EndPlay.
	 */
	UFUNCTION(BlueprintCallable, Category = "UDP Functions")
	bool CloseSendSocket();

	/**
	 * Start listening at given port for udp messages. Will auto-listen on BeginPlay by default. Listen IP of 0.0.0.0 means all connections.
	 */
	UFUNCTION(BlueprintCallable, Category = "UDP Functions")
	bool OpenReceiveSocket(const FString &InListenIP = TEXT("0.0.0.0"), const int32 InListenPort = 3002);

	/**
	 * Close the receiving socket. This is usually automatically done on EndPlay.
	 */
	UFUNCTION(BlueprintCallable, Category = "UDP Functions")
	bool CloseReceiveSocket();

	/**
	 * Emit specified bytes to the udp channel.
	 *
	 * @param Message	Bytes
	 */
	UFUNCTION(BlueprintCallable, Category = "UDP Functions")
	bool EmitBytes(const TArray<uint8> &Bytes);

	/**
	 * Closes everything.
	 */
	UFUNCTION(BlueprintCallable, Category = "UDP Functions")
	void Destroy();

protected:
	TSharedPtr<FUDPNative> Native;
	void LinkupCallbacks();
};