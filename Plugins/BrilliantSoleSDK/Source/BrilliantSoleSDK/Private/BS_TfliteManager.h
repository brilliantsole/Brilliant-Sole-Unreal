// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BS_BaseManager.h"
#include "BS_TfliteTask.h"
#include "BS_SensorType.h"
#include "BS_SensorRate.h"
#include "BS_TfliteConfiguration.h"
#include "BS_TfliteMessageType.h"
#include "BS_TfliteManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_TfliteManager, Log, All);

UCLASS()
class UBS_TfliteManager : public UBS_BaseManager
{
	GENERATED_BODY()

public:
	bool OnRxMessage(EBS_TxRxMessage MessageType, const TArrayView<const uint8> &Message) override;
	void Reset() override;

public:
	void SetConfiguration(const FBS_TfliteConfiguration &TfliteConfiguration, bool bSendImmediately);

	// NAME START
public:
	const FString &GetName() const { return Name; }
	DECLARE_DELEGATE_OneParam(FBS_TfliteNameCallbackLocal, const FString &);
	FBS_TfliteNameCallbackLocal OnNameUpdate;

	static const uint8 MinNameLength;
	static const uint8 MaxNameLength;
	void SetName(const FString &NewName, bool bSendImmediately);

private:
	UPROPERTY()
	FString Name;
	void ParseName(const TArrayView<const uint8> &Message);
	// NAME END

	// TASK START
public:
	const EBS_TfliteTask &GetTask() const { return Task; }
	DECLARE_DELEGATE_OneParam(FBS_TfliteTaskCallbackLocal, EBS_TfliteTask);
	FBS_TfliteTaskCallbackLocal OnTaskUpdate;

	void SetTask(const EBS_TfliteTask NewTask, bool bSendImmediately);

private:
	UPROPERTY()
	EBS_TfliteTask Task;
	void ParseTask(const TArrayView<const uint8> &Message);
	// TASK END

	// SAMPLE RATE START
public:
	const EBS_SensorRate GetSampleRate() const { return SampleRate; }
	DECLARE_DELEGATE_OneParam(FBS_TfliteSampleRateCallbackLocal, EBS_SensorRate);
	FBS_TfliteSampleRateCallbackLocal OnSampleRateUpdate;

	void SetSampleRate(const EBS_SensorRate NewSampleRate, bool bSendImmediately);

private:
	UPROPERTY()
	EBS_SensorRate SampleRate = EBS_SensorRate::Value0;
	void ParseSampleRate(const TArrayView<const uint8> &Message);
	// SAMPLE RATE END

	// SENSOR TYPES START
public:
	const TArray<EBS_SensorType> &GetSensorTypes() const { return SensorTypes; }
	DECLARE_DELEGATE_OneParam(FBS_TfliteSensorTypesCallbackLocal, const TArray<EBS_SensorType> &);
	FBS_TfliteSensorTypesCallbackLocal OnSensorTypesUpdate;

	void SetSensorTypes(const TArray<EBS_SensorType> &NewSensorTypes, bool bSendImmediately);

private:
	UPROPERTY()
	TArray<EBS_SensorType> SensorTypes;
	bool IsSensorTypeValid(const EBS_SensorType SensorType);
	void ParseSensorTypes(const TArrayView<const uint8> &Message);
	// SENSOR TYPES END

	// IS READY START
public:
	const bool GetIsReady() const { return IsReady; }
	DECLARE_DELEGATE_OneParam(FBS_TfliteIsReadyCallbackLocal, bool);
	FBS_TfliteIsReadyCallbackLocal OnIsReadyUpdate;

private:
	UPROPERTY()
	bool IsReady = false;
	void ParseIsReady(const TArrayView<const uint8> &Message);
	// IS READY END

	// CAPTURE DELAY START
public:
	const uint16 GetCaptureDelay() const { return CaptureDelay; }
	DECLARE_DELEGATE_OneParam(FBS_TfliteCaptureDelayCallbackLocal, uint16);
	FBS_TfliteCaptureDelayCallbackLocal OnCaptureDelayUpdate;

	void SetCaptureDelay(const uint16 NewCaptureDelay, bool bSendImmediately);

private:
	UPROPERTY()
	uint16 CaptureDelay = 0;
	void ParseCaptureDelay(const TArrayView<const uint8> &Message);
	// CAPTURE DELAY END

	// THRESHOLD START
public:
	const float GetThreshold() const { return Threshold; }
	DECLARE_DELEGATE_OneParam(FBS_TfliteThresholdCallbackLocal, float);
	FBS_TfliteThresholdCallbackLocal OnThresholdUpdate;

	void SetThreshold(const float NewThreshold, bool bSendImmediately);

private:
	UPROPERTY()
	float Threshold = 0.0f;
	void ParseThreshold(const TArrayView<const uint8> &Message);
	// THRESHOLD END

	// INFERENCING ENABLED START
public:
	const bool GetInferencingEnabled() const { return InferencingEnabled; }
	DECLARE_DELEGATE_OneParam(FBS_TfliteInferencingEnabledCallbackLocal, bool);
	FBS_TfliteInferencingEnabledCallbackLocal OnInferencingEnabledUpdate;

	void SetInferencingEnabled(const bool NewInferencingEnabled);
	void ToggleInferencingEnabled() { SetInferencingEnabled(!InferencingEnabled); }

private:
	UPROPERTY()
	bool InferencingEnabled = false;
	void ParseInferencingEnabled(const TArrayView<const uint8> &Message);
	// INFERENCING ENABLED END

	// INFERENCE START
public:
	DECLARE_DELEGATE_TwoParams(FBS_TfliteInferenceCallbackLocal, const TArray<float> &, const uint64 &);
	FBS_TfliteInferenceCallbackLocal OnInferenceUpdate;

private:
	void ParseInference(const TArrayView<const uint8> &Message);
	// INFERENCE END
};
