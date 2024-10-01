// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_InformationManager.h"
#include "Logging/StructuredLog.h"
#include "BS_ByteParser.h"
#include "BS_TxRxMessageType.h"
#include "BS_TimeUtils.h"

DEFINE_LOG_CATEGORY(LogBS_InformationManager);

bool UBS_InformationManager::OnRxMessage(EBS_TxRxMessage MessageType, const TArrayView<const uint8> &Message)
{
    switch (MessageType)
    {
    case EBS_TxRxMessage::GET_MTU:
        ParseMTU(Message);
        break;
    case EBS_TxRxMessage::GET_ID:
        ParseId(Message);
        break;
    case EBS_TxRxMessage::GET_NAME:
    case EBS_TxRxMessage::SET_NAME:
        ParseName(Message);
        break;
    case EBS_TxRxMessage::GET_TYPE:
    case EBS_TxRxMessage::SET_TYPE:
        ParseType(Message);
        break;
    case EBS_TxRxMessage::GET_CURRENT_TIME:
    case EBS_TxRxMessage::SET_CURRENT_TIME:
        ParseCurrentTime(Message);
        break;
    default:
        return false;
        break;
    }

    return true;
}

void UBS_InformationManager::Reset()
{
    MTU = 0;
    Id = "";
    Name = "";
    Type = EBS_DeviceType::LEFT_INSOLE;
    CurrentTime = 0;
}

// MTU START
void UBS_InformationManager::ParseMTU(const TArrayView<const uint8> &Message)
{
    UE_LOGFMT(LogBS_InformationManager, Verbose, "Parsing MTU...");
    MTU = BS_ByteParser::ParseAs<uint16>(Message, 0, true);
    UE_LOGFMT(LogBS_InformationManager, Verbose, "Parsed MTU: {0}", MTU);
    OnMTU_Update.ExecuteIfBound(MTU);
}
// MTU END

// ID START
void UBS_InformationManager::ParseId(const TArrayView<const uint8> &Message)
{
    UE_LOGFMT(LogBS_InformationManager, Verbose, "Parsing Id...");
    Id = BS_ByteParser::GetString(Message);
    UE_LOGFMT(LogBS_InformationManager, Verbose, "Parsed Id: \"{0}\" ({1} bytes)", Id, Message.Num());
    OnIdUpdate.ExecuteIfBound(Id);
}
// ID END

// NAME START
void UBS_InformationManager::ParseName(const TArrayView<const uint8> &Message)
{
    UE_LOGFMT(LogBS_InformationManager, Verbose, "Parsing Name...");
    Name = BS_ByteParser::GetString(Message);
    UE_LOGFMT(LogBS_InformationManager, Verbose, "Parsed Name: \"{0}\"", Name);
    OnNameUpdate.ExecuteIfBound(Name);
}

const uint8 UBS_InformationManager::MinNameLength = 2;
const uint8 UBS_InformationManager::MaxNameLength = 30;
void UBS_InformationManager::SetName(const FString &NewName)
{
    if (NewName == Name)
    {
        UE_LOGFMT(LogBS_InformationManager, Warning, "Redundant Name - not setting");
        return;
    }
    const auto NewNameLength = NewName.Len();
    if (NewNameLength < MinNameLength || NewNameLength > MaxNameLength)
    {
        UE_LOGFMT(LogBS_InformationManager, Warning, "Name must be between {0}-{1} characters long (got {2})", MinNameLength, MaxNameLength, NewNameLength);
        return;
    }
    UE_LOGFMT(LogBS_InformationManager, Verbose, "Setting Name to \"{0}\"...", NewName);

    const TArray<uint8> TxMessage = BS_ByteParser::StringToArray(NewName);
    SendTxMessages.ExecuteIfBound({{EBS_TxRxMessage::SET_NAME, TxMessage}}, true);
}
// NAME END

// TYPE START
void UBS_InformationManager::ParseType(const TArrayView<const uint8> &Message)
{
    Type = static_cast<EBS_DeviceType>(Message[0]);
    UE_LOGFMT(LogBS_InformationManager, Verbose, "Parsed Type: {0}", UEnum::GetValueAsString(Type));
    OnTypeUpdate.ExecuteIfBound(Type);
}

void UBS_InformationManager::SetType(const EBS_DeviceType NewType)
{
    if (NewType == Type)
    {
        UE_LOGFMT(LogBS_InformationManager, Verbose, "Redundant Type - not setting");
        return;
    }
    UE_LOGFMT(LogBS_InformationManager, Verbose, "Setting Type to {0}...", UEnum::GetValueAsString(NewType));

    const TArray<uint8> TxMessage = {static_cast<uint8>(NewType)};
    SendTxMessages.ExecuteIfBound({{EBS_TxRxMessage::SET_TYPE, TxMessage}}, true);
}
// TYPE END

// CURRENT TIME START
void UBS_InformationManager::ParseCurrentTime(const TArrayView<const uint8> &Message)
{
    CurrentTime = BS_ByteParser::ParseAs<uint64>(Message, 0, true);
    UE_LOGFMT(LogBS_InformationManager, Verbose, "Parsed CurrentTime: {0}", CurrentTime);
    if (CurrentTime == 0)
    {
        UpdateCurrentTime();
    }
    else
    {
        OnCurrentTimeUpdate.ExecuteIfBound(CurrentTime);
    }
}

void UBS_InformationManager::UpdateCurrentTime()
{
    uint64 Milliseconds = TimeUtils::GetMilliseconds();
    UE_LOGFMT(LogBS_InformationManager, Verbose, "Updating CurrentTime to {0}", Milliseconds);
    const TArray<uint8> TxMessage = BS_ByteParser::ToByteArray(Milliseconds);
    SendTxMessages.ExecuteIfBound({{EBS_TxRxMessage::SET_CURRENT_TIME, TxMessage}}, false);
}
// CURRENT TIME END