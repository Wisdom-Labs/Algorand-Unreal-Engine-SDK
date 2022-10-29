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
            request_builders::buildGetBalanceRequest(FString("QTYBYPJVSPT7SXSJQ5CLH2C5EQXXWEBBCKUWZUCGJGOGTODHYG43WQQSCM")),
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

void UAlgorandUnrealManager::loadAccountInfo(const FLoadAccountInfoResponseReceivedDelegate& delegate,
    const FErrorReceivedDelegate& errorDelegate)
{
}

void UAlgorandUnrealManager::loadAccountInfo(TFunction<void(const TResult<int64>&)> callback)
{
    this->requestContextManager_
        .createContext<API::FAlgorandLoadaccountinfoGetDelegate,
        Vertices::VerticesLoadaccountinfoGetRequest>(
            request_builders::buildLoadAccountInfoRequest(FString("QTYBYPJVSPT7SXSJQ5CLH2C5EQXXWEBBCKUWZUCGJGOGTODHYG43WQQSCM")),
            std::bind(&API::AlgorandLoadaccountinfoGet, unrealApi_.Get(),
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

UWorld* UAlgorandUnrealManager::GetWorld() const
{
    return GetOuter()->GetWorld();
}

#undef LOCTEXT_NAMESPACE