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

TSharedPtr<Wallet> createWallet(const FString& address)
{
		return MakeShared<WalletImpl>(address);
}