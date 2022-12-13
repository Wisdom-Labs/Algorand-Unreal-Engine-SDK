// Copyright 2022, Wisdom Labs. All Rights Reserved

#include "UnrealApi.h"
#include "Misc/MessageDialog.h"

#define LOCTEXT_NAMESPACE "FVerticesModule"

namespace {
    using Vertices = algorand::vertices::VerticesSDK;
}

namespace algorand{
namespace api {

// create vertices with its instance from Algorand module
UnrealApi::UnrealApi(TSharedPtr<algorand::vertices::VerticesSDK>& vertices)
{
    vertices_ = vertices;
}

UnrealApi::~UnrealApi() {}
    
void UnrealApi::AlgorandGenerateWalletGet(const Vertices::VerticesGenerateWalletGetRequest& Request, const FAlgorandGenerateWalletGetDelegate& Delegate) const
{
    TSharedRef<Vertices::FVerticesGenerateWalletGetDelegate> delegatePtr(MakeShared<Vertices::FVerticesGenerateWalletGetDelegate>());
    
    delegatePtr->BindLambda([this, Delegate](const Vertices::VerticesGenerateWalletGetResponse& response) {
        OnAlgorandGenerateWalletGetResponse(response, Delegate);
    });
    vertices_->VerticesGenerateWalletGet( Request, delegatePtr.Get());
    
}

/**
 * @brief callback after generate wallet
 */
void UnrealApi::OnAlgorandGenerateWalletGetResponse(const Vertices::VerticesGenerateWalletGetResponse& response, const FAlgorandGenerateWalletGetDelegate& Delegate) const
{
    Delegate.ExecuteIfBound(response);
}

void UnrealApi::AlgorandGetaddressbalanceGet(const Vertices::VerticesGetaddressbalanceGetRequest& Request, const FAlgorandGetaddressbalanceGetDelegate& Delegate) const
{
    TSharedRef<Vertices::FVerticesGetaddressbalanceGetDelegate> delegatePtr(MakeShared<Vertices::FVerticesGetaddressbalanceGetDelegate>());
    
    delegatePtr->BindLambda([this, Delegate](const Vertices::VerticesGetaddressbalanceGetResponse& response) {
        OnAlgorandGetaddressbalanceGetResponse(response, Delegate);
    });
    vertices_->VerticesGetaddressbalanceGet( Request, delegatePtr.Get());
    
}

/**
 * @brief callback after get balance
 */
void UnrealApi::OnAlgorandGetaddressbalanceGetResponse(const Vertices::VerticesGetaddressbalanceGetResponse& response, const FAlgorandGetaddressbalanceGetDelegate& Delegate) const
{
    Delegate.ExecuteIfBound(response);
}

void UnrealApi::AlgorandPaymentTransactionGet(const Vertices::VerticesPaymentTransactionGetRequest& Request, const FAlgorandPaymentTransactionGetDelegate& Delegate) const
{
    TSharedRef<Vertices::FVerticesPaymentTransactionGetDelegate> delegatePtr(MakeShared<Vertices::FVerticesPaymentTransactionGetDelegate>());

    delegatePtr->BindLambda([this, Delegate](const Vertices::VerticesPaymentTransactionGetResponse& response) {
        OnAlgorandPaymentTransactionGetResponse(response, Delegate);
        });
    vertices_->VerticesPaymentTransactionGet(Request, delegatePtr.Get());
}

/**
 * @brief callback after payment tx
 */
void UnrealApi::OnAlgorandPaymentTransactionGetResponse(const Vertices::VerticesPaymentTransactionGetResponse& response, const FAlgorandPaymentTransactionGetDelegate& Delegate) const
{
    Delegate.ExecuteIfBound(response);
}

void UnrealApi::AlgorandApplicationCallTransactionGet(const Vertices::VerticesApplicationCallTransactionGetRequest& Request, const FAlgorandApplicationCallTransactionGetDelegate& Delegate) const
{
    TSharedRef<Vertices::FVerticesApplicationCallTransactionGetDelegate> delegatePtr(MakeShared<Vertices::FVerticesApplicationCallTransactionGetDelegate>());

    delegatePtr->BindLambda([this, Delegate](const Vertices::VerticesApplicationCallTransactionGetResponse& response) {
        OnAlgorandApplicationCallTransactionGetResponse(response, Delegate);
    });

    vertices_->VerticesApplicationCallTransactionGet(Request, delegatePtr.Get());
}

/**
 * @brief callback after application call tx
 */
void UnrealApi::OnAlgorandApplicationCallTransactionGetResponse(const Vertices::VerticesApplicationCallTransactionGetResponse& response, const FAlgorandApplicationCallTransactionGetDelegate& Delegate) const
{
    Delegate.ExecuteIfBound(response);
}

}
}

#undef LOCTEXT_NAMESPACE