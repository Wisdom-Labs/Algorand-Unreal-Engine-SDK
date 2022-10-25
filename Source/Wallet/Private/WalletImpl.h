#pragma once

#include "Wallet.h"

#include "include/vertices/vertices.h"
#include "include/vertices_log.h"
#include "include/utils/base32.h"
#include "include/utils/base64.h"
#include "../../Vertices/Libs/include/sodium.h"
#include "unix_config.h"

#include <cstring>


/// We store anything related to the account into the below structure
/// The private key is used outside of the Vertices library:
///    you don't have to pass the private key to the SDK as signing is done outside
typedef struct {
    unsigned char private_key[ADDRESS_LENGTH];  //!< 32-bytes private key
    account_info_t* vtc_account;               //!< pointer to Vertices account data
} account_t;

typedef enum
{
    ALGORAND_PAYMENT_TRANSACTION = 0,
    ALGORAND_KEY_REGISTRATION_TRANSACTION,
    ALGORAND_ASSET_CONFIGURATION_TRANSACTION,
    ALGORAND_ASSET_TRANSFER_TRANSACTION,
    ALGORAND_ASSET_FREEZE_TRANSACTION,
    ALGORAND_APPLICATION_CALL_TRANSACTION,
} tx_type_t;


// Alice's account is used to send data, keys will be retrived from config/key_files.txt
account_t alice_account;
// Bob is receiving the money 😎
account_t bob_account;

provider_info_t providers;
vertex_t m_vertex;

class WalletImpl : public Wallet
{
public:
    WalletImpl();

    ret_code_t create_new_account();
    ret_code_t load_existing_account();
    int testAlgorand();

private:
    FString config_path, prv_key, pub_key;
};