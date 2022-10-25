#pragma once

#include "BaseModel.h"
#include "UnrealApi.h"

namespace algorand {
namespace api {
	
class UnrealApi::AlgorandGetaddressbalanceGetRequest : public Request
{
public:
    virtual ~AlgorandGetaddressbalanceGetRequest() {}
    
    TOptional<FString> Address;
};

class UnrealApi::AlgorandGetaddressbalanceGetResponse : public Response
{
public:
    virtual ~AlgorandGetaddressbalanceGetResponse() {}
};
}
}