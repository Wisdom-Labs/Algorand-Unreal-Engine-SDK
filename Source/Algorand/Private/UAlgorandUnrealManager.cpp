#include "UAlgorandUnrealManager.h"

#include "TransactionBuilder.h"
#include "TransactionBuilderFactory.h"

#include "RequestBuilders.h"

#include <functional>

#define LOCTEXT_NAMESPACE "FVerticesModule"

namespace {
    using API = algorand::api::UnrealApi;
    using Vertices = algorand::vertices::VerticesSDK;
}

UAlgorandUnrealManager::UAlgorandUnrealManager()
{
    // create Vertices library
    vertices_ = MakeShared<algorand::vertices::VerticesSDK>();
    FString address;
    // Load existing pub address from Vertices SDK
    if (vertices_.IsValid()) {
        address = vertices_->load_pub_key();
    }

    address = !address.IsEmpty() ? address : "O6APBR3UNPVWH7ILBMCI6V53PDZAQQLMV47VKQWHH5753SQPRNDLSE7SWQ";        // default address

    transactionBuilder_ = createTransactionBuilder(address);       
    // create unreal api modules
    unrealApi_ = MakeShared<algorand::api::UnrealApi>(vertices_);
}

UAlgorandUnrealManager* UAlgorandUnrealManager::createInstance(UObject* outer)
{
    UAlgorandUnrealManager* manager = NewObject<UAlgorandUnrealManager>(outer);
        
    return manager;
}

FString UAlgorandUnrealManager::getAddress()
{
    return transactionBuilder_->paymentAddress();
}

void UAlgorandUnrealManager::setAddress(const FString& address)
{
    transactionBuilder_->setPaymentAddress(address);
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
    
    FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Successes", "Got Balance"));
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
                                                    const FUInt64& amount)
{
    this->requestContextManager_
        .createContext<API::FAlgorandPaymentTransactionGetDelegate,
        Vertices::VerticesPaymentTransactionGetRequest>(
            request_builders::buildPaymentTransactionRequest(this->getAddress(),
                                                             receiverAddress,
                                                             amount),
            std::bind(&API::AlgorandPaymentTransactionGet, unrealApi_.Get(),
                std::placeholders::_1, std::placeholders::_2),
            std::bind(&UAlgorandUnrealManager::OnSendPaymentTransactionCompleteCallback, this, std::placeholders::_1)
            );
}

void UAlgorandUnrealManager::OnSendPaymentTransactionCompleteCallback(const Vertices::VerticesPaymentTransactionGetResponse& response) {
    FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Successes", "Got Balance"));
    if (response.IsSuccessful()) {
        FString txID = response.txID;
        SendPaymentTransactionCallback.Broadcast(txID);
    }
    else {
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
    FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Successes", "Got Balance"));
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