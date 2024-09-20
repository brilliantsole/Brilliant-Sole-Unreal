// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Containers/Array.h"
#include "BS_ServerMessageType.h"
#include "BS_ByteParser.h"
#include "Misc/Optional.h"

struct FBS_ServerMessage
{
    EBS_ServerMessageType Type;
    TOptional<TArray<uint8>> Data;

    FBS_ServerMessage(EBS_ServerMessageType InType, const TArray<uint8> &InData)
        : Type(InType), Data(InData)
    {
    }

    FBS_ServerMessage(EBS_ServerMessageType InType)
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
        Array.Append(BS_ByteParser::ToByteArray(DataLength, true));

        if (DataLength > 0)
        {
            Array.Append(Data.GetValue());
        }
    }
};