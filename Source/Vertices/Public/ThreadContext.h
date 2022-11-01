#pragma once

#include "CoreMinimal.h"
#include "Misc/TVariant.h"
#include "BaseModel.h"

namespace {
	using Vertices = algorand::vertices::VerticesSDK;
}

using VerticesDelegate = TVariant<
	Vertices::FVerticesGetaddressbalanceGetDelegate,
	Vertices::FVerticesPaymentTransactionGetDelegate
>;

using VerticesRequest = TVariant <
	Vertices::VerticesGetaddressbalanceGetRequest,
	Vertices::VerticesPaymentTransactionGetRequest
>;

//using VerticesMethod = TVariant<
//	TFunction<void(const Vertices::VerticesGetaddressbalanceGetRequest&, Vertices::FVerticesGetaddressbalanceGetDelegate)>
//	>;

struct ThreadContext {
	algorand::vertices::Request* request;
	TSharedPtr<VerticesDelegate> delegate;
	SIZE_T  method_type;
};

