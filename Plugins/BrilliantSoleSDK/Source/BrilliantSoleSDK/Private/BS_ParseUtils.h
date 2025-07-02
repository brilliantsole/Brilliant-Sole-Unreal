// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BS_UDP_MessageType.h"
#include "BS_ServerMessageType.h"
#include "BS_DeviceEventType.h"
#include "BS_Device.h"
#include "BS_ParseUtils.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_ParseUtils, Warning, All);

DECLARE_DELEGATE_TwoParams(FBS_ServerMessageCallback, EBS_ServerMessage, const TArrayView<const uint8> &);
DECLARE_DELEGATE_ThreeParams(FBS_DeviceEventCallback, UBS_Device *, EBS_DeviceEvent, const TArrayView<const uint8> &);

UCLASS()
class UBS_ParseUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void ParseServerData(const TArrayView<const uint8> &Data, FBS_ServerMessageCallback MessageCallback);
	static void ParseDeviceEventData(UBS_Device *Device, const TArray<uint8> &Data, FBS_DeviceEventCallback MessageCallback);
};
