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
    vertices_ = MakeShared<algorand::vertices::VerticesSDK>();
    unrealApi_ = MakeShared<algorand::api::UnrealApi>(vertices_);
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
                    callback(result::error<int64>(FString("response error")));
                }
            });
}

UWorld* UAlgorandUnrealManager::GetWorld() const
{
    return GetOuter()->GetWorld();
}

#undef LOCTEXT_NAMESPACE