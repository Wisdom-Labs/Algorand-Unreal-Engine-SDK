#pragma once

#include "TransactionBuilder.h"
#include <CoreMinimal.h>

#include "Wallet.h"

class TransactionBuilderImpl : public TransactionBuilder
{
public:
	TransactionBuilderImpl(const FString& address);
	virtual ~TransactionBuilderImpl() {}

	FString paymentAddress() const override;

private:
	TSharedPtr<Wallet> wallet_;
};