/*
	文件	：Singleton.h
	简介	：单件基类，模板实现

	最后修改日期	修改者		修改内容
	---------------------------------------------------------------------------------------------

	---------------------------------------------------------------------------------------------
*/

#ifndef	_SINGLETON_H_
#define	_SINGLETON_H_

template<class T>
class Singleton
{
public:
	static inline T* GetInstancePtr(){	static T _instance;	return &_instance;}
protected:
	Singleton(void){};
	~Singleton(void){};
	Singleton(const Singleton<T>&);
	Singleton<T>& operator=(const Singleton<T>&);

};

#endif
