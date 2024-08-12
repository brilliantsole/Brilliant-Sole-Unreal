// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserves

#include "BS_Subsystem.h"

UBS_Subsystem::UBS_Subsystem()
    : SingletonScannerInstance(nullptr)
{
}

void UBS_Subsystem::Initialize(FSubsystemCollectionBase &Collection)
{
    Super::Initialize(Collection);

    // Ensure the blueprint class is set
    if (!SingletonScannerInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("BS_BleScannerBlueprint is not set in the subsystem."));
        return;
    }

    // Instantiate the singleton instance if not already created
    if (!SingletonScannerInstance)
    {
        UWorld *World = GetWorld();
        if (World)
        {
            SingletonScannerInstance = World->SpawnActor<AActor>(BS_BleScannerBlueprint);
            if (SingletonScannerInstance)
            {
                UE_LOG(LogTemp, Log, TEXT("BS_BleScannerBlueprint instance created."));
            }
        }
    }
}

void UBS_Subsystem::Deinitialize()
{
    Super::Deinitialize();

    // Optionally destroy the singleton instance if needed
    if (SingletonScannerInstance)
    {
        SingletonScannerInstance->Destroy();
        SingletonScannerInstance = nullptr;
    }
}

AActor *UBS_Subsystem::GetScanner()
{
    return SingletonScannerInstance;
}