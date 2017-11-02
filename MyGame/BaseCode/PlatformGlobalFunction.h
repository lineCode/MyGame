#pragma once

#ifdef __cplusplus
#define CQMAIN_EXTERN extern "C"
#else
#define CQMAIN_EXTERN extern
#endif

#include <stdio.h>
#include <vector>
#include <string>

CQMAIN_EXTERN const char* CQGetApplicationDir();
CQMAIN_EXTERN const char* CQGetLibraryCacheDir();




// 功能函数封装
CQMAIN_EXTERN bool	CQDeleteFile(const char* pszDirectory);							// 删除文件
CQMAIN_EXTERN void CQCreateDirectory(const char* pszDirectory);						// 创建目录。
CQMAIN_EXTERN void CQDeleteDirectory(const char* pszDirectory);		                    // 删除目录

CQMAIN_EXTERN FILE* CQfopen(const char* pszFile, const char* pszMode);				// 打开文件。用此函数代替fopen
CQMAIN_EXTERN FILE* CQfopenInCase(const char* pszFile, const char* pszMode);		//转化为小写后再打开




CQMAIN_EXTERN void TestPlatformGlobalFunction();