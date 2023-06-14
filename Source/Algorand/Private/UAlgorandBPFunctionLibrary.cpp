#include "UAlgorandBPFunctionLibrary.h"

#include <string>

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
	double MyDouble = FCString::Atof(*value) * FMath::Pow(10.f, 6);
	uint64 Value64 = static_cast<uint64>(MyDouble);
	return FUInt64(Value64);
}

/**
 * @brief print value as period string after convert long long int
 */
FString
UAlgorandBPFunctionLibrary::Conv_FUInt64ToPeriodString(const FUInt64& value, const FUInt64& count = 3)
{
	const uint8 Period_Pos = 6; 
	FString PeriodString = FString::Printf(TEXT("%llu"), value.Value);;
	
	// Insert period every count digits
	int32 InsertPosition = 0;
	if( PeriodString.Len() - Period_Pos > 0 ) // Insert position for first period
	{
		InsertPosition = PeriodString.Len() - Period_Pos;
		PeriodString.InsertAt(InsertPosition, TEXT("."));	
	}
	
	float myFloat = atof(TCHAR_TO_UTF8(*PeriodString));
	PeriodString = RemoveTrailingZeros(myFloat);

	UE_LOG(LogTemp, Warning, TEXT("Period String: %s"), *PeriodString);
	
	while (InsertPosition > count)
	{
		PeriodString.InsertAt(InsertPosition, TEXT(","));
		InsertPosition -= count;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Period String: %s"), *PeriodString);
	
	return PeriodString;
}

FString UAlgorandBPFunctionLibrary::Conv_PropertiesToString(TMap<FString, FString> map_data)
{
	FString MapAsString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&MapAsString);
	Writer->WriteObjectStart();
	for (auto& Elem : map_data)
	{
		Writer->WriteValue(Elem.Key, Elem.Value);
	}
	Writer->WriteObjectEnd();
	Writer->Close();

	return MapAsString;
}

FString UAlgorandBPFunctionLibrary::RemoveTrailingZeros(float Value)
{
	std::string StdStr = std::to_string(Value);
	StdStr.erase(StdStr.find_last_not_of('0') + 1, std::string::npos);

	// If the last character is '.', remove it as well
	if (StdStr.back() == '.')
		StdStr.pop_back();

	return StdStr.c_str();
}
