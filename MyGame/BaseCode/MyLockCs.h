#pragma once

#include <windows.h>

class CMyLockCs
{
public:
	CMyLockCs();
	~CMyLockCs();
	void Enter();
	void Leave();
private:
	CRITICAL_SECTION cs;
};

