#ifndef _SINGLE_OBJECT_
#define _SINGLE_OBJECT_

template <class T>
class Singleton
{
public:
	//����ģʽ��ֹ����������ڽ���������Щʱ�����GetInstanceʵ������
	static T* Singleton::GetInstance()
	{
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

};
#endif