// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_TfliteManager.h"
#include "Logging/StructuredLog.h"
#include "BS_ByteParser.h"
#include "BS_Message.h"
#include "BS_TimeUtils.h"
#include "BS_SensorConfiguration.h"

DEFINE_LOG_CATEGORY(LogBS_TfliteManager);

bool UBS_TfliteManager::OnRxMessage(uint8 MessageType, const TArray<uint8> &Message)
{
    switch (MessageType)
    {
    case BS_MessageTfliteGetName:
    case BS_MessageTfliteSetName:
        ParseName(Message);
        break;
    case BS_MessageTfliteGetTask:
    case BS_MessageTfliteSetTask:
        ParseTask(Message);
        break;
    case BS_MessageTfliteGetSampleRate:
    case BS_MessageTfliteSetSampleRate:
        ParseSampleRate(Message);
        break;
    case BS_MessageTfliteGetSensorTypes:
    case BS_MessageTfliteSetSensorTypes:
        ParseSensorTypes(Message);
        break;
    case BS_MessageTfliteGetIsReady:
        ParseIsReady(Message);
        break;
    case BS_MessageTfliteGetCaptureDelay:
    case BS_MessageTfliteSetCaptureDelay:
        ParseCaptureDelay(Message);
        break;
    case BS_MessageTfliteGetThreshold:
    case BS_MessageTfliteSetThreshold:
        ParseThreshold(Message);
        break;
    case BS_MessageTfliteGetInferencingEnabled:
    case BS_MessageTfliteSetInferencingEnabled:
        ParseInferencingEnabled(Message);
        break;
    case BS_MessageTfliteInference:
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

// NAME START
void UBS_TfliteManager::ParseName(const TArray<uint8> &Message)
{
    UE_LOGFMT(LogBS_TfliteManager, Log, "Parsing Name...");
    Name = BS_ByteParser::GetString(Message);
    UE_LOGFMT(LogBS_TfliteManager, Log, "Parsed Name: {0}", Name);
    OnNameUpdate.ExecuteIfBound(Name);
}

const uint8 UBS_TfliteManager::MinNameLength = 0;
const uint8 UBS_TfliteManager::MaxNameLength = 30;
void UBS_TfliteManager::SetName(const FString &NewName, bool bSendImmediately)
{
    if (NewName == Name)
    {
        UE_LOGFMT(LogBS_TfliteManager, Warning, "Redundant Name - not setting");
        return;
    }
    const auto NewNameLength = NewName.Len();
    if (NewNameLength < MinNameLength || NewNameLength > MaxNameLength)
    {
        UE_LOGFMT(LogBS_TfliteManager, Warning, "Name must be between {0}-{1} characters long (got {2})", MinNameLength, MaxNameLength, NewNameLength);
        return;
    }
    UE_LOGFMT(LogBS_TfliteManager, Log, "Setting Name to {0}...", NewName);

    const TArray<uint8> TxMessage = BS_ByteParser::StringToArray(NewName);
    SendTxMessages.ExecuteIfBound({{BS_MessageTfliteSetName, TxMessage}}, bSendImmediately);
}
// NAME END

// TASK START
void UBS_TfliteManager::ParseTask(const TArray<uint8> &Message)
{
    Task = static_cast<EBS_TfliteTask>(Message[0]);
    UE_LOGFMT(LogBS_TfliteManager, Log, "Parsed Task: {0}", UEnum::GetValueAsString(Task));
    OnTaskUpdate.ExecuteIfBound(Task);
}

void UBS_TfliteManager::SetTask(const EBS_TfliteTask NewTask, bool bSendImmediately)
{
    if (NewTask == Task)
    {
        UE_LOGFMT(LogBS_TfliteManager, Log, "Redundant Task - not setting");
        return;
    }
    UE_LOGFMT(LogBS_TfliteManager, Log, "Setting Task to {0}...", UEnum::GetValueAsString(NewTask));

    const TArray<uint8> TxMessage = {static_cast<uint8>(NewTask)};
    SendTxMessages.ExecuteIfBound({{BS_MessageTfliteSetTask, TxMessage}}, bSendImmediately);
}
// TASK END

// SAMPLE RATE START
void UBS_TfliteManager::ParseSampleRate(const TArray<uint8> &Message)
{
    UE_LOGFMT(LogBS_TfliteManager, Log, "Parsing SampleRate...");

    uint16 RawSampleRate = BS_ByteParser::ParseAs<uint16>(Message, 0, true);
    UE_LOGFMT(LogBS_TfliteManager, Log, "RawSampleRate: {0}ms", RawSampleRate);

    SampleRate = UBS_SensorConfiguration::GetClosestSensorRate(RawSampleRate);
    RawSampleRate = UBS_SensorConfiguration::GetRawSensorRate(SampleRate);
    UE_LOGFMT(LogBS_TfliteManager, Log, "SampleRate: {0}, RawSampleRate: {1}ms", UEnum::GetValueAsString(SampleRate), RawSampleRate);

    OnSampleRateUpdate.ExecuteIfBound(SampleRate);
}

void UBS_TfliteManager::SetSampleRate(const EBS_SensorRate NewSampleRate, bool bSendImmediately)
{
    if (NewSampleRate == SampleRate)
    {
        UE_LOGFMT(LogBS_TfliteManager, Log, "Redundant SampleRate - not setting");
        return;
    }
    const uint16 NewRawSampleRate = UBS_SensorConfiguration::GetRawSensorRate(NewSampleRate);
    UE_LOGFMT(LogBS_TfliteManager, Log, "Updating SampleRate to {0}, NewRawSampleRate: {1}ms", UEnum::GetValueAsString(NewSampleRate), NewRawSampleRate);
    const TArray<uint8> TxMessage = BS_ByteParser::ToByteArray(NewRawSampleRate);
    SendTxMessages.ExecuteIfBound({{BS_MessageTfliteSetSampleRate, TxMessage}}, bSendImmediately);
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
void UBS_TfliteManager::ParseSensorTypes(const TArray<uint8> &Message)
{
    UE_LOGFMT(LogBS_TfliteManager, Log, "Parsing SensorTypes...");

    TArray<EBS_SensorType> NewSensorTypes;
    for (const uint8 SensorTypeEnum : Message)
    {
        if (SensorTypeEnum >= static_cast<uint8>(EBS_SensorType::COUNT))
        {
            UE_LOGFMT(LogBS_TfliteManager, Error, "invalid SensorTypeEnum {0}", SensorTypeEnum);
            continue;
        }
        const EBS_SensorType SensorType = static_cast<EBS_SensorType>(SensorTypeEnum);

        UE_LOGFMT(LogBS_TfliteManager, Log, "Adding SensorType {0}", UEnum::GetValueAsString(SensorType));
        NewSensorTypes.Add(SensorType);
    }

    SensorTypes = NewSensorTypes;
    UE_LOGFMT(LogBS_TfliteManager, Log, "Parsed {0} SensorTypes", SensorTypes.Num());

    OnSensorTypesUpdate.ExecuteIfBound(SensorTypes);
}

void UBS_TfliteManager::SetSensorTypes(const TArray<EBS_SensorType> &NewSensorTypes, bool bSendImmediately)
{
    UE_LOGFMT(LogBS_TfliteManager, Log, "Updating SensorTypes...");

    TArray<uint8> TxMessage;
    for (const EBS_SensorType SensorType : NewSensorTypes)
    {
        if (!IsSensorTypeValid(SensorType))
        {
            UE_LOGFMT(LogBS_TfliteManager, Error, "invalid SensorType {0}", UEnum::GetValueAsString(SensorType));
            continue;
        }

        UE_LOGFMT(LogBS_TfliteManager, Log, "Adding SensorType {0}", UEnum::GetValueAsString(SensorType));
        TxMessage.Add(static_cast<uint8>(SensorType));
    }

    SendTxMessages.ExecuteIfBound({{BS_MessageTfliteSetSensorTypes, TxMessage}}, bSendImmediately);
}
// SENSOR TYPES END

// IS READY START
void UBS_TfliteManager::ParseIsReady(const TArray<uint8> &Message)
{
    UE_LOGFMT(LogBS_TfliteManager, Log, "Parsing IsReady...");
    IsReady = static_cast<bool>(Message[0]);
    UE_LOGFMT(LogBS_TfliteManager, Log, "Parsed IsReady: {0}", IsReady);
    OnIsReadyUpdate.ExecuteIfBound(IsReady);
}
// IS READY END

// CAPTURE DELAY START
void UBS_TfliteManager::ParseCaptureDelay(const TArray<uint8> &Message)
{
    UE_LOGFMT(LogBS_TfliteManager, Log, "Parsing CaptureDelay...");
    CaptureDelay = BS_ByteParser::ParseAs<uint16>(Message, 0, true);
    UE_LOGFMT(LogBS_TfliteManager, Log, "Parsed CaptureDelay: {0}", CaptureDelay);
    OnCaptureDelayUpdate.ExecuteIfBound(CaptureDelay);
}

void UBS_TfliteManager::SetCaptureDelay(const uint16 NewCaptureDelay, bool bSendImmediately)
{
    if (NewCaptureDelay == CaptureDelay)
    {
        UE_LOGFMT(LogBS_TfliteManager, Log, "Redundant CaptureDelay - not setting");
        return;
    }
    UE_LOGFMT(LogBS_TfliteManager, Log, "Updating CaptureDelay to {0}", NewCaptureDelay);
    const TArray<uint8> TxMessage = BS_ByteParser::ToByteArray(NewCaptureDelay);
    SendTxMessages.ExecuteIfBound({{BS_MessageTfliteSetCaptureDelay, TxMessage}}, bSendImmediately);
}
// CAPTURE DELAY END

// THRESHOLD START
void UBS_TfliteManager::ParseThreshold(const TArray<uint8> &Message)
{
    UE_LOGFMT(LogBS_TfliteManager, Log, "Parsing Threshold...");
    Threshold = BS_ByteParser::ParseAs<float>(Message, 0, true);
    UE_LOGFMT(LogBS_TfliteManager, Log, "Parsed Threshold: {0}", Threshold);
    OnThresholdUpdate.ExecuteIfBound(Threshold);
}

void UBS_TfliteManager::SetThreshold(const float NewThreshold, bool bSendImmediately)
{
    if (NewThreshold == Threshold)
    {
        UE_LOGFMT(LogBS_TfliteManager, Log, "Redundant Threshold - not setting");
        return;
    }
    UE_LOGFMT(LogBS_TfliteManager, Log, "Updating Threshold to {0}", NewThreshold);
    const TArray<uint8> TxMessage = BS_ByteParser::ToByteArray(NewThreshold);
    SendTxMessages.ExecuteIfBound({{BS_MessageTfliteSetThreshold, TxMessage}}, bSendImmediately);
}
// THRESHOLD END

// INFERENCING ENABLED START
void UBS_TfliteManager::ParseInferencingEnabled(const TArray<uint8> &Message)
{
    UE_LOGFMT(LogBS_TfliteManager, Log, "Parsing InferencingEnabled...");
    InferencingEnabled = static_cast<bool>(Message[0]);
    UE_LOGFMT(LogBS_TfliteManager, Log, "Parsed InferencingEnabled: {0}", InferencingEnabled);
    OnInferencingEnabledUpdate.ExecuteIfBound(InferencingEnabled);
}

void UBS_TfliteManager::SetInferencingEnabled(const bool NewInferencingEnabled)
{
    if (NewInferencingEnabled == InferencingEnabled)
    {
        UE_LOGFMT(LogBS_TfliteManager, Log, "Redundant NewInferencingEnabled assignment {0} - Skipping", NewInferencingEnabled);
        return;
    }
    if (NewInferencingEnabled && !IsReady)
    {
        UE_LOGFMT(LogBS_TfliteManager, Warning, "Tflite is not ready - Skipping");
        return;
    }
    UE_LOGFMT(LogBS_TfliteManager, Log, "Updating InferencingEnabled to {0}", NewInferencingEnabled);
    const TArray<uint8> TxMessage = {static_cast<uint8>(NewInferencingEnabled)};
    SendTxMessages.ExecuteIfBound({{BS_MessageTfliteSetInferencingEnabled, TxMessage}}, true);
}
// INFERENCING ENABLED END

// INFERENCE START
void UBS_TfliteManager::ParseInference(const TArray<uint8> &Message)
{
    uint8 Offset = 0;
    const uint16 MessageLength = Message.Num();
    UE_LOGFMT(LogBS_TfliteManager, Log, "MessageLength: {0}", MessageLength);

    uint64 Timestamp = TimeUtils::ParseTimestamp(Message);
    Offset += 2;
    UE_LOGFMT(LogBS_TfliteManager, Log, "Timestamp: {0}ms", Timestamp);

    const uint8 InferenceMessageLegth = MessageLength - Offset;
    const uint8 InferenceSize = sizeof(float);
    if (InferenceMessageLegth % InferenceSize != 0)
    {
        UE_LOGFMT(LogBS_TfliteManager, Error, "Message is not a multiple of 4 (got {0})", InferenceMessageLegth);
        return;
    }

    TArray<float> Inference;
    for (uint8 ClassIndex = 0; Offset < MessageLength; Offset += InferenceSize, ClassIndex++)
    {
        const float ClassValue = BS_ByteParser::ParseAs<float>(Message, Offset, true);
        UE_LOGFMT(LogBS_TfliteManager, Log, "Class #{0} Value: {1}", ClassIndex, ClassValue);
        Inference.Add(ClassValue);
    }

    UE_LOGFMT(LogBS_TfliteManager, Log, "Parsed Inference with {0} classes", Inference.Num());
    OnInferenceUpdate.ExecuteIfBound(Inference, Timestamp);
}
// INFERENCE END