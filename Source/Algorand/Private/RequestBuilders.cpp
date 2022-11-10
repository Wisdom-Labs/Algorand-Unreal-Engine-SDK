#include "RequestBuilders.h"
#include "VerticesApiOperations.h"

namespace {
	using Vertices = algorand::vertices::VerticesSDK;
}
namespace request_builders {
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
									   const FUInt64& amount)
	{
		Vertices::VerticesPaymentTransactionGetRequest request;
		request.senderAddress = sender;
		request.receiverAddress = receiver;
		request.amount = amount;
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