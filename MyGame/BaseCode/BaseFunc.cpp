#include "BaseFunc.h"
#include <stdlib.h>

const int MAX_LOGMSG = 8;
char g_setMsg[MAX_LOGMSG][1024] = { "", "", "" };
DWORD g_dwMsgPt = 0;

void LogMyMsg(const char* fmt, ...)
{
	if (!fmt)
		return;

	char buffer[1024] = "";

	va_list ap;
	va_start(ap, fmt);

	_vsnprintf_s(buffer, sizeof(buffer), fmt, ap);

	// msg cache
	int i = 0;
	for (; i < MAX_LOGMSG; ++i)
	{
		if (0 == strcmp(g_setMsg[i], buffer))
			break;
	}

	if (i < MAX_LOGMSG)
		return;

	SafeStrcpy(g_setMsg[g_dwMsgPt], buffer, sizeof(g_setMsg[g_dwMsgPt]));
	g_dwMsgPt++;
	g_dwMsgPt %= MAX_LOGMSG;

	struct tm newtime;
	__time64_t long_time;
	// Get time as 64-bit integer.  
	_time64(&long_time);

	// Convert to local time.  
	errno_t  err = _localtime64_s(&newtime, &long_time);
	// save msg
	char szLogDirName[512] = "";
	SafeSprintf(szLogDirName, sizeof(szLogDirName) - 1, "%s/%s", CQGetApplicationDir(), "debug");
	CQCreateDirectory(szLogDirName);


	char szLogName[512] = "";
	SafeSprintf(szLogName, sizeof(szLogName) - 1,
		"%s/%u_%u_%u.log",
		szLogDirName,
		newtime.tm_year + 1900,
		newtime.tm_mon + 1,
		newtime.tm_mday);

	FILE* fp = NULL;
	fopen_s(&fp, szLogName, "a+");
	if (!fp)
		return;
	// Convert to an ASCII representation.   
	char timebuf[26];
	err = asctime_s(timebuf, 26, &newtime);

	fprintf(fp, "%s -- %s", buffer, timebuf);
	fclose(fp);
}


void LogMyMsgAlays(const char* fmt, ...)
{
	if (!fmt)
		return;

	char buffer[1024] = "";

	va_list ap;
	va_start(ap, fmt);

	_vsnprintf_s(buffer, sizeof(buffer), fmt, ap);

	struct tm newtime;
	__time64_t long_time;
	// Get time as 64-bit integer.  
	_time64(&long_time);

	// Convert to local time.  
	errno_t  err = _localtime64_s(&newtime, &long_time);
	// save msg
	char szLogDirName[512] = "";
	SafeSprintf(szLogDirName, sizeof(szLogDirName) - 1, "%s/%s", CQGetApplicationDir(), "debug");
	CQCreateDirectory(szLogDirName);


	char szLogName[512] = "";
	SafeSprintf(szLogName, sizeof(szLogName) - 1,
		"%s/%u_%u_%u.log",
		szLogDirName,
		newtime.tm_year + 1900,
		newtime.tm_mon + 1,
		newtime.tm_mday);

	FILE* fp = NULL;
	fopen_s(&fp, szLogName, "a+");
	if (!fp)
		return;
	// Convert to an ASCII representation.   
	char timebuf[26];
	err = asctime_s(timebuf, 26, &newtime);

	fprintf(fp, "%s -- %s", buffer, timebuf);
	fclose(fp);
}

void DebugMyMsg(const char* fmt, ...)
{
	va_list arglist;
	char szMsg[1024] = {};
	va_start(arglist, fmt);
	vsprintf_s(szMsg, fmt, arglist);
	va_end(arglist);
	strncat_s(szMsg, sizeof(szMsg), "\n", _TRUNCATE);
	::OutputDebugStringA(szMsg);
	printf("%s\n", szMsg);
}

void SafeStrcpy(char* pszTarget, const char* pszSource, int nBufSize)
{
	strncpy_s(pszTarget, nBufSize, pszSource, _TRUNCATE);
}

void SafeSprintf(char* pszDest, int nBuffSize, const char* fmt, ...)
{
	if (!fmt)
		return;
	char buffer[1024] = "";
	va_list ap;
	va_start(ap, fmt);

	_vsnprintf_s(buffer, sizeof(buffer), fmt, ap);

	SafeStrcpy(pszDest, buffer, nBuffSize);
}

bool safestrcat(char* pszDest, int nDestMaxSize, const char* pszCat)
{
	if (!pszDest || !pszCat)
		return false;
	strncat_s(pszDest, nDestMaxSize, pszCat, _TRUNCATE);
	pszDest[nDestMaxSize - 1] = '\0';
	return true;
}

void StringToLower(char* pszStr, int nBufSize)
{
	if (NULL == pszStr)
		return;

	for (int i = 0; i < nBufSize; i++)
		pszStr[i] = tolower(pszStr[i]);
}

void StringToUpper(char* pszStr, int nBufSize)
{
	if (NULL == pszStr)
		return;

	for (int i = 0; i < nBufSize; i++)
		pszStr[i] = toupper(pszStr[i]);
}

bool IsEndsWith(const std::string& str, const std::string& pattern, bool lowerCase)
{
	size_t thisLen = str.length();
	size_t patternLen = pattern.length();
	if (thisLen < patternLen || patternLen == 0)
		return false;

	std::string endOfThis = str.substr(thisLen - patternLen, patternLen);
	if (lowerCase)
		StringToLower((char*)endOfThis.c_str(), endOfThis.length());

	return (endOfThis == pattern);
}

std::string GetFileContent(const char *pszFileName)
{
	std::string sRet = "";
	do
	{
		if (!pszFileName)
		{
			break;
		}

		FILE* fp = CQfopenInCase(pszFileName, "r");
		if (!fp)
		{
			break;
		}

		const size_t bufSize = 1024;
		char szBuf[bufSize] = { 0 };
		while (!feof(fp))
		{
			if (NULL != fgets(szBuf, bufSize, fp))
			{
				sRet += szBuf;
			}
		}

		fclose(fp);

	} while (false);

	return sRet;
}

DWORD TimeGet(void)
{
	return ::GetTickCount();
}

UINT64 CQGetTickCount64()
{
	UINT64 dwCurTime = 0xffffffff;
	dwCurTime = dwCurTime << 32;
	dwCurTime |= GetTickCount();
	return dwCurTime;
}

DWORD SysTimeGet(void)
{
	struct tm newtime;
	__time64_t long_time;
	// Get time as 64-bit integer.  
	_time64(&long_time);

	// Convert to local time.  
	errno_t  err = _localtime64_s(&newtime, &long_time);

	DWORD dwTime = newtime.tm_hour * 100 + newtime.tm_min;
	return dwTime;
}

std::wstring gAnsiToUnicode(const char* szStr)
{
	int nLen = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szStr, -1, NULL, 0);
	if (nLen == 0)
	{
		return NULL;
	}
	wchar_t* pResult = new wchar_t[nLen];
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szStr, -1, pResult, nLen);

	std::wstring strText;
	strText = pResult;
	delete[] pResult;
	return strText;
}
