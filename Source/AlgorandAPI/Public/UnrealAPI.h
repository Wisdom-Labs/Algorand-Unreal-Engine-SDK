#pragma once

#include "CoreMinimal.h"
#include "VerticesSDK.h"

namespace {
    using Vertices = algorand::vertices::VerticesSDK;
}

namespace algorand{
namespace api{

class ALGORANDAPI_API UnrealApi
{
public:
    UnrealApi(TSharedPtr<algorand::vertices::VerticesSDK>&);
    ~UnrealApi();

    /* Sets the URL Endpoint*/
    void SetURL(const FString& Url);

    //class AlgorandGetaddressbalanceGetRequest;
    //class AlgorandGetaddressbalanceGetResponse;

    DECLARE_DELEGATE_OneParam(FAlgorandGetaddressbalanceGetDelegate, const Vertices::VerticesGetaddressbalanceGetResponse&);

    void AlgorandGetaddressbalanceGet(const Vertices::VerticesGetaddressbalanceGetRequest& Request, const FAlgorandGetaddressbalanceGetDelegate& Delegate = FAlgorandGetaddressbalanceGetDelegate()) const;

private:
    void OnAlgorandGetaddressbalanceGetResponse(const Vertices::VerticesGetaddressbalanceGetResponse& response, bool bSucceed, const FAlgorandGetaddressbalanceGetDelegate& Delegate) const;

    TSharedPtr<algorand::vertices::VerticesSDK> vertices_;

    FString Url;
};
       
}
}