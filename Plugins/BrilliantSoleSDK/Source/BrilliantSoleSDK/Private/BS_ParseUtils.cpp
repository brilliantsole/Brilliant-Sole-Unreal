// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_ParseUtils.h"
#include "BS_ByteParser.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(LogBS_ParseUtils);

void UBS_ParseUtils::ParseRxData(const TArray<uint8> &Data, FMessageCallback MessageCallback)
{
    const auto DataLength = Data.Num();
    uint8 Offset = 0;
    while (Offset < DataLength)
    {
        const uint8 MessageType = Data[Offset++];
        const uint8 MessageDataLength = BS_ByteParser::ParseAs<uint16>(Data, Offset, true);
        Offset += 2;

        UE_LOGFMT(LogBS_ParseUtils, Log, "Message #{0} ({1} bytes)", MessageType, MessageDataLength);

        const TArrayView<uint8> MessageData((uint8 *)(Data.GetData() + Offset), MessageDataLength);
        MessageCallback.ExecuteIfBound(MessageType, static_cast<TArray<uint8>>(MessageData));

        Offset += MessageDataLength;
    }
}