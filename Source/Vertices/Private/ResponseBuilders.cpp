#include "./ResponseBuilers.h"
#include "VerticesApiOperations.h"

namespace {
	using Vertices = algorand::vertices::VerticesSDK;
}

namespace response_builders {
	Vertices::VerticesGetaddressbalanceGetResponse
		buildGetBalanceResponse(const int& amount)
	{
		Vertices::VerticesGetaddressbalanceGetResponse response;
		response.Amount = amount;
		return response;
	}

	Vertices::VerticesPaymentTransactionGetResponse
		buildPaymentTransactionResponse(const int& amount)
	{
		Vertices::VerticesPaymentTransactionGetResponse response;
		response.Amount = amount;
		return response;
	}
}