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

TSharedPtr<TransactionBuilder>
createTransactionBuilder(const FString& address)
{
	return MakeShared<TransactionBuilderImpl>(address);
}