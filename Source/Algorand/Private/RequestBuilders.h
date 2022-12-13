// Copyright 2022, Wisdom Labs. All Rights Reserved

#pragma once
#include "Models/FUInt64.h"
#include "VerticesSDK.h"

/**
 * ToDo:
 * @brief for asset bases on transactions and wallet provider
 * add requests for new txs
 * Key Registration, Asset Configuration, Freeze and Transafer, State Proof
 */

/**
 * Request Builder
 * how this builds request for wallet and txs
 * according to Request of Vertices Library
 */

namespace request_builders {
	
	/**
	 * @brief build request for generate wallet 
	 * @return request
	 */
	algorand::vertices::VerticesSDK::VerticesGenerateWalletGetRequest
	buildGenerateWalletRequest();

	/**
	 * @brief build request for get balance with following params
	 * like address
	 * @return request
	 */
	algorand::vertices::VerticesSDK::VerticesGetaddressbalanceGetRequest
	buildGetBalanceRequest(const FString& address);

	/**
	 * @brief build request for payment tx with following params
	 * like sender, receiver, amount and notes
	 * @return request
	 */
	algorand::vertices::VerticesSDK::VerticesPaymentTransactionGetRequest
		buildPaymentTransactionRequest(const FString& sender,
									   const FString& receiver,
									   const FUInt64& amount,
									   const FString& notes);

	/**
	 * @brief build request for application call tx with following params
	 * like application ID
	 * @return request
	 */
	algorand::vertices::VerticesSDK::VerticesApplicationCallTransactionGetRequest
		buildApplicationCallTransactionRequest(const FString& sender,
									   const FUInt64& app_ID);
}