// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_Subsystem.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Logging/StructuredLog.h"

UBS_Subsystem::UBS_Subsystem()
{
    GetBleManagerClass();
    GetDeviceManagerClass();
}

void UBS_Subsystem::GetBleManagerClass()
{
    static ConstructorHelpers::FClassFinder<UObject> BleManagerClassFinder(TEXT("/Script/Engine.Blueprint'/BrilliantSoleSDK/Blueprints/Ble/BS_BleManagerBP.BS_BleManagerBP_C'"));
    if (BleManagerClassFinder.Succeeded())
    {
        BleManagerClass = BleManagerClassFinder.Class;
        UE_LOGFMT(LogBS_Subsystem, Log, "found BleManagerClass");
    }
    else
    {
        UE_LOGFMT(LogBS_Subsystem, Error, "failed to find BleManagerClass");
    }
}

void UBS_Subsystem::GetDeviceManagerClass()
{
    static ConstructorHelpers::FClassFinder<UObject> DeviceManagerClassFinder(TEXT("/Script/Engine.Blueprint'/BrilliantSoleSDK/Blueprints/BS_DeviceManagerBP.BS_DeviceManagerBP_C'"));
    if (DeviceManagerClassFinder.Succeeded())
    {
        DeviceManagerClass = DeviceManagerClassFinder.Class;
        UE_LOGFMT(LogBS_Subsystem, Log, "found DeviceManagerClass");
    }
    else
    {
        UE_LOGFMT(LogBS_Subsystem, Error, "failed to find DeviceManagerClass");
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
        UE_LOGFMT(LogBS_Subsystem, Log, "Created new BleManager instance: {0}", BleManagerSingleton->GetName());

        FName MethodName("Initialize");
        UFunction *InitalizeFunction = BleManagerSingleton->FindFunction(MethodName);
        if (InitalizeFunction)
        {
            BleManagerSingleton->ProcessEvent(InitalizeFunction, nullptr);
        }
        else
        {
            UE_LOGFMT(LogBS_Subsystem, Error, "Couldn't find Initialize function");
        }
    }

    return BleManagerSingleton;
}

UObject *UBS_Subsystem::GetDeviceManager()
{
    if (!DeviceManagerSingleton && DeviceManagerClass)
    {
        DeviceManagerSingleton = NewObject<UObject>(this, DeviceManagerClass);
        UE_LOGFMT(LogBS_Subsystem, Log, "Created new DeviceManager instance: {0}", DeviceManagerSingleton->GetName());

        FName MethodName("Initialize");
        UFunction *InitalizeFunction = DeviceManagerSingleton->FindFunction(MethodName);
        if (InitalizeFunction)
        {
            DeviceManagerSingleton->ProcessEvent(InitalizeFunction, nullptr);
        }
        else
        {
            UE_LOGFMT(LogBS_Subsystem, Error, "Couldn't find Initialize function");
        }
    }

    return DeviceManagerSingleton;
}