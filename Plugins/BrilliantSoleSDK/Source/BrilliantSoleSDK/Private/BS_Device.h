// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BS_Device.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_Device, Log, Warning);

UCLASS()
class ABS_Device : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABS_Device();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
