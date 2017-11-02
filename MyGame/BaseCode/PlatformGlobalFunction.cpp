#include "PlatformGlobalFunction.h"
#include "BaseFunc.h"
#include <direct.h>
#include "MyString.h"

CQMAIN_EXTERN const char* CQGetApplicationDir()
{
	static char s_szBuf[MAX_PATH] = "";
	::GetCurrentDirectoryA(MAX_PATH, s_szBuf);
	s_szBuf[MAX_PATH - 1] = '\0';
	return s_szBuf;
}

CQMAIN_EXTERN const char* CQGetLibraryCacheDir()
{
	return CQGetApplicationDir();
}

CQMAIN_EXTERN FILE* CQfopen(const char* pszFile, const char* pszMode)
{
	if (!pszFile || !pszMode)
		return NULL;

	//尝试直接fopen
	FILE* pFile = NULL;
	fopen_s(&pFile, pszFile, pszMode);
	if (pFile != NULL)
		return pFile;

	//尝试完整路径
	char szFullPath[MAX_PATH] = "";
	::SafeSprintf(szFullPath, sizeof(szFullPath), "%s/%s", CQGetApplicationDir(), pszFile);
	fopen_s(&pFile, szFullPath, pszMode);
	return pFile;
}

CQMAIN_EXTERN FILE* CQfopenInCase(const char* pszFile, const char* pszMode)
{
	CMyString str = pszFile;
	str.changeSmallCase();
	str.replaceChar('\\', '/');
	return CQfopen(str.c_str(), pszMode);
}

CQMAIN_EXTERN bool CQDeleteFile(const char* pszDirectory)
{
	return 0 == remove(pszDirectory);
}

CQMAIN_EXTERN void CQCreateDirectory(const char* pszDirectory)
{
	_mkdir(pszDirectory);
}

CQMAIN_EXTERN void CQDeleteDirectory(const char* pszDirectory)
{
	char szFind[MAX_PATH] = { 0 };
	SafeStrcpy(szFind, pszDirectory, sizeof(szFind));
	safestrcat(szFind, sizeof(szFind), "/");
	safestrcat(szFind, sizeof(szFind), "*.*"); // 找所有文件
	WIN32_FIND_DATAA wfd;
	HANDLE hFind = FindFirstFileA(szFind, &wfd);
	if (INVALID_HANDLE_VALUE != hFind)
	{
		do
		{
			if (wfd.cFileName[0] == '.')
				continue; // 过滤这两个目录
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) //发现目录文件夹
			{
				char szFile[MAX_PATH];
				SafeSprintf(szFile, sizeof(szFile), "%s/%s", pszDirectory, wfd.cFileName);
				CQDeleteDirectory(szFile); // 如果找到的是目录，则进入此目录进行递归
			}
			else
			{
				char szFileName[MAX_PATH] = { 0 };
				SafeStrcpy(szFileName, pszDirectory, sizeof(szFileName));
				safestrcat(szFileName, sizeof(szFind), "/");
				safestrcat(szFileName, sizeof(szFind), wfd.cFileName);
				remove(szFileName); //删除文件
			}

		} while (FindNextFileA(hFind, &wfd));
		FindClose(hFind); // 关闭查找句柄
	}
	_rmdir(pszDirectory);//最后删除文件夹
}

void TestPlatformGlobalFunction()
{
	const char* pszApplicationDir = CQGetApplicationDir();
	DebugMyMsg(pszApplicationDir);
	//LogMyMsg(pszApplicationDir);

	const char* pszLibraryDir = CQGetLibraryCacheDir();
	DebugMyMsg(pszLibraryDir);
	//LogMyMsg(pszLibraryDir);

	char strText[256] = "ni hao";
	char strText1[5] = "";

	safestrcat(strText, sizeof(strText), "1232");
	SafeStrcpy(strText1, strText,sizeof(strText1));
	SafeSprintf(strText, sizeof(strText), "%s", "test");
	CMyString strFileName = "my.Hlsl";
	bool bFind = strFileName.endsWith("hlsl");
	strFileName.changeSmallCase();
	bFind = strFileName.endsWith("hlsl");
	strFileName.append("womy");
	strFileName.replaceChar('m', 'p');

	int nTime = SysTimeGet();
}
