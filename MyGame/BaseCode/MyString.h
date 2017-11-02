#pragma once

#include "PlatformInterface.h"
using namespace std;



// 这里要求后续使用CMyString格式化后的字符串长度不能超过4096-1
class CMyString : public string
{
public:
	CMyString();
	CMyString(const char* pstr) { if (pstr) this->assign(pstr); }
	virtual ~CMyString();

	CMyString& operator = (const std::string& str) { this->assign(str); return *this; }
	CMyString& operator = (const char* pstr) { if (pstr) this->assign(pstr); return *this; }

	const char operator [] (int nPos) { return this->at(nPos); }

	bool operator == (const std::string& str) { return (0 == this->compare(str)); }
	bool operator != (const std::string& str) { return (0 != this->compare(str)); }

	bool operator == (const char* pstr)
	{
		if (pstr)
			return (0 == this->compare(pstr));
		else
			return false;
	}
	bool operator != (const char* pstr)
	{
		return !(operator == (pstr));
	}

	operator const char*() const { return this->c_str(); }
	int ToInt() const { return atoi(this->c_str()); }

	void format(const char* fmt, ...);
	void append(int nData);
	void append(unsigned int uData);
	void append(float fData);
	void append(char c);
	void append(const char* pszStr);
	void append(const char* pszStr, int iBeginIndex, int iEndIndex);

	bool endsWith(const string& substr);//后缀
	void changeSmallCase(); //转变为小写
	void replaceChar(char cFind, char cReplace);
	void replaceString(const char* pszFind, const char* pszReplace);
	void replaceStringFromRight(const char* pszFind, const char* pszReplace);
	int replaceStringFromLeft(int iPosBegin, const char* pszFind, const char* pszReplace);
};

