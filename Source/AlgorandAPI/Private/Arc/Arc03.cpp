//
// Created by Bruno on 6/5/2023.
//

#include "Arc/Arc03.h"

const std::string BASE_URL_OF_IPFS = "https://ipfs.io/ipfs/";

Arc03::Arc03(uint64_t asset_id, FString algoRpc, uint64_t algoPort, FString algoToken) {
    // set rpc info
    myAlgoRpc = algoRpc;
    myAlgoPort = algoPort;
    myAlgoTokenHeader = algoToken;
    
    this->from_asset(asset_id);
    this->getAssetByID(asset_id);
}

bool Arc03::IsVerify() {
    std::string suffix("#arc3");
    if (suffix.size() > asset.params.url.Len()) return false;
    return asset.params.url.EndsWith(suffix.c_str(), ESearchCase::IgnoreCase);
}

void Arc03::from_asset_url(std::string &url) {
    RestClient restClient(BASE_URL_OF_IPFS, "");

    std::string asset_url = url;
    std::string prefix("ipfs://");
    if(asset_url.compare(0, prefix.size(), prefix) == 0) {
        asset_url.erase(0, prefix.size());
    }

    auto resp = restClient.get(asset_url);

    try{

        for (auto itr : resp.json->Values) {
            if (std::strcmp(TCHAR_TO_UTF8(*itr.Key) ,"standard") == 0)
                metadata.standard = itr.Value->AsString();
            if (std::strcmp(TCHAR_TO_UTF8(*itr.Key) ,"name") == 0)
                metadata.name = itr.Value->AsString();

            if (std::strcmp(TCHAR_TO_UTF8(*itr.Key) ,"decimals") == 0)
                metadata.decimals = itr.Value->AsNumber();

            if (std::strcmp(TCHAR_TO_UTF8(*itr.Key) ,"description") == 0)
                metadata.description = itr.Value->AsString();
            if (std::strcmp(TCHAR_TO_UTF8(*itr.Key) ,"image") == 0)
                metadata.image = itr.Value->AsString();
            if (std::strcmp(TCHAR_TO_UTF8(*itr.Key) ,"external_url") == 0)
                metadata.external_url = itr.Value->AsString();
            if (std::strcmp(TCHAR_TO_UTF8(*itr.Key) ,"external_url_mimetype") == 0)
                metadata.external_url_mimetype = itr.Value->AsString();
            if (std::strcmp(TCHAR_TO_UTF8(*itr.Key) ,"animation_url") == 0)
                metadata.animation_url = itr.Value->AsString();
            if (std::strcmp(TCHAR_TO_UTF8(*itr.Key) ,"animation_url_mimetype") == 0)
                metadata.animation_url_mimetype = itr.Value->AsString();

            if (std::strcmp(TCHAR_TO_UTF8(*itr.Key) ,"properties") == 0) {
                const FJsonObject & json_properties = *(itr.Value->AsObject());

                for (auto itrr : json_properties.Values) {
                    if(itrr.Value->Type == EJson::String)
                        metadata.properties.Add(itrr.Key, itrr.Value->AsString());
                    if(itrr.Value->Type == EJson::Number)
                        metadata.properties.Add(itrr.Key, FString::FromInt(itrr.Value->AsNumber()));
                }
            }
        }

    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    int x = 0;
}

