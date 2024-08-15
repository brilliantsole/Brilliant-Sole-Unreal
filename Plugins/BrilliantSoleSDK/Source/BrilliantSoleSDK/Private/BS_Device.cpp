// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_Device.h"

// Sets default values
ABS_Device::ABS_Device()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ABS_Device::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABS_Device::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
