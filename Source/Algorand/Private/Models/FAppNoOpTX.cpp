// Copyright 2022, Wisdom Labs. All Rights Reserved

#include "Models/FAppNoOpTX.h"
#include "Models/FUInt64.h"
#include "include/vertices/vertices_config.h"
#include "openssl/sha.h"
#include "Utils/base.h"

#define SHA512_256_DIGEST_LENGTH 32

FAppArg::FAppArg() {}
FAppArg::FAppArg(const uint8_t value_bytes_[], const uint8& len)
{
	if(value_bytes_ != nullptr)
		Value_Bytes.Append((uint8_t *)value_bytes_, len);
}

ret_code_t
UAppNoOpTXFactory::sha512_256(unsigned char const *input,
		   unsigned long ilen,
		   unsigned char *output,
		   unsigned long olen)
{
	ret_code_t err_code = VTC_SUCCESS;
	SHA512_CTX ctx;

	if (ilen == 0 || input == NULL || output == NULL)
	{
		return VTC_ERROR_INVALID_PARAM;
	}

	SHA512_Init(&ctx);

	// Set hash start to SHA-512/256
	ctx.h[0] = 0x22312194FC2BF72C;
	ctx.h[1] = 0x9F555FA3C84C64C2;
	ctx.h[2] = 0x2393B86B6F53B151;
	ctx.h[3] = 0x963877195940EABD;
	ctx.h[4] = 0x96283EE2A88EFFE3;
	ctx.h[5] = 0xBE5E1E2553863992;
	ctx.h[6] = 0x2B0199FC2C85B8AA;
	ctx.h[7] = 0x0EB72DDC81C52CA2;

	// copy input into 128-byte long array
	// as 128 bytes are used in the input vector
	unsigned char input_128[128] = {0};
	memcpy(input_128, input, ilen);

	int ret = SHA512_Update(&ctx, input_128, ilen);

	if (ret == 1)
	{
		if (olen < 128)
		{
			unsigned char buffer_out[128] = {0};

			ret = SHA512_Final(buffer_out, &ctx);
			memcpy(output, buffer_out, olen);
		}
		else
		{
			ret = SHA512_Final(output, &ctx);
		}
	}

	if (ret == 0)
	{
		err_code = VTC_ERROR_INTERNAL;
	}

	return (err_code);
}

FAppArg UAppNoOpTXFactory::FAppArgFromString(const EAppArgType& value_type, const FString& value)
{
	if( EAppArgType::ARG_METHOD == value_type)
	{
		uint8_t Bytes_Method[32];
		ret_code_t ret = sha512_256((const unsigned char *)TCHAR_TO_UTF8(*value), (unsigned long)strlen(TCHAR_TO_UTF8(*value)), (unsigned char *)Bytes_Method, SHA512_256_DIGEST_LENGTH);
		if(ret == VTC_SUCCESS)
			return FAppArg(Bytes_Method, 4);
	}
	else if(value_type == EAppArgType::ARG_INT)
	{
		uint64 u8_Num = UUInt64Factory::UInt64FromString(value).Value;
		TArray<uint8> Bytes_Num;
		Bytes_Num.AddUninitialized(sizeof(uint64));
		FMemory::Memcpy(Bytes_Num.GetData(), &u8_Num, sizeof(uint64));
		Algo::Reverse(Bytes_Num);
		return FAppArg(Bytes_Num.GetData(), APPS_KV_SLICE_MAX_SIZE);
	}
	else if( value_type == EAppArgType::ARG_BYTE_SLICE) {
		TArray<uint8> Bytes_Str;
		FTCHARToUTF8 Converter(*value);
		Bytes_Str.Append(reinterpret_cast<const uint8*>(Converter.Get()), Converter.Length());
		return FAppArg(Bytes_Str.GetData(), APPS_KV_SLICE_MAX_SIZE);
	}
	return FAppArg(new uint8_t[8], APPS_KV_SLICE_MAX_SIZE);
}

FString UAppNoOpTXFactory::FAppResultFromLogs(const EAppLogType& log_type, const FString& logs)
{
	bytes plain_logs = b64_decode(std::string(TCHAR_TO_ANSI(*logs)));
	if(log_type == EAppLogType::LOG_INT)
	{
		uint64_t app_result = 0;

		if(bytes{plain_logs.begin(), plain_logs.begin() + 4} == bytes{21,31,124,117})		// check prefix of log
		{
			for(int i = 0; i < 8; i++)		// convertion from 8bit to 64bit integer
				app_result += plain_logs[i + 4] * pow(256, 8 - i - 1);		

			UE_LOG(LogTemp, Warning, TEXT("👉 Haha This is result of appl NoOpTX: %llu"), (unsigned long long int)app_result);
			return FString::Printf(TEXT("%llu"), app_result);	
		}
	}
	return FString("Can't read correct type of data.");
}