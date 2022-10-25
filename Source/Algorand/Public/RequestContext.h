#pragma once

#include "BaseModel.h"
#include "CoreMinimal.h"
#include "Misc/TVariant.h"
#include "UnrealApi.h"

using ApiDelegate = TVariant<
	algorand::api::UnrealApi::FAlgorandGetaddressbalanceGetDelegate>;

struct RequestContext {
	TSharedPtr<Request>  request;
	TSharedPtr<ApiDelegate> delegate;
};