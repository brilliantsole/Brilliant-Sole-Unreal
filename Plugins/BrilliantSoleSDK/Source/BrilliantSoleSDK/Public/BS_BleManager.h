// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserves

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BS_BleManager.generated.h"

UCLASS()
class BRILLIANTSOLESDK_API ABS_BleManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABS_BleManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
