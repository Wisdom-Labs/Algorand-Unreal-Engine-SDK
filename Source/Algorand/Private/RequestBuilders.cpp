#include "RequestBuilders.h"

namespace request_builders {
algorand::api::UnrealApi::AlgorandGetaddressbalanceGetRequest
buildGetBalanceRequest(const FString& address)
{
	algorand::api::UnrealApi::AlgorandGetaddressbalanceGetRequest request;
	request.Address = address;
	return request;
}
}