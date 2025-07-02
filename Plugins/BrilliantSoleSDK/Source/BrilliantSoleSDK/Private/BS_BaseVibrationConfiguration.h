// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BS_VibrationType.h"
#include "BS_VibrationLocation.h"
#include "BS_VibrationWaveformEffect.h"
#include "BS_BaseVibrationConfiguration.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_BaseVibrationConfiguration, Warning, All);

UCLASS(Abstract)
class BRILLIANTSOLESDK_API UBS_BaseVibrationConfiguration : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BS Vibration Configuration")
    TSet<EBS_VibrationLocation> VibrationLocations;

    const TArray<uint8> ToArray() const;

protected:
    virtual const EBS_VibrationType GetVibrationType() const PURE_VIRTUAL(UBS_BaseVibrationConfiguration::GetVibrationType, return EBS_VibrationType::WAVEFORM_EFFECT;);
    virtual const TArray<uint8> GetVibrationData() const PURE_VIRTUAL(UBS_BaseVibrationConfiguration::GetVibrationData, return {};);

private:
    const uint8 GetVibrationLocationsBitmask() const;
};