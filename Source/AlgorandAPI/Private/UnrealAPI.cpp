/** 
* Algorand Node API
*/

#include "UnrealApi.h"

namespace algorand{
namespace api {

UnrealApi::UnrealApi()
    : Url(TEXT("http://localhost"))
{
}

UnrealApi::~UnrealApi() {}

void UnrealApi::SetURL(const FString& InUrl)
{
    Url = InUrl;
}

}
}