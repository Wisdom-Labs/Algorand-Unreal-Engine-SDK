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

UnrealApi::UnrealApi(TSharedPtr<algorand::vertices::ThreadContextManager>& threadContextManager)
    : Url(TEXT("http://localhost"))
{
    threadContextManager_ = threadContextManager;
}

UnrealApi::~UnrealApi() {}

void UnrealApi::SetURL(const FString& InUrl)
{
    Url = InUrl;
}

void UnrealApi::AlgorandGetaddressbalanceGet(const Vertices::VerticesGetaddressbalanceGetRequest& Request, const FAlgorandGetaddressbalanceGetDelegate& Delegate) const
{
    // IsValid Endpoint Url is not set ,  check this

    this->threadContextManager_->createContext<Vertices::FVerticesGetaddressbalanceGetDelegate,
                                               Vertices::VerticesGetaddressbalanceGetRequest>(
            Request,
            std::bind(&Vertices::VerticesGetaddressbalanceGet, threadContextManager_->getVertices().Get(),
                std::placeholders::_1, std::placeholders::_2),
            [this, Delegate]
            (const auto& response) {
                OnAlgorandGetaddressbalanceGetResponse(response, true, Delegate);
            }
            );
    /*std::bind(&Vertices::VerticesGetaddressbalanceGet, threadContextManager_->getVertices().Get(),
        std::placeholders::_1, std::placeholders::_2),*/
}

void UnrealApi::OnAlgorandGetaddressbalanceGetResponse(const Vertices::VerticesGetaddressbalanceGetResponse& response, bool bSucceed, const FAlgorandGetaddressbalanceGetDelegate& Delegate) const
{
    FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Successes", "I am on UnrealAPI"));
    Delegate.ExecuteIfBound(response);
}

void UnrealApi::AlgorandLoadaccountinfoGet(const Vertices::VerticesLoadaccountinfoGetRequest& Request, const FAlgorandLoadaccountinfoGetDelegate& Delegate) const
{
    // IsValid Endpoint Url is not set ,  check this

    this->threadContextManager_->createContext<Vertices::FVerticesLoadaccountinfoGetDelegate,
        Vertices::VerticesLoadaccountinfoGetRequest>(
            Request,
            std::bind(&Vertices::VerticesLoadaccountinfoGet, threadContextManager_->getVertices().Get(),
                std::placeholders::_1, std::placeholders::_2),
            [this, Delegate]
            (const auto& response) {
                OnAlgorandLoadaccountinfoGetResponse(response, true, Delegate);
            }
    );
}

void UnrealApi::OnAlgorandLoadaccountinfoGetResponse(const Vertices::VerticesLoadaccountinfoGetResponse& response, bool bSucceed, const FAlgorandLoadaccountinfoGetDelegate& Delegate) const
{
    FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Successes", "I am on UnrealAPI Load Account Info"));
    Delegate.ExecuteIfBound(response);
}

}
}

#undef LOCTEXT_NAMESPACE