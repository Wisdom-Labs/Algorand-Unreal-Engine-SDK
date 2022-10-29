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

	Vertices::VerticesLoadaccountinfoGetRequest
		buildLoadAccountInfoRequest(const FString& address)
	{
		Vertices::VerticesLoadaccountinfoGetRequest request;
		request.Address = address;
		return request;
	}
}