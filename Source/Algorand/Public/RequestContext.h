// Copyright 2022, Wisdom Labs. All Rights Reserved

#pragma once
#include "BaseModel.h"
#include "CoreMinimal.h"
#include "Misc/TVariant.h"
#include "UnrealApi.h"

/**
 * ToDo:
 * for asset bases on transactions and wallet provider
 * add like txs
 * Key Registration, Asset Configuration, Freeze and Transafer, State Proof
 */

/**
 * declare ApiDelegate and Request to build request context
 */

/// declaring templates of Api Delegate as TVariant
using ApiDelegate = TVariant<
	algorand::api::UnrealApi::FAlgorandRestoreWalletGetDelegate,
	algorand::api::UnrealApi::FAlgorandInitializeNewWalletGetDelegate,
	algorand::api::UnrealApi::FAlgorandGetBackupMnemonicPhraseGetDelegate,
	algorand::api::UnrealApi::FAlgorandGenerateMnemonicsGetDelegate,
	algorand::api::UnrealApi::FAlgorandGetaddressbalanceGetDelegate,
	algorand::api::UnrealApi::FAlgorandPaymentTransactionGetDelegate,
	algorand::api::UnrealApi::FAlgorandAssetConfigTransactionGetDelegate,
	algorand::api::UnrealApi::FAlgorandAssetTransferTransactionGetDelegate,
	algorand::api::UnrealApi::FAlgorandApplicationCallTransactionGetDelegate,
	algorand::api::UnrealApi::FAlgorandArcAssetDetailsGetDelegate
>;

/// Request Contxet info
struct RequestContext {

	/// request pointer 
	TSharedPtr<algorand::vertices::Request>  request;
 
	///delegate callback
	TSharedPtr<ApiDelegate> delegate;
};