#pragma once

#include "VerticesSDK.h"

namespace response_builders {
	algorand::vertices::VerticesSDK::VerticesGetaddressbalanceGetResponse
		buildGetBalanceResponse(const int& amount);
	algorand::vertices::VerticesSDK::VerticesLoadaccountinfoGetResponse
		buildLoadAccountInfoResponse(const int& amount);
}