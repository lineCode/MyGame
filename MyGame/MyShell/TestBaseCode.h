#pragma once

#include "MyLockCs.h "
#include "MyTimer.h"
class TestBaseCode
{
public:
	static TestBaseCode& GetInstance();
	static TestBaseCode* GetInstancePtr();

	TestBaseCode();
	~TestBaseCode();
	void Release();
	void RunTest();

	void OnThreadBeginAutoFindRoad();

private:
	bool BeginAutoFindRoad();
	void EndAutoFindRoad();

	class CMyThread*	m_pThreadAutoFindRoad;
	class CThreadEventAutoFindRoad* m_pThreadEventAutoFindRoad;
	CMyLockCs m_lockFindRoad;
	CMyTimer mTimerUpdate;
};

#define g_objTestBaseCode TestBaseCode::GetInstance()


