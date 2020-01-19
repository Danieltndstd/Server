#include "StdAfx.h"
#include "ServiceUnits.h"
#include "ControlPacket.h"
#include "AttemperEngineSink.h"
//////////////////////////////////////////////////////////////////////////
#define IDI_MESSAGE				10086
#define IDI_BACKUP				20086
#define IDI_BACKUP_RESET		30086

//
#define IDT_BACKUP_DAY_TIME		86400
#define IDT_BACKUP_RESET_TIME	60

//////////////////////////////////////////////////////////////////////////////////
SLocalTime gLocalTime;

void  SGSYS_GetLocalTime(UINT32 dwTime,SLocalTime* pstLocalTime)
{
	if (dwTime == 0)
	{
		SYSTEMTIME  stTime={0};
		GetLocalTime(&stTime);

		pstLocalTime->m_nYear   = stTime.wYear;
		pstLocalTime->m_nMon    = stTime.wMonth;
		pstLocalTime->m_nMday   = stTime.wDay;
		pstLocalTime->m_nHour   = stTime.wHour;
		pstLocalTime->m_nMin    = stTime.wMinute;
		pstLocalTime->m_nSec    = stTime.wSecond;
		pstLocalTime->m_nWday   = stTime.wDayOfWeek;
	}
	else
	{
		struct tm* pstTime = localtime((time_t*)&dwTime);

		pstLocalTime->m_nSec	= pstTime->tm_sec;
		pstLocalTime->m_nMin	= pstTime->tm_min;
		pstLocalTime->m_nHour	= pstTime->tm_hour;
		pstLocalTime->m_nMday	= pstTime->tm_mday;
		pstLocalTime->m_nMon	= pstTime->tm_mon + 1;
		pstLocalTime->m_nYear	= pstTime->tm_year + 1900;
		pstLocalTime->m_nWday	= pstTime->tm_wday; 
	}
} // SGSYS_GetLocalTime

bool DeleteFileEx(LPCTSTR lpszPath)
{
	SHFILEOPSTRUCT FileOp={0};
	FileOp.fFlags = FOF_ALLOWUNDO |   //允许放回回收站
		FOF_NOCONFIRMATION; //不出现确认对话框
	FileOp.pFrom = lpszPath;
	FileOp.pTo = NULL;      //一定要是NULL
	FileOp.wFunc = FO_DELETE;    //删除操作
	return SHFileOperation(&FileOp) == 0;
}


void BackupAndMoveFile()
{
	TCHAR szPath[MAX_PATH]={0};
	GetCurrentDirectory(CountArray(szPath), szPath);

	SLocalTime locTime;
	SGSYS_GetLocalTime(0, &locTime);

	TCHAR szSrc[MAX_PATH]={0};
	TCHAR szDest[MAX_PATH]={0};
	_sntprintf(szSrc, MAX_PATH, TEXT("%s/Settings/Billboard/lists"), szPath);
	_sntprintf(szDest, MAX_PATH, TEXT("%s/Settings/Billboard/%4d_%02d_%02d"), szPath, locTime.m_nYear, locTime.m_nMon, locTime.m_nMday);

	if (!MoveFile(szSrc, szDest))
	{
		DWORD dwRet = GetLastError();
		//I_TRACE(TEXT("备份失败:[%d]"), dwRet);

		switch (dwRet)
		{
		case 183:
			{
				//I_TRACE(TEXT("存在目录不备份:%s"), szDest);
				break;
			}
		case 2:
			{
				//I_TRACE(TEXT("不存在目录不备份:%s"), szSrc);
				break;
			}
		}
	}
	else
	{
		SGSYS_GetLocalTime(0, &locTime);
		//I_TRACE(TEXT("备份时间:%4d_%02d_%02d %02d:%02d:%02d"), 
		//			locTime.m_nYear, locTime.m_nMon, locTime.m_nMday, locTime.m_nHour, locTime.m_nMin, locTime.m_nSec);
		//写入时间
		_sntprintf(szDest, MAX_PATH, TEXT("%s/%02d_%02d_%02d.ini"), szDest, locTime.m_nHour, locTime.m_nMin, locTime.m_nSec);

		WritePrivateProfileString(TEXT("DATA"), TEXT("TIME"), TEXT(""), szDest);
	}
}

bool makedirs(const TCHAR* root)
{
	if (_taccess(root, 0) == -1)
	{
		int nLen	= lstrlen(root);
		int nStart	= 0;

		for(int i = 0; i < nLen; ++i)
		{
			if(root[i]==TEXT('\\') || root[i]==TEXT('/'))
			{
				TCHAR szPath[MAX_PATH]={0};
				lstrcpyn(szPath, root, i + 1);

				if (_tmkdir(szPath) == -1)
				{
					continue;
				}

				nStart = i;
			}
		}

		if((nLen - nStart)> 0)
		{
			_tmkdir(root);
		}

		if (_taccess(root, 0) == -1)
		{
			return false;
		}
	}

	return true;
} // makedirs



int GetBackupTime()
{
	SGSYS_GetLocalTime(0, &gLocalTime);
	int left = IDT_BACKUP_DAY_TIME - gLocalTime.m_nHour * 3600 - gLocalTime.m_nMin * 60 - gLocalTime.m_nSec;
	return max(1, left);
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CAttemperEngineSink::CAttemperEngineSink()
{
	//状态变量
	m_wCollectItem=INVALID_WORD;

	//设置变量
	m_pInitParameter=NULL;
	m_pBindParameter=NULL;

	//组件变量
	m_pITimerEngine=NULL;
	m_pITCPNetworkEngine=NULL;

	//CTraceService::szLogRoot =TEXT("./SLog/CorrespondServer/");

	return;
}

//析构函数
CAttemperEngineSink::~CAttemperEngineSink()
{
}

//接口查询
VOID * CAttemperEngineSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAttemperEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAttemperEngineSink,Guid,dwQueryVer);
	return NULL;
}

//启动事件
bool CAttemperEngineSink::OnAttemperEngineStart(IUnknownEx * pIUnknownEx)
{
	//绑定参数
	m_pBindParameter=new tagBindParameter[m_pInitParameter->m_wMaxConnect];
	ZeroMemory(m_pBindParameter,sizeof(tagBindParameter)*m_pInitParameter->m_wMaxConnect);
	//StartNextMsgTime(1);
	//StartNextMsgTime(2);
	//StartNextMsgTime(3);
	//if( time(NULL)>= 1483200000 )
	//	return true;

	SGSYS_GetLocalTime(0, &gLocalTime);
	//int left = GetBackupTime();
	//m_pITimerEngine->SetTimer(IDI_BACKUP, left * 1000, 1, 0);
	//m_pITimerEngine->SetTimer(IDI_BACKUP_RESET, IDT_BACKUP_RESET_TIME * 1000, 1, 0);
	//I_TRACE(TEXT("%d秒后进行备份检查"), left);

	OnAttemperEngineBegin( pIUnknownEx );

	return true;
}

//停止事件
bool CAttemperEngineSink::OnAttemperEngineConclude(IUnknownEx * pIUnknownEx)
{
	//m_pITimerEngine->KillTimer(IDI_MESSAGE);
	//状态变量
	m_wCollectItem=INVALID_WORD;
	m_WaitCollectItemArray.RemoveAll();

	//设置变量
	m_pITimerEngine=NULL;
	m_pITCPNetworkEngine=NULL;

	//删除数据
	SafeDeleteArray(m_pBindParameter);

	//设置组件
	m_GlobalInfoManager.ResetData();

	return true;
}

//应答事件
bool CAttemperEngineSink::OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID)
{
	//获取索引
	ASSERT_ERR(LOWORD(dwSocketID)<m_pInitParameter->m_wMaxConnect);
	if (LOWORD(dwSocketID)>=m_pInitParameter->m_wMaxConnect) return false;

	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//设置变量
	pBindParameter->dwSocketID=dwSocketID;
	pBindParameter->dwClientAddr=dwClientAddr;
	pBindParameter->dwActiveTime=(DWORD)time(NULL);

	return true;
}

//数据库事件
bool CAttemperEngineSink::OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	return false;
}

//关闭事件
bool CAttemperEngineSink::OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason)
{
	return false;
}

//连接事件
bool CAttemperEngineSink::OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode)
{
	return false;
}

//读取事件
bool CAttemperEngineSink::OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize)
{
	return true;
}


//房间发送
bool CAttemperEngineSink::SendDataToGame(WORD wServerID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//查找房间
	CGlobalServerItem * pGlobalServerItem=m_GlobalInfoManager.SearchServerItem(wServerID);
	if (pGlobalServerItem==NULL) return false;

	//获取参数
	WORD wBindIndex=pGlobalServerItem->GetIndex();
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//发送数据
	DWORD dwSocketID=pBindParameter->dwSocketID;
	m_pITCPNetworkEngine->SendData(dwSocketID,wMainCmdID,wSubCmdID,pData,wDataSize);

	return true;
}
