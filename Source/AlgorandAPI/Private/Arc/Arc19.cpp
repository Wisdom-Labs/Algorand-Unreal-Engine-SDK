//
// Created by Bruno on 6/19/2023.
//

#include "Arc/Arc19.h"
#include "../Private/Client/base.h"
#include "../Multiformats/cid.hpp"


Arc19::Arc19(const arc_asset& asset_, FString algoRpc, uint64_t algoPort, FString algoToken) {
    // set rpc info
    myAlgoRpc = algoRpc;
    myAlgoPort = algoPort;
    myAlgoTokenHeader = algoToken;

    asset = asset_;
}

bool Arc19::IsVerify() {
    return asset.params.url.StartsWith("template-ipfs") && !asset.params.reserve.IsEmpty();
}

bool Arc19::ParseASAUrl() {
    try{
        TArray<FString> chunks;
        asset.params.url.ParseIntoArray(chunks, UTF8_TO_TCHAR("://"));
        
        if(chunks[0] == "template-ipfs" && chunks[1].StartsWith("{ipfscid:")) {
            // Look for something like: template:ipfs://{ipfscid:1:raw:reserve:sha2-256} and parse into components
            chunks[0] = "ipfs";

            TArray<FString> cidComponents;
            chunks[1].ParseIntoArray(cidComponents, UTF8_TO_TCHAR(":"));
            if (cidComponents.Num() != 5) {
                return false;
            }

            const FString cidVersion = cidComponents[1];    // 1
            const FString cidCodec = cidComponents[2];      // raw
            const FString asaField = cidComponents[3];      // reserve
            const FString cidHash = cidComponents[4];       // sha2-256
            
            if (!cidHash.StartsWith("sha2-256")) {
                return false;
            }

            if (cidCodec != FString("raw") && cidCodec != FString("dag-pb")) {
                return false;
            }

            if (asaField != FString("reserve")) {
                return false;
            }

            int cidCodecCode;
            if (cidCodec == FString("raw"))
                cidCodecCode = 0x55;
            if (cidCodec == FString("dag-pb"))
                cidCodecCode = 0x70;

            const Multiformats::Varint version(FCString::Atoi(*cidVersion));
            const Multiformats::Varint type(cidCodecCode);

            bytes with_checksum = b32_decode(std::string(TCHAR_TO_ANSI(*(asset.params.reserve))));
            bytes public_key(bytes{with_checksum.begin(), with_checksum.begin() + 32});

            Multiformats::Multihash mh_reserve(public_key, "sha2-256");

            FString uri;
            TArray<FString> uri_vec;
            cidHash.ParseIntoArray(uri_vec, UTF8_TO_TCHAR("/"));
            if (uri_vec.Num() > 1) uri = uri_vec[1];

            Multiformats::Cid cid{version, type, mh_reserve};

            FString cid_string;
            if (version == 1)
                cid_string = cid.to_string(Multiformats::Multibase::Protocol::Base32).c_str();  // comparasion encoded
            else {
                cid_string = cid.to_string(Multiformats::Multibase::Protocol::Base58Btc).c_str(); // comparasion encoded
                cid_string.RemoveAt(0,1);
            }

            chunks[1] = cid_string + "/" + uri;

            ipfs_url = chunks[1];
            metadata.standard = "Arc19";
            return true;
        }
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return false;
    }
    return false;
}

void Arc19::from_temp_ipfs() {
    RestClient restClient(BASE_URL_OF_IPFS, "");

    auto resp = restClient.get(std::string(TCHAR_TO_ANSI(*ipfs_url)));

    try {
        if(resp.json.IsValid())
        {
            for (auto itr : resp.json->Values) {
                if (std::strcmp(TCHAR_TO_UTF8(*itr.Key) ,"description") == 0)
                    metadata.description = itr.Value->AsString();
                if (std::strcmp(TCHAR_TO_UTF8(*itr.Key) ,"image") == 0)
                    metadata.image = itr.Value->AsString();

                if(itr.Value->Type == EJson::String)
                    metadata.properties.Add(itr.Key, itr.Value->AsString());
                if(itr.Value->Type == EJson::Number)
                    metadata.properties.Add(itr.Key, FString::FromInt(itr.Value->AsNumber()));
            }
        }
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}
