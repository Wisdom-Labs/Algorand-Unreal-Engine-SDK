#pragma once

#include "Modules/ModuleManager.h"

#include "include/vertices/vertices.h"
#include "include/utils/base32.h"
#include "include/utils/base64.h"
#include "http_weak.h"


#include "unix_config.h"
#include <cstring>
#include "../Libs/include/sodium.h"

namespace algorand {
namespace vertices {

    class VERTICES_API VerticesSDK
    {
    public:
        VerticesSDK();
        ~VerticesSDK();
        void loadVerticesLibrary();
        void setHTTPCURLs();
        void createNewVertices(char* , short, char* , ret_code_t&);
        void vertices_ping_check(ret_code_t&);
        void vertices_version_check(ret_code_t&);

        class VerticesGetaddressbalanceGetRequest;
        class VerticesGetaddressbalanceGetResponse;

        DECLARE_DELEGATE_OneParam(FVerticesGetaddressbalanceGetDelegate, const VerticesGetaddressbalanceGetResponse&);

        void VerticesGetaddressbalanceGet(const VerticesGetaddressbalanceGetRequest&, const FVerticesGetaddressbalanceGetDelegate&);
    
    private:
        void* VerticesHandle;
        void* SodiumHandle;
        bool loaded_;
    };

}
}
