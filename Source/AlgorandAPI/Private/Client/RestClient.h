#ifndef CPP_RESTCLIENT_H
#define CPP_RESTCLIENT_H

#include <vector>
#include <string>
#include <memory>
#include "Json.h"

struct JsonResponse {
    int status;
    TSharedPtr<FJsonObject> json;  
    FJsonValue& operator[](const std::string& name) const {
        return *(json->TryGetField(FString(name.c_str())));
    }
    bool succeeded() const { return status == 200; }
};

TSharedPtr<FJsonObject>
json_parse(std::string body);

class RestClient {
public:
    RestClient(std::string prefix, std::string authorization) :
            prefix(prefix), authorization(authorization) { }
    /**
     * @brief Return the requested information from the API using method
     * @param route API route.
     * @param method HTTP method to make the request with
     * @param request_body raw bytes to be sent as body of request
     * @return JsonResponse with the status code and JSON value from response
     */
    JsonResponse api(const std::string& route,
                     const std::string& method,
                     const std::string& request_body = "") const;

    /**
     * @brief Return the requested information from the API using a GET
     * @param route API route.
     * @return string containing the requested information.
     */
    JsonResponse get(const std::string& route) const;

    /**
     * @brief Return the requested information from the API using a POST
     * @param route API route.
     * @param body Raw bytes to send as body. "" means no body.
     * @return string containing the requested information.
     */
    JsonResponse post(const std::string& route, const std::string& body = "") const;

protected:
    std::string prefix;
    std::string authorization;
};

#endif //CPP_RESTCLIENT_H