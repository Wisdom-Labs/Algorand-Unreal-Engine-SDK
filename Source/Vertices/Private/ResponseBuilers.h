#pragma once

#include "VerticesSDK.h"

namespace response_builders {
	algorand::vertices::VerticesSDK::VerticesGetaddressbalanceGetResponse
		buildGetBalanceResponse(const int& amount);
	algorand::vertices::VerticesSDK::VerticesPaymentTransactionGetResponse
		buildPaymentTransactionResponse(const int& amount);
	algorand::vertices::VerticesSDK::VerticesApplicationCallTransactionGetResponse
		buildApplicationCallTransactionResponse(const int& amount);
}