/**
 * @file    base64.h
 * @brief   BASE64编解码方法
 * @author  xiangwangfeng <xiangwangfeng@gmail.com>
 * @data	2011-4-23
 * @website www.xiangwangfeng.com
 */

#pragma once
#include <string>
#include "GlobalDefine.h"

# ifdef  __cplusplus
extern "C" {
# endif

NAMESPACE_BEGIN(Util)

#ifdef _WIN32
#define BASE64_API	__declspec( dllexport )
#else
#define BASE64_API	
#endif // BASE64_API


void BASE64_API base64Encode(const std::string& input, std::string& output);

void BASE64_API base64Decode(const std::string& input, std::string& output);

int BASE64_API	base64_encode(const char *data, int size, char **str);

NAMESPACE_END(Util)

# ifdef  __cplusplus
}
# endif