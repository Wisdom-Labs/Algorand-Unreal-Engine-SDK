#pragma once

#include "vertices_types.h"

#ifndef VERTICES_HTTP_H
#define VERTICES_HTTP_H

#ifdef __cplusplus
extern "C" {
#endif

    // if not defined before, set default value
#ifndef HTTP_MAXIMUM_CONTENT_LENGTH
#define HTTP_MAXIMUM_CONTENT_LENGTH 9192
#endif

    ret_code_t
        http_init(const provider_info_t* provider,
            size_t(*response_payload_cb)(char* chunk,
                size_t chunk_size));

    ret_code_t
        http_get(const char *url,
            const char* relative_path,
            const char* headers,
            uint32_t* response_code);

    ret_code_t
        http_post(const char *url,
            const char* relative_path,
            char* headers,
            const char* body,
            size_t body_size,
            uint32_t* response_code);

    /// Close/deinit the client
    void
        http_close(void);

#ifdef __cplusplus
}
#endif

#endif //VERTICES_HTTP_H
