#include "MyString.h"
#include "BaseFunc.h"

CMyString::CMyString()
{
}


CMyString::~CMyString()
{
}

void CMyString::format(const char* fmt, ...)
{
	if (!fmt)
		return;

	try {
		char buffer[1024] = { 0 };
		vsprintf_s(buffer, fmt, (char*)((&fmt) + 1));
		this->assign(buffer);
	}
	catch (...)
	{
		::LogMyMsg("Error: too big size of string in format.");
	}
}

void CMyString::append(int nData)
{
	char szData[32] = { 0 };
	_itoa_s(nData, szData, sizeof(szData), 10);
	insert(size(), szData);
}

void CMyString::append(unsigned int uData)
{
	char szData[32] = { 0 };
	_i64toa_s(uData, szData, sizeof(szData), 10);
	insert(size(), szData);
}

void CMyString::append(float fData)
{
	char szData[32] = { 0 };
	sprintf_s(szData, "%.3f", fData);
	insert(size(), szData);
}

void CMyString::append(char c)
{
	insert(size(), 1, c);
}

void CMyString::append(const char* pszStr)
{
	if (strlen(pszStr) > 0)
	{
		insert(size(), pszStr);
	}
}

void CMyString::append(const char* pszStr, int iBeginIndex, int iEndIndex)
{
	for (int i = iBeginIndex; i < iEndIndex; i++)
	{
		push_back(pszStr[i]);
	}
}

bool CMyString::endsWith(const string& substr)
{
	return rfind(substr) == (length() - substr.length());
}

void CMyString::changeSmallCase()
{
	CMyString str;
	const char* pszStr = c_str();
	int off = 'A' - 'a';
	for (unsigned int i = 0; i < size(); i++)
	{
		if (pszStr[i] >= 'A' && pszStr[i] <= 'Z')
		{
			str += pszStr[i] - off;
		}
		else {
			str += pszStr[i];
		}
	}
	*this = str;
}

void CMyString::replaceChar(char cFind, char cReplace)
{
	CMyString str;
	const char* pszStr = c_str();
	for (unsigned int i = 0; i < size(); i++)
	{
		if (pszStr[i] == cFind)
		{
			str += cReplace;
		}
		else {
			str += pszStr[i];
		}
	}
	*this = str;
}

void CMyString::replaceString(const char* pszFind, const char* pszReplace)
{
	int iPosBegin = 0;
	do
	{
		size_t iIndex = find(pszFind, iPosBegin);
		if (npos == iIndex)
			break;
		replace(iIndex, strlen(pszFind), pszReplace);
		iPosBegin = iIndex + strlen(pszReplace);
	} while (true);
}

void CMyString::replaceStringFromRight(const char* pszFind, const char* pszReplace)
{
	size_t iIndex = rfind(pszFind);
	if (npos != iIndex)
	{
		replace(iIndex, strlen(pszFind), pszReplace);
	}
}

int CMyString::replaceStringFromLeft(int iPosBegin, const char* pszFind, const char* pszReplace)
{
	size_t iIndex = find(pszFind, iPosBegin);
	if (npos != iIndex)
	{
		replace(iIndex, strlen(pszFind), pszReplace);
		return iIndex + strlen(pszReplace);
	}
	return -1;
}
