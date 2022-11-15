#pragma once

#include "CoreMinimal.h"
#include "Models/FUInt64.h"

#include "UAlgorandBPFunctionLibrary.generated.h"

UCLASS()
class UAlgorandBPFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure,
        meta = (DisplayName = "FUInt64 To String", CompactNodeTitle = "->",
            BlueprintAutocast),
        Category = "FUInt64")
        static FString Conv_FUInt64ToString(const FUInt64& value);

    UFUNCTION(BlueprintPure,
        meta = (DisplayName = "String FUInt64", CompactNodeTitle = "->",
            BlueprintAutocast),
        Category = "String")
        static FUInt64 Conv_FStringToUInt64(const FString& value);
};
