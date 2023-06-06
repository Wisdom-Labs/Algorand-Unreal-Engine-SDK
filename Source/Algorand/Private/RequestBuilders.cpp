// Copyright 2022, Wisdom Labs. All Rights Reserved

#include "RequestBuilders.h"
#include "VerticesApiOperations.h"

namespace {
	using Vertices = algorand::vertices::VerticesSDK;
}
namespace request_builders {
	Vertices::VerticesRestoreWalletGetRequest
	buildRestoreWalletRequest(const FString& mnemonics)
	{
		Vertices::VerticesRestoreWalletGetRequest request;
		request.Mnemonics = mnemonics;
		return request;
	}

	Vertices::VerticesInitializeNewWalletGetRequest
	buildInitializeNewWalletRequest()
	{
		Vertices::VerticesInitializeNewWalletGetRequest request;
		return request;
	}

	Vertices::VerticesGetBackupMnemonicPhraseGetRequest
	buildGetBackupMnemonicPhraseRequest()
	{
		Vertices::VerticesGetBackupMnemonicPhraseGetRequest request;
		return request;
	}

	Vertices::VerticesGenerateMnemonicsGetRequest
	buildGenerateMnemonicsRequest()
	{
		Vertices::VerticesGenerateMnemonicsGetRequest request;
		return request;
	}

	Vertices::VerticesGetaddressbalanceGetRequest
	buildGetBalanceRequest(const FString& address)
	{
		Vertices::VerticesGetaddressbalanceGetRequest request;
		request.Address = address;
		return request;
	}
	
	Vertices::VerticesPaymentTransactionGetRequest
		buildPaymentTransactionRequest(const FString& sender, 
									   const FString& receiver,
									   const FUInt64& amount,
									   const FString& notes)
	{
		Vertices::VerticesPaymentTransactionGetRequest request;
		request.senderAddress = sender;
		request.receiverAddress = receiver;
		request.amount = amount;
		request.notes = notes;
		return request;
	}
		
	Vertices::VerticesAssetTransferTransactionGetRequest
		buildAssetTransferTransactionRequest(const FString& sender, 
									   const FString& receiver,
									   const FUInt64& asset_id,
									   const FUInt64& amount,
									   const FString& notes)
	{
		Vertices::VerticesAssetTransferTransactionGetRequest request;
		request.senderAddress = sender;
		request.receiverAddress = receiver;
		request.asset_id = asset_id;
		request.amount = amount;
		request.notes = notes;
		return request;
	}

	Vertices::VerticesApplicationCallTransactionGetRequest
		buildApplicationCallTransactionRequest(const FString& sender, 
									   const FUInt64& app_ID)
	{
		Vertices::VerticesApplicationCallTransactionGetRequest request;
		request.senderAddress = sender;
		request.app_ID = app_ID;
		return request;
	}

	Vertices::VerticesArcAssetDetailsGetRequest
		buildArcAssetDetailsRequest(const FUInt64& asset_ID)
	{
		Vertices::VerticesArcAssetDetailsGetRequest request;
		request.asset_ID = asset_ID;
		return request;
	}
}