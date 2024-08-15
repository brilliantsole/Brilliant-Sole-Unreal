// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

class ByteParser
{
public:
    static uint16 GetUint16(const TArray<uint8> &Data, uint8 Offset);
    static int16 GetInt16(const TArray<uint8> &Data, uint8 Offset);

    static uint32 GetUint32(const TArray<uint8> &Data, uint8 Offset);

    static float GetFloat(const TArray<uint8> &Data, uint8 Offset);
    static double GetDouble(const TArray<uint8> &Data, uint8 Offset);
};