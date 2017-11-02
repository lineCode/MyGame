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

	//����ֱ��fopen
	FILE* pFile = NULL;
	fopen_s(&pFile, pszFile, pszMode);
	if (pFile != NULL)
		return pFile;

	//��������·��
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
	safestrcat(szFind, sizeof(szFind), "*.*"); // �������ļ�
	WIN32_FIND_DATAA wfd;
	HANDLE hFind = FindFirstFileA(szFind, &wfd);
	if (INVALID_HANDLE_VALUE != hFind)
	{
		do
		{
			if (wfd.cFileName[0] == '.')
				continue; // ����������Ŀ¼
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) //����Ŀ¼�ļ���
			{
				char szFile[MAX_PATH];
				SafeSprintf(szFile, sizeof(szFile), "%s/%s", pszDirectory, wfd.cFileName);
				CQDeleteDirectory(szFile); // ����ҵ�����Ŀ¼��������Ŀ¼���еݹ�
			}
			else
			{
				char szFileName[MAX_PATH] = { 0 };
				SafeStrcpy(szFileName, pszDirectory, sizeof(szFileName));
				safestrcat(szFileName, sizeof(szFind), "/");
				safestrcat(szFileName, sizeof(szFind), wfd.cFileName);
				remove(szFileName); //ɾ���ļ�
			}

		} while (FindNextFileA(hFind, &wfd));
		FindClose(hFind); // �رղ��Ҿ��
	}
	_rmdir(pszDirectory);//���ɾ���ļ���
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
