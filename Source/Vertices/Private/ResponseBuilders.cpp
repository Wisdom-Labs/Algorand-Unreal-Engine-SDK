// Copyright 2022, Wisdom Labs. All Rights Reserved

#include "./ResponseBuilers.h"
#include "VerticesApiOperations.h"

namespace {
	using Vertices = algorand::vertices::VerticesSDK;
}

namespace response_builders {
	Vertices::VerticesGenerateWalletGetResponse
		buildGenerateWalletResponse(const FString& address)
	{
		Vertices::VerticesGenerateWalletGetResponse response;
		response.Address = address;
		return response;
	}

	Vertices::VerticesGetaddressbalanceGetResponse
		buildGetBalanceResponse(const uint64& amount)
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