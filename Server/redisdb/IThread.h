/*
	�ļ�	��IThread.h
	���	���Ӵ���������ʵ��_Run����ʵ���Լ����߳��ඨ��

	����޸�����	�޸���		�޸�����
	---------------------------------------------------------------------------------------------

	---------------------------------------------------------------------------------------------
*/

#ifndef __ITHREAD_H__
#define __ITHREAD_H__

#include "BaseType.h"


#ifdef WIN32
    //#include <windows.h>
#else
    #include <pthread.h>
#endif
#ifdef WIN32
    #define THREADPROC DWORD WINAPI
#else
    #define THREADPROC void*
#endif


#ifdef WIN32
# define MySleep Sleep
#else
# define MySleep(x) usleep((x)*1000)
#endif


class IThread
{
public:
	IThread(void)
    {
        m_bRun = false;
    }
	virtual ~IThread(void)
    {
    }
	// ��ʼ�߳�
	void Start(void)
    {
        m_bRun = true;
#ifdef WIN32
        ::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)(__ThreadProc),this ,0 ,NULL);
#else
        pthread_t uThreadID;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&uThreadID,&attr, __ThreadProc, this);
#endif
    }
    // �����߳�
    void Stop(void)
    {
        m_bRun = false;
    }
    virtual bool IsRunning()
    {
        return m_bRun;
    }

    static unsigned long GetThreadID()
    {
        unsigned long nRet = 0;
#if WIN32
		unsigned long ulThreadID = GetCurrentThreadId();
		nRet = ulThreadID;
#else 
		pthread_t tid = pthread_self();
		nRet = (unsigned long) tid;
#endif
        return nRet;
    }

protected:
	virtual void _Run(void) = 0;
    virtual VOID OnExit()
    {
        m_bRun = false;
    }
private:
	static THREADPROC __ThreadProc(void* lParam)
    {
        IThread* pThread = (IThread *)lParam;
        pThread->_Run();
        pThread->OnExit();
        return NULL;
    }
private:
    volatile bool m_bRun;
};

#endif
