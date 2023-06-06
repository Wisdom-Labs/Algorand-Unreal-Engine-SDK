//
// Created by Bruno on 6/5/2023.
//

#include "Arc/ArcBase.h"
#include "../Private/Client/IndexerClient.h"

ArcBase::ArcBase(uint64_t asset_id) {
    asset.index = asset_id;
}

void ArcBase::from_asset(uint64_t asset_id) {
    IndexerClient indexerClient("https://mainnet-idx.algonode.network:443", "");

    auto resp = indexerClient.searchForAssets(0,"",algorand::vertices::Address(), "", "", asset_id);

    try {
        if(resp.json->HasField("assets")) {
            if(resp.json->GetArrayField("assets").Num() > 0) {
                TSharedPtr<FJsonObject> json_asset = resp.json->GetArrayField("assets")[0]->AsObject();

                for (auto itr : json_asset->Values) {
                    if (std::strcmp(TCHAR_TO_UTF8(*itr.Key) ,"index") == 0)
                        asset.index = itr.Value->AsNumber();
                    if (std::strcmp(TCHAR_TO_UTF8(*itr.Key) ,"created-at-round") == 0)
                        asset.created_at_round = itr.Value->AsNumber();
                    if (std::strcmp(TCHAR_TO_UTF8(*itr.Key) ,"destroyed-at-round") == 0)
                        asset.destroyed_at_round = itr.Value->AsNumber();
                }

                if(json_asset->HasField("params")) {
                    TSharedPtr<FJsonObject> json_params = json_asset->GetObjectField("params");

                    for (auto itr : json_params->Values) {
                        if (std::strcmp(TCHAR_TO_UTF8(*itr.Key) ,"clawback") == 0)
                            asset.params.clawback = itr.Value->AsString();
                        if (std::strcmp(TCHAR_TO_UTF8(*itr.Key) ,"creator") == 0)
                            asset.params.creator = itr.Value->AsString();

                        if (std::strcmp(TCHAR_TO_UTF8(*itr.Key) ,"decimals") == 0)
                            asset.params.decimals = itr.Value->AsNumber();

                        if (std::strcmp(TCHAR_TO_UTF8(*itr.Key) ,"freeze") == 0)
                            asset.params.freeze = itr.Value->AsString();
                        if (std::strcmp(TCHAR_TO_UTF8(*itr.Key) ,"manager") == 0)
                            asset.params.manager = itr.Value->AsString();
                        if (std::strcmp(TCHAR_TO_UTF8(*itr.Key) ,"name") == 0)
                            asset.params.name = itr.Value->AsString();
                        if (std::strcmp(TCHAR_TO_UTF8(*itr.Key) ,"reserve") == 0)
                            asset.params.reserve = itr.Value->AsString();

                        if (std::strcmp(TCHAR_TO_UTF8(*itr.Key) ,"total") == 0)
                            asset.params.total = itr.Value->AsNumber();

                        if (std::strcmp(TCHAR_TO_UTF8(*itr.Key) ,"unit-name") == 0)
                            asset.params.unit_name = itr.Value->AsString();
                        if (std::strcmp(TCHAR_TO_UTF8(*itr.Key) ,"url") == 0)
                            asset.params.url = itr.Value->AsString();
                    }
                }
            }
        }
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    int x = 0;

}

void ArcBase::from_tx(uint64_t asset_id) {
    IndexerClient indexerClient("https://mainnet-idx.algonode.network:443", "");

    auto resp = indexerClient.searchForTransactions(0,"","","acfg","",0,asset_id,"","",algorand::vertices::Address(),0);

    try {
        if (resp.json->HasField("transactions")) {
            if (resp.json->GetArrayField("transactions").Num() > 0) {
                TSharedPtr<FJsonObject> json_tx = resp.json->GetArrayField("transactions")[0]->AsObject();

                for (auto itr : json_tx->Values) {
                    if (std::strcmp(TCHAR_TO_UTF8(*itr.Key) ,"tx-type") == 0)
                        tx.tx_type = itr.Value->AsString();
                    if (std::strcmp(TCHAR_TO_UTF8(*itr.Key) ,"sender") == 0)
                        tx.sender = itr.Value->AsString();
                    if (std::strcmp(TCHAR_TO_UTF8(*itr.Key) ,"note") == 0)
                        tx.note = itr.Value->AsString();
                }
            }
        }
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    int x = 0;
}