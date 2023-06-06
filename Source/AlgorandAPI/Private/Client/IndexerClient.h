//
// Created by Bruno on 6/3/2023.
//
#include "RestClient.h"
#include <vector>
#include <string>

#include "VerticesSDK.h"

#ifndef CPP_INDEXERCLIENT_H
#define CPP_INDEXERCLIENT_H

class IndexerClient : RestClient {
public:
    /**
     * @brief Initialize the client. Reads INDEXER_ADDRESS, INDEXER_TOKEN
     * from environment.
     */
    IndexerClient();
    /**
     * @brief Initialize the client with passed address for indexer and API token.
     */
    IndexerClient(std::string address, std::string token);

    bool healthy(void);

    JsonResponse searchForAssets(uint64_t limit=0, std::string next_page="",
                                 algorand::vertices::Address creator=algorand::vertices::Address(),
                                 std::string name="",
                                 std::string unit="",
                                 uint64_t assetId=0);

    JsonResponse searchForTransactions(uint64_t limit=0,
                                       std::string next_page="",
                                       std::string note_prefix="",
                                       std::string tx_type="",
                                       std::string txid="",
                                       uint64_t round=0,
                                       uint64_t asset_id=0,
                                       std::string before_time="",
                                       std::string after_time="",
                                       algorand::vertices::Address address=algorand::vertices::Address(),
                                       uint64_t application_id=0);

    JsonResponse account(algorand::vertices::Address addr, uint64_t round=0);
    JsonResponse block(uint64_t round=0);
};

#endif //CPP_INDEXERCLIENT_H
