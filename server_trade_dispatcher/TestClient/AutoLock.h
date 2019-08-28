
#ifndef _AutoLock_H
#define _AutoLock_H

#include <windows.h>
//临界区锁
class CLock_CS
{
public:
	CLock_CS(void)
	{
		::InitializeCriticalSection(&m_cs);
	}

	virtual~CLock_CS(void)
	{
		::DeleteCriticalSection(&m_cs);
	}

	void Lock(void)
	{
		::EnterCriticalSection(&m_cs);
	}

	void Unlock(void)
	{
		::LeaveCriticalSection(&m_cs);
	}
private:
	CRITICAL_SECTION m_cs;
};

//自动锁
class CAutoLock
{
public:
	CAutoLock(CLock_CS & lock)
		: m_lock(lock)
	{
		m_lock.Lock();
	}
	virtual~CAutoLock()
	{
		m_lock.Unlock();
	}
private:
	CLock_CS & m_lock;
};

#endif