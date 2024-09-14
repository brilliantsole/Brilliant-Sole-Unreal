// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BS_TfliteModel.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_TfliteModel, Verbose, Warning);

UCLASS(Blueprintable)
class BRILLIANTSOLESDK_API UBS_TfliteModel : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, Category = "BS Tflite Model")
    static const TArray<uint8> &GetTfliteModel() { return TfliteModel; }

private:
    static const TArray<uint8> TfliteModel;
};