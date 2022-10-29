#pragma once

#include "CoreMinimal.h"

class BLOCKCHAIN_API TransactionBuilder
{
public:
	virtual ~TransactionBuilder() {};

	/*virtual FString generateMnemonic() = 0;
	virtual void setMnemonic(const FString& mnemonic) = 0;*/
	
	virtual FString paymentAddress() const = 0;

};