#pragma once

#include <iostream>
#include <exception>
#include "include/vertices/vertices_errors.h"

class SDKException : public std::exception
{
    const char* msg;
    public:
		SDKException();
        SDKException(ret_code_t& err_code);
		SDKException(char* msg_, ret_code_t& err_code);
	const char * what() const override;
};