#include "StdAfx.h"
#include "AdoThreadLock.h"


CriLock::CriLock()
{
	InitializeCriticalSection(&m_cs);
}

CriLock::~CriLock()
{
	DeleteCriticalSection(&m_cs);
}

bool CriLock::Lock()
{
	EnterCriticalSection(&m_cs);

	return true;
}

bool CriLock::UnLock()
{
	LeaveCriticalSection(&m_cs);

	return true;
}

AutoLock::AutoLock(ILock *lock)
{
	m_pLock = lock;
	m_pLock->Lock();
}

AutoLock::~AutoLock()
{
	m_pLock->UnLock();
}