// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Logging/StructuredLog.h"
#include "UObject/NoExportTypes.h"
#include "BS_BaseConnectionManager.h"
#include "BS_BaseClient.h"
#include "BS_DiscoveredDevice.h"
#include "BS_ClientConnectionManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_ClientConnectionManager, Verbose, All);

UCLASS()
class UBS_ClientConnectionManager : public UBS_BaseConnectionManager
{
	GENERATED_BODY()

public:
	UBS_ClientConnectionManager();

	// CLIENT START
public:
	UFUNCTION(BlueprintPure, Category = "BS Client Connection Manager")
	UBS_BaseClient *GetClient() const { return Client; }

	UFUNCTION(BlueprintCallable, Category = "BS Client Connection Manager")
	void AssignClient(UBS_BaseClient *_Client);

private:
	UBS_BaseClient *Client = nullptr;
	// CLIENT END

	// DISCOVERED DEVICE START
public:
	UFUNCTION(BlueprintPure, Category = "BS Client Connection Manager")
	const FBS_DiscoveredDevice &GetDiscoveredDevice() const { return DiscoveredDevice; }

	UFUNCTION(BlueprintCallable, Category = "BS Client Connection Manager")
	void AssignDiscoveredDevice(const FBS_DiscoveredDevice &NewDiscoveredDevice);

private:
	FBS_DiscoveredDevice DiscoveredDevice;
	// DISCOVERED DEVICE END

	// CONNECTION START
public:
	virtual const bool GetIsConnected_Implementation() const override { return bIsConnected; }

private:
	void SetIsConnected(bool bNewIsConnected);
	bool bIsConnected = false;
	// CONNECTION END

	// MESSAGING START
public:
	virtual void Connect_Implementation(bool &bContinue) override;
	virtual void Disconnect_Implementation(bool &bContinue) override;
	virtual void SendTxData_Implementation(const TArray<uint8> &Data) override;

public:
	void OnDeviceEvent(UBS_Device *Device, EBS_DeviceEvent DeviceEventType, const TArray<uint8> &Message);
	FBS_DeviceEventCallback BoundOnDeviceEvent;
	// MESSAGING END

	// DEVICE INFORMATION START

private:
	static const TArray<EBS_ConnectionMessage> RequiredDeviceInformationMessageTypes;
	static const TArray<FBS_ConnectionMessage> InitializeRequiredDeviceInformationMessages();
	static const TArray<FBS_ConnectionMessage> RequiredDeviceInformationMessages;
	void RequestDeviceInformation();
	// DEVICE INFORMATION END
};
