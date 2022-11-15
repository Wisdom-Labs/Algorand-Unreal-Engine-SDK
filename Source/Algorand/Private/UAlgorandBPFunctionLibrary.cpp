#include "UAlgorandBPFunctionLibrary.h"

FString
UAlgorandBPFunctionLibrary::Conv_FUInt64ToString(const FUInt64& value)
{
    return FString::Printf(TEXT("%llu"), value.Value);
}

FUInt64 
UAlgorandBPFunctionLibrary::Conv_FStringToUInt64(const FString& value)
{
    return FUInt64(FCString::Strtoui64(*value, NULL, 10));
}