#pragma once

#include "CoreMinimal.h"
#include "BS_SensorType.h"
#include "BS_SensorRate.h"
#include "BS_SensorConfiguration.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_SensorConfiguration, Verbose, All);

UCLASS(Blueprintable)
class BRILLIANTSOLESDK_API UBS_SensorConfiguration : public UObject
{
    GENERATED_BODY()

public:
    void Copy(const UBS_SensorConfiguration *Other);
    bool IsEqualTo(const UBS_SensorConfiguration *Other);

    // SENSOR RATES START
public:
    UFUNCTION(BlueprintPure, Category = "BS Sensor Configuration")
    const TMap<EBS_SensorType, EBS_SensorRate> &GetSensorRates() const { return SensorRates; };

    UFUNCTION(BlueprintCallable, Category = "BS Sensor Configuration")
    void Clear();

    UFUNCTION(BlueprintCallable, Category = "BS Sensor Configuration")
    EBS_SensorRate GetSensorRate(EBS_SensorType SensorType, bool &bContainsSensorType) const;
    EBS_SensorRate GetSensorRate(EBS_SensorType SensorType) const;

    UFUNCTION(BlueprintCallable, Category = "BS Sensor Configuration")
    bool IsSensorRateNonZero(EBS_SensorType SensorType) const;

    UFUNCTION(BlueprintCallable, Category = "BS Sensor Configuration")
    void SetSensorRate(EBS_SensorType SensorType, EBS_SensorRate SensorRate, bool &bDidUpdateSensorRate);
    void SetSensorRate(EBS_SensorType SensorType, EBS_SensorRate SensorRate);

    UFUNCTION(BlueprintCallable, Category = "BS Sensor Configuration")
    void SetSensorRates(const TMap<EBS_SensorType, EBS_SensorRate> &NewSensorRates);

    UFUNCTION(BlueprintCallable, Category = "BS Sensor Configuration")
    void ClearSensorRate(EBS_SensorType SensorType);

    UFUNCTION(BlueprintCallable, Category = "BS Sensor Configuration")
    void ToggleSensorRate(EBS_SensorType SensorType, EBS_SensorRate SensorRate, EBS_SensorRate &UpdatedSensorRate);
    void ToggleSensorRate(EBS_SensorType SensorType, EBS_SensorRate SensorRate);

private:
    UPROPERTY()
    TMap<EBS_SensorType, EBS_SensorRate> SensorRates;
    // SENSOR RATES END

    // SENSOR TYPES START
public:
    UFUNCTION(BlueprintPure, Category = "BS Sensor Configuration")
    const TArray<EBS_SensorType> &GetSensorTypes()
    {
        if (bSensorTypesNeedsUpdate)
        {
            UpdateSensorTypes();
        }
        return SensorTypes;
    }

private:
    UPROPERTY()
    TArray<EBS_SensorType> SensorTypes;
    UPROPERTY()
    bool bSensorTypesNeedsUpdate = false;
    void UpdateSensorTypes();
    // SENSOR TYPES END

    // UTILS START
public:
    FString ToString() const;
    void Reset() { SensorRates.Reset(); }

    static EBS_SensorRate GetClosestSensorRate(uint16 RawSensorRate);
    static uint16 GetRawSensorRate(EBS_SensorRate SensorRate) { return static_cast<uint16>(SensorRate); }

    void Parse(const TArray<uint8> &Message);

    const TArray<uint8> ToArray() const;
    // UTILS END
};