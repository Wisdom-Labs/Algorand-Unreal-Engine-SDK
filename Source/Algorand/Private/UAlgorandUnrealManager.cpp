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

UWorld* UAlgorandUnrealManager::GetWorld() const
{
    return GetOuter()->GetWorld();
}