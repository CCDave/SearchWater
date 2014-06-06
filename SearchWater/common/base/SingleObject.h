#ifndef _SINGLE_OBJECT_
#define _SINGLE_OBJECT_
#include "Lock.h"

template <class T>
class Singleton
{
public:
	//单例模式防止拷贝，最好在进程启动早些时候调用GetInstance实例化。
	T* Singleton::GetInstance()
	{
		Lock lock(cs);          
		static T _SingleObject;
		return &_SingleObject;
	}

protected:
	Singleton()
	{
	}
	Singleton(const Singleton &)
	{
	}
	Singleton& operator = (const Singleton &)
	{
	}

public:
	static CCriticalSection cs;
};
#endif