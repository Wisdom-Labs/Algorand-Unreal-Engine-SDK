/*
 * Copyright (c) 2021 Vertices Network <cyril@vertices.network>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Modules/ModuleManager.h"

THIRD_PARTY_INCLUDES_START
#include "AlgorandLibrary/include/vertices/vertices.h"
#include "AlgorandLibrary/include/vertices_log.h"
#include "AlgorandLibrary/include/utils/base32.h"
#include "AlgorandLibrary/include/utils/base64.h"
THIRD_PARTY_INCLUDES_END

#include "unix_config.h"
#include <cstring>
#include <sodium.h>

//typedef enum {
//    PAY_TX = 0,
//    APP_CALL_TX
//} tx_type_t;

static ret_code_t
vertices_evt_handler(vtc_evt_t* evt);

static provider_info_t providers;


/// We store anything related to the account into the below structure
/// The private key is used outside of the Vertices library:
///    you don't have to pass the private key to the SDK as signing is done outside
typedef struct {
    unsigned char private_key[ADDRESS_LENGTH];  //!< 32-bytes private key
    account_info_t* vtc_account;               //!< pointer to Vertices account data
} account_t;

// Alice's account is used to send data, keys will be retrived from config/key_files.txt
static account_t alice_account;
// Bob is receiving the money 😎
static account_t bob_account;

static vertex_t m_vertex;
