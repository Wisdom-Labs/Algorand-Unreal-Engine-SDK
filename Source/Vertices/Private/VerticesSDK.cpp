
#include "VerticesSDK.h"
#include "Misc/Paths.h"
#include "Misc/MessageDialog.h"
#include "Interfaces/IPluginManager.h"
#include "VerticesApiOperations.h"
#include "ResponseBuilers.h"

#include <string>
using namespace std;

DECLARE_LOG_CATEGORY_EXTERN(LogMyAwesomeGame, Log, All);

#define LOCTEXT_NAMESPACE "FVerticesModule"

typedef struct {
    unsigned char private_key[ADDRESS_LENGTH];  //!< 32-bytes private key
    account_info_t* vtc_account;               //!< pointer to Vertices account data
} account_t;

account_t alice_account;

ret_code_t vertices_evt_handler(vtc_evt_t* evt) {
    ret_code_t err_code = VTC_SUCCESS;

    switch (evt->type) {
    case VTC_EVT_TX_READY_TO_SIGN: {
        signed_transaction_t* tx = nullptr;
        err_code = vertices_event_tx_get(evt->bufid, &tx);
        if (err_code == VTC_SUCCESS) {
            LOG_DEBUG("About to sign tx: data length %lu", (unsigned long)(tx->payload_body_length));

            // libsodium wants to have private and public keys concatenated
            unsigned char keys[crypto_sign_ed25519_SECRETKEYBYTES] = { 0 };
            memcpy(keys, alice_account.private_key, sizeof(alice_account.private_key));
            memcpy(&keys[32],
                alice_account.vtc_account->public_key,
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
            LOG_DEBUG("Signature %s (%zu bytes)", b64_signature, b64_signature_len);

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

        FILE* fstx = fopen("./config/../signed_tx.bin", "wb");

        if (fstx == nullptr) {
            return VTC_ERROR_NOT_FOUND;
        }

        fwrite(tx->payload, tx->payload_header_length + tx->payload_body_length, 1, fstx);
        fclose(fstx);

        FILE* ftx = fopen("./config/../tx.bin", "wb");

        if (ftx == nullptr) {
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

    return err_code;
}

provider_info_t providers;

vertex_t m_vertex;

namespace algorand {
    namespace vertices {

        VerticesSDK::VerticesSDK() {
            ret_code_t err_code;
            loadVerticesLibrary();

            if (!loaded_)
            {
                FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("ThirdPartyLibraryError", "Failed to load example third party library"));
                return;
            }

            FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Third Party Plugin", "Loaded C-Vertices-sdk.dll & libsodium.dll from Third Party Plugin sample."));
            setHTTPCURLs();
            createNewVertices(SERVER_URL, SERVER_PORT, SERVER_TOKEN_HEADER, err_code);

            if (err_code == VTC_SUCCESS) {
                FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Vertices", "Created Vertices Network"));
            }

            vertices_ping_check(err_code);
        }

        VerticesSDK::~VerticesSDK () {

            FPlatformProcess::FreeDllHandle(VerticesHandle);
            FPlatformProcess::FreeDllHandle(SodiumHandle);

            VerticesHandle = nullptr;
            SodiumHandle = nullptr;
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

        void VerticesSDK::createNewVertices(char* sever_url, short port, char* server_token_header, ret_code_t& err_code) {
            providers.url = (char*)SERVER_URL;
            providers.port = SERVER_PORT;
            providers.header = (char*)SERVER_TOKEN_HEADER;

            int ret = sodium_init();
            UE_LOG(LogTemp, Warning, TEXT("err_code sodium_init %d"), err_code);

            m_vertex.provider = &providers;
            m_vertex.vertices_evt_handler = &vertices_evt_handler;

            // create new vertex
            err_code = vertices_new(&m_vertex);
            UE_LOG(LogTemp, Warning, TEXT("err_code vertices_new %d"), err_code);
        }

        void VerticesSDK::vertices_ping_check(ret_code_t& err_code) 
        {
            // making sure the provider is accessible
            err_code = vertices_ping();
            UE_LOG(LogTemp, Warning, TEXT("err_code vertices_ping %d"), err_code);
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

            UE_LOG(LogTemp, Warning, TEXT("🏎 Running on %s v.%u.%u.%u"),
                version.network,
                version.major,
                version.minor,
                version.patch);
        }

        void VerticesSDK::VerticesGetaddressbalanceGet(const VerticesGetaddressbalanceGetRequest& Request, const FVerticesGetaddressbalanceGetDelegate& delegate)
        {
            ret_code_t err_code = VTC_SUCCESS;
            FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Vertices", "Get Address Balance"));
            account_t test_account;

            memset(test_account.private_key, 0, 32);
            test_account.vtc_account = nullptr;

            const FString& address = Request.Address.GetValue();
            err_code = vertices_account_new_from_b32((char*)TCHAR_TO_ANSI(*address), &test_account.vtc_account);
            UE_LOG(LogTemp, Warning, TEXT("err_code AlgorandGetaddressbalanceGet %d"), test_account.vtc_account->amount);

            VerticesSDK::VerticesGetaddressbalanceGetResponse response;

            if (err_code == VTC_SUCCESS) {
                response = response_builders::buildGetBalanceResponse(test_account.vtc_account->amount);
                response.SetSuccessful(true);
            }
            else
            {
                response.SetSuccessful(false);
            }
            delegate.ExecuteIfBound(response);
        }

        void VerticesSDK::setHTTPCURLs() {
            set_http_init(&http_init);
            set_http_get(&http_get);
            set_http_post(&http_post);
            set_http_close(&http_close);
        }
    }
}


#undef LOCTEXT_NAMESPACE