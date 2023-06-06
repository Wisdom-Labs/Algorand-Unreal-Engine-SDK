// Copyright 2022, Wisdom Labs. All Rights Reserved

#pragma once
#include "CoreMinimal.h"
#include "VerticesSDK.h"

namespace {
    using Vertices = algorand::vertices::VerticesSDK;
}

/**
 * ToDo:
 * for asset based on new transactions and wallet provider
 * add following txs
* Key Registration, Asset Configuration, Freeze and Transafer, State Proof
 */

/**
 * Unreal API
 * how send request to vertices SDK 
 * and run callback function by delegate after complete tx process
 */

namespace algorand{
namespace api{

class ALGORANDAPI_API UnrealApi
{
public:
    UnrealApi(TSharedPtr<algorand::vertices::VerticesSDK>&);
    ~UnrealApi();

	/// restore wallet api callabck
    DECLARE_DELEGATE_OneParam(FAlgorandRestoreWalletGetDelegate, const Vertices::VerticesRestoreWalletGetResponse&);

	/// initialize new wallet api callabck
    DECLARE_DELEGATE_OneParam(FAlgorandInitializeNewWalletGetDelegate, const Vertices::VerticesInitializeNewWalletGetResponse&);

	/// get backup mnemonics phrase api callabck
    DECLARE_DELEGATE_OneParam(FAlgorandGetBackupMnemonicPhraseGetDelegate, const Vertices::VerticesGetBackupMnemonicPhraseGetResponse&);

	/// generate mnemonics api callabck
    DECLARE_DELEGATE_OneParam(FAlgorandGenerateMnemonicsGetDelegate, const Vertices::VerticesGenerateMnemonicsGetResponse&);

	/// get balance api callabck
    DECLARE_DELEGATE_OneParam(FAlgorandGetaddressbalanceGetDelegate, const Vertices::VerticesGetaddressbalanceGetResponse&);

	/// payment tx api callabck
    DECLARE_DELEGATE_OneParam(FAlgorandPaymentTransactionGetDelegate, const Vertices::VerticesPaymentTransactionGetResponse&);

	/// asset transfer tx api callabck
	DECLARE_DELEGATE_OneParam(FAlgorandAssetTransferTransactionGetDelegate, const Vertices::VerticesAssetTransferTransactionGetResponse&);

	/// application call tx api callback
    DECLARE_DELEGATE_OneParam(FAlgorandApplicationCallTransactionGetDelegate, const Vertices::VerticesApplicationCallTransactionGetResponse&);

	/// arc asset details api callback
	DECLARE_DELEGATE_OneParam(FAlgorandArcAssetDetailsGetDelegate, const Vertices::VerticesArcAssetDetailsGetResponse&);

    /**
     * @brief send api request for restore wallet
     * @param Request value to send as params for calling api
     * @param Delegate is used to implement async task after get response as api result
     */
    void AlgorandRestoreWalletGet(const Vertices::VerticesRestoreWalletGetRequest& Request,
								   const FAlgorandRestoreWalletGetDelegate& Delegate = FAlgorandRestoreWalletGetDelegate()) const;

	/**
     * @brief send api request for initialize new wallet
     * @param Request value to send as params for calling api
     * @param Delegate is used to implement async task after get response as api result
     */
    void AlgorandInitializeNewWalletGet(const Vertices::VerticesInitializeNewWalletGetRequest& Request,
								   const FAlgorandInitializeNewWalletGetDelegate& Delegate = FAlgorandInitializeNewWalletGetDelegate()) const;

	/**
     * @brief send api request for Get Backup Mnemonic Phrase
     * @param Request value to send as params for calling api
     * @param Delegate is used to implement async task after get response as api result
     */
    void AlgorandGetBackupMnemonicPhraseGet(const Vertices::VerticesGetBackupMnemonicPhraseGetRequest& Request,
								   const FAlgorandGetBackupMnemonicPhraseGetDelegate& Delegate = FAlgorandGetBackupMnemonicPhraseGetDelegate()) const;

	/**
     * @brief send api request for Generate Mnemonics
     * @param Request value to send as params for calling api
     * @param Delegate is used to implement async task after get response as api result
     */
    void AlgorandGenerateMnemonicsGet(const Vertices::VerticesGenerateMnemonicsGetRequest& Request,
								   const FAlgorandGenerateMnemonicsGetDelegate& Delegate = FAlgorandGenerateMnemonicsGetDelegate()) const;

	/**
	 * @brief send api request for get balance of specific address
	 * @param Request value to send as params for calling api
	 * @param Delegate is used to implement async task after get response as api result
	 */
    void AlgorandGetaddressbalanceGet(const Vertices::VerticesGetaddressbalanceGetRequest& Request, const FAlgorandGetaddressbalanceGetDelegate& Delegate = FAlgorandGetaddressbalanceGetDelegate()) const;

	/**
	 * @brief send api request for send payment tx
	 * @param Request value to send as params for calling api
	 * @param Delegate is used to implement async task after get response as api result
	 */
    void AlgorandPaymentTransactionGet(const Vertices::VerticesPaymentTransactionGetRequest& Request, const FAlgorandPaymentTransactionGetDelegate& Delegate = FAlgorandPaymentTransactionGetDelegate()) const;

	/**
	 * @brief send api request for send asset transfer tx
	 * @param Request value to send as params for calling api
	 * @param Delegate is used to implement async task after get response as api result
	 */
	void AlgorandAssetTransferTransactionGet(const Vertices::VerticesAssetTransferTransactionGetRequest& Request, const FAlgorandAssetTransferTransactionGetDelegate& Delegate = FAlgorandAssetTransferTransactionGetDelegate()) const;

	/**
	 * @brief send api request for send application call tx
	 * @param Request value to send as params for calling api
	 * @param Delegate is used to implement async task after get response as api result
	 */
    void AlgorandApplicationCallTransactionGet(const Vertices::VerticesApplicationCallTransactionGetRequest& Request, const FAlgorandApplicationCallTransactionGetDelegate& Delegate = FAlgorandApplicationCallTransactionGetDelegate()) const;

	/**
	 * @brief send api request for send arc asset details
	 * @param Request value to send as params for calling api
	 * @param Delegate is used to implement async task after get response as api result
	 */
	void AlgorandArcAssetDetailsGet(const Vertices::VerticesArcAssetDetailsGetRequest& Request, const FAlgorandArcAssetDetailsGetDelegate& Delegate = FAlgorandArcAssetDetailsGetDelegate()) const;

private:
	
    /**
     * @brief callback function to be run after api request of restoring wallet
     * @param response is used to send as Vertices Response type to Algorand module 
     * @param Delegate is used to execute binded callback from Algorand module
     */
    void OnAlgorandRestoreWalletGetResponse(const Vertices::VerticesRestoreWalletGetResponse& response, const FAlgorandRestoreWalletGetDelegate& Delegate) const;

	/**
     * @brief callback function to be run after api request of initializing new wallet
     * @param response is used to send as Vertices Response type to Algorand module 
     * @param Delegate is used to execute binded callback from Algorand module
     */
    void OnAlgorandInitializeNewWalletGetResponse(const Vertices::VerticesInitializeNewWalletGetResponse& response, const FAlgorandInitializeNewWalletGetDelegate& Delegate) const;

	/**
     * @brief callback function to be run after api request of getting backup mnemonic phrase
     * @param response is used to send as Vertices Response type to Algorand module 
     * @param Delegate is used to execute binded callback from Algorand module
     */
    void OnAlgorandGetBackupMnemonicPhraseGetResponse(const Vertices::VerticesGetBackupMnemonicPhraseGetResponse& response, const FAlgorandGetBackupMnemonicPhraseGetDelegate& Delegate) const;

	/**
     * @brief callback function to be run after api request of generating mnemonics
     * @param response is used to send as Vertices Response type to Algorand module 
     * @param Delegate is used to execute binded callback from Algorand module
     */
    void OnAlgorandGenerateMnemonicsGetResponse(const Vertices::VerticesGenerateMnemonicsGetResponse& response, const FAlgorandGenerateMnemonicsGetDelegate& Delegate) const;

	/**
	 * @brief callback function to be run after api request of getting balance
	 * @param response is used to send as Vertices Response type to Algorand module 
	 * @param Delegate is used to execute binded callback from Algorand module
	 */
    void OnAlgorandGetaddressbalanceGetResponse(const Vertices::VerticesGetaddressbalanceGetResponse& response, const FAlgorandGetaddressbalanceGetDelegate& Delegate) const;

	/**
	 * @brief callback function to be run after api request of sending payment tx
	 * @param response is used to send as Vertices Response type to Algorand module 
	 * @param Delegate is used to execute binded callback from Algorand module
	 */
    void OnAlgorandPaymentTransactionGetResponse(const Vertices::VerticesPaymentTransactionGetResponse& response, const FAlgorandPaymentTransactionGetDelegate& Delegate) const;

	/**
	 * @brief callback function to be run after api request of sending asset transfer tx
	 * @param response is used to send as Vertices Response type to Algorand module 
	 * @param Delegate is used to execute binded callback from Algorand module
	 */
	void OnAlgorandAssetTransferTransactionGetResponse(const Vertices::VerticesAssetTransferTransactionGetResponse& response, const FAlgorandAssetTransferTransactionGetDelegate& Delegate) const;

	/**
	 * @brief callback function to be run after api request of sending application call tx
	 * @param response is used to send as Vertices Response type to Algorand module 
	 * @param Delegate is used to execute binded callback from Algorand module
	 */
    void OnAlgorandApplicationCallTransactionGetResponse(const Vertices::VerticesApplicationCallTransactionGetResponse& response, const FAlgorandApplicationCallTransactionGetDelegate& Delegate) const;

	/**
	 * @brief callback function to be run after api request of sending arc asset details
	 * @param response is used to send as Vertices Response type to Algorand module 
	 * @param Delegate is used to execute binded callback from Algorand module
	 */
	void OnAlgorandArcAssetDetailsGetResponse(const Vertices::VerticesArcAssetDetailsGetResponse& response, const FAlgorandArcAssetDetailsGetDelegate& Delegate) const;

	// Algorand modules
    TSharedPtr<algorand::vertices::VerticesSDK> vertices_;
	
};
       
}
}