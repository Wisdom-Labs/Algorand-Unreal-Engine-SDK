#pragma once

#include "Models/FUInt64.h"
#include "CoreMinimal.h"
#include "UAlgorandUnrealManager.h"

#include "AlgorandHandler.generated.h"

UCLASS(minimalapi)
class UAlgorandHandler : public UObject {
	GENERATED_BODY()

public:
	UAlgorandHandler();

	UPROPERTY()
		UAlgorandUnrealManager* algorandManager;

	UWorld* GetWorld() const override;
	
	UFUNCTION(BlueprintCallable, Category = "AlgorandHandler")
	void RunSomeLogic();

	UFUNCTION(BlueprintCallable, Category = "AlgorandHandler")
		void OnGetBalanceCallback(const FUInt64& amount);

	UFUNCTION(BlueprintCallable, Category = "AlgorandHandler")
		void OnSendPaymentTransactionCallback(const FString& txID);

	UFUNCTION(BlueprintCallable, Category = "AlgorandHandler")
		void OnSendApplicationCallTransactionCallback(const FString& txID);

	UFUNCTION(BlueprintCallable, Category = "AlgorandHandler")
		void OnGenerateWalletCallback(const FString& address);
};