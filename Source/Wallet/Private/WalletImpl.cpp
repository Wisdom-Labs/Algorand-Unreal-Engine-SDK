#pragma once
#include "WalletImpl.h"
#include <string>

using namespace std; 

// Sets propertices
WalletImpl::WalletImpl(const FString& address)
	:address(address)
{
	
}

FString WalletImpl::getAddress() const
{
	return this->address;
}

void WalletImpl::setAddress(const FString& address_)
{
	address = address_;
}

/**
 * CAUTION: use only for testing & development purpose
 * This function has been not completed yet.
 */
TSharedPtr<Wallet> createWallet(const FString& address)
{
	return MakeShared<WalletImpl>(address);
}