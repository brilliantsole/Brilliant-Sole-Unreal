// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserves

#include "BS_BleScanner.h"

// Sets default values
ABS_BleScanner::ABS_BleScanner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ABS_BleScanner::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABS_BleScanner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
