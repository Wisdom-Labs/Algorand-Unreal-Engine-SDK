#pragma once

#include "Wallet.h"

/**
 * ToDo:
 * for secure private key and flexible wallet connect
 * add mnemonic private key
 * add wallet connect provider like following status
 * connecting, connected, disconnected, disconnecting, updated network, updated account
 */

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