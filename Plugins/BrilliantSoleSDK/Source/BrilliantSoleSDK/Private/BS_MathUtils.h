// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Logging/StructuredLog.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_MathUtils, Warning, All);

class BS_MathUtils
{
public:
    template <typename T, typename U>
    static constexpr T ClampToRange(U value)
    {
        // Ensure that T and U are arithmetic types (integers or floating-point)
        static_assert(std::is_arithmetic_v<T>, "Target type T must be an arithmetic type.");
        static_assert(std::is_arithmetic_v<U>, "Source type U must be an arithmetic type.");

        // Get the min and max values of the target type
        const T minValue = std::numeric_limits<T>::lowest();
        const T maxValue = std::numeric_limits<T>::max();

        // Clamp the value to fit within the range of the target type
        return static_cast<T>(std::clamp(value, static_cast<U>(minValue), static_cast<U>(maxValue)));
    }
};