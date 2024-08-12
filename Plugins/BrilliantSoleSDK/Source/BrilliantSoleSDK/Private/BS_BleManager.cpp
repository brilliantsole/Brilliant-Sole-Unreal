// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserves

#include "BS_BleManager.h"

// Sets default values
ABS_BleManager::ABS_BleManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ABS_BleManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABS_BleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
