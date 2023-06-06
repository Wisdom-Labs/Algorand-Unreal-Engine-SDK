//
// Created by Bruno on 6/3/2023.
//
#include "IndexerClient.h"
#include <curl/curl.h>
#include <map>

typedef std::map<std::string, std::string> string_map;

static std::string
url_escape(const std::string& s) {
    // passing nullptr is something that is done in the curl source for
    // this function, so it seems safe.
    auto encoded = curl_easy_escape(nullptr, s.c_str(), s.length());
    auto as_string(encoded);
    curl_free(encoded);
    return as_string;
}

static std::string
url_parameters(const string_map& map) {
    std::string params;
    for (auto const& kv : map) {
        params += (params.empty() ? "?" : "&");
//        params += url_escape(kv.first) + "=" + url_escape(kv.second);
        params += kv.first + "=" + kv.second;
    }
    return params;
}

IndexerClient::IndexerClient() :
        IndexerClient("BT4WXMIULS5OZWRRPDXQJMB3GAGC4A5MITAFQARDPEERQBTNYEMXQXO64A", "bLcs4F2SyGY0InF9M6Vl9piFTIZ8Ww281OjKXyE1") {
}

IndexerClient::IndexerClient(std::string address, std::string token) :
        RestClient(address, token) {
}

JsonResponse IndexerClient::searchForAssets(uint64_t limit,
                                            std::string next_page,
                                            algorand::vertices::Address creator,
                                            std::string name,
                                            std::string unit,
                                            uint64_t assetId)
{
    string_map params;
    if (limit != 0)
        params["limit"] = std::to_string(limit);
    if (next_page.size())
        params["next"] = next_page;

    if(!creator.is_zero())
        params["creator"] = creator.as_string;
    if (name.size() > 0)
        params["name"] = name;
    if (unit.size() > 0)
        params["unit"] = unit;

    // asset related
    if (assetId > 0)
        params["asset-id"] = std::to_string(assetId);

    return get("/v2/assets"+url_parameters(params));
}

JsonResponse IndexerClient::searchForTransactions(uint64_t limit,
                                   std::string next_page,
                                   std::string note_prefix,
                                   std::string tx_type,
                                   std::string txid,
                                   uint64_t round,
                                   uint64_t asset_id,
                                   std::string before_time,
                                   std::string after_time,
                                   algorand::vertices::Address address,
                                   uint64_t application_id)
{
    string_map params;
    if (limit != 0)
        params["limit"] = std::to_string(limit);
    if (next_page.size())
        params["next"] = next_page;
    if (tx_type.size())
        params["tx-type"] = tx_type;
    if (txid.size())
        params["txid"] = txid;

    if (round != 0)
        params["round"] = std::to_string(round);

    if (asset_id != 0)
        params["asset-id"] = std::to_string(asset_id);

    if (before_time.size())
        params["before-time"] = before_time;
    if (after_time.size())
        params["after-time"] = after_time;

    if(!address.is_zero())
        params["address"] = address.as_string;
    if (application_id != 0)
        params["application-id"] = std::to_string(application_id);

    return get("/v2/transactions"+url_parameters(params));
}

JsonResponse IndexerClient::account(algorand::vertices::Address addr, uint64_t round) {
    std::string url("/v2/accounts/"+addr.as_string);
    if (round > 0)
        url += "?round=" + std::to_string(round);
    return get(url);
}

JsonResponse IndexerClient::block(uint64_t round) {
    return get("/v2/blocks/"+std::to_string(round));
}

bool
IndexerClient::healthy(void) {
    auto resp(get("/health"));
    return resp.status == 200;
}
