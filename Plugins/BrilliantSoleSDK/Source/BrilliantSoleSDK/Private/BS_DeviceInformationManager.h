// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BS_BaseManager.h"
#include "BS_DeviceInformationManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_DeviceInformationManager, Log, All);

UCLASS()
class UBS_DeviceInformationManager : public UBS_BaseManager
{
	GENERATED_BODY()

public:
	void Reset() override;
};
