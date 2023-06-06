#include "ArcResponseBuilders.h"
#include "VerticesApiOperations.h"

namespace ArcResponseBuilders {
	
	void ArcResponseBuilders::buildArcAssetDetailsResponse(const Arc03&, const FAPIArcAssetDetailsGetDelegate& delegate)
	{
		Vertices::VerticesArcAssetDetailsGetResponse response;
		delegate.ExecuteIfBound(response);
	}

	void ArcResponseBuilders::buildArcAssetDetailsResponse(const Arc69&, const FAPIArcAssetDetailsGetDelegate& delegate)
	{
		Vertices::VerticesArcAssetDetailsGetResponse response;
		delegate.ExecuteIfBound(response);
	}

}
