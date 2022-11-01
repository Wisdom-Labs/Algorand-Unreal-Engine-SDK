#pragma once

#include "BaseModel.h"
#include "CoreMinimal.h"
#include "Misc/TVariant.h"
#include "UnrealApi.h"

using ApiDelegate = TVariant<
	algorand::api::UnrealApi::FAlgorandGetaddressbalanceGetDelegate,
	algorand::api::UnrealApi::FAlgorandPaymentTransactionGetDelegate
>;

struct RequestContext {
	TSharedPtr<algorand::vertices::Request>  request;
	TSharedPtr<ApiDelegate> delegate;
};