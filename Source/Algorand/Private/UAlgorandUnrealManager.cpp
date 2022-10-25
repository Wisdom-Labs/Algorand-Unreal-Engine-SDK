#include "UAlgorandUnrealManager.h"

#include "TransactionBuilder.h"
#include "TransactionBuilderFactory.h"

#include "RequestBuilders.h"

#include <functional>

namespace {
    using API = algorand::api::UnrealApi;
}

UAlgorandUnrealManager::UAlgorandUnrealManager()
{
    vertices_ = MakeShared<algorand::vertices::VerticesSDK>();
    unrealApi_ = MakeShared<algorand::api::UnrealApi>();
}

void UAlgorandUnrealManager::getBalance(
    const FGetBalanceResponseReceivedDelegate& delegate,
    const FErrorReceivedDelegate& errorDelegate)
{

}

void UAlgorandUnrealManager::getBalance(
    TFunction<void(const TResult<int64>&)> callback)
{
    API::AlgorandGetaddressbalanceGetRequest request = request_builders::buildGetBalanceRequest(FString("afwe"));
    this->requestContextManager_
        .createContext<API::FAlgorandGetaddressbalanceGetDelegate,
                       API::AlgorandGetaddressbalanceGetRequest>(
            request_builders::buildGetBalanceRequest(FString("afwe")),
            std::bind(&API::AlgorandGetaddressbalanceGet, unrealApi_.Get(),
                std::placeholders::_1, std::placeholders::_2),
            [callback](const auto& response) {
                if (response.IsSuccessful()) {
                    int64 balance = 5;
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