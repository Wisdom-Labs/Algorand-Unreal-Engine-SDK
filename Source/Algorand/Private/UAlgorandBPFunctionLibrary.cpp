#include "UAlgorandBPFunctionLibrary.h"

/**
 * @brief print value as string after convert long long int
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

/**
 * @brief print value as comma string after convert long long int
 */
FString
UAlgorandBPFunctionLibrary::Conv_FUInt64ToCommaString(const FUInt64& value, const FUInt64& count = 3)
{
	FString CommaString = FString::Printf(TEXT("%llu"), value.Value);;

	// Insert comma every count digits
	int32 InsertPosition = CommaString.Len() - count; // Insert position for first comma

	while (InsertPosition > 0)
	{
		CommaString.InsertAt(InsertPosition, TEXT(","));
		InsertPosition -= count;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Comma String: %s"), *CommaString);
	
	return CommaString;
}