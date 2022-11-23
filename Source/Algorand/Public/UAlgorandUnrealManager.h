#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"

#include "RequestContextManager.h"
#include "Wallet.h"
#include "Models/FUInt64.h"
#include "Models/FError.h"
#include "TResult.h"
#include "UnrealApi.h"
#include "VerticesSDK.h"
#include <exception>

#include "UAlgorandUnrealManager.generated.h"

namespace {
    using Vertices = algorand::vertices::VerticesSDK;
}

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGenerateWalletDelegate, const FString&, address);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetBalanceDelegate, const FUInt64&, money);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPaymentTransactionDelegate, const FString&, txID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FApplicationCallTransactionDelegate, const FString&, txID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FErrorDelegate, const FError&, error);

class TransactionBuilder;

UCLASS(Blueprintable, BlueprintType)
class ALGORAND_API UAlgorandUnrealManager : public UObject
{
    GENERATED_BODY()

public:
    UAlgorandUnrealManager();

    UFUNCTION(BlueprintCallable, Category = "AlgorandUnrealManager")
        static UAlgorandUnrealManager* createInstance(const FString& algoRpc, const FUInt64& algoPort, const FString& algoTokenHeader_, UObject* outer);

    UFUNCTION(BlueprintCallable, Category = "AlgorandUnrealManager")
        void setAlgoRpc(const FString& algoRpc);

    UFUNCTION(BlueprintCallable, Category = "AlgorandUnrealManager")
        void setAlgoPort(const FUInt64& algoPort);

    UFUNCTION(BlueprintCallable, Category = "AlgorandUnrealManager")
        void setAlgoTokenHeader(const FString& algoTokenHeader);

    UFUNCTION(BlueprintCallable, Category = "AlgorandUnrealManager")
        FString getAlgoRpc();

    UFUNCTION(BlueprintCallable, Category = "AlgorandUnrealManager")
        FUInt64 getAlgoPort();

    UFUNCTION(BlueprintCallable, Category = "AlgorandUnrealManager")
        FString getAlgoTokenHeader();

    UFUNCTION(BlueprintCallable, Category = "AlgorandUnrealManager")
        FString getAddress();

    void setAddress(const FString& address);

    UPROPERTY(BlueprintAssignable)
        FErrorDelegate ErrorDelegateCallback;

    UFUNCTION(BlueprintCallable, Category = "AlgorandUnrealManager")
    void generateWallet();

    UPROPERTY(BlueprintAssignable)
        FGenerateWalletDelegate GenerateWalletCallback;

    void OnGenerateWalletCompleteCallback(const Vertices::VerticesGenerateWalletGetResponse& response);

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
    FString myAlgoRpc;
    FUInt64 myAlgoPort;
    FString myAlgoTokenHeader;

    TSharedPtr<TransactionBuilder> transactionBuilder_;
    TSharedPtr<algorand::api::UnrealApi> unrealApi_;

    TSharedPtr<algorand::vertices::VerticesSDK> vertices_;

    RequestContextManager requestContextManager_;
     
};