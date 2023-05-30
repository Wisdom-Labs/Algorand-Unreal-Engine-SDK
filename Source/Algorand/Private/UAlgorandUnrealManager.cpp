// Copyright 2022, Wisdom Labs. All Rights Reserved

#include "UAlgorandUnrealManager.h"
#include "TransactionBuilder.h"
#include "TransactionBuilderFactory.h"
#include "RequestBuilders.h"
#include <functional>

#include "VerticesApiOperations.h"
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
    :myAlgoRpc("https://node.testnet.algoexplorerapi.io") , myAlgoPort(0) , myAlgoTokenHeader("")
{
    FString address;
    // create instance of Vertices library
    vertices_ = MakeShared<algorand::vertices::VerticesSDK>();

    setAlgoRpc(myAlgoRpc);
    setAlgoPort(myAlgoPort);
    setAlgoTokenHeader(myAlgoTokenHeader);

    transactionBuilder_ = createTransactionBuilder("");       
    // create instance of unreal api library
    unrealApi_ = MakeShared<algorand::api::UnrealApi>(vertices_);
}

// create instance of algorand manager using blueprint
UAlgorandUnrealManager* UAlgorandUnrealManager::createInstanceWithParams(const FString& algoRpc, const FUInt64& algoPort, const FString& algoTokenHeader, UObject* outer)
{
    UAlgorandUnrealManager* manager = NewObject<UAlgorandUnrealManager>(outer);
    
    manager->setAlgoRpc(algoRpc);
    manager->setAlgoPort(algoPort);
    manager->setAlgoTokenHeader(algoTokenHeader);
        
    return manager;
}

// create instance of algorand manager using blueprint
UAlgorandUnrealManager* UAlgorandUnrealManager::createInstance(UObject* outer)
{
    UAlgorandUnrealManager* manager = NewObject<UAlgorandUnrealManager>(outer);
        
    return manager;
}

/// set rpc info from algorand manager to vertices instance
void UAlgorandUnrealManager::setAlgoRpcInfo(const FString& algoRpc, const FUInt64& algoPort, const FString& algoTokenHeader)
{
    this->setAlgoRpc(algoRpc);
    this->setAlgoPort(algoPort);
    this->setAlgoTokenHeader(algoTokenHeader);
}

/// set rpc info from algorand manager to vertices instance

void UAlgorandUnrealManager::setAlgoRpc(const FString& algoRpc) {
    myAlgoRpc = algoRpc;
    vertices_->setAlgoRpc(myAlgoRpc);
}

void UAlgorandUnrealManager::setAlgoPort(const FUInt64& algoPort) {
    myAlgoPort = algoPort; 
    vertices_->setAlgoPort(algoPort.Value);
}

void UAlgorandUnrealManager::setAlgoTokenHeader(const FString& algoTokenHeader) {
    myAlgoTokenHeader = algoTokenHeader;
    vertices_->setAlgoTokenHeader(myAlgoTokenHeader);
}

/// get alogrand rpc information

FString UAlgorandUnrealManager::getAlgoRpc()
{
    return myAlgoRpc;
}

FUInt64 UAlgorandUnrealManager::getAlgoPort()
{
    return myAlgoPort;
}

FString UAlgorandUnrealManager::getAlgoTokenHeader()
{
    return myAlgoTokenHeader;
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
 * @brief create its context to send the request to unreal api for asset transfer tx
 */
void UAlgorandUnrealManager::sendAssetTransferTransaction(const FString& senderAddress, ,
                                                        const FString& receiverAddress,
                                                        const FUInt64& asset_ID,
                                                        const FUInt64& amount,
                                                        const FString& notes)
{
    this->requestContextManager_
        .createContext<API::FAlgorandAssetTransferTransactionGetDelegate,
        Vertices::VerticesAssetTransferTransactionGetRequest>(
            request_builders::buildAssetTransferTransactionRequest(this->getAddress(),
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
 * @brief get response from unreal api after payment TX and broadcast the result to binded functions
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
void UAlgorandUnrealManager::sendApplicationCallTransaction(const FUInt64& app_ID)
{
    this->requestContextManager_
        .createContext<API::FAlgorandApplicationCallTransactionGetDelegate,
        Vertices::VerticesApplicationCallTransactionGetRequest>(
            request_builders::buildApplicationCallTransactionRequest(this->getAddress(),
                                                                     app_ID),
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

UWorld* UAlgorandUnrealManager::GetWorld() const
{
    return GetOuter()->GetWorld();
}

#undef LOCTEXT_NAMESPACE