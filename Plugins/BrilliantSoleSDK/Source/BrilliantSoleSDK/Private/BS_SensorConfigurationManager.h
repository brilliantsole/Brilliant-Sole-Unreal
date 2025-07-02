// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BS_BaseManager.h"
#include "BS_SensorConfiguration.h"
#include "BS_SensorConfigurationMessageType.h"
#include "BS_SensorConfigurationManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_SensorConfigurationManager, Warning, All);

DECLARE_DELEGATE_OneParam(FBS_SensorConfigurationCallbackLocal, const UBS_SensorConfiguration *);

UCLASS()
class UBS_SensorConfigurationManager : public UBS_BaseManager
{
	GENERATED_BODY()

public:
	UBS_SensorConfigurationManager();

	bool OnRxMessage(EBS_TxRxMessage MessageType, const TArrayView<const uint8> &Message) override;
	void Reset() override;

public:
	const UBS_SensorConfiguration *GetSensorConfiguration() { return SensorConfiguration; }
	FBS_SensorConfigurationCallbackLocal OnSensorConfigurationUpdate;

	void SetSensorConfiguration(const UBS_SensorConfiguration *NewSensorConfiguration, bool bClearRest = false);
	void ClearSensorConfiguration();

	EBS_SensorRate GetSensorRate(EBS_SensorType SensorType, bool &bContainsSensorType) const { return SensorConfiguration->GetSensorRate(SensorType, bContainsSensorType); }
	const TMap<EBS_SensorType, EBS_SensorRate> &GetSensorRates() const { return SensorConfiguration->GetSensorRates(); }

	bool IsSensorRateNonZero(EBS_SensorType SensorType) const { return SensorConfiguration->IsSensorRateNonZero(SensorType); }

	void SetSensorRate(EBS_SensorType SensorType, EBS_SensorRate SensorRate, bool &bDidUpdateSensorRate);
	void SetSensorRates(const TMap<EBS_SensorType, EBS_SensorRate> &NewSensorRates);

	void ClearSensorRate(EBS_SensorType SensorType);
	void ToggleSensorRate(EBS_SensorType SensorType, EBS_SensorRate SensorRate, EBS_SensorRate &UpdatedSensorRate);

private:
	void ParseSensorConfiguration(const TArrayView<const uint8> &Message);
	UPROPERTY()
	UBS_SensorConfiguration *SensorConfiguration;
	UPROPERTY()
	UBS_SensorConfiguration *TempSensorConfiguration;
	UPROPERTY()
	UBS_SensorConfiguration *TempSensorConfiguration2;
};
