#pragma once

class ILock
{
public:
	virtual bool Lock() = NULL;

	virtual bool UnLock() = NULL;
};

class CriLock : public ILock
{
public:
	CriLock();
	~CriLock();

	bool Lock();
	bool UnLock();

private:
	CRITICAL_SECTION	m_cs;
};

class AutoLock
{
public:
	AutoLock(ILock *lock);
	~AutoLock();

private:
	ILock*	m_pLock;
};