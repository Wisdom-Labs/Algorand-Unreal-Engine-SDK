/** 
* Algorand Node API
*/

#include "UnrealApi.h"
#include "UnrealApiOperations.h"

namespace algorand{
namespace api {

UnrealApi::UnrealApi(TSharedPtr<algorand::vertices::VerticesSDK>& vertices)
    : Url(TEXT("http://localhost"))
{
    vertices_ = vertices;
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
    FString address = FString("QTYBYPJVSPT7SXSJQ5CLH2C5EQXXWEBBCKUWZUCGJGOGTODHYG43WQQSCM");
    this->vertices_->AlgorandGetaddressbalanceGet(address);
}

void UnrealApi::OnAlgorandGetaddressbalanceGetResponse(AlgorandGetaddressbalanceGetResponse response, bool bSucceed, FAlgorandGetaddressbalanceGetDelegate Delegate) const
{
    Delegate.ExecuteIfBound(response);
}

}
}