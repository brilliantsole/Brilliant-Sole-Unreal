// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BS_BaseManager.h"
#include "BS_SensorConfiguration.h"
#include "BS_SensorConfigurationManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_SensorConfigurationManager, Log, All);

DECLARE_DELEGATE_OneParam(FSensorConfigurationCallbackLocal, FBS_SensorConfiguration &);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSensorConfigurationCallback, FBS_SensorConfiguration &, SensorConfiguration);

#define EBS_SensorConfigurationMessage BS_MessageGetSensorConfiguration, \
									   BS_MessageSetSensorConfiguration

UCLASS()
class UBS_SensorConfigurationManager : public UBS_BaseManager
{
	GENERATED_BODY()

public:
	bool OnRxMessage(uint8 MessageType, const TArray<uint8> &Message) override;
	void Reset() override;

public:
	FBS_SensorConfiguration &GetSensorConfiguration() { return SensorConfiguration; }
	FSensorConfigurationCallbackLocal OnSensorConfigurationUpdate;

private:
	void ParseSensorConfiguration(const TArray<uint8> &Message);
	FBS_SensorConfiguration SensorConfiguration;

	void SetSensorConfiguration(const FBS_SensorConfiguration &NewConfiguration);
	void ClearSensorConfiguration();
};
