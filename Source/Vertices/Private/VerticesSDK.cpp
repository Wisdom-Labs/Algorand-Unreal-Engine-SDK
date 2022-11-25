
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

#include "Account.h"
#include <cstring>

#include "SDKException.h"

using namespace std;

DECLARE_LOG_CATEGORY_EXTERN(LogMyAwesomeGame, Log, All);

#define PUBLIC_ADDRESS_LENGTH 58
#define LOCTEXT_NAMESPACE "FVerticesModule"

unsigned char user_password[] = "algorand-sdk";

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

                FILE* fstx;
                errno_t err_no;
                err_no = fopen_s(&fstx, "./config/../signed_tx.bin", "wb");
                
                if (err_no != 0) {
                    return VTC_ERROR_NOT_FOUND;
                }

                fwrite(tx->payload, tx->payload_header_length + tx->payload_body_length, 1, fstx);
                fclose(fstx);

                FILE* ftx;
                err_no = fopen_s(&ftx, "./config/../tx.bin", "wb");

                if (err_no != 0) {
                    return VTC_ERROR_NOT_FOUND;
                }

                // goal-generated transaction files are packed into a map of one element: `txn`.
                // the one-element map takes 4 bytes into our message packed payload <=> `txn`
                // we also add the `map` type before
                // which results in 5-bytes to be added before the payload at `payload_offset`
                char* payload = new char[tx->payload_body_length + 5];
                payload[0] = (char)(unsigned char)0x81; // starting flag for map of one element
                memcpy(&payload[1],
                    &tx->payload[tx->payload_header_length - 4],
                    tx->payload_body_length + 4);

                fwrite(payload, sizeof payload, 1, ftx);
                fclose(ftx);
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
        FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Error", "👉 vertices_evt_handler {MSG}"), Arguments));
        err_code = VTC_ERROR_INVALID_PARAM;
    }
    
    return err_code;
}

#ifdef __cplusplus
} /* extern "C" */
#endif

provider_info_t providers;

vertex_t m_vertex;

namespace algorand {
    namespace vertices {

        VerticesSDK::VerticesSDK() {
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

        void VerticesSDK::setAlgoRpc(const FString& algoRpc) {
            myAlgoRpc = algoRpc;
        }

        void VerticesSDK::setAlgoPort(const int& algoPort) {
            myAlgoPort = algoPort;
        }

        void VerticesSDK::setAlgoTokenHeader(const FString& algoTokenHeader) {
            myAlgoTokenHeader = algoTokenHeader;
        }

        void VerticesSDK::loadVerticesLibrary() {
            loaded_ = false;
            // Get the base directory of this plugin
            FString BaseDir = IPluginManager::Get().FindPlugin("Algorand")->GetBaseDir();

            // Add on the relative location of the third party dll and load it
            FString VerticesPath, SodiumPath;

            VerticesPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/AlgorandLibrary/Win64/vertices.dll"));
            SodiumPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/AlgorandLibrary/Win64/libsodium.dll"));

            VerticesHandle = !VerticesPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*VerticesPath) : nullptr;
            SodiumHandle = !SodiumPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*SodiumPath) : nullptr;

            if (VerticesHandle && SodiumHandle)
            {
                loaded_ = true;
                return;
            }
            else
                loaded_ = false;
        }

        void VerticesSDK::InitVertices(ret_code_t& err_code) {
            /*setAlgoRpc("http://localhost");*/
            /*setAlgoPort(8080);*/
            //setAlgoTokenHeader("X-Algo-API-Token:77aa632e1a023e6b1c79bbb275645cb0ca7ac82cb9d4e92226d9c0029fe35c1c");
            char* url = new char[myAlgoRpc.Len()];
            char* tokenHeader = new char[myAlgoTokenHeader.Len()];
            memcpy(url, TCHAR_TO_ANSI(*myAlgoRpc), myAlgoRpc.Len());
            memcpy(tokenHeader, TCHAR_TO_ANSI(*myAlgoTokenHeader), myAlgoTokenHeader.Len());
            url[myAlgoRpc.Len()] = 0;
            tokenHeader[myAlgoTokenHeader.Len()] = 0;

            createNewVertices(url, myAlgoPort, tokenHeader, err_code);
            vertices_ping_check(err_code);
            vertices_version_check(err_code);
        }

        void VerticesSDK::createNewVertices(char* sever_url, short port, char* server_token_header, ret_code_t& err_code) {
            providers.url = sever_url;
            providers.port = port;
            providers.header = server_token_header;

            int ret = sodium_init();
            UE_LOG(LogTemp, Warning, TEXT("err_code sodium_init %d"), ret);

            m_vertex.provider = &providers;
            m_vertex.vertices_evt_handler = &vertices_evt_handler;

            // create new vertex
            err_code = vertices_new(&m_vertex);
            UE_LOG(LogTemp, Warning, TEXT("err_code vertices_new %d"), err_code);
            checkVTCSuccess(err_code);
        }

        void VerticesSDK::vertices_ping_check(ret_code_t& err_code) 
        {
            // making sure the provider is accessible
            err_code = vertices_ping();
            UE_LOG(LogTemp, Warning, TEXT("err_code vertices_ping %d"), err_code);
            checkVTCSuccess(err_code);
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

            checkVTCSuccess(err_code);
            UE_LOG(LogTemp, Warning, TEXT("🏎 Running on %s v.%u.%u.%u"),
                version.network,
                version.major,
                version.minor,
                version.patch);
        }

        ret_code_t VerticesSDK::create_new_account() {
            ret_code_t err_code = VTC_SUCCESS;

            unsigned char seed[crypto_sign_ed25519_SEEDBYTES] = { 0 };
            unsigned char ed25519_pk[crypto_sign_ed25519_PUBLICKEYBYTES];

            UE_LOG(LogTemp, Display, TEXT("🧾 Creating new random account and storing it (path  config)"));

            unsigned char ed25519_sk[crypto_sign_ed25519_SECRETKEYBYTES];
            randombytes_buf(seed, sizeof(seed));

            crypto_sign_ed25519_seed_keypair(ed25519_pk, ed25519_sk, seed);

            memcpy(sender_account.private_key, ed25519_sk, sizeof(sender_account.private_key));

            config_path = FPaths::ProjectPluginsDir() + "Algorand/Source/Algorand/config/";

            char* config_file = TCHAR_TO_ANSI(*(config_path + "private_key.bin"));

            // encrypting private key
            AES aes(AESKeyLength::AES_128);
            unsigned char* pk_out = aes.EncryptECB(sender_account.private_key, ADDRESS_LENGTH, user_password);

            FILE* fw_priv;
            errno_t err_no;
            
            err_no = fopen_s(&fw_priv, config_file, "wb");
            
            if (err_no != 0) {
                UE_LOG(LogTemp, Error, TEXT("Cannot create ./config/private_key.bin"));
                return VTC_ERROR_NOT_FOUND;
            }
            else {
                fwrite(pk_out, 1, 48, fw_priv);
                fclose(fw_priv);
            }

            delete[] pk_out;
            // encrypted
            // adding account, account address will be computed from binary public key
            err_code = vertices_account_new_from_bin((char*)ed25519_pk, &sender_account.vtc_account);
            UE_LOG(LogTemp, Warning, TEXT("err_code vertices_account_new_from_bin %d"), err_code);

            // we can now store the b32 address in a file
            config_file = TCHAR_TO_ANSI(*(config_path + "public_b32.txt"));
            FILE* fw_pub;

            err_no= fopen_s(&fw_pub, config_file, "w");
            if (err_no == 0) {
                size_t len = strlen(sender_account.vtc_account->public_b32);

                fwrite(sender_account.vtc_account->public_b32, 1, len, fw_pub);
                fwrite("\n", 1, 1, fw_pub);
                fclose(fw_pub);
            }

            checkVTCSuccess(err_code);
            return err_code;
        }

        ret_code_t VerticesSDK::load_existing_account() {
            ret_code_t err_code;

            char public_b32[PUBLIC_B32_STR_MAX_LENGTH] = { 0 };

            size_t bytes_read = 0;

            config_path = FPaths::ProjectPluginsDir() + "Algorand/Source/Algorand/config/";

            char* config_file = TCHAR_TO_ANSI(*(config_path + "private_key.bin"));

            memset(sender_account.private_key, 0, 32);
            sender_account.vtc_account = nullptr;

            // decrypting private key
            unsigned char decrypted_pk[48];
            AES aes(AESKeyLength::AES_128);

            FILE * f_priv;
            errno_t err_no;
            
            err_no = fopen_s(&f_priv, config_file, "rb");
            
            if (err_no == 0) {
                UE_LOG(LogTemp, Display, TEXT("🔑 Loading private key from %s"), *FString(config_file));

                bytes_read = fread(decrypted_pk, 1, 48, f_priv);
                fclose(f_priv);
            }

            if (err_no != 0 || bytes_read != 48) {
                UE_LOG(LogTemp, Display, TEXT(
                    "🤔 private_key.bin does not exist or keys not found."));

                return VTC_ERROR_NOT_FOUND;
            }

            unsigned char *plain_pk = aes.DecryptECB(decrypted_pk, 48, user_password);
            memcpy(sender_account.private_key, plain_pk, ADDRESS_LENGTH);
            delete[] plain_pk;
            // decrypted private key
            config_file = TCHAR_TO_ANSI(*(config_path + "public_b32.txt"));
            FILE * f_pub;

            err_no = fopen_s(&f_pub, config_file, "r");
            
            if (err_no == 0) {
                UE_LOG(LogTemp, Display, TEXT("🔑 Loading public key from: %s"), *FString(config_file));

                bytes_read = fread(public_b32, 1, PUBLIC_B32_STR_MAX_LENGTH, f_pub);
                fclose(f_pub);

                size_t len = strlen(public_b32);
                while (public_b32[len - 1] == '\n' || public_b32[len - 1] == '\r') {
                    public_b32[len - 1] = '\0';
                    len--;
                }
            }

            if (f_pub == nullptr || bytes_read < ADDRESS_LENGTH) {
                UE_LOG(LogTemp, Warning, TEXT(
                    "🤔 public_b32.txt does not exist"));

                return VTC_ERROR_NOT_FOUND;
            }

            err_code = vertices_account_new_from_b32(public_b32, &sender_account.vtc_account);
            UE_LOG(LogTemp, Warning, TEXT("err_code vertices_account_new_from_b32 %d"), err_code);
            checkVTCSuccess(err_code);
            UE_LOG(LogTemp, Display, TEXT("💳 Created Alice's account: %s"), *FString(sender_account.vtc_account->public_b32));

            return VTC_SUCCESS;
        }

        FString VerticesSDK::load_pub_key() {
            char public_b32[PUBLIC_B32_STR_MAX_LENGTH] = { 0 };

            size_t bytes_read = 0;

            config_path = FPaths::ProjectPluginsDir() + "Algorand/Source/Algorand/config/";

            char* config_file = TCHAR_TO_ANSI(*(config_path + "public_b32.txt"));
            FILE* f_pub;
            errno_t err_no;

            err_no = fopen_s(&f_pub, config_file, "r");
            if (err_no == 0) {
                UE_LOG(LogTemp, Display, TEXT("🔑 Loading public key from: %s"), *FString(config_file));

                bytes_read = fread(public_b32, 1, PUBLIC_B32_STR_MAX_LENGTH, f_pub);
                fclose(f_pub);

                size_t len = strlen(public_b32);
                while (public_b32[len - 1] == '\n' || public_b32[len - 1] == '\r') {
                    public_b32[len - 1] = '\0';
                    len--;
                }
            }

            if (err_no != 0) {
                UE_LOG(LogTemp, Warning, TEXT("🤔 public_b32.txt does not exist"));
                throw invalid_argument("🤔 public_b32.txt does not exist");
            }

            if(bytes_read < ADDRESS_LENGTH) {
                UE_LOG(LogTemp, Warning, TEXT("🤔 public_b32.txt format error occured"));
                throw "🤔 public_b32.txt format error occured";
            }

            return FString(strlen(public_b32), public_b32);
        }

        void VerticesSDK::VerticesGenerateWalletGet(const VerticesGenerateWalletGetRequest& Request, const FVerticesGenerateWalletGetDelegate& delegate)
        {           
            auto mnemonic = R"(base giraffe believe make tone transfer wrap attend
                     typical dirt grocery distance outside horn also abstract
                     slim ecology island alter daring equal boil absent
                     carpet)";
            Account account = Account::from_mnemonic(mnemonic);

            AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Request, delegate]()
            {
                ret_code_t err_code = VTC_SUCCESS;
                while (1) {
                    FScopeLock lock(&m_Mutex);

                    if (vertices_usable) {
                        VerticesSDK::VerticesGenerateWalletGetResponse response;
                        vertices_usable = false;
                        
                        try
                        {
                            InitVertices(err_code);
                            err_code = create_new_account();
                            UE_LOG(LogTemp, Display, TEXT("Vertices created new account."));
                            
                            response = response_builders::buildGenerateWalletResponse(FString(sender_account.vtc_account->public_b32));
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
                            InitVertices(err_code);

                            memset(test_account.private_key, 0, 32);
                            test_account.vtc_account = nullptr;

                            const FString& address = Request.Address.GetValue();
                            err_code = vertices_account_new_from_b32((char*)TCHAR_TO_ANSI(*address), &test_account.vtc_account);
                            checkVTCSuccess("vertices_account_new_from_b32 error occured.", err_code);
                            UE_LOG(LogTemp, Warning, TEXT("err_code AlgorandGetaddressbalanceGet %d"), test_account.vtc_account->amount);

                            response = response_builders::buildGetBalanceResponse(test_account.vtc_account->amount);
                            response.SetSuccessful(true);

                            err_code = vertices_account_free(test_account.vtc_account);
                            checkVTCSuccess("vertices_account_free error occured.", err_code);
                        }
                        catch (SDKException& e)
                        {
                            UE_LOG(LogTemp, Error, TEXT("👉 payment tx error: %s"), e.what());
                            
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(e.what()));   
                        }
                        catch (std::exception& ex)
                        {
                            UE_LOG(LogTemp, Error, TEXT("👉 payment tx error: %s"), ex.what());
                            
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

        void VerticesSDK::VerticesPaymentTransactionGet(const VerticesPaymentTransactionGetRequest& Request, const FVerticesPaymentTransactionGetDelegate& delegate)
        {
            AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Request, delegate]()
            {
                ret_code_t err_code = VTC_SUCCESS;
                while (1) {
                    FScopeLock lock(&m_Mutex);

                    if (vertices_usable) {
                        VerticesSDK::VerticesPaymentTransactionGetResponse response;
                        vertices_usable = false;

                        try
                        {
                            // validation Request
                            char* notes = TCHAR_TO_ANSI(*(Request.notes.GetValue()));

                            if(strlen(notes) == 0)
                                notes = "Payment Transaction";
                            
                            if ( Request.receiverAddress.GetValue().Len() != PUBLIC_ADDRESS_LENGTH )
                            {
                                err_code = VTC_ERROR_INVALID_ADDR;
                                checkVTCSuccess("Please input address with correct length.", err_code);
                            }
                            InitVertices(err_code);

                            err_code = load_existing_account();
                            
                            UE_LOG(LogTemp, Display, TEXT("Loaded main account."));

                            if (sender_account.vtc_account->amount < 1000) {
                                FFormatNamedArguments Arguments;
                                Arguments.Add(TEXT("Address"), FText::FromString(sender_account.vtc_account->public_b32));
                                FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Warning", "👉 Go to https://bank.testnet.algorand.network/, dispense Algos to: {Address}"), Arguments));

                                UE_LOG(LogTemp, Warning, TEXT("👉 Go to https://bank.testnet.algorand.network/, dispense Algos to: %s"), *FString(sender_account.vtc_account->public_b32));
                                UE_LOG(LogTemp, Warning,
                                    TEXT("🙄 Amount available on account is too low to pass a transaction, consider adding Algos"));
                                UE_LOG(LogTemp, Warning, TEXT("😎 Then wait for a few seconds for transaction to pass..."));
                                
                                err_code = VTC_ERROR_ASSERT_FAILS;
                                checkVTCSuccess("Amount available on account is too low to pass a transaction, consider adding Algos", err_code);
                            }
                            
                            err_code =
                                vertices_transaction_pay_new(sender_account.vtc_account,
                                    TCHAR_TO_ANSI(*(Request.receiverAddress.GetValue())) /* or ACCOUNT_RECEIVER */,
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
                            UE_LOG(LogTemp, Error, TEXT("👉 payment tx error: %s"), e.what());
                            
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(e.what()));
                        }
                        catch(std::exception& ex)
                        {
                            UE_LOG(LogTemp, Error, TEXT("👉 payment tx error: %s"), ex.what());
                            
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
                            InitVertices(err_code);

                            load_existing_account();

                            if (sender_account.vtc_account->amount < 1000) {
                                FFormatNamedArguments Arguments;
                                Arguments.Add(TEXT("Address"), FText::FromString(sender_account.vtc_account->public_b32));
                                FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Warning", "👉 Go to https://bank.testnet.algorand.network/, dispense Algos to: {Address}"), Arguments));

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
                            for (uint32_t i = 0; i < app_kv.count; ++i) {
                                if (app_kv.values[i].type == VALUE_TYPE_INTEGER) {
                                    UE_LOG(LogTemp, Warning, TEXT("%s: %llu"), app_kv.values[i].name, (long long unsigned) app_kv.values[i].value_uint);
                                }
                                else if (app_kv.values[i].type == VALUE_TYPE_BYTESLICE) {
                                    UE_LOG(LogTemp, Warning, TEXT("%s: %s"), app_kv.values[i].name, app_kv.values[i].value_slice);
                                }
                            }

                            // send application call
                            app_values_t kv = { 0 };
                            kv.count = 1;
                            kv.values[0].type = VALUE_TYPE_INTEGER;
                            kv.values[0].value_uint = 32;

                            err_code = vertices_transaction_app_call(sender_account.vtc_account, Request.app_ID.GetValue(), &kv);
                            checkVTCSuccess("vertices_transaction_app_call error occured", err_code);

                            unsigned char* txID = nullptr;
                            txID = new unsigned char[TRANSACTION_HASH_STR_MAX_LENGTH];

                            size_t queue_size = 1;
                            while (queue_size && err_code == VTC_SUCCESS) {
                                err_code = vertices_event_process(&queue_size, txID);
                            }

                            checkVTCSuccess("vertices_event_process error occured", err_code);

                            //free(txID);

                            err_code = vertices_account_free(sender_account.vtc_account);
                            checkVTCSuccess("vertices_account_free error occured", err_code);
                            response = response_builders::buildApplicationCallTransactionResponse(FString(UTF8_TO_TCHAR(txID)));
                            response.SetSuccessful(true);
                        }
                        catch(SDKException& e)
                        {
                            FFormatNamedArguments Arguments;
                            Arguments.Add(TEXT("MSG"), FText::FromString(e.what()));
                            FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Error", "👉 {MSG}"), Arguments));
                            
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(e.what()));
                        }
                        catch(std::exception& ex)
                        {
                            FFormatNamedArguments Arguments;
                            Arguments.Add(TEXT("MSG"), FText::FromString(ex.what()));
                            FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Error", "👉 {MSG}"), Arguments));
                            
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
            set_http_init(&http_init);
            set_http_get(&http_get);
            set_http_post(&http_post);
            set_http_close(&http_close);
        }
    }
}

#undef PUBLIC_ADDRESS_LENGTH
#undef LOCTEXT_NAMESPACE
