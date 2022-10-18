#include "VerticesSDK.h"


static ret_code_t
vertices_evt_handler(vtc_evt_t* evt) {
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

/// Create new random account
/// Account keys will be stored in files
static ret_code_t
create_new_account() {
    ret_code_t err_code;

    unsigned char seed[crypto_sign_ed25519_SEEDBYTES] = { 0 };
    unsigned char ed25519_pk[crypto_sign_ed25519_PUBLICKEYBYTES];

    LOG_WARNING("🧾 Creating new random account and storing it (path  config)");

    unsigned char ed25519_sk[crypto_sign_ed25519_SECRETKEYBYTES];
    randombytes_buf(seed, sizeof(seed));

    crypto_sign_ed25519_seed_keypair(ed25519_pk, ed25519_sk, seed);

    memcpy(alice_account.private_key, ed25519_sk, sizeof(alice_account.private_key));

    FILE* fw_priv = fopen("./config/private_key.bin", "wb");
    if (fw_priv == nullptr) {
        LOG_ERROR("Cannot create ./config/private_key.bin");
        return VTC_ERROR_NOT_FOUND;
    }
    else {
        fwrite(ed25519_sk, 1, ADDRESS_LENGTH, fw_priv);
        fclose(fw_priv);
    }

    // adding account, account address will be computed from binary public key
    err_code = vertices_account_new_from_bin((char*)ed25519_pk, &alice_account.vtc_account);
    VTC_ASSERT(err_code);

    // we can now store the b32 address in a file
    FILE* fw_pub = fopen("./config/public_b32.txt", "w");
    if (fw_pub != nullptr) {
        size_t len = strlen(alice_account.vtc_account->public_b32);

        fwrite(alice_account.vtc_account->public_b32, 1, len, fw_pub);
        fwrite("\n", 1, 1, fw_pub);
        fclose(fw_pub);
    }

    return err_code;
}

/// Source the account using private/public keys from files.
/// \return \c VTC_ERROR_NOT_FOUND account not found
static ret_code_t
load_existing_account() {
    ret_code_t err_code;

    char public_b32[PUBLIC_B32_STR_MAX_LENGTH] = { 0 };

    size_t bytes_read = 0;

    // we either create a new random account or load it from private and public key files.
    // key files can also be generated using [`algokey`](https://developer.algorand.org/docs/reference/cli/algokey/generate/)
    FILE* f_priv = fopen("./config/private_key.bin", "rb");
    if (f_priv != nullptr) {
        LOG_INFO("🔑 Loading private key from: %s", "./config/private_key.bin");

        bytes_read = fread(alice_account.private_key, 1, ADDRESS_LENGTH, f_priv);
        fclose(f_priv);
    }

    if (f_priv == nullptr || bytes_read != ADDRESS_LENGTH) {
        LOG_WARNING(
            "🤔 private_key.bin does not exist or keys not found. You can pass the -n flag to create a new account");

        return VTC_ERROR_NOT_FOUND;
    }

    FILE* f_pub = fopen("./config/public_b32.txt", "r");
    if (f_pub != nullptr) {
        LOG_INFO("🔑 Loading public key from: %s", "./config/public_b32.txt");

        bytes_read = fread(public_b32, 1, PUBLIC_B32_STR_MAX_LENGTH, f_pub);
        fclose(f_pub);

        size_t len = strlen(public_b32);
        while (public_b32[len - 1] == '\n' || public_b32[len - 1] == '\r') {
            public_b32[len - 1] = '\0';
            len--;
        }
    }

    if (f_pub == nullptr || bytes_read < ADDRESS_LENGTH) {
        LOG_WARNING(
            "🤔 public_b32.txt does not exist or keys not found. You can pass the -n flag to create a new account");

        return VTC_ERROR_NOT_FOUND;
    }

    err_code = vertices_account_new_from_b32(public_b32, &alice_account.vtc_account);
    VTC_ASSERT(err_code);

    LOG_INFO("💳 Created Alice's account: %s", alice_account.vtc_account->public_b32);

    return VTC_SUCCESS;
}

int
testAlgorand() {
    ret_code_t err_code;

    bool create_new = true;                // bug fixing convert false to tru at first.
    tx_type_t run_tx = ALGORAND_PAYMENT_TRANSACTION;

    //   initialize providers ,struct variable
    providers.url = (char*)SERVER_URL;
    providers.port = SERVER_PORT;
    providers.header = (char*)SERVER_TOKEN_HEADER;
    //
    //   initialize alice_account ,struct variable
    memcpy(alice_account.private_key, 0, 32);
    alice_account.vtc_account = nullptr;
    //
    //   initialize bob_account, struct variable
    memcpy(bob_account.private_key, 0 , 32);
    bob_account.vtc_account = nullptr;
    //
    //   initialize m_vertex, struct variable
    m_vertex.provider = &providers;
    m_vertex.vertices_evt_handler = vertices_evt_handler;
    //
    /*int opt;
    while ((opt = getopt(argc, argv, "npa")) != -1) {
        switch (opt) {
        case 'n': {
            create_new = true;
        }
                break;
        case 'p': {
            run_tx = PAY_TX;
        }
                break;
        case 'a': {
            run_tx = APP_CALL_TX;
        }
                break;

        default: {
            fprintf(stderr,
                "Usage:\n%s [-p|-a] [-n] \nSend signed transaction on the blockchain.\n-p (default)\tSend [p]ayment (Alice sends tokens to Bob)\n-a\t\t\t\tSend [a]pplication call (Alice sends integer value to application)\n-n\t\t\t\tCreate [n]ew account",
                argv[0]);
            exit(EXIT_FAILURE);
        }
        }
    }*/

    LOG_INFO("😎 Vertices SDK running on Unix-based OS");

    int ret = sodium_init();
    VTC_ASSERT_BOOL(ret == 0);

    // create new vertex
    err_code = vertices_new(&m_vertex);
    VTC_ASSERT(err_code);

    // making sure the provider is accessible
    err_code = vertices_ping();
    VTC_ASSERT(err_code);

    // ask for provider version
    provider_version_t version = { 0 };
    err_code = vertices_version(&version);
    if (err_code == VTC_ERROR_OFFLINE) {
        LOG_WARNING("Version might not be accurate: old value is being used");
    }
    else {
        VTC_ASSERT(err_code);
    }

    LOG_INFO("🏎 Running on %s v.%u.%u.%u",
        version.network,
        version.major,
        version.minor,
        version.patch);

    // Several ways to create/load accounts:
    if (create_new) {
        // 1) create new one
        err_code = create_new_account();
        VTC_ASSERT(err_code);
    }
    else {
        // 2) from files
        err_code = load_existing_account();
        VTC_ASSERT(err_code);
    }

    //  3) from b32 address
    //      Note: creating a receiver account is not mandatory to send money to the account
    //      but we can use it to load the public key from the account address
    err_code = vertices_account_new_from_b32((char*)ACCOUNT_RECEIVER, &bob_account.vtc_account);
    VTC_ASSERT(err_code);

    LOG_INFO("🤑 %f Algos on Alice's account (%s)",
        alice_account.vtc_account->amount / 1.e6,
        alice_account.vtc_account->public_b32);

    if (alice_account.vtc_account->amount < 1001000) {
        LOG_ERROR(
            "🙄 Amount available on account is too low to pass a transaction, consider adding Algos");
        LOG_INFO("👉 Go to https://bank.testnet.algorand.network/, dispense Algos to: %s",
            alice_account.vtc_account->public_b32);
        LOG_INFO("😎 Then wait for a few seconds for transaction to pass...");
        return 0;
    }

    switch (run_tx) {
    case ALGORAND_PAYMENT_TRANSACTION: {
        // send assets from account 0 to account 1
        char* notes = (char*)"Alice sent 1 Algo to Bob";
        uint64_t AMOUNT_SENT = 100;
        err_code =
            vertices_transaction_pay_new(alice_account.vtc_account,
                (char*)bob_account.vtc_account->public_b32 /* or ACCOUNT_RECEIVER */,AMOUNT_SENT,
                notes);
        VTC_ASSERT(err_code);
    }
               break;

    case ALGORAND_APPLICATION_CALL_TRANSACTION: {
        // get application information
        LOG_INFO("Application %u, global states", APP_ID);

        app_values_t app_kv = { 0 };
        err_code = vertices_application_get(APP_ID, &app_kv);
        VTC_ASSERT(err_code);
        for (uint32_t i = 0; i < app_kv.count; ++i) {
            if (app_kv.values[i].type == VALUE_TYPE_INTEGER) {
                LOG_INFO("%s: %llu", app_kv.values[i].name, (long long unsigned) app_kv.values[i].value_uint);
            }
            else if (app_kv.values[i].type == VALUE_TYPE_BYTESLICE) {
                LOG_INFO("%s: %s", app_kv.values[i].name, app_kv.values[i].value_slice);
            }
        }

        // send application call
        app_values_t kv = { 0 };
        kv.count = 1;
        kv.values[0].type = VALUE_TYPE_INTEGER;
        kv.values[0].value_uint = 32;

        err_code = vertices_transaction_app_call(alice_account.vtc_account, APP_ID, &kv);
        VTC_ASSERT(err_code);
    }
                    break;

    default:
        LOG_ERROR("Unknown action to run");
    }

    // processing
    size_t queue_size = 1;
    while (queue_size && err_code == VTC_SUCCESS) {
        err_code = vertices_event_process(&queue_size);
        VTC_ASSERT(err_code);
    }

    // delete the created accounts from the Vertices wallet
    err_code = vertices_account_free(alice_account.vtc_account);
    VTC_ASSERT(err_code);

    err_code = vertices_account_free(bob_account.vtc_account);
    VTC_ASSERT(err_code);

    return 0;
}
