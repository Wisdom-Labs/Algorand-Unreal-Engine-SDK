#pragma once

#include "CoreMinimal.h"

namespace algorand{
namespace api{

class UnrealApi
{
    public:
    UnrealApi();
    ~UnrealApi();

    /* Sets the URL Endpoint*/
    void SetURL(const FString& Url);

    class AlgorandGetaddressbalanceGetRequest;
    class AlgorandGetaddressbalanceGetResponse;

    DECLARE_DELEGATE_OneParam(FAlgorandGetaddressbalanceGetDelegate, const Unity3dGetaddressbalanceGetResponse&);

    void AlgorandGetaddressbalanceGet(const AlgorandGetaddressbalanceGetRequest& Request, const FAlgorandGetaddressbalanceGetDelegate& Delegate = FAlgorandGetaddressbalanceGetDelegate()) const;

    private:
        void OnAlgorandGetaddressbalanceGetResponse(AlgorandGetaddressbalanceGetRequest HttpRequest, AlgorandGetaddressbalanceGetResponse HttpResponse, bool bSucceeded, FAlgorandGetaddressbalanceGetDelegate Delegate) const;

    FString Url;
}
       
}
}