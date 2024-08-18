// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_Subsystem.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"

UBS_Subsystem::UBS_Subsystem()
{
    static ConstructorHelpers::FClassFinder<UObject> BleManagerClassFinder(TEXT("/Script/Engine.Blueprint'/BrilliantSoleSDK/Blueprints/Ble/BS_BleManagerBP.BS_BleManagerBP_C'"));
    if (BleManagerClassFinder.Succeeded())
    {
        BleManagerClass = BleManagerClassFinder.Class;
        UE_LOG(LogBS_Subsystem, Log, TEXT("found BleManagerClass"));
    }
    else
    {
        UE_LOG(LogBS_Subsystem, Error, TEXT("failed to find BleManagerClass"));
    }
}

void UBS_Subsystem::Initialize(FSubsystemCollectionBase &Collection)
{
    Super::Initialize(Collection);
}

void UBS_Subsystem::Deinitialize()
{
    Super::Deinitialize();
}

UObject *UBS_Subsystem::GetBleManager()
{
    if (!BleManagerSingleton && BleManagerClass)
    {
        BleManagerSingleton = NewObject<UObject>(this, BleManagerClass);
        UE_LOG(LogBS_Subsystem, Log, TEXT("Created new BleManager instance: %s"), *BleManagerSingleton->GetName());

        FName MethodName("Initialize");
        UFunction *InitalizeFunction = BleManagerSingleton->FindFunction(MethodName);
        if (InitalizeFunction)
        {
            BleManagerSingleton->ProcessEvent(InitalizeFunction, nullptr);
        }
        else
        {
            UE_LOG(LogBS_Subsystem, Error, TEXT("Couldn't find Initialize function"));
        }
    }

    return BleManagerSingleton;
}