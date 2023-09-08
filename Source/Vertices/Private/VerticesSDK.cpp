// Copyright 2022, Wisdom Labs. All Rights Reserved

#include "VerticesSDK.h"
#include "Misc/Paths.h"
#include "Misc/MessageDialog.h"
#include "Containers/StringConv.h"
#include "Internationalization/Text.h"
#include "Interfaces/IPluginManager.h"
#include "VerticesApiOperations.h"
#include "ResponseBuilers.h"
#include "HAL/UnrealMemory.h"
#include "AES.h"
#include <cstring>
#include "SDKException.h"
#include "include/vertices/vertices_http.h"
#include "Windows/AllowWindowsPlatformTypes.h"

using namespace std;

DECLARE_LOG_CATEGORY_EXTERN(LogMyAwesomeGame, Log, All);

#define PUBLIC_ADDRESS_LENGTH 58
#define LOCTEXT_NAMESPACE "FVerticesModule"

unsigned char user_password[] = "algorand-sdk";

// algo account struct 
typedef struct {
    unsigned char private_key[ADDRESS_LENGTH];  //!< 32-bytes private key
    account_info_t* vtc_account;               //!< pointer to Vertices account data
} account_t;

account_t sender_account;

#ifdef __cplusplus
extern "C++" {
#endif

ret_code_t vertices_evt_handler(vtc_evt_t* evt) {
    ret_code_t err_code = VTC_SUCCESS;

    try
    {
        switch (evt->type) {
            case VTC_EVT_TX_READY_TO_SIGN: {
                signed_transaction_t* tx = nullptr;
                err_code = vertices_event_tx_get(evt->bufid, &tx);
                if (err_code == VTC_SUCCESS) { 
                    UE_LOG(LogTemp, Display, TEXT("event process About to sign tx: data length %lu"), (unsigned long)(tx->payload_body_length));

                    // libsodium wants to have private and public keys concatenated
                    unsigned char keys[crypto_sign_ed25519_SECRETKEYBYTES] = { 0 };
                    memcpy(keys, sender_account.private_key, sizeof(sender_account.private_key));
                    memcpy(&keys[32],
                        sender_account.vtc_account->public_key,
                        ADDRESS_LENGTH);

                    // prepend "TX" to the payload before signing
                    unsigned char* to_be_signed = new unsigned char[tx->payload_body_length + 2];
                    to_be_signed[0] = 'T';
                    to_be_signed[1] = 'X';

                    // copy body
                    memcpy(&to_be_signed[2],
                        &tx->payload[tx->payload_header_length],
                        tx->payload_body_length);

                    // sign the payload
                    crypto_sign_ed25519_detached(tx->signature,
                        nullptr, to_be_signed, tx->payload_body_length + 2, keys);

                    char b64_signature[128] = { 0 };
                    size_t b64_signature_len = sizeof(b64_signature);
                    b64_encode((const char*)tx->signature,
                        sizeof(tx->signature),
                        b64_signature,
                        &b64_signature_len);
                    UE_LOG(LogTemp, Display, TEXT("event processSignature %s (%zu bytes)"), b64_signature, b64_signature_len);

                    // send event to send the signed TX
                    vtc_evt_t sched_evt;
                    sched_evt.type = VTC_EVT_TX_SENDING;
                    sched_evt.bufid = evt->bufid;
                    err_code = vertices_event_schedule(&sched_evt);
                }
            }
                break;

            case VTC_EVT_TX_SENDING: {
                // let's create transaction files which can then be used with `goal clerk ...`
                signed_transaction_t* tx = nullptr;
                err_code = vertices_event_tx_get(evt->bufid, &tx);

                // goal-generated transaction files are packed into a map of one element: `txn`.
                // the one-element map takes 4 bytes into our message packed payload <=> `txn`
                // we also add the `map` type before
                // which results in 5-bytes to be added before the payload at `payload_offset`
                char* payload = new char[tx->payload_body_length + 5];
                payload[0] = (char)(unsigned char)0x81; // starting flag for map of one element
                memcpy(&payload[1],
                    &tx->payload[tx->payload_header_length - 4],
                    tx->payload_body_length + 4);
            }
                break;

            default:
                break;
        }
    }
    catch (std::exception& ex)
    {
        FFormatNamedArguments Arguments;
        Arguments.Add(TEXT("MSG"), FText::FromString(ex.what()));
        FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Error", "vertices_evt_handler {MSG}"), Arguments));
        err_code = VTC_ERROR_INVALID_PARAM;
    }
    
    return err_code;
}

#ifdef __cplusplus
} /* extern "C" */
#endif

// provider to access vertices lib
provider_info_t providers;

// bridge witch connect vertices module to thirdparty lib
vertex_t m_vertex;

namespace algorand {
    namespace vertices {

        // load thirdparty libs
        VerticesSDK::VerticesSDK() {
            loaded_ = false;
            
            loadVerticesLibrary();
            
            if (!loaded_)
            {
                FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("ThirdPartyLibraryError", "Failed to load example third party library"));
                return;
            }

            UE_LOG(LogTemp, Display, TEXT("Loaded C-Vertices-sdk.dll & libsodium.dll from Third Party Plugin sample."));
            setHTTPCURLs();
            vertices_usable = true;
        }

        VerticesSDK::~VerticesSDK () {

            FPlatformProcess::FreeDllHandle(VerticesHandle);
            FPlatformProcess::FreeDllHandle(SodiumHandle);

            VerticesHandle = nullptr;
            SodiumHandle = nullptr;
        }

        void VerticesSDK::setIndexerRpc(const FString& indexerRpc) {
            myIndexerRpc = indexerRpc;
        }
        
        void VerticesSDK::setAlgodRpc(const FString& algodRpc) {
            myAlgodRpc = algodRpc;
        }

        void VerticesSDK::setAlgoPort(const int& algoPort) {
            myAlgoPort = algoPort;
        }

        void VerticesSDK::setAlgoTokenHeader(const FString& algoTokenHeader) {
            myAlgoTokenHeader = algoTokenHeader;
        }

        void VerticesSDK::loadVerticesLibrary() {
            // Get the base directory of this plugin
            FString BaseDir = IPluginManager::Get().FindPlugin("Algorand")->GetBaseDir();

            // Add on the relative location of the third party dll and load it
            FString VerticesPath, SodiumPath;

            VerticesPath = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/AlgorandLibrary/x64/Release/vertices.dll"));
            SodiumPath = FPaths::Combine(*BaseDir, TEXT("Source/Vertices/Libs/libsodium.dll"));

            VerticesHandle = !VerticesPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*VerticesPath) : nullptr;
            SodiumHandle = !SodiumPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*SodiumPath) : nullptr;

            if (VerticesHandle != nullptr && SodiumHandle != nullptr)
            {
                loaded_ = true;
                return;
            }
            else
                loaded_ = false;
        }

        void VerticesSDK::InitVertices(ret_code_t& err_code) {
            if ( !myAlgoTokenHeader.Contains("X-Algo-API-Token") && !myAlgoTokenHeader.Contains("x-api-key") && !myAlgoTokenHeader.Equals(""))
            {
                err_code = VTC_ERROR_INVALID_PARAM;
                FFormatNamedArguments Arguments;
                
                Arguments.Add(TEXT("MSG"), FText::FromString(myAlgoTokenHeader));
                FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Error","this token header is invalid value: {MSG}"), Arguments));
                checkVTCSuccess("Init Vertices", err_code);    
            }
            
            char* algodurl = new char[myAlgodRpc.Len()];
            char* indexerurl = new char[myIndexerRpc.Len()];
            char* tokenHeader = new char[myAlgoTokenHeader.Len()];
            memcpy(algodurl, TCHAR_TO_ANSI(*myAlgodRpc), myAlgodRpc.Len());
            memcpy(indexerurl, TCHAR_TO_ANSI(*myIndexerRpc), myIndexerRpc.Len());
            memcpy(tokenHeader, TCHAR_TO_ANSI(*myAlgoTokenHeader), myAlgoTokenHeader.Len());
            algodurl[myAlgodRpc.Len()] = 0;
            indexerurl[myIndexerRpc.Len()] = 0;
            tokenHeader[myAlgoTokenHeader.Len()] = 0;
    
            createNewVertices(algodurl,
                            indexerurl,
                                myAlgoPort,
                                tokenHeader,
                                err_code);
            vertices_ping_check(err_code);
            vertices_version_check(err_code);
            
            UE_LOG(LogTemp, Display, TEXT("Created new vertices net."));
        }

        // pass m_vertex through vertices lib
        void VerticesSDK::createNewVertices(char* algod_url, char* indexer_url, short port, char* server_token_header, ret_code_t& err_code) {
            providers.algod_url = algod_url;
            providers.indexer_url = indexer_url;
            providers.port = port;
            providers.header = server_token_header;

            int ret = sodium_init();
            UE_LOG(LogTemp, Warning, TEXT("Vertices sodium_init err_code %d"), ret);

            m_vertex.provider = &providers;
            m_vertex.vertices_evt_handler = &vertices_evt_handler;

            // create new vertex
            err_code = vertices_new(&m_vertex);
            UE_LOG(LogTemp, Warning, TEXT("Vertices new err_code %d"), err_code);
            checkVTCSuccess("Vertices vertices_new", err_code);
        }

        void VerticesSDK::vertices_ping_check(ret_code_t& err_code) 
        {
            // making sure the provider is accessible
            err_code = vertices_ping();
            UE_LOG(LogTemp, Warning, TEXT("Vertices ping err_code %d"), err_code);
            checkVTCSuccess("Vertices ping",err_code);
        }

        void VerticesSDK::vertices_version_check(ret_code_t& err_code) 
        {
            // ask for provider version
            provider_version_t version = { 0 };
            err_code = vertices_version(&version);
            if (err_code == VTC_ERROR_OFFLINE) {
                UE_LOG(LogTemp, Warning, TEXT("Version might not be accurate : old value is being used %d"), err_code);
            }
            else {
                UE_LOG(LogTemp, Warning, TEXT("err_code vertices_version %d"), err_code);
            }

            checkVTCSuccess("Vertices version",err_code);
            UE_LOG(LogTemp, Warning, TEXT("🏎 Running on %s v.%u.%u.%u"),
                version.network,
                version.major,
                version.minor,
                version.patch);
        }

        ret_code_t VerticesSDK::convert_Account_Vertices()
        {
            ret_code_t err_code;
            // copy private key to vertices account
            // if(main_account.secret_key.size() != ADDRESS_LENGTH)
            //     err_code = VTC_ERROR_INVALID_PARAM;
            // checkVTCSuccess("Secret key length is not 32 byte", err_code);

            memset(sender_account.private_key, 0 , ADDRESS_LENGTH);
            memcpy(sender_account.private_key, (unsigned char*)main_account.secret_key.data(), ADDRESS_LENGTH);

            // copy public key to vertices account
            unsigned char pub_key[ADDRESS_LENGTH]; 
            memset(pub_key, 0 , ADDRESS_LENGTH);
            memcpy(pub_key, main_account.public_key().data(), ADDRESS_LENGTH);

            err_code = vertices_account_new_from_bin((char *)pub_key, &sender_account.vtc_account);
            UE_LOG(LogTemp, Warning, TEXT("err_code vertices_account_new_from_bin %d"), err_code);

            return err_code;
        }

        // restore account with mnemonic private keys
        void VerticesSDK::VerticesRestoreWalletGet(const VerticesRestoreWalletGetRequest& Request, const FVerticesRestoreWalletGetDelegate& delegate)
        {           
            ret_code_t err_code = VTC_SUCCESS;
            while (1)
            {
                FScopeLock lock(&m_Mutex);

                if (vertices_usable)
                {
                    VerticesSDK::VerticesRestoreWalletGetResponse response;
                    vertices_usable = false;
            
                    try
                    {
                        if(!loaded_)
                        {
                            UE_LOG(LogTemp, Warning, TEXT("Failed loading of dll libraries."));
                            err_code = VTC_ERROR_ASSERT_FAILS;
                            checkVTCSuccess("Failed loading of dll libraries.", err_code);
                        }
                        auto mnemonics = StringCast<ANSICHAR>(*(Request.Mnemonics.GetValue()));
                        auto s_mnemonics = mnemonics.Get();
                        main_account = Account::from_mnemonic(s_mnemonics);
                        
                        // copy private key to vertices account
                        if(main_account.secret_key.size() != ADDRESS_LENGTH)
                            checkVTCSuccess("Secret key length is not 32 byte", err_code);
                        
                        UE_LOG(LogTemp, Display, TEXT("💳 Created Core account: %s"), *FString(main_account.address.as_string.data()));
                        
                        response = response_builders::buildRestoreWalletResponse(FString(main_account.address.as_string.data()));
                        response.SetSuccessful(true);
                    }
                    catch(SDKException& e)
                    {
                        response.SetSuccessful(false);
                        response.SetResponseString(FString(e.what()));   
                    }
                    catch(std::exception& ex)
                    {
                        response.SetSuccessful(false);
                        response.SetResponseString(FString(ex.what()));
                    }

                    AsyncTask(ENamedThreads::GameThread, [delegate, response]()
                        {
                            delegate.ExecuteIfBound(response);
                        });

                    vertices_usable = true;
                    break;
                }
            };
        }

        // initialize new account
        void VerticesSDK::VerticesInitializeNewWalletGet(const VerticesInitializeNewWalletGetRequest& Request, const FVerticesInitializeNewWalletGetDelegate& delegate)
        {           
            AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Request, delegate]()
            {
                ret_code_t err_code = VTC_SUCCESS;
                while (1) {
                    FScopeLock lock(&m_Mutex);
                    
                    if (vertices_usable) {
                        VerticesSDK::VerticesInitializeNewWalletGetResponse response;
                        vertices_usable = false;
                        
                        try
                        {
                            if(!loaded_)
                            {
                                UE_LOG(LogTemp, Warning, TEXT("Failed loading of dll libraries."));
                                err_code = VTC_ERROR_ASSERT_FAILS;
                                checkVTCSuccess("Failed loading of dll libraries.", err_code);
                            }
                            main_account = Account::initialize_new();

                            // copy private key to vertices account
                            if(main_account.secret_key.size() != ADDRESS_LENGTH)
                                checkVTCSuccess("Secret key length is not 32 byte", err_code);
                            
                            UE_LOG(LogTemp, Display, TEXT("💳 Created Core account: %s"), *FString(main_account.address.as_string.data()));
                            
                            response = response_builders::buildInitializeNewWalletResponse(FString(main_account.address.as_string.data()));
                            response.SetSuccessful(true);
                        }
                        catch(SDKException& e)
                        {
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(e.what()));
                        }
                        catch(std::exception& ex)
                        {
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(ex.what()));
                        }

                        AsyncTask(ENamedThreads::GameThread, [delegate, response]()
                        {
                            delegate.ExecuteIfBound(response);
                        });

                        vertices_usable = true;
                        break;
                    }
                }
            });
        }

        // get backup mnemonic phrase
        void VerticesSDK::VerticesGetBackupMnemonicPhraseGet(const VerticesGetBackupMnemonicPhraseGetRequest& Request, const FVerticesGetBackupMnemonicPhraseGetDelegate& delegate)
        {           
            AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Request, delegate]()
            {
                ret_code_t err_code = VTC_SUCCESS;
                while (1) {
                    FScopeLock lock(&m_Mutex);

                    if (vertices_usable) {
                        VerticesSDK::VerticesGetBackupMnemonicPhraseGetResponse response;
                        vertices_usable = false;
                        
                        try
                        {
                            if(!loaded_)
                            {
                                UE_LOG(LogTemp, Warning, TEXT("Failed loading of dll libraries."));
                                err_code = VTC_ERROR_ASSERT_FAILS;
                                checkVTCSuccess("Failed loading of dll libraries.", err_code);
                            }
                            bytes secret_key, pub_key;

                            if(sender_account.vtc_account->public_key == nullptr || sender_account.private_key == nullptr)
                            {
                                err_code = VTC_ERROR_INVALID_STATE;
                                checkVTCSuccess("Secret key and public key aren't on vertices", err_code);
                            }
                             
                            pub_key = bytes(sender_account.vtc_account->public_key, sender_account.vtc_account->public_key + ADDRESS_LENGTH);
                            secret_key = bytes(sender_account.private_key, sender_account.private_key + ADDRESS_LENGTH);
                            
                            Account account{pub_key, secret_key};

                            std::string mnemonics = account.mnemonic();
                            UE_LOG(LogTemp, Warning, TEXT("VerticesGetBackupMnemonicPhraseGet mnemonics of backup %s"), *FString(mnemonics.data()));
                            
                            response = response_builders::buildGetBackupMnemonicPhraseResponse(FString(mnemonics.data()));
                            response.SetSuccessful(true);
                        }
                        catch(SDKException& e)
                        {
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(e.what()));   
                        }
                        catch(std::exception& ex)
                        {
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(ex.what()));
                        }

                        AsyncTask(ENamedThreads::GameThread, [delegate, response]()
                        {
                            delegate.ExecuteIfBound(response);
                        });

                        vertices_usable = true;
                        break;
                    }
                }
            });
        }

        // generate mnemonics
        void VerticesSDK::VerticesGenerateMnemonicsGet(const VerticesGenerateMnemonicsGetRequest& Request, const FVerticesGenerateMnemonicsGetDelegate& delegate)
        {           
            AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Request, delegate]()
            {
                ret_code_t err_code = VTC_SUCCESS;
                while (1) {
                    FScopeLock lock(&m_Mutex);

                    if (vertices_usable) {
                        VerticesSDK::VerticesGenerateMnemonicsGetResponse response;
                        vertices_usable = false;
                        
                        try
                        {
                            if(!loaded_)
                            {
                                UE_LOG(LogTemp, Warning, TEXT("Failed loading of dll libraries."));
                                err_code = VTC_ERROR_ASSERT_FAILS;
                                checkVTCSuccess("Failed loading of dll libraries.", err_code);
                            }
                            Account account = Account::initialize_new();
                            
                            std::string mnemonics = account.mnemonic();
                            UE_LOG(LogTemp, Warning, TEXT("VerticesGetBackupMnemonicPhraseGet mnemonics of backup %s"), *FString(mnemonics.data()));
                            
                            response = response_builders::buildGenerateMnemonicsResponse(FString(mnemonics.data()));
                            response.SetSuccessful(true);
                        }
                        catch(SDKException& e)
                        {
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(e.what()));   
                        }
                        catch(std::exception& ex)
                        {
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(ex.what()));
                        }

                        AsyncTask(ENamedThreads::GameThread, [delegate, response]()
                        {
                            delegate.ExecuteIfBound(response);
                        });

                        vertices_usable = true;
                        break;
                    }
                }
            });
        }

        void VerticesSDK::VerticesGetaddressbalanceGet(const VerticesGetaddressbalanceGetRequest& Request, const FVerticesGetaddressbalanceGetDelegate& delegate)
        {            
            AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Request, delegate]()
            {
                ret_code_t err_code = VTC_SUCCESS;
                account_t test_account;
                while (1) {
                    FScopeLock lock(&m_Mutex);
                    
                    if (vertices_usable) {
                        VerticesSDK::VerticesGetaddressbalanceGetResponse response;
                        vertices_usable = false;

                        try
                        {
                            if(!loaded_)
                            {
                                UE_LOG(LogTemp, Warning, TEXT("Failed loading of dll libraries."));
                                err_code = VTC_ERROR_ASSERT_FAILS;
                                checkVTCSuccess("Failed loading of dll libraries.", err_code);
                            }
                            // validation Request
                            auto auto_address = StringCast<ANSICHAR>(*(Request.Address.GetValue()));
                            if ( auto_address.Length() == 0 )
                            {
                                err_code = VTC_ERROR_INVALID_PARAM;
                                checkVTCSuccess("When loading address from request , an error occured", err_code);   
                            }
                            
                            InitVertices(err_code);
                            checkVTCSuccess("When initing vertices network, an error occured", err_code);

                            memset(test_account.private_key, 0, 32);
                            test_account.vtc_account = nullptr;
                            
                            const FString& address = Request.Address.GetValue();
                            
                            err_code = vertices_account_new_from_b32((char*)TCHAR_TO_ANSI(*address), &test_account.vtc_account);
                            checkVTCSuccess("vertices_account_new_from_b32 error occured.", err_code);
                            UE_LOG(LogTemp, Warning, TEXT("Amount of account on AlgorandGetaddressbalanceGet %d"), test_account.vtc_account->amount);

                            response = response_builders::buildGetBalanceResponse(test_account.vtc_account->amount);
                            response.SetSuccessful(true);

                            err_code = vertices_account_free(test_account.vtc_account);
                            checkVTCSuccess("vertices_account_free error occured.", err_code);
                        }
                        catch (SDKException& e)
                        {
                            UE_LOG(LogTemp, Error, TEXT("👉 get balance error: %s"), e.what());
                            
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(e.what()));
                        }
                        catch (std::exception& ex)
                        {
                            UE_LOG(LogTemp, Error, TEXT("👉 get balance error: %s"), ex.what());
                            
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(ex.what()));
                        }

                        // after build response, execute delegate to run callback with response
                        AsyncTask(ENamedThreads::GameThread, [delegate, response]()
                        {
                            delegate.ExecuteIfBound(response);
                        });

                        vertices_usable = true;
                        break;
                    }
                    
                }
            });
        }

        void VerticesSDK::VerticesPaymentTransactionGet(const VerticesPaymentTransactionGetRequest& Request, const FVerticesPaymentTransactionGetDelegate& delegate)
        {
            AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Request, delegate]()
            {
                ret_code_t err_code = VTC_SUCCESS;
                account_t receiver_account;
                while (1) {
                    FScopeLock lock(&m_Mutex);

                    if (vertices_usable) {
                        VerticesSDK::VerticesPaymentTransactionGetResponse response;
                        vertices_usable = false;

                        try
                        {
                            if(!loaded_)
                            {
                                UE_LOG(LogTemp, Warning, TEXT("Failed loading of dll libraries."));
                                err_code = VTC_ERROR_ASSERT_FAILS;
                                checkVTCSuccess("Failed loading of dll libraries.", err_code);
                            }
                            // validation Request
                            auto auto_notes = StringCast<ANSICHAR>(*(Request.notes.GetValue()));
                            char* notes = (char *)auto_notes.Get();

                            if(strlen(notes) == 0)
                                notes = "Payment Transaction";
                            
                            if ( Request.receiverAddress.GetValue().Len() != PUBLIC_ADDRESS_LENGTH )
                            {
                                err_code = VTC_ERROR_INVALID_ADDR;
                                checkVTCSuccess("Please input address with correct length.", err_code);
                            }
                            InitVertices(err_code);
                            checkVTCSuccess("When initing vertices network, an error occured", err_code);

                            err_code = convert_Account_Vertices();
                            checkVTCSuccess("Vertices convert_Account_Vertices error", err_code);

                            if (sender_account.vtc_account->amount < 1000) {
                                FFormatNamedArguments Arguments;
                                Arguments.Add(TEXT("Address"), FText::FromString(sender_account.vtc_account->public_b32));
                                FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Warning", "Go to https://bank.testnet.algorand.network/, dispense Algos to: {Address}"), Arguments));

                                UE_LOG(LogTemp, Warning, TEXT("👉 Go to https://bank.testnet.algorand.network/, dispense Algos to: %s"), *FString(sender_account.vtc_account->public_b32));
                                UE_LOG(LogTemp, Warning,
                                    TEXT("🙄 Amount available on account is too low to pass a transaction, consider adding Algos"));
                                UE_LOG(LogTemp, Warning, TEXT("😎 Then wait for a few seconds for transaction to pass..."));
                                
                                err_code = VTC_ERROR_ASSERT_FAILS;
                                checkVTCSuccess("Amount available on account is too low to pass a transaction, consider adding Algos", err_code);
                            }
                            
                            if (sender_account.vtc_account->amount < Request.amount.GetValue())
                            {
                                FFormatNamedArguments Arguments;
                                Arguments.Add(TEXT("Address"), FText::FromString(sender_account.vtc_account->public_b32));
                                FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Warning", "To send algos to {Address}, you should add more algos"), Arguments));
                                err_code = VTC_ERROR_ASSERT_FAILS;
                                checkVTCSuccess("Amount available on account is too low to pass a transaction, consider adding Algos", err_code);
                            }
                            const FString& address = Request.receiverAddress.GetValue();
                            
                            err_code = vertices_account_new_from_b32((char*)TCHAR_TO_ANSI(*address), &receiver_account.vtc_account);
                            checkVTCSuccess("vertices_account_new_from_b32 error occured.", err_code);
                            UE_LOG(LogTemp, Warning, TEXT("Amount of receiver_account on Payment TX %d"), receiver_account.vtc_account->amount);
                            
                            err_code =
                                vertices_transaction_pay_new(sender_account.vtc_account,
                                    (char *)receiver_account.vtc_account->public_key /* or ACCOUNT_RECEIVER */,
                                    (uint64_t)Request.amount.GetValue(),
                                    notes);
                            checkVTCSuccess("vertices_transaction_pay_new error occured", err_code);

                            unsigned char* txID = nullptr;
                            txID = new unsigned char[TRANSACTION_HASH_STR_MAX_LENGTH];

                            size_t queue_size = 1;
                            while (queue_size && err_code == VTC_SUCCESS) {
                                err_code = vertices_event_process(&queue_size, txID);
                            }

                            checkVTCSuccess("vertices_event_process error occured", err_code);
                            UE_LOG(LogTemp, Warning, TEXT("err_code Payment TX ID Success, %s"), txID);

                            err_code = vertices_account_free(sender_account.vtc_account);
                            checkVTCSuccess("vertices_account_free error occured", err_code);
                            UE_LOG(LogTemp, Warning, TEXT("VerticesPaymentTransactionGetRequest Success"));

                            response = response_builders::buildPaymentTransactionResponse(FString(UTF8_TO_TCHAR(txID)));
                            response.SetSuccessful(true);

                            //free(txID);
                        }
                        catch(SDKException& e)
                        {
                            UE_LOG(LogTemp, Error, TEXT("👉 payment tx error: %s"), UTF8_TO_TCHAR(e.what()));
                            
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(e.what()));
                        }
                        catch(std::exception& ex)
                        {
                            UE_LOG(LogTemp, Error, TEXT("👉 payment tx error: %s"), UTF8_TO_TCHAR(ex.what()));
                            
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(ex.what()));
                        }

                        // after build response, execute delegate to run callback with response
                        AsyncTask(ENamedThreads::GameThread, [delegate, response]()
                        {
                            delegate.ExecuteIfBound(response);
                        });

                        vertices_usable = true;
                        break;
                    }
                }
            });
        }
        
        void VerticesSDK::VerticesAssetConfigTransactionGet(const VerticesAssetConfigTransactionGetRequest& Request, const FVerticesAssetConfigTransactionGetDelegate& delegate)
        {
            AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Request, delegate]()
            {
                ret_code_t err_code = VTC_SUCCESS;
                account_t M_Account = {0}, R_Account = {0}, F_Account = {0}, C_Account = {0};  // 5 accounts for acfg
                while (1) {
                    FScopeLock lock(&m_Mutex);

                    if (vertices_usable) {
                        VerticesSDK::VerticesAssetConfigTransactionGetResponse response;
                        vertices_usable = false;

                        try
                        {
                            if(!loaded_)
                            {
                                UE_LOG(LogTemp, Warning, TEXT("Failed loading of dll libraries."));
                                err_code = VTC_ERROR_ASSERT_FAILS;
                                checkVTCSuccess("Failed loading of dll libraries.", err_code);
                            }
                            // validation Request
                            auto auto_notes = StringCast<ANSICHAR>(*(Request.Notes.GetValue()));        // notes
                            char* notes = (char *)auto_notes.Get();

                            if(strlen(notes) == 0)
                                notes = "Asset Config Transaction";

                            if(Request.UnitName.GetValue().Len() > 8)
                            {
                                err_code = VTC_ERROR_INVALID_PARAM;
                                checkVTCSuccess("Transaction Asset Unit Name is too big.", err_code);
                            }
                            
                            auto auto_unit_name = StringCast<ANSICHAR>(*(Request.UnitName.GetValue()));        // unit name        
                            char* unit_name = (char *)auto_unit_name.Get();

                            auto auto_asset_name = StringCast<ANSICHAR>(*(Request.AssetName.GetValue()));        // asset name        
                            char* asset_name = (char *)auto_asset_name.Get();

                            auto auto_url = StringCast<ANSICHAR>(*(Request.Url.GetValue()));        // url        
                            char* url = (char *)auto_url.Get();

                            if ( Request.Creator.GetValue().Len() != PUBLIC_ADDRESS_LENGTH )
                            {
                                err_code = VTC_ERROR_INVALID_ADDR;
                                checkVTCSuccess("Please input creator address with correct length.", err_code);
                            }

                            if ( Request.Manager.GetValue().Len() != PUBLIC_ADDRESS_LENGTH)
                            {
                                err_code = VTC_ERROR_INVALID_ADDR;
                                checkVTCSuccess("Please input manager address with correct length.", err_code);
                            }
                            
                            if ( Request.Reserve.GetValue().Len() != PUBLIC_ADDRESS_LENGTH && Request.Reserve.GetValue().Len() != 0)
                            {
                                err_code = VTC_ERROR_INVALID_ADDR;
                                checkVTCSuccess("Please input reserve address with correct length.", err_code);
                            }

                            if ( Request.Freeze.GetValue().Len() != PUBLIC_ADDRESS_LENGTH && Request.Freeze.GetValue().Len() != 0)
                            {
                                err_code = VTC_ERROR_INVALID_ADDR;
                                checkVTCSuccess("Please input freeze address with correct length.", err_code);
                            }

                            if ( Request.Clawback.GetValue().Len() != PUBLIC_ADDRESS_LENGTH && Request.Clawback.GetValue().Len() != 0 )
                            {
                                err_code = VTC_ERROR_INVALID_ADDR;
                                checkVTCSuccess("Please input clawback address with correct length.", err_code);
                            }
                            
                            InitVertices(err_code);
                            checkVTCSuccess("When initing vertices network, an error occured", err_code);

                            err_code = convert_Account_Vertices();
                            checkVTCSuccess("Vertices convert_Account_Vertices error", err_code);

                            if (sender_account.vtc_account->amount < 1000) {
                                FFormatNamedArguments Arguments;
                                Arguments.Add(TEXT("Address"), FText::FromString(sender_account.vtc_account->public_b32));
                                FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Warning", "Go to https://bank.testnet.algorand.network/, dispense Algos to: {Address}"), Arguments));

                                UE_LOG(LogTemp, Warning, TEXT("👉 Go to https://bank.testnet.algorand.network/, dispense Algos to: %s"), *FString(sender_account.vtc_account->public_b32));
                                UE_LOG(LogTemp, Warning,
                                    TEXT("🙄 Amount available on account is too low to pass a transaction, consider adding Algos"));
                                UE_LOG(LogTemp, Warning, TEXT("😎 Then wait for a few seconds for transaction to pass..."));
                                
                                err_code = VTC_ERROR_ASSERT_FAILS;
                                checkVTCSuccess("Amount available on account is too low to pass a transaction, consider adding Algos", err_code);
                            }

                            FString manager = Request.Manager.GetValue();
                            if ( manager.IsEmpty() )
                                manager = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAY5HFKQ";
                            
                            err_code = vertices_account_new_from_b32((char*)TCHAR_TO_ANSI(*manager), &M_Account.vtc_account);
                            checkVTCSuccess("vertices_account_new_from_b32 error occured.", err_code);
                            UE_LOG(LogTemp, Warning, TEXT("manager account on Asset Config TX %d"), M_Account.vtc_account->amount);
                            
                            FString reserve = Request.Reserve.GetValue();
                            if ( reserve.IsEmpty())
                                reserve = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAY5HFKQ";
                            
                            err_code = vertices_account_new_from_b32((char*)TCHAR_TO_ANSI(*reserve), &R_Account.vtc_account);
                            checkVTCSuccess("vertices_account_new_from_b32 error occured.", err_code);
                            UE_LOG(LogTemp, Warning, TEXT("reserve account on Asset Config TX %d"), R_Account.vtc_account->amount);
                            

                            FString freeze = Request.Freeze.GetValue();
                            if ( freeze.IsEmpty() )
                                freeze = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAY5HFKQ";
                                
                            err_code = vertices_account_new_from_b32((char*)TCHAR_TO_ANSI(*freeze), &F_Account.vtc_account);
                            checkVTCSuccess("vertices_account_new_from_b32 error occured.", err_code);
                            UE_LOG(LogTemp, Warning, TEXT("freeze account on Asset Config TX %d"), F_Account.vtc_account->amount);   

                            FString clawback = Request.Clawback.GetValue();
                            if ( clawback.IsEmpty() )
                                clawback = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAY5HFKQ";
                                
                            err_code = vertices_account_new_from_b32((char*)TCHAR_TO_ANSI(*clawback), &C_Account.vtc_account);
                            checkVTCSuccess("vertices_account_new_from_b32 error occured.", err_code);
                            UE_LOG(LogTemp, Warning, TEXT("clawback account on Asset Config TX %d"), C_Account.vtc_account->amount);   
                            
                            err_code =
                                vertices_transaction_asset_cfg(sender_account.vtc_account,
                                    (char *)M_Account.vtc_account->public_key /* or ACCOUNT_MANAGER */,
                                    (char *)R_Account.vtc_account->public_key /* or ACCOUNT_RESERVE */,
                                    (char *)F_Account.vtc_account->public_key /* or ACCOUNT_FREEZE */,
                                    (char *)C_Account.vtc_account->public_key /* or ACCOUNT_CLAWBACK */,
                                    (uint64_t)Request.AssetId.GetValue(),
                                    (uint64_t)Request.Total.GetValue(),
                                    (uint64_t)Request.Decimals.GetValue(),
                                    (uint8_t)Request.IsFrozen.GetValue(),
                                    unit_name,
                                    asset_name,
                                    url,
                                    notes);
                            checkVTCSuccess("vertices_transaction_asset_config error occured", err_code);
                            
                            unsigned char* txID = nullptr;
                            txID = new unsigned char[TRANSACTION_HASH_STR_MAX_LENGTH];

                            size_t queue_size = 1;
                            while (queue_size && err_code == VTC_SUCCESS) {
                                err_code = vertices_event_process(&queue_size, txID);
                            }

                            checkVTCSuccess("vertices_event_process error occured", err_code);
                            UE_LOG(LogTemp, Warning, TEXT("err_code Asset Config TX ID Success, %s"), txID);

                            err_code = vertices_account_free(sender_account.vtc_account);
                            checkVTCSuccess("vertices_account_free error occured", err_code);
                            UE_LOG(LogTemp, Warning, TEXT("VerticesAssetConfigTransactionGetRequest Success"));

                            InitVertices(err_code);
                            checkVTCSuccess("When reiniting vertices network, an error occured", err_code);

                            UE_LOG(LogTemp, Warning, TEXT("Asset Config TX ASSET TX Success, %s"), UTF8_TO_TCHAR(txID));
                            uint64 asset_id;
                            
                            do{
                                err_code = vertices_asset_id_get(txID, &asset_id);
                                UE_LOG(LogTemp, Warning, TEXT("Asset Config TX ASSET ID Success, %d"), asset_id);
                            }
                            while (err_code != VTC_SUCCESS);

                            UE_LOG(LogTemp, Warning, TEXT("Asset Config TX ASSET ID Success, %d"), asset_id);
                            response = response_builders::buildAssetConfigTransactionResponse(FString(UTF8_TO_TCHAR(txID)), asset_id);
                            response.SetSuccessful(true);

                            //free(txID);
                        }
                        catch(SDKException& e)
                        {
                            UE_LOG(LogTemp, Error, TEXT("👉 asset config tx error: %s"), UTF8_TO_TCHAR(e.what()));
                            
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(e.what()));
                        }
                        catch(std::exception& ex)
                        {
                            UE_LOG(LogTemp, Error, TEXT("👉 asset config tx error: %s"), UTF8_TO_TCHAR(ex.what()));
                            
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(ex.what()));
                        }

                        // after build response, execute delegate to run callback with response
                        AsyncTask(ENamedThreads::GameThread, [delegate, response]()
                        {
                            delegate.ExecuteIfBound(response);
                        });

                        vertices_usable = true;
                        break;
                    }
                }
            });
        }

        void VerticesSDK::VerticesAssetTransferTransactionGet(const VerticesAssetTransferTransactionGetRequest& Request, const FVerticesAssetTransferTransactionGetDelegate& delegate)
        {
            AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Request, delegate]()
            {
                ret_code_t err_code = VTC_SUCCESS;
                account_t asnd_account, arcv_account;
                while (1) {
                    FScopeLock lock(&m_Mutex);

                    if (vertices_usable) {
                        VerticesSDK::VerticesAssetTransferTransactionGetResponse response;
                        vertices_usable = false;

                        try
                        {
                            if(!loaded_)
                            {
                                UE_LOG(LogTemp, Warning, TEXT("Failed loading of dll libraries."));
                                err_code = VTC_ERROR_ASSERT_FAILS;
                                checkVTCSuccess("Failed loading of dll libraries.", err_code);
                            }
                            // validation Request
                            auto auto_notes = StringCast<ANSICHAR>(*(Request.notes.GetValue()));
                            char* notes = (char *)auto_notes.Get();

                            if(strlen(notes) == 0)
                                notes = "Asset Transfer Transaction";

                            if ( Request.senderAddress.GetValue().Len() != PUBLIC_ADDRESS_LENGTH )
                            {
                                err_code = VTC_ERROR_INVALID_ADDR;
                                checkVTCSuccess("Please input address with correct length.", err_code);
                            }
                            
                            if ( Request.receiverAddress.GetValue().Len() != PUBLIC_ADDRESS_LENGTH )
                            {
                                err_code = VTC_ERROR_INVALID_ADDR;
                                checkVTCSuccess("Please input address with correct length.", err_code);
                            }
                            
                            InitVertices(err_code);
                            checkVTCSuccess("When initing vertices network, an error occured", err_code);

                            err_code = convert_Account_Vertices();
                            checkVTCSuccess("Vertices convert_Account_Vertices error", err_code);

                            if (sender_account.vtc_account->amount < 1000) {
                                FFormatNamedArguments Arguments;
                                Arguments.Add(TEXT("Address"), FText::FromString(sender_account.vtc_account->public_b32));
                                FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Warning", "Go to https://bank.testnet.algorand.network/, dispense Algos to: {Address}"), Arguments));

                                UE_LOG(LogTemp, Warning, TEXT("👉 Go to https://bank.testnet.algorand.network/, dispense Algos to: %s"), *FString(sender_account.vtc_account->public_b32));
                                UE_LOG(LogTemp, Warning,
                                    TEXT("🙄 Amount available on account is too low to pass a transaction, consider adding Algos"));
                                UE_LOG(LogTemp, Warning, TEXT("😎 Then wait for a few seconds for transaction to pass..."));
                                
                                err_code = VTC_ERROR_ASSERT_FAILS;
                                checkVTCSuccess("Amount available on account is too low to pass a transaction, consider adding Algos", err_code);
                            }
                            
                            const FString& asnd = Request.senderAddress.GetValue();
                            const FString& arcv = Request.receiverAddress.GetValue();
                            
                            err_code = vertices_account_new_from_b32((char*)TCHAR_TO_ANSI(*asnd), &asnd_account.vtc_account);
                            checkVTCSuccess("vertices_account_new_from_b32 error occured.", err_code);
                            UE_LOG(LogTemp, Warning, TEXT("Amount of sender_account on Asset Transfer TX %d"), asnd_account.vtc_account->amount);

                            err_code = vertices_account_new_from_b32((char*)TCHAR_TO_ANSI(*arcv), &arcv_account.vtc_account);
                            checkVTCSuccess("vertices_account_new_from_b32 error occured.", err_code);
                            UE_LOG(LogTemp, Warning, TEXT("Amount of receiver_account on Asset Transfer TX %d"), arcv_account.vtc_account->amount);

                            err_code =
                                vertices_transaction_asset_xfer(sender_account.vtc_account,
                                    (char *)asnd_account.vtc_account->public_key /* or ACCOUNT_SENDER */,
                                    (char *)arcv_account.vtc_account->public_key /* or ACCOUNT_RECEIVER */,
                                    NULL,
                                    NULL,
                                    (uint64_t)Request.asset_id.GetValue(),
                                    (double)Request.amount.GetValue(),
                                    notes);
                            
                            checkVTCSuccess("vertices_transaction_asset_transfer error occured", err_code);

                            unsigned char* txID = nullptr;
                            txID = new unsigned char[TRANSACTION_HASH_STR_MAX_LENGTH];

                            size_t queue_size = 1;
                            while (queue_size && err_code == VTC_SUCCESS) {
                                err_code = vertices_event_process(&queue_size, txID);
                            }

                            checkVTCSuccess("vertices_event_process error occured", err_code);
                            UE_LOG(LogTemp, Warning, TEXT("err_code Asset Transfer TX ID Success, %s"), txID);

                            err_code = vertices_account_free(sender_account.vtc_account);
                            checkVTCSuccess("vertices_account_free error occured", err_code);
                            UE_LOG(LogTemp, Warning, TEXT("VerticesAssetTransferTransactionGetRequest Success"));

                            response = response_builders::buildAssetTransferTransactionResponse(FString(UTF8_TO_TCHAR(txID)));
                            response.SetSuccessful(true);

                            //free(txID);
                        }
                        catch(SDKException& e)
                        {
                            UE_LOG(LogTemp, Error, TEXT("👉 asset transfer tx error: %s"), UTF8_TO_TCHAR(e.what()));
                            
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(e.what()));
                        }
                        catch(std::exception& ex)
                        {
                            UE_LOG(LogTemp, Error, TEXT("👉 asset transfer tx error: %s"), UTF8_TO_TCHAR(ex.what()));
                            
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(UTF8_TO_TCHAR("Opt-In Transaction should be done at first for Asset Transfer TX.")));
                        }

                        // after build response, execute delegate to run callback with response
                        AsyncTask(ENamedThreads::GameThread, [delegate, response]()
                        {
                            delegate.ExecuteIfBound(response);
                        });

                        vertices_usable = true;
                        break;
                    }
                }
            });
        }
        
        void VerticesSDK::VerticesApplicationCallTransactionGet(const VerticesApplicationCallTransactionGetRequest& Request, const FVerticesApplicationCallTransactionGetDelegate& delegate)
        {
            AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Request, delegate]()
            {
                ret_code_t err_code = VTC_SUCCESS;

                while (1) {
                    FScopeLock lock(&m_Mutex);

                    if (vertices_usable) {
                        VerticesSDK::VerticesApplicationCallTransactionGetResponse response;
                        vertices_usable = false;

                        try
                        {
                            if(!loaded_)
                            {
                                UE_LOG(LogTemp, Warning, TEXT("Failed loading of dll libraries."));
                                err_code = VTC_ERROR_ASSERT_FAILS;
                                checkVTCSuccess("Failed loading of dll libraries.", err_code);
                            }
                            // validation request   
                            if ( Request.app_ID.GetValue() == 0 )
                            {
                                err_code = VTC_ERROR_INVALID_ADDR;
                                checkVTCSuccess("Please input correct app ID.", err_code);
                            }
                            
                            InitVertices(err_code);
                            checkVTCSuccess("When initing vertices network, an error occured", err_code);

                            convert_Account_Vertices();
                            checkVTCSuccess("Vertices convert_Account_Vertices error", err_code);

                            if (sender_account.vtc_account->amount < 1000) {
                                FFormatNamedArguments Arguments;
                                Arguments.Add(TEXT("Address"), FText::FromString(sender_account.vtc_account->public_b32));
                                FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Warning", "Go to https://bank.testnet.algorand.network/, dispense Algos to: {Address}"), Arguments));

                                UE_LOG(LogTemp, Warning, TEXT("👉 Go to https://bank.testnet.algorand.network/, dispense Algos to: %s"), *FString(sender_account.vtc_account->public_b32));
                                UE_LOG(LogTemp, Warning,
                                    TEXT("🙄 Amount available on account is too low to pass a transaction, consider adding Algos"));
                                UE_LOG(LogTemp, Warning, TEXT("😎 Then wait for a few seconds for transaction to pass..."));
                                
                                err_code = VTC_ERROR_ASSERT_FAILS;
                                checkVTCSuccess("Amount available on account is too low to pass a transaction, consider adding Algos", err_code);
                            }
                            
                            // get application information
                            UE_LOG(LogTemp, Warning, TEXT("Application %u, global states"), Request.app_ID.GetValue());

                            app_values_t app_kv = { 0 };
                            err_code = vertices_application_get(Request.app_ID.GetValue(), &app_kv);
                            checkVTCSuccess("vertices_application_get error occured", err_code);

                            /// send application call
                            // set app args as byte_slice type
                            app_values_t kv = {0};
                            kv.count = Request.app_args.Num();
                            if(kv.count > 0)
                            {
                                uint8_t arg_index = 0;
                                for(; arg_index < kv.count; arg_index++)
                                {
                                    kv.values[arg_index].type = VALUE_TYPE_BYTESLICE;
                                    if(arg_index == 0)
                                        memcpy(kv.values[arg_index].value_slice, Request.app_args[arg_index].GetData(), APPS_NOOP_TX_SLICE_MAX_SIZE);
                                    else
                                        memcpy(kv.values[arg_index].value_slice, Request.app_args[arg_index].GetData(), APPS_KV_SLICE_MAX_SIZE);
                                }
                            }
                            
                            err_code = vertices_transaction_app_call(sender_account.vtc_account, 301624623, &kv);
                            // err_code = vertices_transaction_app_call(sender_account.vtc_account, Request.app_ID.GetValue(), &kv);
                            checkVTCSuccess("vertices_transaction_app_call error occured", err_code);

                            unsigned char* txID = nullptr;
                            txID = new unsigned char[TRANSACTION_HASH_STR_MAX_LENGTH];

                            size_t queue_size = 1;
                            while (queue_size && err_code == VTC_SUCCESS) {
                                err_code = vertices_event_process(&queue_size, txID);
                            }

                            checkVTCSuccess("vertices_event_process error occured", err_code);

                            //free(txID);

                            InitVertices(err_code);
                            checkVTCSuccess("When reiniting vertices network, an error occured", err_code);
                            
                            unsigned char logs[MSG_LOGS_MAX_SIZE];
                            err_code = vertices_noop_logs_get((unsigned char *) txID, logs);

                            checkVTCSuccess("When getting logs after NoOp Tx, an error occured", err_code);
                            UE_LOG(LogTemp, Warning, TEXT("👉 Haha This is NoOp Tx Logs Status: %llu"), (long long unsigned) err_code);
                            UE_LOG(LogTemp, Warning, TEXT("👉 Haha This is logs: %s"), (char *) logs);
                            /*unsigned char * u8_logs = new unsigned char[strlen((const char*)logs)];
                            memset(u8_logs, 0, strlen((const char*)logs));
                            len = strlen((const char*)logs);
                            b64_decode((const char *)logs, strlen((const char*)logs), (char *)u8_logs, &len);

                            uint64_t app_result = 0;
                            for(int i = 0; i < 8; i++) {
                                app_result += u8_logs[i + 4] * pow(256, 8 - i - 1);
                            }
                            UE_LOG(LogTemp, Warning, TEXT("👉 Haha This is logs: %llu"), (unsigned long long int)app_result);*/

                            err_code = vertices_account_free(sender_account.vtc_account);
                            checkVTCSuccess("vertices_account_free error occured", err_code);
                            response = response_builders::buildApplicationCallTransactionResponse(FString(UTF8_TO_TCHAR(txID)), FString(UTF8_TO_TCHAR(logs)));
                            response.SetSuccessful(true);
                        }
                        catch(SDKException& e)
                        {
                            UE_LOG(LogTemp, Error, TEXT("👉 application tx error: %s"), UTF8_TO_TCHAR(e.what()));
                            
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(e.what()));
                        }
                        catch(std::exception& ex)
                        {
                            UE_LOG(LogTemp, Error, TEXT("👉 application tx error: %s"), UTF8_TO_TCHAR(ex.what()));
                            
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(ex.what()));
                        }

                        // after build response, execute delegate to run callback with response
                        AsyncTask(ENamedThreads::GameThread, [delegate, response]()
                        {
                            delegate.ExecuteIfBound(response);
                        });

                        vertices_usable = true;
                        break;
                    }
                }
            });
        }

        void VerticesSDK::checkVTCSuccess(ret_code_t& err_code)
        {
            if(err_code != VTC_SUCCESS)
                throw SDKException(err_code);
        }

        void VerticesSDK::checkVTCSuccess(char* msg_, ret_code_t& err_code)
        {
            if(err_code != VTC_SUCCESS)
                throw SDKException(msg_, err_code);
        }

        void VerticesSDK::setHTTPCURLs() 
        {
            try
            {
                if(!loaded_) return;

                // Get the base directory of this plugin
                FString BaseDir = IPluginManager::Get().FindPlugin("Algorand")->GetBaseDir();

                // Add on the relative location of the third party dll and load it
                FString VerticesPath, SodiumPath;

                VerticesPath = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/AlgorandLibrary/x64/Release/vertices.dll"));
                SodiumPath = FPaths::Combine(*BaseDir, TEXT("Source/Vertices/Libs/libsodium.dll"));
                
                VerticesHandle = LoadLibrary(*VerticesPath);
                SodiumHandle = LoadLibrary(*SodiumPath);
                
                set_http_init(&http_init);
                set_http_get(&http_get);
                set_http_post(&http_post);
                set_http_close(&http_close);
            }
            catch (std::exception &ex)
            {
                UE_LOG(LogTemp, Display, TEXT("Failed to load C-Vertices-sdk.dll & libsodium.dll. Error: %s"), *FString(ex.what()));
            }
        }
    }
}

#undef PUBLIC_ADDRESS_LENGTH
#undef LOCTEXT_NAMESPACE
