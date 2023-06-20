#pragma once

#include "VerticesSDK.h"
#include "Arc/Arc03.h"
#include "Arc/Arc19.h"
#include "Arc/Arc69.h"
#include "Arc/AccountAsset.h"

namespace {
	using Vertices = algorand::vertices::VerticesSDK;
}

namespace ArcResponseBuilders {

		/// arc asset details callback
		DECLARE_DELEGATE_OneParam(FAPIArcAssetDetailsGetDelegate, const Vertices::VerticesArcAssetDetailsGetResponse&);
		/// account information callback
		DECLARE_DELEGATE_OneParam(FAPIAccountInfoGetDelegate, const Vertices::VerticesAccountInformationGetResponse&);

		void buildArcAssetDetailsResponse(const Arc03&, const FAPIArcAssetDetailsGetDelegate&);
		void buildArcAssetDetailsResponse(const Arc19&, const FAPIArcAssetDetailsGetDelegate&);
		void buildArcAssetDetailsResponse(const Arc69&, const FAPIArcAssetDetailsGetDelegate&);
		void buildAccountInformationResponse(const AccountAsset&, const FAPIAccountInfoGetDelegate&);

}
