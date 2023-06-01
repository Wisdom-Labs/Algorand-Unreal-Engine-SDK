// Copyright 2022, Wisdom Labs. All Rights Reserved

#include "./ResponseBuilers.h"
#include "VerticesApiOperations.h"

namespace {
	using Vertices = algorand::vertices::VerticesSDK;
}

namespace response_builders {
	Vertices::VerticesRestoreWalletGetResponse
		buildRestoreWalletResponse(const FString& output)
	{
		Vertices::VerticesRestoreWalletGetResponse response;
		response.output = output;
		return response;
	}

	Vertices::VerticesInitializeNewWalletGetResponse
		buildInitializeNewWalletResponse(const FString& output)
	{
		Vertices::VerticesInitializeNewWalletGetResponse response;
		response.output = output;
		return response;
	}

	Vertices::VerticesGetBackupMnemonicPhraseGetResponse
		buildGetBackupMnemonicPhraseResponse(const FString& output)
	{
		Vertices::VerticesGetBackupMnemonicPhraseGetResponse response;
		response.output = output;
		return response;
	}

	Vertices::VerticesGenerateMnemonicsGetResponse
		buildGenerateMnemonicsResponse(const FString& output)
	{
		Vertices::VerticesGenerateMnemonicsGetResponse response;
		response.output = output;
		return response;
	}

	Vertices::VerticesGetaddressbalanceGetResponse
		buildGetBalanceResponse(const uint64& amount)
	{
		Vertices::VerticesGetaddressbalanceGetResponse response;
		response.Amount = amount;
		return response;
	}

	Vertices::VerticesPaymentTransactionGetResponse
		buildPaymentTransactionResponse(const FString& txID)
	{
		Vertices::VerticesPaymentTransactionGetResponse response;
		response.txID = txID;
		return response;
	}

	Vertices::VerticesAssetTransferTransactionGetResponse
		buildAssetTransferTransactionResponse(const FString& txID)
	{
		Vertices::VerticesAssetTransferTransactionGetResponse response;
		response.txID = txID;
		return response;
	}

	Vertices::VerticesApplicationCallTransactionGetResponse
		buildApplicationCallTransactionResponse(const FString& txID)
	{
		Vertices::VerticesApplicationCallTransactionGetResponse response;
		response.txID = txID;
		return response;
	}
}