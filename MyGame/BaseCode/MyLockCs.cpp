
#include "MyLockCs.h"
#include "BaseFunc.h"

CMyLockCs::CMyLockCs()
{
	InitializeCriticalSection(&cs);
}


CMyLockCs::~CMyLockCs()
{
	DeleteCriticalSection(&cs);
}

void CMyLockCs::Enter()
{
	int nRet = 0;
	EnterCriticalSection(&cs);
	if (nRet != 0)
	{
		DebugMyMsg("CMyLockCs::Enter error");
		LogMyMsg("CMyLockCs::Enter error");
	}
}

void CMyLockCs::Leave()
{
	int nRet = 0;
	LeaveCriticalSection(&cs);
	if (nRet != 0)
	{
		DebugMyMsg("CMyLockCs::Leave error");
		LogMyMsg("CMyLockCs::Leave error");
	}
}
