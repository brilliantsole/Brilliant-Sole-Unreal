// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BS_UDP_MessageType.h"
#include "BS_ServerMessageType.h"
#include "BS_ParseUtils.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_ParseUtils, Verbose, All);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FBS_MessageCallback, uint8, MessageType, const TArray<uint8> &, Message);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FBS_UDP_MessageCallback, EBS_UDP_MessageType, MessageType, const TArray<uint8> &, Message);
DECLARE_DELEGATE_TwoParams(FBS_ServerMessageCallback, EBS_ServerMessageType, const TArray<uint8> &);

UCLASS()
class UBS_ParseUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "BS Parse Utils")
	static void ParseRxData(const TArray<uint8> &Data, FBS_MessageCallback MessageCallback);

	UFUNCTION(BlueprintCallable, Category = "BS Parse Utils")
	static void ParseUDP_Data(const TArray<uint8> &Data, FBS_UDP_MessageCallback MessageCallback);

	static void ParseServerData(const TArray<uint8> &Data, FBS_ServerMessageCallback MessageCallback);
};
