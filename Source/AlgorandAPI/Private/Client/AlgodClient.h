//
// Created by Bruno on 6/3/2023.
//
#include "RestClient.h"
#include <vector>
#include <string>
#include "VerticesSDK.h"

#ifndef CPP_ALGODCLIENT_H
#define CPP_ALGODCLIENT_H

class AlgodClient : RestClient {
public:
    /**
     * @brief Initialize the client. Reads INDEXER_ADDRESS, INDEXER_TOKEN
     * from environment.
     */
    AlgodClient();
    /**
     * @brief Initialize the client with passed address for indexer and API token.
     */
    AlgodClient(std::string address, std::string token);

    bool healthy(void);

    JsonResponse accountInformation(algorand::vertices::Address addr);
    JsonResponse account(algorand::vertices::Address addr, uint64_t round=0);
    JsonResponse block(uint64_t round=0);
};

#endif //CPP_ALGODCLIENT_H
