// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Containers/Array.h"
#include "BS_UDP_MessageType.h"
#include "BS_ByteParser.h"
#include "Misc/Optional.h"

struct FBS_UDP_Message
{
    EBS_UDP_MessageType Type;
    TOptional<TArray<uint8>> Data;

    FBS_UDP_Message(EBS_UDP_MessageType InType, const TArray<uint8> &InData)
        : Type(InType), Data(InData)
    {
    }

    FBS_UDP_Message(EBS_UDP_MessageType InType)
        : Type(InType)
    {
    }

    uint16 Num() const
    {
        return 3 + DataNum();
    }

    uint16 DataNum() const
    {
        return Data.IsSet() ? Data.GetValue().Num() : 0;
    }

    void AppendTo(TArray<uint8> &Array) const
    {
        Array.Add(static_cast<uint8>(Type));

        uint16 DataLength = DataNum();
        Array.Append(BS_ByteParser::ToByteArray<uint16>(DataLength));
        // Array.Add(DataLength & 0xFF);        // Low byte
        // Array.Add((DataLength >> 8) & 0xFF); // High byte

        if (DataLength > 0)
        {
            Array.Append(Data.GetValue());
        }
    }
};