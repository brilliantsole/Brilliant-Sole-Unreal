// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BS_BaseManager.h"
#include "BS_TfliteManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_TfliteManager, Log, All);

#define EBS_TfliteMessage BS_MessageTfliteGetName,               \
						  BS_MessageTfliteSetName,               \
						  BS_MessageTfliteGetTask,               \
						  BS_MessageTfliteSetTask,               \
						  BS_MessageTfliteGetSampleRate,         \
						  BS_MessageTfliteSetSampleRate,         \
						  BS_MessageTfliteGetSensorTypes,        \
						  BS_MessageTfliteSetSensorTypes,        \
						  BS_MessageTfliteGetIsReady,            \
						  BS_MessageTfliteGetCaptureDelay,       \
						  BS_MessageTfliteSetCaptureDelay,       \
						  BS_MessageTfliteGetThreshold,          \
						  BS_MessageTfliteSetThreshold,          \
						  BS_MessageTfliteGetInferencingEnabled, \
						  BS_MessageTfliteSetInferencingEnabled, \
						  BS_MessageTfliteInference

UCLASS()
class UBS_TfliteManager : public UBS_BaseManager
{
	GENERATED_BODY()

	public:
	bool OnRxMessage(uint8 MessageType, const TArray<uint8> &Message) override;
};
