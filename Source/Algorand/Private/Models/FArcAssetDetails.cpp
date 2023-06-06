#include "Models/FArcAssetDetails.h"
#include "VerticesApiOperations.h"

FArcAssetDetails::FArcAssetDetails()
{
	standard = EArcType::Arc00;
	unit_name = "";
	total = 0;
	decimals = 0;
	description = "";
	clawback = "";
	creator = "";
	freeze = "";
	manager = "";
	reserve = "";
	media_url = "";
	external_url = "";
	animation_url = "";
	properties = TMap<FString, FString>();
}

FArcAssetDetails::FArcAssetDetails(const algorand::vertices::VerticesSDK::VerticesArcAssetDetailsGetResponse& response)
{
	standard = EArcType(response.standard);
	unit_name = response.unit_name;
	total = response.total;
	decimals = response.decimals;
	description = response.description;
	clawback = response.clawback;
	creator = response.creator;
	freeze = response.freeze;
	manager = response.manager;
	reserve = response.reserve;
	media_url = response.media_url;
	external_url = response.external_url;
	animation_url = response.animation_url;
	properties = response.properties;
}

