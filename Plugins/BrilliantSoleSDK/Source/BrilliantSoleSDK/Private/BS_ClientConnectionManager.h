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

	// MESSAGING START
public:
	virtual void Connect_Implementation(bool &bContinue) override;
	virtual void Disconnect_Implementation(bool &bContinue) override;
	virtual void SendTxData_Implementation(const TArray<uint8> &Data) override;
	// MESSAGING END
};
