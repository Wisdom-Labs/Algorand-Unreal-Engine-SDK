// Copyright 2022, Wisdom Labs. All Rights Reserved

#pragma once

#include "Modules/ModuleManager.h"
#include "include/vertices/vertices.h"
#include "include/utils/base32.h"
#include "include/utils/base64.h"
#include "http_weak.h"
#include "../Private/include/sodium.h"
#include <cstring>
#include "../Private/Account.h"
#include "../Private/SDKException.h"

/**
 * ToDo:
 * for asset bases on transactions and wallet provider
 * add like txs
 * Key Registration and Freeze, State Proof
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
            * @param indexerRpc indexer rpc url
            */
        void setIndexerRpc(const FString& indexerRpc);
     
        /**
         * @brief set rpc url of algorand node 
         * @param algoRpc algoRpc rpc url
         */
        void setAlgodRpc(const FString& algodRpc);

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
        void createNewVertices(char* , char*,  short, char* , ret_code_t&);

        /**
         * @brief convert mnemonic account to vertices account
         * @return return status of converting
         */
        ret_code_t convert_Account_Vertices();
     
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
        class VerticesRestoreWalletGetRequest;
        class VerticesRestoreWalletGetResponse;
        class VerticesInitializeNewWalletGetRequest;
        class VerticesInitializeNewWalletGetResponse;
        class VerticesGetBackupMnemonicPhraseGetRequest;
        class VerticesGetBackupMnemonicPhraseGetResponse;
        class VerticesGenerateMnemonicsGetRequest;
        class VerticesGenerateMnemonicsGetResponse;
        class VerticesGetaddressbalanceGetRequest;
        class VerticesGetaddressbalanceGetResponse;
        class VerticesPaymentTransactionGetRequest;
        class VerticesPaymentTransactionGetResponse;
        class VerticesAssetConfigTransactionGetRequest;
        class VerticesAssetConfigTransactionGetResponse;
        class VerticesAssetTransferTransactionGetRequest;
        class VerticesAssetTransferTransactionGetResponse;
        class VerticesApplicationCallTransactionGetRequest;
        class VerticesApplicationCallTransactionGetResponse;
        class VerticesArcAssetDetailsGetRequest;
        class VerticesArcAssetDetailsGetResponse;
        class VerticesAccountInformationGetRequest;
        class VerticesAccountInformationGetResponse;

        /// restore wallet callback
        DECLARE_DELEGATE_OneParam(FVerticesRestoreWalletGetDelegate, const VerticesRestoreWalletGetResponse&);

        /// initialize new wallet callback
        DECLARE_DELEGATE_OneParam(FVerticesInitializeNewWalletGetDelegate, const VerticesInitializeNewWalletGetResponse&);

        /// get backup mnemonic phrase callback
        DECLARE_DELEGATE_OneParam(FVerticesGetBackupMnemonicPhraseGetDelegate, const VerticesGetBackupMnemonicPhraseGetResponse&);

        /// generate mnemonics callback
        DECLARE_DELEGATE_OneParam(FVerticesGenerateMnemonicsGetDelegate, const VerticesGenerateMnemonicsGetResponse&);

        /// get balance callabck
        DECLARE_DELEGATE_OneParam(FVerticesGetaddressbalanceGetDelegate, const VerticesGetaddressbalanceGetResponse&);

        /// payment tx callabck
        DECLARE_DELEGATE_OneParam(FVerticesPaymentTransactionGetDelegate, const VerticesPaymentTransactionGetResponse&);

        /// asset config tx callabck
        DECLARE_DELEGATE_OneParam(FVerticesAssetConfigTransactionGetDelegate, const VerticesAssetConfigTransactionGetResponse&);

        /// asset transfer tx callabck
        DECLARE_DELEGATE_OneParam(FVerticesAssetTransferTransactionGetDelegate, const VerticesAssetTransferTransactionGetResponse&);

        /// application call tx callback
        DECLARE_DELEGATE_OneParam(FVerticesApplicationCallTransactionGetDelegate, const VerticesApplicationCallTransactionGetResponse&);

        void VerticesRestoreWalletGet(const VerticesRestoreWalletGetRequest&, const FVerticesRestoreWalletGetDelegate&);
        void VerticesInitializeNewWalletGet(const VerticesInitializeNewWalletGetRequest&, const FVerticesInitializeNewWalletGetDelegate&);
        void VerticesGetBackupMnemonicPhraseGet(const VerticesGetBackupMnemonicPhraseGetRequest&, const FVerticesGetBackupMnemonicPhraseGetDelegate&);
        void VerticesGenerateMnemonicsGet(const VerticesGenerateMnemonicsGetRequest&, const FVerticesGenerateMnemonicsGetDelegate&);
        void VerticesGetaddressbalanceGet(const VerticesGetaddressbalanceGetRequest&, const FVerticesGetaddressbalanceGetDelegate&);
        void VerticesPaymentTransactionGet(const VerticesPaymentTransactionGetRequest&, const FVerticesPaymentTransactionGetDelegate&);
        void VerticesAssetConfigTransactionGet(const VerticesAssetConfigTransactionGetRequest&, const FVerticesAssetConfigTransactionGetDelegate&);
        void VerticesAssetTransferTransactionGet(const VerticesAssetTransferTransactionGetRequest&, const FVerticesAssetTransferTransactionGetDelegate&);
        void VerticesApplicationCallTransactionGet(const VerticesApplicationCallTransactionGetRequest&, const FVerticesApplicationCallTransactionGetDelegate&);
    
    private:
        // algorand rpc info for creating vertices
        FString myIndexerRpc;
        FString myAlgodRpc;
        int myAlgoPort;
        FString myAlgoTokenHeader;

        // main algo account
        Account main_account;
     
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

