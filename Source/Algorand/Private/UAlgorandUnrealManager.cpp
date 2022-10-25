#include "UAlgorandUnrealManager.h"

#include "TransactionBuilder.h"
#include "TransactionBuilderFactory.h"

#include "RequestBuilders.h"

#include <functional>

namespace {
    namespace API = algorand::api::UnrealApi;
}

UAlograndUnrealManager::UAlgorandUnrealManager()
{

}

void UAlgorandUnrealManager::getBalance(
    const FGetBalanceResponseReceivedDelegate& delegate,
    const FErrorReceivedDelegate& errorDelegate)
{

}

void UAlgorandUnrealManager::getBalance(
    TFunction<void(const TResult<int64>&) callback>)
{
       
}

UWorld* UAlgorandUnrealManager::GetWorld() const
{
    return GetOuter()->GetWorld();
}