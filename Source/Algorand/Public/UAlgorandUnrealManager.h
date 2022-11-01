#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"

#include "RequestContextManager.h"
#include "ThreadContextManager.h"
#include "Wallet.h"
#include "Models/FUInt64.h"
#include "Models/FError.h"
#include "TResult.h"
#include "UnrealApi.h"
#include "VerticesSDK.h"

#include "UAlgorandUnrealManager.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FGetBalanceResponseReceivedDelegate, const FUInt64&, money);
DECLARE_DYNAMIC_DELEGATE_OneParam(FPaymentTransactionResponseReceivedDelegate, const FUInt64&, money);
DECLARE_DYNAMIC_DELEGATE_OneParam(FApplicationCallTransactionResponseReceivedDelegate, const FUInt64&, money);
DECLARE_DYNAMIC_DELEGATE_OneParam(FErrorReceivedDelegate, const FError&, error);

class TransactionBuilder;

UCLASS(BlueprintType, Blueprintable)
class ALGORAND_API UAlgorandUnrealManager : public UObject
{
    GENERATED_BODY()

public:
    UAlgorandUnrealManager();

    UFUNCTION(BlueprintCallable, Category = "AlgorandManager")
        static UAlgorandUnrealManager* createInstance(UObject* outer);

    UFUNCTION(BlueprintCallable, Category = "AlgorandUnrealManager")
    FString getAddress();

    UFUNCTION(BlueprintCallable, Category = "AlgorandUnrealManager")
    void getBalance(const FGetBalanceResponseReceivedDelegate& delegate,
                    const FErrorReceivedDelegate& errorDelegate);
    void getBalance(TFunction<void(const TResult<int64>&)> callback);

    UFUNCTION(BlueprintCallable, Category = "AlgorandUnrealManager")
    void sendPaymentTransaction(const FPaymentTransactionResponseReceivedDelegate& delegate,
                         const FErrorReceivedDelegate& errorDelegate);
    void sendPaymentTransaction(const FString& receiverAddress,
                                const uint64_t& amount,
                                TFunction<void(const TResult<int64>&)> callback);   

    UFUNCTION(BlueprintCallable, Category = "AlgorandUnrealManager")
    void sendApplicationCallTransaction(const FApplicationCallTransactionResponseReceivedDelegate& delegate,
                         const FErrorReceivedDelegate& errorDelegate);
    void sendApplicationCallTransaction(const uint64_t& app_ID,
                                TFunction<void(const TResult<int64>&)> callback);   

    UWorld* GetWorld() const override;

private:
    TSharedPtr<TransactionBuilder> transactionBuilder_;
    TSharedPtr<algorand::api::UnrealApi> unrealApi_;

    TSharedPtr<algorand::vertices::ThreadContextManager> threadContextManager_;

    RequestContextManager requestContextManager_;
     
};