#pragma once

#include "TransactionBuilder.h"
#include <CoreMinimal.h>

#include "Wallet.h"

class TransactionBuilderImpl : public TransactionBuilder
{
public:
	TransactionBuilderImpl(const FString& address);
	virtual ~TransactionBuilderImpl() {}

	/* Returns payment address. */
	FString paymentAddress() const override;

	/* Set wallet address. */
	void setPaymentAddress(const FString& address) const override;

private:
	TSharedPtr<Wallet> wallet_;
};