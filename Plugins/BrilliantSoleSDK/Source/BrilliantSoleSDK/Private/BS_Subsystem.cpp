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

UClass *UBS_Subsystem::GetClass(const FString &ClassPath)
{
    ConstructorHelpers::FClassFinder<UObject> ClassFinder(*ClassPath);
    if (ClassFinder.Succeeded())
    {
        UE_LOGFMT(LogBS_Subsystem, Log, "Found Class at path: {0}", ClassPath);
        return ClassFinder.Class;
    }
    else
    {
        UE_LOGFMT(LogBS_Subsystem, Error, "Failed to find Class at path: {0}", ClassPath);
        return nullptr;
    }
}

void UBS_Subsystem::GetBleManagerClass()
{
    BleManagerClass = GetClass(TEXT("/Script/Engine.Blueprint'/BrilliantSoleSDK/Blueprints/Ble/BS_BleManagerBP.BS_BleManagerBP_C'"));
}

void UBS_Subsystem::GetDeviceManagerClass()
{
    DeviceManagerClass = GetClass(TEXT("/Script/Engine.Blueprint'/BrilliantSoleSDK/Blueprints/BS_DeviceManagerBP.BS_DeviceManagerBP_C'"));
}

void UBS_Subsystem::Initialize(FSubsystemCollectionBase &Collection)
{
    Super::Initialize(Collection);
}

void UBS_Subsystem::Deinitialize()
{
    Super::Deinitialize();
}

UObject *UBS_Subsystem::CreateSingleton(UClass *SingletonClass)
{
    UObject *Singleton;

    Singleton = NewObject<UObject>(this, SingletonClass);
    UE_LOGFMT(LogBS_Subsystem, Log, "Created new Singleton instance: {0}", Singleton->GetName());

    FName MethodName("Initialize");
    UFunction *InitalizeFunction = Singleton->FindFunction(MethodName);
    if (InitalizeFunction)
    {
        Singleton->ProcessEvent(InitalizeFunction, nullptr);
    }
    else
    {
        UE_LOGFMT(LogBS_Subsystem, Error, "Couldn't find Initialize function");
    }

    return Singleton;
}

UObject *UBS_Subsystem::GetBleManager()
{
    if (!BleManagerSingleton && BleManagerClass)
    {
        BleManagerSingleton = CreateSingleton(BleManagerClass);
    }
    return BleManagerSingleton;
}

UObject *UBS_Subsystem::GetDeviceManager()
{
    if (!DeviceManagerSingleton && DeviceManagerClass)
    {
        DeviceManagerSingleton = CreateSingleton(DeviceManagerClass);
    }
    return DeviceManagerSingleton;
}