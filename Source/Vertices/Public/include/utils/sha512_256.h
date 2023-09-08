/*
 * Copyright (c) 2021 Vertices Network <cyril@vertices.network>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef VERTICES_SDK_LIB_SHA512256_H
#define VERTICES_SDK_LIB_SHA512256_H

#include "../vertices/vertices_errors.h"

#if defined _WIN32 || defined _WIN64
#define SHA512_256_IMPORT __declspec(dllimport)
#elif defined __linux__
#define SHA512_256_IMPORT __attribute__((visibility("default")))
#else
#define SHA512_256_IMPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

    VERTICES_API ret_code_t
        sha512_256(unsigned char const* input,
            unsigned long ilen,
            unsigned char* output,
            unsigned long olen);

#ifdef __cplusplus
}
#endif

#endif //VERTICES_SDK_LIB_SHA512256_H
