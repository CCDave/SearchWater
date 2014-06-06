#ifndef _SINGLE_OBJECT_
#define _SINGLE_OBJECT_
#include "Lock.h"

template <class T>
class Singleton
{
public:
	//����ģʽ��ֹ����������ڽ���������Щʱ�����GetInstanceʵ������
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