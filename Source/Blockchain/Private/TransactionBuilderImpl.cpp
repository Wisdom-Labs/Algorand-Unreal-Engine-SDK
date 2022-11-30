#include "TransactionBuilderImpl.h"

#include <TransactionBuilderFactory.h>

// sets propertices.  wallet address
TransactionBuilderImpl::TransactionBuilderImpl(const FString& address)
	: wallet_(createWallet(address))
{
}

FString TransactionBuilderImpl::paymentAddress() const
{
	return this->wallet_->getAddress();
}

void TransactionBuilderImpl::setPaymentAddress(const FString& address) const
{
	return this->wallet_->setAddress(address);
}

/**
 * CAUTION: use only for testing & development purpose
 * This function has been not completed yet.
 */

TSharedPtr<TransactionBuilder>
createTransactionBuilder(const FString& address)
{
	return MakeShared<TransactionBuilderImpl>(address);
}