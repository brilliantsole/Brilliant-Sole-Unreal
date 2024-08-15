// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_Subsystem.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"

UBS_Subsystem::UBS_Subsystem()
{
    UE_LOG(LogBS_Subsystem, Log, TEXT("UBS_Subsystem"));
}

void UBS_Subsystem::Initialize(FSubsystemCollectionBase &Collection)
{
    Super::Initialize(Collection);
    UE_LOG(LogBS_Subsystem, Log, TEXT("UBS_Subsystem::Initialize"));
    BleManagerSingleton = CreateBlueprintInstance("/Script/Engine.Blueprint'/BrilliantSoleSDK/Blueprints/Ble/BS_BleManagerBlueprint.BS_BleManagerBlueprint'");
}

void UBS_Subsystem::Deinitialize()
{
    Super::Deinitialize();
}

AActor *UBS_Subsystem::CreateBlueprintInstance(const FString &BlueprintPath)
{
    UClass *LoadedClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *BlueprintPath));

    AActor *BlueprintInstance = nullptr;

    if (LoadedClass)
    {
        BlueprintInstance = NewObject<AActor>(this, LoadedClass);
        UE_LOG(LogBS_Subsystem, Log, TEXT("Created new singleton instance: %s"), *BlueprintInstance->GetName());
    }
    else
    {
        UE_LOG(LogBS_Subsystem, Error, TEXT("Failed to load Blueprint class from path: %s"), *BlueprintPath);
    }

    return BlueprintInstance;
}