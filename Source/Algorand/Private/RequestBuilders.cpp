// Copyright 2022, Wisdom Labs. All Rights Reserved

#include "RequestBuilders.h"
#include "VerticesApiOperations.h"

namespace {
	using Vertices = algorand::vertices::VerticesSDK;
}
namespace request_builders {
	Vertices::VerticesGenerateWalletGetRequest
	buildGenerateWalletRequest()
	{
		Vertices::VerticesGenerateWalletGetRequest request;
		return request;
	}

	Vertices::VerticesGetaddressbalanceGetRequest
	buildGetBalanceRequest(const FString& address)
	{
		Vertices::VerticesGetaddressbalanceGetRequest request;
		request.Address = address;
		return request;
	}
	
	Vertices::VerticesPaymentTransactionGetRequest
		buildPaymentTransactionRequest(const FString& sender, 
									   const FString& receiver,
									   const FUInt64& amount,
									   const FString& notes)
	{
		Vertices::VerticesPaymentTransactionGetRequest request;
		request.senderAddress = sender;
		request.receiverAddress = receiver;
		request.amount = amount;
		request.notes = notes;
		return request;
	}

	Vertices::VerticesApplicationCallTransactionGetRequest
		buildApplicationCallTransactionRequest(const FString& sender, 
									   const FUInt64& app_ID)
	{
		Vertices::VerticesApplicationCallTransactionGetRequest request;
		request.senderAddress = sender;
		request.app_ID = app_ID;
		return request;
	}
}