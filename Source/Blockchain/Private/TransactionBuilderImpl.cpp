#include "TransactionBuilderImpl.h"

#include <TransactionBuilderFactory.h>

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

TSharedPtr<TransactionBuilder>
createTransactionBuilder(const FString& address)
{
	return MakeShared<TransactionBuilderImpl>(address);
}