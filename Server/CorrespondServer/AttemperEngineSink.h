#ifndef ATTEMPER_ENGINE_SINK_HEAD_FILE
#define ATTEMPER_ENGINE_SINK_HEAD_FILE

#pragma once

#include "InitParameter.h"
#include "GlobalInfoManager.h"
#include <map>

//////////////////////////////////////////////////////////////////////////////////
//数据定义
typedef struct
{
	INT32 m_nSec;       ///< Seconds.     [0-60]
	INT32 m_nMin;       ///< Minutes.     [0-59]
	INT32 m_nHour;      ///< Hours.       [0-23]
	INT32 m_nMday;      ///< Day.         [1-31]
	INT32 m_nMon;       ///< Month.       [0-11]
	INT32 m_nYear;      ///< Year
	INT32 m_nWday;      ///< Day of week. [0-6]
}SLocalTime;

void  SGSYS_GetLocalTime(UINT32 dwTime,SLocalTime* pstLocalTime);

int GetBackupTime();
bool DeleteFileEx(LPCTSTR lpszPath);

void BackupAndMoveFile();
bool makedirs(const TCHAR* root);

extern SLocalTime gLocalTime;


//服务类型
enum enServiceKind
{
	ServiceKind_None,				//无效服务
	ServiceKind_Game,				//游戏服务
	ServiceKind_Plaza,				//广场服务
};

//绑定参数
struct tagBindParameter
{
	//网络数据
	DWORD							dwSocketID;							//网络标识
	DWORD							dwClientAddr;						//连接地址
	DWORD							dwActiveTime;						//激活时间

	//附加数据
	WORD							wServiceID;							//服务标识
	enServiceKind					ServiceKind;						//服务类型
};

//数组定义
typedef CWHArray<WORD> CWordArrayTemplate;

//////////////////////////////////////////////////////////////////////////////////

//调度钩子
class CAttemperEngineSink : public IAttemperEngineSink
{
	//友元定义
	friend class CServiceUnits;

	//状态变量
protected:
	WORD							m_wCollectItem;						//汇总连接
	CWordArrayTemplate				m_WaitCollectItemArray;				//汇总等待

	//变量定义
protected:
	CInitParameter *				m_pInitParameter;					//配置参数
	tagBindParameter *				m_pBindParameter;					//辅助数组

	//组件变量
protected:
	CGlobalInfoManager				m_GlobalInfoManager;				//全局管理

	//组件接口
protected:
	ITimerEngine *					m_pITimerEngine;					//时间引擎
	ITCPNetworkEngine *				m_pITCPNetworkEngine;				//网络引擎

	//函数定义
public:
	//构造函数
	CAttemperEngineSink();
	//析构函数
	virtual ~CAttemperEngineSink();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { return; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//异步接口
public:
	//启动事件
	virtual bool OnAttemperEngineBegin(IUnknownEx * pIUnknownEx);
	//启动事件
	virtual bool OnAttemperEngineStart(IUnknownEx * pIUnknownEx);
	//停止事件
	virtual bool OnAttemperEngineConclude(IUnknownEx * pIUnknownEx);

	//事件接口
public:
	//控制事件
	virtual bool OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize);
	//自定事件
	virtual bool OnEventAttemperData(WORD wRequestID, VOID * pData, WORD wDataSize);

	//内核事件
public:
	//时间事件
	virtual bool OnEventTimer(DWORD dwTimerID, WPARAM wBindParam);
	//数据库事件
	virtual bool OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	//连接事件
public:
	//连接事件
	virtual bool OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode);
	//关闭事件
	virtual bool OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason);
	//读取事件
	virtual bool OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize);

	//网络事件
public:
	//应答事件
	virtual bool OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID);
	//关闭事件
	virtual bool OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID);
	//读取事件
	virtual bool OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//网络事件
protected:
	//注册服务
	bool OnTCPNetworkMainRegister(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//服务状态
	bool OnTCPNetworkMainServiceInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//用户汇总
	bool OnTCPNetworkMainUserCollect(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//远程服务
	bool OnTCPNetworkMainRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//管理服务
	bool OnTCPNetworkMainManagerService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//辅助函数
protected:
	//发送列表
	bool SendServerListItem(DWORD dwSocketID);
	//比赛列表
	bool SendMatchListItem(DWORD dwSocketID);

	//数据发送
protected:
	//房间发送
	bool SendDataToGame(WORD wServerID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//用户发送
	bool SendDataToUser(WORD wServerID, DWORD dwUserID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

private:
	//定时消息发送
	void StartNextMsgTime(int index);
	//发送消息
	void SendMsg(int index);

};

//////////////////////////////////////////////////////////////////////////////////

#endif