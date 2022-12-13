#pragma once

#include <CoreMinimal.h>

class TransactionBuilder;

/**
 * Create TX Builder with address encoded by base32
 * Returns instance pointer of TX Builder class 
 * @param address account address, encoded by base32
 */

BLOCKCHAIN_API TSharedPtr<TransactionBuilder>
createTransactionBuilder(const FString& address);