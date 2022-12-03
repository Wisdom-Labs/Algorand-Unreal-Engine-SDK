// Copyright 2022, Wisdom Labs. All Rights Reserved

#pragma once
#include "include/vertices_log.h"

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

// set function pointer of http_init
HTTP_WEAK_IMPORT ret_code_t set_http_init(
    ret_code_t(*http_init_handler)(const provider_info_t* provider,
        size_t(*response_payload_cb)(char* chunk, size_t chunk_size)));

// set function pointer of http_get
HTTP_WEAK_IMPORT ret_code_t set_http_get(
    ret_code_t(*http_get_handler)(const provider_info_t* provider,
        const char* relative_path,
        const char* headers,
        uint32_t* response_code));

// set function pointer of http_post
HTTP_WEAK_IMPORT ret_code_t set_http_post(
    ret_code_t(*http_post_handler)(const provider_info_t* provider,
        const char* relative_path,
        char* headers,
        const char* body,
        size_t body_size,
        uint32_t* response_code));

// set function pointer of http_close
HTTP_WEAK_IMPORT ret_code_t set_http_close(
    void(*http_close_handler)(void));


#ifdef __cplusplus
}
#endif