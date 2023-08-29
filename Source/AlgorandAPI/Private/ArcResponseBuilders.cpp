#include "ArcResponseBuilders.h"
#include "VerticesApiOperations.h"

namespace ArcResponseBuilders {
	
	void ArcResponseBuilders::buildArcAssetDetailsResponse(const Arc03& arc_asset, const FAPIArcAssetDetailsGetDelegate& delegate)
	{
		Vertices::VerticesArcAssetDetailsGetResponse response;
		
		response.standard = 1;		// arc03 
		
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
		
		AsyncTask(ENamedThreads::GameThread, [delegate, response]()
		{
			delegate.ExecuteIfBound(response);
		});
	}

	void ArcResponseBuilders::buildArcAssetDetailsResponse(const Arc19& arc_asset, const FAPIArcAssetDetailsGetDelegate& delegate)
	{
		Vertices::VerticesArcAssetDetailsGetResponse response;
		
		response.standard = 2;			// arc19

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
		response.properties = arc_asset.metadata.properties; 
		
		AsyncTask(ENamedThreads::GameThread, [delegate, response]()
		{
			delegate.ExecuteIfBound(response);
		});
	}

	void ArcResponseBuilders::buildArcAssetDetailsResponse(const Arc69& arc_asset, const FAPIArcAssetDetailsGetDelegate& delegate)
	{
		Vertices::VerticesArcAssetDetailsGetResponse response;
		
		response.standard = 3;	// arc69

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
		
		AsyncTask(ENamedThreads::GameThread, [delegate, response]()
		{
			delegate.ExecuteIfBound(response);
		});
	}

	void ArcResponseBuilders::buildAccountInformationResponse(const AccountAsset& arc_asset, const FAPIAccountInfoGetDelegate& delegate)
	{
		Vertices::VerticesAccountInformationGetResponse response;

		if(arc_asset.info.Num() != 0)
		{
			for(int i = 0; i < arc_asset.info.Num(); i++)
			{
				response.assetIDs.Add(FString::Printf(TEXT("%llu"), arc_asset.info[i].id));
				response.assetNames.Add(arc_asset.info[i].name);
			}
				
			response.SetSuccessful(true);		
		}
		else
		{
			response.SetSuccessful(false);
			response.SetResponseString("can't fetch account Info for assets.");
		}
		
		AsyncTask(ENamedThreads::GameThread, [delegate, response]()
		{
			delegate.ExecuteIfBound(response);
		});
	}
}
