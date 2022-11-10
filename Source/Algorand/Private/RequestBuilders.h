#pragma once

#include "Models/FUInt64.h"
#include "VerticesSDK.h"

namespace request_builders {
	algorand::vertices::VerticesSDK::VerticesGetaddressbalanceGetRequest
	buildGetBalanceRequest(const FString& address);
	algorand::vertices::VerticesSDK::VerticesPaymentTransactionGetRequest
		buildPaymentTransactionRequest(const FString& sender,
									   const FString& receiver,
									   const FUInt64& amount);
	algorand::vertices::VerticesSDK::VerticesApplicationCallTransactionGetRequest
		buildApplicationCallTransactionRequest(const FString& sender,
									   const FUInt64& app_ID);
}