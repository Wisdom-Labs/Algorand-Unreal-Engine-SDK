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

UAlgorandUnrealManager::UAlgorandUnrealManager()
    :myAlgoRpc("https://node.testnet.algoexplorerapi.io") , myAlgoPort(0) , myAlgoTokenHeader("")
{
    FString address;
    // create Vertices library
    vertices_ = MakeShared<algorand::vertices::VerticesSDK>();

    setAlgoRpc(myAlgoRpc);
    setAlgoPort(myAlgoPort);
    setAlgoTokenHeader(myAlgoTokenHeader);

    // Load existing pub address from Vertices SDK
    if (vertices_.IsValid()) {
        try
        {
            address = vertices_->load_pub_key();
        }
        catch (std::exception& ex)
        {
            address = !address.IsEmpty() ? address : "O6APBR3UNPVWH7ILBMCI6V53PDZAQQLMV47VKQWHH5753SQPRNDLSE7SWQ";        // default address
            FFormatNamedArguments Arguments;
            Arguments.Add(TEXT("MSG"), FText::FromString(ex.what()));
            FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Error", "👉 {MSG}"), Arguments));
        }
    }

    transactionBuilder_ = createTransactionBuilder(address);       
    // create unreal api modules
    unrealApi_ = MakeShared<algorand::api::UnrealApi>(vertices_);
}

UAlgorandUnrealManager* UAlgorandUnrealManager::createInstance(const FString& algoRpc, const FUInt64& algoPort, const FString& algoTokenHeader, UObject* outer)
{
    UAlgorandUnrealManager* manager = NewObject<UAlgorandUnrealManager>(outer);
    manager->setAlgoRpc(algoRpc);
    manager->setAlgoPort(algoPort);
    manager->setAlgoTokenHeader(algoTokenHeader);
        
    return manager;
}

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

void UAlgorandUnrealManager::generateWallet()
{
    this->requestContextManager_
        .createContext<API::FAlgorandGenerateWalletGetDelegate,
        Vertices::VerticesGenerateWalletGetRequest>(
            request_builders::buildGenerateWalletRequest(),
            std::bind(&API::AlgorandGenerateWalletGet, unrealApi_.Get(),
                std::placeholders::_1, std::placeholders::_2),
            std::bind(&UAlgorandUnrealManager::OnGenerateWalletCompleteCallback, this , std::placeholders::_1)
        );
}

void UAlgorandUnrealManager::OnGenerateWalletCompleteCallback(const Vertices::VerticesGenerateWalletGetResponse& response) {
    if (response.IsSuccessful()) {
        FString address = response.Address;
        GenerateWalletCallback.Broadcast(address);
        setAddress(address);
    }
    else {
        if (!ErrorDelegateCallback.IsBound()) {
            ErrorDelegateCallback.Broadcast(FError("ErrorDelegateCallback is not bound"));
        }
    }
}

void UAlgorandUnrealManager::getBalance()
{
    this->requestContextManager_
        .createContext<API::FAlgorandGetaddressbalanceGetDelegate,
        Vertices::VerticesGetaddressbalanceGetRequest>(
            request_builders::buildGetBalanceRequest(this->getAddress()),
            std::bind(&API::AlgorandGetaddressbalanceGet, unrealApi_.Get(),
                std::placeholders::_1, std::placeholders::_2),
            std::bind(&UAlgorandUnrealManager::OnGetBalanceCompleteCallback, this , std::placeholders::_1)
        );
}

void UAlgorandUnrealManager::OnGetBalanceCompleteCallback(const Vertices::VerticesGetaddressbalanceGetResponse& response) {
    if (response.IsSuccessful()) {
        uint64 balance = response.Amount;
        GetBalanceCallback.Broadcast(FUInt64(balance));
    }
    else {
        if (!ErrorDelegateCallback.IsBound()) {
            ErrorDelegateCallback.Broadcast(FError("ErrorDelegateCallback is not bound"));
        }
    }
}

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

void UAlgorandUnrealManager::OnSendPaymentTransactionCompleteCallback(const Vertices::VerticesPaymentTransactionGetResponse& response) {
    if (response.IsSuccessful()) {
        FString txID = response.txID;
        SendPaymentTransactionCallback.Broadcast(txID);
        FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Payment Transaction", "sent payment tx successfully."));
    }
    else {
        FFormatNamedArguments Arguments;
        Arguments.Add(TEXT("MSG"), FText::FromString(response.GetResponseString()));
        FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Error", "👉 {MSG}"), Arguments));
        
        if (!ErrorDelegateCallback.IsBound()) {
            ErrorDelegateCallback.Broadcast(FError("ErrorDelegateCallback is not bound"));
        }
    }
}

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

void UAlgorandUnrealManager::OnSendApplicationCallTransactionCompleteCallback(const Vertices::VerticesApplicationCallTransactionGetResponse& response) {
    if (response.IsSuccessful()) {
        FString txID = response.txID;
        SendApplicationCallTransactionCallback.Broadcast(txID);
    }
    else {
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