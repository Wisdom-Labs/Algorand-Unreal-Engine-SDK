#include "UAlgorandBPFunctionLibrary.h"

FString
UAlgorandBPFunctionLibrary::Conv_FUInt64ToString(const FUInt64& value)
{
    return FString::Printf(TEXT("%llu"), value.Value);
}
