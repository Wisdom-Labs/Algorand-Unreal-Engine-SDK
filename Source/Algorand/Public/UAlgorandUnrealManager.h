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

namespace {
    using Vertices = algorand::vertices::VerticesSDK;
}

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetBalanceDelegate, const FUInt64&, money);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPaymentTransactionDelegate, const FString&, txID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FApplicationCallTransactionDelegate, const FString&, txID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FErrorDelegate, const FError&, error);

class TransactionBuilder;

UCLASS(BlueprintType, Blueprintable)
class ALGORAND_API UAlgorandUnrealManager : public UObject
{
    GENERATED_BODY()

public:
    UAlgorandUnrealManager();

    UFUNCTION(BlueprintCallable, Category = "AlgorandUnrealManager")
        static UAlgorandUnrealManager* createInstance(UObject* outer);

    UFUNCTION(BlueprintCallable, Category = "AlgorandUnrealManager")
        FString getAddress();

    UPROPERTY(BlueprintAssignable)
        FErrorDelegate ErrorDelegateCallback;

    UFUNCTION(BlueprintCallable, Category = "AlgorandUnrealManager")
    void getBalance();

    UPROPERTY(BlueprintAssignable)
        FGetBalanceDelegate GetBalanceCallback;

    void OnGetBalanceCompleteCallback(const Vertices::VerticesGetaddressbalanceGetResponse& response);

    UFUNCTION(BlueprintCallable, Category = "AlgorandUnrealManager")
    void sendPaymentTransaction(const FString& receiverAddress,
                                const FUInt64& amount);   

    UPROPERTY(BlueprintAssignable)
        FPaymentTransactionDelegate SendPaymentTransactionCallback;

    void OnSendPaymentTransactionCompleteCallback(const Vertices::VerticesPaymentTransactionGetResponse& response);

    UFUNCTION(BlueprintCallable, Category = "AlgorandUnrealManager")
    void sendApplicationCallTransaction(const FUInt64& app_ID);

    UPROPERTY(BlueprintAssignable)
        FApplicationCallTransactionDelegate SendApplicationCallTransactionCallback;

    void OnSendApplicationCallTransactionCompleteCallback(const Vertices::VerticesApplicationCallTransactionGetResponse& response);

    UWorld* GetWorld() const override;

private:
    TSharedPtr<TransactionBuilder> transactionBuilder_;
    TSharedPtr<algorand::api::UnrealApi> unrealApi_;

    TSharedPtr<algorand::vertices::ThreadContextManager> threadContextManager_;

    RequestContextManager requestContextManager_;
     
};