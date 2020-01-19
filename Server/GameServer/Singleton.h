#pragma once

template<typename T>
class Singleton
{
	typedef T object_type;
public:
	static  object_type *  Instance()			//c++0x static�ֲ��������̰߳�ȫ�� ���ؼ���
	{
		static object_type _object; 
		return  &_object;
	}

protected:
	Singleton() {};

	Singleton(const Singleton<object_type> &);

	Singleton<object_type>& operator =(const Singleton<object_type> &);
};
