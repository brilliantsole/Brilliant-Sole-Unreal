// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BS_BaseManager.h"
#include "BS_VibrationConfiguration.h"
#include "BS_VibrationMessageType.h"
#include "BS_VibrationLocation.h"
#include "BS_VibrationManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_VibrationManager, Warning, All);

DECLARE_DELEGATE_OneParam(FBS_VibrationLocationsCallbackLocal, const TArray<EBS_VibrationLocation> &);

UCLASS()
class UBS_VibrationManager : public UBS_BaseManager
{
	GENERATED_BODY()

public:
	bool OnRxMessage(EBS_TxRxMessage MessageType, const TArrayView<const uint8> &Message) override;
	void Reset() override;

	// VIBRATION LOCATIONS START
public:
	const TArray<EBS_VibrationLocation> &GetVibrationLocations() const { return VibrationLocations; }
	FBS_VibrationLocationsCallbackLocal OnVibrationLocationsUpdate;

private:
	UPROPERTY()
	TArray<EBS_VibrationLocation> VibrationLocations;
	void ParseVibrationLocations(const TArrayView<const uint8> &Message);
	// VIBRATION LOCATIONS END

	// TRIGGER VIBRATION START
public:
	void TriggerVibration(const TArray<FBS_VibrationConfiguration> &VibrationConfigurations);
	// TRIGGER VIBRATION END
};
