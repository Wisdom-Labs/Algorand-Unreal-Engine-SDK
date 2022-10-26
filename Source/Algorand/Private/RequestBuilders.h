#pragma once

#include "VerticesSDK.h"

namespace request_builders {
	algorand::vertices::VerticesSDK::VerticesGetaddressbalanceGetRequest
	buildGetBalanceRequest(const FString& address);
}