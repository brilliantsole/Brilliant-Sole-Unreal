// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BS_BaseManager.h"
#include "BS_InformationManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_InformationManager, Log, All);

#define EBS_MTU_Message BS_MessageGetMTU
#define EBS_DeviceIdMessage BS_MessageGetId
#define EBS_DeviceNameMessage BS_MessageGetName, \
							  BS_MessageSetName
#define EBS_DeviceTypeMessage BS_MessageGetType, \
							  BS_MessageSetType
#define EBS_CurrentTimeMessage BS_MessageGetCurrentTime, \
							   BS_MessageSetCurrentTime

UCLASS()
class UBS_InformationManager : public UBS_BaseManager
{
	GENERATED_BODY()

	public:
	bool OnRxMessage(uint8 MessageType, const TArray<uint8> &Message) override;
};
