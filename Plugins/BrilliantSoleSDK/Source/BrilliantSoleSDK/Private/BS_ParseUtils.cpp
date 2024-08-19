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
        const uint8 MessageDataLength = ByteParser::GetUint16(Data, Offset);
        Offset += 2;

        UE_LOG(LogBS_ParseUtils, Log, TEXT("Message #%u (%u bytes)"), MessageType, MessageDataLength);

        TArrayView<uint8> MessageData((uint8 *)(Data.GetData() + Offset), MessageDataLength);
        MessageCallback.ExecuteIfBound(MessageType, (TArray<uint8>)MessageData);

        Offset += MessageDataLength;
    }
}