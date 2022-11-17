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

        void setAlgoRpc(const FString& algoRpc);
        void setAlgoPort(const int& algoPort);
        void setAlgoTokenHeader(const FString& algoTokenHeader);

        void loadVerticesLibrary();
        void setHTTPCURLs();
        void InitVertices(ret_code_t&);
        void createNewVertices(char* , short, char* , ret_code_t&);
        ret_code_t create_new_account();
        ret_code_t load_existing_account();
        FString load_pub_key();
        void vertices_ping_check(ret_code_t&);
        void vertices_version_check(ret_code_t&);

        class VerticesGenerateWalletGetRequest;
        class VerticesGenerateWalletGetResponse;
        class VerticesGetaddressbalanceGetRequest;
        class VerticesGetaddressbalanceGetResponse;
        class VerticesPaymentTransactionGetRequest;
        class VerticesPaymentTransactionGetResponse;
        class VerticesApplicationCallTransactionGetRequest;
        class VerticesApplicationCallTransactionGetResponse;

        DECLARE_DELEGATE_OneParam(FVerticesGenerateWalletGetDelegate, const VerticesGenerateWalletGetResponse&);
        DECLARE_DELEGATE_OneParam(FVerticesGetaddressbalanceGetDelegate, const VerticesGetaddressbalanceGetResponse&);
        DECLARE_DELEGATE_OneParam(FVerticesPaymentTransactionGetDelegate, const VerticesPaymentTransactionGetResponse&);
        DECLARE_DELEGATE_OneParam(FVerticesApplicationCallTransactionGetDelegate, const VerticesApplicationCallTransactionGetResponse&);

        void VerticesGenerateWalletGet(const VerticesGenerateWalletGetRequest&, const FVerticesGenerateWalletGetDelegate&);
        void VerticesGetaddressbalanceGet(const VerticesGetaddressbalanceGetRequest&, const FVerticesGetaddressbalanceGetDelegate&);
        void VerticesPaymentTransactionGet(const VerticesPaymentTransactionGetRequest&, const FVerticesPaymentTransactionGetDelegate&);
        void VerticesApplicationCallTransactionGet(const VerticesApplicationCallTransactionGetRequest&, const FVerticesApplicationCallTransactionGetDelegate&);
    
    private:
        FString myAlgoRpc;
        int myAlgoPort;
        FString myAlgoTokenHeader;

        FCriticalSection m_Mutex;

        void* VerticesHandle;
        void* SodiumHandle;
        bool loaded_;
        bool vertices_usable;
        FString config_path;
    };

}
}
