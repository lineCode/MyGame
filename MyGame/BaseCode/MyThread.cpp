
#include "MyThread.h"
#include "BaseFunc.h"

#include <process.h>

CMyThread::CMyThread(IThreadEvent& event)
	: m_event(event), m_hThread(NULL)
{
	m_dwWorkInterval = 0;
}


CMyThread::~CMyThread()
{
	if (m_hThread)
	{
		::CloseHandle(m_hThread);
		m_hThread = NULL;
	}
}

void CMyThread::EndThread()
{
	if (m_hThread)
	{
		DWORD r = ::WaitForSingleObject(m_hThread, INFINITE);
		if (r == WAIT_TIMEOUT)
		{
			LogMyMsg("WAIT_TIMEOUT");
		}
		else if (r == WAIT_OBJECT_0)
		{
			LogMyMsg("WAIT_OBJECT_0");
		}
		else
		{

		}
	}
}
typedef unsigned(__stdcall *MYTHREAD_FUNC)(void *);

bool CMyThread::Create(bool bSuspend /*= SUSPEND*/, DWORD dwWorkInterval /*= INFINITE*/)
{
	DWORD dwCreationFlags = 0;
	if (bSuspend)
		dwCreationFlags = CREATE_SUSPENDED;
	int id = 0;
	m_hThread = (HANDLE)_beginthreadex(NULL,			// default security
		0,				// default stack size
		(MYTHREAD_FUNC)RunThread,		// pointer to thread routine
		this,			// argument for thread
		dwCreationFlags,// start it right away if 0, else suspend
		(unsigned *)&id);
	if (!m_hThread)
	{
		return false;
	}
	m_dwWorkInterval = dwWorkInterval;
	return true;
}

DWORD CMyThread::Run(void)
{
	// init
	m_event.OnThreadCreate();
	while (true)
	{
		try
		{
			// work interval is time out
			if (m_event.OnThreadProcess())
				break;
		}
		catch (...)
		{
			::LogMyMsg("CATCH: *** CMyThread::RunThread Call OnThreadProcess crash! *** at %s, %d", __FILE__, __LINE__);
		}
	}
	m_event.OnThreadDestroy();
	return 0;
}

DWORD WINAPI CMyThread::RunThread(LPVOID pThreadParameter)
{
	CMyThread* pThread = (CMyThread*)pThreadParameter;
	if (!pThread) {
		return NULL;
	}
	return pThread->Run();
}

CMyThread* CMyThread::CreateNew(IThreadEvent& refEvent, bool bSuspend /*= SUSPEND*/, DWORD dwWorkInterval /*= INFINITE*/)
{
	CMyThread* pThread = new CMyThread(refEvent);
	if (!pThread)
		return NULL;

	if (!pThread->Create(bSuspend, dwWorkInterval))
	{
		delete pThread;
		return NULL;
	}

	return pThread;
}
