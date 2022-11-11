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


    DECLARE_DELEGATE_OneParam(FAlgorandGetaddressbalanceGetDelegate, const Vertices::VerticesGetaddressbalanceGetResponse&);
    DECLARE_DELEGATE_OneParam(FAlgorandPaymentTransactionGetDelegate, const Vertices::VerticesPaymentTransactionGetResponse&);
    DECLARE_DELEGATE_OneParam(FAlgorandApplicationCallTransactionGetDelegate, const Vertices::VerticesApplicationCallTransactionGetResponse&);

    void AlgorandGetaddressbalanceGet(const Vertices::VerticesGetaddressbalanceGetRequest& Request, const FAlgorandGetaddressbalanceGetDelegate& Delegate = FAlgorandGetaddressbalanceGetDelegate()) const;
    void AlgorandPaymentTransactionGet(const Vertices::VerticesPaymentTransactionGetRequest& Request, const FAlgorandPaymentTransactionGetDelegate& Delegate = FAlgorandPaymentTransactionGetDelegate()) const;
    void AlgorandApplicationCallTransactionGet(const Vertices::VerticesApplicationCallTransactionGetRequest& Request, const FAlgorandApplicationCallTransactionGetDelegate& Delegate = FAlgorandApplicationCallTransactionGetDelegate()) const;

private:
    void OnAlgorandGetaddressbalanceGetResponse(const Vertices::VerticesGetaddressbalanceGetResponse& response, const FAlgorandGetaddressbalanceGetDelegate& Delegate) const;
    void OnAlgorandPaymentTransactionGetResponse(const Vertices::VerticesPaymentTransactionGetResponse& response, const FAlgorandPaymentTransactionGetDelegate& Delegate) const;
    void OnAlgorandApplicationCallTransactionGetResponse(const Vertices::VerticesApplicationCallTransactionGetResponse& response, const FAlgorandApplicationCallTransactionGetDelegate& Delegate) const;

    TSharedPtr<algorand::vertices::VerticesSDK> vertices_;

    FString Url;
};
       
}
}