// Copyright 2022, Wisdom Labs. All Rights Reserved

#include "UAlgorandUnrealManager.h"
#include "TransactionBuilder.h"
#include "TransactionBuilderFactory.h"
#include "RequestBuilders.h"
#include <functional>

#include "VerticesApiOperations.h"
#include "Misc/MessageDialog.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMyAwesomeGame, Log, All);

#define LOCTEXT_NAMESPACE "FAlgorandManagerModule"

namespace {
    using API = algorand::api::UnrealApi;
    using Vertices = algorand::vertices::VerticesSDK;
}

// create vertices_ , transactionBuilder_ , unrealApi_ and load payment address 
// Sets default values
UAlgorandUnrealManager::UAlgorandUnrealManager()
    :myAlgodRpc("https://testnet-algorand.api.purestake.io/ps2") , myAlgodPort(0) , myAlgodTokenHeader("x-api-key:bLcs4F2SyGY0InF9M6Vl9piFTIZ8Ww281OjKXyE1"), myIndexerRpc("https://testnet-algorand.api.purestake.io/idx2"), myIndexerPort(0), myIndexerTokenHeader("x-api-key:bLcs4F2SyGY0InF9M6Vl9piFTIZ8Ww281OjKXyE1")
{
    FString address;
    // create instance of Vertices library
    vertices_ = MakeShared<algorand::vertices::VerticesSDK>();

    transactionBuilder_ = createTransactionBuilder("");       
    // create instance of unreal api library
    unrealApi_ = MakeShared<algorand::api::UnrealApi>(vertices_);

    setAlgodRpcInfo(myAlgodRpc, myAlgodPort, myAlgodTokenHeader);
    setIndexerRpcInfo(myIndexerRpc, myIndexerPort, myIndexerTokenHeader);        
}

// create instance of algorand manager using blueprint
UAlgorandUnrealManager* UAlgorandUnrealManager::createInstanceWithParams(const FString& algodRpc,
                                                                         const FUInt64& algodPort,
                                                                         const FString& algodTokenHeader,
                                                                         const FString& indexerRpc,
                                                                         const FUInt64& indexerPort,
                                                                         const FString& indexerTokenHeader,
                                                                         UObject* outer)
{
    UAlgorandUnrealManager* manager = NewObject<UAlgorandUnrealManager>(outer);

    manager->setAlgodRpcInfo(algodRpc, algodPort, algodTokenHeader);
    manager->setIndexerRpcInfo(indexerRpc, indexerPort, indexerTokenHeader);
    return manager;
}

// create instance of algorand manager using blueprint
UAlgorandUnrealManager* UAlgorandUnrealManager::createInstance(UObject* outer)
{
    UAlgorandUnrealManager* manager = NewObject<UAlgorandUnrealManager>(outer);
        
    return manager;
}

/// set rpc info from algorand manager to vertices instance
void UAlgorandUnrealManager::setAlgodRpcInfo(const FString& algodRpc, const FUInt64& algodPort, const FString& algodTokenHeader)
{
    myAlgodRpc = algodRpc;
    myAlgodPort = algodPort;
    myAlgodTokenHeader = algodTokenHeader;
    
    vertices_->setAlgodRpc(myAlgodRpc);
    vertices_->setAlgoPort(myAlgodPort);
    vertices_->setAlgoTokenHeader(myAlgodTokenHeader);

    unrealApi_->setAlgodRpcInfo(myAlgodRpc, myAlgodPort, myAlgodTokenHeader);
}

/// get algod rpc info from algorand manager
FRPCInfo UAlgorandUnrealManager::getAlgodRpcInfo()
{
    FRPCInfo rpc_info;
    rpc_info.url = myAlgodRpc;
    rpc_info.port = myAlgodPort;
    rpc_info.token = myAlgodTokenHeader;

    return rpc_info;
}

/// set rpc info from algorand manager to unrealapi instance
void UAlgorandUnrealManager::setIndexerRpcInfo(const FString& indexerRpc, const FUInt64& indexerPort, const FString& indexerTokenHeader)
{
    myIndexerRpc = indexerRpc;
    myIndexerPort = indexerPort;
    myIndexerTokenHeader = indexerTokenHeader;

    vertices_->setIndexerRpc(myIndexerRpc);
    
    unrealApi_->setIndexerRpcInfo(myIndexerRpc,myIndexerPort, myIndexerTokenHeader);
}

/// get indexer rpc info from algorand manager
FRPCInfo UAlgorandUnrealManager::getIndexerRpcInfo()
{
    FRPCInfo rpc_info;
    rpc_info.url = myIndexerRpc;
    rpc_info.port = myIndexerPort;
    rpc_info.token = myIndexerTokenHeader;

    return rpc_info;
}

/// get algod rpc net info 
FString UAlgorandUnrealManager::getAlgodRpcNet()
{
    if(myAlgodRpc.Contains("mainnet"))
        return "MainNet";
    if(myAlgodRpc.Contains("testnet"))
        return "TestNet";
    if(myAlgodRpc.Contains("devnet"))
        return "DevNet";
    return "LocalNet";
}

/// get indexer rpc net info 
FString UAlgorandUnrealManager::getIndexerRpcNet()
{
    if(myIndexerRpc.Contains("mainnet"))
        return "MainNet";
    if(myIndexerRpc.Contains("testnet"))
        return "TestNet";
    if(myIndexerRpc.Contains("devnet"))
        return "DevNet";
    return "LocalNet";
}

FString UAlgorandUnrealManager::getAddress()
{
    return transactionBuilder_->paymentAddress();
}

void UAlgorandUnrealManager::setAddress(const FString& address)
{
    transactionBuilder_->setPaymentAddress(address);
}

/**
 * @brief create its context to send the request to unreal api for restore wallet
 */
 void UAlgorandUnrealManager::restoreWallet(const FString& mnemonics)
{
    this->requestContextManager_
        .createContext<API::FAlgorandRestoreWalletGetDelegate,
        Vertices::VerticesRestoreWalletGetRequest>(
            request_builders::buildRestoreWalletRequest(mnemonics),
            std::bind(&API::AlgorandRestoreWalletGet, unrealApi_.Get(),
                std::placeholders::_1, std::placeholders::_2),
            std::bind(&UAlgorandUnrealManager::OnRestoreWalletCompleteCallback, this , std::placeholders::_1)
        );
}

/**
 * @brief get response from unreal api after restore wallet and broadcast the result to binded functions
 */
void UAlgorandUnrealManager::OnRestoreWalletCompleteCallback(const Vertices::VerticesRestoreWalletGetResponse& response) {
    if (response.IsSuccessful()) {
        FString output = response.output;
        setAddress(output);
        RestoreWalletCallback.Broadcast(output);
    }
    else {
        if (!ErrorDelegateCallback.IsBound()) {
            ErrorDelegateCallback.Broadcast(FError("ErrorDelegateCallback is not bound"));
        }
    }
}

/**
 * @brief create its context to send the request to unreal api for initialize new wallet
 */
 void UAlgorandUnrealManager::initializeNewWallet()
{
    this->requestContextManager_
        .createContext<API::FAlgorandInitializeNewWalletGetDelegate,
        Vertices::VerticesInitializeNewWalletGetRequest>(
            request_builders::buildInitializeNewWalletRequest(),
            std::bind(&API::AlgorandInitializeNewWalletGet, unrealApi_.Get(),
                std::placeholders::_1, std::placeholders::_2),
            std::bind(&UAlgorandUnrealManager::OnInitializeNewWalletCompleteCallback, this , std::placeholders::_1)
        );
}

/**
 * @brief get response from unreal api after initialize new wallet and broadcast the result to binded functions
 */
void UAlgorandUnrealManager::OnInitializeNewWalletCompleteCallback(const Vertices::VerticesInitializeNewWalletGetResponse& response) {
    if (response.IsSuccessful()) {
        FString output = response.output;
        setAddress(output);
        InitializeNewWalletCallback.Broadcast(output);
    }
    else {
        if (!ErrorDelegateCallback.IsBound()) {
            ErrorDelegateCallback.Broadcast(FError("ErrorDelegateCallback is not bound"));
        }
    }
}

/**
 * @brief create its context to send the request to unreal api for get backup mnemonic phrase
 */
 void UAlgorandUnrealManager::getBackupMnemonicPhrase()
{
    this->requestContextManager_
        .createContext<API::FAlgorandGetBackupMnemonicPhraseGetDelegate,
        Vertices::VerticesGetBackupMnemonicPhraseGetRequest>(
            request_builders::buildGetBackupMnemonicPhraseRequest(),
            std::bind(&API::AlgorandGetBackupMnemonicPhraseGet, unrealApi_.Get(),
                std::placeholders::_1, std::placeholders::_2),
            std::bind(&UAlgorandUnrealManager::OnGetBackupMnemonicPhraseCompleteCallback, this , std::placeholders::_1)
        );
}

/**
 * @brief get response from unreal api after get backup mnemonic phrase and broadcast the result to binded functions
 */
void UAlgorandUnrealManager::OnGetBackupMnemonicPhraseCompleteCallback(const Vertices::VerticesGetBackupMnemonicPhraseGetResponse& response) {
    if (response.IsSuccessful()) {
        FString output = response.output;
        GetBackupMnemonicPhraseCallback.Broadcast(output);
    }
    else {
        if (!ErrorDelegateCallback.IsBound()) {
            ErrorDelegateCallback.Broadcast(FError("ErrorDelegateCallback is not bound"));
        }
    }
}

/**
 * @brief create its context to send the request to unreal api for generate mnemonics
 */
 void UAlgorandUnrealManager::generateMnemonics()
{
    this->requestContextManager_
        .createContext<API::FAlgorandGenerateMnemonicsGetDelegate,
        Vertices::VerticesGenerateMnemonicsGetRequest>(
            request_builders::buildGenerateMnemonicsRequest(),
            std::bind(&API::AlgorandGenerateMnemonicsGet, unrealApi_.Get(),
                std::placeholders::_1, std::placeholders::_2),
            std::bind(&UAlgorandUnrealManager::OnGenerateMnemonicsCompleteCallback, this , std::placeholders::_1)
        );
}

/**
 * @brief get response from unreal api after generate mnemonics and broadcast the result to binded functions
 */
void UAlgorandUnrealManager::OnGenerateMnemonicsCompleteCallback(const Vertices::VerticesGenerateMnemonicsGetResponse& response) {
    if (response.IsSuccessful()) {
        FString output = response.output;
        GenerateMnemonicsCallback.Broadcast(output);
    }
    else {
        if (!ErrorDelegateCallback.IsBound()) {
            ErrorDelegateCallback.Broadcast(FError("ErrorDelegateCallback is not bound"));
        }
    }
}

/**
 * @brief create its context to send the request to unreal api for get balance
 */
void UAlgorandUnrealManager::getBalance(const FString& address)
{
    this->requestContextManager_
        .createContext<API::FAlgorandGetaddressbalanceGetDelegate,
        Vertices::VerticesGetaddressbalanceGetRequest>(
            request_builders::buildGetBalanceRequest(address),
            std::bind(&API::AlgorandGetaddressbalanceGet, unrealApi_.Get(),
                std::placeholders::_1, std::placeholders::_2),
            std::bind(&UAlgorandUnrealManager::OnGetBalanceCompleteCallback, this , std::placeholders::_1)
        );
}

/**
 * @brief get response from unreal api after get balance and broadcast the result to binded functions
 */
void UAlgorandUnrealManager::OnGetBalanceCompleteCallback(const Vertices::VerticesGetaddressbalanceGetResponse& response) {
    if (response.IsSuccessful()) {
        uint64 balance = response.Amount;
        
        GetBalanceCallback.Broadcast(FUInt64(balance));
        FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Get Balance Action", "Got Balance successfully."));

        if(balance < 1000)
        {
            FFormatNamedArguments Arguments;
            Arguments.Add(TEXT("Address"), FText::FromString(getAddress()));
            FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Warning", "Go to https://bank.testnet.algorand.network/, dispense Algos to: {Address}"), Arguments));
        }
    }
    else {
        FFormatNamedArguments Arguments;
        Arguments.Add(TEXT("MSG"), FText::FromString(response.GetResponseString()));
        FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Error", "{MSG}"), Arguments));
        
        if (!ErrorDelegateCallback.IsBound()) {
            ErrorDelegateCallback.Broadcast(FError("ErrorDelegateCallback is not bound"));
        }
    }
}

/**
 * @brief create its context to send the request to unreal api for payment tx
 */
void UAlgorandUnrealManager::sendPaymentTransaction(const FString& receiverAddress,
                                                    const FUInt64& amount,
                                                    const FString& notes)
{
    this->requestContextManager_
        .createContext<API::FAlgorandPaymentTransactionGetDelegate,
        Vertices::VerticesPaymentTransactionGetRequest>(
            request_builders::buildPaymentTransactionRequest(this->getAddress(),
                                                             receiverAddress,
                                                             amount,
                                                             notes),
            std::bind(&API::AlgorandPaymentTransactionGet, unrealApi_.Get(),
                std::placeholders::_1, std::placeholders::_2),
            std::bind(&UAlgorandUnrealManager::OnSendPaymentTransactionCompleteCallback, this, std::placeholders::_1)
            );
}

/**
 * @brief get response from unreal api after payment TX and broadcast the result to binded functions
 */
void UAlgorandUnrealManager::OnSendPaymentTransactionCompleteCallback(const Vertices::VerticesPaymentTransactionGetResponse& response) {
    if (response.IsSuccessful()) {
        FString txID = response.txID;
        SendPaymentTransactionCallback.Broadcast(txID);
        FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Payment Transaction", "sent payment tx successfully."));
    }
    else {
        FFormatNamedArguments Arguments;
        Arguments.Add(TEXT("MSG"), FText::FromString(response.GetResponseString()));
        FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Error", "{MSG}"), Arguments));
        
        if (!ErrorDelegateCallback.IsBound()) {
            ErrorDelegateCallback.Broadcast(FError("ErrorDelegateCallback is not bound"));
        }
    }
}

/**
 * @brief create its context to send the request to unreal api for asset config tx
 */
void UAlgorandUnrealManager::sendAssetConfigTransaction(const FString& manager,
                                                        const FString& reserve,
                                                        const FString& freeze,
                                                        const FString& clawback,
                                                        const FUInt64& asset_id,
                                                        const FUInt64& total,
                                                        const FUInt64& decimals,
                                                        const FString& isFrozen,
                                                        const FString& unit_name,
                                                        const FString& asset_name,
                                                        const FString& url,
                                                        const FString& notes)
{
    this->requestContextManager_
        .createContext<API::FAlgorandAssetConfigTransactionGetDelegate,
        Vertices::VerticesAssetConfigTransactionGetRequest>(
            request_builders::buildAssetConfigTransactionRequest(this->getAddress(),
                                                                 manager,
                                                                 reserve,
                                                                 freeze,
                                                                 clawback,
                                                                 asset_id,
                                                                 total,
                                                                 decimals,
                                                                 isFrozen,
                                                                 unit_name,
                                                                 asset_name,
                                                                 url,
                                                                 notes),
            std::bind(&API::AlgorandAssetConfigTransactionGet, unrealApi_.Get(),
                std::placeholders::_1, std::placeholders::_2),
            std::bind(&UAlgorandUnrealManager::OnSendAssetConfigTransactionCompleteCallback, this, std::placeholders::_1)
            );
}

/**
 * @brief create its context to send the request to unreal api for asset transfer tx
 */
void UAlgorandUnrealManager::sendAssetTransferTransaction(const FString& senderAddress,
                                                        const FString& receiverAddress,
                                                        const FUInt64& asset_ID,
                                                        const FString& amount,
                                                        const FString& notes)
{
    this->requestContextManager_
        .createContext<API::FAlgorandAssetTransferTransactionGetDelegate,
        Vertices::VerticesAssetTransferTransactionGetRequest>(
            request_builders::buildAssetTransferTransactionRequest(senderAddress,
                                                             receiverAddress,
                                                             asset_ID,
                                                             amount,
                                                             notes),
            std::bind(&API::AlgorandAssetTransferTransactionGet, unrealApi_.Get(),
                std::placeholders::_1, std::placeholders::_2),
            std::bind(&UAlgorandUnrealManager::OnSendAssetTransferTransactionCompleteCallback, this, std::placeholders::_1)
            );
}

/**
 * @brief get response from unreal api after asset config TX and broadcast the result to binded functions
 */
void UAlgorandUnrealManager::OnSendAssetConfigTransactionCompleteCallback(const Vertices::VerticesAssetConfigTransactionGetResponse& response) {
    if (response.IsSuccessful()) {
        FString TxId = response.txID;
        uint64 AssetId = response.assetID;
        SendAssetConfigTransactionCallback.Broadcast(TxId, FUInt64(AssetId));
        FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Asset Config Transaction", "sent asset config tx successfully."));
    }
    else {
        FFormatNamedArguments Arguments;
        Arguments.Add(TEXT("MSG"), FText::FromString(response.GetResponseString()));
        FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Error", "{MSG}"), Arguments));
        
        if (!ErrorDelegateCallback.IsBound()) {
            ErrorDelegateCallback.Broadcast(FError("ErrorDelegateCallback is not bound"));
        }
    }
}

/**
 * @brief get response from unreal api after asset config TX and broadcast the result to binded functions
 */
void UAlgorandUnrealManager::OnSendAssetTransferTransactionCompleteCallback(const Vertices::VerticesAssetTransferTransactionGetResponse& response) {
    if (response.IsSuccessful()) {
        FString txID = response.txID;
        SendAssetTransferTransactionCallback.Broadcast(txID);
        FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Asset Transfer Transaction", "sent asset transfer tx successfully."));
    }
    else {
        FFormatNamedArguments Arguments;
        Arguments.Add(TEXT("MSG"), FText::FromString(response.GetResponseString()));
        FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Error", "{MSG}"), Arguments));
        
        if (!ErrorDelegateCallback.IsBound()) {
            ErrorDelegateCallback.Broadcast(FError("ErrorDelegateCallback is not bound"));
        }
    }
}

/**
 * @brief create its context to send the request to unreal api for application call TX
 */
void UAlgorandUnrealManager::sendApplicationCallTransaction(const FUInt64& app_ID, const TArray<FAppArg>& app_args, const EAppOnCompleteTX& app_complete_tx)
{
    this->requestContextManager_
        .createContext<API::FAlgorandApplicationCallTransactionGetDelegate,
        Vertices::VerticesApplicationCallTransactionGetRequest>(
            request_builders::buildApplicationCallTransactionRequest(this->getAddress(),
                                                                     app_ID,
                                                                     app_args,
                                                                     app_complete_tx),
            std::bind(&API::AlgorandApplicationCallTransactionGet, unrealApi_.Get(),
                std::placeholders::_1, std::placeholders::_2),
            std::bind(&UAlgorandUnrealManager::OnSendApplicationCallTransactionCompleteCallback, this, std::placeholders::_1)
            );
}

/**
 * @brief get response from unreal api after application call TX and broadcast the result to binded functions
 */
void UAlgorandUnrealManager::OnSendApplicationCallTransactionCompleteCallback(const Vertices::VerticesApplicationCallTransactionGetResponse& response) {
    if (response.IsSuccessful()) {
        FString txID = response.txID;
        SendApplicationCallTransactionCallback.Broadcast(txID);
        FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Application Transaction", "sent application tx successfully."));
    }
    else {
        FFormatNamedArguments Arguments;
        Arguments.Add(TEXT("MSG"), FText::FromString(response.GetResponseString()));
        FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Error", "{MSG}"), Arguments));
        
        if (!ErrorDelegateCallback.IsBound()) {
            ErrorDelegateCallback.Broadcast(FError("ErrorDelegateCallback is not bound"));
        }
    }
}

/**
 * @brief create its context to send the request to unreal api for arc asset details
 */
void UAlgorandUnrealManager::fetchArcAssetDetails(const FUInt64& asset_ID)
{
    this->requestContextManager_
        .createContext<API::FAlgorandArcAssetDetailsGetDelegate,
        Vertices::VerticesArcAssetDetailsGetRequest>(
            request_builders::buildArcAssetDetailsRequest(asset_ID),
            std::bind(&API::AlgorandArcAssetDetailsGet, unrealApi_.Get(),
                std::placeholders::_1, std::placeholders::_2),
            std::bind(&UAlgorandUnrealManager::OnFetchArcAssetDetailsCompleteCallback, this, std::placeholders::_1)
            );
}

/**
 * @brief get response from unreal api after fetching arc asset details and broadcast the result to binded functions
 */
void UAlgorandUnrealManager::OnFetchArcAssetDetailsCompleteCallback(const Vertices::VerticesArcAssetDetailsGetResponse& response) {
    if (response.IsSuccessful()) {
        FArcAssetDetails arcNft(response);
        FetchArcAssetDetailsCallback.Broadcast(arcNft);
        FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Arc Asset Details", "sent request to fetch details of arc asset successfully."));
    }
    else {
        FFormatNamedArguments Arguments;
        Arguments.Add(TEXT("MSG"), FText::FromString(response.GetResponseString()));
        FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Error", "{MSG}"), Arguments));
        
        if (!ErrorDelegateCallback.IsBound()) {
            ErrorDelegateCallback.Broadcast(FError("ErrorDelegateCallback is not bound"));
        }
    }
}

/**
 * @brief create its context to send the request to unreal api for account information
 */
void UAlgorandUnrealManager::fetchAccountInformation(const FString& address)
{
    this->requestContextManager_
        .createContext<API::FAlgorandAccountInformationGetDelegate,
        Vertices::VerticesAccountInformationGetRequest>(
            request_builders::buildAccountInformationRequest(address),
            std::bind(&API::AlgorandAccountInformationGet, unrealApi_.Get(),
                std::placeholders::_1, std::placeholders::_2),
            std::bind(&UAlgorandUnrealManager::OnFetchAccountInformationCompleteCallback, this, std::placeholders::_1)
            );
}

/**
 * @brief get response from unreal api after fetching arc asset details and broadcast the result to binded functions
 */
void UAlgorandUnrealManager::OnFetchAccountInformationCompleteCallback(const Vertices::VerticesAccountInformationGetResponse& response) {
    if (response.IsSuccessful()) {
        FetchAccountInformationCallback.Broadcast(response.assetIDs, response.assetNames);
        FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Account Information", "sent request to fetch account info successfully."));
    }
    else {
        FFormatNamedArguments Arguments;
        Arguments.Add(TEXT("MSG"), FText::FromString(response.GetResponseString()));
        FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Error", "{MSG}"), Arguments));
        
        if (!ErrorDelegateCallback.IsBound()) {
            ErrorDelegateCallback.Broadcast(FError("ErrorDelegateCallback is not bound"));
        }
    }
}

UWorld* UAlgorandUnrealManager::GetWorld() const
{
    return GetOuter()->GetWorld();
}

#undef LOCTEXT_NAMESPACE