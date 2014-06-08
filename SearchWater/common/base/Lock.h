#ifndef _LOCK_
#define _LOCK_
#include <atlsync.h>

class Lock
{
private:       
	CCriticalSection m_cs;
public:
	Lock(CCriticalSection cs) : m_cs(cs)
	{
		m_cs.Enter();
	}
	~Lock()
	{
		m_cs.Leave();
	}
};

#endif
