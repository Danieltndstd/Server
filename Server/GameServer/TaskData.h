#pragma once

#include "DataBasePacket.h"
#include "AdoThreadLock.h"
#include <vector>
#include <list>
#include <hash_map>

#include <string>
#include "..\..\服务器组件\libcurl\include\curl.h"
//#include "ITask.h"
 
using namespace std;

//只能后期优化了

//错误20次重新申请连接
#define ERROR_NUM 20

struct tagTaskJob
{
	DWORD			dwUserID;
	TCHAR			szEventCode[30];

	tagTaskJob()
	{
		dwUserID = 0;
		ZeroMemory(szEventCode,  sizeof TCHAR * 30);
	}

	tagTaskJob(const tagTaskJob& other)
	{
		dwUserID = other.dwUserID;
		lstrcpyn(szEventCode, other.szEventCode, 30);
	}

	const tagTaskJob& operator = (const tagTaskJob& other)
	{
		dwUserID = other.dwUserID;
		lstrcpyn(szEventCode, other.szEventCode, 30);

		return *this;
	}
};

class CTaskData //: public ITask
{
public:
	CTaskData(void);
	~CTaskData(void);


	bool							Create();

	bool							ResumeThread();

	bool							CloseThread();



	void							ParseTaskData(DBO_GR_TaskMap* pTaskData);

	void							Run();

	string							DoOneJob(DWORD dwUserID, TCHAR* szEventCode);

	void							DoJob(DWORD dwUserID, TCHAR* szEventCode);

	static size_t					CopyData(void *ptr, size_t size, size_t nmemb, void *stream);

	static string					UTF8ToGBK(const std::string& strUTF8);
	
	static unsigned int __stdcall	_ThreadFunc(void* params);

protected:
	bool							IsExistEventCode(TCHAR* szEventCode);

private:
	vector<list<tagTaskData>>	m_taskMap;

	list<tagTaskJob>			m_listJob;

	bool						m_bExit;

	CriLock						m_lock;

	HANDLE						m_handle;

	HANDLE						m_hEvent;

	TCHAR						m_eventApi[256];

	TCHAR						m_admin[32];

	TCHAR						m_password[32];
	
	CURL*						m_pCurl; 

	static char					m_szWebData[256];

	static size_t				m_dataSize;

	DWORD						m_dwErrorNum;			//失败次数， 100次的话重新来过
};

