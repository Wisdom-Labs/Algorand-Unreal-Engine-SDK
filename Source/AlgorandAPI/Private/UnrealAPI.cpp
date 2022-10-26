/** 
* Algorand Node API
*/

#include "UnrealApi.h"
#include "Misc/MessageDialog.h"

#define LOCTEXT_NAMESPACE "FVerticesModule"

namespace {
    using Vertices = algorand::vertices::VerticesSDK;
}

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

void UnrealApi::AlgorandGetaddressbalanceGet(const Vertices::VerticesGetaddressbalanceGetRequest& Request, const FAlgorandGetaddressbalanceGetDelegate& Delegate) const
{
    // IsValid Endpoint Url is not set ,  check this

    Vertices::FVerticesGetaddressbalanceGetDelegate delegate;

    delegate.BindLambda([this, Delegate]
        (const auto& response) {
            OnAlgorandGetaddressbalanceGetResponse(response, true, Delegate);
        });

    this->vertices_->VerticesGetaddressbalanceGet(Request, delegate);
}

void UnrealApi::OnAlgorandGetaddressbalanceGetResponse(const Vertices::VerticesGetaddressbalanceGetResponse& response, bool bSucceed, const FAlgorandGetaddressbalanceGetDelegate& Delegate) const
{
    FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Successes", "I am on UnrealAPI"));
    Delegate.ExecuteIfBound(response);
}

}
}

#undef LOCTEXT_NAMESPACE