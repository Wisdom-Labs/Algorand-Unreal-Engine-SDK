#include "UAlgorandBPFunctionLibrary.h"

/**
 * @brief print value as string after convert to long long int
 */
FString
UAlgorandBPFunctionLibrary::Conv_FUInt64ToString(const FUInt64& value)
{
    return FString::Printf(TEXT("%llu"), value.Value);
}

/**
 * @brief print value as FUInt64 using Strtoui64
 */
FUInt64 
UAlgorandBPFunctionLibrary::Conv_FStringToUInt64(const FString& value)
{
    return FUInt64(FCString::Strtoui64(*value, NULL, 10));
}