// Copyright 2022, Wisdom Labs. All Rights Reserved

#include "SDKException.h"

SDKException::SDKException()
{
	msg = "Exception happened";
}

// VTC error cases supporting from vertices_errors.h
SDKException::SDKException(ret_code_t& err_code) {
	switch(err_code)
	{
		case VTC_ERROR_INTERNAL: msg = "ERROR: INTERNAL"; break;
		case VTC_ERROR_NO_MEM: msg = "ERROR: NO_MEM"; break;
		case VTC_ERROR_NOT_FOUND: msg = "ERROR: NOT_FOUND"; break;
		case VTC_ERROR_INVALID_PARAM: msg = "ERROR: INVALID_PARAM"; break;
		case VTC_ERROR_INVALID_STATE: msg = "ERROR: INVALID_STATE"; break;
		case VTC_ERROR_INVALID_ADDR: msg = "ERROR: INVALID_ADDR"; break;
		case VTC_ERROR_OFFLINE: msg = "ERROR: OFFLINE"; break;
		case VTC_ERROR_ASSERT_FAILS: msg = "ERROR: ASSERT_FAILS"; break;
		case VTC_ERROR_HTTP_BASE: msg = "ERROR: HTTP_BASE"; break;
		default: msg = "ERROR: OCCURED"; break;
	}
}

SDKException::SDKException(char* msg_, ret_code_t& err_code)
{
	switch(err_code)
	{
		case VTC_ERROR_INTERNAL: msg = msg_; break;
		case VTC_ERROR_NO_MEM: msg = msg_; break;
		case VTC_ERROR_NOT_FOUND: msg = msg_; break;
		case VTC_ERROR_INVALID_PARAM: msg = msg_; break;
		case VTC_ERROR_INVALID_STATE: msg = msg_; break;
		case VTC_ERROR_INVALID_ADDR: msg = msg_; break;
		case VTC_ERROR_OFFLINE: msg = msg_; break;
		case VTC_ERROR_ASSERT_FAILS: msg = msg_; break;
		case VTC_ERROR_HTTP_BASE: msg = msg_; break;
		default: break;
	}
}

const char* SDKException::what() const
{
	return msg;
}
