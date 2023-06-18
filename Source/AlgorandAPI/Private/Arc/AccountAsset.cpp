//
// Created by Bruno on 6/17/2023.
//

#include "Arc/AccountAsset.h"
#include "../Private/Client/AlgodClient.h"

AccountAsset::AccountAsset(FString algoRpc, uint64_t algoPort, FString algoToken)
{
    myAlgoRpc = algoRpc;
    myAlgoPort = algoPort;
    myAlgoTokenHeader = algoToken;
}

void AccountAsset::getInformation(FString address) {
    std::string url;
    if(myAlgoPort != 0)
        url = TCHAR_TO_UTF8(*(myAlgoRpc + ":" + FString::FromInt(myAlgoPort)));
    else
        url = TCHAR_TO_UTF8(*(myAlgoRpc));
    
    AlgodClient algodClient(url, std::string(TCHAR_TO_UTF8(*myAlgoTokenHeader)));

    std::string s_address = TCHAR_TO_UTF8(*address);
    auto resp = algodClient.account(algorand::vertices::Address(s_address));

    try {
        if(resp.json->HasField("created-assets")) {
            if(resp.json->GetArrayField("created-assets").Num() > 0) {
                TArray<TSharedPtr<FJsonValue>> json_assets = resp.json->GetArrayField("created-assets");

                // Search for a member in each object of the array
                for (int i = 0; i < json_assets.Num(); i++) {
                    TSharedPtr<FJsonObject> asset = json_assets[i]->AsObject();

                    created_asset c_asset;
                    if(asset->HasField("index"))
                        c_asset.id = asset->GetNumberField("index");
                    if(asset->HasField("params"))
                        if(asset->GetObjectField("params")->HasField("name"))
                            c_asset.name = asset->GetObjectField("params")->GetStringField("name");
                    
                    info.Add(c_asset);
                }
            }
        }
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    int x = 0;
}
