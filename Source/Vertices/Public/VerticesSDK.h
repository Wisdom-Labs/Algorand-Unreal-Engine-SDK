// Copyright 2022, Wisdom Labs. All Rights Reserved

#pragma once
#include "Modules/ModuleManager.h"
#include "include/vertices/vertices.h"
#include "include/utils/base32.h"
#include "include/utils/base64.h"
#include "http_weak.h"
#include "../Private/include/sodium.h"
#include <cstring>
#include "../Private/SDKException.h"

/**
 * ToDo:
 * for asset bases on transactions and wallet provider
 * add like txs
 * Key Registration, Asset Configuration, Freeze and Transafer, State Proof
 */

/**
 * Vertices SDK
 * how call thirdparty library and send request
 * and execute allocated delegate with response
 * ue5 async task is used for multi-tasking
 */

namespace algorand {
namespace vertices {

    class VERTICES_API VerticesSDK
    {
    public:
        VerticesSDK();
        ~VerticesSDK();

        /**
         * @brief set rpc url of algorand node 
         * @param algoRpc algoRpc rpc url
         */
        void setAlgoRpc(const FString& algoRpc);

        /**
         * @brief set rpc port of algorand node
         * @param algoPort algoPort rpc port
         */
        void setAlgoPort(const int& algoPort);

        /**
         * @brief set rpc tokenHeader of algorand node
         * @param algoTokenHeader algorand rpc tokenHeader
         */
        void setAlgoTokenHeader(const FString& algoTokenHeader);

        /**
         * @brief load vertices library from thirdparty module
         */
        void loadVerticesLibrary();

        /**
         * @brief pass its pointer of  updated functions through Vertices for HttpCurls 
         */
        void setHTTPCURLs();

        /**
         * @brief validate rpc info and set them to Vertices lib
         * and create new vertices instance and validate how the lib works
         */
        void InitVertices(ret_code_t&);

        /**
         * @brief create new Vertices with rpc info
         */
        void createNewVertices(char* , short, char* , ret_code_t&);

        /**
         * @brief create new account including pub_key and priv_key on Vertices lib
         * @return return status of creation account, if VTC_SUCCESS, successful, not failed
         */
        ret_code_t create_new_account();

        /**
         * @brief load existing account from files stored on disk
         * @return return status of loading keys, if VTC_SUCCESS, successful, not failed
         */
        ret_code_t load_existing_account();

        /**
         * @brief load public key which length is 32byte 
         * @return return status of loading keys, if VTC_SUCCESS, successful, not failed
         */
        FString load_pub_key();

        /**
         * @brief check whether vertices lib works using simple api request
         */
        void vertices_ping_check(ret_code_t&);

        /**
         * @brief check vertices vertions with api request
         */
        void vertices_version_check(ret_code_t&);

        /**
         * @brief check whether vertices actions is successful with err_code
         * @param err_code if VTC_SUCCESS, successful, not failed
         */
        void checkVTCSuccess(ret_code_t& err_code); // error handling

        /**
         * @brief check status of vertices actions with err_code
         * @param msg_ message as output for exception process
         * @param err_code if VTC_SUCCESS, successful, not failed
         */
        void checkVTCSuccess(char* msg_, ret_code_t& err_code);

        /**
         * @brief set Request and Response for building api request and its result
         */
        class VerticesGenerateWalletGetRequest;
        class VerticesGenerateWalletGetResponse;
        class VerticesGetaddressbalanceGetRequest;
        class VerticesGetaddressbalanceGetResponse;
        class VerticesPaymentTransactionGetRequest;
        class VerticesPaymentTransactionGetResponse;
        class VerticesApplicationCallTransactionGetRequest;
        class VerticesApplicationCallTransactionGetResponse;

        /// generate wallet callback
        DECLARE_DELEGATE_OneParam(FVerticesGenerateWalletGetDelegate, const VerticesGenerateWalletGetResponse&);

        /// get balance callabck
        DECLARE_DELEGATE_OneParam(FVerticesGetaddressbalanceGetDelegate, const VerticesGetaddressbalanceGetResponse&);

        /// payment tx callabck
        DECLARE_DELEGATE_OneParam(FVerticesPaymentTransactionGetDelegate, const VerticesPaymentTransactionGetResponse&);

        /// application call tx callback
        DECLARE_DELEGATE_OneParam(FVerticesApplicationCallTransactionGetDelegate, const VerticesApplicationCallTransactionGetResponse&);

        void VerticesGenerateWalletGet(const VerticesGenerateWalletGetRequest&, const FVerticesGenerateWalletGetDelegate&);
        void VerticesGetaddressbalanceGet(const VerticesGetaddressbalanceGetRequest&, const FVerticesGetaddressbalanceGetDelegate&);
        void VerticesPaymentTransactionGet(const VerticesPaymentTransactionGetRequest&, const FVerticesPaymentTransactionGetDelegate&);
        void VerticesApplicationCallTransactionGet(const VerticesApplicationCallTransactionGetRequest&, const FVerticesApplicationCallTransactionGetDelegate&);
    
    private:
        // algorand rpc info for creating vertices
        FString myAlgoRpc;
        int myAlgoPort;
        FString myAlgoTokenHeader;

        // mutex for multi-threads
        FCriticalSection m_Mutex;

        // variables for loading thirdparty libs
        void* VerticesHandle;
        void* SodiumHandle;

     
        bool loaded_;
        bool vertices_usable;
        FString config_path;
    };

}
}

