// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BS_TxMessage.h"
#include "BS_BaseManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_BaseManager, Warning, All);

DECLARE_DELEGATE_TwoParams(FSendTxMessagesDelegate, const TArray<FBS_TxMessage> &, bool);

UCLASS(Abstract)
class UBS_BaseManager : public UObject
{
	GENERATED_BODY()

public:
	FSendTxMessagesDelegate SendTxMessages;
	virtual void OnSendTxData() {}

	virtual bool OnRxMessage(EBS_TxRxMessage MessageType, const TArrayView<const uint8> &Message) { return false; }
	virtual void Reset() {}
};
