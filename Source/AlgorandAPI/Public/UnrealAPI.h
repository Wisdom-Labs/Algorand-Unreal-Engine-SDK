#pragma once

#include "CoreMinimal.h"

namespace algorand{
namespace api{

class ALGORANDAPI_API UnrealApi
{
public:
    UnrealApi();
    ~UnrealApi();

    /* Sets the URL Endpoint*/
    void SetURL(const FString& Url);

    class AlgorandGetaddressbalanceGetRequest;
    class AlgorandGetaddressbalanceGetResponse;

    DECLARE_DELEGATE_OneParam(FAlgorandGetaddressbalanceGetDelegate, const AlgorandGetaddressbalanceGetResponse&);

    void AlgorandGetaddressbalanceGet(const AlgorandGetaddressbalanceGetRequest& Request, const FAlgorandGetaddressbalanceGetDelegate& Delegate = FAlgorandGetaddressbalanceGetDelegate()) const;

private:
    void OnAlgorandGetaddressbalanceGetResponse(AlgorandGetaddressbalanceGetResponse response, bool bSucceed, FAlgorandGetaddressbalanceGetDelegate Delegate) const;

    FString Url;
};
       
}
}