//
// Created by Bruno on 6/17/2023.
//

#ifndef CPP_ACCOUNTINFO_H
#define CPP_ACCOUNTINFO_H

#include <string>
#include <vector>
#include <cstdint>

typedef struct {
    uint64_t id;
    FString name;
} created_asset;

typedef TArray<created_asset> AccountInfo;

class AccountAsset {
public:
    AccountAsset() {}
    AccountAsset(FString algoRpc, uint64_t algoPort, FString algoToken);
    ~AccountAsset() {}

    void getInformation(FString address);

    AccountInfo info;

    FString myAlgoRpc;
    uint64_t myAlgoPort;
    FString myAlgoTokenHeader;
};


#endif //CPP_ACCOUNTINFO_H
