// Copyright 2022, Wisdom Labs. All Rights Reserved

#pragma once
#include "CoreMinimal.h"
#include "Models/FUInt64.h"

#include "UAlgorandBPFunctionLibrary.generated.h"

/**
 * @brief BlueprintFunctionLibrary
 * how library converts user-defined and standard data type
 * to wanted one on blueprint
 */

UCLASS()
class UAlgorandBPFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * @brief convert new defined FUInt64 to FString
     * @param value value for convert
     * @return converted value as string 
     */
    UFUNCTION(BlueprintPure,
        meta = (DisplayName = "FUInt64 To String", CompactNodeTitle = "->",
            BlueprintAutocast),
        Category = "FUInt64")
        static FString Conv_FUInt64ToString(const FUInt64& value);

    /**
     * @brief convert FString to new defined FUInt64
     * @param value value for convert
     * @return converted value as FUInt64 
     */
    UFUNCTION(BlueprintPure,
        meta = (DisplayName = "String FUInt64", CompactNodeTitle = "->",
            BlueprintAutocast),
        Category = "String")
        static FUInt64 Conv_FStringToUInt64(const FString& value);

    /**
     * @brief convert new defined FUInt64 to FString with period   
     * @param value value for convert
     * @return converted value as string 
     */
    UFUNCTION(BlueprintPure,
        meta = (DisplayName = "FUInt64 To Period String", CompactNodeTitle = "->",
            BlueprintAutocast),
        Category = "FUInt64")
        static FString Conv_FUInt64ToPeriodString(const FUInt64& value, const FUInt64& count);

    /**
     * @brief static function to get TMap from FString
     * @param value value to convert as string
     * @return converted value as TMap 
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "TMap To JSON String", Keywords = "FArcAssetDetails"),
              Category = "ArcAsset")
        static FString Conv_PropertiesToString(TMap<FString, FString> map_data);

    // Function to remove trailing zeros from a float value and return as string
    static FString RemoveTrailingZeros(float Value);
};
