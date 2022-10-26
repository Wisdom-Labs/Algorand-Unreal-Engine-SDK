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
    }
}