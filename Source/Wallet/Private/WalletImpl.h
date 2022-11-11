#pragma once

#include "Wallet.h"

class WalletImpl : public Wallet
{
public:
    WalletImpl(const FString& address);
    
    FString getAddress() const override;
    void setAddress(const FString& address_) override;

private:
    FString address;
};