// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "BS_PressureData.h"
#include "Logging/StructuredLog.h"
#include "BS_ByteParser.h"

DEFINE_LOG_CATEGORY(LogBS_PressureData);

void FBS_PressureData::Parse(const TArray<uint8> &Message, const float &Scalar, const TArray<FVector2D> &PressurePositions)
{
    Sensors.Reset(PressurePositions.Num());

    uint8 MessageLength = Message.Num();
    for (uint8 Offset = 0; Offset < MessageLength; Offset += 2)
    {
        uint16 RawPressureValue = BS_ByteParser::ParseAs<uint16>(Message, Offset);
        UE_LOGFMT(LogBS_PressureData, Log, "RawPressureValue #{0}: {1}", Sensors.Num(), RawPressureValue);

        // FILL
    }
}