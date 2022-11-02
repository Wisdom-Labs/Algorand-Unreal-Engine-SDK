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
		buildPaymentTransactionResponse(const FString& txID)
	{
		Vertices::VerticesPaymentTransactionGetResponse response;
		response.txID = txID;
		return response;
	}

	Vertices::VerticesApplicationCallTransactionGetResponse
		buildApplicationCallTransactionResponse(const FString& txID)
	{
		Vertices::VerticesApplicationCallTransactionGetResponse response;
		response.txID = txID;
		return response;
	}
}