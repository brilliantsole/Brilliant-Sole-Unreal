// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_ByteParser.h"

DEFINE_LOG_CATEGORY(LogBS_ByteParser);

FString BS_ByteParser::GetString(const TArrayView<const uint8> &Data, bool bIncludesLength)
{
    uint8 Offset = 0;
    uint8 StringLength = Data.Num();
    if (bIncludesLength)
    {
        // StringLength = FMath::Min(Data[Offset++], StringLength);
        StringLength = Data[Offset++];
    }
    UE_LOGFMT(LogBS_ByteParser, Verbose, "Parsing String with length {0}", StringLength);
    FString String;
    String.Empty(StringLength);
    while (String.Len() < StringLength)
    {
        String.AppendChar(static_cast<TCHAR>(Data[Offset++]));
    }
    UE_LOGFMT(LogBS_ByteParser, Verbose, "Parsed String: {0}", String);
    return String;
}

TArray<uint8> BS_ByteParser::StringToArray(const FString &String, bool bIncludeLength)
{
    TArray<uint8> ByteArray;
    FTCHARToUTF8 UTF8String(*String);
    if (bIncludeLength)
    {
        ByteArray.Add(String.Len());
    }
    ByteArray.Append(reinterpret_cast<const uint8 *>(UTF8String.Get()), UTF8String.Length());
    return ByteArray;
}