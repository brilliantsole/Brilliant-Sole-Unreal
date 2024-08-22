// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

class ByteParser
{
public:
    static uint16 GetUint16(const TArray<uint8> &Data, uint8 Offset = 0);
    static int16 GetInt16(const TArray<uint8> &Data, uint8 Offset = 0);

    static uint32 GetUint32(const TArray<uint8> &Data, uint8 Offset = 0);
    static uint64 GetUint64(const TArray<uint8> &Data, uint8 Offset = 0);

    static float GetFloat(const TArray<uint8> &Data, uint8 Offset = 0);
    static double GetDouble(const TArray<uint8> &Data, uint8 Offset = 0);

    static FString GetString(const TArray<uint8> &Data);

    static TArray<uint8> Uint64AsArray(const uint64 &Value);
};