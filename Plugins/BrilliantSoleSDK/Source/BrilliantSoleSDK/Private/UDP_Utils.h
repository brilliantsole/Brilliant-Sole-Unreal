// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Logging/StructuredLog.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UDP_Utils.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogUDP_Utils, Log, All);

UCLASS()
class UUDP_Utils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UDP Port Utils")
	static bool FindAvailableUdpPort(int32 &OutPort);
};
