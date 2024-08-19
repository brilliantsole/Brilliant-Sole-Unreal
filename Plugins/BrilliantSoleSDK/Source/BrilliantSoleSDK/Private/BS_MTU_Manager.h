// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BS_BaseManager.h"
#include "BS_MTU_Manager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_MTU_Manager, Log, All);

#define EBS_MTU_Message BS_MessageGetMTU

UCLASS()
class UBS_MTU_Manager : public UBS_BaseManager
{
	GENERATED_BODY()

	public:
	bool OnRxMessage(uint8 MessageType, const TArray<uint8> &Message) override;
};
