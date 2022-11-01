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
    DECLARE_DELEGATE_OneParam(FAlgorandPaymentTransactionGetDelegate, const Vertices::VerticesPaymentTransactionGetResponse&);
    DECLARE_DELEGATE_OneParam(FAlgorandApplicationCallTransactionGetDelegate, const Vertices::VerticesApplicationCallTransactionGetResponse&);

    void AlgorandGetaddressbalanceGet(const Vertices::VerticesGetaddressbalanceGetRequest& Request, const FAlgorandGetaddressbalanceGetDelegate& Delegate = FAlgorandGetaddressbalanceGetDelegate()) const;
    void AlgorandPaymentTransactionGet(const Vertices::VerticesPaymentTransactionGetRequest& Request, const FAlgorandPaymentTransactionGetDelegate& Delegate = FAlgorandPaymentTransactionGetDelegate()) const;
    void AlgorandApplicationCallTransactionGet(const Vertices::VerticesApplicationCallTransactionGetRequest& Request, const FAlgorandApplicationCallTransactionGetDelegate& Delegate = FAlgorandApplicationCallTransactionGetDelegate()) const;

private:
    void OnAlgorandGetaddressbalanceGetResponse(const Vertices::VerticesGetaddressbalanceGetResponse& response, bool bSucceed, const FAlgorandGetaddressbalanceGetDelegate& Delegate) const;
    void OnAlgorandPaymentTransactionGetResponse(const Vertices::VerticesPaymentTransactionGetResponse& response, bool bSucceed, const FAlgorandPaymentTransactionGetDelegate& Delegate) const;
    void OnAlgorandApplicationCallTransactionGetResponse(const Vertices::VerticesApplicationCallTransactionGetResponse& response, bool bSucceed, const FAlgorandApplicationCallTransactionGetDelegate& Delegate) const;

    TSharedPtr<algorand::vertices::ThreadContextManager> threadContextManager_;

    FString Url;
};
       
}
}