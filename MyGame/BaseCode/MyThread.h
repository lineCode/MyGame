#pragma once
#include "PlatformInterface.h"

const DWORD TIME_WAITINGCLOSE = 3000;		// 3000ms


class IThreadEvent
{
public:
	virtual ~IThreadEvent() {}
	virtual void	OnThreadCreate(void) {} //����true��ʱ���߳�ֱ�ӽ���
	virtual void	OnThreadDestroy(void) {}
	virtual bool	OnThreadProcess(void) { return true; } //����true��ʱ������ѭ��
};

class CMyThread
{
public:
	CMyThread(IThreadEvent& event);
	virtual ~CMyThread();
	void EndThread();
public:
	enum { SUSPEND = true, RUN = false };
	bool	Create(bool bSuspend = SUSPEND, DWORD dwWorkInterval = INFINITE);


private:
	DWORD Run(void);

protected:
	DWORD			m_dwWorkInterval;
	IThreadEvent&	m_event;

private:
	HANDLE		m_hThread;

	static DWORD WINAPI RunThread(LPVOID pThreadParameter);

public:
	static CMyThread*	CreateNew(IThreadEvent& refEvent, bool bSuspend = SUSPEND, DWORD dwWorkInterval = INFINITE);
};

