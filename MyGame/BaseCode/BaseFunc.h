#pragma once

#include "PlatformInterface.h"

template <typename T>
struct TAutoZeroMem
{
	TAutoZeroMem()
	{
		::memset(static_cast<T*>(this), 0, sizeof(T));
	}
};
#define STRUCT_AUTO_ZERO(x) struct x : TAutoZeroMem<x>

#ifndef RETURN_IF_TRUE
#define RETURN_IF_TRUE(VALUE) { if(VALUE) return;}
#endif

#ifndef SAFE_NEW
#define SAFE_NEW(p, s) { if(NULL == p) p = new s; }
#endif

#ifndef SAFE_DELETE
#ifdef _DEBUG
#define SAFE_DELETE(p) { if(p) delete p; p=NULL; }
#else
#define SAFE_DELETE(p) { if(p){ try{ delete p; } catch(...){} p=NULL; }}
#endif
#endif

#ifndef SAFE_DELETE_EX
#ifdef _DEBUG
#define SAFE_DELETE_EX(p) { if(p) delete [] p; p=NULL; }
#else
#define SAFE_DELETE_EX(p) { if(p){ try{ delete [] p; } catch(...){} p=NULL; }}
#endif
#endif

#ifndef	SAFE_RELEASE
#define SAFE_RELEASE(ptr)	{ if(ptr){ try{ ptr->Release(); }catch(...){} ptr = 0; } }
#endif

#ifndef SAFE_DELETE_ARRAY_ARRAY
#ifdef _DEBUG
#define SAFE_DELETE_ARRAY_ARRAY(p, n) { if(p) { for(int i = 0; i < n; i++) delete[] p[i]; delete[] p; p = NULL;} }
#else
#define SAFE_DELETE_ARRAY_ARRAY(p, n) { if(p){ try{ for(int i = 0; i < n; i++) delete[] p[i]; delete[] p; p = NULL;} catch(...){} p=NULL;}}
#endif
#endif

#ifndef SAFE_DELETE_VEC
#define SAFE_DELETE_VEC(vec) { for (int i= vec.size()-1; i>=0; i--) delete vec[i]; vec.clear();}
#endif

#ifndef SAFE_DELETE_MAP
#define SAFE_DELETE_MAP(map, map_type) { for (map_type::iterator it= map.begin(); it != map.end(); it++) delete it->second; map.clear();}
#endif



void	LogMyMsg(const char* fmt, ...);
void	LogMyMsgAlays(const char* fmt, ...);

void DebugMyMsg(const char* fmt, ...);

void	SafeStrcpy(char* pszTarget, const char* pszSource, int nBufSize);//字符串拷贝

void SafeSprintf(char* pszDest, int nBuffSize, const char* fmt, ...);

bool	safestrcat(char* pszDest, int nDestMaxSize, const char* strCat);//字符串拼接


void	StringToLower(char* pszStr, int nBufSize); //大写字母转换为小写
void	StringToUpper(char* pszStr, int nBufSize); //小写字母转换为大写
bool	IsEndsWith(const std::string& str, const std::string& pattern, bool lowerCase);//后缀


//读取文件全部内容
std::string GetFileContent(const char *pszFileName);


//时间
// 返回自操作系统启动以来的毫秒数, 内部调用timegettime()函数
DWORD	TimeGet(void);

UINT64	CQGetTickCount64();

// 返回本地时间的"小时+分钟"的数字格式, 即 hour*100+min, 例如21点30分则返回: 2130
DWORD	SysTimeGet(void);


//颜色  ARGB
inline DWORD ColorMakeARGB(BYTE btA, BYTE btR, BYTE btG, BYTE btB)
{
	return (DWORD(btA) << 24) | (DWORD(btR) << 16) | (DWORD(btG) << 8) | DWORD(btB);
}

inline DWORD ColorSetA(IN OUT DWORD& dwColor, BYTE btA)
{
	return dwColor & 0x00FFFFFF | (DWORD(btA) << 24);
}

inline DWORD ColorSetR(IN OUT DWORD& dwColor, BYTE btR)
{
	return dwColor & 0xFF00FFFF | (DWORD(btR) << 16);
}

inline DWORD ColorSetG(IN OUT DWORD& dwColor, BYTE btG)
{
	return dwColor & 0xFFFF00FF | (DWORD(btG) << 8);
}

inline DWORD ColorSetB(DWORD dwColor, BYTE btB)
{
	return dwColor & 0xFFFFFF00 | DWORD(btB);
}

inline BYTE ColorGetA(DWORD dwColor)
{
	return BYTE((dwColor & 0xFF000000) >> 24);
}

inline BYTE ColorGetR(DWORD dwColor)
{
	return BYTE((dwColor & 0x00FF0000) >> 16);
}

inline BYTE ColorGetG(DWORD dwColor)
{
	return BYTE((dwColor & 0x0000FF00) >> 8);
}

inline BYTE ColorGetB(DWORD dwColor)
{
	return BYTE(dwColor & 0x000000FF);
}
