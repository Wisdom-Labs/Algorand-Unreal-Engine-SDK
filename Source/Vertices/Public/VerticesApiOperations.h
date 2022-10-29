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

        class VerticesSDK::VerticesLoadaccountinfoGetRequest : public Request
        {
        public:
            virtual ~VerticesLoadaccountinfoGetRequest() {}

            TOptional<FString> Address;
        };

        class VerticesSDK::VerticesLoadaccountinfoGetResponse : public Response
        {
        public:
            ~VerticesLoadaccountinfoGetResponse() {}
            int Amount;
        };
    }
}