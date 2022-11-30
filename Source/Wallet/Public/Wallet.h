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

	/* Returns wallet address. */
	virtual FString getAddress() const = 0;

	/* Set wallet address. */
	virtual void setAddress(const FString& address_) = 0;

};


/**
 * Create wallet with address encoded by base32
 * Returns instance pointer of Wallet class 
 * @param address account address, encoded by base32
 */

WALLET_API TSharedPtr<Wallet> createWallet(const FString& address);