//
// Created by Bruno on 6/4/2023.
//

#ifndef CPP_ARC03_H
#define CPP_ARC03_H

#include "ArcBase.h"
#include <map>

typedef struct {
    FString standard;
    FString name;
    uint64_t decimals;
    FString  description;
    FString  image;
    FString  image_mimetype;
    FString  external_url;
    FString  external_url_mimetype;
    FString  animation_url;
    FString animation_url_mimetype;
    TMap<FString, FString> properties;
} arc03_metadata;

class Arc03 : public ArcBase{
public:
    Arc03() : ArcBase() {}
    Arc03(const arc_asset& asset_, FString algoRpc, uint64_t algoPort, FString algoToken);

    bool IsVerify();
    void from_asset_url(std::string& url);
    arc03_metadata metadata;
};


#endif //CPP_ARC03_H
