// Copyright 2022, Wisdom Labs. All Rights Reserved

#pragma once
#include "CoreMinimal.h"

#include "FError.generated.h"

/**
 * @brief this struct is used as error type on blueprint 
 */
USTRUCT(BlueprintType)
struct FError {
    GENERATED_USTRUCT_BODY()

        UPROPERTY(BlueprintReadOnly, Category = "Default")
        FString message;

    FError() {}

    FError(const FString& errorMessage) : message(errorMessage) {}
};