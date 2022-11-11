#pragma once
#include "WalletImpl.h"
#include <string>

using namespace std; 

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

TSharedPtr<Wallet> createWallet(const FString& address)
{
	return MakeShared<WalletImpl>(address);
}