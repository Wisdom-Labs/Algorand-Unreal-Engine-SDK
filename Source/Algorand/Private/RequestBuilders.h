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
	 * @brief build request for restore wallet 
	 * @return request
	 */
	algorand::vertices::VerticesSDK::VerticesRestoreWalletGetRequest
	buildRestoreWalletRequest(const FString& mnemonics);

	/**
	 * @brief build request for initialize new wallet 
	 * @return request
	 */
	algorand::vertices::VerticesSDK::VerticesInitializeNewWalletGetRequest
	buildInitializeNewWalletRequest();

	/**
	 * @brief build request for get backup mnemonic phrase
	 * @return request
	 */
	algorand::vertices::VerticesSDK::VerticesGetBackupMnemonicPhraseGetRequest
	buildGetBackupMnemonicPhraseRequest();

	/**
	 * @brief build request for generate mnemonics
	 * @return request
	 */
	algorand::vertices::VerticesSDK::VerticesGenerateMnemonicsGetRequest
	buildGenerateMnemonicsRequest();

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
	 * @brief build request for asset config tx with following params
	 * like creator, manager,reserve, freeze, clawback, asset_id, total, decimals, unit_name,asset_name, url, notes
	 * @return request
	 */
	 Vertices::VerticesAssetConfigTransactionGetRequest
		buildAssetConfigTransactionRequest(const FString& creator, 
									   const FString& manager,
									   const FString& reserve,
									   const FString& freeze,
									   const FString& clawback,
									   const FUInt64& asset_id,
									   const FUInt64& total,
									   const FUInt64& decimals,
									   const FString& unit_name,
									   const FString& asset_name,
									   const FString& url,
									   const FString& notes);

	/**
	 * @brief build request for asset transfer tx with following params
	 * like sender, receiver, amount and notes
	 * @return request
	 */
	algorand::vertices::VerticesSDK::VerticesAssetTransferTransactionGetRequest
		buildAssetTransferTransactionRequest(const FString& sender,
									   const FString& receiver,
									   const FUInt64& asset_id,
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

	/**
	 * @brief build request for arc asset details with following params
	 * like asset ID
	 * @return request
	 */
	algorand::vertices::VerticesSDK::VerticesArcAssetDetailsGetRequest
		buildArcAssetDetailsRequest(const FUInt64& asset_ID);
}