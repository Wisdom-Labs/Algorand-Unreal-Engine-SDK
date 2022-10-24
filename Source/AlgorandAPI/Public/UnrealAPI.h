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

    private:
    FString Url;
}
       
}
}