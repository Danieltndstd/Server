#pragma once


class ITask
{
public:
	virtual ~ITask(void){};

	virtual void DoJob(DWORD dwUserID, TCHAR* szEventCode) = 0;
};

