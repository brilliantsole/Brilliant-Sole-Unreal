// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserves

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BS_Subsystem.generated.h"

UCLASS()
class BRILLIANTSOLESDK_API UBS_Subsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UBS_Subsystem();

	virtual void Initialize(FSubsystemCollectionBase &Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "BS Ble Scanner")
	AActor *GetScanner();

private:
	UPROPERTY()
	AActor *SingletonScannerInstance;

	UPROPERTY(EditDefaultsOnly, Category = "BS Ble Scanner")
	TSubclassOf<AActor> BS_BleScannerBlueprint;
};
