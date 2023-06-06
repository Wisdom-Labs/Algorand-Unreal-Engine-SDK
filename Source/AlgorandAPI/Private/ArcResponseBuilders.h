#pragma once

#include "VerticesSDK.h"
#include "Arc/Arc03.h"
#include "Arc/Arc69.h"

namespace {
	using Vertices = algorand::vertices::VerticesSDK;
}

namespace ArcResponseBuilders {

		/// arc asset details callback
		DECLARE_DELEGATE_OneParam(FAPIArcAssetDetailsGetDelegate, const Vertices::VerticesArcAssetDetailsGetResponse&);

		void buildArcAssetDetailsResponse(const Arc03&, const FAPIArcAssetDetailsGetDelegate&);
		void buildArcAssetDetailsResponse(const Arc69&, const FAPIArcAssetDetailsGetDelegate&);

}
