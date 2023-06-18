//
// Created by Bruno on 6/3/2023.
//
#include "AlgodClient.h"
#include <curl/curl.h>
#include <map>

typedef std::map<std::string, std::string> string_map;

AlgodClient::AlgodClient() :
        AlgodClient("BT4WXMIULS5OZWRRPDXQJMB3GAGC4A5MITAFQARDPEERQBTNYEMXQXO64A", "bLcs4F2SyGY0InF9M6Vl9piFTIZ8Ww281OjKXyE1") {
}

AlgodClient::AlgodClient(std::string address, std::string token) :
        RestClient(address, token) {
}

JsonResponse AlgodClient::accountInformation(algorand::vertices::Address addr) {
    std::string url("/v2/accounts/"+addr.as_string);

    return get(url);
}

JsonResponse AlgodClient::account(algorand::vertices::Address addr, uint64_t round) {
    std::string url("/v2/accounts/"+addr.as_string);
    if (round > 0)
        url += "?round=" + std::to_string(round);
    return get(url);
}

JsonResponse AlgodClient::block(uint64_t round) {
    return get("/v2/blocks/"+std::to_string(round));
}

bool
AlgodClient::healthy(void) {
    auto resp(get("/health"));
    return resp.status == 200;
}
