#include "ArcResponseBuilders.h"
#include "VerticesApiOperations.h"

namespace ArcResponseBuilders {
	
	void ArcResponseBuilders::buildArcAssetDetailsResponse(const Arc03& arc_asset, const FAPIArcAssetDetailsGetDelegate& delegate)
	{
		Vertices::VerticesArcAssetDetailsGetResponse response;

		if(arc_asset.metadata.standard.Contains("arc69"))
			response.standard = 2;
		else
			response.standard = 1;
		response.unit_name = arc_asset.asset.params.unit_name;
		response.total = arc_asset.asset.params.total;
		response.decimals = arc_asset.asset.params.decimals;
		response.description = arc_asset.metadata.description;
		response.clawback = arc_asset.asset.params.clawback;
		response.creator = arc_asset.asset.params.creator;
		response.freeze = arc_asset.asset.params.freeze;
		response.manager = arc_asset.asset.params.manager;
		response.reserve = arc_asset.asset.params.reserve;
		response.media_url = arc_asset.metadata.image;
		response.external_url = arc_asset.metadata.external_url;
		response.animation_url = arc_asset.metadata.animation_url;
		response.properties = arc_asset.metadata.properties; 
		
		delegate.ExecuteIfBound(response);
	}

	void ArcResponseBuilders::buildArcAssetDetailsResponse(const Arc69& arc_asset, const FAPIArcAssetDetailsGetDelegate& delegate)
	{
		Vertices::VerticesArcAssetDetailsGetResponse response;
		if(arc_asset.metadata.standard.Contains("arc69"))
			response.standard = 2;
		else
			response.standard = 1;
		response.unit_name = arc_asset.asset.params.unit_name;
		response.total = arc_asset.asset.params.total;
		response.decimals = arc_asset.asset.params.decimals;
		response.description = arc_asset.metadata.description;
		response.clawback = arc_asset.asset.params.clawback;
		response.creator = arc_asset.asset.params.creator;
		response.freeze = arc_asset.asset.params.freeze;
		response.manager = arc_asset.asset.params.manager;
		response.reserve = arc_asset.asset.params.reserve;
		response.media_url = arc_asset.metadata.media_url;
		response.external_url = arc_asset.metadata.external_url;
		response.properties = arc_asset.metadata.properties;

		response.SetSuccessful(true);
		
		delegate.ExecuteIfBound(response);
	}

}
