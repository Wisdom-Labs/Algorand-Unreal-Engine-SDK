#pragma once

#include "CoreMinimal.h"

#include "FError.generated.h"

USTRUCT(BlueprintType)
struct FError {
    GENERATED_USTRUCT_BODY()

        UPROPERTY(BlueprintReadOnly, Category = "Default")
        FString message;

    FError() {}

    FError(const FString& errorMessage) : message(errorMessage) {}
};