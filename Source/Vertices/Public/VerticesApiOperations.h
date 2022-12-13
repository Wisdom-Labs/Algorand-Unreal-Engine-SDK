// Copyright 2022, Wisdom Labs. All Rights Reserved

#pragma once
#include "CoreMinimal.h"
#include "BaseModel.h"

namespace algorand {
    namespace vertices {
        
        // Request for Generate wallet
        class VerticesSDK::VerticesGenerateWalletGetRequest : public Request
        {
        public:
            virtual ~VerticesGenerateWalletGetRequest() {}

        };

        // Response for Generate wallet
        class VerticesSDK::VerticesGenerateWalletGetResponse : public Response
        {
        public:
            ~VerticesGenerateWalletGetResponse() {}
            FString Address;
        };

        // Request for Get Address
        class VerticesSDK::VerticesGetaddressbalanceGetRequest : public Request
        {
        public:
            virtual ~VerticesGetaddressbalanceGetRequest() {}

            TOptional<FString> Address;
        };

        // Response for Get Address
        class VerticesSDK::VerticesGetaddressbalanceGetResponse : public Response
        {
        public:
            ~VerticesGetaddressbalanceGetResponse() {}
            uint64 Amount;
        };

        // Request for Payment TX
        class VerticesSDK::VerticesPaymentTransactionGetRequest : public Request
        {
        public:
            virtual ~VerticesPaymentTransactionGetRequest() {}

            TOptional<FString> senderAddress;
            TOptional<FString> receiverAddress;
            TOptional<uint64_t> amount;
            TOptional<FString> notes;
        };

        // Response for Payment TX
        class VerticesSDK::VerticesPaymentTransactionGetResponse : public Response
        {
        public:
            ~VerticesPaymentTransactionGetResponse() {}
            FString txID;
        };

        // Request for Application Call TX
        class VerticesSDK::VerticesApplicationCallTransactionGetRequest : public Request
        {
        public:
            virtual ~VerticesApplicationCallTransactionGetRequest() {}

            TOptional<FString> senderAddress;
            TOptional<uint64_t> app_ID;
        };

        // Response for Application Call TX
        class VerticesSDK::VerticesApplicationCallTransactionGetResponse : public Response
        {
        public:
            ~VerticesApplicationCallTransactionGetResponse() {}
            FString txID;
        };
    }
}