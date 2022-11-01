#pragma once

#include "CoreMinimal.h"
#include "BaseModel.h"

namespace algorand {
    namespace vertices {

        class VerticesSDK::VerticesGetaddressbalanceGetRequest : public Request
        {
        public:
            virtual ~VerticesGetaddressbalanceGetRequest() {}

            TOptional<FString> Address;
        };

        class VerticesSDK::VerticesGetaddressbalanceGetResponse : public Response
        {
        public:
            ~VerticesGetaddressbalanceGetResponse() {}
            int Amount;
        };

        class VerticesSDK::VerticesPaymentTransactionGetRequest : public Request
        {
        public:
            virtual ~VerticesPaymentTransactionGetRequest() {}

            TOptional<FString> senderAddress;
            TOptional<FString> receiverAddress;
            TOptional<uint64_t> amount;
        };

        class VerticesSDK::VerticesPaymentTransactionGetResponse : public Response
        {
        public:
            ~VerticesPaymentTransactionGetResponse() {}
            int Amount;
        };
    }
}