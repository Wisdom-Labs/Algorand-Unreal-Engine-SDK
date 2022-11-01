#pragma once

#include "VerticesSDK.h"

namespace request_builders {
	algorand::vertices::VerticesSDK::VerticesGetaddressbalanceGetRequest
	buildGetBalanceRequest(const FString& address);
	algorand::vertices::VerticesSDK::VerticesPaymentTransactionGetRequest
		buildPaymentTransactionRequest(const FString& sender,
									   const FString& receiver,
									   const uint64_t& amount);
}