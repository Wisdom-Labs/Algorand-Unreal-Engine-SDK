#pragma once

THIRD_PARTY_INCLUDES_START
#include "AlgorandLibrary/include/vertices_log.h"
#include "AlgorandLibrary/include/compilers.h"
#include "AlgorandLibrary/include/vertices/vertices_http.h"
THIRD_PARTY_INCLUDES_END

#ifdef __cplusplus
extern "C" {
#endif

#if defined _WIN32 || defined _WIN64
#define HTTP_WEAK_IMPORT __declspec(dllimport)
#elif defined __linux__
#define HTTP_WEAK_IMPORT __attribute__((visibility("default")))
#else
#define HTTP_WEAK_IMPORT
#endif

HTTP_WEAK_IMPORT ret_code_t set_http_init(
    ret_code_t(*http_init_handler)(const provider_info_t* provider,
        size_t(*response_payload_cb)(char* chunk, size_t chunk_size)));

HTTP_WEAK_IMPORT ret_code_t set_http_get(
    ret_code_t(*http_get_handler)(const provider_info_t* provider,
        const char* relative_path,
        const char* headers,
        uint32_t* response_code));

HTTP_WEAK_IMPORT ret_code_t set_http_post(
    ret_code_t(*http_post_handler)(const provider_info_t* provider,
        const char* relative_path,
        char* headers,
        const char* body,
        size_t body_size,
        uint32_t* response_code));

HTTP_WEAK_IMPORT ret_code_t set_http_close(
    ret_code_t(*http_close_handler)());


#ifdef __cplusplus
}
#endif