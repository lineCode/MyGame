
#include "MyTimer.h"
#include "BaseFunc.h"


CMyTimer::CMyTimer()
{
	m_u64Interval = 0;
	m_u64UpdateTime = 0;
	m_u64PauseTime = 0;
}

CMyTimer::~CMyTimer()
{

}
void	CMyTimer::Copy(CMyTimer& tmSrc)
{
	m_u64UpdateTime = tmSrc.m_u64UpdateTime;
	m_u64Interval = tmSrc.m_u64Interval;
	m_u64PauseTime = tmSrc.m_u64PauseTime;
}
void	CMyTimer::Update()
{
	m_u64UpdateTime = ::CQGetTickCount64();
	m_u64PauseTime = 0;
}

bool	CMyTimer::IsTimeOut()
{
	if (m_u64PauseTime == 0)
	{
		return (IsActive() && ::CQGetTickCount64() - m_u64UpdateTime >= m_u64Interval);
	}
	else
	{
		return false;
	}

}
UINT64	CMyTimer::GetLeaveTime()
{
	if (!IsActive())
		return 0;
	int64 i64LeaveTime;
	if (m_u64PauseTime == 0)
	{
		i64LeaveTime = m_u64Interval + m_u64UpdateTime - ::CQGetTickCount64();
	}
	else
	{
		i64LeaveTime = m_u64Interval + m_u64UpdateTime - m_u64PauseTime;
	}
	return __max(i64LeaveTime, 0);
}

UINT64	CMyTimer::GetPassTime()
{
	if (!IsActive())
		return 0;
	int64 i64PassTime;
	if (m_u64PauseTime == 0)
	{
		i64PassTime = ::CQGetTickCount64() - m_u64UpdateTime;
	}
	else
	{
		i64PassTime = m_u64PauseTime - m_u64UpdateTime;
	}
	return __max(i64PassTime, 0);
}

bool CMyTimer::SetTimeOut()
{
	if (!IsActive())
		return false;
	UINT64 dwTickCount = ::CQGetTickCount64();
	if (dwTickCount > m_u64Interval)
	{
		m_u64UpdateTime = dwTickCount - m_u64Interval;
	}
	else
	{
		return false;
	}
	m_u64PauseTime = 0;
	return true;
}

void CMyTimer::Pause()
{
	m_u64PauseTime = ::CQGetTickCount64();
}

void CMyTimer::Continue()
{
	m_u64UpdateTime += (::CQGetTickCount64() - m_u64PauseTime);
	m_u64PauseTime = 0;
}


void	CMyTimer::Startup(UINT64 u64MilliSecs)
{
	m_u64Interval = u64MilliSecs;
	Update();
	if (0 == u64MilliSecs)
		Clear();
}

void	CMyTimer::SetPassTime(UINT64 u64MilliSecs)
{
	m_u64UpdateTime -= u64MilliSecs;
}

bool CMyTimer::ToNextTime()
{
	if (IsTimeOut())
	{
		Update();
		return true;
	}
	return false;
}