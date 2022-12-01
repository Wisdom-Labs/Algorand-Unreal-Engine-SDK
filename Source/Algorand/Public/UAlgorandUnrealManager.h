// Copyright 2022, Wisdom Labs. All Rights Reserved

#pragma once
#include "CoreMinimal.h"
#include "Engine/World.h"
#include "RequestContextManager.h"
#include "Wallet.h"
#include "Models/FUInt64.h"
#include "Models/FError.h"
#include "UnrealApi.h"
#include "VerticesSDK.h"
#include <exception>
#include "UAlgorandUnrealManager.generated.h"

/**
 * ToDo:
 * for asset bases on transactions and wallet provider
 * add like txs
 * Key Registration, Asset Configuration, Freeze and Transafer, State Proof
 */

/**
 * Algorand Manager
 * how callback works for c++ and blueprint events
 * dynamic and multicast delegates are used for c++ and blueprint callback
 * when c++ callback is called, it calls sendEvent to trigger the delegate.
 * ue5 async task is used for calling delegation.
 */

namespace {
    // Export Procedures
     
    using Vertices = algorand::vertices::VerticesSDK;
}

/// generate wallet callback
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGenerateWalletDelegate, const FString&, address);

/// get balance callback
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetBalanceDelegate, const FUInt64&, money);

/// payment tx callback
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPaymentTransactionDelegate, const FString&, txID);

/// application call tx callback
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FApplicationCallTransactionDelegate, const FString&, txID);

/// error callback
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FErrorDelegate, const FError&, error);

class TransactionBuilder;

UCLASS(Blueprintable, BlueprintType)
class ALGORAND_API UAlgorandUnrealManager : public UObject
{
    GENERATED_BODY()

public:
    UAlgorandUnrealManager();

    /**
     * Create Instance of Algorand manager
     * @param algoRpc algorand rpc url https://node.testnet.algoexplorerapi.io
     * @param algoPort algorand rpc port 0
     * @param algoTokenHeader algorand rpc token header "" , if localhost, X-Algo-API-Token: , if using purestake api, x-api-key:
     * @param outer root on level
     * @return AlgorandUnrealManager as a pointer
     */
    UFUNCTION(BlueprintCallable, Category = "AlgorandUnrealManager")
        static UAlgorandUnrealManager* createInstance(const FString& algoRpc, const FUInt64& algoPort, const FString& algoTokenHeader_, UObject* outer);

    /**
     * Set rpc url of algorand node
     * @param algoRpc algoRpc algorand rpc url
     */
    UFUNCTION(BlueprintCallable, Category = "AlgorandUnrealManager")
        void setAlgoRpc(const FString& algoRpc);

    /**
     * Set rpc port of algorand node
     * @param algoPort algorand rpc port
     */
    UFUNCTION(BlueprintCallable, Category = "AlgorandUnrealManager")
        void setAlgoPort(const FUInt64& algoPort);

    /**
     * Set rpc tokenHeader of algorand node
     * @param algoTokenHeader algorand rpc tokenHeader
     */
    UFUNCTION(BlueprintCallable, Category = "AlgorandUnrealManager")
        void setAlgoTokenHeader(const FString& algoTokenHeader);

    /**
     * get rpc url of algorand node
     * @return algorand rpc url as string
     */
    UFUNCTION(BlueprintCallable, Category = "AlgorandUnrealManager")
        FString getAlgoRpc();

    /**
     * get rpc port of algorand node
     * @return algorand rpc port as uint64
     */
    UFUNCTION(BlueprintCallable, Category = "AlgorandUnrealManager")
        FUInt64 getAlgoPort();

    /**
     * get rpc token header of algorand node
     * @return algorand rpc token header as string
     */
    UFUNCTION(BlueprintCallable, Category = "AlgorandUnrealManager")
        FString getAlgoTokenHeader();

    /**
     * get account address connected to algorand node
     * @return account address as string
     */
    UFUNCTION(BlueprintCallable, Category = "AlgorandUnrealManager")
        FString getAddress();

    /**
     * set account address connected to algorand node
     * @param  address account connected to node
     */
    void setAddress(const FString& address);

    /**
     * error information callback
     */
    UPROPERTY(BlueprintAssignable)
        FErrorDelegate ErrorDelegateCallback;

	/**
	 * generate new account
	 */
    UFUNCTION(BlueprintCallable, Category = "AlgorandUnrealManager")
    void generateWallet();

	/**
	 * generate wallet information callback
	 */
    UPROPERTY(BlueprintAssignable)
        FGenerateWalletDelegate GenerateWalletCallback;

	/**
	 * get response after generate new account
	 * @param response address got by generating new account
	 */
    void OnGenerateWalletCompleteCallback(const Vertices::VerticesGenerateWalletGetResponse& response);

	/**
	 * get balance by specific address
	 * 
	 */
    UFUNCTION(BlueprintCallable, Category = "AlgorandUnrealManager")
    void getBalance();

	/**
	 * get balance information callback
	 */
    UPROPERTY(BlueprintAssignable)
        FGetBalanceDelegate GetBalanceCallback;

	/**
	 * get response after get balance
	 * @param response token amount by specific address
	 */
    void OnGetBalanceCompleteCallback(const Vertices::VerticesGetaddressbalanceGetResponse& response);

	/**
	 * send payment TX to algorand node
	 * @param receiverAddress address which take token 
	 * @param amount token amount to send
	 * @param notes tx description when send payment tx
	 */
    UFUNCTION(BlueprintCallable, Category = "AlgorandUnrealManager")
    void sendPaymentTransaction(const FString& receiverAddress,
                                const FUInt64& amount,
                                const FString& notes);   
	/**
	 * payment transaction information callback
	 */ 
    UPROPERTY(BlueprintAssignable)
        FPaymentTransactionDelegate SendPaymentTransactionCallback;

	/**
	 * get response after payment tx
	 * @param response txID after payment tx
	 */
    void OnSendPaymentTransactionCompleteCallback(const Vertices::VerticesPaymentTransactionGetResponse& response);

	/**
	 * send application call TX to algorand node
	 * @param app_ID application id created by algorand node
	 */
    UFUNCTION(BlueprintCallable, Category = "AlgorandUnrealManager")
    void sendApplicationCallTransaction(const FUInt64& app_ID);

	/**
	 * application transaction information callback
	 */
    UPROPERTY(BlueprintAssignable)
        FApplicationCallTransactionDelegate SendApplicationCallTransactionCallback;

	/**
	 * get response after application call tx
	 * @param response txID after payment tx
	 */ 
    void OnSendApplicationCallTransactionCompleteCallback(const Vertices::VerticesApplicationCallTransactionGetResponse& response);

	/**
	 * return world of outer
	 * @return outer world
	 */ 
    UWorld* GetWorld() const override;

private:
	// algorand rpc information
    FString myAlgoRpc;
    FUInt64 myAlgoPort;
    FString myAlgoTokenHeader;

	// Algorand modules
    TSharedPtr<TransactionBuilder> transactionBuilder_;
    TSharedPtr<algorand::api::UnrealApi> unrealApi_;
    TSharedPtr<algorand::vertices::VerticesSDK> vertices_;

	// context manager to send api request 
    RequestContextManager requestContextManager_;
     
};