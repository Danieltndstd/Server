#include "StdAfx.h"
#include "TaskData.h"


#pragma comment (lib, "wldap32.lib" )
#pragma comment (lib, "libcurl.lib")

char CTaskData::m_szWebData[256];
size_t CTaskData::m_dataSize = 0;

CTaskData::CTaskData(void)
{
	m_pCurl = curl_easy_init(); 
	m_bExit = false;
	m_hEvent = CreateEvent(NULL, false, false, NULL);

	m_dwErrorNum = 0;
}


CTaskData::~CTaskData(void)
{
	if (m_hEvent != INVALID_HANDLE_VALUE)
		CloseHandle(m_hEvent);
	if (m_handle != INVALID_HANDLE_VALUE)
		CloseHandle(m_handle);

	curl_easy_cleanup(m_pCurl);  
}


bool CTaskData::Create()
{

	//获取路径
	TCHAR szWorkDir[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szWorkDir,CountArray(szWorkDir));

	//构造路径
	TCHAR szIniFile[MAX_PATH]=TEXT("");
	_sntprintf(szIniFile,CountArray(szIniFile),TEXT("%s\\TaskSystem.ini"),szWorkDir);

	GetPrivateProfileString(TEXT("TaskEvent"), TEXT("EventAPI"), TEXT(""), m_eventApi, CountArray(m_eventApi), szIniFile);

	GetPrivateProfileString(TEXT("TaskEvent"), TEXT("Admin"), TEXT("admin"), m_admin, CountArray(m_admin), szIniFile);

	GetPrivateProfileString(TEXT("TaskEvent"), TEXT("Password"), TEXT("f"), m_password, CountArray(m_password), szIniFile);


	m_bExit = false;
	m_handle = (HANDLE)_beginthreadex(NULL, 0, _ThreadFunc, this, 0 , NULL);	

	return true;
}

bool CTaskData::ResumeThread()
{
	SetEvent(m_hEvent);

	return true;
}

bool CTaskData::CloseThread()
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

unsigned int __stdcall CTaskData::_ThreadFunc(void* params)
{
	CTaskData* pThread = (CTaskData*)params;

	while (!pThread->m_bExit)
	{
		DWORD ret = WaitForSingleObject(pThread->m_hEvent, INFINITE);

		if (ret == WAIT_OBJECT_0)
		{
			pThread->Run();
		}
	}

	return 0;
}

void CTaskData::Run()
{
	if (m_bExit) return;

	m_lock.Lock();

	auto iter = m_listJob.begin();

	if (iter == m_listJob.end())
	{
		ResetEvent(m_hEvent);
		m_lock.UnLock();
		return ;
	}

	string retJob = DoOneJob(iter->dwUserID, iter->szEventCode);

	if (retJob.compare("success") == 0 || retJob.compare("unauth") == 0 || retJob.compare("failed") == 0) 
	{
		m_listJob.erase(iter);
		m_lock.UnLock();
	}
	//else if (retJob.compare("failed") == 0 || retJob.compare("exception")==0 || retJob.empty())
	//{
	//	tagTaskJob obj = *iter;
	//	m_listJob.erase(iter);
	//	m_listJob.push_back(obj);
	//	m_lock.UnLock();
	//	Sleep(5000);	
	//}
	else
	{
		tagTaskJob obj = *iter;
		m_listJob.erase(iter);
		m_listJob.push_back(obj);
		m_lock.UnLock();

		m_dwErrorNum++;

		Sleep(5000);	
	}

	Run();
}

void CTaskData::ParseTaskData(DBO_GR_TaskMap* pTaskData)
{
	for (auto iter = m_taskMap.begin(); iter != m_taskMap.end(); ++iter)
	{
		iter->clear();
	}

	m_taskMap.clear();

	vector<tagTaskData> taskVector;
	for (int i = 0; i < pTaskData->dwCount; ++i)
	{
		tagTaskData task;
		task = pTaskData->tagTask[i];

		taskVector.push_back(task);
	}

	//先提取链式任务头 用hash表存储任务关系
	hash_map<int, tagTaskData> hashMap;

	for (auto iter = taskVector.begin(); iter != taskVector.end(); )
	{
		if (iter->dwParentID == 0)
		{
			list<tagTaskData> taskList;
			taskList.push_back(*iter);
			m_taskMap.push_back(taskList);
			taskVector.erase(iter);
		}
		else 
		{
			hashMap[iter->dwParentID] = *iter;
			iter++;
		}
	}
	
	for (auto iter = m_taskMap.begin(); iter != m_taskMap.end(); ++iter)
	{
		DWORD dwParentID = iter->begin()->dwTaskID;
		while (hashMap.find(dwParentID) != hashMap.end())
		{
			iter->push_back(hashMap[dwParentID]);
			dwParentID = hashMap[dwParentID].dwTaskID;
		}
	}
}

size_t CTaskData::CopyData(void *ptr, size_t size, size_t nmemb, void *stream)
{
	int res_size;
	res_size = size * nmemb;
	memcpy(m_szWebData + m_dataSize, ptr, res_size);
	m_dataSize += res_size*2;
	
	return size * nmemb;
}


string CTaskData::UTF8ToGBK(const std::string& strUTF8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);  
	WCHAR* wszGBK = new WCHAR[len+1];
	memset(wszGBK, 0, len * 2 + 2);  
	MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, wszGBK, len);  

	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);  
	char *szGBK = new char[len + 1];  
	memset(szGBK, 0, len + 1);  
	WideCharToMultiByte(CP_ACP,0, wszGBK, -1, szGBK, len, NULL, NULL);   
	std::string strTemp(szGBK);  
	delete[]szGBK;  
	delete[]wszGBK;  
	return strTemp;  	
}

void CTaskData::DoJob(DWORD dwUserID, TCHAR* szEventCode)
{
	tagTaskJob tagJob;
	tagJob.dwUserID = dwUserID;
	lstrcpyn(tagJob.szEventCode, szEventCode, 30);
	
	AutoLock lock(&m_lock);

	m_listJob.push_back(tagJob);

	auto iter = m_listJob.begin();

	if (iter  != m_listJob.end())
	{
		ResumeThread();
	}
}

string CTaskData::DoOneJob(DWORD dwUserID, TCHAR* szEventCode)
{
//	if (!IsExistEventCode(szEventCode)) return string("noexit");

	if (m_dwErrorNum >= ERROR_NUM)
	{
		if (m_pCurl != nullptr)
		{
			curl_easy_cleanup(m_pCurl);
			m_pCurl = nullptr;
		}

		m_pCurl = curl_easy_init();
		m_dwErrorNum = 0;
	}

	CStringA strEventCode(szEventCode);
	CStringA http(m_eventApi);
	CStringA fmt;
	//fmt.Format("http://192.168.1.99:2444/api/GameEventTrigger?userId=%d&event_code=%s&username=admin&pwd=f", dwUserID, strEventCode);

	fmt.Format("/api/GameEventTrigger?userId=%d&event_code=%s&username=%s&pwd=%s", dwUserID, strEventCode, CStringA(m_admin), CStringA(m_password));

	http += fmt;

	CURLcode retCode;

	OutputDebugString(CString(http));
	 
	ZeroMemory(m_szWebData, 256);
	m_dataSize = 0;

	string ret("noexit");

	if (m_pCurl != nullptr)
	{
		curl_easy_setopt(m_pCurl, CURLOPT_URL, http);  
		curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDSIZE,0);
		curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDS, 0);
		curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, &CTaskData::CopyData);
		curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, 5);
		retCode =  curl_easy_perform(m_pCurl);  
		
		ret = UTF8ToGBK(string(m_szWebData));
	}

	return ret;
}


bool CTaskData::IsExistEventCode(TCHAR* szEventCode)
{
	if (szEventCode == nullptr) return false;

	for (auto iter = m_taskMap.begin(); iter != m_taskMap.end(); ++iter)
	{
		for (auto interIter = iter->begin(); interIter != iter->end(); ++interIter)
		{
			if (lstrcmp(szEventCode, interIter->szEventCode) == 0) return true;
		}
	}

	return false;
}