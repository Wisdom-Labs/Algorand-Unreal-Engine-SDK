#pragma once

#include "CoreMinimal.h"
#include "VerticesSDK.h"
#include "ThreadContextManager.h"

namespace {
    using Vertices = algorand::vertices::VerticesSDK;
}

namespace algorand{
namespace api{

class ALGORANDAPI_API UnrealApi
{
public:
    UnrealApi(TSharedPtr<algorand::vertices::ThreadContextManager>&);
    ~UnrealApi();

    /* Sets the URL Endpoint*/
    void SetURL(const FString& Url);


    DECLARE_DELEGATE_OneParam(FAlgorandGetaddressbalanceGetDelegate, const Vertices::VerticesGetaddressbalanceGetResponse&);
    DECLARE_DELEGATE_OneParam(FAlgorandLoadaccountinfoGetDelegate, const Vertices::VerticesLoadaccountinfoGetResponse&);

    void AlgorandGetaddressbalanceGet(const Vertices::VerticesGetaddressbalanceGetRequest& Request, const FAlgorandGetaddressbalanceGetDelegate& Delegate = FAlgorandGetaddressbalanceGetDelegate()) const;
    void AlgorandLoadaccountinfoGet(const Vertices::VerticesLoadaccountinfoGetRequest& Request, const FAlgorandLoadaccountinfoGetDelegate& Delegate = FAlgorandLoadaccountinfoGetDelegate()) const;
    /*void AlgorandGetApplicationIDGet(const Vertices::VerticesGetaddressbalanceGetRequest& Request, const FAlgorandGetaddressbalanceGetDelegate& Delegate = FAlgorandGetaddressbalanceGetDelegate()) const;
    void AlgorandSendPaymentTransactionGet(const Vertices::VerticesGetaddressbalanceGetRequest& Request, const FAlgorandGetaddressbalanceGetDelegate& Delegate = FAlgorandGetaddressbalanceGetDelegate()) const;
    void AlgorandSendApplicationTransactionGet(const Vertices::VerticesGetaddressbalanceGetRequest& Request, const FAlgorandGetaddressbalanceGetDelegate& Delegate = FAlgorandGetaddressbalanceGetDelegate()) const;*/

private:
    void OnAlgorandGetaddressbalanceGetResponse(const Vertices::VerticesGetaddressbalanceGetResponse& response, bool bSucceed, const FAlgorandGetaddressbalanceGetDelegate& Delegate) const;
    void OnAlgorandLoadaccountinfoGetResponse(const Vertices::VerticesLoadaccountinfoGetResponse& response, bool bSucceed, const FAlgorandLoadaccountinfoGetDelegate& Delegate) const;

    TSharedPtr<algorand::vertices::ThreadContextManager> threadContextManager_;

    FString Url;
};
       
}
}