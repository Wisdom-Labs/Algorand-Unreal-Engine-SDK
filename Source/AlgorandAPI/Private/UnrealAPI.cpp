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
    
void UnrealApi::AlgorandRestoreWalletGet(const Vertices::VerticesRestoreWalletGetRequest& Request, const FAlgorandRestoreWalletGetDelegate& Delegate) const
{
    TSharedRef<Vertices::FVerticesRestoreWalletGetDelegate> delegatePtr(MakeShared<Vertices::FVerticesRestoreWalletGetDelegate>());
    
    delegatePtr->BindLambda([this, Delegate](const Vertices::VerticesRestoreWalletGetResponse& response) {
        OnAlgorandRestoreWalletGetResponse(response, Delegate);
    });
    vertices_->VerticesRestoreWalletGet( Request, delegatePtr.Get());
}
 
/**
 * @brief callback after restore wallet
 */
void UnrealApi::OnAlgorandRestoreWalletGetResponse(const Vertices::VerticesRestoreWalletGetResponse& response, const FAlgorandRestoreWalletGetDelegate& Delegate) const
{
    Delegate.ExecuteIfBound(response);
}
   
void UnrealApi::AlgorandInitializeNewWalletGet(const Vertices::VerticesInitializeNewWalletGetRequest& Request, const FAlgorandInitializeNewWalletGetDelegate& Delegate) const
{
    TSharedRef<Vertices::FVerticesInitializeNewWalletGetDelegate> delegatePtr(MakeShared<Vertices::FVerticesInitializeNewWalletGetDelegate>());
    
    delegatePtr->BindLambda([this, Delegate](const Vertices::VerticesInitializeNewWalletGetResponse& response) {
        OnAlgorandInitializeNewWalletGetResponse(response, Delegate);
    });
    vertices_->VerticesInitializeNewWalletGet( Request, delegatePtr.Get());
}

/**
 * @brief callback after initialize new wallet
 */
void UnrealApi::OnAlgorandInitializeNewWalletGetResponse(const Vertices::VerticesInitializeNewWalletGetResponse& response, const FAlgorandInitializeNewWalletGetDelegate& Delegate) const
{
    Delegate.ExecuteIfBound(response);
}
    
void UnrealApi::AlgorandGetBackupMnemonicPhraseGet(const Vertices::VerticesGetBackupMnemonicPhraseGetRequest& Request, const FAlgorandGetBackupMnemonicPhraseGetDelegate& Delegate) const
{
    TSharedRef<Vertices::FVerticesGetBackupMnemonicPhraseGetDelegate> delegatePtr(MakeShared<Vertices::FVerticesGetBackupMnemonicPhraseGetDelegate>());
    
    delegatePtr->BindLambda([this, Delegate](const Vertices::VerticesGetBackupMnemonicPhraseGetResponse& response) {
        OnAlgorandGetBackupMnemonicPhraseGetResponse(response, Delegate);
    });
    vertices_->VerticesGetBackupMnemonicPhraseGet( Request, delegatePtr.Get());
}

/**
 * @brief callback after get backup mnemonic phrase
 */
void UnrealApi::OnAlgorandGetBackupMnemonicPhraseGetResponse(const Vertices::VerticesGetBackupMnemonicPhraseGetResponse& response, const FAlgorandGetBackupMnemonicPhraseGetDelegate& Delegate) const
{
    Delegate.ExecuteIfBound(response);
}
    
void UnrealApi::AlgorandGenerateMnemonicsGet(const Vertices::VerticesGenerateMnemonicsGetRequest& Request, const FAlgorandGenerateMnemonicsGetDelegate& Delegate) const
{
    TSharedRef<Vertices::FVerticesGenerateMnemonicsGetDelegate> delegatePtr(MakeShared<Vertices::FVerticesGenerateMnemonicsGetDelegate>());
    
    delegatePtr->BindLambda([this, Delegate](const Vertices::VerticesGenerateMnemonicsGetResponse& response) {
        OnAlgorandGenerateMnemonicsGetResponse(response, Delegate);
    });
    vertices_->VerticesGenerateMnemonicsGet( Request, delegatePtr.Get());
}

/**
 * @brief callback after generate mnemonics
 */
void UnrealApi::OnAlgorandGenerateMnemonicsGetResponse(const Vertices::VerticesGenerateMnemonicsGetResponse& response, const FAlgorandGenerateMnemonicsGetDelegate& Delegate) const
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