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

	/// generate wallet api callabck
    DECLARE_DELEGATE_OneParam(FAlgorandGenerateWalletGetDelegate, const Vertices::VerticesGenerateWalletGetResponse&);

	/// get balance api callabck
    DECLARE_DELEGATE_OneParam(FAlgorandGetaddressbalanceGetDelegate, const Vertices::VerticesGetaddressbalanceGetResponse&);

	/// payment tx api callabck
    DECLARE_DELEGATE_OneParam(FAlgorandPaymentTransactionGetDelegate, const Vertices::VerticesPaymentTransactionGetResponse&);

	/// application call tx api callback
    DECLARE_DELEGATE_OneParam(FAlgorandApplicationCallTransactionGetDelegate, const Vertices::VerticesApplicationCallTransactionGetResponse&);

    /**
     * @brief send api request for generate wallet
     * @param Request value to send as params for calling api
     * @param Delegate is used to implement async task after get response as api result
     */
    void AlgorandGenerateWalletGet(const Vertices::VerticesGenerateWalletGetRequest& Request,
								   const FAlgorandGenerateWalletGetDelegate& Delegate = FAlgorandGenerateWalletGetDelegate()) const;

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
	 * @brief send api request for send application call tx
	 * @param Request value to send as params for calling api
	 * @param Delegate is used to implement async task after get response as api result
	 */
    void AlgorandApplicationCallTransactionGet(const Vertices::VerticesApplicationCallTransactionGetRequest& Request, const FAlgorandApplicationCallTransactionGetDelegate& Delegate = FAlgorandApplicationCallTransactionGetDelegate()) const;

private:
	
    /**
     * @brief callback function to be run after api request of generating wallet
     * @param response is used to send as Vertices Response type to Algorand module 
     * @param Delegate is used to execute binded callback from Algorand module
     */
    void OnAlgorandGenerateWalletGetResponse(const Vertices::VerticesGenerateWalletGetResponse& response, const FAlgorandGenerateWalletGetDelegate& Delegate) const;

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
	 * @brief callback function to be run after api request of sending application call tx
	 * @param response is used to send as Vertices Response type to Algorand module 
	 * @param Delegate is used to execute binded callback from Algorand module
	 */
    void OnAlgorandApplicationCallTransactionGetResponse(const Vertices::VerticesApplicationCallTransactionGetResponse& response, const FAlgorandApplicationCallTransactionGetDelegate& Delegate) const;

	// Algorand modules
    TSharedPtr<algorand::vertices::VerticesSDK> vertices_;

};
       
}
}