#pragma once

#include "Models/FArcAssetDetails.h"
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
		void OnSendAssetTransferTransactionCallback(const FString& txID);

	UFUNCTION(BlueprintCallable, Category = "AlgorandHandler")
		void OnSendApplicationCallTransactionCallback(const FString& txID);

	UFUNCTION(BlueprintCallable, Category = "AlgorandHandler")
		void OnGetArcAssetDetailsCallback(const FArcAssetDetails& asset_Details);

	UFUNCTION(BlueprintCallable, Category = "AlgorandHandler")
		void OnRestoreWalletCallback(const FString& output);

	UFUNCTION(BlueprintCallable, Category = "AlgorandHandler")
		void OnInitializeNewWalletCallback(const FString& output);

	UFUNCTION(BlueprintCallable, Category = "AlgorandHandler")
		void OnGetBackupMnemonicPhraseCallback(const FString& output);

	UFUNCTION(BlueprintCallable, Category = "AlgorandHandler")
		void OnGenerateMnemonicsCallback(const FString& output);
};