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
DECLARE_DYNAMIC_DELEGATE_OneParam(FLoadAccountInfoResponseReceivedDelegate, const FUInt64&, money);
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
    void loadAccountInfo(const FLoadAccountInfoResponseReceivedDelegate& delegate,
                         const FErrorReceivedDelegate& errorDelegate);
    void loadAccountInfo(TFunction<void(const TResult<int64>&)> callback);   // account_t

    UWorld* GetWorld() const override;

private:
    TSharedPtr<TransactionBuilder> transactionBuilder_;
    TSharedPtr<algorand::api::UnrealApi> unrealApi_;

    TSharedPtr<algorand::vertices::ThreadContextManager> threadContextManager_;

    RequestContextManager requestContextManager_;
     
};