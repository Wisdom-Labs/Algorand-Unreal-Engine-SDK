#pragma once

#include "VerticesSDK.h"

namespace request_builders {
	algorand::vertices::VerticesSDK::VerticesGetaddressbalanceGetRequest
	buildGetBalanceRequest(const FString& address);
	algorand::vertices::VerticesSDK::VerticesLoadaccountinfoGetRequest
		buildLoadAccountInfoRequest(const FString& address);
}