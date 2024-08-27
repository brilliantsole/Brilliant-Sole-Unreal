// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved
// https://www.ti.com/lit/ds/symlink/drv2605.pdf section 11.2

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBS_VibrationWaveformEffect : uint8
{
    NONE = 0 UMETA(DisplayName = "None"),

    StrongClick_100 = 1 UMETA(DisplayName = "Strong Click - 100%"),
    StrongClick_60 = 2 UMETA(DisplayName = "Strong Click - 60%"),
    StrongClick_30 = 3 UMETA(DisplayName = "Strong Click - 30%"),

    SharpClick_100 = 4 UMETA(DisplayName = "Sharp Click - 100%"),
    SharpClick_60 = 5 UMETA(DisplayName = "Sharp Click - 60%"),
    SharpClick_30 = 6 UMETA(DisplayName = "Sharp Click - 30%"),

    SoftBump_100 = 7 UMETA(DisplayName = "Soft Bump - 100%"),
    SoftBump_60 = 8 UMETA(DisplayName = "Soft Bump - 60%"),
    SoftBump_30 = 9 UMETA(DisplayName = "Soft Bump - 30%"),

    DoubleClick_100 = 10 UMETA(DisplayName = "Double Click - 100%"),
    DoubleClick_60 = 11 UMETA(DisplayName = "Double Click - 60%"),

    TripleClick_100 = 12 UMETA(DisplayName = "Triple Click - 100%"),

    SoftFuzz_60 = 13 UMETA(DisplayName = "Soft Fuzz - 60%"),

    StrongBuzz_100 = 14 UMETA(DisplayName = "Strong Buzz - 100%"),

    Alert_750ms = 15 UMETA(DisplayName = "750 ms Alert - 100%"),
    Alert_1000ms = 16 UMETA(DisplayName = "1000 ms Alert - 100%"),

    StrongClick1_100 = 17 UMETA(DisplayName = "Strong Click 1 100%"),
    StrongClick2_80 = 18 UMETA(DisplayName = "Strong Click 2 - 80%"),
    StrongClick3_60 = 19 UMETA(DisplayName = "Strong Click 3 - 60%"),
    StrongClick4_30 = 20 UMETA(DisplayName = "Strong Click 4 - 30%"),

    MediumClick_100 = 21 UMETA(DisplayName = "Medium Click 1 - 100%"),
    MediumClick_80 = 22 UMETA(DisplayName = "Medium Click 2 - 80%"),
    MediumClick_60 = 23 UMETA(DisplayName = "Medium Click 3 - 60%"),

    SharpTick_100 = 24 UMETA(DisplayName = "Sharp Tick 1 - 100%"),
    SharpTick_80 = 25 UMETA(DisplayName = "Sharp Tick 2 - 80%"),
    SharpTick_60 = 26 UMETA(DisplayName = "Sharp Tick 3 - 60%"),

    ShortDoubleClickStrong_100 = 27 UMETA(DisplayName = "Short Double Click Strong 1 - 100%"),
    ShortDoubleClickStrong_80 = 28 UMETA(DisplayName = "Short Double Click Strong 2 - 80%"),
    ShortDoubleClickStrong_60 = 29 UMETA(DisplayName = "Short Double Click Strong 3 - 60%"),
    ShortDoubleClickStrong_30 = 30 UMETA(DisplayName = "Short Double Click Strong 4 - 30%"),

    ShortDoubleClickMedium_100 = 31 UMETA(DisplayName = "Short Double Click Medium 1 - 100%"),
    ShortDoubleClickMedium_80 = 32 UMETA(DisplayName = "Short Double Click Medium 2 - 80%"),
    ShortDoubleClickMedium_60 = 33 UMETA(DisplayName = "Short Double Click Medium 3 - 60%"),

    ShortDoubleSharpTick_100 = 34 UMETA(DisplayName = "Short Double Sharp Tick 1 - 100%"),
    ShortDoubleSharpTick_80 = 35 UMETA(DisplayName = "Short Double Sharp Tick 2 - 80%"),
    ShortDoubleSharpTick_60 = 36 UMETA(DisplayName = "Short Double Sharp Tick 3 - 60%"),

    LongDoubleSharpClickStrong_100 = 37 UMETA(DisplayName = "Long Double Sharp Click Strong 1 - 100%"),
    LongDoubleSharpClickStrong_80 = 38 UMETA(DisplayName = "Long Double Sharp Click Strong 2 - 80%"),
    LongDoubleSharpClickStrong_60 = 39 UMETA(DisplayName = "Long Double Sharp Click Strong 3 - 60%"),
    LongDoubleSharpClickStrong_30 = 40 UMETA(DisplayName = "Long Double Sharp Click Strong 4 - 30%"),

    LongDoubleSharpClickMedium_100 = 41 UMETA(DisplayName = "Long Double Sharp Click Medium 1 - 100%"),
    LongDoubleSharpClickMedium_80 = 42 UMETA(DisplayName = "Long Double Sharp Click Medium 2 - 80%"),
    LongDoubleSharpClickMedium_60 = 43 UMETA(DisplayName = "Long Double Sharp Click Medium 3 - 60%"),

    LongDoubleSharpTick_100 = 44 UMETA(DisplayName = "Long Double Sharp Tick 1 - 100%"),
    LongDoubleSharpTick_80 = 45 UMETA(DisplayName = "Long Double Sharp Tick 2 - 80%"),
    LongDoubleSharpTick_60 = 46 UMETA(DisplayName = "Long Double Sharp Tick 3 - 60%"),

    Buzz_100 = 47 UMETA(DisplayName = "Buzz 1 - 100%"),
    Buzz_80 = 48 UMETA(DisplayName = "Buzz 2 - 80%"),
    Buzz_60 = 49 UMETA(DisplayName = "Buzz 3 - 60%"),
    Buzz_40 = 50 UMETA(DisplayName = "Buzz 4 - 40%"),
    Buzz_20 = 51 UMETA(DisplayName = "Buzz 5 - 20%"),

    PulsingStrong_100 = 52 UMETA(DisplayName = "Pulsing Strong 1 - 100%"),
    PulsingStrong_60 = 53 UMETA(DisplayName = "Pulsing Strong 2 - 60%"),

    PulsingMedium_100 = 54 UMETA(DisplayName = "Pulsing Medium 1 - 100%"),
    PulsingMedium_60 = 55 UMETA(DisplayName = "Pulsing Medium 2 - 60%"),

    PulsingSharp_100 = 56 UMETA(DisplayName = "Pulsing Sharp 1 - 100%"),
    PulsingSharp_60 = 57 UMETA(DisplayName = "Pulsing Sharp 2 - 60%"),

    TransitionClick_100 = 58 UMETA(DisplayName = "Transition Click 1 - 100%"),
    TransitionClick_80 = 59 UMETA(DisplayName = "Transition Click 2 - 80%"),
    TransitionClick_60 = 60 UMETA(DisplayName = "Transition Click 3 - 60%"),
    TransitionClick_40 = 61 UMETA(DisplayName = "Transition Click 4 - 40%"),
    TransitionClick_20 = 62 UMETA(DisplayName = "Transition Click 5 - 20%"),
    TransitionClick_10 = 63 UMETA(DisplayName = "Transition Click 6 - 10%"),

    TransitionHum_100 = 64 UMETA(DisplayName = "Transition Hum 1 - 100%"),
    TransitionHum_80 = 65 UMETA(DisplayName = "Transition Hum 2 - 80%"),
    TransitionHum_60 = 66 UMETA(DisplayName = "Transition Hum 3 - 60%"),
    TransitionHum_40 = 67 UMETA(DisplayName = "Transition Hum 4 - 40%"),
    TransitionHum_20 = 68 UMETA(DisplayName = "Transition Hum 5 - 20%"),
    TransitionHum_10 = 69 UMETA(DisplayName = "Transition Hum 6 - 10%"),

    TransitionRampDownLongSmooth1_100 = 70 UMETA(DisplayName = "Transition Ramp Down Long Smooth 1 - 100 to 0%"),
    TransitionRampDownLongSmooth2_100 = 71 UMETA(DisplayName = "Transition Ramp Down Long Smooth 2 - 100 to 0%"),

    TransitionRampDownMediumSmooth1_100 = 72 UMETA(DisplayName = "Transition Ramp Down Medium Smooth 1 - 100 to 0%"),
    TransitionRampDownMediumSmooth2_100 = 73 UMETA(DisplayName = "Transition Ramp Down Medium Smooth 2 - 100 to 0%"),

    TransitionRampDownShortSmooth1_100 = 74 UMETA(DisplayName = "Transition Ramp Down Short Smooth 1 - 100 to 0%"),
    TransitionRampDownShortSmooth2_100 = 75 UMETA(DisplayName = "Transition Ramp Down Short Smooth 2 - 100 to 0%"),

    TransitionRampDownLongSharp1_100 = 76 UMETA(DisplayName = "Transition Ramp Down Long Sharp 1 - 100 to 0%"),
    TransitionRampDownLongSharp2_100 = 77 UMETA(DisplayName = "Transition Ramp Down Long Sharp 2 - 100 to 0%"),

    TransitionRampDownMediumSharp1_100 = 78 UMETA(DisplayName = "Transition Ramp Down Medium Sharp 1 - 100 to 0%"),
    TransitionRampDownMediumSharp2_100 = 79 UMETA(DisplayName = "Transition Ramp Down Medium Sharp 2 - 100 to 0%"),

    TransitionRampDownShortSharp1_100 = 80 UMETA(DisplayName = "Transition Ramp Down Short Sharp 1 - 100 to 0%"),
    TransitionRampDownShortSharp2_100 = 81 UMETA(DisplayName = "Transition Ramp Down Short Sharp 2 - 100 to 0%"),

    TransitionRampUpLongSmooth1_100 = 82 UMETA(DisplayName = "Transition Ramp Up Long Smooth 1 - 100 to 0%"),
    TransitionRampUpLongSmooth2_100 = 83 UMETA(DisplayName = "Transition Ramp Up Long Smooth 2 - 100 to 0%"),

    TransitionRampUpMediumSmooth1_100 = 84 UMETA(DisplayName = "Transition Ramp Up Medium Smooth 1 - 100 to 0%"),
    TransitionRampUpMediumSmooth2_100 = 85 UMETA(DisplayName = "Transition Ramp Up Medium Smooth 2 - 100 to 0%"),

    TransitionRampUpShortSmooth1_100 = 86 UMETA(DisplayName = "Transition Ramp Up Short Smooth 1 - 100 to 0%"),
    TransitionRampUpShortSmooth2_100 = 87 UMETA(DisplayName = "Transition Ramp Up Short Smooth 2 - 100 to 0%"),

    TransitionRampUpLongSharp1_100 = 88 UMETA(DisplayName = "Transition Ramp Up Long Sharp 1 - 100 to 0%"),
    TransitionRampUpLongSharp2_100 = 89 UMETA(DisplayName = "Transition Ramp Up Long Sharp 2 - 100 to 0%"),

    TransitionRampUpMediumSharp1_100 = 90 UMETA(DisplayName = "Transition Ramp Up Medium Sharp 1 - 100 to 0%"),
    TransitionRampUpMediumSharp2_100 = 91 UMETA(DisplayName = "Transition Ramp Up Medium Sharp 2 - 100 to 0%"),

    TransitionRampUpShortSharp1_100 = 92 UMETA(DisplayName = "Transition Ramp Up Short Sharp 1 - 100 to 0%"),
    TransitionRampUpShortSharp2_100 = 93 UMETA(DisplayName = "Transition Ramp Up Short Sharp 2 - 100 to 0%"),

    TransitionRampDownLongSmooth1_50 = 94 UMETA(DisplayName = "Transition Ramp Down Long Smooth 1 - 50 to 0%"),
    TransitionRampDownLongSmooth2_50 = 95 UMETA(DisplayName = "Transition Ramp Down Long Smooth 2 - 50 to 0%"),

    TransitionRampDownMediumSmooth1_50 = 96 UMETA(DisplayName = "Transition Ramp Down Medium Smooth 1 - 50 to 0%"),
    TransitionRampDownMediumSmooth2_50 = 97 UMETA(DisplayName = "Transition Ramp Down Medium Smooth 2 - 50 to 0%"),

    TransitionRampDownShortSmooth1_50 = 98 UMETA(DisplayName = "Transition Ramp Down Short Smooth 1 - 50 to 0%"),
    TransitionRampDownShortSmooth2_50 = 99 UMETA(DisplayName = "Transition Ramp Down Short Smooth 2 - 50 to 0%"),

    TransitionRampDownLongSharp1_50 = 100 UMETA(DisplayName = "Transition Ramp Down Long Sharp 1 - 50 to 0%"),
    TransitionRampDownLongSharp2_50 = 101 UMETA(DisplayName = "Transition Ramp Down Long Sharp 2 - 50 to 0%"),

    TransitionRampDownMediumSharp1_50 = 102 UMETA(DisplayName = "Transition Ramp Down Medium Sharp 1 - 50 to 0%"),
    TransitionRampDownMediumSharp2_50 = 103 UMETA(DisplayName = "Transition Ramp Down Medium Sharp 2 - 50 to 0%"),

    TransitionRampDownShortSharp1_5 = 104 UMETA(DisplayName = "Transition Ramp Down Short Sharp 1 - 50 to 0%"),
    TransitionRampDownShortSharp2_50 = 105 UMETA(DisplayName = "Transition Ramp Down Short Sharp 2 - 50 to 0%"),

    TransitionRampUpLongSmooth1_50 = 106 UMETA(DisplayName = "Transition Ramp Up Long Smooth 1 - 0 to 50%"),
    TransitionRampUpLongSmooth2_50 = 107 UMETA(DisplayName = "Transition Ramp Up Long Smooth 2 - 0 to 50%"),

    TransitionRampUpMediumSmooth1_50 = 108 UMETA(DisplayName = "Transition Ramp Up Medium Smooth 1 - 0 to 50%"),
    TransitionRampUpMediumSmooth2_50 = 109 UMETA(DisplayName = "Transition Ramp Up Medium Smooth 2 - 0 to 50%"),

    TransitionRampUpShortSmooth1_50 = 110 UMETA(DisplayName = "Transition Ramp Up Short Smooth 1 - 0 to 50%"),
    TransitionRampUpShortSmooth2_50 = 111 UMETA(DisplayName = "Transition Ramp Up Short Smooth 2 - 0 to 50%"),

    TransitionRampUpLongSharp1_50 = 112 UMETA(DisplayName = "Transition Ramp Up Long Sharp 1 - 0 to 50%"),
    TransitionRampUpLongSharp2_50 = 113 UMETA(DisplayName = "Transition Ramp Up Long Sharp 2 - 0 to 50%"),

    TransitionRampUpMediumSharp1_50 = 114 UMETA(DisplayName = "Transition Ramp Up Medium Sharp 1 - 0 to 50%"),
    TransitionRampUpMediumSharp2_50 = 115 UMETA(DisplayName = "Transition Ramp Up Medium Sharp 2 - 0 to 50%"),

    TransitionRampUpShortSharp1_50 = 116 UMETA(DisplayName = "Transition Ramp Up Short Sharp 1 - 0 to 50%"),
    TransitionRampUpShortSharp2_50 = 117 UMETA(DisplayName = "Transition Ramp Up Short Sharp 2 - 0 to 50%"),

    LongBuzz_100 = 118 UMETA(DisplayName = "Long buzz for programmatic stopping - 100%"),

    SmoothHum_50 = 119 UMETA(DisplayName = "Smooth Hum 1 (No kick or brake pulse) - 50%"),
    SmoothHum_40 = 120 UMETA(DisplayName = "Smooth Hum 2 (No kick or brake pulse) - 40%"),
    SmoothHum_30 = 121 UMETA(DisplayName = "Smooth Hum 3 (No kick or brake pulse) - 30%"),
    SmoothHum_20 = 122 UMETA(DisplayName = "Smooth Hum 4 (No kick or brake pulse) - 20%"),
    SmoothHum_10 = 123 UMETA(DisplayName = "Smooth Hum 5 (No kick or brake pulse) - 10%"),

    COUNT UMETA(Hidden),
};

ENUM_RANGE_BY_COUNT(EBS_VibrationWaveformEffect, static_cast<int32>(EBS_VibrationWaveformEffect::COUNT))