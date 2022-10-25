/** 
* Algorand Node API
*/

#include "UnrealApi.h"
#include "UnrealApiOperations.h"

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

void UnrealApi::AlgorandGetaddressbalanceGet(const AlgorandGetaddressbalanceGetRequest& Request, const FAlgorandGetaddressbalanceGetDelegate& Delegate) const
{
    // IsValid Endpoint Url is not set ,  check this

    // Request process for http
    // if result , call onAlgorandgetbalance with Delegate
    if (Url.Find("local"))
    {
        FString newUrl = Url.Replace(TEXT("local"), TEXT("oal"), ESearchCase::CaseSensitive );
    }
    
}

void UnrealApi::OnAlgorandGetaddressbalanceGetResponse(AlgorandGetaddressbalanceGetResponse response, bool bSucceed, FAlgorandGetaddressbalanceGetDelegate Delegate) const
{
    Delegate.ExecuteIfBound(response);
}

}
}