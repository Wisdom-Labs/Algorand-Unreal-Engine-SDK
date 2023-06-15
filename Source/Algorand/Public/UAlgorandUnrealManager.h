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
 * Key Registration, Asset Freeze, State Proof
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
 * asset config tx callback
 * @param txID transaction hash
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAssetConfigTransactionDelegate, const FString&, txID);

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
     * @param algodRpc algorand rpc url https://testnet-api.algonode.cloud
     * @param algodPort algorand rpc port 443
     * @param algodTokenHeader algorand rpc token header "" , if localhost, X-Algo-API-Token: , if using purestake api, x-api-key:
	 * @param indexerRpc algorand rpc url https://testnet-idx.algonode.network
	 * @param indexerPort algorand rpc port 443
	 * @param indexerTokenHeader algorand rpc token header "" , if localhost, X-Algo-API-Token: , if using purestake api, x-api-key:
     * @param outer root on level
     * @return AlgorandUnrealManager as a pointer
     */
    UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "createInstanceWithParams", Keywords = "AlgorandManager"),
			  Category = "AlgorandUnrealManager")
        static UAlgorandUnrealManager* createInstanceWithParams(const FString& algodRpc,
																const FUInt64& algodPort,
																const FString& algodTokenHeader_,
																const FString& indexerRpc,
																const FUInt64& indexerPort,
																const FString& indexerTokenHeader_,
																UObject* outer);

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
	 * @param algodRpc algorand rpc url https://testnet-api.algonode.cloud
	 * @param algodPort algorand rpc port 443
	 * @param algodTokenHeader algorand rpc token header "" , if localhost, X-Algo-API-Token: , if using purestake api, x-api-key:
	 * @return AlgorandUnrealManager as a pointer
	 */
	UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "setAlgodRpcInfo", Keywords = "AlgorandManager"),
			  Category = "AlgorandUnrealManager")
		void setAlgodRpcInfo(const FString& algodRpc, const FUInt64& algodPort, const FString& algodTokenHeader_);

	/**
	 * Create Instance of Algorand manager
	 * @param indexerRpc algorand rpc url https://testnet-idx.algonode.network
	 * @param indexerPort algorand rpc port 443
	 * @param indexerTokenHeader algorand rpc token header "" , if localhost, X-Algo-API-Token: , if using purestake api, x-api-key:
	 * @return AlgorandUnrealManager as a pointer
	 */
	UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "setIndexerRpcInfo", Keywords = "AlgorandManager"),
			  Category = "AlgorandUnrealManager")
		void setIndexerRpcInfo(const FString& indexerRpc, const FUInt64& indexerPort, const FString& indexerTokenHeader);

	/**
	 * get current rpc net connected to algorand node
	 * @return rpc net as string
	 */
	UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "getAlgodRpcNet", Keywords = "AlgodRpcInfo"),
			  Category = "AlgorandUnrealManager")
		FString getAlgodRpcNet();

	/**
	 * get current rpc net connected to algorand node
	 * @return rpc net as string
	 */
	UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "getIndexerRpcNet", Keywords = "IndexerRpcInfo"),
			  Category = "AlgorandUnrealManager")
		FString getIndexerRpcNet();
	
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
	 * @param creator address which create token
	 * @param manager address which take token 
	 * @param reserve address which take token 
	 * @param freeze address which take token 
	 * @param clawback address which take token 
	 * @param asset_id of token to be sent
	 * @param total amount of token to be sent
	 * @param decimals of token to be sent
	 * @param unit_name of token to be sent
	 * @param asset_name of token to be sent
	 * @param url of token to be sent
	 * @param notes tx description when send payment tx
	 */
	UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "sendAssetConfigTX", Keywords = "AssetConfigTX"),
			  Category = "AlgorandUnrealManager")
	void sendAssetConfigTransaction(const FString& manager,
									const FString& reserve,
									const FString& freeze,
									const FString& clawback,
									const FUInt64& asset_id,
									const FUInt64& total,
									const FUInt64& decimals,
									const FString& isFrozen,
									const FString& unit_name,
									const FString& asset_name,
									const FString& url,
									const FString& notes);   

	/**
	 * asset transfer transaction information callback
	 */ 
	UPROPERTY(BlueprintAssignable, Category = "MultiCastDelegate")
	FAssetConfigTransactionDelegate SendAssetConfigTransactionCallback;

	/**
	 * get response after asset config tx
	 * @param response txID after asset config tx
	 */
	void OnSendAssetConfigTransactionCompleteCallback(const Vertices::VerticesAssetConfigTransactionGetResponse& response);

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
    FString myAlgodRpc;
    FUInt64 myAlgodPort;
    FString myAlgodTokenHeader;

	FString myIndexerRpc;
	FUInt64 myIndexerPort;
	FString myIndexerTokenHeader;

	// Algorand modules
    TSharedPtr<TransactionBuilder> transactionBuilder_;
    TSharedPtr<algorand::api::UnrealApi> unrealApi_;
    TSharedPtr<algorand::vertices::VerticesSDK> vertices_;

	// context manager to send api request 
    RequestContextManager requestContextManager_;
     
};