// Copyright 2022, Wisdom Labs. All Rights Reserved

#pragma once
#include "CoreMinimal.h"
#include "Engine/World.h"
#include "RequestContextManager.h"
#include "Wallet.h"
#include "Models/FArcAssetDetails.h"
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

/**
 * restore wallet callback 
 * @param output generated address
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRestoreWalletDelegate, const FString&, output);

/**
 * initialize new wallet callback 
 * @param output generated address
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInitializeNewWalletDelegate, const FString&, output);

/**
 * get backup mnemonics phrase callback 
 * @param output backup mnemonics
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetBackupMnemonicPhraseDelegate, const FString&, output);

/**
 * generate mnemonics callback 
 * @param output generated mnemonics
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGenerateMnemonicsDelegate, const FString&, output);

/**
 * get balance callback
 * @param money account balance
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetBalanceDelegate, const FUInt64&, money);

/**
 * payment tx callback
 * @param txID transaction hash
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPaymentTransactionDelegate, const FString&, txID);

/**
 * asset transfer tx callback
 * @param txID transaction hash
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAssetTransferTransactionDelegate, const FString&, txID);

/**
 * application call tx callback
 * @param txID transaction hash
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FApplicationCallTransactionDelegate, const FString&, txID);

/**
 * nft details callback
 * @param assetDetails Arc Asset details
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FArcAssetDetailsDelegate, const FArcAssetDetails&, assetDetails);

/**
 * error callback
 * @param error 
*/
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
    UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "createInstanceWithParams", Keywords = "AlgorandManager"),
			  Category = "AlgorandUnrealManager")
        static UAlgorandUnrealManager* createInstanceWithParams(const FString& algoRpc, const FUInt64& algoPort, const FString& algoTokenHeader_, UObject* outer);

	/**
	 * Create Instance of Algorand manager
	 * @param outer root on level
	 * @return AlgorandUnrealManager as a pointer
	 * 
	 */
	UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "CreateInstance", Keywords = "AlgorandManager"),
			  Category = "AlgorandUnrealManager")
		static UAlgorandUnrealManager* createInstance(UObject* outer);

	/**
	 * Create Instance of Algorand manager
	 * @param algoRpc algorand rpc url https://node.testnet.algoexplorerapi.io
	 * @param algoPort algorand rpc port 0
	 * @param algoTokenHeader algorand rpc token header "" , if localhost, X-Algo-API-Token: , if using purestake api, x-api-key:
	 * @return AlgorandUnrealManager as a pointer
	 */
	UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "setAlgoRpcInfo", Keywords = "AlgorandManager"),
			  Category = "AlgorandUnrealManager")
		void setAlgoRpcInfo(const FString& algoRpc, const FUInt64& algoPort, const FString& algoTokenHeader_);

    /**
     * Set rpc url of algorand node
     * @param algoRpc algoRpc algorand rpc url
     */
    UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "setAlgoRpc", Keywords = "SetAlgo"),
			  Category = "AlgorandUnrealManager")
        void setAlgoRpc(const FString& algoRpc);

    /**
     * Set rpc port of algorand node
     * @param algoPort algorand rpc port
     */
    UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "setAlgoPort", Keywords = "SetAlgo"),
    		  Category = "AlgorandUnrealManager")
        void setAlgoPort(const FUInt64& algoPort);

    /**
     * Set rpc tokenHeader of algorand node
     * @param algoTokenHeader algorand rpc tokenHeader
     */
    UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "setAlgoTokenHeader", Keywords = "SetAlgo"),
    		  Category = "AlgorandUnrealManager")
        void setAlgoTokenHeader(const FString& algoTokenHeader);

    /**
     * get rpc url of algorand node
     * @return algorand rpc url as string
     */
    UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "getAlgoRpc", Keywords = "GetAlgo"),
    	      Category = "AlgorandUnrealManager")
        FString getAlgoRpc();

    /**
     * get rpc port of algorand node
     * @return algorand rpc port as uint64
     */
	
    UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "getAlgoPort", Keywords = "GetAlgo"),
    		  Category = "AlgorandUnrealManager")
        FUInt64 getAlgoPort();

    /**
     * get rpc token header of algorand node
     * @return algorand rpc token header as string
     */
    UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "getAlgoTokenHeader", Keywords = "GetAlgo"),
    		  Category = "AlgorandUnrealManager")
        FString getAlgoTokenHeader();

    /**
     * get account address connected to algorand node
     * @return account address as string
     */
    UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "getAddress", Keywords = "Address"),
    		  Category = "AlgorandUnrealManager")
        FString getAddress();

    /**
     * set account address connected to algorand node
     * @param  address account connected to node
     */
    void setAddress(const FString& address);

    /**
     * error information callback
     */
    UPROPERTY(BlueprintAssignable, Category = "MultiCastDelegate")
        FErrorDelegate ErrorDelegateCallback;

	/**
	 * Restore Wallet
	 */
    UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "RestoreWallet", Keywords = "Wallet"),
    		  Category = "AlgorandUnrealManager")
    void restoreWallet(const FString& mnemonics);

	/**
	 * restore wallet callback
	 */
	UPROPERTY(BlueprintAssignable, Category = "MultiCastDelegate")
        FRestoreWalletDelegate RestoreWalletCallback;

	/**
	 * get response after restore wallet with mnemonics
	 * @param response address got by restoring wallet
	 */
    void OnRestoreWalletCompleteCallback(const Vertices::VerticesRestoreWalletGetResponse& response);
    
    /**
	 * Initialize New Wallet
	 */
    UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "InitializeNewWallet", Keywords = "Wallet"),
    		  Category = "AlgorandUnrealManager")
    void initializeNewWallet();

	/**
	 * initialize new wallet callback
	 */
	UPROPERTY(BlueprintAssignable, Category = "MultiCastDelegate")
        FInitializeNewWalletDelegate InitializeNewWalletCallback;

	/**
	 * get response after initialize new wallet
	 * @param response address got by initializing new wallet
	 */
    void OnInitializeNewWalletCompleteCallback(const Vertices::VerticesInitializeNewWalletGetResponse& response);

	/**
	 * Get Backup Mnemonics Phrase
	 */
    UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "GetBackupMnemonicsPhrase", Keywords = "Wallet"),
    		  Category = "AlgorandUnrealManager")
    void getBackupMnemonicPhrase();

	/**
	 * get backup mnemonic phrase
	 */
	UPROPERTY(BlueprintAssignable, Category = "MultiCastDelegate")
        FGetBackupMnemonicPhraseDelegate GetBackupMnemonicPhraseCallback;

	/**
	 * get response of backup mnemonics phrase
	 * @param response mnemonics phrase of backup
	 */
    void OnGetBackupMnemonicPhraseCompleteCallback(const Vertices::VerticesGetBackupMnemonicPhraseGetResponse& response);
    
	/**
	 * Generate Mnemonics
	 */
    UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "GenerateMnemonics", Keywords = "Wallet"),
    		  Category = "AlgorandUnrealManager")
    void generateMnemonics();

	/**
	 * generate mnemonics callback
	 */
	UPROPERTY(BlueprintAssignable, Category = "MultiCastDelegate")
        FGenerateMnemonicsDelegate GenerateMnemonicsCallback;

	/**
	 * get response after generating mnemonics
	 * @param response mnemonics got by generating mnemonics
	 */
    void OnGenerateMnemonicsCompleteCallback(const Vertices::VerticesGenerateMnemonicsGetResponse& response);
    
	/**
	 * get balance by specific address
	 * 
	 */
    UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "getBalance", Keywords = "Balance"),
    		  Category = "AlgorandUnrealManager")
    void getBalance(const FString& address);

	/**
	 * get balance information callback
	 */
	UPROPERTY(BlueprintAssignable, Category = "MultiCastDelegate")
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
    UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "sendPaymentTX", Keywords = "PaymentTX"),
    		  Category = "AlgorandUnrealManager")
    void sendPaymentTransaction(const FString& receiverAddress,
                                const FUInt64& amount,
                                const FString& notes);   
	/**
	 * payment transaction information callback
	 */ 
	UPROPERTY(BlueprintAssignable, Category = "MultiCastDelegate")
        FPaymentTransactionDelegate SendPaymentTransactionCallback;

	/**
	 * get response after payment tx
	 * @param response txID after payment tx
	 */
    void OnSendPaymentTransactionCompleteCallback(const Vertices::VerticesPaymentTransactionGetResponse& response);

	/**
	 * send asset transfer TX to algorand node
	 * @param senderAddress address which take token
	 * @param receiverAddress address which take token 
	 * @param asset_ID of token to be sent
	 * @param amount of token to be sent
	 * @param notes tx description when send payment tx
	 */
	UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "sendAssetTransferTX", Keywords = "AssetTransferTX"),
			  Category = "AlgorandUnrealManager")
	void sendAssetTransferTransaction(const FString& senderAddress,
								const FString& receiverAddress,
								const FUInt64& asset_ID,
								const FUInt64& amount,
								const FString& notes);   
	/**
	 * asset transfer transaction information callback
	 */ 
	UPROPERTY(BlueprintAssignable, Category = "MultiCastDelegate")
	FAssetTransferTransactionDelegate SendAssetTransferTransactionCallback;

	/**
	 * get response after asset transfer tx
	 * @param response txID after asset transfer tx
	 */
	void OnSendAssetTransferTransactionCompleteCallback(const Vertices::VerticesAssetTransferTransactionGetResponse& response);

	/**
	 * send application call TX to algorand node
	 * @param app_ID application id created by algorand node
	 */
    UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "sendApplicationCallTX", Keywords = "ApplCallTX"),	
    		  Category = "AlgorandUnrealManager")
    void sendApplicationCallTransaction(const FUInt64& app_ID);

	/**
	 * application transaction information callback
	 */
	UPROPERTY(BlueprintAssignable, Category = "MultiCastDelegate")
        FApplicationCallTransactionDelegate SendApplicationCallTransactionCallback;

	/**
	 * get response after application call tx
	 * @param response txID after application call tx
	 */ 
    void OnSendApplicationCallTransactionCompleteCallback(const Vertices::VerticesApplicationCallTransactionGetResponse& response);

	/**
	 * send request to algorand node to fetech arc-asset details
	 * @param asset_ID asset id to be fetched from algorand node
	 */
	UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "fetchArcAssetDetails", Keywords = "ArcAsset"),	
			  Category = "AlgorandUnrealManager")
	void fetchArcAssetDetails(const FUInt64& asset_ID);

	/**
	 * arc asset details callback
	 */
	UPROPERTY(BlueprintAssignable, Category = "MultiCastDelegate")
	FArcAssetDetailsDelegate FetchArcAssetDetailsCallback;

	/**
	 * get response after arc asset details
	 * @param response after arc asset details
	 */ 
	void OnFetchArcAssetDetailsCompleteCallback(const Vertices::VerticesArcAssetDetailsGetResponse& response);

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