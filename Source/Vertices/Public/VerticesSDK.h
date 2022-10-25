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
    
    private:
        void* VerticesHandle;
        void* SodiumHandle;
    };

}
}
