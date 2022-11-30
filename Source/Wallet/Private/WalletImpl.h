#pragma once

#include "Wallet.h"

class WalletImpl : public Wallet
{
public:
    WalletImpl(const FString& address);

    /* Returns wallet address. */
    FString getAddress() const override;

    /* Set wallet address. */
    void setAddress(const FString& address_) override;

private:
    FString address;
};