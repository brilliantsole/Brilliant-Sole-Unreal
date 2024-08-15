// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Logging/LogMacros.h"
#include "BS_Subsystem.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogBS_Subsystem, Log, All);

UCLASS()
class BRILLIANTSOLESDK_API UBS_Subsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UBS_Subsystem();

	virtual void Initialize(FSubsystemCollectionBase &Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "BS Subsystem")
	UObject *GetBleManager() const
	{
		return BleManagerSingleton;
	}
	UFUNCTION(BlueprintCallable, Category = "BS Subsystem")
	void SetBleManager(UObject *NewBleManagerSingleton)
	{
		if (BleManagerSingleton != nullptr)
		{
			UE_LOG(LogBS_Subsystem, Warning, TEXT("already assigned BleManagerSingleton"));
			return;
		}
		BleManagerSingleton = NewBleManagerSingleton;
		UE_LOG(LogBS_Subsystem, Log, TEXT("assigned BleManagerSingleton"));
	}

private:
	UPROPERTY()
	UObject *BleManagerSingleton;
};
