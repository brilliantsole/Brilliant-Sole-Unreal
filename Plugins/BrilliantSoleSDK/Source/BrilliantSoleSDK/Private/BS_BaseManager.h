// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BS_BaseManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_BaseManager, Log, All);

DECLARE_DELEGATE_TwoParams(FSendTxMessageDelegate, const TArray<uint8> &, bool);

UCLASS(Abstract)
class UBS_BaseManager : public UObject
{
	GENERATED_BODY()

public:
	FSendTxMessageDelegate SendTxMessage;
	virtual bool OnRxMessage(uint8 MessageType, const TArray<uint8> &Message) { return false; }
};
