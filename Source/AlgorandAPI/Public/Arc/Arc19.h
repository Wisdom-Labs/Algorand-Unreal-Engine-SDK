//
// Created by Bruno on 6/19/2023.
//

#ifndef CPP_ARC19_H
#define CPP_ARC19_H


#include "ArcBase.h"
#include <map>

typedef struct {
    FString standard;
    FString  description;
    FString image;
    TMap<FString, FString> properties;
} arc19_metadata;

class Arc19 : public ArcBase{
public:
    Arc19() : ArcBase() {}
    Arc19(const arc_asset& asset_, FString algoRpc, uint64_t algoPort, FString algoToken);

    bool IsVerify();
    bool ParseASAUrl();
    void from_temp_ipfs();

    arc19_metadata metadata;
    FString ipfs_url;
};


#endif //CPP_ARC19_H
