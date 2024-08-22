#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BS_SensorType.h"
#include "BS_SensorRate.h"
#include "BS_SensorConfiguration.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_SensorConfiguration, Log, All);

USTRUCT(BlueprintType)
struct FBS_SensorConfiguration
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, Category = "BS Sensor Configuration")
    TMap<EBS_SensorType, EBS_SensorRate> SensorRates;

    void Reset() { SensorRates.Reset(); }

    FString ToString();

    static EBS_SensorRate GetClosestSensorRate(uint16 RawSensorRate);
    static uint16 GetRawSensorRate(EBS_SensorRate SensorRate) { return static_cast<uint16>(SensorRate) * 10; }

    void Parse(const TArray<uint8> &Message);
};