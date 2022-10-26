#pragma once

#include "VerticesSDK.h"

namespace response_builders {
	algorand::vertices::VerticesSDK::VerticesGetaddressbalanceGetResponse
		buildGetBalanceResponse(const int& amount);
}