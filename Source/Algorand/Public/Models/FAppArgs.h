#include "Containers/List.h"

#pragma once
#include "include/vertices/vertices_errors.h"
#include "FAppArgs.generated.h"

#define PACKED
#pragma pack(push, 1)

#define APP_ARGS_MAX_SIZE 8
/**
 * @brief user-defined FAppCallTX
 */

UENUM(BlueprintType)
enum class EAppOnCompleteTX : uint8 {
	NOOP = 0 UMETA(DisplayName = "NOOP"),
	OPT_IN = 1 UMETA(DisplayName = "OPT_IN"),
	CLOSE_OUT = 2 UMETA(DisplayName = "CLOSE_OUT"),
	CLEAR_STATE = 3 UMETA(DisplayName = "CLEAR_STATE"),
	UPDATE_APP = 4 UMETA(DisplayName = "UPDATE_APP"),
	DELETE_APP = 5 UMETA(DisplayName = "DELETE_APP")
};

UENUM(BlueprintType)
enum class EAppArgType : uint8 {
	VALUE_METHOD = 0 UMETA(DisplayName = "Arg Method"),
	VALUE_INT = 1 UMETA(DisplayName = "Arg Integer"),
	VALUE_BYTE_SLICE= 2 UMETA(DisplayName = "Arg Byte Array")
};

USTRUCT(BlueprintType)
struct ALGORAND_API FAppArg {
	GENERATED_BODY()
	
	TArray<uint8_t> Value_Bytes;
	FAppArg();
	FAppArg(const uint8_t value_bytes_[], const uint8_t& len);
};

#pragma pack(pop)
#undef PACKED


/**
 * @brief used to convert FString to FAppArg 
 */
UCLASS(BlueprintType)
class ALGORAND_API UAppArgFactory : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * @brief static function to get FString & Type from FAppArg
	 * @param value value to convert as app arg
	 * @return converted value as FAppArg 
	 */
	UFUNCTION(BlueprintCallable, Category = "App Arg")
	static FAppArg FAppArgFromString(const EAppArgType& value_type, const FString& value);

	static ret_code_t
	sha512_256(unsigned char const *input,
				unsigned long ilen,
				unsigned char *output,
				unsigned long olen);
};
