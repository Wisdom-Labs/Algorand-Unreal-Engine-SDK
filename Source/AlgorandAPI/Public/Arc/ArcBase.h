//
// Created by Bruno on 6/5/2023.
//

#ifndef CPP_ARCBASE_H
#define CPP_ARCBASE_H
#include <string>

const std::string BASE_URL_OF_IPFS = "https://ipfs.io/ipfs/";

#include "../Private/Client/RestClient.h"
#include <iostream>
#include <vector>
#include <string>

typedef struct {
    FString clawback;
    FString creator;
    uint64_t decimals;
    FString freeze;
    FString manager;
    FString name;
    FString reserve;
    uint64_t total;
    FString unit_name;
    FString url;
} arc_asset_params;

typedef struct {
    uint64_t index;
    uint64_t created_at_round;
    uint64_t destroyed_at_round;
    arc_asset_params params;
} arc_asset;

typedef struct {
    FString tx_type;
    FString sender;
    uint64_t created_asset;
    FString note;
} arc_tx;

class ArcBase {
public:
    ArcBase() {asset.index = 0;}
    ArcBase(FString algoRpc, uint64_t algoPort, FString algoToken);

    void from_asset(uint64_t asset_id);
    void getAssetByID(uint64_t asset_id);

    arc_asset asset;
    arc_tx tx;

    // algorand rpc information
    FString myAlgoRpc;
    uint64_t myAlgoPort;
    FString myAlgoTokenHeader;
};


#endif //CPP_ARCBASE_H
