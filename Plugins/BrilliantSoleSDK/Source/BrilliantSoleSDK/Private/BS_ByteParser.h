// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Logging/StructuredLog.h"
#include "Algo/Reverse.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_ByteParser, Verbose, All);

class BS_ByteParser
{
public:
    template <typename T>
    static TArray<uint8> ToByteArray(const T &Value, bool bIsLittleEndian = false)
    {
        TArray<uint8> ByteArray;
        const uint8 Size = sizeof(T);
        ByteArray.SetNumZeroed(Size);

        FMemory::Memcpy(ByteArray.GetData(), &Value, Size);
        // UE_LOGFMT(LogBS_ByteParser, Verbose, "bIsLittleEndian {0} PLATFORM_LITTLE_ENDIAN {1}", bIsLittleEndian, PLATFORM_LITTLE_ENDIAN);
        if (bIsLittleEndian != PLATFORM_LITTLE_ENDIAN)
        {
            UE_LOGFMT(LogBS_ByteParser, Verbose, "Reversing bytes...");
            Algo::Reverse(ByteArray);
        }

        return ByteArray;
    }

    template <typename T>
    static T ParseAs(const TArray<uint8> &ByteArray, uint16 Offset = 0, bool bIsLittleEndian = false)
    {
        if (ByteArray.Num() < Offset + sizeof(T))
        {
            UE_LOGFMT(LogBS_ByteParser, Error, "Byte array does not have enough bytes to parse the target type starting from the given Offset.");
            return T(); // Return default value of T if size mismatch
        }

        T ParsedValue;
        FMemory::Memcpy(&ParsedValue, ByteArray.GetData() + Offset, sizeof(T));
        UE_LOGFMT(LogBS_ByteParser, Verbose, "ParsedValue: {0}", ParsedValue);
        // UE_LOGFMT(LogBS_ByteParser, Verbose, "bIsLittleEndian: {0} PLATFORM_LITTLE_ENDIAN: {1}", bIsLittleEndian, PLATFORM_LITTLE_ENDIAN);
        if (bIsLittleEndian != PLATFORM_LITTLE_ENDIAN)
        {
            ParsedValue = ByteSwap(ParsedValue);
            UE_LOGFMT(LogBS_ByteParser, Verbose, "Swapped ParsedValue: {0}", ParsedValue);
        }

        return ParsedValue;
    }

    static FString GetString(const TArray<uint8> &ByteArray, bool bIncludesLength = false);
    static TArray<uint8> StringToArray(const FString &String, bool bIncludeLength = false);
};