// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BS_UDP_MessageType.h"
#include "BS_ParseUtils.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_ParseUtils, Verbose, All);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FBS_MessageCallback, uint8, MessageType, const TArray<uint8> &, Message);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FBS_UDP_MessageCallback, EBS_UDP_MessageType, MessageType, const TArray<uint8> &, Message);

UCLASS()
class UBS_ParseUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "BS Parse Utils")
	static void ParseRxData(const TArray<uint8> &Data, FBS_MessageCallback MessageCallback);

	UFUNCTION(BlueprintCallable, Category = "BS Parse Utils")
	static void ParseUDP_Data(const TArray<uint8> &Data, FBS_UDP_MessageCallback MessageCallback);
};
