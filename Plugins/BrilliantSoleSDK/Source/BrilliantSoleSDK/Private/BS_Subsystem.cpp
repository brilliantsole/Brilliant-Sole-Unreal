// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_Subsystem.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Logging/StructuredLog.h"

UBS_Subsystem::UBS_Subsystem()
{
    GetBleManagerClass();
    GetDeviceManagerClass();
    GetUDP_ClientClass();
    GetDeviceBP_Class();
}

void UBS_Subsystem::Initialize(FSubsystemCollectionBase &Collection)
{
    Super::Initialize(Collection);
}

void UBS_Subsystem::Deinitialize()
{
    Super::Deinitialize();
}

// HELPERS START
UClass *UBS_Subsystem::GetClass(const FString &ClassPath)
{
    ConstructorHelpers::FClassFinder<UObject> ClassFinder(*ClassPath);
    if (ClassFinder.Succeeded())
    {
        UE_LOGFMT(LogBS_Subsystem, Verbose, "Found Class at path: {0}", ClassPath);
        return ClassFinder.Class;
    }
    else
    {
        UE_LOGFMT(LogBS_Subsystem, Error, "Failed to find Class at path: {0}", ClassPath);
        return nullptr;
    }
}

UObject *UBS_Subsystem::CreateSingleton(UClass *SingletonClass, bool bInitialize)
{
    UObject *Singleton;

    Singleton = NewObject<UObject>(this, SingletonClass);
    UE_LOGFMT(LogBS_Subsystem, Verbose, "Created new Singleton instance: {0}", Singleton->GetName());

    if (bInitialize)
    {
        CallFunction(Singleton, "Initialize");
    }

    return Singleton;
}

void UBS_Subsystem::CallFunction(UObject *Singleton, FString FunctionName)
{
    FName MethodName(FunctionName);
    UFunction *InitalizeFunction = Singleton->FindFunction(MethodName);
    if (InitalizeFunction)
    {
        Singleton->ProcessEvent(InitalizeFunction, nullptr);
    }
    else
    {
        UE_LOGFMT(LogBS_Subsystem, Error, "Couldn't find Initialize function");
    }
}
// HELPERS END

// BS DEVICE BP START
void UBS_Subsystem::GetDeviceBP_Class()
{
    DeviceBP_Class = GetClass(TEXT("/Script/Engine.Blueprint'/BrilliantSoleSDK/Blueprints/BS_DeviceBP.BS_DeviceBP_C'"));
}
// BS DEVICE BP END

// BLE MANAGER START
void UBS_Subsystem::GetBleManagerClass()
{
    BleManagerClass = GetClass(TEXT("/Script/Engine.Blueprint'/BrilliantSoleSDK/Blueprints/Ble/BS_BleManagerBP.BS_BleManagerBP_C'"));
}

UObject *UBS_Subsystem::GetBleManager()
{
    if (!BleManagerSingleton && BleManagerClass)
    {
        BleManagerSingleton = CreateSingleton(BleManagerClass, true);
        GetDeviceManager();

        FName MethodName("AssignBS_DeviceManager");
        UFunction *InitalizeFunction = BleManagerSingleton->FindFunction(MethodName);
        if (InitalizeFunction)
        {
            UE_LOGFMT(LogBS_Subsystem, Log, "Assigning DeviceManagerSingleton to BleManagerSingleton...");
            BleManagerSingleton->ProcessEvent(InitalizeFunction, &DeviceManagerSingleton);
        }
        else
        {
            UE_LOGFMT(LogBS_Subsystem, Error, "Couldn't find AssignBS_DeviceManager function");
        }
    }
    return BleManagerSingleton;
}

// BLE MANAGER END

// DEVICE MANAGER START
void UBS_Subsystem::GetDeviceManagerClass()
{
    DeviceManagerClass = GetClass(TEXT("/Script/Engine.Blueprint'/BrilliantSoleSDK/Blueprints/BS_DeviceManagerBP.BS_DeviceManagerBP_C'"));
}

UObject *UBS_Subsystem::GetDeviceManager()
{
    if (!DeviceManagerSingleton && DeviceManagerClass)
    {
        DeviceManagerSingleton = CreateSingleton(DeviceManagerClass, true);
    }
    return DeviceManagerSingleton;
}
// DEVICE MANAGER END

// UDP MANAGER START
void UBS_Subsystem::GetUDP_ClientClass()
{
    UDP_ClientClass = GetClass(TEXT("/Script/Engine.Blueprint'/BrilliantSoleSDK/Blueprints/UDP/BS_UDP_ClientBP.BS_UDP_ClientBP_C'"));
}

UObject *UBS_Subsystem::GetUDP_Client()
{
    if (!UDP_ClientSingleton && UDP_ClientClass)
    {
        UDP_ClientSingleton = CreateSingleton(UDP_ClientClass, false);
    }
    return UDP_ClientSingleton;
}

// UDP MANAGER END