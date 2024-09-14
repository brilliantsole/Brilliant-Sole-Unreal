// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BS_TfliteTask.h"
#include "BS_SensorRate.h"
#include "BS_SensorType.h"
#include "BS_TfliteSensorTypeBitFlag.h"
#include "Logging/StructuredLog.h"
#include "BS_TfliteConfiguration.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_TfliteConfiguration, Verbose, All);

USTRUCT(BlueprintType, meta = (ShowOnlyInnerProperties, FullyExpand = "true"))
struct FBS_TfliteConfiguration
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BS Tflite Configuration")
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BS Tflite Configuration")
    EBS_TfliteTask Task;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BS Tflite Configuration")
    EBS_SensorRate SampleRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BS Tflite Configuration", meta = (DisplayName = "Sensor Types", Bitmask, BitmaskEnum = "/Script/BrilliantSoleSDK.EBS_TfliteSensorTypeBitFlag"))
    uint8 SensorTypesBitmask = 0;
    const TArray<EBS_SensorType> GetSensorTypes() const
    {
        TArray<EBS_SensorType> SensorTypes;
        for (auto TfliteSensorType : TEnumRange<EBS_TfliteSensorTypeBitFlag>())
        {
            const uint8 TfliteSensorTypeBitFlag = static_cast<uint8>(TfliteSensorType);
            if (SensorTypesBitmask & TfliteSensorTypeBitFlag)
            {
                UE_LOGFMT(LogTemp, Verbose, "Found TfliteSensorType {0}", UEnum::GetValueAsString(TfliteSensorType));
                switch (TfliteSensorType)
                {
                case EBS_TfliteSensorTypeBitFlag::NONE:
                case EBS_TfliteSensorTypeBitFlag::COUNT:
                    UE_LOGFMT(LogTemp, Verbose, "Skipping {0}", UEnum::GetValueAsString(TfliteSensorType));
                    break;
                case EBS_TfliteSensorTypeBitFlag::PRESSURE:
                    SensorTypes.Add(EBS_SensorType::PRESSURE);
                    break;
                case EBS_TfliteSensorTypeBitFlag::LINEAR_ACCELERATION:
                    SensorTypes.Add(EBS_SensorType::LINEAR_ACCELERATION);
                    break;
                case EBS_TfliteSensorTypeBitFlag::GYROSCOPE:
                    SensorTypes.Add(EBS_SensorType::GYROSCOPE);
                    break;
                case EBS_TfliteSensorTypeBitFlag::MAGNETOMETER:
                    SensorTypes.Add(EBS_SensorType::MAGNETOMETER);
                    break;
                default:
                    UE_LOGFMT(LogTemp, Verbose, "Skipping TfliteSensorType {0} (Raw {1})", UEnum::GetValueAsString(TfliteSensorType), TfliteSensorTypeBitFlag);
                    break;
                }
            }
        }
        return SensorTypes;
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BS Tflite Configuration", meta = (DisplayName = "Capture Delay (ms)", ClampMin = "0", ClampMax = "5000", UIMin = "0", UIMax = "5000"))
    int32 CaptureDelay;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BS Tflite Configuration", meta = (ClampMin = "0", ClampMax = "1", UIMin = "0", UIMax = "1"))
    float Threshold;
};