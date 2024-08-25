// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BS_BaseManager.h"
#include "BS_SensorConfiguration.h"
#include "BS_SensorConfigurationManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_SensorConfigurationManager, Log, All);

DECLARE_DELEGATE_OneParam(FSensorConfigurationCallbackLocal, const UBS_SensorConfiguration *);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSensorConfigurationCallback, const UBS_SensorConfiguration *, SensorConfiguration);

#define EBS_SensorConfigurationMessage BS_MessageGetSensorConfiguration, \
									   BS_MessageSetSensorConfiguration

UCLASS()
class UBS_SensorConfigurationManager : public UBS_BaseManager
{
	GENERATED_BODY()

public:
	UBS_SensorConfigurationManager();

	bool OnRxMessage(uint8 MessageType, const TArray<uint8> &Message) override;
	void Reset() override;

public:
	const UBS_SensorConfiguration *GetSensorConfiguration() { return SensorConfiguration; }
	FSensorConfigurationCallbackLocal OnSensorConfigurationUpdate;

	void SetSensorConfiguration(const UBS_SensorConfiguration *NewSensorConfiguration);
	void ClearSensorConfiguration();

	EBS_SensorRate GetSensorRate(EBS_SensorType SensorType, bool &bContainsSensorType) const { return SensorConfiguration->GetSensorRate(SensorType, bContainsSensorType); }
	const TMap<EBS_SensorType, EBS_SensorRate> &GetSensorRates() const { return SensorConfiguration->GetSensorRates(); }

	void SetSensorRate(EBS_SensorType SensorType, EBS_SensorRate SensorRate, bool &bDidUpdateSensorRate);
	void SetSensorRates(const TMap<EBS_SensorType, EBS_SensorRate> &NewSensorRates);

	void ClearSensorRate(EBS_SensorType SensorType);
	void ToggleSensorRate(EBS_SensorType SensorType, EBS_SensorRate SensorRate, EBS_SensorRate &UpdatedSensorRate);

private:
	void ParseSensorConfiguration(const TArray<uint8> &Message);
	UBS_SensorConfiguration *SensorConfiguration;
	UBS_SensorConfiguration *TempSensorConfiguration;
};
