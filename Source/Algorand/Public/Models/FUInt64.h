// Copyright 2022, Wisdom Labs. All Rights Reserved

#pragma once
#include "FUInt64.generated.h"

#define PACKED
#pragma pack(push, 1)

/**
 * @brief user-defined uint64
 */

USTRUCT(BlueprintType)
struct ALGORAND_API FUInt64 {
    GENERATED_BODY()

    uint64 Value;

    operator uint64() const;
    FUInt64();
    FUInt64(uint64 value);
};

#pragma pack(pop)
#undef PACKED

/**
 * @brief used to convert FUInt64 to FString 
 */
UCLASS(BlueprintType)
class ALGORAND_API UUInt64Factory : public UObject
{
    GENERATED_BODY()

public:
    /**
     * @brief static function to get FUInt64 from FString
     * @param value value to convert as string
     * @return converted value as FUInt64 
     */
    UFUNCTION(BlueprintCallable, Category = "UInt64")
        static FUInt64 UInt64FromString(const FString& value);
};