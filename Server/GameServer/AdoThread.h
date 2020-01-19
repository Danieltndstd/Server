#pragma once

#include "CAdoAdapter.h"
#include "Singleton.h"
#include <list>
#include "IJob.h"
#include "AdoThreadLock.h"
#include <memory>

using std::list;
using namespace std::tr1;


class AdoThread : public Singleton<AdoThread>
{
public:
	AdoThread(void);
	~AdoThread(void);

	bool							Create();

	bool							ResumeThread();

	bool							CloseAdoThread();

	bool							PostTask(shared_ptr<IJob>  task);

	CAdoAdapter*					GetAdoInterface();

	void							DoJob();

	static unsigned int __stdcall	_ThreadFunc(void* params);

private:
	CAdoAdapter			m_pAdoAdapter;

	bool				m_bExit;

	CriLock				m_lock;

	HANDLE				m_handle;

	HANDLE				m_hEvent;

	list<shared_ptr<IJob>>		m_task;
};


