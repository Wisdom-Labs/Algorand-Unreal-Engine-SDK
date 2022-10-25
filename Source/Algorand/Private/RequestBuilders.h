#pragma once

#include "UnrealApiOperations.h"

namespace request_builders {
algorand::api::UnrealApi::AlgorandGetaddressbalanceGetRequest
buildGetBalanceRequest(const FString& address);
}