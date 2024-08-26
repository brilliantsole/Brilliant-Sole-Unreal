// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Logging/StructuredLog.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_ByteParser, Log, All);

class ByteParser
{
public:
    template <typename T>
    static TArray<uint8> ToByteArray(const T &Value)
    {
        TArray<uint8> ByteArray;
        const uint8 Size = sizeof(T);
        ByteArray.SetNumZeroed(Size);
        for (uint8 i = 0; i < Size; i++)
        {
            ByteArray[i] = (Value >> (i * 8)) & 0xFF;
        }
        return ByteArray;
    }

    template <typename T>
    static T ParseAs(const TArray<uint8> &ByteArray, uint8 Offset = 0)
    {
        if (ByteArray.Num() < Offset + sizeof(T))
        {
            UE_LOGFMT(LogBS_ByteParser, Error, "Byte array does not have enough bytes to parse the target type starting from the given Offset.");
            return T(); // Return default value of T if size mismatch
        }

        T ParsedValue;
        FMemory::Memcpy(&ParsedValue, ByteArray.GetData() + Offset, sizeof(T));
        return ParsedValue;
    }

    static FString GetString(const TArray<uint8> &Data);
    static TArray<uint8> StringToArray(const FString &String);
};