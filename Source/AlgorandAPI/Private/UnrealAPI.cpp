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

void UnrealApi::AlgorandGenerateWalletGet(const Vertices::VerticesGenerateWalletGetRequest& Request, const FAlgorandGenerateWalletGetDelegate& Delegate) const
{
    // IsValid Endpoint Url is not set ,  check this

    TSharedRef<Vertices::FVerticesGenerateWalletGetDelegate> delegatePtr(MakeShared<Vertices::FVerticesGenerateWalletGetDelegate>());
    
    delegatePtr->BindLambda([this, Delegate](const Vertices::VerticesGenerateWalletGetResponse& response) {
        OnAlgorandGenerateWalletGetResponse(response, Delegate);
    });
    vertices_->VerticesGenerateWalletGet( Request, delegatePtr.Get());
    
}

void UnrealApi::OnAlgorandGenerateWalletGetResponse(const Vertices::VerticesGenerateWalletGetResponse& response, const FAlgorandGenerateWalletGetDelegate& Delegate) const
{
    Delegate.ExecuteIfBound(response);
}

void UnrealApi::AlgorandGetaddressbalanceGet(const Vertices::VerticesGetaddressbalanceGetRequest& Request, const FAlgorandGetaddressbalanceGetDelegate& Delegate) const
{
    // IsValid Endpoint Url is not set ,  check this

    TSharedRef<Vertices::FVerticesGetaddressbalanceGetDelegate> delegatePtr(MakeShared<Vertices::FVerticesGetaddressbalanceGetDelegate>());
    
    delegatePtr->BindLambda([this, Delegate](const Vertices::VerticesGetaddressbalanceGetResponse& response) {
        OnAlgorandGetaddressbalanceGetResponse(response, Delegate);
    });
    vertices_->VerticesGetaddressbalanceGet( Request, delegatePtr.Get());
    
}

void UnrealApi::OnAlgorandGetaddressbalanceGetResponse(const Vertices::VerticesGetaddressbalanceGetResponse& response, const FAlgorandGetaddressbalanceGetDelegate& Delegate) const
{
    Delegate.ExecuteIfBound(response);
}

void UnrealApi::AlgorandPaymentTransactionGet(const Vertices::VerticesPaymentTransactionGetRequest& Request, const FAlgorandPaymentTransactionGetDelegate& Delegate) const
{
    // IsValid Endpoint Url is not set ,  check this

    TSharedRef<Vertices::FVerticesPaymentTransactionGetDelegate> delegatePtr(MakeShared<Vertices::FVerticesPaymentTransactionGetDelegate>());

    delegatePtr->BindLambda([this, Delegate](const Vertices::VerticesPaymentTransactionGetResponse& response) {
        OnAlgorandPaymentTransactionGetResponse(response, Delegate);
        });
    vertices_->VerticesPaymentTransactionGet(Request, delegatePtr.Get());
}

void UnrealApi::OnAlgorandPaymentTransactionGetResponse(const Vertices::VerticesPaymentTransactionGetResponse& response, const FAlgorandPaymentTransactionGetDelegate& Delegate) const
{
    Delegate.ExecuteIfBound(response);
}

void UnrealApi::AlgorandApplicationCallTransactionGet(const Vertices::VerticesApplicationCallTransactionGetRequest& Request, const FAlgorandApplicationCallTransactionGetDelegate& Delegate) const
{
    // IsValid Endpoint Url is not set ,  check this
    TSharedRef<Vertices::FVerticesApplicationCallTransactionGetDelegate> delegatePtr(MakeShared<Vertices::FVerticesApplicationCallTransactionGetDelegate>());

    delegatePtr->BindLambda([this, Delegate](const Vertices::VerticesApplicationCallTransactionGetResponse& response) {
        OnAlgorandApplicationCallTransactionGetResponse(response, Delegate);
    });

    vertices_->VerticesApplicationCallTransactionGet(Request, delegatePtr.Get());
}

void UnrealApi::OnAlgorandApplicationCallTransactionGetResponse(const Vertices::VerticesApplicationCallTransactionGetResponse& response, const FAlgorandApplicationCallTransactionGetDelegate& Delegate) const
{
    Delegate.ExecuteIfBound(response);
}

}
}

#undef LOCTEXT_NAMESPACE