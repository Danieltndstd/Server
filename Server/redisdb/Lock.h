/*
	文件	：lock.h
	简介	：跨平台智能锁定义

	最后修改日期	修改者		修改内容
	---------------------------------------------------------------------------------------------

	---------------------------------------------------------------------------------------------
*/

#ifndef __LOCK_H__
#define __LOCK_H__


#ifdef WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include "wtypes.h"
#else
#include <pthread.h> 
#endif

/**
* 作为模板参数来实现非线程保护模式
*/
class KNoneLock
{
public:
	void Lock() const
	{
	}
	void Unlock() const
	{
	}
};

class CLock
{
public:
	
	CLock(void)
    {
        //m_bNeedUnlock = false; 
        // Initialize the critical section one time only.
#ifdef WIN32
        InitializeCriticalSection(&m_Section); 
#else
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&m_mutex,&attr);	
#endif
        //ReLock(); 	
    }
	~CLock(void)
    {
        UnLock();
#ifdef WIN32
        ::DeleteCriticalSection(&m_Section);
#else
        pthread_mutex_destroy(&m_mutex);
        pthread_mutexattr_destroy(&attr);
#endif
    }

    void Lock(void)
    {
#ifdef WIN32
        EnterCriticalSection( &m_Section ); 
#else
        pthread_mutex_lock (&m_mutex);
#endif

    }
    void UnLock(void)
    {
        //m_bNeedUnlock = true;
        /*   m_bNeedUnlock = false;*/
#ifdef WIN32
        LeaveCriticalSection(&m_Section);
#else
        pthread_mutex_unlock(&m_mutex);	
#endif
    }


private:

#ifdef WIN32
	// Global variable
	CRITICAL_SECTION m_Section; 
#else 
	pthread_mutex_t m_mutex;
	pthread_mutexattr_t attr;
#endif
	//volatile bool m_bNeedUnlock; 

private:
};

class NullLock
{
public:
	NullLock(){}
	~NullLock(){}

	void Lock(){};
	void UnLock(){};

};

template<class LockT>
class SmartLock
{
private:
public:
    SmartLock(LockT& oLock):m_oLock(oLock)
    {
		m_oLock.Lock();	
    }
    ~SmartLock()
    {		
        m_oLock.UnLock();
    }
    void Lock()
    {

		/*if( m_bNeedUnLock )
		{
			return; 
		}
		m_bNeedUnLock = true; */
        m_oLock.Lock();
    }
    void Unlock()
    {
		/*if( !m_bNeedUnLock )
		{
			return; 
		}
		m_bNeedUnLock = false;*/ 
        m_oLock.UnLock();
    }
private:
    LockT& m_oLock;
};

typedef SmartLock<CLock> CSmartLock;

#endif //__LOCK_H__
