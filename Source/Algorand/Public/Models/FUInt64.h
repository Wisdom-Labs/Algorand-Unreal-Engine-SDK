#pragma once

#include "FUInt64.generated.h"

USTRUCT(BlueprintType)
struct FUInt64 {
    GENERATED_BODY()

    uint64 Value;

    operator uint64() const;
    FUInt64();
    FUInt64(uint64 value);
}

UCLASS(BlueprintType)
class UUInt64Factory : public UObject 
{
    GENERATED_BODY()

    public:
    UFUNCTION(BlueprintCallable, Category = "UInt64")
    static FUInt64 UInt64FromString(const FString& value);
}