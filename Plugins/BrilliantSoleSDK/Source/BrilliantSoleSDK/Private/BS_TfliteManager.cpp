// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_TfliteManager.h"
#include "Logging/StructuredLog.h"
#include "BS_ByteParser.h"
#include "BS_TxRxMessageType.h"
#include "BS_TimeUtils.h"
#include "BS_MathUtils.h"
#include "BS_SensorConfiguration.h"

DEFINE_LOG_CATEGORY(LogBS_TfliteManager);

bool UBS_TfliteManager::OnRxMessage(EBS_TxRxMessage MessageType, const TArrayView<const uint8> &Message)
{
    switch (MessageType)
    {
    case EBS_TxRxMessage::GET_TFLITE_NAME:
    case EBS_TxRxMessage::SET_TFLITE_NAME:
        ParseName(Message);
        break;
    case EBS_TxRxMessage::GET_TFLITE_TASK:
    case EBS_TxRxMessage::SET_TFLITE_TASK:
        ParseTask(Message);
        break;
    case EBS_TxRxMessage::GET_TFLITE_SAMPLE_RATE:
    case EBS_TxRxMessage::SET_TFLITE_SAMPLE_RATE:
        ParseSampleRate(Message);
        break;
    case EBS_TxRxMessage::GET_TFLITE_SENSOR_TYPES:
    case EBS_TxRxMessage::SET_TFLITE_SENSOR_TYPES:
        ParseSensorTypes(Message);
        break;
    case EBS_TxRxMessage::IS_TFLITE_READY:
        ParseIsReady(Message);
        break;
    case EBS_TxRxMessage::GET_TFLITE_CAPTURE_DELAY:
    case EBS_TxRxMessage::SET_TFLITE_CAPTURE_DELAY:
        ParseCaptureDelay(Message);
        break;
    case EBS_TxRxMessage::GET_TFLITE_THRESHOLD:
    case EBS_TxRxMessage::SET_TFLITE_THRESHOLD:
        ParseThreshold(Message);
        break;
    case EBS_TxRxMessage::GET_TFLITE_INFERENCING_ENABLED:
    case EBS_TxRxMessage::SET_TFLITE_INFERENCING_ENABLED:
        ParseInferencingEnabled(Message);
        break;
    case EBS_TxRxMessage::TFLITE_INFERENCE:
        ParseInference(Message);
        break;
    default:
        return false;
    }
    return true;
}

void UBS_TfliteManager::Reset()
{
    Name = "";
    Task = EBS_TfliteTask::CLASSIFICATION;
    SampleRate = EBS_SensorRate::Value0;
    SensorTypes.Reset();
    IsReady = false;
    CaptureDelay = 0;
    Threshold = 0.0f;
    InferencingEnabled = false;
}

void UBS_TfliteManager::SetConfiguration(const FBS_TfliteConfiguration &_TfliteConfiguration, bool bSendImmediately)
{
    UE_LOGFMT(LogBS_TfliteManager, Verbose, "Setting Configuration...");
    TfliteConfiguration = _TfliteConfiguration;
    SetName(TfliteConfiguration.Name, false);
    SetTask(TfliteConfiguration.Task, false);
    SetSensorTypes(TfliteConfiguration.GetSensorTypes(), false);
    SetSampleRate(TfliteConfiguration.SampleRate, false);
    SetCaptureDelay(BS_MathUtils::ClampToRange<uint16>(TfliteConfiguration.CaptureDelay), false);
    SetThreshold(TfliteConfiguration.Threshold, bSendImmediately);
}

// NAME START
void UBS_TfliteManager::ParseName(const TArrayView<const uint8> &Message)
{
    UE_LOGFMT(LogBS_TfliteManager, Verbose, "Parsing Name...");
    Name = BS_ByteParser::GetString(Message);
    UE_LOGFMT(LogBS_TfliteManager, Verbose, "Parsed Name: {0}", Name);
    OnNameUpdate.ExecuteIfBound(Name);
}

const uint8 UBS_TfliteManager::MinNameLength = 0;
const uint8 UBS_TfliteManager::MaxNameLength = 30;
void UBS_TfliteManager::SetName(const FString &NewName, bool bSendImmediately)
{
    if (NewName == Name)
    {
        UE_LOGFMT(LogBS_TfliteManager, Verbose, "Redundant Name - not setting");
        return;
    }
    const auto NewNameLength = NewName.Len();
    if (NewNameLength < MinNameLength || NewNameLength > MaxNameLength)
    {
        UE_LOGFMT(LogBS_TfliteManager, Warning, "Name must be between {0}-{1} characters long (got {2})", MinNameLength, MaxNameLength, NewNameLength);
        return;
    }
    UE_LOGFMT(LogBS_TfliteManager, Verbose, "Setting Name to {0}...", NewName);

    const TArray<uint8> TxMessage = BS_ByteParser::StringToArray(NewName);
    SendTxMessages.ExecuteIfBound({{EBS_TxRxMessage::SET_TFLITE_NAME, TxMessage}}, bSendImmediately);
}
// NAME END

// TASK START
void UBS_TfliteManager::ParseTask(const TArrayView<const uint8> &Message)
{
    Task = static_cast<EBS_TfliteTask>(Message[0]);
    UE_LOGFMT(LogBS_TfliteManager, Verbose, "Parsed Task: {0}", UEnum::GetValueAsString(Task));
    OnTaskUpdate.ExecuteIfBound(Task);
}

void UBS_TfliteManager::SetTask(const EBS_TfliteTask NewTask, bool bSendImmediately)
{
    if (NewTask == Task)
    {
        UE_LOGFMT(LogBS_TfliteManager, Verbose, "Redundant Task - not setting");
        return;
    }
    UE_LOGFMT(LogBS_TfliteManager, Verbose, "Setting Task to {0}...", UEnum::GetValueAsString(NewTask));

    const TArray<uint8> TxMessage = {static_cast<uint8>(NewTask)};
    SendTxMessages.ExecuteIfBound({{EBS_TxRxMessage::SET_TFLITE_TASK, TxMessage}}, bSendImmediately);
}
// TASK END

// SAMPLE RATE START
void UBS_TfliteManager::ParseSampleRate(const TArrayView<const uint8> &Message)
{
    UE_LOGFMT(LogBS_TfliteManager, Verbose, "Parsing SampleRate...");

    uint16 RawSampleRate = BS_ByteParser::ParseAs<uint16>(Message, 0, true);
    UE_LOGFMT(LogBS_TfliteManager, Verbose, "RawSampleRate: {0}ms", RawSampleRate);

    SampleRate = UBS_SensorConfiguration::GetClosestSensorRate(RawSampleRate);
    RawSampleRate = UBS_SensorConfiguration::GetRawSensorRate(SampleRate);
    UE_LOGFMT(LogBS_TfliteManager, Verbose, "SampleRate: {0}, RawSampleRate: {1}ms", UEnum::GetValueAsString(SampleRate), RawSampleRate);

    OnSampleRateUpdate.ExecuteIfBound(SampleRate);
}

void UBS_TfliteManager::SetSampleRate(const EBS_SensorRate NewSampleRate, bool bSendImmediately)
{
    if (NewSampleRate == SampleRate)
    {
        UE_LOGFMT(LogBS_TfliteManager, Verbose, "Redundant SampleRate - not setting");
        return;
    }
    const uint16 NewRawSampleRate = UBS_SensorConfiguration::GetRawSensorRate(NewSampleRate);
    UE_LOGFMT(LogBS_TfliteManager, Verbose, "Updating SampleRate to {0}, NewRawSampleRate: {1}ms", UEnum::GetValueAsString(NewSampleRate), NewRawSampleRate);
    const TArray<uint8> TxMessage = BS_ByteParser::ToByteArray(NewRawSampleRate, true);
    SendTxMessages.ExecuteIfBound({{EBS_TxRxMessage::SET_TFLITE_SAMPLE_RATE, TxMessage}}, bSendImmediately);
}
// SAMPLE RATE END

// SENSOR TYPES START
bool UBS_TfliteManager::IsSensorTypeValid(const EBS_SensorType SensorType)
{
    switch (SensorType)
    {
    case EBS_SensorType::PRESSURE:
    case EBS_SensorType::LINEAR_ACCELERATION:
    case EBS_SensorType::GYROSCOPE:
    case EBS_SensorType::MAGNETOMETER:
        return true;
    default:
        return false;
    }
}
void UBS_TfliteManager::ParseSensorTypes(const TArrayView<const uint8> &Message)
{
    UE_LOGFMT(LogBS_TfliteManager, Verbose, "Parsing SensorTypes...");

    TArray<EBS_SensorType> NewSensorTypes;
    for (const uint8 SensorTypeEnum : Message)
    {
        if (SensorTypeEnum >= static_cast<uint8>(EBS_SensorType::COUNT))
        {
            UE_LOGFMT(LogBS_TfliteManager, Error, "invalid SensorTypeEnum {0}", SensorTypeEnum);
            continue;
        }
        const EBS_SensorType SensorType = static_cast<EBS_SensorType>(SensorTypeEnum);

        UE_LOGFMT(LogBS_TfliteManager, Verbose, "Adding SensorType {0}", UEnum::GetValueAsString(SensorType));
        NewSensorTypes.Add(SensorType);
    }

    SensorTypes = NewSensorTypes;
    UE_LOGFMT(LogBS_TfliteManager, Verbose, "Parsed {0} SensorTypes", SensorTypes.Num());

    OnSensorTypesUpdate.ExecuteIfBound(SensorTypes);
}

void UBS_TfliteManager::SetSensorTypes(const TArray<EBS_SensorType> &NewSensorTypes, bool bSendImmediately)
{
    UE_LOGFMT(LogBS_TfliteManager, Verbose, "Updating SensorTypes...");

    TArray<uint8> TxMessage;
    for (const EBS_SensorType SensorType : NewSensorTypes)
    {
        if (!IsSensorTypeValid(SensorType))
        {
            UE_LOGFMT(LogBS_TfliteManager, Error, "invalid SensorType {0}", UEnum::GetValueAsString(SensorType));
            continue;
        }

        UE_LOGFMT(LogBS_TfliteManager, Verbose, "Adding SensorType {0}", UEnum::GetValueAsString(SensorType));
        TxMessage.Add(static_cast<uint8>(SensorType));
    }

    SendTxMessages.ExecuteIfBound({{EBS_TxRxMessage::SET_TFLITE_SENSOR_TYPES, TxMessage}}, bSendImmediately);
}
// SENSOR TYPES END

// IS READY START
void UBS_TfliteManager::ParseIsReady(const TArrayView<const uint8> &Message)
{
    UE_LOGFMT(LogBS_TfliteManager, Verbose, "Parsing IsReady...");
    IsReady = static_cast<bool>(Message[0]);
    UE_LOGFMT(LogBS_TfliteManager, Verbose, "Parsed IsReady: {0}", IsReady);
    OnIsReadyUpdate.ExecuteIfBound(IsReady);
}
// IS READY END

// CAPTURE DELAY START
void UBS_TfliteManager::ParseCaptureDelay(const TArrayView<const uint8> &Message)
{
    UE_LOGFMT(LogBS_TfliteManager, Verbose, "Parsing CaptureDelay...");
    CaptureDelay = BS_ByteParser::ParseAs<uint16>(Message, 0, true);
    UE_LOGFMT(LogBS_TfliteManager, Verbose, "Parsed CaptureDelay: {0}", CaptureDelay);
    OnCaptureDelayUpdate.ExecuteIfBound(CaptureDelay);
}

void UBS_TfliteManager::SetCaptureDelay(const uint16 NewCaptureDelay, bool bSendImmediately)
{
    if (NewCaptureDelay == CaptureDelay)
    {
        UE_LOGFMT(LogBS_TfliteManager, Verbose, "Redundant CaptureDelay - not setting");
        return;
    }
    UE_LOGFMT(LogBS_TfliteManager, Verbose, "Updating CaptureDelay to {0}", NewCaptureDelay);
    const TArray<uint8> TxMessage = BS_ByteParser::ToByteArray(NewCaptureDelay, true);
    SendTxMessages.ExecuteIfBound({{EBS_TxRxMessage::SET_TFLITE_CAPTURE_DELAY, TxMessage}}, bSendImmediately);
}
// CAPTURE DELAY END

// THRESHOLD START
void UBS_TfliteManager::ParseThreshold(const TArrayView<const uint8> &Message)
{
    UE_LOGFMT(LogBS_TfliteManager, Verbose, "Parsing Threshold...");
    Threshold = BS_ByteParser::ParseAs<float>(Message, 0, true);
    UE_LOGFMT(LogBS_TfliteManager, Verbose, "Parsed Threshold: {0}", Threshold);
    OnThresholdUpdate.ExecuteIfBound(Threshold);
}

void UBS_TfliteManager::SetThreshold(const float NewThreshold, bool bSendImmediately)
{
    if (NewThreshold == Threshold)
    {
        UE_LOGFMT(LogBS_TfliteManager, Verbose, "Redundant Threshold - not setting");
        return;
    }
    UE_LOGFMT(LogBS_TfliteManager, Verbose, "Updating Threshold to {0}", NewThreshold);
    const TArray<uint8> TxMessage = BS_ByteParser::ToByteArray(NewThreshold, true);
    SendTxMessages.ExecuteIfBound({{EBS_TxRxMessage::SET_TFLITE_THRESHOLD, TxMessage}}, bSendImmediately);
}
// THRESHOLD END

// INFERENCING ENABLED START
void UBS_TfliteManager::ParseInferencingEnabled(const TArrayView<const uint8> &Message)
{
    UE_LOGFMT(LogBS_TfliteManager, Verbose, "Parsing InferencingEnabled...");
    InferencingEnabled = static_cast<bool>(Message[0]);
    UE_LOGFMT(LogBS_TfliteManager, Verbose, "Parsed InferencingEnabled: {0}", InferencingEnabled);
    OnInferencingEnabledUpdate.ExecuteIfBound(InferencingEnabled);
}

void UBS_TfliteManager::SetInferencingEnabled(const bool NewInferencingEnabled)
{
    if (NewInferencingEnabled == InferencingEnabled)
    {
        UE_LOGFMT(LogBS_TfliteManager, Verbose, "Redundant NewInferencingEnabled assignment {0} - Skipping", NewInferencingEnabled);
        return;
    }
    if (NewInferencingEnabled && !IsReady)
    {
        UE_LOGFMT(LogBS_TfliteManager, Warning, "Tflite is not ready - Skipping");
        return;
    }
    UE_LOGFMT(LogBS_TfliteManager, Verbose, "Updating InferencingEnabled to {0}", NewInferencingEnabled);
    const TArray<uint8> TxMessage = {static_cast<uint8>(NewInferencingEnabled)};
    SendTxMessages.ExecuteIfBound({{EBS_TxRxMessage::SET_TFLITE_INFERENCING_ENABLED, TxMessage}}, true);
}
// INFERENCING ENABLED END

// INFERENCE START
void UBS_TfliteManager::ParseInference(const TArrayView<const uint8> &Message)
{
    uint8 Offset = 0;
    const uint16 MessageLength = Message.Num();
    UE_LOGFMT(LogBS_TfliteManager, Verbose, "MessageLength: {0}", MessageLength);

    uint64 Timestamp = TimeUtils::ParseTimestamp(Message);
    Offset += 2;
    UE_LOGFMT(LogBS_TfliteManager, Verbose, "Timestamp: {0}ms", Timestamp);

    const uint8 InferenceMessageLength = MessageLength - Offset;
    const uint8 InferenceSize = sizeof(float);
    if (InferenceMessageLength % InferenceSize != 0)
    {
        UE_LOGFMT(LogBS_TfliteManager, Error, "InferenceMessageLength is not a multiple of 4 (got {0})", InferenceMessageLength);
        return;
    }

    TArray<float> Inference;
    uint8 MaxClassIndex = 0;
    float MaxClassValue = -1.0f;
    for (uint8 ClassIndex = 0; Offset < MessageLength; Offset += InferenceSize, ClassIndex++)
    {
        const float ClassValue = BS_ByteParser::ParseAs<float>(Message, Offset, true);
        UE_LOGFMT(LogBS_TfliteManager, Verbose, "Class #{0} Value: {1}", ClassIndex, ClassValue);
        Inference.Add(ClassValue);

        if (Task == EBS_TfliteTask::CLASSIFICATION && ClassValue > MaxClassValue)
        {
            MaxClassValue = ClassValue;
            MaxClassIndex = ClassIndex;
        }
    }

    UE_LOGFMT(LogBS_TfliteManager, Verbose, "Parsed Inference with {0} classes", Inference.Num());
    OnInferenceUpdate.ExecuteIfBound(Inference, Timestamp);

    if (Task == EBS_TfliteTask::CLASSIFICATION)
    {
        UE_LOGFMT(LogBS_TfliteManager, Verbose, "Classification: #{0} with value {1}", MaxClassIndex, MaxClassValue);

        OnClassificationUpdate.ExecuteIfBound(MaxClassIndex, MaxClassValue, Timestamp);

        if (!TfliteConfiguration.Classifications.IsEmpty())
        {
            if (TfliteConfiguration.Classifications.Num() == (Inference.Num() - 1))
            {
                if (TfliteConfiguration.Classifications.IsValidIndex(MaxClassIndex - 1))
                {
                    OnNamedClassificationUpdate.ExecuteIfBound(TfliteConfiguration.Classifications[MaxClassIndex - 1], MaxClassIndex, MaxClassValue, Timestamp);
                }
                else
                {
                    UE_LOGFMT(LogBS_TfliteManager, Error, "Invalid Classification Index {0}", MaxClassIndex);
                }
            }
            else
            {
                UE_LOGFMT(LogBS_TfliteManager, Error, "Expected max {0} Classifications, got {1}", Inference.Num() - 1, TfliteConfiguration.Classifications.Num());
            }
        }
    }
}
// INFERENCE END