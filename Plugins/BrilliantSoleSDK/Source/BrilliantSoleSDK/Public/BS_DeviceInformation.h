// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Logging/StructuredLog.h"
#include "BS_DeviceInformationType.h"
#include "BS_DeviceInformation.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_DeviceInformation, Verbose, All);

USTRUCT(BlueprintType, meta = (ShowOnlyInnerProperties, FullyExpand = "true"))
struct FBS_DeviceInformation
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, Category = "BS Device Information")
    FString ManufacturerName;

    UPROPERTY(BlueprintReadOnly, Category = "BS Device Information")
    FString ModelNumber;

    UPROPERTY(BlueprintReadOnly, Category = "BS Device Information")
    FString SerialNumber;

    UPROPERTY(BlueprintReadOnly, Category = "BS Device Information")
    FString HardwareRevision;

    UPROPERTY(BlueprintReadOnly, Category = "BS Device Information")
    FString FirmwareRevision;

    UPROPERTY(BlueprintReadOnly, Category = "BS Device Information")
    FString SoftwareRevision;

private:
    bool bDidGetAllInformation;

public:
    bool GetDidGetAllInformation() const { return bDidGetAllInformation; }

public:
    void Reset();
    void SetValue(const EBS_DeviceInformationType DeviceInformationType, const TArray<uint8> &Value);
    void UpdateDidGetAllInformation();
};