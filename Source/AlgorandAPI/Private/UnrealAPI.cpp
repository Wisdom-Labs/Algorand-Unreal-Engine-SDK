// Copyright 2022, Wisdom Labs. All Rights Reserved

#include "UnrealApi.h"
#include "ArcResponseBuilders.h"
#include "VerticesApiOperations.h"
#include "Arc/Arc03.h"
#include "Arc/Arc19.h"
#include "Arc/Arc69.h"
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

void UnrealApi::setAlgodRpcInfo(const FString& algodRpc, const uint64_t& algodPort, const FString& algodTokenHeader_) {
    myAlgodRpc = algodRpc;
    myAlgodPort = algodPort;
    myAlgodTokenHeader = algodTokenHeader_;
}

void UnrealApi::setIndexerRpcInfo(const FString& indexerRpc, const uint64_t& indexerPort, const FString& indexerTokenHeader) {
    myIndexerRpc = indexerRpc;
    myIndexerPort = indexerPort;
    myIndexerTokenHeader = indexerTokenHeader;
}
    
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

void UnrealApi::AlgorandAssetConfigTransactionGet(const Vertices::VerticesAssetConfigTransactionGetRequest& Request, const FAlgorandAssetConfigTransactionGetDelegate& Delegate) const
{
    TSharedRef<Vertices::FVerticesAssetConfigTransactionGetDelegate> delegatePtr(MakeShared<Vertices::FVerticesAssetConfigTransactionGetDelegate>());

    delegatePtr->BindLambda([this, Delegate](const Vertices::VerticesAssetConfigTransactionGetResponse& response) {
        OnAlgorandAssetConfigTransactionGetResponse(response, Delegate);
        });
    vertices_->VerticesAssetConfigTransactionGet(Request, delegatePtr.Get());
}

/**
 * @brief callback after asset config tx
 */
void UnrealApi::OnAlgorandAssetConfigTransactionGetResponse(const Vertices::VerticesAssetConfigTransactionGetResponse& response, const FAlgorandAssetConfigTransactionGetDelegate& Delegate) const
{
    Delegate.ExecuteIfBound(response);
}

void UnrealApi::AlgorandAssetTransferTransactionGet(const Vertices::VerticesAssetTransferTransactionGetRequest& Request, const FAlgorandAssetTransferTransactionGetDelegate& Delegate) const
{
    TSharedRef<Vertices::FVerticesAssetTransferTransactionGetDelegate> delegatePtr(MakeShared<Vertices::FVerticesAssetTransferTransactionGetDelegate>());

    delegatePtr->BindLambda([this, Delegate](const Vertices::VerticesAssetTransferTransactionGetResponse& response) {
        OnAlgorandAssetTransferTransactionGetResponse(response, Delegate);
        });
    vertices_->VerticesAssetTransferTransactionGet(Request, delegatePtr.Get());
}

/**
 * @brief callback after asset transfer tx
 */
void UnrealApi::OnAlgorandAssetTransferTransactionGetResponse(const Vertices::VerticesAssetTransferTransactionGetResponse& response, const FAlgorandAssetTransferTransactionGetDelegate& Delegate) const
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

void UnrealApi::AlgorandArcAssetDetailsGet(const Vertices::VerticesArcAssetDetailsGetRequest& Request, const FAlgorandArcAssetDetailsGetDelegate& Delegate) const
{
    AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Request, Delegate]()
    {   
        TSharedRef<ArcResponseBuilders::FAPIArcAssetDetailsGetDelegate> delegatePtr(MakeShared<ArcResponseBuilders::FAPIArcAssetDetailsGetDelegate>());

        ArcBase temp_arc(myIndexerRpc, myIndexerPort, myIndexerTokenHeader);
        temp_arc.from_asset(Request.asset_ID.GetValue());
        
        Arc03 arc03_data(temp_arc.asset, myIndexerRpc, myIndexerPort, myIndexerTokenHeader);
        if(arc03_data.IsVerify())
        {
            auto param_url = StringCast<ANSICHAR>(*(arc03_data.asset.params.url));
            std::string s_url = param_url.Get();
            arc03_data.from_asset_url(s_url);
            
            delegatePtr->BindLambda([this, Delegate](const Vertices::VerticesArcAssetDetailsGetResponse& response) {
                OnAlgorandArcAssetDetailsGetResponse(response, Delegate);
            });

            ArcResponseBuilders::buildArcAssetDetailsResponse(arc03_data, delegatePtr.Get());
            return;
        }
        
        Arc19 arc19_data(temp_arc.asset, myIndexerRpc, myIndexerPort, myIndexerTokenHeader);
        if(arc19_data.IsVerify())
        {
            arc19_data.ParseASAUrl();
            arc19_data.from_temp_ipfs();

            delegatePtr->BindLambda([this, Delegate](const Vertices::VerticesArcAssetDetailsGetResponse& response) {
                OnAlgorandArcAssetDetailsGetResponse(response, Delegate);
            });

            ArcResponseBuilders::buildArcAssetDetailsResponse(arc19_data, delegatePtr.Get());
            return;
        }
        
        Arc69 arc69_data(temp_arc.asset, myIndexerRpc, myIndexerPort, myIndexerTokenHeader);
        if(arc69_data.IsVerify())
        {
            auto tx_note = StringCast<ANSICHAR>(*(arc69_data.tx.note));
            std::string s_tx_note = tx_note.Get();
            arc69_data.from_tx_note(s_tx_note);

            delegatePtr->BindLambda([this, Delegate](const Vertices::VerticesArcAssetDetailsGetResponse& response) {
                OnAlgorandArcAssetDetailsGetResponse(response, Delegate);
            });

            ArcResponseBuilders::buildArcAssetDetailsResponse(arc69_data, delegatePtr.Get());
            return;
        }

        Vertices::VerticesArcAssetDetailsGetResponse response;
        response.SetSuccessful(false);
        response.SetResponseString("This arc asset doesn't have correct type.");
        OnAlgorandArcAssetDetailsGetResponse(response, Delegate);
        AsyncTask(ENamedThreads::GameThread, [Delegate, response]()
        {
            Delegate.ExecuteIfBound(response);
        });
    });
}

/**
 * @brief callback after arc asset details
 */
void UnrealApi::OnAlgorandArcAssetDetailsGetResponse(const Vertices::VerticesArcAssetDetailsGetResponse& response, const FAlgorandArcAssetDetailsGetDelegate& Delegate) const
{
    Delegate.ExecuteIfBound(response);
}

void UnrealApi::AlgorandAccountInformationGet(const Vertices::VerticesAccountInformationGetRequest& Request, const FAlgorandAccountInformationGetDelegate& Delegate) const
{
    AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Request, Delegate]()
    {   
        TSharedRef<ArcResponseBuilders::FAPIAccountInfoGetDelegate> delegatePtr(MakeShared<ArcResponseBuilders::FAPIAccountInfoGetDelegate>());

        if ( Request.address.GetValue().Len() == 58 )
        {
            AccountAsset accountInfo(myAlgodRpc, myAlgodPort, myAlgodTokenHeader);
            accountInfo.getInformation(Request.address.GetValue());
            
            delegatePtr->BindLambda([this, Delegate](const Vertices::VerticesAccountInformationGetResponse& response) {
                OnAlgorandAccountInformationGetResponse(response, Delegate);
            });

            ArcResponseBuilders::buildAccountInformationResponse(accountInfo, delegatePtr.Get());   
        }
        else
        {
            Vertices::VerticesAccountInformationGetResponse response;
            response.SetSuccessful(false);
            response.SetResponseString("Address Length is invalid.");
            
            AsyncTask(ENamedThreads::GameThread, [Delegate, response]()
		    {
			    Delegate.ExecuteIfBound(response);
		    });
        }
    });
}

/**
 * @brief callback after arc asset details
 */
void UnrealApi::OnAlgorandAccountInformationGetResponse(const Vertices::VerticesAccountInformationGetResponse& response, const FAlgorandAccountInformationGetDelegate& Delegate) const
{
    Delegate.ExecuteIfBound(response);
}

}
}

#undef LOCTEXT_NAMESPACE