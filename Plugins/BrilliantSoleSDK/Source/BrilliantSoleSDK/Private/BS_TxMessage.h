// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Containers/Array.h"
#include "BS_TxRxMessageType.h"
#include "BS_ByteParser.h"
#include "Misc/Optional.h"

struct FBS_TxMessage
{
    EBS_TxRxMessage Type;
    TOptional<TArray<uint8>> Data;

    FBS_TxMessage(EBS_TxRxMessage InType, const TArray<uint8> &InData)
        : Type(InType), Data(InData)
    {
    }

    FBS_TxMessage(EBS_TxRxMessage InType)
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