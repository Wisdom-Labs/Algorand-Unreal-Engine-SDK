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

    class VerticesSDK
    {
    public:
        VerticesSDK();
        ~VerticesSDK() {};
        //ret_code_t vertices_evt_handler(vtc_evt_t* evt);
        ret_code_t create_new_account();
        ret_code_t load_existing_account();
        int testAlgorand();
    private:
        FString config_path, prv_key, pub_key;
    };

}
}
