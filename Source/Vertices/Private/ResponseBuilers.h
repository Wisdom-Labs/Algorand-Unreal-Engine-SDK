// Copyright 2022, Wisdom Labs. All Rights Reserved

#pragma once
#include "VerticesSDK.h"

namespace response_builders {
	algorand::vertices::VerticesSDK::VerticesRestoreWalletGetResponse
		buildRestoreWalletResponse(const FString& output);
	algorand::vertices::VerticesSDK::VerticesInitializeNewWalletGetResponse
		buildInitializeNewWalletResponse(const FString& output);
	algorand::vertices::VerticesSDK::VerticesGetBackupMnemonicPhraseGetResponse
		buildGetBackupMnemonicPhraseResponse(const FString& output);
	algorand::vertices::VerticesSDK::VerticesGenerateMnemonicsGetResponse
		buildGenerateMnemonicsResponse(const FString& output);
	algorand::vertices::VerticesSDK::VerticesGetaddressbalanceGetResponse
		buildGetBalanceResponse(const uint64& amount);
	algorand::vertices::VerticesSDK::VerticesPaymentTransactionGetResponse
		buildPaymentTransactionResponse(const FString& txID);
	algorand::vertices::VerticesSDK::VerticesAssetConfigTransactionGetResponse
		buildAssetConfigTransactionResponse(const FString& txID, const uint64& assetID);
	algorand::vertices::VerticesSDK::VerticesAssetTransferTransactionGetResponse
		buildAssetTransferTransactionResponse(const FString& txID);
	algorand::vertices::VerticesSDK::VerticesApplicationCallTransactionGetResponse
		buildApplicationCallTransactionResponse(const FString& txID);
}