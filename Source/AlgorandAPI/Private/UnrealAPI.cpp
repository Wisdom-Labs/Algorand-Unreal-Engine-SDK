/** 
* Algorand Node API
*/

#include "UnrealApi.h"

namespace algorand{
namespace api {

UnrealApi::UnrealApi()
    : Url(TEXT("http://localhost"))
{
}

UnrealApi::~UnrealApi() {}

void UnrealApi::SetURL(const FString& InUrl)
{
    Url = InUrl;
}

void UnreanlApi::AlgorandGetaddressbalanceGet(const AlgorandGetaddressbalanceGetRequest& Request, const FAlgorandGetaddressbalanceGetDelegate& Delegate) const
{
    // IsValid Endpoint Url is not set ,  check this

    // Request process for http
    // if result , call onAlgorandgetbalance with Delegate
}

void UnrealApi::OnAlgorandGetaddressbalanceGetResponse(const AlgorandGetaddressbalanceGetResponse& response, bool bSucceed, FAlgorandGetaddressbalanceGetDelegate Delegate)
{
    Delegate.ExecuteIfBound(response);
}

}
}