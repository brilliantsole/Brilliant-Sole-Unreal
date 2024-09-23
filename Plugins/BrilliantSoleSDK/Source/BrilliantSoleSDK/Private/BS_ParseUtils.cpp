// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_ParseUtils.h"
#include "BS_ByteParser.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(LogBS_ParseUtils);

void UBS_ParseUtils::ParseRxData(const TArray<uint8> &Data, FBS_MessageCallback MessageCallback)
{
    const auto DataLength = Data.Num();
    uint16 Offset = 0;

    UE_LOGFMT(LogBS_ParseUtils, Verbose, "Parsing {0} bytes...", DataLength);

    while (Offset < DataLength)
    {
        UE_LOGFMT(LogBS_ParseUtils, Verbose, "Parsing Message at {0}...", Offset);

        const uint8 MessageType = Data[Offset++];
        const uint16 MessageDataLength = BS_ByteParser::ParseAs<uint16>(Data, Offset, true);
        Offset += 2;

        UE_LOGFMT(LogBS_ParseUtils, Verbose, "Message {0} ({1} bytes)", MessageType, MessageDataLength);

        const TArrayView<uint8> MessageData((uint8 *)(Data.GetData() + Offset), MessageDataLength);
        MessageCallback.ExecuteIfBound(MessageType, static_cast<TArray<uint8>>(MessageData));

        Offset += MessageDataLength;
        UE_LOGFMT(LogBS_ParseUtils, Verbose, "New Offset: {0}/{1}", Offset, DataLength);
    }
}

void UBS_ParseUtils::ParseUDP_Data(const TArray<uint8> &Data, FBS_UDP_MessageCallback MessageCallback)
{
    const auto DataLength = Data.Num();
    uint16 Offset = 0;

    UE_LOGFMT(LogBS_ParseUtils, Verbose, "Parsing {0} bytes...", DataLength);

    while (Offset < DataLength)
    {
        UE_LOGFMT(LogBS_ParseUtils, Verbose, "Parsing Message at {0}...", Offset);

        const uint8 MessageTypeEnum = Data[Offset++];
        if (MessageTypeEnum >= static_cast<uint8>(EBS_UDP_MessageType::COUNT))
        {
            UE_LOGFMT(LogBS_ParseUtils, Error, "invalid MessageTypeEnum {0}", MessageTypeEnum);
            continue;
        }
        const EBS_UDP_MessageType MessageType = static_cast<EBS_UDP_MessageType>(MessageTypeEnum);

        const uint16 MessageDataLength = BS_ByteParser::ParseAs<uint16>(Data, Offset, true);
        Offset += 2;

        UE_LOGFMT(LogBS_ParseUtils, Verbose, "Message {0} ({1} bytes)", UEnum::GetValueAsString(MessageType), MessageDataLength);

        const TArrayView<uint8> MessageData((uint8 *)(Data.GetData() + Offset), MessageDataLength);
        MessageCallback.ExecuteIfBound(MessageType, static_cast<TArray<uint8>>(MessageData));

        Offset += MessageDataLength;
        UE_LOGFMT(LogBS_ParseUtils, Verbose, "New Offset: {0}/{1}", Offset, DataLength);
    }
}

void UBS_ParseUtils::ParseServerData(const TArray<uint8> &Data, FBS_ServerMessageCallback MessageCallback)
{
    const auto DataLength = Data.Num();
    uint16 Offset = 0;

    UE_LOGFMT(LogBS_ParseUtils, Verbose, "Parsing {0} bytes...", DataLength);

    while (Offset < DataLength)
    {
        UE_LOGFMT(LogBS_ParseUtils, Verbose, "Parsing Message at {0}...", Offset);

        const uint8 MessageTypeEnum = Data[Offset++];
        if (MessageTypeEnum >= static_cast<uint8>(EBS_ServerMessage::COUNT))
        {
            UE_LOGFMT(LogBS_ParseUtils, Error, "invalid ServerEventEnum {0}", MessageTypeEnum);
            continue;
        }
        const EBS_ServerMessage MessageType = static_cast<EBS_ServerMessage>(MessageTypeEnum);

        const uint16 MessageDataLength = BS_ByteParser::ParseAs<uint16>(Data, Offset, true);
        Offset += 2;

        UE_LOGFMT(LogBS_ParseUtils, Verbose, "Message {0} ({1} bytes)", UEnum::GetValueAsString(MessageType), MessageDataLength);

        const TArrayView<uint8> MessageData((uint8 *)(Data.GetData() + Offset), MessageDataLength);
        MessageCallback.ExecuteIfBound(MessageType, static_cast<TArray<uint8>>(MessageData));

        Offset += MessageDataLength;
        UE_LOGFMT(LogBS_ParseUtils, Verbose, "New Offset: {0}/{1}", Offset, DataLength);
    }
}

void UBS_ParseUtils::ParseDeviceEventData(UBS_Device *Device, const TArray<uint8> &Data, FBS_DeviceEventCallback MessageCallback)
{
    const auto DataLength = Data.Num();
    uint16 Offset = 0;

    UE_LOGFMT(LogBS_ParseUtils, Verbose, "Parsing {0} bytes...", DataLength);

    while (Offset < DataLength)
    {
        UE_LOGFMT(LogBS_ParseUtils, Verbose, "Parsing Message at {0}...", Offset);

        const uint8 MessageTypeEnum = Data[Offset++];
        if (MessageTypeEnum >= static_cast<uint8>(EBS_DeviceEvent::COUNT))
        {
            UE_LOGFMT(LogBS_ParseUtils, Error, "invalid DeviceEventEnum {0}", MessageTypeEnum);
            continue;
        }
        const EBS_DeviceEvent MessageType = static_cast<EBS_DeviceEvent>(MessageTypeEnum);

        const uint16 MessageDataLength = BS_ByteParser::ParseAs<uint16>(Data, Offset, true);
        Offset += 2;

        UE_LOGFMT(LogBS_ParseUtils, Verbose, "Message {0} ({1} bytes)", static_cast<uint8>(MessageType), MessageDataLength);

        const TArrayView<uint8> MessageData((uint8 *)(Data.GetData() + Offset), MessageDataLength);
        MessageCallback.ExecuteIfBound(Device, MessageType, static_cast<TArray<uint8>>(MessageData));

        Offset += MessageDataLength;
        UE_LOGFMT(LogBS_ParseUtils, Verbose, "New Offset: {0}/{1}", Offset, DataLength);
    }
}