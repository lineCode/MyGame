#pragma once
#include "PlatformInterface.h"

const int TIMER_ONE_MINUTE = 60 * 1000;
const int TIMER_ONE_HOUR = 60 * TIMER_ONE_MINUTE;

class CMyTimer
{
public:
	CMyTimer();
	virtual ~CMyTimer();

public:
	void	Copy(CMyTimer& tmSrc);
	void	Update();
	bool	IsTimeOut();
	bool	ToNextTime();
	void	SetInterval(UINT64 u64MilliSecs) { m_u64Interval = u64MilliSecs; }

	void	Startup(UINT64 u64MilliSecs);//定时器开始
	void	SetPassTime(UINT64 u64MilliSecs);//设置经过的时间
	bool	SetTimeOut(); //直接设置到时间到的点
	void	Clear() { m_u64Interval = m_u64UpdateTime = m_u64PauseTime = 0; }
	bool	IsActive() { return m_u64UpdateTime != 0; }
	void    Pause();
	void    Continue();
	bool    IsPause() { return (m_u64PauseTime != 0); }

	UINT64	GetInterval() { return m_u64Interval; }
	UINT64	GetUpdateTime() { return m_u64UpdateTime; }
	void	SetUpdateTime(UINT64 u64UpdateTime) { m_u64UpdateTime = u64UpdateTime; }
	UINT64	GetLeaveTime();
	UINT64	GetPassTime();
	//为保持同步,不可在调用GetLeaveTime()后调GetLeaveTime(OUT int& nMinute, OUT int& nSec)
	void GetLeaveTime(OUT int& nMinute, OUT int& nSec) {
		int nLeaveSec = (int)(this->GetLeaveTime() + 500) / 1000;
		nMinute = nLeaveSec / 60; nSec = nLeaveSec % 60;
	}
protected:
	UINT64	m_u64Interval;
	UINT64	m_u64UpdateTime;
	UINT64 m_u64PauseTime;
};
