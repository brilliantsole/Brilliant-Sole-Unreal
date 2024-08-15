// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_Subsystem.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"

UBS_Subsystem::UBS_Subsystem()
{
    GetBleManager();
}

void UBS_Subsystem::Initialize(FSubsystemCollectionBase &Collection)
{
    Super::Initialize(Collection);
}

void UBS_Subsystem::Deinitialize()
{
    Super::Deinitialize();
}

//"/BrilliantSoleSDK/Blueprints/Ble/BS_BleManagerBlueprint.BS_BleManagerBlueprint_C"