#pragma once

#include <CoreMinimal.h>

class TransactionBuilder;

BLOCKCHAIN_API TSharedPtr<TransactionBuilder>
createTransactionBuilder(const FString& address);