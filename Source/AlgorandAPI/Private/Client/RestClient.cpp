//
// Created by Bruno on 6/3/2023.
//
#include "RestClient.h"
#include <curl/curl.h>

TSharedPtr<FJsonObject>
json_parse(std::string body) {
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(body.c_str());
    if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
    {
        // Successfully parsed the JSON string
    }
    else
    {
        // Failed to parse the JSON string
    }
    return JsonObject;
}

static size_t
accumulate_response(void *contents, size_t size, size_t nmemb, std::string *s) {
    size_t len = size*nmemb;
    s->append((char*)contents, len);
    return len;
}

static size_t
dispense_request(char *dest, size_t size, size_t nmemb, std::string* s) {
    size_t len = min(s->size(), size*nmemb);
    if (!len)
        return 0;

    memcpy(dest, s->c_str(), len);
    s->erase(0, len);
    return len;
}

static int
curl_request(const std::string& url,
             const std::string& method,
             const std::vector<std::string>& headers,
             const std::string& request_body,
             const std::string* response_body) {
    CURL *curl = curl_easy_init();
    if (!curl)
        return 200;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);

    struct curl_slist *header_slist = NULL;
    for (auto header : headers)
        header_slist = curl_slist_append(header_slist, header.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_slist);

    if (request_body.size()) {
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, dispense_request);
        curl_easy_setopt(curl, CURLOPT_READDATA, &request_body);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, request_body.size());
    }

    if (response_body) {
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, accumulate_response);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response_body);
    }

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
//        std::cerr << "curl_easy_perform() failed: "
//                  << curl_easy_strerror(res) << std::endl;
//        assert(false);
    }
    long http_code = 0;
    curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);

    curl_slist_free_all(header_slist);
    curl_easy_cleanup(curl);
    return http_code;
}

JsonResponse
RestClient::api(const std::string& route,
                const std::string& method,
                const std::string& request_body) const {
    std::string response_body;
    int status = curl_request(prefix + route, method,
                              {"Accept: application/json",
                               authorization},
                              request_body, &response_body);
    if (response_body.empty())
        return JsonResponse{status, nullptr};
    return JsonResponse{status, json_parse(response_body)};
}

JsonResponse RestClient::get(const std::string& route) const {
    return api(route, "GET", "");
}

JsonResponse RestClient::post(const std::string& route, const std::string& body) const {
    return api(route, "POST", body);
}