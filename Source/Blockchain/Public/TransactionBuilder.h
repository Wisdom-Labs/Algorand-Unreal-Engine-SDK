#pragma once

#include "CoreMinimal.h"

class BLOCKCHAIN_API TransactionBuilder
{
public:
	virtual ~TransactionBuilder() {};
	
	/* Returns payment address. */
	virtual FString paymentAddress() const = 0;

	/* Set wallet address. */
	virtual void setPaymentAddress(const FString& address) const = 0;
};