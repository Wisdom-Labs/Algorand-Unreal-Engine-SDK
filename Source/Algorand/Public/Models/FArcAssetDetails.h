#pragma once

#include "FUInt64.h"
#include "VerticesSDK.h"
#include "FArcAssetDetails.generated.h"

#define PACKED
#pragma pack(push, 1)

/**
 * @brief user-defined FArcAssetDetailsStruct
 */

UENUM(BlueprintType)
enum class EArcType : uint8 {
	Arc00 = 0 UMETA(Hidden),
	Arc03 = 1 UMETA(DisplayName = "Arc03"),
	Arc69 = 2 UMETA(DisplayName = "Arc69")
};

USTRUCT(BlueprintType)
struct ALGORAND_API FArcAssetDetails
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArcAsset")
	EArcType standard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArcAsset")
	FString unit_name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArcAsset")
	FUInt64 total;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArcAsset")
	FUInt64 decimals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArcAsset")
	FString description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArcAsset")
	FString clawback;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArcAsset")
	FString creator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArcAsset")
	FString freeze;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArcAsset")
	FString manager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArcAsset")
	FString reserve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArcAsset")
	FString media_url;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArcAsset")
	FString external_url;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArcAsset")
	FString animation_url;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArcAsset")
	TMap<FString, FString> properties;

	FArcAssetDetails();
	FArcAssetDetails(const algorand::vertices::VerticesSDK::VerticesArcAssetDetailsGetResponse& response);
	
};

USTRUCT(BlueprintType)
struct ALGORAND_API FCreatedAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CreatedAsset")
	FString ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CreatedAsset")
	FString Name;
};

#pragma pack(pop)
#undef PACKED
