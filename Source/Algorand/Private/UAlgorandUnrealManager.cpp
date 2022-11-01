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
    threadContextManager_ = MakeShared<algorand::vertices::ThreadContextManager>();
    threadContextManager_->createVertices();

    transactionBuilder_ = createTransactionBuilder("O6APBR3UNPVWH7ILBMCI6V53PDZAQQLMV47VKQWHH5753SQPRNDLSE7SWQ");       // default address
    // create unreal api modules
    unrealApi_ = MakeShared<algorand::api::UnrealApi>(threadContextManager_);
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

void UAlgorandUnrealManager::getBalance(
    const FGetBalanceResponseReceivedDelegate& delegate,
    const FErrorReceivedDelegate& errorDelegate)
{

}

void UAlgorandUnrealManager::getBalance(
    TFunction<void(const TResult<int64>&)> callback)
{
    this->requestContextManager_
        .createContext<API::FAlgorandGetaddressbalanceGetDelegate,
                       Vertices::VerticesGetaddressbalanceGetRequest>(
            request_builders::buildGetBalanceRequest(this->getAddress()),
            std::bind(&API::AlgorandGetaddressbalanceGet, unrealApi_.Get(),
                std::placeholders::_1, std::placeholders::_2),
            [callback](const auto& response) {
                FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Successes", "Got Balance"));
                if (response.IsSuccessful()) {
                    int64 balance = response.Amount;
                    callback(result::ok(balance));
                }
                else {
                    callback(result::error<int64>(response.GetResponseString()));
                }
            });
}

void UAlgorandUnrealManager::sendPaymentTransaction(
    const FPaymentTransactionResponseReceivedDelegate& delegate,
    const FErrorReceivedDelegate& errorDelegate)
{
}

void UAlgorandUnrealManager::sendPaymentTransaction(const FString& receiverAddress,
                                                    const uint64_t& amount,
                                                    TFunction<void(const TResult<int64>&)> callback)
{
    this->requestContextManager_
        .createContext<API::FAlgorandPaymentTransactionGetDelegate,
        Vertices::VerticesPaymentTransactionGetRequest>(
            request_builders::buildPaymentTransactionRequest(this->getAddress(),
                                                             receiverAddress,
                                                             amount),
            std::bind(&API::AlgorandPaymentTransactionGet, unrealApi_.Get(),
                std::placeholders::_1, std::placeholders::_2),
            [callback](const auto& response) {
                FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Successes", "Payment Transaction"));
                if (response.IsSuccessful()) {
                    int64 balance = response.Amount;
                    callback(result::ok(balance));
                }
                else {
                    callback(result::error<int64>(response.GetResponseString()));
                }
            });
}

void UAlgorandUnrealManager::sendApplicationCallTransaction(
    const FApplicationCallTransactionResponseReceivedDelegate& delegate,
    const FErrorReceivedDelegate& errorDelegate)
{

}

void UAlgorandUnrealManager::sendApplicationCallTransaction(const uint64_t& app_ID,
                                                    TFunction<void(const TResult<int64>&)> callback)
{
    this->requestContextManager_
        .createContext<API::FAlgorandApplicationCallTransactionGetDelegate,
        Vertices::VerticesApplicationCallTransactionGetRequest>(
            request_builders::buildApplicationCallTransactionRequest(this->getAddress(),
                                                                     app_ID),
            std::bind(&API::AlgorandApplicationCallTransactionGet, unrealApi_.Get(),
                std::placeholders::_1, std::placeholders::_2),
            [callback](const auto& response) {
                FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Successes", "Payment Transaction"));
                if (response.IsSuccessful()) {
                    int64 balance = response.Amount;
                    callback(result::ok(balance));
                }
                else {
                    callback(result::error<int64>(response.GetResponseString()));
                }
            });
}

UWorld* UAlgorandUnrealManager::GetWorld() const
{
    return GetOuter()->GetWorld();
}

#undef LOCTEXT_NAMESPACE