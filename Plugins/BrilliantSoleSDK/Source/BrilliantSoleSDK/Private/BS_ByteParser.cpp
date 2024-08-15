// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_ByteParser.h"

uint16 ByteParser::GetUint16(const TArray<uint8> &Data, uint8 Offset)
{
    if (Offset >= 0 && Offset + sizeof(uint16) - 1 < Data.Num())
    {
        uint16 ParsedUint16 = (static_cast<uint16>(Data[Offset + 1]) << 8) |
                              (static_cast<uint16>(Data[Offset]));

        return ParsedUint16;
    }
    else
    {
        return 0;
    }
}
int16 ByteParser::GetInt16(const TArray<uint8> &Data, uint8 Offset)
{
    if (Offset >= 0 && Offset + sizeof(int16) - 1 < Data.Num())
    {
        int16 ParsedInt16 = (static_cast<int16>(Data[Offset + 1]) << 8) |
                            (static_cast<int16>(Data[Offset]));

        return ParsedInt16;
    }
    else
    {
        return 0;
    }
}

uint32 ByteParser::GetUint32(const TArray<uint8> &Data, uint8 Offset)
{
    if (Offset >= 0 && Offset + sizeof(uint32) - 1 < Data.Num())
    {
        uint32 ParsedUint32 = (static_cast<uint32>(Data[Offset + 3]) << 24) |
                              (static_cast<uint32>(Data[Offset + 2]) << 16) |
                              (static_cast<uint32>(Data[Offset + 1]) << 8) |
                              (static_cast<uint32>(Data[Offset]));

        return ParsedUint32;
    }
    else
    {
        return 0;
    }
}

float ByteParser::GetFloat(const TArray<uint8> &Data, uint8 Offset)
{
    if (Offset >= 0 && Offset + sizeof(float) - 1 < Data.Num())
    {
        uint32 RawData = 0;
        for (int32 i = 0; i < 4; ++i)
        {
            RawData |= (static_cast<uint32>(Data[Offset + i]) << (i * 8));
        }

        float ParsedFloat;
        FMemory::Memcpy(&ParsedFloat, &RawData, sizeof(float));

        return ParsedFloat;
    }
    else
    {
        return 0.0f;
    }
}
double ByteParser::GetDouble(const TArray<uint8> &Data, uint8 Offset)
{
    if (Offset >= 0 && Offset + sizeof(double) - 1 < Data.Num())
    {
        uint64 RawData = 0;
        for (int32 i = 0; i < 8; ++i)
        {
            RawData |= (static_cast<uint64>(Data[Offset + i]) << (i * 8));
        }

        double ParsedDouble;
        FMemory::Memcpy(&ParsedDouble, &RawData, sizeof(double));

        return ParsedDouble;
    }
    else
    {
        return 0.0;
    }
}