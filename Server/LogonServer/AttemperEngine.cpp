#include "StdAfx.h"
#include "ServiceUnits.h"
#include "ControlPacket.h"
#include "AttemperEngineSink.h"
#include <string>
#include <atlimage.h>
#include <afxinet.h>

using namespace std;
//////////////////////////////////////////////////////////////////////////////////

//时间标识
#define IDI_LOAD_GAME_LIST			1									//加载列表
#define IDI_CONNECT_CORRESPOND		2									//重连标识
#define IDI_COLLECT_ONLINE_INFO		3									//统计在线
#define IDI_BROADCAST_MSG			4									//广播消息
#define IDI_REFRESH_STORE			5									//道具商城
#define IDI_CHECK_VERIFICATION		6									//验证码失效
#define IDI_TIME_RANK				7									//获取排行

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CAttemperEngineSink::CAttemperEngineSink()
{
	//状态变量
	m_bNeekCorrespond=true;
	m_bShowServerStatus=false;

	//状态变量
	m_pInitParameter=NULL;
	m_pBindParameter=NULL;

	//组件变量
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngine=NULL;
	m_pITCPSocketService=NULL;

	//CTraceService::szLogRoot =TEXT("./SLog/LogonServer/");
}

//析构函数
CAttemperEngineSink::~CAttemperEngineSink()
{
}

//接口查询
VOID * CAttemperEngineSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAttemperEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE(IServerUserItemSink, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAttemperEngineSink,Guid,dwQueryVer);	
	return NULL;
}

//启动事件
bool CAttemperEngineSink::OnAttemperEngineStart(IUnknownEx * pIUnknownEx)
{
	//绑定参数
	m_pBindParameter=new tagBindParameter[m_pInitParameter->m_wMaxConnect];
	ZeroMemory(m_pBindParameter,sizeof(tagBindParameter)*m_pInitParameter->m_wMaxConnect);

//   	m_ConnParam.SetHost("127.0.0.1");
//   	m_ConnParam.SetServerPort(6379);
//   	m_ConnParam.SetTimeOut(5);
//   	m_ConnParam.SetPassword("aaqgdwxf");
//   	m_RedisClient.Init(m_ConnParam);
//   	m_RedisClient.ConnectDB();
//   	ENUM_REDIS_CONN_STATUS enumRedisConnStatus = m_RedisClient.GetConnStatus();
//   	if (enumRedisConnStatus != REDIS_CONN_RUNNING)
//   	{
//   		I_TRACE(TEXT("Connect Redis DB Failed!"));
//   	}
//  	if (m_RedisClient.AuthDB() == true)
//  	{
//  		//TRACE_LOG(TEXT("Connect Redis DB Success!"), TraceLevel_Warning);
//  	}
//	TRACE_LOG(TEXT("Connect Redis DB Success!"), TraceLevel_Warning);
// 	m_RedisClient.Publish();

	OnAttemperEngineBegin( pIUnknownEx );
	OnAttemperEngineBegin_BCode(pIUnknownEx);

	return true;
}

//停止事件
bool CAttemperEngineSink::OnAttemperEngineConclude(IUnknownEx * pIUnknownEx)
{
	//状态变量
	m_bNeekCorrespond=true;

	//组件变量
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngine=NULL;
	m_pITCPSocketService=NULL;

	//删除数据
	SafeDeleteArray(m_pBindParameter);

	//列表组件
	m_ServerListManager.ResetServerList();

	m_ServerUserManager.DeleteUserItem();

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

//关闭事件
bool CAttemperEngineSink::OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID)
{
	//清除信息
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	if (pBindParameter==NULL) return false;

	//获取用户
	IServerUserItem * pIServerUserItem=pBindParameter->pIServerUserItem;
	if ( pIServerUserItem != NULL ) pIServerUserItem->SetUserStatus(US_NULL,INVALID_TABLE,INVALID_CHAIR);
	else if ( pIServerUserItem == NULL )
	{
		WORD wIndex=0;
		do
		{
			IServerUserItem * pIUserItem=m_ServerUserManager.EnumUserItem(wIndex++);
			if (pIUserItem==NULL) break;
			if ( pIUserItem->GetBindIndex() == wBindIndex) 
			{
				TCHAR szMsg[128] = {0};
				_stprintf(szMsg,TEXT("玩家%s连接断开，资源没释放"),pIUserItem->GetNickName());
				TRACE_LOG(szMsg,TraceLevel_Warning);

				tagUserInfo *pUserInfo = pIUserItem->GetUserInfo();
				pUserInfo->cbUserStatus = US_NULL;
				m_ServerUserManager.DeleteUserItem(pIUserItem);
// 				DBR_GP_LogonOut LogonOut = { 0 };
// 				LogonOut.dwUserID = pIUserItem->GetUserID();
// 				LogonOut.dwInOutIndex = pIUserItem->GetInoutIndex();
// 
// 				m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOGON_OUT, 0, &LogonOut, sizeof(LogonOut));
				break;
			}

		} while (true);
	}
	else TRACE_LOG(TEXT("玩家连接断开，资源没释放"),TraceLevel_Warning);

	ZeroMemory((m_pBindParameter+wBindIndex),sizeof(tagBindParameter));

	return false;
}


//关闭事件
bool CAttemperEngineSink::OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason)
{
	//协调连接
	if (wServiceID==NETWORK_CORRESPOND)
	{
		//重连判断
		if (m_bNeekCorrespond==true)
		{
			//构造提示
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("与协调服务器的连接关闭了，%ld 秒后将重新连接"),m_pInitParameter->m_wConnectTime);

			//提示消息
			TRACE_LOG(szDescribe,TraceLevel_Warning);

			//设置时间
			ASSERT_ERR(m_pITimerEngine!=NULL);
			m_pITimerEngine->SetTimer(IDI_CONNECT_CORRESPOND,m_pInitParameter->m_wConnectTime*1000L,1,0);

			return true;
		}
	}

	return false;
}

//连接事件
bool CAttemperEngineSink::OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode)
{
	//协调连接
	if (wServiceID==NETWORK_CORRESPOND)
	{
		//错误判断
		if (nErrorCode!=0)
		{
			//构造提示
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("协调服务器连接失败 [ %ld ]，%ld 秒后将重新连接"),
				nErrorCode,m_pInitParameter->m_wConnectTime);

			//提示消息
			TRACE_LOG(szDescribe,TraceLevel_Warning);

			//设置时间
			ASSERT_ERR(m_pITimerEngine!=NULL);
			m_pITimerEngine->SetTimer(IDI_CONNECT_CORRESPOND,m_pInitParameter->m_wConnectTime*1000L,1,0);

			return false;
		}

		//提示消息
		TRACE_LOG(TEXT("正在注册游戏登录服务器..."),TraceLevel_Normal);

		//变量定义
		CMD_CS_C_RegisterPlaza RegisterPlaza;
		ZeroMemory(&RegisterPlaza,sizeof(RegisterPlaza));

		//设置变量
		lstrcpyn(RegisterPlaza.szServerName,m_pInitParameter->m_szServerName,CountArray(RegisterPlaza.szServerName));
		lstrcpyn(RegisterPlaza.szServerAddr,m_pInitParameter->m_ServiceAddress.szAddress,CountArray(RegisterPlaza.szServerAddr));

		//发送数据
		m_pITCPSocketService->SendData(MDM_CS_REGISTER,SUB_CS_C_REGISTER_PLAZA,&RegisterPlaza,sizeof(RegisterPlaza));

		return true;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
