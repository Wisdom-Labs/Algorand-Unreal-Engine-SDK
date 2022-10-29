#pragma once

#include "Wallet.h"

class WalletImpl : public Wallet
{
public:
    WalletImpl(const FString& address);
    
    FString getAddress() const override;

private:
    FString address;
};