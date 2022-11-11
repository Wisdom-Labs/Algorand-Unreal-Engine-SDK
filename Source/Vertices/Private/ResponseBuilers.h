#pragma once

#include "VerticesSDK.h"

namespace response_builders {
	algorand::vertices::VerticesSDK::VerticesGenerateWalletGetResponse
		buildGenerateWalletResponse(const FString& address);
	algorand::vertices::VerticesSDK::VerticesGetaddressbalanceGetResponse
		buildGetBalanceResponse(const uint64& amount);
	algorand::vertices::VerticesSDK::VerticesPaymentTransactionGetResponse
		buildPaymentTransactionResponse(const FString& txID);
	algorand::vertices::VerticesSDK::VerticesApplicationCallTransactionGetResponse
		buildApplicationCallTransactionResponse(const FString& txID);
}