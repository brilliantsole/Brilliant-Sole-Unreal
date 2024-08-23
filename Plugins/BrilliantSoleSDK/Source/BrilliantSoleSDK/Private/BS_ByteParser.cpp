// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_ByteParser.h"

DEFINE_LOG_CATEGORY(LogByteParser);

FString ByteParser::GetString(const TArray<uint8> &Data)
{
    auto StringLength = Data.Num();
    FString String;
    String.Empty(StringLength);
    for (uint8 i = 0; i < StringLength; i++)
    {
        String.AppendChar(static_cast<TCHAR>(Data[i]));
    }
    return String;
}

TArray<uint8> ByteParser::StringToArray(const FString &String)
{
    TArray<uint8> ByteArray;
    FTCHARToUTF8 UTF8String(*String);
    ByteArray.Append(reinterpret_cast<const uint8 *>(UTF8String.Get()), UTF8String.Length());
    return ByteArray;
}