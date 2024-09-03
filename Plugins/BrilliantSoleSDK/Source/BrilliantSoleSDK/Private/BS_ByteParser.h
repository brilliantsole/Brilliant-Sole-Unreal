// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Logging/StructuredLog.h"
#include "Algo/Reverse.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_ByteParser, Log, All);

class BS_ByteParser
{
public:
    template <typename T>
    static TArray<uint8> ToByteArray(const T &Value, bool bIsLittleEndian = false)
    {
        TArray<uint8> ByteArray;
        const uint8 Size = sizeof(T);
        ByteArray.SetNumZeroed(Size);

        // for (uint8 i = 0; i < Size; i++)
        // {
        //     ByteArray[i] = (Value >> (i * 8)) & 0xFF;
        // }

        FMemory::Memcpy(ByteArray.GetData(), &Value, Size);
        if (bIsLittleEndian && !PLATFORM_LITTLE_ENDIAN)
        {
            Algo::Reverse(ByteArray);
        }

        return ByteArray;
    }

    template <typename T>
    static T ParseAs(const TArray<uint8> &ByteArray, uint8 Offset = 0, bool bIsLittleEndian = false)
    {
        if (ByteArray.Num() < Offset + sizeof(T))
        {
            UE_LOGFMT(LogBS_ByteParser, Error, "Byte array does not have enough bytes to parse the target type starting from the given Offset.");
            return T(); // Return default value of T if size mismatch
        }

        T ParsedValue;
        FMemory::Memcpy(&ParsedValue, ByteArray.GetData() + Offset, sizeof(T));
        UE_LOGFMT(LogBS_ByteParser, Log, "ParsedValue: {0}", ParsedValue);
        if (!bIsLittleEndian)
        {
            ParsedValue = ByteSwap(ParsedValue);
            UE_LOGFMT(LogBS_ByteParser, Log, "Swapped ParsedValue: {0}", ParsedValue);
        }

        return ParsedValue;
    }

    static FString GetString(const TArray<uint8> &Data);
    static TArray<uint8> StringToArray(const FString &String);
};