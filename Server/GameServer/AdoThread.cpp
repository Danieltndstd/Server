#include "StdAfx.h"
#include "AdoThread.h"

#define DBR_GR_WRITE_VIDEO 900

AdoThread::AdoThread(void)
{
	m_bExit = false;
	m_hEvent = CreateEvent(NULL, false, false, NULL);
}

AdoThread::~AdoThread(void)
{
	if (m_hEvent != INVALID_HANDLE_VALUE)
		CloseHandle(m_hEvent);
	if (m_handle != INVALID_HANDLE_VALUE)
		CloseHandle(m_handle);
}

bool AdoThread::Create()
{
	m_bExit = false;
	m_handle = (HANDLE)_beginthreadex(NULL, 0, _ThreadFunc, this, 0 , NULL);	

	return true;
}


CAdoAdapter* AdoThread::GetAdoInterface()
{
	return &m_pAdoAdapter;
}

unsigned int __stdcall AdoThread::_ThreadFunc(void* params)
{
	AdoThread* pThread = (AdoThread*)params;

	while (!pThread->m_bExit)
	{
		DWORD ret = WaitForSingleObject(pThread->m_hEvent, INFINITE);

		if (ret == WAIT_OBJECT_0)
		{
			pThread->DoJob();
		}
	}

	return 0;
}

bool AdoThread::PostTask( shared_ptr<IJob> task)
{
	AutoLock lock(&m_lock);
	
	m_task.push_back(task);

	auto iter = m_task.begin();

	if (iter != m_task.end())
	{
		ResumeThread();
	}

	return true;
}

bool AdoThread::ResumeThread()
{
	SetEvent(m_hEvent);

	return true;
}

void AdoThread::DoJob()
{
	if (m_bExit) return;

	m_lock.Lock();

	auto iter = m_task.begin();

	if (iter == m_task.end())
	{
		ResetEvent(m_hEvent);
		m_lock.UnLock();
		return ;
	}

	bool bJob = (*iter)->DoJob(&m_pAdoAdapter);

	if (bJob == false) 
	{
		m_lock.UnLock();
		Sleep(5000);
	}
	else 
	{
		iter->reset();
		m_task.erase(iter);
		m_lock.UnLock();
	}

	DoJob();
}

bool AdoThread::CloseAdoThread()
{
	m_bExit = true;

	ResumeThread();


	if (m_handle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_handle);
		m_handle = INVALID_HANDLE_VALUE;
	}
	return true;
}