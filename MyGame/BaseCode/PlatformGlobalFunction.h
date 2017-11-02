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




// ���ܺ�����װ
CQMAIN_EXTERN bool	CQDeleteFile(const char* pszDirectory);							// ɾ���ļ�
CQMAIN_EXTERN void CQCreateDirectory(const char* pszDirectory);						// ����Ŀ¼��
CQMAIN_EXTERN void CQDeleteDirectory(const char* pszDirectory);		                    // ɾ��Ŀ¼

CQMAIN_EXTERN FILE* CQfopen(const char* pszFile, const char* pszMode);				// ���ļ����ô˺�������fopen
CQMAIN_EXTERN FILE* CQfopenInCase(const char* pszFile, const char* pszMode);		//ת��ΪСд���ٴ�




CQMAIN_EXTERN void TestPlatformGlobalFunction();