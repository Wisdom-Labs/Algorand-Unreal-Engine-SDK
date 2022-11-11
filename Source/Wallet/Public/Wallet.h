#pragma once

#include "include/vertices/vertices_types.h"
#include <CoreMinimal.h>

typedef struct {
	unsigned char private_key[ADDRESS_LENGTH];  //!< 32-bytes private key
	account_info_t* vtc_account;               //!< pointer to Vertices account data
} account_t;

class WALLET_API Wallet 
{
public:
	virtual ~Wallet() {};

	/*
	virtual std::string generateMnemonic() const = 0;

    virtual std::string getMnemonic() const = 0;
    virtual void setMnemonic(const std::string& mnemonic) = 0;
	*/

	virtual FString getAddress() const = 0;
	virtual void setAddress(const FString& address_) = 0;

};

WALLET_API TSharedPtr<Wallet> createWallet(const FString& address);