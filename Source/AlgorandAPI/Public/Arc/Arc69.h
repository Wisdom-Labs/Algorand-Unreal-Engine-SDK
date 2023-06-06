//
// Created by Bruno on 6/4/2023.
//

#ifndef CPP_ARC69_H
#define CPP_ARC69_H

#include "ArcBase.h"
#include <map>

typedef struct {
    FString standard;
    TArray<FString> authors;
    FString  description;
    FString external_url;
    FString media_url;
    FString mime_type;
    TMap<FString, FString> properties;
} arc69_metadata;

class Arc69 : public ArcBase{
public:
    Arc69() : ArcBase() {}
    Arc69(uint64_t asset_id);

    bool IsVerify();
    void from_tx_note(std::string& tx_note);
    arc69_metadata metadata;
};


#endif //CPP_ARC69_H
