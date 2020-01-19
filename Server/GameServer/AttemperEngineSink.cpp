#include "StdAfx.h"
#include "ServiceUnits.h"
#include "ControlPacket.h"
#include <atlconv.h>
#include "AttemperEngineSink.h"

//////////////////////////////////////////////////////////////////////////////////
//时间标识

#define IDI_LOAD_ANDROID_USER		(IDI_MAIN_MODULE_START+1)			//机器信息
#define IDI_REPORT_SERVER_INFO		(IDI_MAIN_MODULE_START+2)			//房间信息
#define IDI_CONNECT_CORRESPOND		(IDI_MAIN_MODULE_START+3)			//连接时间
#define IDI_GAME_SERVICE_PULSE		(IDI_MAIN_MODULE_START+4)			//服务脉冲
#define IDI_DISTRIBUTE_ANDROID		(IDI_MAIN_MODULE_START+5)			//分配机器
#define IDI_DBCORRESPOND_NOTIFY		(IDI_MAIN_MODULE_START+6)			//缓存通知
#define IDI_LOAD_SYSTEM_MESSAGE		(IDI_MAIN_MODULE_START+7)			//系统消息
#define IDI_SEND_SYSTEM_MESSAGE		(IDI_MAIN_MODULE_START+8)			//系统消息
#define IDI_LOAD_SENSITIVE_WORD		(IDI_MAIN_MODULE_START+9)			//加载敏感词

#define IDI_ANDRIOND_DELAY_JION		(IDI_MAIN_MODULE_START+10)			//机器人延迟加载，使机器人加入时，更加自然
#define IDI_REFRESH_ANDROID			(IDI_MAIN_MODULE_START+11)			//刷新机器人时间戳

#define IDI_REDIS_SCROLL_MESSAGE	(IDI_MAIN_MODULE_START + 13)		// 读取滚动消息
#define IDI_LOAD_SCROLL_MESSAGE		(IDI_MAIN_MODULE_START + 14)		//读取滚动消息
#define IDI_CLEAR_SCROLL_MESSAGE	(IDI_MAIN_MODULE_START + 15)		//清除滚动消息
#define IDI_LOAD_TIP_MESSAGE		(IDI_MAIN_MODULE_START + 16)		//弹窗消息

#define IDI_USER_EXCEPTION_DETECT	(IDI_MAIN_MODULE_START + 17)		//玩家异常检测
#define IDI_USER_HEART_DETECT	(IDI_MAIN_MODULE_START + 18)		//玩家心跳检测

#define IDI_DISTRIBUTE_USER		    (IDI_MAIN_MODULE_START+19)			//分配用户

#define IDI_USER_ONLINE_DETECT		(IDI_MAIN_MODULE_START + 20)		//玩家心跳检测

//////////////////////////////////////////////////////////////////////////////////shut
//时间定义 秒

#define TIME_LOAD_ANDROID_USER		600L								//加载机器
#define TIME_DISTRIBUTE_ANDROID		8L									//分配用户
#define TIME_REPORT_SERVER_INFO		30L									//上报时间
#define TIME_DBCORRESPOND_NOTIFY	3L									//缓存通知时间
#define TIME_LOAD_SYSTEM_MESSAGE	3600L								//系统消息时间
#define TIME_SEND_SYSTEM_MESSAGE	10L								    //系统消息时间
#define TIME_LOAD_SENSITIVE_WORD	5L									//加载敏感词时间
#define TIME_REFRESH_ANDROID		60L									//机器人刷新时间戳
#define	TIME_TIMING_AWARD			1800L								//30分钟定时颁奖时间
#define	TIME_ANDROID_IS_ALLOWED_SIT_DOWN			45L								//45秒读取一次机器人是否坐下配置
#define TIME_LOAD_SCROLL_MESSAGE		25L								//读取滚动消息
#define TIME_CLEAR_SCROLL_MESSAGE		300L							//清除滚动消息
#define TIME_LOAD_TIP_MESSAGE		28L									//弹窗消息
#define TIME_USER_EXCEPTION_DETECT	5L									//异常检测时间
#define TIME_USER_HEART_DETECT		3L									//心跳检测时间
#define TIME_USER_ONLINE_DETECT		10L									//在线玩家查询

#define TIME_REDIS_SCROLL_MES		1L									// Redis滚动消息
#define KIND_FISH					2016
#define KIND_ZJH					6

TCHAR							g_szLogRoot[MAX_PATH];

//////////////////////////////////////////////////////////////////////////
namespace Utils {
	std::string ConvertFromUtf16ToUtf8(const std::wstring& wstr)
	{
		std::string convertedString;
		int requiredSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, 0, 0, 0, 0);
		if (requiredSize > 0)
		{
			std::vector<char> buffer(requiredSize);
			WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &buffer[0], requiredSize, 0, 0);
			convertedString.assign(buffer.begin(), buffer.end() - 1);
		}
		return convertedString;
	}

	std::wstring ConvertFromUtf8ToUtf16(const std::string& str)
	{
		std::wstring convertedString;
		int requiredSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, 0, 0);
		if (requiredSize > 0)
		{
			std::vector<wchar_t> buffer(requiredSize);
			MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &buffer[0], requiredSize);
			convertedString.assign(buffer.begin(), buffer.end() - 1);
		}

		return convertedString;
	}

	//宽字节转窄字节
	std::string ConvertW2A(const std::wstring &wstr)
	{
		setlocale(LC_ALL, ".936");
		size_t nSize = wstr.length() * 2 + 1;
		char *psz = new char[nSize];

		memset(psz, 0, nSize);
		wcstombs(psz, wstr.c_str(), nSize);
		std::string str = psz;
		delete[]psz;

		return str;
	}

	//窄字节转宽字节
	std::wstring ConvertA2W(const string &str)
	{
		setlocale(LC_ALL, ".936");
		size_t nSize = str.length() + 1;
		wchar_t *wpsz = new wchar_t[nSize];

		memset(wpsz, 0, sizeof(wchar_t)*nSize);
		mbstowcs(wpsz, str.c_str(), nSize);
		std::wstring wstr = wpsz;
		delete[]wpsz;

		return wstr;
	}
}
//////////////////////////////////////////////////////////////////////////////////
//构造函数
CAttemperEngineSink::CAttemperEngineSink()
{
	//状态变量
	m_bCollectUser=false;
	m_bNeekCorrespond=true;

	//绑定数据
	m_pNormalParameter=NULL;
	m_pAndroidParameter=NULL;

	//状态变量
	m_pInitParameter=NULL;
	m_pGameParameter=NULL;
	m_pGameServiceAttrib=NULL;
	m_pGameServiceOption=NULL;

	//组件变量
	m_pITimerEngine=NULL;
	m_pIAttemperEngine=NULL;
	m_pITCPSocketService=NULL;
	m_pITCPNetworkEngine=NULL;
	m_pIGameServiceManager=NULL;

	//数据引擎
	m_pIRecordDataBaseEngine=NULL;
	m_pIKernelDataBaseEngine=NULL;
	m_pIDBCorrespondManager=NULL;

	//配置数据
	ZeroMemory(&m_DataConfigColumn,sizeof(m_DataConfigColumn));
	ZeroMemory(&m_DataConfigProperty,sizeof(m_DataConfigProperty));

	//比赛变量
	m_pIGameMatchServiceManager=NULL;

}

//析构函数
CAttemperEngineSink::~CAttemperEngineSink()
{
	//删除数据
	SafeDeleteArray(m_pNormalParameter);
	SafeDeleteArray(m_pAndroidParameter);

	//删除桌子
	for (INT_PTR i=0;i<m_TableFrameArray.GetCount();i++)
	{
		SafeRelease(m_TableFrameArray[i]);
	}

	//清理数据
	m_KickUserItemMap.RemoveAll();
	m_DistributeManage.RemoveAll();

	//ClearSystemMessageData();

	return;
}

//接口查询
VOID * CAttemperEngineSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IMainServiceFrame,Guid,dwQueryVer);
	QUERYINTERFACE(IAttemperEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE(IServerUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAttemperEngineSink,Guid,dwQueryVer);
	return NULL;
}

//启动事件
bool CAttemperEngineSink::OnAttemperEngineStart(IUnknownEx * pIUnknownEx)
{
	//绑定信息
	m_pAndroidParameter=new tagBindParameter[MAX_ANDROID];
	ZeroMemory(m_pAndroidParameter,sizeof(tagBindParameter)*MAX_ANDROID);

	//绑定信息
	m_pNormalParameter=new tagBindParameter[m_pGameServiceOption->wMaxPlayer];
	ZeroMemory(m_pNormalParameter,sizeof(tagBindParameter)*m_pGameServiceOption->wMaxPlayer);

	//创建房间guid
	GUID guid;
	CoCreateGuid(&guid);
	StringFromGUID2(guid, m_guidRoom, 40);

	//if( time(NULL)>= 1483200000 )
	//	return true;

	CString strguid(m_guidRoom);

	strguid.Remove(L'{');
	strguid.Remove(L'}');
	ZeroMemory(m_guidRoom, 40 * sizeof WCHAR);
	CopyMemory(m_guidRoom, strguid.GetBuffer(), strguid.GetLength() * sizeof WCHAR);

	//分配规则
	m_DistributeManage.SetDistributeRule(m_pGameServiceOption->cbDistributeRule);

	//配置机器
	if (InitAndroidUser()==false)
	{
		ASSERT_ERR(FALSE);
		return false;
	}

	//配置桌子
	if (InitTableFrameArray()==false)
	{
		ASSERT_ERR(FALSE);
		return false;
	}

	//设置接口
	if (m_ServerUserManager.SetServerUserItemSink(QUERY_ME_INTERFACE(IServerUserItemSink))==false)
	{
		ASSERT_ERR(FALSE);
		return false;
	}

	//启动机器
	if (m_AndroidUserManager.StartService()==false)
	{
		ASSERT_ERR(FALSE);
		return false;
	}

	if ( m_pGameServiceOption->wServerType != GAME_GENRE_MATCH )
	{
		//刷新机器人时间戳定时器
		m_pITimerEngine->SetTimer(IDI_REFRESH_ANDROID, TIME_REFRESH_ANDROID * 1000L, TIMES_INFINITY, NULL);
		m_pITimerEngine->SetTimer(IDI_DISTRIBUTE_ANDROID,TIME_DISTRIBUTE_ANDROID*1000L,TIMES_INFINITY,NULL);

		//m_pITimerEngine->SetTimer(IDI_LOAD_ANDROID_USER,2*1000L,TIMES_INFINITY,NULL);
	}

	m_pITimerEngine->SetTimer(IDI_DBCORRESPOND_NOTIFY,TIME_DBCORRESPOND_NOTIFY*1000L,TIMES_INFINITY,NULL);

	//m_pITimerEngine->SetTimer(IDI_LOAD_SYSTEM_MESSAGE,80*1000L,TIMES_INFINITY,NULL);
	//m_pITimerEngine->SetTimer(IDI_SEND_SYSTEM_MESSAGE,15*1000L,TIMES_INFINITY,NULL);

	//延时加载敏感词
	//m_pITimerEngine->SetTimer(IDI_LOAD_SENSITIVE_WORD,TIME_LOAD_SENSITIVE_WORD*1000L,TIMES_INFINITY,NULL);

	//防作弊模式
// 	if ((m_pGameServiceOption->cbDistributeRule&DISTRIBUTE_ALLOW) != 0)
// 	{
// 		m_pITimerEngine->SetTimer(IDI_DISTRIBUTE_USER, m_pGameServiceOption->wDistributeTimeSpace * 1000, TIMES_INFINITY, NULL);
// 	}
	m_pITimerEngine->SetTimer(IDI_DISTRIBUTE_USER, 3 * 1000, TIMES_INFINITY, NULL);


	OnAttemperEngineBegin( pIUnknownEx );
	OnAttemperEngineBegin_BCode( pIUnknownEx );

	return true;
}

//停止事件
bool CAttemperEngineSink::OnAttemperEngineConclude(IUnknownEx * pIUnknownEx)
{
	//删除桌子
	for (INT_PTR i = 0; i < m_TableFrameArray.GetCount(); i++)
	{
		SafeRelease(m_TableFrameArray[i]);
	}

	//清理数据
	m_KickUserItemMap.RemoveAll();
	m_DistributeManage.RemoveAll();

	//状态变量
	m_bCollectUser=false;
	m_bNeekCorrespond=true;

	//配置信息
	m_pInitParameter=NULL;
	m_pGameServiceAttrib=NULL;
	m_pGameServiceOption=NULL;

	//组件变量
	m_pITimerEngine=NULL;
	m_pITCPSocketService=NULL;
	m_pITCPNetworkEngine=NULL;

	//数据引擎
	m_pIRecordDataBaseEngine=NULL;
	m_pIKernelDataBaseEngine=NULL;

	//绑定数据
	SafeDeleteArray(m_pNormalParameter);
	SafeDeleteArray(m_pAndroidParameter);

	//删除用户
	m_TableFrameArray.RemoveAll();
	m_ServerUserManager.DeleteUserItem();
	m_ServerListManager.ResetServerList();
	m_DistributeManage.RemoveAll();

	//停止服务
	m_AndroidUserManager.ConcludeService();

	//停止比赛
	if(m_pIGameMatchServiceManager!=NULL)
	{
		m_pIGameMatchServiceManager->StopService();
	}

	//清除消息数据
	ClearSystemMessageData();

	//清除机器人
	PostReleaseAllAndroid();

	//复位关键字
	m_WordsFilter.ResetSensitiveWordArray();

	return true;
}



//启动事件
bool CAttemperEngineSink::OnAttemperEngineBegin(IUnknownEx * pIUnknownEx)
{
	// 主命令注册
	{
		REGISTER_FUNCTION_4(m_mMainFunctions, MDM_GR_LOGON, CAttemperEngineSink::OnTCPNetworkMainLogon, this);	// 登录命令
		REGISTER_FUNCTION_4(m_mMainFunctions, MDM_GR_USER, CAttemperEngineSink::OnTCPNetworkMainUser, this);	// 用户命令
		REGISTER_FUNCTION_4(m_mMainFunctions, MDM_GF_GAME, CAttemperEngineSink::OnTCPNetworkMainGame, this);	// 游戏命令
		REGISTER_FUNCTION_4(m_mMainFunctions, MDM_GF_FRAME, CAttemperEngineSink::OnTCPNetworkMainFrame, this);	// 框架命令
		REGISTER_FUNCTION_4(m_mMainFunctions, MDM_GR_INSURE, CAttemperEngineSink::OnTCPNetworkMainInsure, this);	// 银行命令
		REGISTER_FUNCTION_4(m_mMainFunctions, MDM_GR_MANAGE, CAttemperEngineSink::OnTCPNetworkMainManage, this);	// 管理命令
		REGISTER_FUNCTION_4(m_mMainFunctions, MDM_GR_MATCH, CAttemperEngineSink::OnTCPNetworkMainMatch, this);	// 比赛命令
		REGISTER_FUNCTION_4(m_mMainFunctions, MDM_GR_HEART, CAttemperEngineSink::OnTCPNetworkMainHeart, this);	// 心跳命令
	}

	//数据库事件注册
	{
		REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GR_LOGON_SUCCESS, CAttemperEngineSink::OnDBLogonSuccess, this);	// 登录成功
		REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GR_LOGON_SUCCESS_H5, CAttemperEngineSink::OnDBLogonSuccessH5, this);	// 登录成功
		REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GR_LOGON_FAILURE, CAttemperEngineSink::OnDBLogonFailure, this);	// 登录失败
		REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GR_GAME_PARAMETER, CAttemperEngineSink::OnDBGameParameter, this);	// 游戏参数
		REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GR_GAME_COLUMN_INFO, CAttemperEngineSink::OnDBGameColumnInfo, this);	// 列表信息
		REGISTER_FUNCTION_3(m_mDbFunctions, DBR_GR_GAME_ANDROID_INFO, CAttemperEngineSink::OnDBGameAndroidInfo, this);	// 机器信息
		REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GR_GAME_PROPERTY_INFO, CAttemperEngineSink::OnDBGamePropertyInfo, this);	// 道具信息
		REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GR_USER_INSURE_INFO, CAttemperEngineSink::OnDBUserInsureInfo, this);	// 银行信息
		REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GR_USER_INSURE_SUCCESS, CAttemperEngineSink::OnDBUserInsureSuccess, this);	// 银行成功
		REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GR_USER_INSURE_FAILURE, CAttemperEngineSink::OnDBUserInsureFailure, this);	// 银行失败
		REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GR_USER_INSURE_USER_INFO, CAttemperEngineSink::OnDBUserInsureUserInfo, this);	// 用户信息
		REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GR_SYSTEM_MESSAGE_RESULT, CAttemperEngineSink::OnDBSystemMessage, this);	// 系统消息
		REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GR_SENSITIVE_WORDS, CAttemperEngineSink::OnDBSensitiveWords, this);	// 加载敏感词
		REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GR_ANDRIOD_RELEASE_DELTE, CAttemperEngineSink::OnDBAndroidReleaseAndDelete, this);	// 释放机器人
		REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GR_ANDRIOD_APPLY, CAttemperEngineSink::OnDBAndroidApply, this);	// 申请机器人
		REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_UPDATE_ATTRIBUTE, CAttemperEngineSink::OnDBUpdateAttribute, this);	// 刷新通用属性

	}

	//设置日志路径
	ZeroMemory(m_szPath,CountArray(m_szPath));
	CWHService::GetWorkDirectory(m_szPath,CountArray(m_szPath));
	_sntprintf(g_szLogRoot,sizeof(g_szLogRoot),TEXT("%s/SLog/GameServer/%s"),m_szPath,m_pGameServiceOption->szServerName);
	
	//日志设定
	m_fileLog.SetRoot(TEXT("./SLog/GameServer/%s"), m_pGameServiceOption->szServerName);
	m_UserFileLog.SetRoot(TEXT("./SLog/UserAction/%s__User"), m_pGameServiceOption->szServerName);
	m_ServerFileLog.SetRoot(TEXT("./SLog/UserAction/%s____Server"), m_pGameServiceOption->szServerName);

	
	return true;
}

//控制事件
bool CAttemperEngineSink::OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize)
{
	switch (wIdentifier)
	{
	case CT_CONNECT_CORRESPOND:		//连接协调
		{
			//发起连接
			tagAddressInfo * pCorrespondAddress=&m_pInitParameter->m_CorrespondAddress;
			m_pITCPSocketService->Connect(pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//构造提示
			TCHAR szString[512]=TEXT("");
			_sntprintf(szString,CountArray(szString),TEXT("正在连接协调服务器 [ %s:%d ]"),pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//提示消息
			TRACE_LOG(szString,TraceLevel_Normal);

			return true;
		}
	case CT_LOAD_SERVICE_CONFIG:	//加载配置
		{
			ZeroMemory(m_szPath,CountArray(m_szPath));
			CWHService::GetWorkDirectory(m_szPath,CountArray(m_szPath));

			//加载配置
			m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_PARAMETER,0L,NULL,0L);
			
			//加载列表
			m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_GAME_COLUMN,0L,NULL,0L);

			//加载机器
			if (m_pGameServiceOption->wServerType != GAME_GENRE_MATCH)
			{
				DBR_GR_GameAndroidApply dbrAndriond = {0};
				dbrAndriond.dwCount = m_pGameServiceOption->wAndroidCount;
				CopyMemory(dbrAndriond.guidRoom, m_guidRoom, 40 * sizeof WCHAR);
				dbrAndriond.dwRoomID = m_pGameServiceOption->wServerID;
				m_pIDBCorrespondManager->PostDataBaseRequest(0l, DBR_GR_ANDRIOD_APPLY, 0l, (void*)&dbrAndriond, sizeof(DBR_GR_GameAndroidApply));
			}

			//加载道具
			m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_GAME_PROPERTY,0L,NULL,0L);

			//加载消息
			//if ( m_pGameServiceOption->wServerType != GAME_GENRE_MATCH )
				//m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_SYSTEM_MESSAGE,0L,NULL,0L);


			//弹窗消息
			m_pITimerEngine->SetTimer(IDI_LOAD_TIP_MESSAGE,TIME_LOAD_TIP_MESSAGE * 1000L,TIMES_INFINITY,0);
			// Redis 滚动消息
			m_pITimerEngine->SetTimer(IDI_REDIS_SCROLL_MESSAGE, TIME_REDIS_SCROLL_MES * 1000L, TIMES_INFINITY, 0);
			

			//滚动消息
			m_pITimerEngine->SetTimer(IDI_LOAD_SCROLL_MESSAGE,TIME_LOAD_SCROLL_MESSAGE * 1000L,TIMES_INFINITY,0);

			//清除滚动消息
			m_pITimerEngine->SetTimer(IDI_CLEAR_SCROLL_MESSAGE,TIME_CLEAR_SCROLL_MESSAGE * 1000L,TIMES_INFINITY,0);

			//启动异常玩家检测
			m_pITimerEngine->SetTimer(IDI_USER_EXCEPTION_DETECT,TIME_USER_EXCEPTION_DETECT * 1000L,TIMES_INFINITY,0);

			//启动玩家心跳检测
			//m_pITimerEngine->SetTimer(IDI_USER_HEART_DETECT,TIME_USER_HEART_DETECT * 1000,TIMES_INFINITY,0);

			//启动在线玩家查询
			m_pITimerEngine->SetTimer(IDI_USER_ONLINE_DETECT, TIME_USER_ONLINE_DETECT * 1000, TIMES_INFINITY, 0);

			return true;
		}
	}

	return false;
}

//调度事件
bool CAttemperEngineSink::OnEventAttemperData(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	switch (wRequestID)
	{
	case SE_KICK_ALL_USER_REQUEST:	//踢出所有用户
		{
			//D_TRACE(TEXT("开始踢出所有用户"));

			if (m_pGameServiceOption != NULL && m_pNormalParameter != NULL)
			{
				//消息处理
				CMD_GR_KickAllUser * pKillAllUser=(CMD_GR_KickAllUser *)pData;

				//效验数据
				ASSERT(wDataSize<=sizeof(CMD_GR_KickAllUser));
				if( wDataSize > sizeof(CMD_GR_KickAllUser) ) return true;
				ASSERT(wDataSize==CountStringBuffer(pKillAllUser->szKickMessage));
				if (wDataSize!=CountStringBuffer(pKillAllUser->szKickMessage)) return true;

				TCHAR szMsg[128]={0};
				_sntprintf(szMsg, 128, TEXT("[%s]临时维护，请稍后再进入游戏！"), m_pGameServiceAttrib == NULL ? TEXT("此游戏") : m_pGameServiceAttrib->szGameName);

				//解散所有游戏
				for (INT_PTR i=0;i<m_TableFrameArray.GetCount();i++)
				{
					//获取桌子
					CTableFrame * pTableFrame=m_TableFrameArray[i];
					if ( !pTableFrame->IsGameStarted() ) continue;

					//D_TRACE(TEXT("解散[%s]"), pTableFrame->GetGameServiceAttrib()->szGameName);

					pTableFrame->DismissGame();
				}

				tagBindParameter *pBindParameter = m_pNormalParameter;
				for( INT i = 0; i < m_pGameServiceOption->wMaxPlayer; i++ )
				{
					//目录用户
					IServerUserItem * pITargerUserItem= pBindParameter->pIServerUserItem;
					if (pITargerUserItem==NULL) 
					{
						pBindParameter++;
						continue;
					}

					//D_TRACE(TEXT("踢出[%s]"), pITargerUserItem->GetNickName());


					//发送消息
					SendRoomMessage(pITargerUserItem,szMsg,SMT_CHAT|SMT_EJECT|SMT_CLOSE_LINK|SMT_CLOSE_ROOM);

					pBindParameter++;

					pITargerUserItem->SetUserStatus(US_NULL, pITargerUserItem->GetTableID(), pITargerUserItem->GetChairID());
				} 

				//D_TRACE(TEXT("踢除玩家完成"));
			}

			return true;
		}
	default:
		break;
	}
	return false;
}

//时间事件
bool CAttemperEngineSink::OnEventTimer(DWORD dwTimerID, WPARAM wBindParam)
{
	//调度时间
	if ((dwTimerID>=IDI_MAIN_MODULE_START)&&(dwTimerID<=IDI_MAIN_MODULE_FINISH))
	{
		//时间处理
		switch (dwTimerID)
		{
		case IDI_REFRESH_ANDROID:		//刷新机器人时间戳
			{
				do 
				{
					if ( m_pGameServiceOption->wServerType == GAME_GENRE_MATCH ) break;

					WORD wCount = m_AndroidUserManager.GetAndroidCount();
					if ( wCount >= m_pGameServiceOption->wAndroidCount ) break;

					DBR_GR_GameAndroidApply dbrAndriond = {0};
					dbrAndriond.dwCount = m_pGameServiceOption->wAndroidCount - m_AndroidUserManager.GetAndroidCount();
					CopyMemory(dbrAndriond.guidRoom, m_guidRoom, 40 * sizeof WCHAR);
					dbrAndriond.dwRoomID = m_pGameServiceOption->wServerID;
					m_pIDBCorrespondManager->PostDataBaseRequest(0l, DBR_GR_ANDRIOD_APPLY, 0l, (void*)&dbrAndriond, sizeof(DBR_GR_GameAndroidApply));

				} while (0);

				return true;
			}
		case IDI_DISTRIBUTE_USER: //分配用户
		{
			//执行分组
			PerformDistribute();

			return true;
		}
		case IDI_USER_EXCEPTION_DETECT:		//玩家异常检测
			{
				WORD wIndex = 0;
				do 
				{
					IServerUserItem * pIServerUserItem = m_ServerUserManager.EnumUserItem(wIndex++);
					if (pIServerUserItem == NULL) break;

					WORD wTableID = pIServerUserItem->GetTableID();
					WORD wChairID = pIServerUserItem->GetChairID();
					BYTE cbStatus = pIServerUserItem->GetUserStatus();
					BYTE cbClientKind = pIServerUserItem->GetUserInfo()->cbClientKind;
					if ( cbStatus == US_SIT || cbStatus == US_READY )
					{
						CTableFrame * pTableFrame = m_TableFrameArray[wTableID];
						if (pTableFrame == NULL) continue;
						if (pTableFrame->IsGameStarted()) continue;
						if ( pIServerUserItem->IsClientReady() == true ) continue;
						if ( pIServerUserItem->IsAndroidUser() ==false ) continue;

						I_TRACE(TEXT("自动开始 %s %d-%d 状态[%d],机器[%d]"),pIServerUserItem->GetNickName(),
							pIServerUserItem->GetTableID(),pIServerUserItem->GetChairID(),pIServerUserItem->GetUserStatus(),pIServerUserItem->IsAndroidUser());
						if (cbClientKind == CLIENT_KIND_WEBSOCKET)
						pTableFrame->OnEventSocketFrame(SUB_GF_GAME_OPTION_H5, NULL, 0, pIServerUserItem);
						else pTableFrame->OnEventSocketFrame(SUB_GF_GAME_OPTION, NULL, 0, pIServerUserItem);
					}

					

				} while (true);
				
				return true;
			}
		case IDI_USER_ONLINE_DETECT:
		{
			//return true;
			WORD wIndex = 0;
			BYTE cbUserCount = 0;
			CString cs;
			do 
			{			
				IServerUserItem * pIServerUserItem = m_ServerUserManager.EnumUserItem(wIndex++);
				if (pIServerUserItem == NULL) break;
				if ( pIServerUserItem->IsAndroidUser() ) continue;
				cs.Format(TEXT("在线 %s 桌椅[%d-%d],状态[%d],机器[%d]"), pIServerUserItem->GetNickName(), pIServerUserItem->GetTableID(), pIServerUserItem->GetChairID(), pIServerUserItem->GetUserStatus(), pIServerUserItem->IsAndroidUser());
				//I_TRACE(cs.GetBuffer(0));				
				m_fileLog.Log(cs.GetBuffer(0));
				
				DetectKickUser(pIServerUserItem,TRUE);
				cbUserCount++;
			} while (true);
			if (cbUserCount>0)
				m_fileLog.Log(TEXT("----------------------------------------"));
			

			return true;
		}
		case IDI_USER_HEART_DETECT:
			{
				return true;
			}
		case IDI_REPORT_SERVER_INFO:	//房间信息
			{
				//变量定义
				CMD_CS_C_ServerOnLine ServerOnLine;
				ZeroMemory(&ServerOnLine,sizeof(ServerOnLine));

				//设置变量
				ServerOnLine.dwOnLineCount=m_ServerUserManager.GetUserItemCount();

				//发送数据
				m_pITCPSocketService->SendData(MDM_CS_SERVICE_INFO,SUB_CS_C_SERVER_ONLINE,&ServerOnLine,sizeof(ServerOnLine));

				return true;
			}
		case IDI_CONNECT_CORRESPOND:	//连接协调
			{
				//发起连接
				tagAddressInfo * pCorrespondAddress=&m_pInitParameter->m_CorrespondAddress;
				m_pITCPSocketService->Connect(pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

				//构造提示
				TCHAR szString[512]=TEXT("");
				_sntprintf(szString,CountArray(szString),TEXT("正在连接协调服务器 [ %s:%d ]"),pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

				//提示消息
				//TRACE_LOG(szString,TraceLevel_Normal);
				I_TRACE(szString);

				return true;
			}
		case IDI_GAME_SERVICE_PULSE:	//服务维护
			{
				return true;
			}
		case IDI_DISTRIBUTE_ANDROID:	//分配机器
			{
				//动作处理
				if (m_AndroidUserManager.GetAndroidCount()>0 && m_pGameServiceOption->wServerType!=GAME_GENRE_MATCH)
				{
					//变量定义
					bool bAllowDynamicJoin=CServerRule::IsAllowDynamicJoin(m_pGameServiceOption->dwServerRule);			//动态加入
					bool bAllowAndroidAttend=CServerRule::IsAllowAndroidAttend(m_pGameServiceOption->dwServerRule);		//允许陪玩
					bool bAllowAndroidSimulate=CServerRule::IsAllowAndroidSimulate(m_pGameServiceOption->dwServerRule);	//允许占位
					bool bAllowAvertCheatMode=(CServerRule::IsAllowAvertCheatMode(m_pGameServiceOption->dwServerRule)&&(m_pGameServiceAttrib->wChairCount < MAX_CHAIR));

					//动态配置机器人行为
					//CString szFileName;
					//szFileName.Format(TEXT("%s/Settings/Android/%d_AndroidOption.ini"),m_szPath,m_pGameServiceOption->dwRoomID);

					//WORD wNotSitDown	= GetPrivateProfileInt(TEXT("Parameter"),TEXT("NotSitDown"),0,(LPCTSTR)szFileName);
					//if ( wNotSitDown != 0 ) return true;

					//模拟处理
					//允许占位且不是防作弊模式
					if (bAllowAndroidSimulate==true && bAllowAvertCheatMode==false)
					{
						//机器状态
						tagAndroidUserInfo AndroidSimulate; 
						m_AndroidUserManager.GetAndroidUserInfo(AndroidSimulate,ANDROID_SIMULATE);

						//机器处理
						if (AndroidSimulate.wFreeUserCount>0)
						{
							for (WORD i=0;i<8;i++)
							{
								//随机桌子
								WORD wTableID=rand()%(__max(m_pGameServiceOption->wTableCount/3,1));

								//获取桌子
								CTableFrame * pTableFrame=m_TableFrameArray[wTableID];
								if ((pTableFrame->IsGameStarted()==true)&&(bAllowDynamicJoin==false)) continue;

								//桌子状况
								tagTableUserInfo TableUserInfo;
								WORD wUserSitCount=pTableFrame->GetTableUserInfo(TableUserInfo);

								//分配判断
								if (TableUserInfo.wTableUserCount>0) continue;
								if ((wUserSitCount>=TableUserInfo.wMinUserCount)&&(m_pGameServiceAttrib->wChairCount<MAX_CHAIR)) continue;

								//坐下判断
								if (AndroidSimulate.wFreeUserCount>=TableUserInfo.wMinUserCount)
								{
									//变量定义
									WORD wHandleCount=0;
									WORD wWantAndroidCount=TableUserInfo.wMinUserCount;

									//数据调整
									if (m_pGameServiceAttrib->wChairCount>TableUserInfo.wMinUserCount)
									{
										WORD wChairCount=m_pGameServiceAttrib->wChairCount;
										WORD wFreeUserCount=AndroidSimulate.wFreeUserCount;
										WORD wOffUserCount=__min(wChairCount,wFreeUserCount)-TableUserInfo.wMinUserCount;
										wWantAndroidCount+=((wOffUserCount > 0) ? (rand()%(wOffUserCount+1)) : 0);
									}

									//坐下处理
									for (WORD j=0;j<AndroidSimulate.wFreeUserCount;j++)
									{
										//变量定义
										WORD wChairID=pTableFrame->GetRandNullChairID();

										//无效过滤
										//ASSERT_ERR(wChairID!=INVALID_CHAIR);
										if (wChairID==INVALID_CHAIR) continue;

										//用户坐下
										IAndroidUserItem * pIAndroidUserItem=AndroidSimulate.pIAndroidUserFree[j];
										if (pTableFrame->PerformSitDownAction(wChairID,pIAndroidUserItem->GetMeUserItem())==true)
										{
											//设置变量
											wHandleCount++;

											//完成判断
											if (wHandleCount>=wWantAndroidCount) 
											{
												return true;
											}
										}
									}

									if(wHandleCount > 0) return true;
								}
							}
						}
					}

					//陪打处理
					if (bAllowAndroidAttend==true)
					{
						//被动状态
						tagAndroidUserInfo AndroidPassivity;
						m_AndroidUserManager.GetAndroidUserInfo(AndroidPassivity,ANDROID_PASSIVITY);

						if(bAllowAvertCheatMode) //防作弊模式陪打
						{
							//坐下处理
							for (WORD j=0;j<AndroidPassivity.wFreeUserCount;j++)
							{
								IAndroidUserItem * pIAndroidUserItem=AndroidPassivity.pIAndroidUserFree[j];
								if (pIAndroidUserItem->GetMeUserItem()==NULL) continue;
								if (InsertDistribute(pIAndroidUserItem->GetMeUserItem())==true) return true;
							}
						}
						else
						{
							//被动处理
							if (AndroidPassivity.wFreeUserCount>0)
							{
								//百人游戏
								if(m_pGameServiceAttrib->wChairCount >= MAX_CHAIR)
								{
									for (INT_PTR i=0;i<(m_pGameServiceOption->wTableCount);i++)
									{
										//获取桌子
										CTableFrame * pTableFrame=m_TableFrameArray[i];
										if ((pTableFrame->IsGameStarted()==true)&&(bAllowDynamicJoin==false)) continue;

										//桌子状况
										tagTableUserInfo TableUserInfo;
										WORD wUserSitCount=pTableFrame->GetTableUserInfo(TableUserInfo);

										//分配判断
										if (wUserSitCount>m_pGameServiceAttrib->wChairCount*2/3) continue;

										//变量定义
										IServerUserItem * pIServerUserItem=NULL;
										WORD wChairID=pTableFrame->GetRandNullChairID();

										//无效过滤
										ASSERT_ERR(wChairID!=INVALID_CHAIR);
										if (wChairID==INVALID_CHAIR) continue;

										//坐下处理
										for (WORD j=0;j<AndroidPassivity.wFreeUserCount;j++)
										{
											IAndroidUserItem * pIAndroidUserItem=AndroidPassivity.pIAndroidUserFree[j];
											if (pTableFrame->PerformSitDownAction(wChairID,pIAndroidUserItem->GetMeUserItem())==true) return true;
										}
									}
								}
								else
								{
									for (INT_PTR i=0;i<m_pGameServiceOption->wTableCount;i++)
									{
										//获取桌子
										CTableFrame * pTableFrame=m_TableFrameArray[i];
										if ((pTableFrame->IsGameStarted()==true)&&(bAllowDynamicJoin==false)) continue;

										//桌子状况
										tagTableUserInfo TableUserInfo;
										WORD wUserSitCount=pTableFrame->GetTableUserInfo(TableUserInfo);

										//分配判断
										if (wUserSitCount==0) continue;
										if (TableUserInfo.wTableUserCount==0) continue;
										if ((wUserSitCount>=TableUserInfo.wMinUserCount)&&(rand()%10>5)) continue;

										//变量定义
										IServerUserItem * pIServerUserItem=NULL;
										WORD wChairID=pTableFrame->GetRandNullChairID();

										//无效过滤
										ASSERT_ERR(wChairID!=INVALID_CHAIR);
										if (wChairID==INVALID_CHAIR) continue;

										//坐下处理
										for (WORD j=0;j<AndroidPassivity.wFreeUserCount;j++)
										{
											IAndroidUserItem * pIAndroidUserItem=AndroidPassivity.pIAndroidUserFree[j];
											if (pTableFrame->PerformSitDownAction(wChairID,pIAndroidUserItem->GetMeUserItem())==true) return true;
										}
									}
								}
							}
						}
					}

					//陪打处理
					if (bAllowAndroidAttend==true)
					{
						//主动状态
						tagAndroidUserInfo AndroidInitiative;
						m_AndroidUserManager.GetAndroidUserInfo(AndroidInitiative,ANDROID_INITIATIVE);
						WORD wAllAndroidCount = AndroidInitiative.wFreeUserCount+AndroidInitiative.wPlayUserCount+AndroidInitiative.wSitdownUserCount;

						if(bAllowAvertCheatMode)
						{
							//坐下处理
							for (WORD j=0;j<AndroidInitiative.wFreeUserCount;j++)
							{
								IAndroidUserItem * pIAndroidUserItem=AndroidInitiative.pIAndroidUserFree[j];
								if (pIAndroidUserItem->GetMeUserItem()==NULL) continue;
								//I_TRACE(TEXT("机器人插入分组"));
								if (InsertDistribute(pIAndroidUserItem->GetMeUserItem())==true) return true;
							}
						}
						else
						{
							//主动处理
							if (AndroidInitiative.wFreeUserCount>0)
							{
								for (INT_PTR i=0;i<m_pGameServiceOption->wTableCount;i++)
								{
									//获取桌子
									CTableFrame * pTableFrame=m_TableFrameArray[i];
									if ((pTableFrame->IsGameStarted()==true)&&(bAllowDynamicJoin==false)) continue;

									//桌子状况
									tagTableUserInfo TableUserInfo;
									WORD wUserSitCount=pTableFrame->GetTableUserInfo(TableUserInfo);

									//分配判断
									if((m_pGameServiceAttrib->wChairCount<MAX_CHAIR) && wUserSitCount>(TableUserInfo.wMinUserCount-1)) continue;

									//变量定义
									IServerUserItem * pIServerUserItem=NULL;
									WORD wChairID=pTableFrame->GetRandNullChairID();

									//无效过滤
									ASSERT_ERR(wChairID!=INVALID_CHAIR);
									if (wChairID==INVALID_CHAIR) continue;

									//坐下处理
									for (WORD j=0;j<AndroidInitiative.wFreeUserCount;j++)
									{
										IAndroidUserItem * pIAndroidUserItem=AndroidInitiative.pIAndroidUserFree[j];
										if (pTableFrame->PerformSitDownAction(wChairID,pIAndroidUserItem->GetMeUserItem())==true) return true;
									}
								}
							}
						}
					}

					if ( bAllowAvertCheatMode == false )
					{
						//起立处理
						WORD wStandUpCount=0;
						WORD wRandCount=((rand()%3)+1);
						INT_PTR nIndex = rand()%(__max(m_pGameServiceOption->wTableCount,1));
						for (INT_PTR i=nIndex;i<m_pGameServiceOption->wTableCount+nIndex;++i)
						{
							//获取桌子
							INT_PTR nTableIndex=i%m_pGameServiceOption->wTableCount;
							CTableFrame * pTableFrame=m_TableFrameArray[nTableIndex];
							if (pTableFrame->IsGameStarted()==true) continue;

							//桌子状况
							tagTableUserInfo TableUserInfo;
							WORD wUserSitCount=pTableFrame->GetTableUserInfo(TableUserInfo);

							//用户过虑
							bool bRand = ((rand()%100)>50);
							if (TableUserInfo.wTableAndroidCount==0) continue;
							if ((TableUserInfo.wTableUserCount>0)&&(bAllowAndroidAttend==true) && bRand) continue;
							if (TableUserInfo.wTableAndroidCount>=TableUserInfo.wMinUserCount && bRand) continue;

							//起立处理
							for (WORD j=0;j<pTableFrame->GetChairCount();j++)
							{
								//获取用户
								IServerUserItem * pIServerUserItem=pTableFrame->GetTableUserItem(j);
								if (pIServerUserItem==NULL) continue;

								//用户起立
								if ((pIServerUserItem->IsAndroidUser()==true)&&(pTableFrame->PerformStandUpAction(pIServerUserItem)==true))
								{
									wStandUpCount++;
									if(wStandUpCount>=wRandCount)
										return true;
									else
										break;
								}
							}
						}

						//起立处理
						nIndex = rand()%(__max(m_pGameServiceOption->wTableCount,1));
						for (INT_PTR i=nIndex;i<m_pGameServiceOption->wTableCount+nIndex;++i)
						{
							//获取桌子
							INT_PTR nTableIndex=i%m_pGameServiceOption->wTableCount;
							CTableFrame * pTableFrame=m_TableFrameArray[nTableIndex];
							if (pTableFrame->IsGameStarted()==true) continue;

							//桌子状况
							tagTableUserInfo TableUserInfo;
							WORD wUserSitCount=pTableFrame->GetTableUserInfo(TableUserInfo);

							//用户过虑
							bool bRand = ((rand()%100)>50);
							if (TableUserInfo.wTableAndroidCount==0) continue;
							if ((TableUserInfo.wTableUserCount>0)&&(bAllowAndroidAttend==true) && bRand) continue;

							//起立处理
							for (WORD j=0;j<pTableFrame->GetChairCount();j++)
							{
								//获取用户
								IServerUserItem * pIServerUserItem=pTableFrame->GetTableUserItem(j);
								if (pIServerUserItem==NULL) continue;

								//用户起立
								
								if ((pIServerUserItem->IsAndroidUser()==true)&&(pTableFrame->PerformStandUpAction(pIServerUserItem)==true))
								{
									wStandUpCount++;
									if(wStandUpCount>=wRandCount)
										return true;
									else
										break;
								}
							}
						}
					}
				}	


				return true;
			}
		case IDI_DBCORRESPOND_NOTIFY: //缓存定时处理
			{
				if(m_pIDBCorrespondManager) m_pIDBCorrespondManager->OnTimerNotify();
				return true;
			}
		case IDI_LOAD_SYSTEM_MESSAGE: //系统消息 作废
			{
				//清除消息数据
				//ClearSystemMessageData();

				//加载消息
				//m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_SYSTEM_MESSAGE,0L,NULL,0L);

				return true;
			}
			//弹窗消息
		case IDI_LOAD_TIP_MESSAGE:
			{

// 				GameServer::GSLogonFailure gsLogonFailure;
// 				gsLogonFailure.set_errorcode(152345);
// 				gsLogonFailure.set_describestring(Utils::ConvertFromUtf16ToUtf8(TEXT("faklfdjks话费卡丝黛芬妮加快速度发货就是的看法和借款收到")).c_str());
// 
// 				std::string serializeStr;
// 				gsLogonFailure.SerializeToString(&serializeStr);
// 		
// 				//读取弹窗消息
// 				m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_TIP_MESSAGE,0L, (void*)serializeStr.c_str(), (WORD)serializeStr.size());
				m_pIDBCorrespondManager->PostDataBaseRequest(0L, DBR_GR_LOAD_TIP_MESSAGE, 0L, NULL,0);

				return true;
			}
			// 读取 Redis 系统滚动消息
// 		case TIME_REDIS_SYSTEM_MESSAGE:
// 			{
// 				return true;
// 			}
			//读取 Redis 滚动消息
		case IDI_REDIS_SCROLL_MESSAGE:
			{
				m_pIDBCorrespondManager->PostDataBaseRequest(0L, DBR_GR_SCROLL_MESSAGE, 0L, NULL, 0);
// 				CString CStrKey;
// 				CStrKey.Format(TEXT("activity:listener:key"));
// 				std::string Key("0"), Valus("0");
// 				Key = CT2A(CStrKey.GetBuffer());
// 				TINT count;
// 				m_RedisClent.hlen(Key, count);
// 				DATAITEM array;
// 				m_RedisClent.hgetall(Key, array);
				//CTraceService::TraceString(strArray->GetAt(i), TraceLevel_Exception);
				
				return true;

//  				CString CStrKey;
//  				CStrKey.Format(TEXT("Fish:%d:ScrollMessage"), m_pGameServiceOption->wRoomType);
//  				std::string Key("0"), Valus("0");
//  				Key = CT2A(CStrKey.GetBuffer());
//  				m_RedisClent.get(Key, Valus);
//  				std::string  NewValus("0");
//  				m_RedisClent.set(Key, NewValus);
//  
//  				CString MesBox(Valus.c_str());
//  				CStringArray* strArray = DivString(MesBox, TEXT("#"));
//  
//  				for (int i = 0; i < strArray->GetSize(); i++)
//  				{
//  					if (strArray->GetAt(i) != TEXT("") && strArray->GetAt(i) != TEXT("0"))
//  					{
//  						CTraceService::TraceString(strArray->GetAt(i), TraceLevel_Exception);
// 
// 						//GameServer::RedisRollMessage  RedisRollMes; CT2A(CStrKey.GetBuffer())
// 						//RedisRollMes.set_RedisRollMsg(Utils::ConvertFromUtf16ToUtf8(CT2A(strArray->GetAt(i).GetBuffer())));
// 						//std::string serializeStr;
// 						//RedisRollMes.SerializeToString(&serializeStr);
// 
// 						//发送用户
// 						WORD wIndex = 0;
// 						do
// 						{
// 							//获取用户
// 							IServerUserItem * pIServerUserItem = m_ServerUserManager.EnumUserItem(wIndex++);
// 							if (pIServerUserItem == NULL) break;
// 
// 							CString str;
// 							str.Format(TEXT("向用户 %s -- %d 发送滚动消息 \n	%s"), 
// 								pIServerUserItem->GetNickName(), pIServerUserItem->GetUserID(), strArray->GetAt(i));
// 							CTraceService::TraceString(str, TraceLevel_Exception);
// 							//发送数据
// 							//SendData(pIServerUserItem, MDM_GF_FRAME, SUB_GF_REDIS_SCROLL_MES, (void*)serializeStr.c_str(), (WORD)serializeStr.size());
// 						} while (true);
//  					}
//  				}
// 
// 	
				

 				return true;
			}
			//读取滚动消息
		case IDI_LOAD_SCROLL_MESSAGE:
			{
				//读取滚动消息
				m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_SCROLL_MESSAGE,0L,NULL,0L);

				return true;
			}
			//清除滚动消息
		case IDI_CLEAR_SCROLL_MESSAGE:
			{
				//清除滚动消息
				m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_CLEAR_SCROLL_MESSAGE,0L,NULL,0L);

				return true;
			}
		case IDI_LOAD_SENSITIVE_WORD:	//加载敏感词
			{
				//投递请求
				m_pIRecordDataBaseEngine->PostDataBaseRequest(DBR_GR_LOAD_SENSITIVE_WORDS,0,NULL,0);				
				return true;
			}
		case IDI_SEND_SYSTEM_MESSAGE: //系统消息 作废，改为登录服发送系统消息
			{

				return true;

				//数量判断
				if(m_SystemMessageList.GetCount()==0) return true;

				//时效判断
				DWORD dwCurrTime = (DWORD)time(NULL);
				POSITION pos = m_SystemMessageList.GetHeadPosition();
				while(pos != NULL)
				{
					POSITION tempPos = pos;
					tagSystemMessage *pTagSystemMessage = m_SystemMessageList.GetNext(pos);
					if(pTagSystemMessage->dwLastTime+pTagSystemMessage->SystemMessage.dwTimeRate < dwCurrTime)
					{
						//更新数据
						pTagSystemMessage->dwLastTime=dwCurrTime;

						//构造消息
						CMD_GR_SendMessage SendMessage = {};
						SendMessage.cbAllRoom = FALSE;
						SendMessage.cbGame = (pTagSystemMessage->SystemMessage.cbMessageType==1)?TRUE:FALSE;
						SendMessage.cbRoom = (pTagSystemMessage->SystemMessage.cbMessageType==2)?TRUE:FALSE;
						if(pTagSystemMessage->SystemMessage.cbMessageType==3)
						{
							SendMessage.cbGame = TRUE;
							SendMessage.cbRoom = TRUE;
						}
						lstrcpyn(SendMessage.szSystemMessage,pTagSystemMessage->SystemMessage.szSystemMessage,CountArray(SendMessage.szSystemMessage));
						SendMessage.wChatLength = lstrlen(SendMessage.szSystemMessage)+1;

						//发送消息
						WORD wSendSize = sizeof(SendMessage)-sizeof(SendMessage.szSystemMessage)+CountStringBuffer(SendMessage.szSystemMessage);
						SendSystemMessage(&SendMessage,wSendSize);
					}
				}


				return true;
			}

		case IDI_ANDRIOND_DELAY_JION:
			{
				tagAndroidUserInfo AndroidSimulate; 
				m_AndroidUserManager.GetAndroidUserInfo(AndroidSimulate,ANDROID_SIMULATE);
				bool bAllowDynamicJoin=CServerRule::IsAllowDynamicJoin(m_pGameServiceOption->dwServerRule);

				INT wTableID = wBindParam;
				if (AndroidSimulate.wFreeUserCount > 0)
				{
					CTableFrame * pTableFrame=m_TableFrameArray[wTableID];
					if ((pTableFrame->IsGameStarted()==true)&&(bAllowDynamicJoin==false)) return true;

					//桌子状况
					tagTableUserInfo TableUserInfo;
					WORD wUserSitCount=pTableFrame->GetTableUserInfo(TableUserInfo);

					//分配判断
					if (TableUserInfo.wTableUserCount>0) return true;

					//变量定义
					WORD wChairID=pTableFrame->GetRandNullChairID();

					//无效过滤
					if (wChairID==INVALID_CHAIR) return true;

					//用户坐下
					IAndroidUserItem * pIAndroidUserItem=AndroidSimulate.pIAndroidUserFree[0];
					pTableFrame->PerformSitDownAction(wChairID,pIAndroidUserItem->GetMeUserItem());

				}

				return true;
			}
		}

	}

	//机器时器
	if ((dwTimerID>=IDI_REBOT_MODULE_START)&&(dwTimerID<=IDI_REBOT_MODULE_FINISH))
	{
		//时间处理
		m_AndroidUserManager.OnEventTimerPulse(dwTimerID,wBindParam);

		return true;
	}

	//比赛定时器
	if((dwTimerID>=IDI_MATCH_MODULE_START)&&(dwTimerID<IDI_MATCH_MODULE_FINISH))
	{
		if(m_pIGameMatchServiceManager!=NULL) m_pIGameMatchServiceManager->OnEventTimer(dwTimerID,wBindParam);
		return true;
	}

	//桌子时间
	if ((dwTimerID>=IDI_TABLE_MODULE_START)&&(dwTimerID<=IDI_TABLE_MODULE_FINISH))
	{
		//桌子号码
		DWORD dwTableTimerID=dwTimerID-IDI_TABLE_MODULE_START;
		WORD wTableID=(WORD)(dwTableTimerID/TIME_TABLE_MODULE_RANGE);

		//时间效验
		if (wTableID>=(WORD)m_TableFrameArray.GetCount()) 
		{
			ASSERT_ERR(FALSE);
			return false;
		}

		//时间通知
		CTableFrame * pTableFrame=m_TableFrameArray[wTableID];
		return pTableFrame->OnEventTimer(dwTableTimerID%TIME_TABLE_MODULE_RANGE,wBindParam);
	}

	return false;
}

//数据库事件
bool CAttemperEngineSink::OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	bool bResult = false;

	do
	{
		auto it = m_mDbFunctions.find(wRequestID);
		if (it == m_mDbFunctions.end())
			break;

		bResult = (it->second)(dwContextID, pData, wDataSize);

	} while (0);


	//比赛事件
	if(wRequestID>=DBO_GR_MATCH_EVENT_START && wRequestID<=DBO_GR_MATCH_EVENT_END)
	{
		//参数效验
		if(m_pIGameMatchServiceManager==NULL) return false;

		tagBindParameter * pBindParameter=GetBindParameter(LOWORD(dwContextID));
		IServerUserItem *pIServerUserItem=pBindParameter!=NULL?pBindParameter->pIServerUserItem:NULL;

		return m_pIGameMatchServiceManager->OnEventDataBase(wRequestID,pIServerUserItem,pData,wDataSize);
	}

	return bResult;
}

//关闭事件
bool CAttemperEngineSink::OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason)
{
	//协调连接
	if (wServiceID==NETWORK_CORRESPOND)
	{
		//设置变量
		m_bCollectUser=false;

		//删除时间
		m_pITimerEngine->KillTimer(IDI_REPORT_SERVER_INFO);

		//重连判断
		if (m_bNeekCorrespond==true)
		{
			//构造提示
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("与协调服务器的连接关闭了，%ld 秒后将重新连接"),m_pInitParameter->m_wConnectTime);

			//提示消息
			//TRACE_LOG(szDescribe,TraceLevel_Warning);
			I_TRACE(szDescribe);

			//设置时间
			ASSERT_ERR(m_pITimerEngine!=NULL);
			m_pITimerEngine->SetTimer(IDI_CONNECT_CORRESPOND,m_pInitParameter->m_wConnectTime*1000L,1,0);
		}

		return true;
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
			//TRACE_LOG(szDescribe,TraceLevel_Warning);
			I_TRACE(szDescribe);

			//设置时间
			ASSERT_ERR(m_pITimerEngine!=NULL);
			m_pITimerEngine->SetTimer(IDI_CONNECT_CORRESPOND,m_pInitParameter->m_wConnectTime*1000L,1,0);

			return false;
		}

		//提示消息
		TRACE_LOG(TEXT("正在发送游戏房间注册信息..."),TraceLevel_Normal);

		//变量定义
		CMD_CS_C_RegisterServer RegisterServer;
		ZeroMemory(&RegisterServer,sizeof(RegisterServer));

		//服务端口
		CServiceUnits * pServiceUnits=CServiceUnits::g_pServiceUnits;
		RegisterServer.wServerPort=pServiceUnits->m_TCPNetworkEngine->GetCurrentPort();

		//构造数据
		RegisterServer.wKindID=m_pGameServiceOption->wKindID;
		RegisterServer.wNodeID=m_pGameServiceOption->wNodeID;
		RegisterServer.wSortID=m_pGameServiceOption->wSortID;
		RegisterServer.wServerID=m_pGameServiceOption->wServerID;
		RegisterServer.wServerType = m_pGameServiceOption->wServerType;
		RegisterServer.dwOnLineCount=m_ServerUserManager.GetUserItemCount();
		RegisterServer.dwFullCount=m_pGameServiceOption->wMaxPlayer-RESERVE_USER_COUNT;
		lstrcpyn(RegisterServer.szServerName,m_pGameServiceOption->szServerName,CountArray(RegisterServer.szServerName));
		lstrcpyn(RegisterServer.szServerAddr,m_pInitParameter->m_ServiceAddress.szAddress,CountArray(RegisterServer.szServerAddr));
		RegisterServer.lCellScore = m_pGameServiceOption->lCellScore;									//游戏基数
		RegisterServer.cbRoomType = m_pGameServiceOption->wRoomType;

		//限制配置
		RegisterServer.lRestrictScore	= m_pGameServiceOption->lRestrictScore	;						//限制积分
		RegisterServer.lMinTableScore	= m_pGameServiceOption->lMinTableScore	;						//最低积分
		RegisterServer.lMinEnterScore	= m_pGameServiceOption->lMinEnterScore	;						//最低积分
		RegisterServer.lMaxEnterScore	= m_pGameServiceOption->lMaxEnterScore	;						//最高积分
		RegisterServer.lMinEnterTiLi	= m_pGameServiceOption->lMinEnterTiLi	;						//最低体力

		RegisterServer.wCardBoxIndex	= m_pGameServiceOption->wCardBoxIndex	;						//宝箱索引

					
		//设置时间 
		ASSERT_ERR(m_pITimerEngine!=NULL);
		m_pITimerEngine->SetTimer(IDI_REPORT_SERVER_INFO,TIME_REPORT_SERVER_INFO*1000L,TIMES_INFINITY,0);

		//发送数据
		ASSERT_ERR(m_pITCPSocketService != NULL);
		m_pITCPSocketService->SendData(MDM_CS_REGISTER, SUB_CS_C_REGISTER_SERVER, &RegisterServer, sizeof(RegisterServer));
		

		return true;
	}

	return true;
}

//读取事件
bool CAttemperEngineSink::OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//协调连接
	if (wServiceID==NETWORK_CORRESPOND)
	{
		switch (Command.wMainCmdID)
		{
		case MDM_CS_REGISTER:		//注册服务
			{
				return OnTCPSocketMainRegister(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_SERVICE_INFO:	//服务信息
			{
				return OnTCPSocketMainServiceInfo(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_USER_COLLECT:	//用户汇总
			{
				return OnTCPSocketMainUserCollect(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_MANAGER_SERVICE: //管理服务
			{
				return OnTCPSocketMainManagerService(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_REMOTE_SERVICE:	//远程服务
			{
				return OnTCPSocketMainRemoteService(Command.wSubCmdID,pData,wDataSize);
			}
		}
	}

	//错误断言
	ASSERT_ERR(FALSE);

	return true;
}

//远程服务
bool CAttemperEngineSink::OnTCPSocketMainRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID) {
	case  SUB_CS_S_USER_CLEAR:
	{
		ASSERT(wDataSize == sizeof(CMD_CS_S_ClearUser));
		if (wDataSize != sizeof(CMD_CS_S_ClearUser)) return false;

		CMD_CS_S_ClearUser* pClearUser = (CMD_CS_S_ClearUser*)pData;

		IServerUserItem * pIServerUserItem = m_ServerUserManager.SearchUserItem(pClearUser->dwUserID);
		if (pIServerUserItem == NULL) return true;
		WORD wTableID = pIServerUserItem->GetTableID();
		DWORD dwUserID = pIServerUserItem->GetUserID();
		BYTE cbStatus = pIServerUserItem->GetUserStatus();

		I_TRACE(TEXT("准备踢人UserID[%d],TableID[%d],Status[%d],限时[%d]秒"),dwUserID,wTableID, cbStatus,pClearUser->dwLimitTime*60);
		tagKickUser KickUser = { 0 };
		KickUser.dwLimitTime = pClearUser->dwLimitTime * 60;
		KickUser.dwStartKickTime = (DWORD)time(NULL);
		m_KickUserItemMap[dwUserID] = KickUser;

		bool bResult = false;
		bResult = DetectKickUser(pIServerUserItem, TRUE);
		if (bResult)
			I_TRACE(TEXT("踢人完毕"));
		else I_TRACE(TEXT("玩家正在游戏中，延迟踢人"));
		return true;
	}
	}
	return true;
}

//应答事件
bool CAttemperEngineSink::OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID)
{
	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	//设置变量
	if (pBindParameter!=NULL)
	{
		pBindParameter->dwSocketID=dwSocketID;
		pBindParameter->dwClientAddr=dwClientAddr;
		pBindParameter->dwActiveTime=(DWORD)time(NULL);

		return true;
	}

	//错误断言

	ASSERT_ERR(FALSE);
	return false;
}

//关闭事件
bool CAttemperEngineSink::OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID)
{
	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
	I_TRACE(TEXT("OnEventTCPNetworkShut SocketID:%d"), wBindIndex);
	g_Log.LogToFile(g_szLogRoot, TEXT("准备断开链接 SocketID[%d]"),dwSocketID);

	if (pBindParameter==NULL) return false;

	//获取用户
	IServerUserItem * pIServerUserItem=pBindParameter->pIServerUserItem;

	//用户处理
	if (pIServerUserItem!=NULL)
	{
		//变量定义
		WORD wTableID=pIServerUserItem->GetTableID();

		if ( pIServerUserItem->IsAndroidUser() == false)
		g_Log.LogToFile(g_szLogRoot,TEXT("\nOnEventTCPNetworkShut UserID[%d] NickName[%s]:UserScore[%I64d]"),pIServerUserItem->GetUserID(),pIServerUserItem->GetNickName(),pIServerUserItem->GetUserScore());
		//断线处理 机器人没有掉线，一律踢出房间
		if (wTableID!=INVALID_TABLE && pIServerUserItem->IsAndroidUser()==false)
		{
			CTableFrame *pTableFrame = m_TableFrameArray[wTableID];
			BYTE cbStartMode = 0;
			if ( pTableFrame ) cbStartMode = pTableFrame->GetStartMode();		

			//捕鱼走这里
			//试玩场强退解散游戏		
			if( (cbStartMode&START_MODE_TIME_CONTROL && m_pGameServiceAttrib->wChairCount<MAX_CHAIR) ||	
				( m_pGameServiceOption->wRoomType == 4 ))
			{
				if (pTableFrame)
				{
					I_TRACE(TEXT("H5玩家连接断开，先起立，再断开"));
					if ( pIServerUserItem->GetUserInfo()->cbClientKind == CLIENT_KIND_WEBSOCKET )
						pTableFrame->PerformStandUpActionH5(pIServerUserItem);
					else {
						pTableFrame->PerformStandUpAction(pIServerUserItem);
					}
					pIServerUserItem->SetUserStatus(US_NULL, INVALID_TABLE, INVALID_CHAIR);
				}
				
			}
			else
			{						
				//I_TRACE(TEXT("玩家 %s ,Status:%d,"),pIServerUserItem->GetNickName(),pIServerUserItem->GetUserStatus());
				//断线通知
				ASSERT_ERR(wTableID<m_pGameServiceOption->wTableCount);
				m_TableFrameArray[wTableID]->OnEventUserOffLine(pIServerUserItem);
				//解除绑定
				pIServerUserItem->DetachBindStatus();
				
			}

		}
		else
		{    
			I_TRACE(TEXT("关闭连接回调 %s ,Status:%d,%d-%d"), pIServerUserItem->GetNickName(),pIServerUserItem->GetUserStatus(),pIServerUserItem->GetTableID(),pIServerUserItem->GetChairID());
			pIServerUserItem->SetUserStatus(US_NULL,INVALID_TABLE,INVALID_CHAIR);
		}
	}
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
				I_TRACE(szMsg);

				pIUserItem->SetUserStatus(US_NULL,INVALID_TABLE,INVALID_CHAIR);
				break;
			}

		} while (true);
	}

	//清除信息
	pBindParameter->pIServerUserItem=NULL;
	ZeroMemory(pBindParameter,sizeof(tagBindParameter));

	return false;
}

//读取事件
bool CAttemperEngineSink::OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	bool bResult = false;

	do 
	{
		auto it = m_mMainFunctions.find(Command.wMainCmdID);
		if(it == m_mMainFunctions.end())
			break;

		bResult = (it->second)(Command.wSubCmdID, pData, wDataSize, dwSocketID);

	} while (0);

	return bResult;
}

//房间消息
bool CAttemperEngineSink::SendRoomMessage(LPCTSTR lpszMessage, WORD wType)
{
	//变量定义
	CMD_CM_SystemMessage SystemMessage;
	ZeroMemory(&SystemMessage,sizeof(SystemMessage));

	//构造数据
	SystemMessage.wType=wType;
	SystemMessage.wLength=lstrlen(lpszMessage)+1;
	lstrcpyn(SystemMessage.szString,lpszMessage,CountArray(SystemMessage.szString));

	//数据属性
	WORD wHeadSize=sizeof(SystemMessage)-sizeof(SystemMessage.szString);
	WORD wSendSize=wHeadSize+CountStringBuffer(SystemMessage.szString);

	//发送数据
	m_AndroidUserManager.SendDataToClient(MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&SystemMessage,wSendSize);
	m_pITCPNetworkEngine->SendDataBatch(MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&SystemMessage,wSendSize,BG_COMPUTER);
	m_pITCPNetworkEngine->SendDataBatch(MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&SystemMessage,wSendSize,BG_MOBILE);

	return true;
}

//游戏消息
bool CAttemperEngineSink::SendGameMessage(LPCTSTR lpszMessage, WORD wType)
{
	//变量定义
	CMD_CM_SystemMessage SystemMessage;
	ZeroMemory(&SystemMessage,sizeof(SystemMessage));

	//构造数据
	SystemMessage.wType=wType;
	SystemMessage.wLength=lstrlen(lpszMessage)+1;
	lstrcpyn(SystemMessage.szString,lpszMessage,CountArray(SystemMessage.szString));

	//数据属性
	WORD wHeadSize=sizeof(SystemMessage)-sizeof(SystemMessage.szString);
	WORD wSendSize=wHeadSize+CountStringBuffer(SystemMessage.szString);

	//发送数据
	m_AndroidUserManager.SendDataToClient(MDM_GF_FRAME,SUB_GF_SYSTEM_MESSAGE,&SystemMessage,wSendSize);
	m_pITCPNetworkEngine->SendDataBatch(MDM_GF_FRAME,SUB_GF_SYSTEM_MESSAGE,&SystemMessage,wSendSize,BG_COMPUTER);
	m_pITCPNetworkEngine->SendDataBatch(MDM_GF_FRAME,SUB_GF_SYSTEM_MESSAGE,&SystemMessage,wSendSize,BG_MOBILE);

	return true;
}

//房间消息
//#define SMT_CHAT						0x0001								//聊天消息
//#define SMT_EJECT						0x0002								//弹出消息
//#define SMT_GLOBAL					0x0004								//全局消息
//#define SMT_PROMPT					0x0008								//提示消息
//#define SMT_TABLE_ROLL				0x0010								//滚动消息
bool CAttemperEngineSink::SendRoomMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType)
{
	//效验参数
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	if ( pIServerUserItem->GetUserInfo()->cbClientKind == CLIENT_KIND_WEBSOCKET )
	{
		GameServer::GSSystemMessage gsSystemMessage;
		gsSystemMessage.set_messagetype(wType);
		gsSystemMessage.set_messagestring(Utils::ConvertFromUtf16ToUtf8(lpszMessage));
		std::string serializeStr;
		gsSystemMessage.SerializeToString(&serializeStr);
		
		SendData(pIServerUserItem, MDM_CM_SYSTEM, SUB_CM_SYSTEM_MESSAGE,(void*)serializeStr.c_str(), (WORD)serializeStr.size());
		return true;
	}

	//发送数据
	if (pIServerUserItem->GetBindIndex()!=INVALID_WORD)
	{
		//变量定义
		CMD_CM_SystemMessage SystemMessage;
		ZeroMemory(&SystemMessage,sizeof(SystemMessage));

		//构造数据
		SystemMessage.wType=wType;
		SystemMessage.wLength=lstrlen(lpszMessage)+1;
		lstrcpyn(SystemMessage.szString,lpszMessage,CountArray(SystemMessage.szString));

		//变量定义
		WORD dwUserIndex=pIServerUserItem->GetBindIndex();
		tagBindParameter * pBindParameter=GetBindParameter(dwUserIndex);

		//数据属性
		WORD wHeadSize=sizeof(SystemMessage)-sizeof(SystemMessage.szString);
		WORD wSendSize=wHeadSize+CountStringBuffer(SystemMessage.szString);

		//发送数据
		if (pIServerUserItem->IsAndroidUser()==true)
		{
			//机器用户
			WORD wBindIndex=pIServerUserItem->GetBindIndex();
			tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
			m_AndroidUserManager.SendDataToClient(pBindParameter->dwSocketID,MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&SystemMessage,wSendSize);

			//关闭处理
			if ((wType&(SMT_CLOSE_ROOM|SMT_CLOSE_LINK))!=0) {
				
				m_AndroidUserManager.DeleteAndroidUserItem(pBindParameter->dwSocketID);
			}
		}
		else
		{
			//常规用户
			WORD wBindIndex=pIServerUserItem->GetBindIndex();
			tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
			m_pITCPNetworkEngine->SendData(pBindParameter->dwSocketID,MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&SystemMessage,wSendSize);
		}

		return true;
	}

	return false;
}

//游戏消息
bool CAttemperEngineSink::SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType)
{
	//效验参数
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//发送数据
	if ((pIServerUserItem->GetBindIndex()!=INVALID_WORD)&&(pIServerUserItem->IsClientReady()==true))
	{
		//变量定义
		CMD_CM_SystemMessage SystemMessage;
		ZeroMemory(&SystemMessage,sizeof(SystemMessage));

		//构造数据
		SystemMessage.wType=wType;
		SystemMessage.wLength=lstrlen(lpszMessage)+1;
		lstrcpyn(SystemMessage.szString,lpszMessage,CountArray(SystemMessage.szString));

		//变量定义
		WORD dwUserIndex=pIServerUserItem->GetBindIndex();
		tagBindParameter * pBindParameter=GetBindParameter(dwUserIndex);

		//数据属性
		WORD wHeadSize=sizeof(SystemMessage)-sizeof(SystemMessage.szString);
		WORD wSendSize=wHeadSize+CountStringBuffer(SystemMessage.szString);

		//发送数据
		if (pIServerUserItem->IsAndroidUser()==true)
		{
			//机器用户
			WORD wBindIndex=pIServerUserItem->GetBindIndex();
			tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
			m_AndroidUserManager.SendDataToClient(pBindParameter->dwSocketID,MDM_GF_FRAME,SUB_GF_SYSTEM_MESSAGE,&SystemMessage,wSendSize);

			//关闭处理
			if ((wType&(SMT_CLOSE_ROOM|SMT_CLOSE_LINK))!=0) {
				
				m_AndroidUserManager.DeleteAndroidUserItem(pBindParameter->dwSocketID);
			}
		}
		else
		{
			//常规用户
			WORD wBindIndex=pIServerUserItem->GetBindIndex();
			tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
			m_pITCPNetworkEngine->SendData(pBindParameter->dwSocketID,MDM_GF_FRAME,SUB_GF_SYSTEM_MESSAGE,&SystemMessage,wSendSize);
		}

		return true;
	}

	return false;
}

//房间消息
bool CAttemperEngineSink::SendRoomMessage(DWORD dwSocketID, LPCTSTR lpszMessage, WORD wType, bool bAndroid)
{
	//变量定义
	CMD_CM_SystemMessage SystemMessage;
	ZeroMemory(&SystemMessage,sizeof(SystemMessage));

	//构造数据
	SystemMessage.wType=wType;
	SystemMessage.wLength=lstrlen(lpszMessage)+1;
	lstrcpyn(SystemMessage.szString,lpszMessage,CountArray(SystemMessage.szString));

	//数据属性
	WORD wHeadSize=sizeof(SystemMessage)-sizeof(SystemMessage.szString);
	WORD wSendSize=wHeadSize+CountStringBuffer(SystemMessage.szString);

	//发送数据
	if (bAndroid)
	{
		//机器用户
		m_AndroidUserManager.SendDataToClient(dwSocketID,MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&SystemMessage,wSendSize);
	}
	else
	{
		//常规用户
		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&SystemMessage,wSendSize);
	}

	return true;
}

bool CAttemperEngineSink::SendMatchMessage(WORD wMainCmdID, WORD wSubCmdID)
{
	//发送消息
	ASSERT_ERR(m_pITCPSocketService!=NULL);
	m_pITCPSocketService->SendData(wMainCmdID,wSubCmdID);
	return true;
}

bool CAttemperEngineSink::SendMatchMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//发送消息
	ASSERT_ERR(m_pITCPSocketService!=NULL);
	m_pITCPSocketService->SendData(wMainCmdID,wSubCmdID,pData,wDataSize);
	return true;
}

//发送数据
bool CAttemperEngineSink::SendData(BYTE cbSendMask, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//机器数据
	if ((cbSendMask&BG_COMPUTER)!=0)
	{
		m_AndroidUserManager.SendDataToClient(wMainCmdID,wSubCmdID,pData,wDataSize);
	}

	//用户数据
	m_pITCPNetworkEngine->SendDataBatch(wMainCmdID,wSubCmdID,pData,wDataSize,cbSendMask);

	return true;
}

//发送数据
bool CAttemperEngineSink::SendData(DWORD dwSocketID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//发送数据
	if (LOWORD(dwSocketID)!=INVALID_WORD)
	{
		if (LOWORD(dwSocketID)>=INDEX_ANDROID)
		{
			//机器用户
			//I_TRACE(TEXT("发送机器人数据:SocketID[%d],M_Cmd[%d],S_Cmd[%d]"),dwSocketID,wMainCmdID,wSubCmdID);
			m_AndroidUserManager.SendDataToClient(dwSocketID,wMainCmdID,wSubCmdID,pData,wDataSize);
		}
		else 
		{
			//网络用户
			m_pITCPNetworkEngine->SendData(dwSocketID,wMainCmdID,wSubCmdID,pData,wDataSize);
		}
	}

	return true;
}

//发送数据
bool CAttemperEngineSink::SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//发送数据
	if (pIServerUserItem->GetBindIndex()!=INVALID_WORD)
	{
		if (pIServerUserItem->IsAndroidUser()==true)
		{
			//机器用户
			WORD wBindIndex=pIServerUserItem->GetBindIndex();
			tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
			m_AndroidUserManager.SendDataToClient(pBindParameter->dwSocketID,wMainCmdID,wSubCmdID,pData,wDataSize);
		}
		else
		{
			//常规用户
			WORD wBindIndex=pIServerUserItem->GetBindIndex();
			tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
			m_pITCPNetworkEngine->SendData(pBindParameter->dwSocketID,wMainCmdID,wSubCmdID,pData,wDataSize);
		}

		return true;
	}

	return false;
}

bool CAttemperEngineSink::PostDataBaseRequest(IServerUserItem* pIServerUserItem, WORD wRequest, VOID *pData, WORD wDataSize)
{
	//效验参数
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//发送数据
	if (pIServerUserItem->GetBindIndex()!=INVALID_WORD)
	{
		WORD wBindIndex=pIServerUserItem->GetBindIndex();
		tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

		m_pIKernelDataBaseEngine->PostDataBaseRequest(wRequest, pBindParameter->dwSocketID, pData, wDataSize);

		return true;
	}

	return false;
}

//用户积分
bool CAttemperEngineSink::OnEventUserItemScore(IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	//效验参数
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//比赛自己写分
	if (m_pGameServiceOption->wServerType == GAME_GENRE_MATCH) return true;

	if ( cbReason == SCORE_REASON_WRITE || cbReason == SCORE_REASON_INSURE || cbReason == SCORE_REASON_PROPERTY || 
		 cbReason == SCORE_REASON_MEDAL )
	{
		tagUserInfo * pUserInfo = pIServerUserItem->GetUserInfo();
		DWORD dwUserID = pUserInfo->dwUserID;
		WORD wTableID = pUserInfo->wTableID;
		DWORD dwWinCount = pUserInfo->dwWinCount;
		DWORD dwLostCount = pUserInfo->dwLostCount;
		DWORD dwDrawCount = pUserInfo->dwDrawCount;
		DWORD dwFleeCount = pUserInfo->dwFleeCount;
		SCORE lUserScore = pUserInfo->lScore;

		WORD wBindIndex = pIServerUserItem->GetBindIndex();
		tagBindParameter * pBindParameter = GetBindParameter(wBindIndex);
		if (pBindParameter == NULL) return true;

		//if (pBindParameter != NULL && pBindParameter->cbClientKind == CLIENT_KIND_WEBSOCKET) 
		{

			GameServer::GSUserScore gsUserScore;
			gsUserScore.set_userid((int32)dwUserID);
			gsUserScore.mutable_userscore()->set_userscore((int64)lUserScore);
			gsUserScore.mutable_userscore()->set_wincount((int32)dwWinCount);
			gsUserScore.mutable_userscore()->set_lostcount((int32)dwLostCount);
			gsUserScore.mutable_userscore()->set_drawcount((int32)dwDrawCount);
			gsUserScore.mutable_userscore()->set_fleecount((int32)dwFleeCount);

			std::string serializeStr;
			gsUserScore.SerializeToString(&serializeStr);
			CTableFrame * pTableFrame = m_TableFrameArray[wTableID];
			if (pTableFrame)
				pTableFrame->SendTableData(INVALID_CHAIR, SUB_GR_USER_SCORE, (void*)serializeStr.c_str(), (WORD)serializeStr.size(), MDM_GR_USER);
		}
// 		else{
// 			CMD_GR_UserScore UserScore;
// 
// 			//构造数据
// 			UserScore.dwUserID = pUserInfo->dwUserID;
// 			UserScore.UserScore.dwWinCount = pUserInfo->dwWinCount;
// 			UserScore.UserScore.dwLostCount = pUserInfo->dwLostCount;
// 			UserScore.UserScore.dwDrawCount = pUserInfo->dwDrawCount;
// 			UserScore.UserScore.dwFleeCount = pUserInfo->dwFleeCount;
// 			UserScore.UserScore.dwUserMedal = pUserInfo->dwUserMedal;
// 			UserScore.UserScore.dwExperience = pUserInfo->dwExperience;
// 			UserScore.UserScore.lLoveLiness = pUserInfo->lLoveLiness;
// 
// 			//构造积分
// 			UserScore.UserScore.lGrade = pUserInfo->lGrade;
// 			UserScore.UserScore.lInsure = pUserInfo->lInsure;
// 
// 			//构造积分	
// 			UserScore.UserScore.lScore = pUserInfo->lScore;
// 
// 			//UserScore.UserScore.lScore += pIServerUserItem->GetTrusteeScore();
// 			//UserScore.UserScore.lScore += pIServerUserItem->GetFrozenedScore();
// 
// 			//发送数据
// 			SendData(BG_COMPUTER, MDM_GR_USER, SUB_GR_USER_SCORE, &UserScore, sizeof(UserScore));
// 			SendData(BG_MOBILE, MDM_GR_USER, SUB_GR_USER_SCORE, &UserScore, sizeof(UserScore));
// 		}	

		//即时写分
		if ((CServerRule::IsImmediateWriteScore(m_pGameServiceOption->dwServerRule)==true)&&(pIServerUserItem->IsVariation()==true))
		{
			if ( pIServerUserItem->IsAutoVariation() == true )
			{
				TCHAR szDescribe[128]=TEXT("");
				if (m_pGameServiceOption->wServerType&(GAME_GENRE_GOLD|GAME_GENRE_SCORE))
					_sntprintf(szDescribe,CountArray(szDescribe),TEXT("您携带的金币不足") SCORE_STRING TEXT(" ,系统已自动补分!"),m_pGameServiceOption->lMinEnterScore);
				SendGameMessage(pIServerUserItem,szDescribe,SMT_CHAT|SMT_EJECT);
			}

			//变量定义
			DBR_GR_WriteGameScore WriteGameScore;
			ZeroMemory(&WriteGameScore,sizeof(WriteGameScore));

			//用户信息
			WriteGameScore.dwUserID=pIServerUserItem->GetUserID();
			WriteGameScore.dwDBQuestID=pIServerUserItem->GetDBQuestID();
			WriteGameScore.dwClientAddr=pIServerUserItem->GetClientAddr();
			WriteGameScore.dwInoutIndex=pIServerUserItem->GetInoutIndex();
			WriteGameScore.wTableID = pIServerUserItem->GetTableID();

			//提取积分
			pIServerUserItem->DistillVariation(WriteGameScore.VariationInfo);


			//I_TRACE(TEXT("玩家%s 本局写分 %I64d"),pIServerUserItem->GetNickName(),WriteGameScore.VariationInfo.lScore);
			//g_Log.LogToFile(g_szLogRoot,TEXT("\n [用户写分] UserID[%d], WriteScore[%I64d],WriteRevenue[%I64d]"),pIServerUserItem->GetUserID(),WriteGameScore.VariationInfo.lScore,WriteGameScore.VariationInfo.lRevenue);
			//投递请求
			m_pIDBCorrespondManager->PostDataBaseRequest(WriteGameScore.dwUserID,DBR_GR_WRITE_GAME_SCORE,0L,&WriteGameScore,sizeof(WriteGameScore));
			
		}

		//通知桌子
		if(pIServerUserItem->GetTableID()!=INVALID_TABLE)
		{
			m_TableFrameArray[pIServerUserItem->GetTableID()]->OnUserScroeNotify(pIServerUserItem->GetChairID(),pIServerUserItem,cbReason);
		}
	}

	if ( cbReason == SCORE_REASON_WRITE )
	{


	}

	return true;
}

//用户状态
bool CAttemperEngineSink::OnEventUserItemStatus(IServerUserItem * pIServerUserItem, WORD wOldTableID, WORD wOldChairID, BYTE cbNotifyClient)
{
	//效验参数
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	DWORD dwUserID = pIServerUserItem->GetUserID();
	WORD wTableID = pIServerUserItem->GetTableID();
	WORD wChairID = pIServerUserItem->GetChairID();
	BYTE cbUserStatus = pIServerUserItem->GetUserStatus();

	WORD wBindIndex = pIServerUserItem->GetBindIndex();
	tagBindParameter * pBindParameter = GetBindParameter(wBindIndex);
	if (pBindParameter == NULL) {
		CString cs;
		cs.Format(TEXT("连接为空....UserID[%d],%d-%d,Status[%d]"), dwUserID, wTableID, wChairID, cbUserStatus);
		I_TRACE(cs.GetBuffer(0));
		m_ServerFileLog.Log(cs.GetBuffer(0));

		return true;
	}
	CString cs1;
	cs1.Format(TEXT("【状态变化开始】|桌椅[%d-%d] 玩家[%d] 状态[%d]"), wTableID, wChairID, dwUserID, cbUserStatus);
	m_ServerFileLog.Log(cs1.GetBuffer(0));

	//1.同桌玩家信息下发给坐下玩家
	//2.坐下玩家信息下发给同桌玩家
	if (wTableID>=0 && wTableID < m_pGameServiceOption->wTableCount)
	{

		CTableFrame * pTableFrame = m_TableFrameArray[wTableID];
		if (pTableFrame != NULL)
		{
			do 
			{
				if (pIServerUserItem->GetUserStatus() == US_SIT ||
					pIServerUserItem->GetUserStatus() == US_READY || //刚进来时下发同桌玩家
					pIServerUserItem->GetUserStatus() == US_PLAYING) //断线重连时下发同桌玩家
				{
					//if (pIServerUserItem->IsEnterTable() && m_pGameServiceOption->wKindID != 2016) break;

					//先下发自己
					if (pIServerUserItem->GetUserInfo()->cbClientKind == CLIENT_KIND_WEBSOCKET)
					{
						SendUserInfoPacketH5(pIServerUserItem, pBindParameter->dwSocketID);
						CString cs;
						cs.Format(TEXT("【主命令[3],次命令[100]】| 玩家进来[%d] 状态[%d] 桌椅[%d-%d]【个人信息发自己】"), pIServerUserItem->GetUserID(),pIServerUserItem->GetUserStatus(),pIServerUserItem->GetTableID(),pIServerUserItem->GetChairID());
						m_ServerFileLog.Log(cs.GetBuffer(0));
					}

					for ( auto i =0;i<pTableFrame->GetChairCount();++i )
					{
						IServerUserItem* pUserItem = pTableFrame->GetTableUserItem(i);
						if ( pUserItem == NULL ) continue;
						CString cs;
						cs.Format(TEXT("【同桌列表】桌号[%d] 桌椅[%d-%d] 玩家[%d] 状态[%d]"),pTableFrame->GetTableID(),pUserItem->GetTableID(),pUserItem->GetChairID(),pUserItem->GetUserID(),pUserItem->GetUserStatus());
						m_ServerFileLog.Log(cs.GetBuffer(0));
					}

					for (int i = 0; i < pTableFrame->GetChairCount(); ++i) {
						IServerUserItem* pUserItem = pTableFrame->GetTableUserItem(i);
						if (pUserItem == NULL) continue;
						if (pIServerUserItem->GetUserID() == pUserItem->GetUserID()) continue;  //过滤掉自己

						

						//坐下玩家是H5，广播本桌其他玩家给H5
						if (pIServerUserItem->GetUserInfo()->cbClientKind == CLIENT_KIND_WEBSOCKET) {
							WORD wBindIndex = pIServerUserItem->GetBindIndex();
							tagBindParameter * pBindParameter = GetBindParameter(wBindIndex);
							if (pBindParameter)
							{
								SendUserInfoPacketH5(pUserItem, pBindParameter->dwSocketID);  //别人发给自己
								CString cs;
								cs.Format(TEXT("【主命令[3],次命令[100]】|【同桌信息发自己】同桌是[%d],玩家自己[%d] "), pUserItem->GetUserID(), pIServerUserItem->GetUserID());
								m_ServerFileLog.Log(cs.GetBuffer(0));
							}

																						  //坐下玩家下发给同桌玩家
							WORD wTableUserBindIndex = pUserItem->GetBindIndex();
							tagBindParameter* pTableUserBindParam = GetBindParameter(wTableUserBindIndex);
							//同桌玩家必须是H5
							if (pTableUserBindParam)
							{
								SendUserInfoPacketH5(pIServerUserItem, pTableUserBindParam->dwSocketID); //自己发给别人
								CString cs;
								cs.Format(TEXT("【主命令[3],次命令[100]】|【自己信息发同桌】同桌[%d],玩家自己[%d] "), pUserItem->GetUserID(), pIServerUserItem->GetUserID());
								m_ServerFileLog.Log(cs.GetBuffer(0));
							}
						}
						else if (pIServerUserItem->IsAndroidUser() == true)
						{
							//自己发给别人
							WORD wTableUserBindIndex = pUserItem->GetBindIndex();
							tagBindParameter* pTableUserBindParam = GetBindParameter(wTableUserBindIndex);
							if (pTableUserBindParam) {
								SendUserInfoPacketH5(pIServerUserItem, pTableUserBindParam->dwSocketID);
								CString cs;
								cs.Format(TEXT("【主命令[3],次命令[100]】|【机器人发给别人】玩家自己[%d] "),pIServerUserItem->GetUserID());
								m_ServerFileLog.Log(cs.GetBuffer(0));
							}
						}
					}

					pIServerUserItem->SetEnterTable(TRUE); //进桌完成不再下发同桌玩家
				}
			} while (0);
					

		}
	}

	//3,玩家状态发给同桌玩家
	if ( pIServerUserItem->GetUserStatus() != US_NULL )
	{
		tagUserInfo* pUserInfo = pIServerUserItem->GetUserInfo();
		WORD wLastTableID = pUserInfo->wLastTableID;
		if (wLastTableID < 0 || wLastTableID >= m_pGameServiceOption->wTableCount) {
			I_TRACE(TEXT("error !!!"));
			return false;

		}

		GameServer::GSUserStatus gsUserStatus;
		gsUserStatus.set_userid(dwUserID);
		gsUserStatus.mutable_userstatus()->set_tableid((int32)wTableID);
		gsUserStatus.mutable_userstatus()->set_chairid((int32)wChairID);
		gsUserStatus.mutable_userstatus()->set_userstatus((GameServer::enumUserStatus)cbUserStatus);
		std::string serializeStr;
		gsUserStatus.SerializeToString(&serializeStr);
		if (pIServerUserItem->GetUserInfo()->cbClientKind == CLIENT_KIND_WEBSOCKET)
		{
			CString cs;
			cs.Format(TEXT("【主命令[3],次命令[102]】|【状态变化发自己】 玩家[%d] 状态[%d] 桌椅[%d-%d]"), pIServerUserItem->GetUserID(), cbUserStatus, wTableID, wChairID);
			m_ServerFileLog.Log(cs.GetBuffer(0));

			//发自己	
			SendData(pIServerUserItem, MDM_GR_USER, SUB_GR_USER_STATUS, (void*)serializeStr.c_str(), (WORD)serializeStr.size());
		}
		else if (pIServerUserItem->IsAndroidUser())
		{
			CMD_GR_UserStatus UserStatus;
			ZeroMemory(&UserStatus, sizeof(UserStatus));
			UserStatus.dwUserID = dwUserID;
			UserStatus.UserStatus.wTableID = wTableID;
			UserStatus.UserStatus.wChairID = wChairID;
			UserStatus.UserStatus.cbUserStatus = cbUserStatus;
			//发自己
			SendData(pIServerUserItem, MDM_GR_USER, SUB_GR_USER_STATUS, &UserStatus, sizeof(UserStatus));
		}


		//玩家状态发给同桌玩家
		if (cbNotifyClient == TRUE)
		{
			CTableFrame * pTableFrame = m_TableFrameArray[wLastTableID];
			for (int i = 0; i < pTableFrame->GetChairCount(); ++i) {
				if (i == pIServerUserItem->GetUserInfo()->wLastChairID) continue;  //过滤自己

				IServerUserItem* pUserItem = pTableFrame->GetTableUserItem(i);
				if (pUserItem == NULL) continue;

				CString cs;
				cs.Format(TEXT("【主命令[3],次命令[102]】|【状态变化发同桌】同桌[%d] 玩家[%d] 状态[%d] 桌椅[%d-%d]"), pUserItem->GetUserID(), pIServerUserItem->GetUserID(), cbUserStatus, wTableID, wChairID);
				m_ServerFileLog.Log(cs.GetBuffer(0));

				SendData(pUserItem, MDM_GR_USER, SUB_GR_USER_STATUS, (void*)serializeStr.c_str(), (WORD)serializeStr.size());
			}
		}
	}

	//离开判断
	if (pIServerUserItem->GetUserStatus()==US_NULL)
	{
		WORD wBindIndex=pIServerUserItem->GetBindIndex();
		tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
		
		//离开处理
		OnEventUserLogout(pIServerUserItem, 0L);

		if (pBindParameter!=NULL)
		{
			I_TRACE(TEXT("关闭连接 %s %d-%d,Status:%d,SocketID:%d"), pIServerUserItem->GetNickName(), pIServerUserItem->GetTableID(), pIServerUserItem->GetChairID(), pIServerUserItem->GetUserStatus(),pIServerUserItem->GetBindIndex());
			//绑定处理
 			if (pBindParameter->pIServerUserItem==pIServerUserItem)
 			{
 			 	pBindParameter->pIServerUserItem=NULL;
 			 }

			//中断网络  链接由客户端断开
// 			if (pBindParameter->dwSocketID!=0L)
// 			{
// 				if (LOWORD(pBindParameter->dwSocketID)>=INDEX_ANDROID)
// 				{
// 					//m_AndroidUserManager.DeleteAndroidUserItem(pBindParameter->dwSocketID);
// 				}
// 				else
// 				{
// 					
// 					m_pITCPNetworkEngine->ShutDownSocket(pBindParameter->dwSocketID);
// 				}
// 			}
		}
		
	}
	cs1.Format(TEXT("【状态变化结束】----------------"));
	m_ServerFileLog.Log(cs1.GetBuffer(0));
	return true;
}

//用户权限
bool CAttemperEngineSink::OnEventUserItemRight(IServerUserItem *pIServerUserItem, DWORD dwAddRight, DWORD dwRemoveRight,bool bGameRight)
{
	//效验参数
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//变量定义
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();

	DBR_GR_ManageUserRight ManageUserRight= {0};
	ManageUserRight.dwUserID = pUserInfo->dwUserID;
	ManageUserRight.dwAddRight = dwAddRight;
	ManageUserRight.dwRemoveRight = dwRemoveRight;
	ManageUserRight.bGameRight=bGameRight;

	//发送请求
	m_pIDBCorrespondManager->PostDataBaseRequest(ManageUserRight.dwUserID,DBR_GR_MANAGE_USER_RIGHT,0,&ManageUserRight,sizeof(ManageUserRight));

	return true;
}

//登录成功
bool CAttemperEngineSink::OnDBLogonSuccessH5(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//变量定义
	WORD wBindIndex = LOWORD(dwContextID);
	tagBindParameter * pBindParameter = GetBindParameter(wBindIndex);
	DBO_GR_LogonSuccess * pDBOLogonSuccess = (DBO_GR_LogonSuccess *)pData;

	//变量定义
	bool bAndroidUser = (wBindIndex >= INDEX_ANDROID);
	bool bMobileUser = (pBindParameter->cbClientKind == CLIENT_KIND_MOBILE);

	//被踢判断
	tagKickUser KickUser = { 0 };
	if (m_KickUserItemMap.Lookup(pDBOLogonSuccess->dwUserID, KickUser) == TRUE)
	{
		//时效判断
		DWORD dwCurrTime = (DWORD)time(NULL);
		//if (dwKickTime + VALID_TIME_KICK_BY_MANAGER > dwCurrTime)
		if (KickUser.dwStartKickTime + KickUser.dwLimitTime > dwCurrTime)
		{
			//发送失败
			CString cs;
			cs.Format(TEXT("您已被管理员请出房间,%d分钟内不能进入！"),KickUser.dwLimitTime/60);
			SendLogonFailure(cs.GetBuffer(0), 0, pBindParameter->dwSocketID);
			I_TRACE(cs.GetBuffer(0));
			//解除锁定
			PerformUnlockScore(pDBOLogonSuccess->dwUserID, pDBOLogonSuccess->dwInoutIndex, LER_SERVER_CONDITIONS);

			return true;
		}
		else
		{
			//移除玩家
			m_KickUserItemMap.RemoveKey(pDBOLogonSuccess->dwUserID);
		}
	}
	I_TRACE(TEXT("-----------------登录成功SocketID:%d----------"), dwContextID);
	g_Log.LogToFile(g_szLogRoot, TEXT("登录成功SocketID[%d]"), dwContextID);

	//下面要赋值，必须设为NULL
	if (pBindParameter->pIServerUserItem != NULL)
	{
		pBindParameter->pIServerUserItem = NULL;
	}

	if (pBindParameter->dwSocketID != dwContextID)
	{
		PerformUnlockScore(pDBOLogonSuccess->dwUserID, pDBOLogonSuccess->dwInoutIndex, LER_NORMAL);

		return true;
	}

	//切换判断
	IServerUserItem * pIServerUserItem = m_ServerUserManager.SearchUserItem(pDBOLogonSuccess->dwUserID);
	if (pIServerUserItem != NULL)
	{
		I_TRACE(TEXT("切换连接SocketID:%d"),dwContextID);
		//切换用户
		SwitchUserItemConnectH5(pIServerUserItem, pDBOLogonSuccess->szMachineID, wBindIndex);

		if (pIServerUserItem->IsAndroidUser() == false)
		{
			g_Log.LogToFile(g_szLogRoot, TEXT("\n UserID[%d]-[数据库信息] NickName[%s]:UserScore[%I64d]"), pDBOLogonSuccess->dwUserID, pDBOLogonSuccess->szNickName, pDBOLogonSuccess->lScore);
			g_Log.LogToFile(g_szLogRoot, TEXT("\n UserID[%d]-[内存信息] NickName[%s]:UserScore[%I64d]"), pIServerUserItem->GetUserID(), pIServerUserItem->GetNickName(), pIServerUserItem->GetUserScore());
		}

		return true;
	}

	//查找机器
	if (bAndroidUser == true && m_pGameServiceOption->wRoomType != 4)
	{
		//查找机器
		DWORD dwUserID = pDBOLogonSuccess->dwUserID;
		IAndroidUserItem * pIAndroidUserItem = m_AndroidUserManager.SearchAndroidUserItem(dwUserID, dwContextID);

		//修改积分

		if (pIAndroidUserItem != NULL && m_pGameServiceAttrib->wChairCount < MAX_CHAIR)
		{
			//设置机器人随身携带金币
			if (m_pGameServiceOption->wServerType != GAME_GENRE_MATCH&&GAME_GENRE_SCORE != m_pGameServiceOption->wServerType)
			{
				if (m_pGameServiceOption->lMinEnterScore <= 10000)
					pDBOLogonSuccess->lScore = (long long)((float)rand() / 32767.0f * (float)(10000) * 3);
				else pDBOLogonSuccess->lScore = (long long)((float)rand() / 32767.0f * (float)(m_pGameServiceOption->lMinEnterScore) * 3);
			}
			if (m_pGameServiceOption->lMinEnterScore != 0 && pDBOLogonSuccess->lScore <= m_pGameServiceOption->lMinEnterScore)
			{
				if (m_pGameServiceAttrib->wChairCount == MAX_CHAIR)
					pDBOLogonSuccess->lScore = pDBOLogonSuccess->lScore + m_pGameServiceOption->lMinEnterScore * 3;
				else
					pDBOLogonSuccess->lScore += m_pGameServiceOption->lMinEnterScore;
			}


			//大于最大分，带入最大分
			if (m_pGameServiceOption->lMaxEnterScore && pDBOLogonSuccess->lScore >= m_pGameServiceOption->lMaxEnterScore) pDBOLogonSuccess->lScore = m_pGameServiceOption->lMaxEnterScore;
		}

		if (pIAndroidUserItem != NULL && m_pGameServiceAttrib->wChairCount >= MAX_CHAIR)
		{
			//pDBOLogonSuccess->lScore = (long long)((float)rand()/32767.0f * (float)(1000000)*3);
			pDBOLogonSuccess->lScore = (long long)((float)rand() / 32767.0f * (float)(10000000) * 3);

		}
	}

	//最低分数
	if ((m_pGameServiceOption->lMinEnterScore != 0L) && (pDBOLogonSuccess->lScore < m_pGameServiceOption->lMinEnterScore))
	{
		I_TRACE(TEXT("分数不够SocketID:%d"), dwContextID);
		//发送失败
		TCHAR szMsg[128] = TEXT("");
		_sntprintf(szMsg, CountArray(szMsg), TEXT("当前房间准入%0.2f，您金币不足无法进入！"), double(m_pGameServiceOption->lMinEnterScore) / 1000);
		SendLogonFailure(szMsg, 0, pBindParameter->dwSocketID);
		//解除锁定
		PerformUnlockScore(pDBOLogonSuccess->dwUserID, pDBOLogonSuccess->dwInoutIndex, LER_SERVER_CONDITIONS);

		return true;
	}

	//满人判断
	WORD wMaxPlayer = m_pGameServiceOption->wMaxPlayer;
	DWORD dwOnlineCount = m_ServerUserManager.GetUserItemCount();
	I_TRACE(TEXT("房间最大人数:%d,保留人数[%d],在线人数[%d]"), wMaxPlayer, RESERVE_USER_COUNT, dwOnlineCount);
	if ((pDBOLogonSuccess->cbMasterOrder == 0) && (dwOnlineCount > (DWORD)(wMaxPlayer - RESERVE_USER_COUNT)))
	{
		I_TRACE(TEXT("满人判断SocketID:%d"), dwContextID);
		//发送失败
		SendLogonFailure(TEXT("抱歉，由于此房间已经人满，普通玩家不能继续进入了！"), 0, pBindParameter->dwSocketID);

		//解除锁定
		PerformUnlockScore(pDBOLogonSuccess->dwUserID, pDBOLogonSuccess->dwInoutIndex, LER_SERVER_FULL);

		if (bAndroidUser)
			m_AndroidUserManager.DeleteAndroidUserItem(pBindParameter->dwSocketID);

		return true;
	}


	//用户变量
	tagUserInfo UserInfo;
	tagUserInfoPlus UserInfoPlus;
	ZeroMemory(&UserInfo, sizeof(UserInfo));
	ZeroMemory(&UserInfoPlus, sizeof(UserInfoPlus));

	//属性资料
	UserInfo.wFaceID = pDBOLogonSuccess->wFaceID;
	UserInfo.dwUserID = pDBOLogonSuccess->dwUserID;
	UserInfo.dwGameID = pDBOLogonSuccess->dwGameID;
	UserInfo.dwGroupID = pDBOLogonSuccess->dwGroupID;
	UserInfo.dwCustomID = pDBOLogonSuccess->dwCustomID;

	lstrcpyn(UserInfo.szNickName, pDBOLogonSuccess->szNickName, CountArray(UserInfo.szNickName));

	//用户资料
	UserInfo.cbGender = pDBOLogonSuccess->cbGender;
	UserInfo.cbMemberOrder = pDBOLogonSuccess->cbMemberOrder;
	UserInfo.cbMasterOrder = pDBOLogonSuccess->cbMasterOrder;
	lstrcpyn(UserInfo.szGroupName, pDBOLogonSuccess->szGroupName, CountArray(UserInfo.szGroupName));
	lstrcpyn(UserInfo.szUnderWrite, pDBOLogonSuccess->szUnderWrite, CountArray(UserInfo.szUnderWrite));

	//状态设置
	UserInfo.cbUserStatus = US_FREE;
	UserInfo.wTableID = INVALID_TABLE;
	UserInfo.wChairID = INVALID_CHAIR;

	//积分信息
	UserInfo.lScore = pDBOLogonSuccess->lScore;
	UserInfo.lInsure = pDBOLogonSuccess->lInsure;
	UserInfo.lRechargeScore = pDBOLogonSuccess->lRechargeScore;
	UserInfo.lExchangeScore = pDBOLogonSuccess->lExchangeScore;
	UserInfo.lRevenue = pDBOLogonSuccess->lRevenue;
	UserInfo.dwWinCount = pDBOLogonSuccess->dwWinCount;
	UserInfo.dwLostCount = pDBOLogonSuccess->dwLostCount;
	UserInfo.dwDrawCount = pDBOLogonSuccess->dwDrawCount;
	UserInfo.dwFleeCount = pDBOLogonSuccess->dwFleeCount;
	UserInfo.dwUserMedal = pDBOLogonSuccess->dwUserMedal;
	UserInfo.dwExperience = pDBOLogonSuccess->dwExperience;
	UserInfo.lLoveLiness = pDBOLogonSuccess->lLoveLiness;
	UserInfo.wAvatarID = pDBOLogonSuccess->wAvatarID;
	//add new 连接类型
	UserInfo.cbClientKind = pBindParameter->cbClientKind;

	//登录信息
	UserInfoPlus.dwLogonTime = (DWORD)time(NULL);
	UserInfoPlus.dwInoutIndex = pDBOLogonSuccess->dwInoutIndex;

	//用户权限
	UserInfoPlus.dwUserRight = pDBOLogonSuccess->dwUserRight;
	UserInfoPlus.dwMasterRight = pDBOLogonSuccess->dwMasterRight;

	//辅助变量
	UserInfoPlus.bMobileUser = bMobileUser;
	UserInfoPlus.bAndroidUser = bAndroidUser;
	UserInfoPlus.lRestrictScore = m_pGameServiceOption->lRestrictScore;
	lstrcpyn(UserInfoPlus.szPassword, pDBOLogonSuccess->szPassword, CountArray(UserInfoPlus.szPassword));

	//连接信息
	UserInfoPlus.wBindIndex = wBindIndex;
	UserInfoPlus.dwClientAddr = pBindParameter->dwClientAddr;
	lstrcpyn(UserInfoPlus.szMachineID, pDBOLogonSuccess->szMachineID, CountArray(UserInfoPlus.szMachineID));

	//入场条件
	UserInfoPlus.lInitBlood = m_pGameServiceOption->dwBloodScore;
	UserInfoPlus.lEnterPower = m_pGameServiceOption->lMinEnterTiLi;
	UserInfoPlus.lEnterScore = m_pGameServiceOption->lMinEnterScore;
	UserInfoPlus.lMaxEnterScore = m_pGameServiceOption->lMaxEnterScore;
	UserInfoPlus.cbRoomType = (BYTE)m_pGameServiceOption->wRoomType;
	if (m_pGameServiceOption->dwRoomID)
		UserInfoPlus.cbBloodRoom = m_pGameServiceOption->wRoomKind;

	//激活用户
	m_ServerUserManager.InsertUserItem(&pIServerUserItem, UserInfo, UserInfoPlus);

	//错误判断
	if (pIServerUserItem == NULL)
	{
		//错误断言
		ASSERT_ERR(FALSE);

		//解除锁定
		PerformUnlockScore(pDBOLogonSuccess->dwUserID, pDBOLogonSuccess->dwInoutIndex, LER_SERVER_FULL);

		//断开用户
		if (bAndroidUser == true)
		{

			m_AndroidUserManager.DeleteAndroidUserItem(dwContextID);
		}
		else
		{
			m_pITCPNetworkEngine->ShutDownSocket(dwContextID);
		}

		return true;
	}

	//设置用户
	pBindParameter->pIServerUserItem = pIServerUserItem;

	//登录事件
	OnEventUserLogonH5(pIServerUserItem, false);

	//汇总用户
	if (m_bCollectUser == true)
	{
		//变量定义
		CMD_CS_C_UserEnter UserEnter;
		ZeroMemory(&UserEnter, sizeof(UserEnter));

		//设置变量
		UserEnter.dwUserID = pIServerUserItem->GetUserID();
		UserEnter.dwGameID = pIServerUserItem->GetGameID();
		lstrcpyn(UserEnter.szNickName, pIServerUserItem->GetNickName(), CountArray(UserEnter.szNickName));

		//辅助信息
		UserEnter.cbGender = pIServerUserItem->GetGender();
		UserEnter.cbMemberOrder = pIServerUserItem->GetMemberOrder();
		UserEnter.cbMasterOrder = pIServerUserItem->GetMasterOrder();

		//发送消息
		ASSERT_ERR(m_pITCPSocketService != NULL);
		m_pITCPSocketService->SendData(MDM_CS_USER_COLLECT, SUB_CS_C_USER_ENTER, &UserEnter, sizeof(UserEnter));
	}
	if (pIServerUserItem->IsAndroidUser() == false)
		g_Log.LogToFile(g_szLogRoot, TEXT("\n OnDBLogonSuccessH5 UserID[%d],NickName[%s]:UserScore[%I64d]"), pIServerUserItem->GetUserID(), pIServerUserItem->GetNickName(), pIServerUserItem->GetUserScore());
	return true;
}

//登录成功
bool CAttemperEngineSink::OnDBLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//变量定义
	WORD wBindIndex=LOWORD(dwContextID);
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
	DBO_GR_LogonSuccess * pDBOLogonSuccess=(DBO_GR_LogonSuccess *)pData;

	//下面要赋值，必须设为NULL
	if ( pBindParameter->pIServerUserItem!=NULL )
	{
		pBindParameter->pIServerUserItem=NULL;
	}

	if ( pBindParameter->dwSocketID!=dwContextID )
	{
		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_NORMAL);

		return true;
	}

	//变量定义
	bool bAndroidUser=(wBindIndex>=INDEX_ANDROID);
	bool bMobileUser=(pBindParameter->cbClientKind==CLIENT_KIND_MOBILE);

	//被踢判断
	//DWORD dwKickTime;
	//if(m_KickUserItemMap.Lookup(pDBOLogonSuccess->dwUserID,dwKickTime)==TRUE)
	//{
	//	//时效判断
	//	DWORD dwCurrTime = (DWORD)time(NULL);
	//	if(dwKickTime+VALID_TIME_KICK_BY_MANAGER > dwCurrTime)
	//	{
	//		//发送失败
	//		SendLogonFailure(TEXT("您已被管理员请出房间,1小时之内不能进入！"),0,pBindParameter->dwSocketID);

	//		//解除锁定
	//		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_SERVER_CONDITIONS);

	//		return true;
	//	}
	//	else
	//	{
	//		//移除玩家
	//		m_KickUserItemMap.RemoveKey(pDBOLogonSuccess->dwUserID);
	//	}
	//}

	//切换判断
	IServerUserItem * pIServerUserItem=m_ServerUserManager.SearchUserItem(pDBOLogonSuccess->dwUserID);
	if (pIServerUserItem!=NULL)
	{
		//切换用户
		SwitchUserItemConnect(pIServerUserItem,pDBOLogonSuccess->szMachineID,wBindIndex,pDBOLogonSuccess->cbDeviceType,pDBOLogonSuccess->wBehaviorFlags,pDBOLogonSuccess->wPageTableCount);

		//解除锁定 重连进来不能让玩家离开 网狐bug
		//PerformUnlockScore(pIServerUserItem->GetUserID(),pIServerUserItem->GetInoutIndex(),LER_USER_IMPACT);

		if ( pIServerUserItem->IsAndroidUser() == false)
		{
			g_Log.LogToFile(g_szLogRoot,TEXT("\n OnDBLogonSuccess1 UserID[%d],NickName[%s]:UserScore[%I64d]"),pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->szNickName,pDBOLogonSuccess->lScore);
			g_Log.LogToFile(g_szLogRoot,TEXT("\n OnDBLogonSuccess1 UserID[%d],NickName[%s]:UserScore[%I64d]"),pIServerUserItem->GetUserID(),pIServerUserItem->GetNickName(),pIServerUserItem->GetUserScore());
		}
		
		return true;
	}

	//查找机器
// 	if (bAndroidUser==true && m_pGameServiceOption->wRoomType != 4)
// 	{
// 		//查找机器
// 		DWORD dwUserID=pDBOLogonSuccess->dwUserID;
// 		IAndroidUserItem * pIAndroidUserItem=m_AndroidUserManager.SearchAndroidUserItem(dwUserID,dwContextID);
// 
// 		//修改积分
// 
// 		if (pIAndroidUserItem!=NULL && m_pGameServiceAttrib->wChairCount < MAX_CHAIR)
// 		{
// 			//设置机器人随身携带金币
// 				if (m_pGameServiceOption->wServerType!=GAME_GENRE_MATCH&&GAME_GENRE_SCORE!=m_pGameServiceOption->wServerType)
// 				{
// 					if ( m_pGameServiceOption->lMinEnterScore <= 10000 )
// 						pDBOLogonSuccess->lScore = (long long)((float)rand()/32767.0f * (float)(10000)*3);
// 					else pDBOLogonSuccess->lScore = (long long)((float)rand()/32767.0f * (float)(m_pGameServiceOption->lMinEnterScore)*3);
// 				}
// 				if ( m_pGameServiceOption->lMinEnterScore!=0 && pDBOLogonSuccess->lScore <= m_pGameServiceOption->lMinEnterScore)
// 				{
// 					if ( m_pGameServiceAttrib->wChairCount==MAX_CHAIR )
// 					 pDBOLogonSuccess->lScore= pDBOLogonSuccess->lScore+m_pGameServiceOption->lMinEnterScore*3;
// 					else
// 					 pDBOLogonSuccess->lScore+=m_pGameServiceOption->lMinEnterScore;
// 				}
// 				
// 
// 				//大于最大分，带入最大分
// 				//if (  m_pGameServiceOption->lMaxEnterScore && pDBOLogonSuccess->lScore >= m_pGameServiceOption->lMaxEnterScore ) pDBOLogonSuccess->lScore = m_pGameServiceOption->lMaxEnterScore;
// 		}
// 
// 		if (pIAndroidUserItem!=NULL && m_pGameServiceAttrib->wChairCount >= MAX_CHAIR)
// 		{
// 			//pDBOLogonSuccess->lScore = (long long)((float)rand()/32767.0f * (float)(1000000)*3);
// 			pDBOLogonSuccess->lScore = (long long)((float)rand()/32767.0f * (float)(10000000)*3);
// 
// 		}
// 	}

	//最低分数
	if ((m_pGameServiceOption->lMinEnterScore!=0L)&&(pDBOLogonSuccess->lScore<m_pGameServiceOption->lMinEnterScore))
	{
		//发送失败
		TCHAR szMsg[128]=TEXT("");
		_sntprintf(szMsg,CountArray(szMsg), TEXT("抱歉，您的游戏金币低于当前游戏房间的最低进入金币%0.2f，不能进入当前游戏房间！"), double(m_pGameServiceOption->lMinEnterScore)/1000);
		SendLogonFailure(szMsg,0,pBindParameter->dwSocketID);
		I_TRACE(TEXT("机器人 当前分[%I64d],准入分[%I64d]"),pDBOLogonSuccess->lScore, m_pGameServiceOption->lMinEnterScore);
		//解除锁定
		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_SERVER_CONDITIONS);

		return true;
	}

	//满人判断
	WORD wMaxPlayer=m_pGameServiceOption->wMaxPlayer;
	DWORD dwOnlineCount=m_ServerUserManager.GetUserItemCount();
	if ((pDBOLogonSuccess->cbMasterOrder==0)&&(dwOnlineCount>(DWORD)(wMaxPlayer-RESERVE_USER_COUNT)))
	{
		//发送失败
		SendLogonFailure(TEXT("抱歉，由于此房间已经人满，普通玩家不能继续进入了！"),0,pBindParameter->dwSocketID);

		//解除锁定
		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_SERVER_FULL);

		if ( bAndroidUser )
			m_AndroidUserManager.DeleteAndroidUserItem(pBindParameter->dwSocketID);

		return true;
	}


	//用户变量
	tagUserInfo UserInfo;
	tagUserInfoPlus UserInfoPlus;
	ZeroMemory(&UserInfo,sizeof(UserInfo));
	ZeroMemory(&UserInfoPlus,sizeof(UserInfoPlus));

	//属性资料
	UserInfo.wFaceID=pDBOLogonSuccess->wFaceID;
	UserInfo.dwUserID=pDBOLogonSuccess->dwUserID;
	UserInfo.dwGameID=pDBOLogonSuccess->dwGameID;
	UserInfo.dwGroupID=pDBOLogonSuccess->dwGroupID;
	UserInfo.dwCustomID=pDBOLogonSuccess->dwCustomID;
	
// 	if (CUserRight::CanKillOutUser(pDBOLogonSuccess->dwUserRight)==true )
// 	{
// 		TCHAR vc[32]={0};
// 		INT nTmp=0;
// 		for(int i=0;i<8;i++)
// 		{
// 			if(rand()%2==0)//数字
// 			{
// 				vc[i]=TEXT('0')+rand()%9+1;
// 			}
// 			else//字母
// 			{
// 				nTmp=rand()%26;
// 				while(nTmp==14)
// 				{
// 					nTmp=rand()%26;
// 				}			
// 				vc[i]=nTmp+65;	
// 			}
// 		}
// 		lstrcpyn(UserInfo.szNickName,vc,CountArray(UserInfo.szNickName));
// 		g_Log.LogToFile(g_szLogRoot,TEXT("\n OnDBLogonSuccess gm号nickName:%s"),UserInfo.szNickName);
// 	}
// 	else
		lstrcpyn(UserInfo.szNickName,pDBOLogonSuccess->szNickName,CountArray(UserInfo.szNickName));

	//用户资料
	UserInfo.cbGender=pDBOLogonSuccess->cbGender;
	UserInfo.cbMemberOrder=pDBOLogonSuccess->cbMemberOrder;
	UserInfo.cbMasterOrder=pDBOLogonSuccess->cbMasterOrder;
	lstrcpyn(UserInfo.szGroupName,pDBOLogonSuccess->szGroupName,CountArray(UserInfo.szGroupName));
	lstrcpyn(UserInfo.szUnderWrite,pDBOLogonSuccess->szUnderWrite,CountArray(UserInfo.szUnderWrite));

	//状态设置
	UserInfo.cbUserStatus=US_FREE;
	UserInfo.wTableID=INVALID_TABLE;
	UserInfo.wChairID=INVALID_CHAIR;

	//积分信息
	UserInfo.lScore=pDBOLogonSuccess->lScore;
	UserInfo.lInsure=pDBOLogonSuccess->lInsure;
	UserInfo.lRechargeScore = pDBOLogonSuccess->lRechargeScore;
	UserInfo.lExchangeScore = pDBOLogonSuccess->lExchangeScore;
	UserInfo.lRevenue = pDBOLogonSuccess->lRevenue;
	UserInfo.dwWinCount=pDBOLogonSuccess->dwWinCount;
	UserInfo.dwLostCount=pDBOLogonSuccess->dwLostCount;
	UserInfo.dwDrawCount=pDBOLogonSuccess->dwDrawCount;
	UserInfo.dwFleeCount=pDBOLogonSuccess->dwFleeCount;
	UserInfo.dwUserMedal=pDBOLogonSuccess->dwUserMedal;
	UserInfo.dwExperience=pDBOLogonSuccess->dwExperience;
	UserInfo.lLoveLiness=pDBOLogonSuccess->lLoveLiness;
	UserInfo.wAvatarID = pDBOLogonSuccess->wAvatarID;
	UserInfo.cbClientKind = pBindParameter->cbClientKind;
	//登录信息
	UserInfoPlus.dwLogonTime=(DWORD)time(NULL);
	UserInfoPlus.dwInoutIndex=pDBOLogonSuccess->dwInoutIndex;

	//用户权限
	UserInfoPlus.dwUserRight=pDBOLogonSuccess->dwUserRight;
	UserInfoPlus.dwMasterRight=pDBOLogonSuccess->dwMasterRight;

	//辅助变量
	UserInfoPlus.bMobileUser=bMobileUser;
	UserInfoPlus.bAndroidUser=bAndroidUser;
	UserInfoPlus.lRestrictScore=m_pGameServiceOption->lRestrictScore;
	lstrcpyn(UserInfoPlus.szPassword,pDBOLogonSuccess->szPassword,CountArray(UserInfoPlus.szPassword));

	//连接信息
	UserInfoPlus.wBindIndex=wBindIndex;
	UserInfoPlus.dwClientAddr=pBindParameter->dwClientAddr;
	lstrcpyn(UserInfoPlus.szMachineID,pDBOLogonSuccess->szMachineID,CountArray(UserInfoPlus.szMachineID));

	//入场条件
	UserInfoPlus.lInitBlood		= m_pGameServiceOption->dwBloodScore;
	UserInfoPlus.lEnterPower	= m_pGameServiceOption->lMinEnterTiLi;
	UserInfoPlus.lEnterScore	= m_pGameServiceOption->lMinEnterScore;
	UserInfoPlus.lMaxEnterScore	= m_pGameServiceOption->lMaxEnterScore;
	UserInfoPlus.cbRoomType		= (BYTE)m_pGameServiceOption->wRoomType;
	if ( m_pGameServiceOption->dwRoomID )
	UserInfoPlus.cbBloodRoom	= m_pGameServiceOption->wRoomKind;

	//激活用户
	m_ServerUserManager.InsertUserItem(&pIServerUserItem,UserInfo,UserInfoPlus);

	//错误判断
	if (pIServerUserItem==NULL)
	{
		//错误断言
		ASSERT_ERR(FALSE);

		//解除锁定
		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_SERVER_FULL);

		//断开用户
		if (bAndroidUser==true)
		{
			
			m_AndroidUserManager.DeleteAndroidUserItem(dwContextID);
		}
		else
		{
			m_pITCPNetworkEngine->ShutDownSocket(dwContextID);
		}

		return true;
	}

	//设置用户
	pBindParameter->pIServerUserItem=pIServerUserItem;

	//登录事件
	OnEventUserLogon(pIServerUserItem,false);

	//汇总用户
	if (m_bCollectUser==true)
	{
		//变量定义
		CMD_CS_C_UserEnter UserEnter;
		ZeroMemory(&UserEnter,sizeof(UserEnter));

		//设置变量
		UserEnter.dwUserID=pIServerUserItem->GetUserID();
		UserEnter.dwGameID=pIServerUserItem->GetGameID();
		lstrcpyn(UserEnter.szNickName,pIServerUserItem->GetNickName(),CountArray(UserEnter.szNickName));

		//辅助信息
		UserEnter.cbGender=pIServerUserItem->GetGender();
		UserEnter.cbMemberOrder=pIServerUserItem->GetMemberOrder();
		UserEnter.cbMasterOrder=pIServerUserItem->GetMasterOrder();

		//发送消息
		ASSERT_ERR(m_pITCPSocketService!=NULL);
		m_pITCPSocketService->SendData(MDM_CS_USER_COLLECT,SUB_CS_C_USER_ENTER,&UserEnter,sizeof(UserEnter));
	}
	if ( pIServerUserItem->IsAndroidUser() == false)
	g_Log.LogToFile(g_szLogRoot,TEXT("\n OnDBLogonSuccess2 UserID[%d],NickName[%s]:UserScore[%I64d]"),pIServerUserItem->GetUserID(),pIServerUserItem->GetNickName(),pIServerUserItem->GetUserScore());
	else {
		//I_TRACE(TEXT("机器人[%d],UserID[%d],分数[%I64d]"), pIServerUserItem->IsAndroidUser(),pIServerUserItem->GetUserID(), pIServerUserItem->GetUserScore());
	}
	return true;
}

//登录失败
bool CAttemperEngineSink::OnDBLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	tagBindParameter * pBindParameter=GetBindParameter(LOWORD(dwContextID));
	if ((pBindParameter->dwSocketID!=dwContextID)/*||(pBindParameter->pIServerUserItem!=NULL)*/) return true;

	

	//发送错误
	DBO_GR_LogonFailure * pLogonFailure=(DBO_GR_LogonFailure *)pData;
	//SendLogonFailure(pLogonFailure->szDescribeString,pLogonFailure->lResultCode,dwContextID);
	//下发登录失败消息

	if (pBindParameter && pBindParameter->cbClientKind == CLIENT_KIND_WEBSOCKET)
	{
		GameServer::GSLogonFailure gsLogonFailure;
		gsLogonFailure.set_errorcode(pLogonFailure->lResultCode);
		gsLogonFailure.set_describestring(Utils::ConvertFromUtf16ToUtf8(pLogonFailure->szDescribeString));
		std::string serializeStr;
		gsLogonFailure.SerializeToString(&serializeStr);
		SendData(pBindParameter->dwSocketID, MDM_GR_LOGON, SUB_GR_LOGON_FAILURE, (void*)serializeStr.c_str(), (WORD)serializeStr.size());
		return true;
	}

	//变量定义
	CMD_GR_LogonFailure LogonFailure;
	ZeroMemory(&LogonFailure,sizeof(LogonFailure));

	//构造数据
	LogonFailure.lErrorCode=pLogonFailure->lResultCode;
	lstrcpyn(LogonFailure.szDescribeString,pLogonFailure->szDescribeString,CountArray(LogonFailure.szDescribeString));

	//数据属性
	WORD wDataSize2=CountStringBuffer(LogonFailure.szDescribeString);
	WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);

	//发送数据
	SendData(dwContextID,MDM_GR_LOGON,SUB_GR_LOGON_FAILURE,&LogonFailure,wHeadSize+wDataSize2);

	//断开连接
	if (LOWORD(dwContextID)>=INDEX_ANDROID)
	{
		//CTraceService::TraceString(TEXT("OnDBLogonFailure 机器人登录失败"),TraceLevel_Info);
		I_TRACE(TEXT("OnDBLogonFailure 机器人登录失败"));
		m_AndroidUserManager.DeleteAndroidUserItem(dwContextID);
	}
	else
	{
		g_Log.LogToFile(g_szLogRoot,TEXT("\nOnDBLogonFailure "));
		m_pITCPNetworkEngine->ShutDownSocket(dwContextID);
	}
	
	return true;
}

//配置信息
bool CAttemperEngineSink::OnDBGameParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT_ERR(wDataSize==sizeof(DBO_GR_GameParameter));
	if (wDataSize!=sizeof(DBO_GR_GameParameter)) return false;

	//变量定义
	DBO_GR_GameParameter * pGameParameter=(DBO_GR_GameParameter *)pData;

	//汇率信息
	m_pGameParameter->wMedalRate=pGameParameter->wMedalRate;
	m_pGameParameter->wRevenueRate=pGameParameter->wRevenueRate;

	//版本信息
	m_pGameParameter->dwClientVersion=pGameParameter->dwClientVersion;
	m_pGameParameter->dwServerVersion=pGameParameter->dwServerVersion;

	//版本效验
	if (VERSION_EFFICACY==TRUE)
	{
		//版本判断
		bool bVersionInvalid=false;
		if (m_pGameParameter->dwClientVersion!=m_pGameServiceAttrib->dwClientVersion) bVersionInvalid=true;
		if (m_pGameParameter->dwServerVersion!=m_pGameServiceAttrib->dwServerVersion) bVersionInvalid=true;

		//提示信息
		if (bVersionInvalid==true)
		{
			TRACE_LOG(TEXT("平台数据库服务组件版本注册信息与当前组件版本信息不一致"),TraceLevel_Warning);
		}
	}

	return true;
}

//列表信息
bool CAttemperEngineSink::OnDBGameColumnInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//变量定义
	DBO_GR_GameColumnInfo * pGameColumnInfo=(DBO_GR_GameColumnInfo *)pData;
	WORD wHeadSize=sizeof(DBO_GR_GameColumnInfo)-sizeof(pGameColumnInfo->ColumnItemInfo);

	//效验参数
	ASSERT_ERR((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pGameColumnInfo->cbColumnCount*sizeof(pGameColumnInfo->ColumnItemInfo[0]))));
	if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pGameColumnInfo->cbColumnCount*sizeof(pGameColumnInfo->ColumnItemInfo[0])))) return false;

	//数据处理
	if (pGameColumnInfo->cbColumnCount==0)
	{
		//默认列表
	}
	else
	{
		//拷贝数据
		m_DataConfigColumn.cbColumnCount=pGameColumnInfo->cbColumnCount;
		CopyMemory(m_DataConfigColumn.ColumnItem,pGameColumnInfo->ColumnItemInfo,pGameColumnInfo->cbColumnCount*sizeof(pGameColumnInfo->ColumnItemInfo[0]));
	}

	return true;
}

//机器信息
bool CAttemperEngineSink::OnDBGameAndroidInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//变量定义
	DBO_GR_GameAndroidInfo * pGameAndroidInfo=(DBO_GR_GameAndroidInfo *)pData;
	WORD wHeadSize=sizeof(DBO_GR_GameAndroidInfo)-sizeof(pGameAndroidInfo->AndroidParameter);

	//效验参数
	ASSERT_ERR((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pGameAndroidInfo->wAndroidCount*sizeof(pGameAndroidInfo->AndroidParameter[0]))));
	if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pGameAndroidInfo->wAndroidCount*sizeof(pGameAndroidInfo->AndroidParameter[0])))) return false;

	//设置机器
	if (pGameAndroidInfo->lResultCode==DB_SUCCESS)
	{
		m_AndroidUserManager.SetAndroidStock(pGameAndroidInfo->AndroidParameter,pGameAndroidInfo->wAndroidCount);
	}

	return true;
}

//刷新通用属性
bool CAttemperEngineSink::OnDBUpdateAttribute(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_UPDATE_ATTRIBUTE,pData,wDataSize);

	return true;
}

//申请机器人
bool CAttemperEngineSink::OnDBAndroidApply(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	DBO_GR_GameAndroidApply * pGameAndroidInfo=(DBO_GR_GameAndroidApply *)pData;
	WORD wHeadSize=sizeof(DBO_GR_GameAndroidApply)-sizeof(pGameAndroidInfo->AndroidParameter);

	//效验参数
	ASSERT_ERR((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pGameAndroidInfo->wAndroidCount*sizeof(pGameAndroidInfo->AndroidParameter[0]))));
	if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pGameAndroidInfo->wAndroidCount*sizeof(pGameAndroidInfo->AndroidParameter[0])))) return false;

	if (pGameAndroidInfo->lResultCode == DB_SUCCESS)
	{
		m_AndroidUserManager.SetAndroidStock(pGameAndroidInfo->AndroidParameter,pGameAndroidInfo->wAndroidCount);
	}

	return true;
}

bool CAttemperEngineSink::OnDBAndroidReleaseAndDelete(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	DBO_GR_GameAndriodRelease* pAndroid = (DBO_GR_GameAndriodRelease *)pData;

	if (wDataSize != sizeof DBO_GR_GameAndriodRelease) return false;

	tagBindParameter* pTagBind = GetBindParameter(pAndroid->wBindId);

	m_AndroidUserManager.DeleteAndroidUserItem(pTagBind->dwSocketID);

	return true;
}

//道具信息
bool CAttemperEngineSink::OnDBGamePropertyInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//变量定义
	DBO_GR_GamePropertyInfo * pGamePropertyInfo=(DBO_GR_GamePropertyInfo *)pData;
	WORD wHeadSize=sizeof(DBO_GR_GamePropertyInfo)-sizeof(pGamePropertyInfo->PropertyInfo);

	//效验参数
	ASSERT_ERR((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pGamePropertyInfo->cbPropertyCount*sizeof(pGamePropertyInfo->PropertyInfo[0]))));
	if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pGamePropertyInfo->cbPropertyCount*sizeof(pGamePropertyInfo->PropertyInfo[0])))) return false;

	//获取状态
	CServiceUnits * pServiceUnits=CServiceUnits::g_pServiceUnits;
	enServiceStatus ServiceStatus=pServiceUnits->GetServiceStatus();

	//设置道具
	if (pGamePropertyInfo->lResultCode==DB_SUCCESS)
	{
		//设置管理
		m_GamePropertyManager.SetGamePropertyInfo(pGamePropertyInfo->PropertyInfo,pGamePropertyInfo->cbPropertyCount);

		//拷贝数据
		m_DataConfigProperty.cbPropertyCount=pGamePropertyInfo->cbPropertyCount;
		CopyMemory(m_DataConfigProperty.PropertyInfo,pGamePropertyInfo->PropertyInfo,pGamePropertyInfo->cbPropertyCount*sizeof(pGamePropertyInfo->PropertyInfo[0]));
	}

	//事件通知
	if (ServiceStatus!=ServiceStatus_Service && m_pGameServiceOption->wServerType != GAME_GENRE_MATCH)
	{
		CP_ControlResult ControlResult;
		ControlResult.cbSuccess=ER_SUCCESS;
		SendUIControlPacket(UI_SERVICE_CONFIG_RESULT,&ControlResult,sizeof(ControlResult));
	}

	return true;
}

//银行信息
bool CAttemperEngineSink::OnDBUserInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	tagBindParameter * pBindParameter=GetBindParameter(LOWORD(dwContextID));
	if ((pBindParameter->dwSocketID!=dwContextID)||(pBindParameter->pIServerUserItem==NULL)) return true;

	//获取用户
	ASSERT_ERR(GetBindUserItem(LOWORD(dwContextID))!=NULL);
	IServerUserItem * pIServerUserItem=GetBindUserItem(LOWORD(dwContextID));

	//变量定义
	DBO_GR_UserInsureInfo * pUserInsureInfo=(DBO_GR_UserInsureInfo *)pData;

	//变量定义
	CMD_GR_S_UserInsureInfo UserInsureInfo;
	ZeroMemory(&UserInsureInfo,sizeof(UserInsureInfo));

	//构造数据
	UserInsureInfo.cbActivityGame=pUserInsureInfo->cbActivityGame;
	UserInsureInfo.wRevenueTake=pUserInsureInfo->wRevenueTake;
	UserInsureInfo.wRevenueTransfer=pUserInsureInfo->wRevenueTransfer;
	UserInsureInfo.wServerID=pUserInsureInfo->wServerID;
	UserInsureInfo.lUserInsure=pUserInsureInfo->lUserInsure;
	UserInsureInfo.lUserScore+=pIServerUserItem->GetUserScore();
	UserInsureInfo.lUserScore+=pIServerUserItem->GetTrusteeScore();
	UserInsureInfo.lUserScore+=pIServerUserItem->GetFrozenedScore();
	UserInsureInfo.lTransferPrerequisite=pUserInsureInfo->lTransferPrerequisite;

	//发送数据
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GR_INSURE,SUB_GR_USER_INSURE_INFO,&UserInsureInfo,sizeof(UserInsureInfo));

	return true;
}

//银行成功
bool CAttemperEngineSink::OnDBUserInsureSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	tagBindParameter * pBindParameter=GetBindParameter(LOWORD(dwContextID));
	if ((pBindParameter->dwSocketID!=dwContextID)||(pBindParameter->pIServerUserItem==NULL)) return true;

	//获取用户
	ASSERT_ERR(GetBindUserItem(LOWORD(dwContextID))!=NULL);
	IServerUserItem * pIServerUserItem=GetBindUserItem(LOWORD(dwContextID));

	//变量定义
	DBO_GR_UserInsureSuccess * pUserInsureSuccess=(DBO_GR_UserInsureSuccess *)pData;

	//变量定义
	SCORE lFrozenedScore=pUserInsureSuccess->lFrozenedScore;
	SCORE lInsureRevenue=pUserInsureSuccess->lInsureRevenue;
	SCORE lVariationScore=pUserInsureSuccess->lVariationScore;
	SCORE lVariationInsure=pUserInsureSuccess->lVariationInsure;

	//解冻积分
	if ((lFrozenedScore>0L)&&(pIServerUserItem->UnFrozenedUserScore(lFrozenedScore)==false))
	{
		ASSERT_ERR(FALSE);
		return false;
	}

	//银行操作
	if (pIServerUserItem->ModifyUserInsure(lVariationScore,lVariationInsure,lInsureRevenue)==false)
	{
		ASSERT_ERR(FALSE);
		return false;
	}

	//变量定义
	CMD_GR_S_UserInsureSuccess UserInsureSuccess;
	ZeroMemory(&UserInsureSuccess,sizeof(UserInsureSuccess));

	//构造变量
	UserInsureSuccess.cbActivityGame=pUserInsureSuccess->cbActivityGame;
	UserInsureSuccess.lUserScore=pIServerUserItem->GetUserScore()+pIServerUserItem->GetTrusteeScore();
	UserInsureSuccess.lUserInsure=pUserInsureSuccess->lSourceInsure+pUserInsureSuccess->lVariationInsure;
	lstrcpyn(UserInsureSuccess.szDescribeString,pUserInsureSuccess->szDescribeString,CountArray(UserInsureSuccess.szDescribeString));

	//发送数据
	WORD wDescribe=CountStringBuffer(UserInsureSuccess.szDescribeString);
	WORD wHeadSize=sizeof(UserInsureSuccess)-sizeof(UserInsureSuccess.szDescribeString);
	SendData(pIServerUserItem,MDM_GR_INSURE,SUB_GR_USER_INSURE_SUCCESS,&UserInsureSuccess,wHeadSize+wDescribe);

	return true;
}

//操作失败
bool CAttemperEngineSink::OnDBUserInsureFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	tagBindParameter * pBindParameter=GetBindParameter(LOWORD(dwContextID));
	if ((pBindParameter->dwSocketID!=dwContextID)||(pBindParameter->pIServerUserItem==NULL)) return true;

	//获取用户
	WORD wBindIndex=LOWORD(dwContextID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//发送错误
	DBO_GR_UserInsureFailure * pUserInsureFailure=(DBO_GR_UserInsureFailure *)pData;
	SendInsureFailure(pIServerUserItem,pUserInsureFailure->szDescribeString,pUserInsureFailure->lResultCode,pUserInsureFailure->cbActivityGame);

	//解冻积分
	if ((pUserInsureFailure->lFrozenedScore>0L)&&(pIServerUserItem->UnFrozenedUserScore(pUserInsureFailure->lFrozenedScore)==false))
	{
		ASSERT_ERR(FALSE);
		return false;
	}

	return true;
}

//用户信息
bool CAttemperEngineSink::OnDBUserInsureUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	tagBindParameter * pBindParameter=GetBindParameter(LOWORD(dwContextID));
	if ((pBindParameter->dwSocketID!=dwContextID)||(pBindParameter->pIServerUserItem==NULL)) return true;

	//获取用户
	ASSERT_ERR(GetBindUserItem(LOWORD(dwContextID))!=NULL);
	IServerUserItem * pIServerUserItem=GetBindUserItem(LOWORD(dwContextID));

	//变量定义
	DBO_GR_UserTransferUserInfo * pTransferUserInfo=(DBO_GR_UserTransferUserInfo *)pData;

	//变量定义
	CMD_GR_S_UserTransferUserInfo UserTransferUserInfo;
	ZeroMemory(&UserTransferUserInfo,sizeof(UserTransferUserInfo));

	//构造变量
	UserTransferUserInfo.cbActivityGame=pTransferUserInfo->cbActivityGame;
	UserTransferUserInfo.dwTargetGameID=pTransferUserInfo->dwGameID;
	lstrcpyn(UserTransferUserInfo.szNickName,pTransferUserInfo->szNickName,CountArray(UserTransferUserInfo.szNickName));

	//发送数据
	SendData(pIServerUserItem,MDM_GR_INSURE,SUB_GR_USER_TRANSFER_USER_INFO,&UserTransferUserInfo,sizeof(UserTransferUserInfo));

	return true;
}

//系统消息
bool CAttemperEngineSink::OnDBSystemMessage(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//参数校验
	ASSERT_ERR(wDataSize==sizeof(DBR_GR_SystemMessage));
	if(wDataSize!=sizeof(DBR_GR_SystemMessage)) return false;

	//提取数据
	DBR_GR_SystemMessage * pSystemMessage = (DBR_GR_SystemMessage *)pData;
	if(pSystemMessage==NULL) return false;

	//重复判断
	POSITION pos = m_SystemMessageList.GetHeadPosition();
	while(pos != NULL)
	{
		POSITION tempPos = pos;
		tagSystemMessage *pTagSystemMessage = m_SystemMessageList.GetNext(pos);
		if(pTagSystemMessage->SystemMessage.cbMessageID == pSystemMessage->cbMessageID)
		{
			//更新数据
			pTagSystemMessage->dwLastTime=0;
			CopyMemory(&pTagSystemMessage->SystemMessage, pSystemMessage, sizeof(DBR_GR_SystemMessage));

			return true;
		}
	}

	//定于变量
	tagSystemMessage  *pSendMessage=new tagSystemMessage;
	ZeroMemory(pSendMessage, sizeof(tagSystemMessage));

	//设置变量
	CopyMemory(&pSendMessage->SystemMessage,pSystemMessage, sizeof(DBR_GR_SystemMessage));

	//记录消息
	m_SystemMessageList.AddTail(pSendMessage);

	return true;
}

//加载敏感词
bool CAttemperEngineSink::OnDBSensitiveWords(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//开始加载
	if(dwContextID==0xfffe)
	{
		m_WordsFilter.ResetSensitiveWordArray();
		m_pITimerEngine->KillTimer(IDI_LOAD_SENSITIVE_WORD);
		return true;			
	}

	//加载完成
	if(dwContextID==0xffff)
	{
		m_WordsFilter.FinishAdd();
		return true;
	}

	//加载敏感词
	const TCHAR *pWords=(const TCHAR*)pData;
	m_WordsFilter.AddSensitiveWords(pWords);
	return true;
}

//注册事件
bool CAttemperEngineSink::OnTCPSocketMainRegister(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_REGISTER_FAILURE:		//注册失败
		{
			//变量定义
			CMD_CS_S_RegisterFailure * pRegisterFailure=(CMD_CS_S_RegisterFailure *)pData;

			//效验参数
			ASSERT_ERR(wDataSize>=(sizeof(CMD_CS_S_RegisterFailure)-sizeof(pRegisterFailure->szDescribeString)));
			if (wDataSize<(sizeof(CMD_CS_S_RegisterFailure)-sizeof(pRegisterFailure->szDescribeString))) return false;

			//关闭处理
			m_bNeekCorrespond=false;
			m_pITCPSocketService->CloseSocket();

			//显示消息
			if (lstrlen(pRegisterFailure->szDescribeString)>0)
			{
				TRACE_LOG(pRegisterFailure->szDescribeString,TraceLevel_Exception);
			}

			//事件通知
			CP_ControlResult ControlResult;
			ControlResult.cbSuccess=ER_FAILURE;
			SendUIControlPacket(UI_CORRESPOND_RESULT,&ControlResult,sizeof(ControlResult));

			return true;
		}
	}

	return true;
}

//列表事件
bool CAttemperEngineSink::OnTCPSocketMainServiceInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_SERVER_INFO:		//房间信息
		{
			//废弃列表
			m_ServerListManager.DisuseServerItem();

			return true;
		}
	case SUB_CS_S_SERVER_ONLINE:	//房间人数
		{
			//效验参数
			ASSERT_ERR(wDataSize==sizeof(CMD_CS_S_ServerOnLine));
			if (wDataSize!=sizeof(CMD_CS_S_ServerOnLine)) return false;

			//变量定义
			CMD_CS_S_ServerOnLine * pServerOnLine=(CMD_CS_S_ServerOnLine *)pData;

			//查找房间
			CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(pServerOnLine->wServerID);

			//设置人数
			if (pGameServerItem!=NULL)
			{
				pGameServerItem->m_GameServer.dwOnLineCount=pServerOnLine->dwOnLineCount;
			}

			return true;
		}
	case SUB_CS_S_SERVER_INSERT:	//房间插入
		{
			//效验参数
			ASSERT_ERR(wDataSize%sizeof(tagGameServer)==0);
			if (wDataSize%sizeof(tagGameServer)!=0) return false;

			//变量定义
			WORD wItemCount=wDataSize/sizeof(tagGameServer);
			tagGameServer * pGameServer=(tagGameServer *)pData;

			//更新数据
			for (WORD i=0;i<wItemCount;i++)
			{
				m_ServerListManager.InsertGameServer(pGameServer++);
			}

			return true;
		}
	case SUB_CS_S_SERVER_MODIFY:	//房间修改
		{
			//效验参数
			ASSERT_ERR(wDataSize==sizeof(CMD_CS_S_ServerModify));
			if (wDataSize!=sizeof(CMD_CS_S_ServerModify)) return false;

			//变量定义
			CMD_CS_S_ServerModify * pServerModify=(CMD_CS_S_ServerModify *)pData;

			//查找房间
			CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(pServerModify->wServerID);

			//设置房间
			if (pGameServerItem!=NULL)
			{
				pGameServerItem->m_GameServer.wNodeID=pServerModify->wNodeID;
				pGameServerItem->m_GameServer.wSortID=pServerModify->wSortID;
				pGameServerItem->m_GameServer.wServerPort=pServerModify->wServerPort;
				pGameServerItem->m_GameServer.dwOnLineCount=pServerModify->dwOnLineCount;
				pGameServerItem->m_GameServer.dwFullCount=pServerModify->dwFullCount;
				lstrcpyn(pGameServerItem->m_GameServer.szServerName,pServerModify->szServerName,CountArray(pGameServerItem->m_GameServer.szServerName));
				lstrcpyn(pGameServerItem->m_GameServer.szServerAddr,pServerModify->szServerAddr,CountArray(pGameServerItem->m_GameServer.szServerAddr));
			}

			return true;
		}
	case SUB_CS_S_SERVER_REMOVE:	//房间删除
		{
			//效验参数
			ASSERT_ERR(wDataSize==sizeof(CMD_CS_S_ServerRemove));
			if (wDataSize!=sizeof(CMD_CS_S_ServerRemove)) return false;

			//变量定义
			CMD_CS_S_ServerRemove * pServerRemove=(CMD_CS_S_ServerRemove *)pData;

			//变量定义
			m_ServerListManager.DeleteGameServer(pServerRemove->wServerID);

			return true;
		}
	case SUB_CS_S_SERVER_FINISH:	//房间完成
		{
			//清理列表
			m_ServerListManager.CleanServerItem();

			//事件处理
			CP_ControlResult ControlResult;
			ControlResult.cbSuccess=ER_SUCCESS;
			SendUIControlPacket(UI_CORRESPOND_RESULT,&ControlResult,sizeof(ControlResult));

			return true;
		}
	}

	return true;
}

//汇总事件
bool CAttemperEngineSink::OnTCPSocketMainUserCollect(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_COLLECT_REQUEST:	//用户汇总
		{
			//变量定义
			CMD_CS_C_UserEnter UserEnter;
			ZeroMemory(&UserEnter,sizeof(UserEnter));

			//发送用户
			WORD wIndex=0;
			do
			{
				//获取用户
				IServerUserItem * pIServerUserItem=m_ServerUserManager.EnumUserItem(wIndex++);
				if (pIServerUserItem==NULL) break;

				//设置变量
				UserEnter.dwUserID=pIServerUserItem->GetUserID();
				UserEnter.dwGameID=pIServerUserItem->GetGameID();
				lstrcpyn(UserEnter.szNickName,pIServerUserItem->GetNickName(),CountArray(UserEnter.szNickName));

				//辅助信息
				UserEnter.cbGender=pIServerUserItem->GetGender();
				UserEnter.cbMemberOrder=pIServerUserItem->GetMemberOrder();
				UserEnter.cbMasterOrder=pIServerUserItem->GetMasterOrder();

				//发送数据
				ASSERT_ERR(m_pITCPSocketService!=NULL);
				m_pITCPSocketService->SendData(MDM_CS_USER_COLLECT,SUB_CS_C_USER_ENTER,&UserEnter,sizeof(UserEnter));

			} while (true);

			//汇报完成
			m_bCollectUser=true;
			m_pITCPSocketService->SendData(MDM_CS_USER_COLLECT,SUB_CS_C_USER_FINISH);

			return true;
		}
	}

	return true;
}

//管理服务
bool CAttemperEngineSink::OnTCPSocketMainManagerService(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_SYSTEM_MESSAGE:	//系统消息
		{
			//消息处理
			SendSystemMessage((CMD_GR_SendMessage *)pData, wDataSize);

			return true;
		}
	case SUB_CS_S_PROPERTY_TRUMPET:  //喇叭消息
		{
			//发送数据
			m_pITCPNetworkEngine->SendDataBatch(MDM_GR_USER,SUB_GR_PROPERTY_TRUMPET,pData,wDataSize,BG_COMPUTER);

			return true;
		}
	case SUB_CS_S_GLAD_MESSAGE:		//喜报消息
		{
			//D_TRACE(TEXT("游戏服务器收到喜报消息"));

			//效验参数
			ASSERT(wDataSize==sizeof(CMD_CS_S_SendGladMsg));
			if (wDataSize!=sizeof(CMD_CS_S_SendGladMsg)) return false;

			//变量定义
			CMD_CS_S_SendGladMsg * pGladMsg=(CMD_CS_S_SendGladMsg *)pData;

			CMD_GR_SendGladMsg gladMsg;
			CopyMemory(&gladMsg, pGladMsg, sizeof(gladMsg));
			//发送数据
			m_pITCPNetworkEngine->SendDataBatch(MDM_GR_USER,SUB_GR_GLAD_MESSAGE,&gladMsg, sizeof(gladMsg),0);

			return true;
		}
	}

	return true;
}

//登录处理
bool CAttemperEngineSink::OnTCPNetworkMainLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GR_LOGON_USERID:		//I D 登录
		{
			return OnTCPNetworkSubLogonUserID(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_LOGON_MOBILE:		//手机登录
		{
			return OnTCPNetworkSubLogonMobile(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_LOGON_ACCOUNTS:		//帐号登录
		{
			return OnTCPNetworkSubLogonAccounts(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_LOGON_TOKEN:
		{
			return OnTCPNetworkSubLogonTokenID(pData, wDataSize, dwSocketID);
		}
	case SUB_GR_LOGON_TOKEN_LUA:
		{
			return OnTCPNetworkSubLogonTokenIDByLua(pData, wDataSize, dwSocketID);
		}
	}

	return true;
}

//用户处理
bool CAttemperEngineSink::OnTCPNetworkMainUser(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GR_USER_LOOKON:		//用户旁观
		{
			return OnTCPNetworkSubUserLookon(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_USER_SITDOWN:		//用户坐下
		{
			return OnTCPNetworkSubUserSitDown(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_USER_SITDOWN_H5:   //H5坐下
		{
			return OnTCPNetworkSubUserSitDownH5(pData, wDataSize, dwSocketID);
		}
	case SUB_GR_USER_STANDUP:		//用户起立
		{
			return OnTCPNetworkSubUserStandUp(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_USER_STANDUP_H5:	//H5起来
		{
			return OnTCPNetworkSubUserStandUpH5(pData, wDataSize, dwSocketID);
		}
	case SUB_GR_USER_CHAIR_REQ:    //用户换桌
		{
			return OnTCPNetworkSubUserChangeTable(pData, wDataSize, dwSocketID);
		}
	case SUB_GR_USER_CHANGE_TABLE_H5:  //H5换桌
		{
			return OnTCPNetworkSubUserChangeTableH5(pData, wDataSize, dwSocketID);
		}
	case SUB_GR_USER_CHAIR_INFO_REQ: //请求椅子用户信息
		{
			return OnTCPNetworkSubChairUserInfoReq(pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//银行处理
bool CAttemperEngineSink::OnTCPNetworkMainInsure(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GR_QUERY_INSURE_INFO:		//银行查询
		{
			return OnTCPNetworkSubQueryInsureInfo(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_SAVE_SCORE_REQUEST:		//存款请求
		{
			return OnTCPNetworkSubSaveScoreRequest(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_TAKE_SCORE_REQUEST:		//取款请求
		{
			return OnTCPNetworkSubTakeScoreRequest(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_TRANSFER_SCORE_REQUEST:	//转账请求
		{
			return OnTCPNetworkSubTransferScoreRequest(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_QUERY_USER_INFO_REQUEST:	//查询用户
		{
			return OnTCPNetworkSubQueryUserInfoRequest(pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//管理处理
bool CAttemperEngineSink::OnTCPNetworkMainManage(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GR_QUERY_OPTION:		//查询设置
		{
			return OnTCPNetworkSubQueryOption(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_OPTION_SERVER:		//房间设置
		{
			return OnTCPNetworkSubOptionServer(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_KILL_USER:          //踢出用户
		{
			return OnTCPNetworkSubManagerKickUser(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_LIMIT_USER_CHAT:	//限制聊天
		{
			return OnTCPNetworkSubLimitUserChat(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_KICK_ALL_USER:		//踢出用户
		{
			return OnTCPNetworkSubKickAllUser(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_SEND_MESSAGE:		//发布消息
		{
			return OnTCPNetworkSubSendMessage(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_DISMISSGAME:        //解散游戏
		{
			return OnTCPNetworkSubDismissGame(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_SEND_WARNING:		//发送警告
		{
			return OnTCPNetworkSubSendWarning(pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//比赛命令
bool CAttemperEngineSink::OnTCPNetworkMainMatch(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//获取信息
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//效验接口
	ASSERT_ERR(m_pIGameMatchServiceManager!=NULL);
	if (m_pIGameMatchServiceManager==NULL) return false;

	//消息处理
	return m_pIGameMatchServiceManager->OnEventSocketMatch(wSubCmdID,pData,wDataSize,pIServerUserItem,dwSocketID);
}

//游戏处理
bool CAttemperEngineSink::OnTCPNetworkMainGame(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//获取信息
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//处理过虑
	WORD wTableID=pIServerUserItem->GetTableID();
	WORD wChairID=pIServerUserItem->GetChairID();
	if ((wTableID==INVALID_TABLE)||(wChairID==INVALID_CHAIR)) return true;

	//消息处理 
	CTableFrame * pTableFrame=m_TableFrameArray[wTableID];
	if (pTableFrame) {
		bool bResult = pTableFrame->OnEventSocketGame(wSubCmdID, pData, wDataSize, pIServerUserItem);
		if ( bResult == false )
			I_TRACE(TEXT("游戏消息失败 SubCmdID:%d,TableID:%d"), wSubCmdID, wTableID);
		return bResult;
	}
	return false;
}

//框架处理
bool CAttemperEngineSink::OnTCPNetworkMainFrame(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//获取信息
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//处理过虑
	WORD wTableID=pIServerUserItem->GetTableID();
	WORD wChairID=pIServerUserItem->GetChairID();
	if ((wTableID==INVALID_TABLE)||(wChairID==INVALID_CHAIR)) return true;

	//消息处理 
	CTableFrame * pTableFrame=m_TableFrameArray[wTableID];
	if ( pTableFrame )
	{
		bool bResult = pTableFrame->OnEventSocketFrame(wSubCmdID, pData, wDataSize, pIServerUserItem);
		if (bResult == false)
			I_TRACE(TEXT("框架消息失败 SubCmdID:%d,TableID:%d"), wSubCmdID, wTableID);
		return bResult;
	}
	return false;
}

//I D 登录 机器人登录
bool CAttemperEngineSink::OnTCPNetworkSubLogonUserID(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT_ERR(wDataSize>=sizeof(CMD_GR_LogonUserID));
	if (wDataSize<sizeof(CMD_GR_LogonUserID)) return false;

	//处理消息
	CMD_GR_LogonUserID * pLogonUserID=(CMD_GR_LogonUserID *)pData;
	pLogonUserID->szPassword[CountArray(pLogonUserID->szPassword)-1]=0;
	pLogonUserID->szMachineID[CountArray(pLogonUserID->szMachineID)-1]=0;

	//绑定信息
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIBindUserItem=GetBindUserItem(wBindIndex);
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	//重复判断
	if ((pBindParameter==NULL)||(pIBindUserItem!=NULL))
	{ 
		ASSERT_ERR(FALSE);
		return false;
	}

	//房间判断
	if(pLogonUserID->wKindID != m_pGameServiceOption->wKindID)
	{
		//发送失败
		SendLogonFailure(TEXT("很抱歉，此游戏房间已经关闭了，不允许继续进入！"),LOGON_FAIL_SERVER_INVALIDATION,dwSocketID);
		return true;
	}

	////机器人和真人不许互踢
	//IServerUserItem * pIServerUserItem=m_ServerUserManager.SearchUserItem(pLogonUserID->dwUserID);
	//if (pIServerUserItem!=NULL)
	//{
	//	if((pIServerUserItem->IsAndroidUser() && (pBindParameter->dwClientAddr!=0L))
	//		|| (!pIServerUserItem->IsAndroidUser() && (pBindParameter->dwClientAddr==0L)))
	//	{
	//		SendRoomMessage(dwSocketID, TEXT("该账号已在此房间游戏，且不允许踢出，请咨询管理员！"), SMT_CHAT|SMT_EJECT|SMT_GLOBAL|SMT_CLOSE_ROOM,(pBindParameter->dwClientAddr==0L));
	//		return true;
	//	}
	//}

	//版本信息
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;

	//切换判断
// 	if((pIServerUserItem!=NULL)&&(pIServerUserItem->ContrastLogonPass(pLogonUserID->szPassword)==true))
// 	{
// 		SwitchUserItemConnect(pIServerUserItem,pLogonUserID->szMachineID,wBindIndex);
// 		return true;
// 	}

	//变量定义
	DBR_GR_LogonUserID LogonUserID;
	ZeroMemory(&LogonUserID,sizeof(LogonUserID));

	//构造数据
	LogonUserID.dwUserID=pLogonUserID->dwUserID;
	LogonUserID.dwClientAddr=pBindParameter->dwClientAddr;
	lstrcpyn(LogonUserID.szPassword,pLogonUserID->szPassword,CountArray(LogonUserID.szPassword));
	lstrcpyn(LogonUserID.szMachineID,pLogonUserID->szMachineID,CountArray(LogonUserID.szMachineID));

	//投递请求
	m_pIDBCorrespondManager->PostDataBaseRequest(LogonUserID.dwUserID, DBR_GR_LOGON_USERID, dwSocketID, &LogonUserID, sizeof(LogonUserID));

	return true;
}

//手机登录
bool CAttemperEngineSink::OnTCPNetworkSubLogonMobile(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT_ERR(wDataSize >= sizeof(CMD_GR_LogonMobile));
	if (wDataSize < sizeof(CMD_GR_LogonMobile)) return false;

	//处理消息
	CMD_GR_LogonMobile * pLogonUserID = (CMD_GR_LogonMobile*)pData;
	pLogonUserID->szPassword[CountArray(pLogonUserID->szPassword) - 1] = 0;
	pLogonUserID->szMachineID[CountArray(pLogonUserID->szMachineID) - 1] = 0;

	//绑定信息
	WORD wBindIndex = LOWORD(dwSocketID);
	IServerUserItem * pIBindUserItem = GetBindUserItem(wBindIndex);
	tagBindParameter * pBindParameter = GetBindParameter(wBindIndex);

	if (pBindParameter == NULL)
	{
		ASSERT_ERR(FALSE);
		return false;
	}

	if ( pIBindUserItem != NULL)
	{
		return false;
	}
	//重复判断
// 	if ((pBindParameter == NULL) || (pIBindUserItem != NULL))
// 	{
// 		ASSERT_ERR(FALSE);
// 		return false;
// 	}

	//版本信息
	pBindParameter->cbClientKind = CLIENT_KIND_MOBILE;

// 	IServerUserItem * pIServerUserItem = m_ServerUserManager.SearchUserItem(pLogonUserID->dwUserID);
// 	//切换判断
// 	if ((pIServerUserItem != NULL) && (pIServerUserItem->ContrastLogonPass(pLogonUserID->szPassword) == true))
// 	{
// 		SwitchUserItemConnect(pIServerUserItem, pLogonUserID->szMachineID, wBindIndex);
// 		return true;
// 	}

	//变量定义
	DBR_GR_LogonUserID LogonUserID;
	ZeroMemory(&LogonUserID, sizeof(LogonUserID));

	//构造数据
	LogonUserID.dwUserID = pLogonUserID->dwUserID;
	LogonUserID.dwClientAddr = pBindParameter->dwClientAddr;
	lstrcpyn(LogonUserID.szPassword, pLogonUserID->szPassword, CountArray(LogonUserID.szPassword));
	lstrcpyn(LogonUserID.szMachineID, pLogonUserID->szMachineID, CountArray(LogonUserID.szMachineID));

	//投递请求
	m_pIDBCorrespondManager->PostDataBaseRequest(LogonUserID.dwUserID, DBR_GR_LOGON_USERID, dwSocketID, &LogonUserID, sizeof(LogonUserID));
	
	g_Log.LogToFile(g_szLogRoot,TEXT("\n OnTCPNetworkSubLogonMobile UserID[%d]"),LogonUserID.dwUserID);
	return true;
}

bool CAttemperEngineSink::OnTCPNetworkSubLogonTokenIDByLua(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT_ERR(wDataSize >= sizeof(CMD_GR_LogonTokenID));
	if (wDataSize < sizeof(CMD_GR_LogonTokenID)) return false;

	//处理消息
	CMD_GR_LogonTokenID * pLogonTokenID = (CMD_GR_LogonTokenID*)pData;
	pLogonTokenID->szTokenID[CountArray(pLogonTokenID->szTokenID) - 1] = 0;
	pLogonTokenID->szMachineID[CountArray(pLogonTokenID->szMachineID) - 1] = 0;

	//绑定信息
	WORD wBindIndex = LOWORD(dwSocketID);
	IServerUserItem * pIBindUserItem = GetBindUserItem(wBindIndex);
	tagBindParameter * pBindParameter = GetBindParameter(wBindIndex);

	if (pBindParameter == NULL)
	{
		ASSERT_ERR(FALSE);
		return false;
	}

	if (pIBindUserItem != NULL)
	{
		return false;
	}


	//版本信息
	pBindParameter->cbClientKind = CLIENT_KIND_MOBILE;

	//变量定义
	DBR_GR_LogonTokenID LogonTokenID;
	ZeroMemory(&LogonTokenID, sizeof(LogonTokenID));

	//构造数据
	LogonTokenID.dwUserID = pLogonTokenID->dwUserID;
	LogonTokenID.dwClientAddr = pBindParameter->dwClientAddr;
	LogonTokenID.cbDeviceType = pLogonTokenID->cbDeviceType;
	LogonTokenID.dwMerchantID = pLogonTokenID->dwMerchantID;
	lstrcpyn(LogonTokenID.szTokenID, pLogonTokenID->szTokenID, CountArray(LogonTokenID.szTokenID));
	lstrcpyn(LogonTokenID.szMachineID, pLogonTokenID->szMachineID, CountArray(LogonTokenID.szMachineID));
	lstrcpyn(LogonTokenID.szClientIP, pLogonTokenID->szClientIP, CountArray(LogonTokenID.szClientIP));

	//投递请求
	m_pIDBCorrespondManager->PostDataBaseRequest(LogonTokenID.dwUserID, DBR_GR_LOGON_MOBILE, dwSocketID, &LogonTokenID, sizeof(LogonTokenID));

	I_TRACE(TEXT("OnTCPNetworkSubLogonTokenIDByLua %d"), LogonTokenID.dwUserID);
	g_Log.LogToFile(g_szLogRoot, TEXT("\n OnTCPNetworkSubLogonTokenIDByLua UserID[%d]"), LogonTokenID.dwUserID);
	return true;
}

//Token登录
bool CAttemperEngineSink::OnTCPNetworkSubLogonTokenID(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//绑定信息
	WORD wBindIndex = LOWORD(dwSocketID);
	IServerUserItem * pIBindUserItem = GetBindUserItem(wBindIndex);
	tagBindParameter * pBindParameter = GetBindParameter(wBindIndex);

	if (pBindParameter == NULL)
	{
		ASSERT_ERR(FALSE);
		return false;
	}

	if (pIBindUserItem != NULL)
	{
		return false;
	}
	I_TRACE(TEXT("------------------发起登录SocketID:%d--------------------------"),dwSocketID);
	//版本信息
	pBindParameter->cbClientKind = CLIENT_KIND_WEBSOCKET;

	//变量定义
	DBR_GR_LogonTokenID LogonTokenID;
	ZeroMemory(&LogonTokenID, sizeof(LogonTokenID));



	GameServer::GSLogonTokenID gsLogonTokenID;
	gsLogonTokenID.ParseFromArray(pData, wDataSize);

	//构造数据
	LogonTokenID.cbDeviceType = gsLogonTokenID.devicetype();
	LogonTokenID.dwClientAddr = pBindParameter->dwClientAddr;;
	LogonTokenID.dwMerchantID = gsLogonTokenID.merchantid();
	LogonTokenID.dwUserID = gsLogonTokenID.userid();

	lstrcpyn(LogonTokenID.szClientIP, Utils::ConvertA2W(gsLogonTokenID.clientip()).c_str(), CountArray(LogonTokenID.szClientIP));
	lstrcpyn(LogonTokenID.szMachineID, Utils::ConvertA2W(gsLogonTokenID.machineid()).c_str(), CountArray(LogonTokenID.szMachineID));
	lstrcpyn(LogonTokenID.szTokenID, Utils::ConvertA2W(gsLogonTokenID.tokenid()).c_str(), CountArray(LogonTokenID.szTokenID));

	//投递请求
	m_pIDBCorrespondManager->PostDataBaseRequest(LogonTokenID.dwUserID, DBR_GR_LOGON_TOKENID, dwSocketID, &LogonTokenID, sizeof(LogonTokenID));

	I_TRACE(TEXT("OnTCPNetworkSubLogonTokenID %d"), LogonTokenID.dwUserID);
	//g_Log.LogToFile(g_szLogRoot, TEXT("\n UserID[%d]-[发起登录]-SocketID[%d]"), LogonTokenID.dwUserID, dwSocketID);
	CString cs;
	cs.Format(TEXT("【主命令[1],次命令[4]】|【登录】用户[%d],BindIndex[%d] SocketID[%d],IP[%d]"), LogonTokenID.dwUserID, wBindIndex, dwSocketID, LogonTokenID.dwClientAddr);
	m_UserFileLog.Log(cs.GetBuffer(0));
	return true;
}

//帐号登录
bool CAttemperEngineSink::OnTCPNetworkSubLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT_ERR(wDataSize>=sizeof(CMD_GR_LogonAccounts));
	if (wDataSize<sizeof(CMD_GR_LogonAccounts)) return false;

	//处理消息
	CMD_GR_LogonAccounts * pLogonAccounts=(CMD_GR_LogonAccounts *)pData;
	pLogonAccounts->szPassword[CountArray(pLogonAccounts->szPassword)-1]=0;
	pLogonAccounts->szAccounts[CountArray(pLogonAccounts->szAccounts)-1]=0;
	pLogonAccounts->szMachineID[CountArray(pLogonAccounts->szMachineID)-1]=0;

	//绑定信息
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIBindUserItem=GetBindUserItem(wBindIndex);
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	//重复判断
	if (pIBindUserItem!=NULL)
	{ 
		ASSERT_ERR(FALSE);
		return false;
	}

	////机器人和真人不许互踢
	//IServerUserItem * pIServerUserItem=m_ServerUserManager.SearchUserItem(pLogonAccounts->szAccounts);
	//if (pIServerUserItem!=NULL)
	//{
	//	if((pIServerUserItem->IsAndroidUser() && (pBindParameter->dwClientAddr!=0L))
	//		|| (!pIServerUserItem->IsAndroidUser() && (pBindParameter->dwClientAddr==0L)))
	//	{
	//		SendRoomMessage(dwSocketID, TEXT("该账号已在此房间游戏，且不允许踢出，请咨询管理员！"), SMT_CHAT|SMT_EJECT|SMT_GLOBAL|SMT_CLOSE_ROOM,(pBindParameter->dwClientAddr==0L));
	//		return false;
	//	}
	//}

	//版本信息
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;

	//切换判断
// 	if ((pIServerUserItem!=NULL)&&(pIServerUserItem->ContrastLogonPass(pLogonAccounts->szPassword)==true))
// 	{
// 		SwitchUserItemConnect(pIServerUserItem,pLogonAccounts->szMachineID,wBindIndex);
// 		return true;
// 	}

	//变量定义
	DBR_GR_LogonAccounts LogonAccounts;
	ZeroMemory(&LogonAccounts,sizeof(LogonAccounts));

	//构造数据
	LogonAccounts.dwClientAddr=pBindParameter->dwClientAddr;
	lstrcpyn(LogonAccounts.szAccounts,pLogonAccounts->szAccounts,CountArray(LogonAccounts.szAccounts));
	lstrcpyn(LogonAccounts.szPassword,pLogonAccounts->szPassword,CountArray(LogonAccounts.szPassword));
	lstrcpyn(LogonAccounts.szMachineID,pLogonAccounts->szMachineID,CountArray(LogonAccounts.szMachineID));

	//投递请求
	//m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOGON_ACCOUNTS,dwSocketID,&LogonAccounts,sizeof(LogonAccounts));
	m_pIKernelDataBaseEngine->PostDataBaseRequest(DBR_GR_LOGON_ACCOUNTS,dwSocketID,&LogonAccounts,sizeof(LogonAccounts));

	g_Log.LogToFile(g_szLogRoot,TEXT("\n OnTCPNetworkSubLogonAccounts Account[%d]"),LogonAccounts.szAccounts);
	return true;
}

//用户旁观
bool CAttemperEngineSink::OnTCPNetworkSubUserLookon(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	return true;
}

//用户坐下
bool CAttemperEngineSink::OnTCPNetworkSubUserSitDownH5(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//获取用户
	WORD wBindIndex = LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem = GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT_ERR(pIServerUserItem != NULL);
	if (pIServerUserItem == NULL) return false;

	CString cs;
	cs.Format(TEXT("【主命令[3],次命令[15]】|【坐下】用户[%d][%s],BindIndex[%d],SocketID[%d]"),pIServerUserItem->GetUserID(),pIServerUserItem->GetNickName(), wBindIndex, wBindIndex);
	m_UserFileLog.Log(cs.GetBuffer(0));

	// lchq 2019 10 24 断线重连
	if (pIServerUserItem->GetTableID() != INVALID_TABLE&&pIServerUserItem->GetChairID() != INVALID_CHAIR)
	{
		if (pIServerUserItem->GetUserStatus() >= US_PLAYING && m_pGameServiceOption->wKindID != KIND_FISH)
		{
			//变量定义
			WORD wTableID = pIServerUserItem->GetTableID();
			WORD wChairID = pIServerUserItem->GetChairID();

			pIServerUserItem->SetHeartTime((DWORD)time(NULL));
			I_TRACE(TEXT("重连玩家 %s %d-%d Status:%d"), pIServerUserItem->GetNickName(), pIServerUserItem->GetTableID(), pIServerUserItem->GetChairID(), pIServerUserItem->GetUserStatus());
			
			pIServerUserItem->SetEnterTable(FALSE);
			pIServerUserItem->SetUserStatus(US_PLAYING, wTableID, wChairID);
			return true;
		}
	}

	GameServer::GSUserSitDown gsUserSitDown;
	gsUserSitDown.ParseFromArray(pData, wDataSize);

	CMD_GR_UserSitDown UserSitDown = { 0 };
	UserSitDown.wTableID = gsUserSitDown.tableid();
	UserSitDown.wChairID = gsUserSitDown.chairid();
	lstrcpyn(UserSitDown.szPassword, Utils::ConvertA2W(gsUserSitDown.tabletokenid()).c_str(), CountArray(UserSitDown.szPassword));

	//玩家当前信息
	WORD wTableID = pIServerUserItem->GetTableID();
	WORD wChairID = pIServerUserItem->GetChairID();
	BYTE cbUserStatus = pIServerUserItem->GetUserStatus();

	//重复判断
	if ((UserSitDown.wTableID < m_pGameServiceOption->wTableCount) && (UserSitDown.wChairID < m_pGameServiceAttrib->wChairCount))
	{
		CTableFrame * pTableFrame = m_TableFrameArray[UserSitDown.wTableID];
		if (pTableFrame->GetTableUserItem(UserSitDown.wChairID) == pIServerUserItem) return true;
	}

	//用户判断
	if (cbUserStatus == US_PLAYING)
	{
		if ( m_pGameServiceOption->wKindID != KIND_FISH)
			SendRequestFailureH5(pIServerUserItem, TEXT("您正在游戏中，暂时不能离开，请先结束当前游戏！"), REQUEST_FAILURE_NORMAL);
		return true;
	}

	//已经
	if (wTableID != INVALID_TABLE)
	{
		CTableFrame * pTableFrame = m_TableFrameArray[wTableID];
		if (pTableFrame != NULL && pTableFrame->PerformStandUpActionH5(pIServerUserItem) == false) return true;
	}

	//请求调整
	WORD wRequestTableID = UserSitDown.wTableID;
	WORD wRequestChairID = UserSitDown.wChairID;

	//动态加入
	bool bDynamicJoin = true;
	if (m_pGameServiceAttrib->cbDynamicJoin == FALSE) bDynamicJoin = false;
	if (CServerRule::IsAllowDynamicJoin(m_pGameServiceOption->dwServerRule) == false) bDynamicJoin = false;

	//系统配桌
	if (wRequestTableID >= m_TableFrameArray.GetCount())
	{
		//起始桌子
		WORD wStartTableID = rand() % m_pGameServiceOption->wTableCount;
		//暂时不随机
		//wStartTableID = 0;
		//寻找位置
		for (WORD i = wStartTableID; i<m_TableFrameArray.GetCount() + wStartTableID; i++)
		{
			WORD wStartID = 0;
			if (m_pGameServiceOption->wKindID != KIND_FISH)
				wStartID = i % m_pGameServiceOption->wTableCount;

			//游戏状态
			if ((m_TableFrameArray[wStartID]->IsGameStarted() == true) && (bDynamicJoin == false))continue;

			if (m_pGameServiceOption->wKindID != KIND_FISH)
			{
				tagTableUserInfo userInfo = { 0 };
				m_TableFrameArray[wStartID]->GetTableUserInfo(userInfo);

				//寻找空位置桌子
				if (userInfo.wTableUserCount > 0) continue;
			}		

			//获取空位
			WORD wNullChairID = m_TableFrameArray[wStartID]->GetNullChairID();

			//调整结果
			if (wNullChairID != INVALID_CHAIR)
			{
				wRequestTableID = wStartID;
				wRequestChairID = wNullChairID;

				break;
			}
		}

		//结果判断
		if ((wRequestTableID == INVALID_CHAIR) || (wRequestChairID == INVALID_CHAIR))
		{
			SendRequestFailureH5(pIServerUserItem, TEXT("当前游戏房间已经人满为患了，暂时没有可以让您加入的位置，请稍后再试！"), REQUEST_FAILURE_QUIT);
			return true;
		}
	}

	//椅子调整
	if (wRequestChairID >= m_pGameServiceAttrib->wChairCount)
	{
		//效验参数
		ASSERT_ERR(wRequestTableID<m_TableFrameArray.GetCount());
		if (wRequestTableID >= m_TableFrameArray.GetCount()) return false;

		//查找空位
		wRequestChairID = m_TableFrameArray[wRequestTableID]->GetNullChairID();

		//结果判断
		if (wRequestChairID == INVALID_CHAIR)
		{
			SendRequestFailureH5(pIServerUserItem, TEXT("由于此游戏桌暂时没有可以让您加入的位置了，请选择另外的游戏桌！"), REQUEST_FAILURE_QUIT);
			return true;
		}
	}

	//坐下处理
	CTableFrame * pTableFrame = m_TableFrameArray[wRequestTableID];
	if (pTableFrame != NULL)
	{
		bool bResult = false;
		bResult = pTableFrame->PerformSitDownActionH5(wRequestChairID, pIServerUserItem, UserSitDown.szPassword);
		//更新最近桌子
		if (bResult) {
			I_TRACE(TEXT("H5坐下 nickName:%s,TableID:%d,ChairID:%d"),pIServerUserItem->GetNickName(),pIServerUserItem->GetTableID(),pIServerUserItem->GetChairID());
		}
	}

	return true;
}

//用户坐下
bool CAttemperEngineSink::OnTCPNetworkSubUserSitDown(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{

	//效验参数
	ASSERT_ERR(wDataSize==sizeof(CMD_GR_UserSitDown));
	if (wDataSize!=sizeof(CMD_GR_UserSitDown)) return false;

	//效验数据
	CMD_GR_UserSitDown * pUserSitDown=(CMD_GR_UserSitDown *)pData;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;


	//消息处理
	WORD wTableID=pIServerUserItem->GetTableID();
	WORD wChairID=pIServerUserItem->GetChairID();
	BYTE cbUserStatus=pIServerUserItem->GetUserStatus();

	//重复判断
	if ((pUserSitDown->wTableID<m_pGameServiceOption->wTableCount)&&(pUserSitDown->wChairID<m_pGameServiceAttrib->wChairCount))
	{
		CTableFrame * pTableFrame=m_TableFrameArray[pUserSitDown->wTableID];
		if (pTableFrame->GetTableUserItem(pUserSitDown->wChairID)==pIServerUserItem) return true;
	}

	//用户判断
	if (cbUserStatus==US_PLAYING)
	{
		SendRequestFailure(pIServerUserItem,TEXT("您正在游戏中，暂时不能离开，请先结束当前游戏！"), REQUEST_FAILURE_NORMAL);
		return true;
	}

	//离开处理
	if (wTableID!=INVALID_TABLE)
	{
		CTableFrame * pTableFrame=m_TableFrameArray[wTableID];
		if (pTableFrame != NULL && pTableFrame->PerformStandUpAction(pIServerUserItem)==false) return true;
	}

// 	if (CServerRule::IsAllowAvertCheatMode(m_pGameServiceOption->dwServerRule) && (m_pGameServiceAttrib->wChairCount < MAX_CHAIR))
// 	{
// 		I_TRACE(TEXT("加入分组"));
// 		//加入分组
// 		InsertDistribute(pIServerUserItem);
// 		return true;
// 	}

	{
		CString szFileName;
		szFileName.Format(TEXT("%s/Settings/RoomID/UserID/%d.ini"),m_szPath,pIServerUserItem->GetUserID());

		DWORD dwLimitSitDown = 0;
		dwLimitSitDown	= GetPrivateProfileInt(TEXT("Parameter"),TEXT("LimitSitDown"),0,(LPCTSTR)szFileName);

		if ( dwLimitSitDown !=0 )
		{
			SendRequestFailure(pIServerUserItem,TEXT("当前游戏房间已经人满为患了，暂时没有可以让您加入的位置，请稍后再试！"), REQUEST_FAILURE_QUIT);
			return true;
		}
	}

	//请求调整
	WORD wRequestTableID=pUserSitDown->wTableID;
	WORD wRequestChairID=pUserSitDown->wChairID;

	//动态加入
	bool bDynamicJoin=true;
	if (m_pGameServiceAttrib->cbDynamicJoin==FALSE) bDynamicJoin=false;
	if (CServerRule::IsAllowDynamicJoin(m_pGameServiceOption->dwServerRule)==false) bDynamicJoin=false;

	//gm号追踪黑名单玩家 gm号坐下，遍历黑名单玩家桌子号
// 	{
// 		if (CUserRight::CanKillOutUser(pIServerUserItem->GetUserRight())==true)
// 		{
// 			WORD wIndex=0;
// 			WORD wTableID=INVALID_TABLE;
// 			WORD wNullChairID=INVALID_CHAIR;
// 			BYTE cbStopSearch = 0;
// 			do
// 			{
// 				IServerUserItem * pIUserItem=m_ServerUserManager.EnumUserItem(wIndex++);
// 				if (pIUserItem==NULL) break;
// 				//先定位桌子，再定位桌子是否人满
// 				if (CUserRight::IsGameCheatUser(pIUserItem->GetUserRight())==true)
// 					wTableID = pIUserItem->GetTableID();
// 				if ( wTableID <= m_TableFrameArray.GetCount())
// 					wNullChairID=m_TableFrameArray[wTableID]->GetNullChairID();
// 				if ( wNullChairID != INVALID_CHAIR ) //有空椅子
// 				{
// 					wRequestTableID = wTableID;
// 					wRequestChairID = wNullChairID;
// 					CTableFrame * pTableFrame=m_TableFrameArray[wRequestTableID];
// 					if ( pTableFrame != NULL )
// 						pTableFrame->PerformSitDownAction(wRequestChairID,pIServerUserItem,pUserSitDown->szPassword);
// 					
// 					//已经找到黑名单玩家停止搜索
// 					cbStopSearch = 1;
// 					break;
// 				}
// 
// 			} while (true);
// 			if ( cbStopSearch )
// 				return true;
// 		}
// 	}

	//桌子调整
	if (wRequestTableID>=m_TableFrameArray.GetCount())
	{
		//起始桌子
		WORD wStartTableID= rand() % m_pGameServiceOption->wTableCount;
		wStartTableID = 0;
		//DWORD dwServerRule=m_pGameServiceOption->dwServerRule;
		//if ((CServerRule::IsAllowAvertCheatMode(dwServerRule)==true)&&(m_pGameServiceAttrib->wChairCount<MAX_CHAIR)) wStartTableID=1;
		//if ((CServerRule::IsAllowAvertCheatMode(dwServerRule)==true)&&(m_pGameServiceAttrib->wChairCount>=MAX_CHAIR)) wStartTableID=0;

		//寻找位置
		for (WORD i=wStartTableID;i<m_TableFrameArray.GetCount()+wStartTableID;i++)
		{
			WORD wStartID = 0;
			wStartID = i % m_pGameServiceOption->wTableCount;
			//游戏状态
			if ((m_TableFrameArray[wStartID]->IsGameStarted()==true)&&(bDynamicJoin==false))continue;

			//获取空位
			WORD wNullChairID=m_TableFrameArray[wStartID]->GetNullChairID();

			//调整结果
			if (wNullChairID!=INVALID_CHAIR)
			{
				bool bLimitSameIP = false;
				/*
				if ( m_pGameServiceOption->wRoomType != 4 && CUserRight::CanKillOutUser(pIServerUserItem->GetUserRight())==false )
				{
					CString szFileName;
					szFileName.Format(TEXT("%s/Settings/RoomID/%d.ini"),m_szPath,m_pGameServiceOption->dwRoomID);

					DWORD dwAllowSameIP = 0;
					dwAllowSameIP	= GetPrivateProfileInt(TEXT("Parameter"),TEXT("AllowSameIP"),0,(LPCTSTR)szFileName);


					//判定此桌其他玩家IP是否相同
					//dwAllowSameIP=0 一律允许  1不允许
					if (  dwAllowSameIP==1)
					{
						DWORD dwUserIP=pIServerUserItem->GetClientAddr();
						for( WORD j=0;j<m_pGameServiceAttrib->wChairCount;++j )
						{
							CTableFrame * pTableFrame=m_TableFrameArray[i];
							IServerUserItem * pITableUserItem=pTableFrame->GetTableUserItem(j);
							if ((pITableUserItem!=NULL)&&(pITableUserItem != pIServerUserItem)&&(!pITableUserItem->IsAndroidUser())&&(pITableUserItem->GetMasterOrder()==0)&&(pITableUserItem->GetClientAddr()==dwUserIP))
							{
								//D_TRACE(TEXT("出现桌子相同"));
								bLimitSameIP = true;
								break;
							}
						}
					}

				}

				//IP同桌标识
				if ( bLimitSameIP ) continue;
				*/
				wRequestTableID= wStartID;
				wRequestChairID=wNullChairID;

				break;
			}
		}

		//结果判断
		if ((wRequestTableID==INVALID_CHAIR)||(wRequestChairID==INVALID_CHAIR))
		{
			SendRequestFailure(pIServerUserItem,TEXT("当前游戏房间已经人满为患了，暂时没有可以让您加入的位置，请稍后再试！"), REQUEST_FAILURE_QUIT);
			return true;
		}
	}

	//椅子调整
	if (wRequestChairID>=m_pGameServiceAttrib->wChairCount)
	{
		//效验参数
		ASSERT_ERR(wRequestTableID<m_TableFrameArray.GetCount());
		if (wRequestTableID>=m_TableFrameArray.GetCount()) return false;

		//查找空位
		wRequestChairID=m_TableFrameArray[wRequestTableID]->GetNullChairID();

		//结果判断
		if (wRequestChairID==INVALID_CHAIR)
		{
			SendRequestFailure(pIServerUserItem,TEXT("由于此游戏桌暂时没有可以让您加入的位置了，请选择另外的游戏桌！"), REQUEST_FAILURE_QUIT);
			return true;
		}
	}

	//坐下处理
	CTableFrame * pTableFrame=m_TableFrameArray[wRequestTableID];
	if ( pTableFrame != NULL )
	{
		bool bResult = false;
		bResult = pTableFrame->PerformSitDownAction(wRequestChairID, pIServerUserItem, pUserSitDown->szPassword);
		//更新最近桌子
		if (bResult) {
			I_TRACE(TEXT("nickName:%s,LastTableID:%d"), pIServerUserItem->GetNickName(), pIServerUserItem->GetTableID());
		}
	}

	return true;
}

//用户起立
bool CAttemperEngineSink::OnTCPNetworkSubUserStandUpH5(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	//ASSERT_ERR(wDataSize == sizeof(CMD_GR_UserStandUp));
	//if (wDataSize != sizeof(CMD_GR_UserStandUp)) return false;

	GameServer::GSUserStandUp gsUserStandUp;
	gsUserStandUp.ParseFromArray(pData, wDataSize);
	CMD_GR_UserStandUp UserStandUp = { 0 };
	UserStandUp.wTableID = gsUserStandUp.tableid();
	UserStandUp.wChairID = gsUserStandUp.chairid();
	UserStandUp.cbForceLeave = (BYTE)gsUserStandUp.forceleave();

	//效验数据
	//CMD_GR_UserStandUp * pUserStandUp = (CMD_GR_UserStandUp *)&UserStandUp;
	//if (pUserStandUp->wChairID >= m_pGameServiceAttrib->wChairCount) return false;
	//if (pUserStandUp->wTableID >= (WORD)m_TableFrameArray.GetCount()) return false;

	//获取用户
	WORD wBindIndex = LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem = GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT_ERR(pIServerUserItem != NULL);
	if (pIServerUserItem == NULL) return false;

	DeleteDistribute(pIServerUserItem);

	//消息处理
	WORD wTableID = pIServerUserItem->GetTableID();
	WORD wChairID = pIServerUserItem->GetChairID();
	//if ((wTableID != pUserStandUp->wTableID) || (wChairID != pUserStandUp->wChairID)) return true;


	//用户判断
 	if ( pIServerUserItem->GetUserStatus() == US_PLAYING && m_pGameServiceOption->wKindID != KIND_FISH)
 	{
 		SendRequestFailureH5(pIServerUserItem, TEXT("您正在游戏中，暂时不能离开，请先结束当前游戏！"), REQUEST_FAILURE_NORMAL);
 		return true;
 	}

	//离开处理
	if (wTableID != INVALID_TABLE)
	{
		CTableFrame * pTableFrame = m_TableFrameArray[wTableID];
		if ( pTableFrame )
		{
			I_TRACE(TEXT("离开处理 UserID[%d] cbStatus[%d]"), pIServerUserItem->GetUserID(), pIServerUserItem->GetUserStatus());

			if (pTableFrame->PerformStandUpAction(pIServerUserItem,TRUE) == false)
			{
				m_UserFileLog.Log(TEXT("玩家[%d] 桌椅[%d-%d] 状态[%d] 请求起立失败"),pIServerUserItem->GetUserID(),pIServerUserItem->GetTableID(),pIServerUserItem->GetChairID(),pIServerUserItem->GetUserStatus());
			 }
		}
	}
	GameServer::GSUserLeave gsUserLeave;
	gsUserLeave.set_userid(gsUserStandUp.userid());
	std::string serializeStr;
	gsUserLeave.SerializeToString(&serializeStr);
	SendData(dwSocketID, MDM_GR_USER, SUB_GR_USER_LEAVE, (void*)serializeStr.c_str(), (WORD)serializeStr.size());
	I_TRACE(TEXT("主动离开 UserID[%d] cbStatus[%d]"), pIServerUserItem->GetUserID(), pIServerUserItem->GetUserStatus());
	CString cs;
	cs.Format(TEXT("【主命令[3],次命令[16]】|【离开】用户[%d][%s],BindIndex[%d],SocketID[%d]"), pIServerUserItem->GetUserID(),pIServerUserItem->GetNickName(), wBindIndex, dwSocketID);
	m_UserFileLog.Log(cs.GetBuffer(0));

	pIServerUserItem->SetUserStatus(US_NULL, INVALID_TABLE, INVALID_CHAIR);

	return true;
}

//用户起立
bool CAttemperEngineSink::OnTCPNetworkSubUserStandUp(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT_ERR(wDataSize==sizeof(CMD_GR_UserStandUp));
	if (wDataSize!=sizeof(CMD_GR_UserStandUp)) return false;

	//效验数据
	CMD_GR_UserStandUp * pUserStandUp=(CMD_GR_UserStandUp *)pData;
	if (pUserStandUp->wChairID>=m_pGameServiceAttrib->wChairCount) return false;
	if (pUserStandUp->wTableID>=(WORD)m_TableFrameArray.GetCount()) return false;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//test
	DeleteDistribute(pIServerUserItem);


	//if(CServerRule::IsAllowAvertCheatMode(m_pGameServiceOption->dwServerRule)&&(m_pGameServiceAttrib->wChairCount < MAX_CHAIR))
	//{
	//	//取消分组
	//	DeleteDistribute(pIServerUserItem);

	//	if(pUserStandUp->wTableID==INVALID_TABLE) return true;
	//}

	//消息处理
	WORD wTableID=pIServerUserItem->GetTableID();
	WORD wChairID=pIServerUserItem->GetChairID();
	if ((wTableID!=pUserStandUp->wTableID)||(wChairID!=pUserStandUp->wChairID)) return true;


	//用户判断
	if (/*(pUserStandUp->cbForceLeave==FALSE)&&*/(pIServerUserItem->GetUserStatus()==US_PLAYING))
	{
		SendRequestFailure(pIServerUserItem,TEXT("您正在游戏中，暂时不能离开，请先结束当前游戏！"), REQUEST_FAILURE_NORMAL);
		return true;
	}

	//离开处理
	if (wTableID!=INVALID_TABLE)
	{
		CTableFrame * pTableFrame=m_TableFrameArray[wTableID];
		if (pTableFrame != NULL && pTableFrame->PerformStandUpAction(pIServerUserItem)==false) return true;
	}

	return true;
}

//H5换桌
bool CAttemperEngineSink::OnTCPNetworkSubUserChangeTableH5(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//获取用户
	WORD wBindIndex = LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem = GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT_ERR(pIServerUserItem != NULL);
	if (pIServerUserItem == NULL) return false;

	CString cs;
	cs.Format(TEXT("【主命令[3],次命令[17]】|【换桌】用户[%d][%s] BindIndex[%d] SocketID[%d] 状态[%d] 桌椅[%d-%d] "), pIServerUserItem->GetUserID(),pIServerUserItem->GetNickName(), wBindIndex, dwSocketID, pIServerUserItem->GetUserStatus(), pIServerUserItem->GetTableID(), pIServerUserItem->GetChairID());
	m_UserFileLog.Log(cs.GetBuffer(0));

	if (DetectKickUser(pIServerUserItem,TRUE)) return true;

	//用户状态
	if (pIServerUserItem->GetUserStatus() == US_PLAYING && m_pGameServiceOption->wKindID != KIND_FISH)
	{
		SendRequestFailureH5(pIServerUserItem, TEXT("您正在游戏中，暂时不能离开，请先结束当前游戏！"), REQUEST_FAILURE_NORMAL);
		return true;
	}

	WORD wRequestTableID = INVALID_TABLE;
	WORD wRequestChairID = INVALID_CHAIR;
	bool bRet = false;
	//查找桌子
	for (INT_PTR i = 0; i < (m_pGameServiceOption->wTableCount); i++)
	{
		//过滤同桌
		if (i == pIServerUserItem->GetTableID())continue;

		//获取桌子
		CTableFrame * pTableFrame = m_TableFrameArray[i];
		if (pTableFrame->IsGameStarted() == true) continue;
		if (pTableFrame->IsTableLocked()) continue;

		tagTableUserInfo userInfo = { 0 };
		pTableFrame->GetTableUserInfo(userInfo);

		//寻找空位置桌子
		if (userInfo.wTableUserCount > 0) continue;

		//获取空位
		WORD wChairID = pTableFrame->GetNullChairID();

		if (wChairID == INVALID_CHAIR) continue;

		wRequestTableID = i;
		wRequestChairID = wChairID;
	
		//结果判断
		if ((wRequestTableID == INVALID_CHAIR) || (wRequestChairID == INVALID_CHAIR))
		{
			SendRequestFailureH5(pIServerUserItem, TEXT("当前游戏房间已经人满为患了，暂时没有可以让您加入的位置，请稍后再试！"), REQUEST_FAILURE_QUIT);
			return true;
		}

		//离开处理
		if (pIServerUserItem->GetTableID() != INVALID_TABLE)
		{
			CTableFrame * pTableFrame = m_TableFrameArray[pIServerUserItem->GetTableID()];
			if (pTableFrame->PerformStandUpActionH5(pIServerUserItem) == false) return true;
		}

		//用户坐下
		bRet = pTableFrame->PerformSitDownActionH5(wChairID, pIServerUserItem);
		if (bRet)
		{
			I_TRACE(TEXT("H5换桌 %s %d-%d Status:%d"),pIServerUserItem->GetNickName(),pIServerUserItem->GetTableID(),pIServerUserItem->GetChairID(),pIServerUserItem->GetUserStatus());
		}
		return true;
	}

	//失败
	SendRequestFailureH5(pIServerUserItem, TEXT("没找到可进入的游戏桌！"), REQUEST_FAILURE_QUIT);
	return true;
}

bool CAttemperEngineSink::OnTCPNetworkSubUserChangeTable(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//用户状态
	if(pIServerUserItem->GetUserStatus() == US_PLAYING)
	{
		//失败
		SendRequestFailure(pIServerUserItem,TEXT("您正在游戏中，暂时不能离开，请先结束当前游戏！"),REQUEST_FAILURE_NORMAL);
		return true;
	}

	WORD wRequestTableID = INVALID_TABLE;
	WORD wRequestChairID = INVALID_CHAIR;

	//查找桌子
	for (INT_PTR i=0;i<(m_pGameServiceOption->wTableCount);i++)
	{
		//过滤同桌
		if(i == pIServerUserItem->GetTableID())continue;

		//获取桌子
		CTableFrame * pTableFrame=m_TableFrameArray[i];
		if (pTableFrame->IsGameStarted()==true) continue;
		if(pTableFrame->IsTableLocked()) continue;

		//无效过滤
		WORD wChairID=pTableFrame->GetRandNullChairID();
		if (wChairID==INVALID_CHAIR) continue;


		if (wChairID!=INVALID_CHAIR)
		{
			CString szFileName;
			szFileName.Format(TEXT("%s/Settings/RoomID/%d.ini"),m_szPath,m_pGameServiceOption->dwRoomID);

			DWORD dwAllowSameIP = 0;
			dwAllowSameIP	= GetPrivateProfileInt(TEXT("Parameter"),TEXT("AllowSameIP"),0,(LPCTSTR)szFileName);

			bool bLimitSameIP = false;
			//判定此桌其他玩家IP是否相同
			if (  dwAllowSameIP==0)
			{
				DWORD dwUserIP=pIServerUserItem->GetClientAddr();
				for( WORD j=0;j<m_pGameServiceAttrib->wChairCount;++j )
				{
					CTableFrame * pTableFrame=m_TableFrameArray[i];
					IServerUserItem * pITableUserItem=pTableFrame->GetTableUserItem(j);
					if ((pITableUserItem!=NULL)&&(pITableUserItem != pIServerUserItem)&&(!pITableUserItem->IsAndroidUser())&&(pITableUserItem->GetMasterOrder()==0)&&(pITableUserItem->GetClientAddr()==dwUserIP))
					{
						bLimitSameIP = true;
						break;
					}
				}
			}
			//IP同桌标识
			if ( bLimitSameIP ) continue;

			wRequestTableID = i;
			wRequestChairID = wChairID;
		}

		//结果判断
		if ((wRequestTableID==INVALID_CHAIR)||(wRequestChairID==INVALID_CHAIR))
		{
			SendRequestFailure(pIServerUserItem,TEXT("当前游戏房间已经人满为患了，暂时没有可以让您加入的位置，请稍后再试！"), REQUEST_FAILURE_QUIT);
			return true;
		}

		//离开处理
		if (pIServerUserItem->GetTableID()!=INVALID_TABLE)
		{
			CTableFrame * pTableFrame=m_TableFrameArray[pIServerUserItem->GetTableID()];
			if (pTableFrame->PerformStandUpAction(pIServerUserItem)==false) return true;
		}

		//用户坐下
		pTableFrame->PerformSitDownAction(wChairID,pIServerUserItem);
		return true;
	}

	//失败
	m_TableFrameArray[0]->SendRequestFailure(pIServerUserItem,TEXT("没找到可进入的游戏桌！"),REQUEST_FAILURE_NORMAL);
	return true;
}

//请求更换位置
bool CAttemperEngineSink::OnTCPNetworkSubUserChairReq(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//用户状态
	if(pIServerUserItem->GetUserStatus() == US_PLAYING)
	{
		//失败
		SendRequestFailure(pIServerUserItem,TEXT("您正在游戏中，暂时不能离开，请先结束当前游戏！"),REQUEST_FAILURE_NORMAL);
		return true;
	}

	WORD wRequestTableID = INVALID_TABLE;
	WORD wRequestChairID = INVALID_CHAIR;

	//查找桌子
	for (INT_PTR i=0;i<(m_pGameServiceOption->wTableCount);i++)
	{
		//过滤同桌
		if(i == pIServerUserItem->GetTableID())continue;

		//获取桌子
		CTableFrame * pTableFrame=m_TableFrameArray[i];
		if (pTableFrame->IsGameStarted()==true) continue;
		if(pTableFrame->IsTableLocked()) continue;

		//无效过滤
		WORD wChairID=pTableFrame->GetRandNullChairID();
		if (wChairID==INVALID_CHAIR) continue;


		if (wChairID!=INVALID_CHAIR)
		{
			CString szFileName;
			szFileName.Format(TEXT("%s/Settings/RoomID/%d.ini"),m_szPath,m_pGameServiceOption->dwRoomID);

			DWORD dwAllowSameIP = 0;
			dwAllowSameIP	= GetPrivateProfileInt(TEXT("Parameter"),TEXT("AllowSameIP"),0,(LPCTSTR)szFileName);

			bool bLimitSameIP = false;
			//判定此桌其他玩家IP是否相同
			if (  dwAllowSameIP==0)
			{
				DWORD dwUserIP=pIServerUserItem->GetClientAddr();
				for( WORD j=0;j<m_pGameServiceAttrib->wChairCount;++j )
				{
					CTableFrame * pTableFrame=m_TableFrameArray[i];
					IServerUserItem * pITableUserItem=pTableFrame->GetTableUserItem(j);
					if ((pITableUserItem!=NULL)&&(pITableUserItem != pIServerUserItem)&&(!pITableUserItem->IsAndroidUser())&&(pITableUserItem->GetMasterOrder()==0)&&(pITableUserItem->GetClientAddr()==dwUserIP))
					{
						bLimitSameIP = true;
						break;
					}
				}
			}
			//IP同桌标识
			if ( bLimitSameIP ) continue;

			wRequestTableID = i;
			wRequestChairID = wChairID;
		}

		//结果判断
		if ((wRequestTableID==INVALID_CHAIR)||(wRequestChairID==INVALID_CHAIR))
		{
			SendRequestFailure(pIServerUserItem,TEXT("当前游戏房间已经人满为患了，暂时没有可以让您加入的位置，请稍后再试！"), REQUEST_FAILURE_QUIT);
			return true;
		}

		//离开处理
		if (pIServerUserItem->GetTableID()!=INVALID_TABLE)
		{
			CTableFrame * pTableFrame=m_TableFrameArray[pIServerUserItem->GetTableID()];
			if (pTableFrame->PerformStandUpAction(pIServerUserItem)==false) return true;
		}

		//用户坐下
		pTableFrame->PerformSitDownAction(wChairID,pIServerUserItem);
		return true;
	}

	//
	if(pIServerUserItem->GetTableID() != INVALID_TABLE)
	{
		//获取桌子
		CTableFrame * pTableFrame=m_TableFrameArray[pIServerUserItem->GetTableID()];
		if (pTableFrame->IsGameStarted()==false && pTableFrame->IsTableLocked()==false)
		{
			//无效过滤
			WORD wChairID=pTableFrame->GetRandNullChairID();
			if (wChairID!=INVALID_CHAIR)
			{
				//离开处理
				if (pIServerUserItem->GetTableID()!=INVALID_TABLE)
				{
					CTableFrame * pTableFrame=m_TableFrameArray[pIServerUserItem->GetTableID()];
					if (pTableFrame->PerformStandUpAction(pIServerUserItem)==false) return true;
				}

				//用户坐下
				pTableFrame->PerformSitDownAction(wChairID,pIServerUserItem);
				return true;
			}
		}
	}

	//失败
	m_TableFrameArray[0]->SendRequestFailure(pIServerUserItem,TEXT("没找到可进入的游戏桌！"),REQUEST_FAILURE_NORMAL);
	return true;
}

//请求椅子用户信息
bool CAttemperEngineSink::OnTCPNetworkSubChairUserInfoReq(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT_ERR(wDataSize==sizeof(CMD_GR_ChairUserInfoReq));
	if (wDataSize!=sizeof(CMD_GR_ChairUserInfoReq)) return false;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//变量定义
	CMD_GR_ChairUserInfoReq * pUserInfoReq = (CMD_GR_ChairUserInfoReq *)pData;
	if(pUserInfoReq->wTableID == INVALID_TABLE) return true;
	if(pUserInfoReq->wTableID >= m_pGameServiceOption->wTableCount)return true;

	//发送消息
	WORD wChairCout = m_TableFrameArray[pUserInfoReq->wTableID]->GetChairCount();
	for(WORD wIndex = 0; wIndex < wChairCout; wIndex++)
	{
		//获取用户
		if(pUserInfoReq->wChairID != INVALID_CHAIR && wIndex != pUserInfoReq->wChairID)continue;
		IServerUserItem * pTagerIServerUserItem=m_TableFrameArray[pUserInfoReq->wTableID]->GetTableUserItem(wIndex);
		if(pTagerIServerUserItem==NULL)continue;

		//变量定义
		BYTE cbBuffer[SOCKET_TCP_PACKET]={0};
		tagMobileUserInfoHead * pUserInfoHead=(tagMobileUserInfoHead *)cbBuffer;
		CSendPacketHelper SendPacket(cbBuffer+sizeof(tagMobileUserInfoHead),sizeof(cbBuffer)-sizeof(tagMobileUserInfoHead));
		tagUserInfo *pUserInfo = pTagerIServerUserItem->GetUserInfo();

		//用户属性
		pUserInfoHead->wFaceID=pUserInfo->wFaceID;
		pUserInfoHead->dwGameID=pUserInfo->dwGameID;
		pUserInfoHead->dwUserID=pUserInfo->dwUserID;
		pUserInfoHead->dwCustomID=pUserInfo->dwCustomID;

		//用户属性
		pUserInfoHead->cbGender=pUserInfo->cbGender;
		pUserInfoHead->cbMemberOrder=pUserInfo->cbMemberOrder;

		//用户状态
		pUserInfoHead->wTableID=pUserInfo->wTableID;
		pUserInfoHead->wChairID=pUserInfo->wChairID;
		pUserInfoHead->cbUserStatus=pUserInfo->cbUserStatus;

		//用户局数
		pUserInfoHead->dwWinCount=pUserInfo->dwWinCount;
		pUserInfoHead->dwLostCount=pUserInfo->dwLostCount;
		pUserInfoHead->dwDrawCount=pUserInfo->dwDrawCount;
		pUserInfoHead->dwFleeCount=pUserInfo->dwFleeCount;
		pUserInfoHead->dwExperience=pUserInfo->dwExperience;

		//用户成绩
		pUserInfoHead->lScore=pUserInfo->lScore;
		pUserInfoHead->lScore+=pIServerUserItem->GetTrusteeScore();
		pUserInfoHead->lScore+=pIServerUserItem->GetFrozenedScore();

		//叠加信息
		SendPacket.AddPacket(pUserInfo->szNickName,DTP_GR_NICK_NAME);

		//发送消息
		WORD wHeadSize=sizeof(tagMobileUserInfoHead);
		SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_USER_ENTER,cbBuffer,wHeadSize+SendPacket.GetDataSize());
	}

	return true;
}



//查询银行
bool CAttemperEngineSink::OnTCPNetworkSubQueryInsureInfo(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT_ERR(wDataSize==sizeof(CMD_GR_C_QueryInsureInfoRequest));
	if (wDataSize!=sizeof(CMD_GR_C_QueryInsureInfoRequest)) return false;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//变量定义
	CMD_GR_C_QueryInsureInfoRequest * pQueryInsureInfoRequest = (CMD_GR_C_QueryInsureInfoRequest *)pData;

	//房间判断
	ASSERT_ERR((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)!=0);
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)==0)
	{
		SendInsureFailure(pIServerUserItem,TEXT("此房间禁止游戏存取款，查询操作失败！"),0L,pQueryInsureInfoRequest->cbActivityGame);

	}


	//变量定义
	DBR_GR_QueryInsureInfo QueryInsureInfo;
	ZeroMemory(&QueryInsureInfo,sizeof(QueryInsureInfo));

	//构造数据
	QueryInsureInfo.cbActivityGame=pQueryInsureInfoRequest->cbActivityGame;
	QueryInsureInfo.dwUserID=pIServerUserItem->GetUserID();
	QueryInsureInfo.dwClientAddr=pIServerUserItem->GetClientAddr();

	//投递请求
	m_pIDBCorrespondManager->PostDataBaseRequest(QueryInsureInfo.dwUserID,DBR_GR_QUERY_INSURE_INFO,dwSocketID,&QueryInsureInfo,sizeof(QueryInsureInfo));

	return true;
}

//存款请求
bool CAttemperEngineSink::OnTCPNetworkSubSaveScoreRequest(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT_ERR(wDataSize==sizeof(CMD_GR_C_SaveScoreRequest));
	if (wDataSize!=sizeof(CMD_GR_C_SaveScoreRequest)) return false;

	//房间判断
	ASSERT_ERR((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)!=0);
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)==0) return false;

	//变量定义
	CMD_GR_C_SaveScoreRequest * pSaveScoreRequest=(CMD_GR_C_SaveScoreRequest *)pData;

	//效验参数
	ASSERT_ERR(pSaveScoreRequest->lSaveScore>0L);
	if (pSaveScoreRequest->lSaveScore<=0L) return false;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//规则判断
	if(pSaveScoreRequest->cbActivityGame == FALSE && CServerRule::IsForfendSaveInRoom(m_pGameServiceOption->dwServerRule))
	{
		//发送数据
		SendInsureFailure(pIServerUserItem,TEXT("此房间禁止房间存款，存入操作失败！"),0L,pSaveScoreRequest->cbActivityGame);
		return true;
	}

	//规则判断
	if(pSaveScoreRequest->cbActivityGame == TRUE && CServerRule::IsForfendSaveInGame(m_pGameServiceOption->dwServerRule))
	{
		//发送数据
		SendInsureFailure(pIServerUserItem,TEXT("此房间禁止游戏存款，存入操作失败！"),0L,pSaveScoreRequest->cbActivityGame);
		return true;
	}

	//变量定义
	SCORE lConsumeQuota=0L;
	SCORE lUserWholeScore=pIServerUserItem->GetUserScore()+pIServerUserItem->GetTrusteeScore();

	//获取限额
	if (pIServerUserItem->GetTableID()!=INVALID_TABLE)
	{
		WORD wTableID=pIServerUserItem->GetTableID();
		lConsumeQuota=m_TableFrameArray[wTableID]->QueryConsumeQuota(pIServerUserItem);
	}
	else
	{
		lConsumeQuota=pIServerUserItem->GetUserScore()+pIServerUserItem->GetTrusteeScore();
	}

	//限额判断
	if (pSaveScoreRequest->lSaveScore>lConsumeQuota)
	{
		if (lConsumeQuota<lUserWholeScore)
		{
			//构造提示
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("由于您正在游戏中，元宝可存入额度为 %I64d，存入操作失败！"),lConsumeQuota);

			//发送数据
			SendInsureFailure(pIServerUserItem,szDescribe,0L,pSaveScoreRequest->cbActivityGame);
		}
		else
		{
			//发送数据
			SendInsureFailure(pIServerUserItem,TEXT("您的金币余额不足，存入操作失败！"),0L,pSaveScoreRequest->cbActivityGame);
		}

		return true;
	}

	//锁定积分
	if (pIServerUserItem->FrozenedUserScore(pSaveScoreRequest->lSaveScore)==false)
	{
		ASSERT_ERR(FALSE);
		return false;
	}

	//变量定义
	DBR_GR_UserSaveScore UserSaveScore;
	ZeroMemory(&UserSaveScore,sizeof(UserSaveScore));

	//构造数据
	UserSaveScore.cbActivityGame=pSaveScoreRequest->cbActivityGame;
	UserSaveScore.dwUserID=pIServerUserItem->GetUserID();
	UserSaveScore.lSaveScore=pSaveScoreRequest->lSaveScore;
	UserSaveScore.dwClientAddr=pIServerUserItem->GetClientAddr();
	lstrcpyn(UserSaveScore.szMachineID,pIServerUserItem->GetMachineID(),CountArray(UserSaveScore.szMachineID));

	//投递请求
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_USER_SAVE_SCORE,dwSocketID,&UserSaveScore,sizeof(UserSaveScore));

	return true;
}

//取款请求
bool CAttemperEngineSink::OnTCPNetworkSubTakeScoreRequest(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT_ERR(wDataSize==sizeof(CMD_GR_C_TakeScoreRequest));
	if (wDataSize!=sizeof(CMD_GR_C_TakeScoreRequest)) return false;

	//房间判断
	ASSERT_ERR((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)!=0);
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)==0) return false;

	//变量定义
	CMD_GR_C_TakeScoreRequest * pTakeScoreRequest=(CMD_GR_C_TakeScoreRequest *)pData;
	pTakeScoreRequest->szInsurePass[CountArray(pTakeScoreRequest->szInsurePass)-1]=0;

	//效验参数
	ASSERT_ERR(pTakeScoreRequest->lTakeScore>0L);
	if (pTakeScoreRequest->lTakeScore<=0L) return false;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//规则判断
	if(pTakeScoreRequest->cbActivityGame == FALSE && CServerRule::IsForfendTakeInRoom(m_pGameServiceOption->dwServerRule))
	{
		//发送数据
		SendInsureFailure(pIServerUserItem,TEXT("此房间禁止房间取款，取出操作失败！"),0L,pTakeScoreRequest->cbActivityGame);
		return true;
	}

	//规则判断
	if(pTakeScoreRequest->cbActivityGame == TRUE && CServerRule::IsForfendTakeInGame(m_pGameServiceOption->dwServerRule))
	{
		//发送数据
		SendInsureFailure(pIServerUserItem,TEXT("此房间禁止游戏取款，取出操作失败！"),0L,pTakeScoreRequest->cbActivityGame);
		return true;
	}

	//变量定义
	DBR_GR_UserTakeScore UserTakeScore;
	ZeroMemory(&UserTakeScore,sizeof(UserTakeScore));

	//构造数据
	UserTakeScore.cbActivityGame=pTakeScoreRequest->cbActivityGame;
	UserTakeScore.dwUserID=pIServerUserItem->GetUserID();
	UserTakeScore.lTakeScore=pTakeScoreRequest->lTakeScore;
	UserTakeScore.dwClientAddr=pIServerUserItem->GetClientAddr();
	lstrcpyn(UserTakeScore.szPassword,pTakeScoreRequest->szInsurePass,CountArray(UserTakeScore.szPassword));
	lstrcpyn(UserTakeScore.szMachineID,pIServerUserItem->GetMachineID(),CountArray(UserTakeScore.szMachineID));

	//投递请求
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_USER_TAKE_SCORE,dwSocketID,&UserTakeScore,sizeof(UserTakeScore));

	return true;
}

//转账请求
bool CAttemperEngineSink::OnTCPNetworkSubTransferScoreRequest(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT_ERR(wDataSize==sizeof(CMD_GP_C_TransferScoreRequest));
	if (wDataSize!=sizeof(CMD_GP_C_TransferScoreRequest)) return false;

	//房间判断
	ASSERT_ERR((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)!=0);
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)==0) return false;

	//变量定义
	CMD_GP_C_TransferScoreRequest * pTransferScoreRequest=(CMD_GP_C_TransferScoreRequest *)pData;
	pTransferScoreRequest->szNickName[CountArray(pTransferScoreRequest->szNickName)-1]=0;
	pTransferScoreRequest->szInsurePass[CountArray(pTransferScoreRequest->szInsurePass)-1]=0;

	//效验参数
	ASSERT_ERR(pTransferScoreRequest->lTransferScore>0L);
	if (pTransferScoreRequest->lTransferScore<=0L) return false;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//变量定义
	DBR_GR_UserTransferScore UserTransferScore;
	ZeroMemory(&UserTransferScore,sizeof(UserTransferScore));

	//构造数据
	UserTransferScore.cbActivityGame=pTransferScoreRequest->cbActivityGame;
	UserTransferScore.dwUserID=pIServerUserItem->GetUserID();
	UserTransferScore.dwClientAddr=pIServerUserItem->GetClientAddr();
	UserTransferScore.cbByNickName=pTransferScoreRequest->cbByNickName;
	UserTransferScore.lTransferScore=pTransferScoreRequest->lTransferScore;
	lstrcpyn(UserTransferScore.szNickName,pTransferScoreRequest->szNickName,CountArray(UserTransferScore.szNickName));
	lstrcpyn(UserTransferScore.szMachineID,pIServerUserItem->GetMachineID(),CountArray(UserTransferScore.szMachineID));
	lstrcpyn(UserTransferScore.szPassword,pTransferScoreRequest->szInsurePass,CountArray(UserTransferScore.szPassword));

	//投递请求
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_USER_TRANSFER_SCORE,dwSocketID,&UserTransferScore,sizeof(UserTransferScore));

	return true;
}

//查询用户请求
bool CAttemperEngineSink::OnTCPNetworkSubQueryUserInfoRequest(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT_ERR(wDataSize==sizeof(CMD_GR_C_QueryUserInfoRequest));
	if (wDataSize!=sizeof(CMD_GR_C_QueryUserInfoRequest)) return false;

	//房间判断
	ASSERT_ERR((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)!=0);
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)==0) return false;

	//变量定义
	CMD_GR_C_QueryUserInfoRequest * pQueryUserInfoRequest=(CMD_GR_C_QueryUserInfoRequest *)pData;
	pQueryUserInfoRequest->szNickName[CountArray(pQueryUserInfoRequest->szNickName)-1]=0;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//变量定义
	DBR_GR_QueryTransferUserInfo QueryTransferUserInfo;
	ZeroMemory(&QueryTransferUserInfo,sizeof(QueryTransferUserInfo));

	//构造数据
	QueryTransferUserInfo.cbActivityGame=pQueryUserInfoRequest->cbActivityGame;
	QueryTransferUserInfo.cbByNickName=pQueryUserInfoRequest->cbByNickName;
	lstrcpyn(QueryTransferUserInfo.szNickName,pQueryUserInfoRequest->szNickName,CountArray(QueryTransferUserInfo.szNickName));
	QueryTransferUserInfo.dwUserID=pIServerUserItem->GetUserID();

	//投递请求
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_QUERY_TRANSFER_USER_INFO,dwSocketID,&QueryTransferUserInfo,sizeof(QueryTransferUserInfo));

	return true;
}

//查询设置
bool CAttemperEngineSink::OnTCPNetworkSubQueryOption(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT_ERR((pIServerUserItem!=NULL)&&(pIServerUserItem->GetMasterOrder()>0));
	if ((pIServerUserItem==NULL)||(pIServerUserItem->GetMasterOrder()==0)) return false;

	//变量定义
	CMD_GR_OptionCurrent OptionCurrent;
	ZeroMemory(&OptionCurrent,sizeof(OptionCurrent));

	//挂接属性
	OptionCurrent.ServerOptionInfo.wKindID=m_pGameServiceOption->wKindID;
	OptionCurrent.ServerOptionInfo.wNodeID=m_pGameServiceOption->wNodeID;
	OptionCurrent.ServerOptionInfo.wSortID=m_pGameServiceOption->wSortID;

	//房间配置
	OptionCurrent.ServerOptionInfo.wRevenueRatio=m_pGameServiceOption->wRevenueRatio;
	OptionCurrent.ServerOptionInfo.lServiceScore=m_pGameServiceOption->lServiceScore;
	OptionCurrent.ServerOptionInfo.lRestrictScore=m_pGameServiceOption->lRestrictScore;
	OptionCurrent.ServerOptionInfo.lMinTableScore=m_pGameServiceOption->lMinTableScore;
	OptionCurrent.ServerOptionInfo.lMinEnterScore=m_pGameServiceOption->lMinEnterScore;
	OptionCurrent.ServerOptionInfo.lMaxEnterScore=m_pGameServiceOption->lMaxEnterScore;
	OptionCurrent.ServerOptionInfo.lMinEnterTiLi=m_pGameServiceOption->lMinEnterTiLi;

	//会员限制
	OptionCurrent.ServerOptionInfo.cbMinEnterMember=m_pGameServiceOption->cbMinEnterMember;
	OptionCurrent.ServerOptionInfo.cbMaxEnterMember=m_pGameServiceOption->cbMaxEnterMember;

	//房间属性
	OptionCurrent.ServerOptionInfo.dwServerRule=m_pGameServiceOption->dwServerRule;
	lstrcpyn(OptionCurrent.ServerOptionInfo.szServerName,m_pGameServiceOption->szServerName,CountArray(OptionCurrent.ServerOptionInfo.szServerName));

	//聊天规则
	OptionCurrent.dwRuleMask|=SR_FORFEND_GAME_CHAT;
	OptionCurrent.dwRuleMask|=SR_FORFEND_ROOM_CHAT;
	OptionCurrent.dwRuleMask|=SR_FORFEND_WISPER_CHAT;
	OptionCurrent.dwRuleMask|=SR_FORFEND_WISPER_ON_GAME;

	//房间规则
	OptionCurrent.dwRuleMask|=SR_FORFEND_ROOM_ENTER;
	OptionCurrent.dwRuleMask|=SR_FORFEND_GAME_ENTER;
	OptionCurrent.dwRuleMask|=SR_FORFEND_GAME_LOOKON;

	//银行规则
	OptionCurrent.dwRuleMask|=SR_FORFEND_TAKE_IN_ROOM;
	OptionCurrent.dwRuleMask|=SR_FORFEND_TAKE_IN_GAME;
	OptionCurrent.dwRuleMask|=SR_FORFEND_SAVE_IN_ROOM;
	OptionCurrent.dwRuleMask|=SR_FORFEND_SAVE_IN_GAME;

	//其他规则
	//OptionCurrent.dwRuleMask|=SR_RECORD_GAME_TRACK;
	OptionCurrent.dwRuleMask|=SR_FORFEND_GAME_RULE;
	OptionCurrent.dwRuleMask|=SR_FORFEND_LOCK_TABLE;
	OptionCurrent.dwRuleMask|=SR_ALLOW_ANDROID_SIMULATE;

	//组件规则
	if (m_pGameServiceAttrib->cbDynamicJoin==TRUE) OptionCurrent.dwRuleMask|=SR_ALLOW_DYNAMIC_JOIN;
	if (m_pGameServiceAttrib->cbAndroidUser==TRUE) OptionCurrent.dwRuleMask|=SR_ALLOW_ANDROID_ATTEND;
	//if (m_pGameServiceAttrib->cbOffLineTrustee==TRUE) OptionCurrent.dwRuleMask|=SR_ALLOW_OFFLINE_TRUSTEE;

	//模式规则
	if ((m_pGameServiceOption->wServerType&(GAME_GENRE_GOLD|GAME_GENRE_MATCH|GAME_GENRE_SCORE))==0) OptionCurrent.dwRuleMask|=SR_RECORD_GAME_SCORE;
	if ((m_pGameServiceOption->wServerType&(GAME_GENRE_GOLD|GAME_GENRE_MATCH|GAME_GENRE_SCORE))==0) OptionCurrent.dwRuleMask|=SR_IMMEDIATE_WRITE_SCORE;

	//发送数据
	SendData(pIServerUserItem,MDM_GR_MANAGE,SUB_GR_OPTION_CURRENT,&OptionCurrent,sizeof(OptionCurrent));

	return true;
}

//房间设置
bool CAttemperEngineSink::OnTCPNetworkSubOptionServer(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT_ERR(wDataSize==sizeof(CMD_GR_ServerOption));
	if (wDataSize!=sizeof(CMD_GR_ServerOption)) return false;

	//变量定义
	CMD_GR_ServerOption * pServerOption=(CMD_GR_ServerOption *)pData;
	tagServerOptionInfo * pServerOptionInfo=&pServerOption->ServerOptionInfo;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT_ERR((pIServerUserItem!=NULL)&&(pIServerUserItem->GetMasterOrder()>0));
	if ((pIServerUserItem==NULL)||(pIServerUserItem->GetMasterOrder()==0)) return false;

	//变量定义
	bool bModifyServer=false;

	//挂接节点
	if (m_pGameServiceOption->wNodeID!=pServerOptionInfo->wNodeID)
	{
		bModifyServer=true;
		m_pGameServiceOption->wNodeID=pServerOptionInfo->wNodeID;
	}

	//挂接类型
	if ((pServerOptionInfo->wKindID!=0)&&(m_pGameServiceOption->wKindID!=pServerOptionInfo->wKindID))
	{
		bModifyServer=true;
		m_pGameServiceOption->wKindID=pServerOptionInfo->wKindID;
	}

	//挂接排序
	if ((pServerOptionInfo->wSortID!=0)&&(m_pGameServiceOption->wSortID!=pServerOptionInfo->wSortID))
	{
		bModifyServer=true;
		m_pGameServiceOption->wSortID=pServerOptionInfo->wSortID;
	}

	//房间名字
	if ((pServerOptionInfo->szServerName[0]!=0)&&(lstrcmp(m_pGameServiceOption->szServerName,pServerOptionInfo->szServerName)!=0))
	{
		bModifyServer=true;
		lstrcpyn(m_pGameServiceOption->szServerName,pServerOptionInfo->szServerName,CountArray(m_pGameServiceOption->szServerName));
	}

	//税收配置
	m_pGameServiceOption->wRevenueRatio=pServerOptionInfo->wRevenueRatio;
	m_pGameServiceOption->lServiceScore=pServerOptionInfo->lServiceScore;

	//房间配置
	m_pGameServiceOption->lRestrictScore=pServerOptionInfo->lRestrictScore;
	m_pGameServiceOption->lMinTableScore=pServerOptionInfo->lMinTableScore;
	m_pGameServiceOption->lMinEnterScore=pServerOptionInfo->lMinEnterScore;
	m_pGameServiceOption->lMaxEnterScore=pServerOptionInfo->lMaxEnterScore;
	m_pGameServiceOption->lMinEnterTiLi=pServerOptionInfo->lMinEnterTiLi;

	//会员限制
	m_pGameServiceOption->cbMinEnterMember=pServerOptionInfo->cbMinEnterMember;
	m_pGameServiceOption->cbMaxEnterMember=pServerOptionInfo->cbMaxEnterMember;

	//聊天规则
	CServerRule::SetForfendGameChat(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendGameChat(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendRoomChat(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendRoomChat(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendWisperChat(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendWisperChat(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendWisperOnGame(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendWisperOnGame(pServerOptionInfo->dwServerRule));

	//房间规则
	CServerRule::SetForfendRoomEnter(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendRoomEnter(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendGameEnter(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendGameEnter(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendGameLookon(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendGameLookon(pServerOptionInfo->dwServerRule));

	//银行规则
	CServerRule::SetForfendTakeInRoom(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendTakeInRoom(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendTakeInGame(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendTakeInGame(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendSaveInRoom(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendSaveInRoom(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendSaveInGame(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendSaveInGame(pServerOptionInfo->dwServerRule));

	//其他规则
	CServerRule::SetRecordGameTrack(m_pGameServiceOption->dwServerRule,CServerRule::IsRecordGameTrack(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendGameRule(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendGameRule(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendLockTable(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendLockTable(pServerOptionInfo->dwServerRule));

	//动态加入
	if (m_pGameServiceAttrib->cbDynamicJoin==TRUE)
	{
		CServerRule::SetAllowDynamicJoin(m_pGameServiceOption->dwServerRule,CServerRule::IsAllowDynamicJoin(pServerOptionInfo->dwServerRule));
	}

	//机器管理
	if (m_pGameServiceAttrib->cbAndroidUser==TRUE)
	{
		CServerRule::SetAllowAndroidAttend(m_pGameServiceOption->dwServerRule,CServerRule::IsAllowAndroidAttend(pServerOptionInfo->dwServerRule));
	}

	//断线托管
	if (m_pGameServiceAttrib->cbOffLineTrustee==TRUE)
	{
		CServerRule::SetAllowOffLineTrustee(m_pGameServiceOption->dwServerRule,CServerRule::IsAllowOffLineTrustee(pServerOptionInfo->dwServerRule));
	}

	//记录成绩
	if ((m_pGameServiceOption->wServerType&(GAME_GENRE_GOLD|GAME_GENRE_MATCH|GAME_GENRE_SCORE))==0)
	{
		CServerRule::SetRecordGameScore(m_pGameServiceOption->dwServerRule,CServerRule::IsRecordGameScore(pServerOptionInfo->dwServerRule));
	}

	//立即写分
	if ((m_pGameServiceOption->wServerType&(GAME_GENRE_GOLD|GAME_GENRE_MATCH|GAME_GENRE_SCORE))==0)
	{
		CServerRule::SetImmediateWriteScore(m_pGameServiceOption->dwServerRule,CServerRule::IsImmediateWriteScore(pServerOptionInfo->dwServerRule));
	}

	//调整参数
	CServiceUnits * pServiceUnits=CServiceUnits::g_pServiceUnits;
	pServiceUnits->RectifyServiceParameter();

	//发送修改
	if (bModifyServer==true)
	{
		//变量定义
		CMD_CS_C_ServerModify ServerModify;
		ZeroMemory(&ServerModify,sizeof(ServerModify));

		//服务端口
		ServerModify.wServerPort=pServiceUnits->m_TCPNetworkEngine->GetCurrentPort();

		//房间信息
		ServerModify.wKindID=m_pGameServiceOption->wKindID;
		ServerModify.wNodeID=m_pGameServiceOption->wNodeID;
		ServerModify.wSortID=m_pGameServiceOption->wSortID;
		ServerModify.dwOnLineCount=m_ServerUserManager.GetUserItemCount();
		ServerModify.dwFullCount=m_pGameServiceOption->wMaxPlayer-RESERVE_USER_COUNT;
		lstrcpyn(ServerModify.szServerName,m_pGameServiceOption->szServerName,CountArray(ServerModify.szServerName));
		lstrcpyn(ServerModify.szServerAddr,m_pInitParameter->m_ServiceAddress.szAddress,CountArray(ServerModify.szServerAddr));

		//发送数据
		m_pITCPSocketService->SendData(MDM_CS_SERVICE_INFO,SUB_CS_C_SERVER_MODIFY,&ServerModify,sizeof(ServerModify));
	}

	//发送信息
	SendRoomMessage(pIServerUserItem,TEXT("当前游戏服务器房间的“运行值”状态配置数据修改成功"),SMT_CHAT|SMT_EJECT);

	//输出信息
	TCHAR szBuffer[128]=TEXT("");
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("远程修改房间配置通知 管理员 %s [ %ld ]"),pIServerUserItem->GetNickName(),pIServerUserItem->GetUserID());

	//输出信息
	TRACE_LOG(szBuffer,TraceLevel_Info);

	return true;
}

//踢出用户
bool CAttemperEngineSink::OnTCPNetworkSubManagerKickUser(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT_ERR(wDataSize==sizeof(CMD_GR_KickUser));
	if (wDataSize!=sizeof(CMD_GR_KickUser)) return false;

	//变量定义
	CMD_GR_KickUser * pKickUser=(CMD_GR_KickUser *)pData;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT_ERR((pIServerUserItem!=NULL)&&(pIServerUserItem->GetMasterOrder()>0));
	if ((pIServerUserItem==NULL)||(pIServerUserItem->GetMasterOrder()==0)) return false;

	//权限判断
	ASSERT_ERR(CMasterRight::CanKillUser(pIServerUserItem->GetMasterRight())==true);
	if (CMasterRight::CanKillUser(pIServerUserItem->GetMasterRight())==false) return false;

	//目标用户
	IServerUserItem * pITargetUserItem = m_ServerUserManager.SearchUserItem(pKickUser->dwTargetUserID);
	if(pITargetUserItem==NULL) return true;

	//用户状态
	if(pITargetUserItem->GetUserStatus()==US_PLAYING) return true;

	//踢出记录
	tagKickUser KickUser = { 0 };
	KickUser.dwStartKickTime = (DWORD)time(NULL);
	KickUser.dwLimitTime = 600;
	m_KickUserItemMap[pITargetUserItem->GetUserID()]= KickUser;

	//请离桌子
	WORD wTargerTableID = pITargetUserItem->GetTableID();
	if(wTargerTableID != INVALID_TABLE)
	{
		//发送消息
		SendGameMessage(pITargetUserItem,TEXT("你已被管理员请离桌子！"),SMT_CHAT|SMT_CLOSE_GAME);

		CTableFrame * pTableFrame=m_TableFrameArray[wTargerTableID];
		if (pTableFrame->PerformStandUpAction(pITargetUserItem)==false) return true;
	}

	//发送通知
	LPCTSTR pszMessage=TEXT("你已被管理员请离此游戏房间！");
	SendRoomMessage(pITargetUserItem,pszMessage,SMT_CHAT|SMT_EJECT|SMT_GLOBAL|SMT_CLOSE_ROOM);

	pITargetUserItem->SetUserStatus(US_NULL,INVALID_TABLE,INVALID_CHAIR);

	return true;
}

//限制聊天
bool CAttemperEngineSink::OnTCPNetworkSubLimitUserChat(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验数据
	ASSERT_ERR(wDataSize==sizeof(CMD_GR_LimitUserChat));
	if (wDataSize!=sizeof(CMD_GR_LimitUserChat)) return false;

	//消息处理
	CMD_GR_LimitUserChat * pLimitUserChat=(CMD_GR_LimitUserChat *)pData;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);
	if (pIServerUserItem==NULL) return false;

	//权限判断
	//ASSERT_ERR(CMasterRight::CanLimitUserChat(pIServerUserItem->GetMasterRight())==true);
	//if (CMasterRight::CanLimitUserChat(pIServerUserItem->GetMasterRight())==false) return false;

	//目标用户
	IServerUserItem * pITargerUserItem=m_ServerUserManager.SearchUserItem(pLimitUserChat->dwTargetUserID);
	if (pITargerUserItem==NULL) return true;

	//变量定义
	DWORD dwAddRight = 0, dwRemoveRight = 0;

	//大厅聊天
	if (pLimitUserChat->cbLimitFlags==OSF_ROOM_CHAT)
	{
		if (CMasterRight::CanLimitRoomChat(pIServerUserItem->GetMasterRight())==false) return false;

		if( pLimitUserChat->cbLimitValue == TRUE )
			dwAddRight |= UR_CANNOT_ROOM_CHAT;
		else
			dwRemoveRight |= UR_CANNOT_ROOM_CHAT;
	}

	//游戏聊天
	if (pLimitUserChat->cbLimitFlags==OSF_GAME_CHAT)
	{
		if (CMasterRight::CanLimitGameChat(pIServerUserItem->GetMasterRight())==false) return false;

		if( pLimitUserChat->cbLimitValue == TRUE )
			dwAddRight |= UR_CANNOT_GAME_CHAT;
		else
			dwRemoveRight |= UR_CANNOT_GAME_CHAT;
	}

	//大厅私聊
	if (pLimitUserChat->cbLimitFlags==OSF_ROOM_WISPER)
	{
		if (CMasterRight::CanLimitWisper(pIServerUserItem->GetMasterRight())==false) return false;

		if( pLimitUserChat->cbLimitValue == TRUE )
			dwAddRight |= UR_CANNOT_WISPER;
		else
			dwRemoveRight |= UR_CANNOT_WISPER;
	}

	//发送喇叭
	if(pLimitUserChat->cbLimitFlags==OSF_SEND_BUGLE)
	{
		if (CMasterRight::CanLimitUserChat(pIServerUserItem->GetMasterRight())==false) return false;

		if(pLimitUserChat->cbLimitValue == TRUE)
			dwAddRight |= UR_CANNOT_BUGLE;
		else
			dwRemoveRight |= UR_CANNOT_BUGLE;
	}

	if( dwAddRight != 0 || dwRemoveRight != 0 )
	{
		pITargerUserItem->ModifyUserRight(dwAddRight,dwRemoveRight);

		//发送通知
		CMD_GR_ConfigUserRight cur = {0};
		cur.dwUserRight = pITargerUserItem->GetUserRight();

		SendData( pITargerUserItem,MDM_GR_CONFIG,SUB_GR_CONFIG_USER_RIGHT,&cur,sizeof(cur) );

		//发送消息
		SendRoomMessage(pIServerUserItem,TEXT("用户聊天权限配置成功！"),SMT_CHAT);
	}
	else return false;

	return true;
}

//踢出所有用户
bool CAttemperEngineSink::OnTCPNetworkSubKickAllUser(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//消息处理
	CMD_GR_KickAllUser * pKillAllUser=(CMD_GR_KickAllUser *)pData;

	//效验数据
	ASSERT_ERR(wDataSize<=sizeof(CMD_GR_KickAllUser));
	if( wDataSize > sizeof(CMD_GR_KickAllUser) ) return false;
	ASSERT_ERR(wDataSize==CountStringBuffer(pKillAllUser->szKickMessage));
	if (wDataSize!=CountStringBuffer(pKillAllUser->szKickMessage)) return false;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);
	if (pIServerUserItem==NULL) return false;

	//权限判断
	ASSERT_ERR(CMasterRight::CanKillUser(pIServerUserItem->GetMasterRight())==true);
	if (CMasterRight::CanKillUser(pIServerUserItem->GetMasterRight())==false) return false;

	//解散所有游戏
	for (INT_PTR i=0;i<m_TableFrameArray.GetCount();i++)
	{
		//获取桌子
		CTableFrame * pTableFrame=m_TableFrameArray[i];
		if ( !pTableFrame->IsGameStarted() ) continue;

		pTableFrame->DismissGame();
	}

	tagBindParameter *pBindParameter = m_pNormalParameter;
	for( INT i = 0; i < m_pGameServiceOption->wMaxPlayer; i++ )
	{
		//目录用户
		IServerUserItem * pITargerUserItem= pBindParameter->pIServerUserItem;
		if (pITargerUserItem==NULL || pITargerUserItem==pIServerUserItem ) 
		{
			pBindParameter++;
			continue;
		}

		//发送消息
		SendRoomMessage(pITargerUserItem,pKillAllUser->szKickMessage,SMT_CHAT|SMT_EJECT|SMT_CLOSE_LINK|SMT_CLOSE_ROOM);

		pBindParameter++;
	} 

	return true;
}

//发布消息
bool CAttemperEngineSink::OnTCPNetworkSubSendMessage(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);
	if (pIServerUserItem==NULL) return false;

	//权限判断
	ASSERT_ERR(CMasterRight::CanIssueMessage(pIServerUserItem->GetMasterRight())==true);
	if (CMasterRight::CanIssueMessage(pIServerUserItem->GetMasterRight())==false) return false;

	//消息处理
	return SendSystemMessage((CMD_GR_SendMessage *)pData, wDataSize);
}

//解散游戏
bool CAttemperEngineSink::OnTCPNetworkSubDismissGame(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验数据
	ASSERT_ERR(wDataSize==sizeof(CMD_GR_DismissGame));
	if (wDataSize!=sizeof(CMD_GR_DismissGame)) return false;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);
	if (pIServerUserItem==NULL) return false;

	//权限判断
	//ASSERT_ERR(CMasterRight::CanDismissGame(pIServerUserItem->GetMasterRight())==true);
	//if (CMasterRight::CanDismissGame(pIServerUserItem->GetMasterRight())==false) return false;

	//消息处理
	CMD_GR_DismissGame * pDismissGame=(CMD_GR_DismissGame *)pData;

	//效验数据
	if(pDismissGame->wDismissTableNum >= m_TableFrameArray.GetCount()) return true;

	//解散游戏
	CTableFrame *pTableFrame=m_TableFrameArray[pDismissGame->wDismissTableNum];
	if(pTableFrame)
	{
		//if(pTableFrame->IsGameStarted()) 
		pTableFrame->DismissGame();
	}

	return true;
}

bool CAttemperEngineSink::OnTCPNetworkSubSendWarning(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验数据
	ASSERT_ERR(wDataSize==sizeof(CMD_GR_SendWarning));
	if (wDataSize!=sizeof(CMD_GR_SendWarning)) return false;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);
	if (pIServerUserItem==NULL) return false;

	return true;
}

//用户登录
VOID CAttemperEngineSink::OnEventUserLogonH5(IServerUserItem * pIServerUserItem, bool bAlreadyOnLine)
{
	//获取参数
	WORD wBindIndex = pIServerUserItem->GetBindIndex();
	bool bAndroidUser = pIServerUserItem->IsAndroidUser();
	tagBindParameter * pBindParameter = GetBindParameter(wBindIndex);

	GameServer::GSLogonSuccess logonSuccess;
	logonSuccess.set_userright((int64)pIServerUserItem->GetUserRight());
	logonSuccess.set_masterright((int64)pIServerUserItem->GetMasterRight());
	std::string serializeStr;
	logonSuccess.SerializeToString(&serializeStr);
	SendData(pBindParameter->dwSocketID, MDM_GR_LOGON, SUB_GR_LOGON_SUCCESS, (void*)serializeStr.c_str(), (WORD)serializeStr.size());

	//登录完成
	GameServer::GSLogonFinish LogonFinish;
	LogonFinish.set_resultcode(0);
	LogonFinish.set_resultstr("logonFinish");
	serializeStr.clear();
	LogonFinish.SerializeToString(&serializeStr);
	SendData(pBindParameter->dwSocketID, MDM_GR_LOGON, SUB_GR_LOGON_FINISH,(void*)serializeStr.c_str(), (WORD)serializeStr.size());

	//玩家数据
	I_TRACE(TEXT("H5玩家登录 userID[%d],nickName[%s] %d-%d Status[%d] SocketID[%d]"), pIServerUserItem->GetUserID(),
		pIServerUserItem->GetNickName(), pIServerUserItem->GetTableID(),pIServerUserItem->GetChairID(),
		pIServerUserItem->GetUserStatus(), pIServerUserItem->GetBindIndex());
	//SendUserInfoPacketH5(pIServerUserItem, pBindParameter->dwSocketID);


	//网络设置
	if (bAndroidUser == false)
	{
		if (pBindParameter->cbClientKind == CLIENT_KIND_MOBILE)
		{
			m_pITCPNetworkEngine->AllowBatchSend(pBindParameter->dwSocketID, true, BG_MOBILE);
		}
		else
		{
			m_pITCPNetworkEngine->AllowBatchSend(pBindParameter->dwSocketID, true, BG_COMPUTER);
		}
	}

	return;
}

//用户登录
VOID CAttemperEngineSink::OnEventUserLogon(IServerUserItem * pIServerUserItem, bool bAlreadyOnLine)
{
	//获取参数
	WORD wBindIndex=pIServerUserItem->GetBindIndex();
	bool bAndroidUser=pIServerUserItem->IsAndroidUser();
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	//登录处理
	//if (pIServerUserItem->IsMobileUser()==false)
	{
		if ( pIServerUserItem->IsAndroidUser() == false )
		{
			//变量定义
			CMD_GR_LogonSuccess LogonSuccess;
			CMD_GR_ConfigServerEx ConfigServer;
			ZeroMemory(&LogonSuccess, sizeof(LogonSuccess));
			ZeroMemory(&ConfigServer, sizeof(ConfigServer));

			//登录成功
			LogonSuccess.dwUserRight = pIServerUserItem->GetUserRight();
			LogonSuccess.dwMasterRight = pIServerUserItem->GetMasterRight();
			SendData(pBindParameter->dwSocketID, MDM_GR_LOGON, SUB_GR_LOGON_SUCCESS, &LogonSuccess, sizeof(LogonSuccess));

			//房间配置
			ConfigServer.wTableCount = m_pGameServiceOption->wTableCount;
			ConfigServer.wChairCount = m_pGameServiceAttrib->wChairCount;
			ConfigServer.wServerType = m_pGameServiceOption->wServerType;
			ConfigServer.dwServerRule = m_pGameServiceOption->dwServerRule;
			ConfigServer.lCellScore = m_pGameServiceOption->lCellScore;
			ConfigServer.dwExpValue = m_pGameServiceOption->wRewardExp;
			ConfigServer.dwBloodLower = m_pGameServiceOption->dwBloodLower;

			SendData(pBindParameter->dwSocketID, MDM_GR_CONFIG, SUB_GR_CONFIG_SERVER, &ConfigServer, sizeof(ConfigServer));

			//列表配置 
			WORD wConfigColumnHead = sizeof(m_DataConfigColumn) - sizeof(m_DataConfigColumn.ColumnItem);
			WORD wConfigColumnInfo = m_DataConfigColumn.cbColumnCount * sizeof(m_DataConfigColumn.ColumnItem[0]);
			SendData(pBindParameter->dwSocketID, MDM_GR_CONFIG, SUB_GR_CONFIG_COLUMN, &m_DataConfigColumn, wConfigColumnHead + wConfigColumnInfo);

			//配置完成
			SendData(pBindParameter->dwSocketID, MDM_GR_CONFIG, SUB_GR_CONFIG_FINISH, NULL, 0);

			//玩家数据
			SendUserInfoPacket(pIServerUserItem, pBindParameter->dwSocketID);

			//在线用户
			WORD wUserIndex = 0;
			IServerUserItem * pIServerUserItemSend = NULL;
			while (true)
			{
				pIServerUserItemSend = m_ServerUserManager.EnumUserItem(wUserIndex++);
				if (pIServerUserItemSend == NULL) break;
				if (pIServerUserItemSend == pIServerUserItem) continue;
				//I_TRACE(TEXT("在线玩家 %s 状态%d,tableID:%d"), pIServerUserItemSend->GetNickName(), pIServerUserItemSend->GetUserStatus(), pIServerUserItemSend->GetTableID());
				SendUserInfoPacket(pIServerUserItemSend, pBindParameter->dwSocketID);
			}

			//桌子状态
			CMD_GR_TableInfo TableInfo;
			TableInfo.wTableCount = (WORD)m_TableFrameArray.GetCount();
			ASSERT_ERR(TableInfo.wTableCount < CountArray(TableInfo.TableStatusArray));
			for (WORD i = 0; i < TableInfo.wTableCount; i++)
			{
				CTableFrame * pTableFrame = m_TableFrameArray[i];
				TableInfo.TableStatusArray[i].cbTableLock = pTableFrame->IsTableLocked() ? TRUE : FALSE;
				TableInfo.TableStatusArray[i].cbPlayStatus = pTableFrame->IsTableStarted() ? TRUE : FALSE;
			}

			//桌子状态
			WORD wHeadSize = sizeof(TableInfo) - sizeof(TableInfo.TableStatusArray);
			WORD wSendSize = wHeadSize + TableInfo.wTableCount * sizeof(TableInfo.TableStatusArray[0]);
			SendData(pBindParameter->dwSocketID, MDM_GR_STATUS, SUB_GR_TABLE_INFO, &TableInfo, wSendSize);

			//发送通知
			if (bAlreadyOnLine == false)
			{
				SendUserInfoPacket(pIServerUserItem, INVALID_DWORD);
			}
		}

		//登录完成
		//I_TRACE(TEXT("机器玩家登录nickName:%s,Score:%I64d,IsAndroid:%d"), pIServerUserItem->GetNickName(),pIServerUserItem->GetUserScore() ,pIServerUserItem->IsAndroidUser());
		SendData(pBindParameter->dwSocketID,MDM_GR_LOGON,SUB_GR_LOGON_FINISH,NULL,0);
	}

	//网络设置
	if (bAndroidUser==false)
	{
		if (pBindParameter->cbClientKind==CLIENT_KIND_MOBILE)
		{
			m_pITCPNetworkEngine->AllowBatchSend(pBindParameter->dwSocketID,true,BG_MOBILE);
		}
		else
		{
			m_pITCPNetworkEngine->AllowBatchSend(pBindParameter->dwSocketID,true,BG_COMPUTER);
		}
	}

	if(m_pIGameMatchServiceManager!=NULL)
	{
		m_pIGameMatchServiceManager->SendMatchInfo(pIServerUserItem);
	}
	return;
}

//用户离开
VOID CAttemperEngineSink::OnEventUserLogout(IServerUserItem * pIServerUserItem, DWORD dwLeaveReason)
{
	if ( pIServerUserItem == NULL ) return;

	//变量定义
	DBR_GR_LeaveGameServer LeaveGameServer;
	ZeroMemory(&LeaveGameServer,sizeof(LeaveGameServer));

	//提取成绩
	pIServerUserItem->QueryRecordInfo(LeaveGameServer.RecordInfo);
	pIServerUserItem->DistillVariation(LeaveGameServer.VariationInfo);

	//用户信息
	LeaveGameServer.dwLeaveReason=dwLeaveReason;
	LeaveGameServer.dwUserID=pIServerUserItem->GetUserID();
	LeaveGameServer.dwInoutIndex=pIServerUserItem->GetInoutIndex();
	//在线时长即为游戏时长,以分钟为单位
	LeaveGameServer.dwOnLineTimeCount=(DWORD)(time(NULL))-pIServerUserItem->GetLogonTime();

	//连接信息
	LeaveGameServer.dwClientAddr=pIServerUserItem->GetClientAddr();
	lstrcpyn(LeaveGameServer.szMachineID,pIServerUserItem->GetMachineID(),CountArray(LeaveGameServer.szMachineID));
	
	WORD wBindIndex = pIServerUserItem->GetBindIndex();
	tagBindParameter * pBindParameter = GetBindParameter(wBindIndex);
	if (pBindParameter) {
		I_TRACE(TEXT("Socket[%d], %s 用户离开"), pBindParameter->dwSocketID, pIServerUserItem->GetNickName());
		m_ServerFileLog.Log(TEXT("【用户离开写数据库】|用户[%d][%s],分数[%I64d] 状态[%d] 桌椅[%d-%d] BindIndex[%d] SocketID[%d]"),
			pIServerUserItem->GetUserID(),pIServerUserItem->GetNickName(),pIServerUserItem->GetUserScore(),
			pIServerUserItem->GetUserStatus(),pIServerUserItem->GetTableID(),pIServerUserItem->GetChairID(),wBindIndex,pBindParameter->dwSocketID);
	}
	else
	{
		CString cs;
		cs.Format(TEXT("非正常离开[%d],BindIndex[%d]"), pIServerUserItem->GetUserID(), pIServerUserItem->GetBindIndex());
		m_ServerFileLog.Log(cs.GetBuffer(0));
		I_TRACE(cs.GetBuffer(0));
	}
	
	//投递请求
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_LEAVE_GAME_SERVER,0L,&LeaveGameServer,sizeof(LeaveGameServer), TRUE);

	//汇总用户
	if (m_bCollectUser==true)
	{
		//变量定义
		CMD_CS_C_UserLeave UserLeave;
		ZeroMemory(&UserLeave,sizeof(UserLeave));

		//设置变量
		UserLeave.dwUserID=pIServerUserItem->GetUserID();

		//发送消息
		m_pITCPSocketService->SendData(MDM_CS_USER_COLLECT,SUB_CS_C_USER_LEAVE,&UserLeave,sizeof(UserLeave));
	}

	DeleteDistribute(pIServerUserItem);

	//比赛房间退出不投递数据库
	if(m_pIGameMatchServiceManager!=NULL)m_pIGameMatchServiceManager->OnUserQuitGame(pIServerUserItem, 1);

	//删除用户
	m_ServerUserManager.DeleteUserItem(pIServerUserItem);

	return;
}

//解锁元宝
bool CAttemperEngineSink::PerformUnlockScore(DWORD dwUserID, DWORD dwInoutIndex, DWORD dwLeaveReason)
{
	//变量定义
	DBR_GR_LeaveGameServer LeaveGameServer;
	ZeroMemory(&LeaveGameServer,sizeof(LeaveGameServer));

	//设置变量
	LeaveGameServer.dwUserID=dwUserID;
	LeaveGameServer.dwInoutIndex=dwInoutIndex;
	LeaveGameServer.dwLeaveReason=dwLeaveReason;

	//投递请求
	m_pIDBCorrespondManager->PostDataBaseRequest(dwUserID,DBR_GR_LEAVE_GAME_SERVER,0L,&LeaveGameServer,sizeof(LeaveGameServer));

	return true;
}

//版本检查
bool CAttemperEngineSink::PerformCheckVersion(DWORD dwPlazaVersion, DWORD dwFrameVersion, DWORD dwClientVersion, DWORD dwSocketID)
{
	return true;
}

//切换连接
bool CAttemperEngineSink::SwitchUserItemConnectH5(IServerUserItem * pIServerUserItem, TCHAR szMachineID[LEN_MACHINE_ID], WORD wTargetIndex)
{
	//效验参数
	ASSERT_ERR((pIServerUserItem != NULL) && (wTargetIndex != INVALID_WORD));
	if ((pIServerUserItem == NULL) || (wTargetIndex == INVALID_WORD)) return false;

	//断开用户
	if (pIServerUserItem->GetBindIndex() != INVALID_WORD)
	{
		//绑定参数
		WORD wSourceIndex = pIServerUserItem->GetBindIndex();
		tagBindParameter * pSourceParameter = GetBindParameter(wSourceIndex);
		tagBindParameter * pTargetParameter = GetBindParameter(wTargetIndex);
		I_TRACE(TEXT("wSourceIndex[%d],SrcSocketID[%d],wTargetIndex[%d],TargetSocketID[%d]"),wSourceIndex, pSourceParameter->dwSocketID,
			wTargetIndex, pTargetParameter->dwSocketID);
		//解除绑定
		//ASSERT_ERR((pSourceParameter != NULL) && (pSourceParameter->pIServerUserItem == pIServerUserItem));
		if ((pSourceParameter != NULL) && (pSourceParameter->pIServerUserItem == pIServerUserItem)) {
			//发送通知
			LPCTSTR pszMessage = TEXT("您的账号在另一个地方登录，您被迫离开!");
			if (pIServerUserItem->IsAndroidUser() == false)
				SendRequestFailureH5(pIServerUserItem, pszMessage, REQUEST_FAILURE_QUIT);
			pSourceParameter->pIServerUserItem = NULL;
		}	

		//断开用户
		if (pIServerUserItem->IsAndroidUser() == true)
		{
			//m_AndroidUserManager.DeleteAndroidUserItem(pSourceParameter->dwSocketID);
		}
		else
		{
			I_TRACE(TEXT("关闭旧连接wSourceIndex:%d,dwSocketID:%d"), wSourceIndex, pSourceParameter->dwSocketID);
			//if ( m_pGameServiceOption->wKindID != KIND_ZJH)
			//m_pITCPNetworkEngine->ShutDownSocket(pSourceParameter->dwSocketID);
		}
	}

	//变量定义
	bool bAndroidUser = (wTargetIndex >= INDEX_ANDROID);
	tagBindParameter * pTargetParameter = GetBindParameter(wTargetIndex);

	//激活用户
	I_TRACE(TEXT("设置新连接 wTargetIndex:%d,dwSocketID:%d"), wTargetIndex, pTargetParameter->dwSocketID);
	pTargetParameter->pIServerUserItem = pIServerUserItem;
	pIServerUserItem->SetUserParameter(pTargetParameter->dwClientAddr, wTargetIndex, szMachineID, bAndroidUser, false);

	//登录事件
	OnEventUserLogonH5(pIServerUserItem, true);

	//状态切换
	bool bIsOffLine = false;
	//if (pIServerUserItem->GetUserStatus() == US_OFFLINE || pIServerUserItem->GetUserStatus() == US_PLAYING)
	if (pIServerUserItem->GetUserStatus() >= US_PLAYING && m_pGameServiceOption->wKindID == KIND_FISH)
	{
		//变量定义
		WORD wTableID = pIServerUserItem->GetTableID();
		WORD wChairID = pIServerUserItem->GetChairID();

		pIServerUserItem->SetHeartTime((DWORD)time(NULL));
		I_TRACE(TEXT("重连玩家 %s %d-%d Status:%d"), pIServerUserItem->GetNickName(),pIServerUserItem->GetTableID(),pIServerUserItem->GetChairID(),pIServerUserItem->GetUserStatus());
		//设置状态
		bIsOffLine = true;
		pIServerUserItem->SetEnterTable(FALSE);
		pIServerUserItem->SetUserStatus(US_PLAYING, wTableID, wChairID);
	}

	//机器判断
	LPCTSTR pszMachineID = pIServerUserItem->GetMachineID();
	bool bSameMachineID = (lstrcmp(pszMachineID, szMachineID) == 0);

	//安全提示
	if ((bAndroidUser == false) && (bIsOffLine == false) && (bSameMachineID == false))
	{
		//不用告知自己
		//SendRoomMessage(pIServerUserItem, TEXT("请注意，您的帐号在另一地方进入了此游戏房间，对方被迫离开！"), SMT_EJECT | SMT_CHAT | SMT_GLOBAL);
	}

	return true;
}

//切换连接
bool CAttemperEngineSink::SwitchUserItemConnect(IServerUserItem * pIServerUserItem, TCHAR szMachineID[LEN_MACHINE_ID], WORD wTargetIndex,BYTE cbDeviceType,WORD wBehaviorFlags,WORD wPageTableCount)
{
	//效验参数
	ASSERT_ERR((pIServerUserItem!=NULL)&&(wTargetIndex!=INVALID_WORD));
	if ((pIServerUserItem==NULL)||(wTargetIndex==INVALID_WORD)) return false;

	//断开用户
	if (pIServerUserItem->GetBindIndex()!=INVALID_WORD)
	{
		//发送通知
		LPCTSTR pszMessage=TEXT("请注意，您的帐号在另一地方进入了此游戏房间，您被迫离开！");
		if (pIServerUserItem->IsAndroidUser() == false )
			SendRoomMessage(pIServerUserItem,pszMessage,SMT_CHAT|SMT_EJECT|SMT_GLOBAL|SMT_CLOSE_ROOM);

		//绑定参数
		WORD wSourceIndex=pIServerUserItem->GetBindIndex();
		tagBindParameter * pSourceParameter=GetBindParameter(wSourceIndex);

		//解除绑定
		ASSERT_ERR((pSourceParameter!=NULL)&&(pSourceParameter->pIServerUserItem==pIServerUserItem));
		if ((pSourceParameter!=NULL)&&(pSourceParameter->pIServerUserItem==pIServerUserItem)) pSourceParameter->pIServerUserItem=NULL;

		//断开用户
		if (pIServerUserItem->IsAndroidUser()==true)
		{
			//m_AndroidUserManager.DeleteAndroidUserItem(pSourceParameter->dwSocketID);
		}
		else
		{
			m_pITCPNetworkEngine->ShutDownSocket(pSourceParameter->dwSocketID);
		}
	}

	//状态切换
	bool bIsOffLine=false;
	if (pIServerUserItem->GetUserStatus()==US_OFFLINE)
	{
		//变量定义
		WORD wTableID=pIServerUserItem->GetTableID();
		WORD wChairID=pIServerUserItem->GetChairID();

		pIServerUserItem->SetHeartTime((DWORD)time(NULL));
		//设置状态
		bIsOffLine=true;
		pIServerUserItem->SetUserStatus(US_PLAYING,wTableID,wChairID);
	}

	//机器判断
	LPCTSTR pszMachineID=pIServerUserItem->GetMachineID();
	bool bSameMachineID=(lstrcmp(pszMachineID,szMachineID)==0);

	//变量定义
	bool bAndroidUser=(wTargetIndex>=INDEX_ANDROID);
	tagBindParameter * pTargetParameter=GetBindParameter(wTargetIndex);

	//激活用户
	pTargetParameter->pIServerUserItem=pIServerUserItem;
	pIServerUserItem->SetUserParameter(pTargetParameter->dwClientAddr,wTargetIndex,szMachineID,bAndroidUser,false);

	//手机标识
	if(pTargetParameter->cbClientKind==CLIENT_KIND_MOBILE)
	{
		pIServerUserItem->SetMobileUser(true);
		SetMobileUserParameter(pIServerUserItem,cbDeviceType,wBehaviorFlags,wPageTableCount);
	}

	//登录事件
	OnEventUserLogon(pIServerUserItem,true);

	//安全提示
	if ((bAndroidUser==false)&&(bIsOffLine==false)&&(bSameMachineID==false))
	{
		SendRoomMessage(pIServerUserItem,TEXT("请注意，您的帐号在另一地方进入了此游戏房间，对方被迫离开！"),SMT_EJECT|SMT_CHAT|SMT_GLOBAL);
	}

	return true;
}

//登录失败
bool CAttemperEngineSink::SendLogonFailure(LPCTSTR pszString, LONG lErrorCode, DWORD dwSocketID)
{
	tagBindParameter * pBindParameter = GetBindParameter(LOWORD(dwSocketID));
	if ( pBindParameter )
	{
		if ( pBindParameter->cbClientKind == CLIENT_KIND_WEBSOCKET)
		{
			GameServer::GSLogonFailure gsLogonFailure;
			gsLogonFailure.set_errorcode(lErrorCode);
			gsLogonFailure.set_describestring(Utils::ConvertFromUtf16ToUtf8(pszString).c_str());

			std::string serializeStr;
			gsLogonFailure.SerializeToString(&serializeStr);

			

			//客户端断开
			//m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);
			IServerUserItem* pIServerUserItem = pBindParameter->pIServerUserItem;
			if (pIServerUserItem)
				pIServerUserItem->SetUserStatus(US_NULL, INVALID_TABLE, INVALID_CHAIR);

			SendData(dwSocketID, MDM_GR_LOGON, SUB_GR_LOGON_FAILURE, (void*)serializeStr.c_str(), (WORD)serializeStr.size());
			//OnEventUserLogout(pBindParameter->pIServerUserItem, 0L);
		}
		else if (pBindParameter->cbClientKind == CLIENT_KIND_MOBILE)
		{
			//变量定义
			CMD_GR_LogonFailure LogonFailure;
			ZeroMemory(&LogonFailure, sizeof(LogonFailure));

			//构造数据
			LogonFailure.lErrorCode = lErrorCode;
			lstrcpyn(LogonFailure.szDescribeString, pszString, CountArray(LogonFailure.szDescribeString));

			//数据属性
			WORD wDataSize = CountStringBuffer(LogonFailure.szDescribeString);
			WORD wHeadSize = sizeof(LogonFailure) - sizeof(LogonFailure.szDescribeString);

			//发送数据
			SendData(dwSocketID, MDM_GR_LOGON, SUB_GR_LOGON_FAILURE, &LogonFailure, wHeadSize + wDataSize);

			m_pITCPNetworkEngine->ShutDownSocket(pBindParameter->dwSocketID);
		}
	}

	return true;
}

//银行失败
bool CAttemperEngineSink::SendInsureFailure(IServerUserItem * pIServerUserItem, LPCTSTR pszString, LONG lErrorCode,BYTE cbActivityGame)
{
	//效验参数
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//变量定义
	CMD_GR_S_UserInsureFailure UserInsureFailure;
	ZeroMemory(&UserInsureFailure,sizeof(UserInsureFailure));

	//构造数据
	UserInsureFailure.cbActivityGame=cbActivityGame;
	UserInsureFailure.lErrorCode=lErrorCode;
	lstrcpyn(UserInsureFailure.szDescribeString,pszString,CountArray(UserInsureFailure.szDescribeString));

	//数据属性
	WORD wDataSize=CountStringBuffer(UserInsureFailure.szDescribeString);
	WORD wHeadSize=sizeof(UserInsureFailure)-sizeof(UserInsureFailure.szDescribeString);

	//发送数据
	SendData(pIServerUserItem,MDM_GR_INSURE,SUB_GR_USER_INSURE_FAILURE,&UserInsureFailure,wHeadSize+wDataSize);

	return true;
}

//请求失败
bool CAttemperEngineSink::SendRequestFailure(IServerUserItem * pIServerUserItem, LPCTSTR pszDescribe, LONG lErrorCode)
{
	//变量定义
	CMD_GR_RequestFailure RequestFailure;
	ZeroMemory(&RequestFailure,sizeof(RequestFailure));

	//设置变量
	RequestFailure.lErrorCode=lErrorCode;
	lstrcpyn(RequestFailure.szDescribeString,pszDescribe,CountArray(RequestFailure.szDescribeString));

	//发送数据
	WORD wDataSize=CountStringBuffer(RequestFailure.szDescribeString);
	WORD wHeadSize=sizeof(RequestFailure)-sizeof(RequestFailure.szDescribeString);
	SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_REQUEST_FAILURE,&RequestFailure,wHeadSize+wDataSize);

	return true;
}

//请求失败
bool CAttemperEngineSink::SendRequestFailureH5(IServerUserItem * pIServerUserItem, LPCTSTR pszDescribe, LONG lErrorCode)
{
	GameServer::GSUserRequestFailure gsUserRequestFailure;
	gsUserRequestFailure.set_errorcode(lErrorCode);
	gsUserRequestFailure.set_describestring(Utils::ConvertFromUtf16ToUtf8(pszDescribe).c_str());

	std::string serializeStr;
	gsUserRequestFailure.SerializeToString(&serializeStr);

	SendData(pIServerUserItem, MDM_GR_USER, SUB_GR_REQUEST_FAILURE, (void*)serializeStr.c_str(), (WORD)serializeStr.size());


	if ( lErrorCode == REQUEST_FAILURE_NETWORK)
	{
		tagBindParameter * pBindParameter = GetBindParameter(pIServerUserItem->GetBindIndex());
		if (pBindParameter == NULL) return true;
		//请求失败，断开连接
		if (pBindParameter->cbClientKind == CLIENT_KIND_WEBSOCKET)
		{
			m_pITCPNetworkEngine->ShutDownSocket(pBindParameter->dwSocketID);
		}
	}
	return true;
}

//道具失败
bool CAttemperEngineSink::SendPropertyFailure(IServerUserItem * pIServerUserItem, LPCTSTR pszDescribe, LONG lErrorCode,WORD wRequestArea)
{
	//变量定义
	CMD_GR_PropertyFailure PropertyFailure;
	ZeroMemory(&PropertyFailure,sizeof(PropertyFailure));

	//设置变量
	PropertyFailure.lErrorCode=lErrorCode;
	PropertyFailure.wRequestArea=wRequestArea;
	lstrcpyn(PropertyFailure.szDescribeString,pszDescribe,CountArray(PropertyFailure.szDescribeString));

	//发送数据
	WORD wDataSize=CountStringBuffer(PropertyFailure.szDescribeString);
	WORD wHeadSize=sizeof(PropertyFailure)-sizeof(PropertyFailure.szDescribeString);
	SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_PROPERTY_FAILURE,&PropertyFailure,wHeadSize+wDataSize);

	return true;
}

//发送用户
bool CAttemperEngineSink::SendUserInfoPacketH5(IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
	if (pIServerUserItem == NULL) return false;

	tagUserInfo* pUserInfo = pIServerUserItem->GetUserInfo();
	GameServer::GSUserEnter gsUserEnter;
	gsUserEnter.set_userid((int32)pUserInfo->dwUserID);
	gsUserEnter.set_faceid((int32)pUserInfo->wFaceID);
	gsUserEnter.set_gender((int32)pUserInfo->cbGender);
	gsUserEnter.set_score((int64)pUserInfo->lScore);
	gsUserEnter.set_nickname(Utils::ConvertFromUtf16ToUtf8(pUserInfo->szNickName));
	gsUserEnter.mutable_userstatus()->set_tableid((int32)pUserInfo->wTableID);
	gsUserEnter.mutable_userstatus()->set_chairid((int32)pUserInfo->wChairID);
	gsUserEnter.mutable_userstatus()->set_userstatus((GameServer::enumUserStatus)pUserInfo->cbUserStatus);
	
	std::string serializeStr;
	gsUserEnter.SerializeToString(&serializeStr);
	SendData(dwSocketID, MDM_GR_USER, SUB_GR_USER_ENTER, (void*)serializeStr.c_str(), (WORD)serializeStr.size());

	return true;
}

//发送用户
bool CAttemperEngineSink::SendUserInfoPacket(IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
	//效验参数
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

// 	tagUserInfo* ptagUserInfo = pIServerUserItem->GetUserInfo();
// 	if (ptagUserInfo->cbClientKind == CLIENT_KIND_WEBSOCKET) {
// 		SendUserInfoPacketH5(pIServerUserItem, dwSocketID);
// 		return true;
// 	}

	//变量定义
	BYTE cbBuffer[SOCKET_TCP_PACKET];
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();
	tagUserInfoHead * pUserInfoHead=(tagUserInfoHead *)cbBuffer;
	CSendPacketHelper SendPacket(cbBuffer+sizeof(tagUserInfoHead),sizeof(cbBuffer)-sizeof(tagUserInfoHead));

	//用户属性
	pUserInfoHead->wFaceID=pUserInfo->wFaceID;
	pUserInfoHead->dwGameID=pUserInfo->dwGameID;
	pUserInfoHead->dwUserID=pUserInfo->dwUserID;
	pUserInfoHead->dwGroupID=pUserInfo->dwGroupID;
	pUserInfoHead->dwCustomID=pUserInfo->dwCustomID;

	//用户属性
	pUserInfoHead->cbGender=pUserInfo->cbGender;
	pUserInfoHead->cbMemberOrder=pUserInfo->cbMemberOrder;
	pUserInfoHead->cbMasterOrder=pUserInfo->cbMasterOrder;

	//用户状态
	pUserInfoHead->wTableID=pUserInfo->wTableID;
	pUserInfoHead->wChairID=pUserInfo->wChairID;
	pUserInfoHead->cbUserStatus=pUserInfo->cbUserStatus;

	//用户局数
	pUserInfoHead->dwWinCount=pUserInfo->dwWinCount;
	pUserInfoHead->dwLostCount=pUserInfo->dwLostCount;
	pUserInfoHead->dwDrawCount=pUserInfo->dwDrawCount;
	pUserInfoHead->dwFleeCount=pUserInfo->dwFleeCount;
	pUserInfoHead->dwUserMedal=pUserInfo->dwUserMedal;
	pUserInfoHead->dwExperience=pUserInfo->dwExperience;
	pUserInfoHead->lLoveLiness=pUserInfo->lLoveLiness;

	//用户积分
	pUserInfoHead->lGrade=pUserInfo->lGrade;
	pUserInfoHead->lInsure=pUserInfo->lInsure;

	//用户成绩
	pUserInfoHead->lScore=pUserInfo->lScore;
// 	if ( m_pGameServiceOption->dwRoomID == 0)
// 	{
// 		pUserInfoHead->lScore+=pIServerUserItem->GetTrusteeScore();
// 		pUserInfoHead->lScore+=pIServerUserItem->GetFrozenedScore();
// 	}
	pUserInfoHead->lRevenue			= pUserInfo->lRevenue;
	pUserInfoHead->dwAvatarID		= pUserInfo->wAvatarID;
	pUserInfoHead->lRechargeScore = pUserInfo->lRechargeScore;
	pUserInfoHead->lExchangeScore = pUserInfo->lExchangeScore;

	//叠加信息
	SendPacket.AddPacket(pUserInfo->szNickName,DTP_GR_NICK_NAME);
	SendPacket.AddPacket(pUserInfo->szGroupName,DTP_GR_GROUP_NAME);
	SendPacket.AddPacket(pUserInfo->szUnderWrite,DTP_GR_UNDER_WRITE);
	
	//发送数据
	if (dwSocketID==INVALID_DWORD)
	{
		WORD wHeadSize=sizeof(tagUserInfoHead);
		SendData(BG_COMPUTER, MDM_GR_USER,SUB_GR_USER_ENTER,cbBuffer,wHeadSize+SendPacket.GetDataSize());
		SendData(BG_MOBILE, MDM_GR_USER, SUB_GR_USER_ENTER, cbBuffer, wHeadSize + SendPacket.GetDataSize());
		SendUserInfoPacketBatchToMobileUser(pIServerUserItem);
	}
	else
	{
		WORD wHeadSize=sizeof(tagUserInfoHead);
		SendData(dwSocketID,MDM_GR_USER,SUB_GR_USER_ENTER,cbBuffer,wHeadSize+SendPacket.GetDataSize());
	}

	return true;
}

//广播道具
bool CAttemperEngineSink::SendPropertyMessage(DWORD dwSourceID,DWORD dwTargerID,WORD wPropertyIndex,WORD wPropertyCount)
{
	//构造结构
	CMD_GR_S_PropertyMessage  PropertyMessage;
	PropertyMessage.wPropertyIndex=wPropertyIndex;
	PropertyMessage.dwSourceUserID=dwSourceID;
	PropertyMessage.dwTargerUserID=dwTargerID;
	PropertyMessage.wPropertyCount=wPropertyCount;

	//在线用户
	WORD wUserIndex=0;
	IServerUserItem * pIServerUserItemSend=NULL;
	while (true)
	{
		pIServerUserItemSend=m_ServerUserManager.EnumUserItem(wUserIndex++);
		if (pIServerUserItemSend==NULL) break;
		SendData(pIServerUserItemSend,MDM_GR_USER,SUB_GR_PROPERTY_MESSAGE,&PropertyMessage,sizeof(PropertyMessage));
	}

	return true;
}

//道具效应
bool CAttemperEngineSink::SendPropertyEffect(IServerUserItem * pIServerUserItem)
{
	//参数校验
	if(pIServerUserItem==NULL) return false;

	//构造结构
	CMD_GR_S_PropertyEffect  PropertyEffect;
	PropertyEffect.wUserID =pIServerUserItem->GetUserID();
	PropertyEffect.cbMemberOrder=pIServerUserItem->GetMemberOrder();

	//在线用户
	WORD wUserIndex=0;
	IServerUserItem * pIServerUserItemSend=NULL;
	while (true)
	{
		pIServerUserItemSend=m_ServerUserManager.EnumUserItem(wUserIndex++);
		if (pIServerUserItemSend==NULL) break;
		SendData(pIServerUserItemSend,MDM_GR_USER,SUB_GR_PROPERTY_EFFECT,&PropertyEffect,sizeof(PropertyEffect));
	}

	return true;
}

//道具事件
bool CAttemperEngineSink::OnEventPropertyBuyPrep(WORD cbRequestArea,WORD wPropertyIndex,IServerUserItem *pISourceUserItem,IServerUserItem *pTargetUserItem)
{
	//目标玩家
	if ( pTargetUserItem == NULL )
	{
		//发送消息
		SendPropertyFailure(pISourceUserItem,TEXT("赠送失败，您要赠送的玩家已经离开！"), 0L,cbRequestArea);

		return false;
	}

	//房间判断
	if ( (m_pGameServiceOption->wServerType == GAME_GENRE_GOLD || m_pGameServiceOption->wServerType == GAME_GENRE_MATCH ) && 
		(wPropertyIndex== PROPERTY_ID_SCORE_CLEAR||wPropertyIndex==PROPERTY_ID_TWO_CARD||wPropertyIndex == PROPERTY_ID_FOUR_CARD||wPropertyIndex == PROPERTY_ID_POSSESS) )
	{
		//发送消息
		SendPropertyFailure(pISourceUserItem,TEXT("此房间不可以使用此道具,购买失败"), 0L,cbRequestArea);

		return false;
	}

	//查找道具
	tagPropertyInfo * pPropertyInfo=m_GamePropertyManager.SearchPropertyItem(wPropertyIndex);

	//有效效验
	if(pPropertyInfo==NULL)
	{
		//发送消息
		SendPropertyFailure(pISourceUserItem,TEXT("此道具还未启用,购买失败！"), 0L,cbRequestArea);

		return false;
	}

	//自己使用
	if((pPropertyInfo->wIssueArea&PT_SERVICE_AREA_MESELF)==0 && pISourceUserItem==pTargetUserItem) 
	{
		//发送消息
		SendPropertyFailure(pISourceUserItem,TEXT("此道具不可自己使用,购买失败！"), 0L,cbRequestArea);

		return false;
	}

	//玩家使用
	if((pPropertyInfo->wIssueArea&PT_SERVICE_AREA_PLAYER)==0 && pISourceUserItem!=pTargetUserItem) 
	{
		//发送消息
		SendPropertyFailure(pISourceUserItem,TEXT("此道具不可赠送给玩家,只能自己使用,购买失败！"), 0L,cbRequestArea);

		return false;
	}

	//旁观范围
	if((pPropertyInfo->wIssueArea&PT_SERVICE_AREA_LOOKON)==0)  
	{
		//变量定义
		WORD wTableID = pTargetUserItem->GetTableID();
		if(wTableID!=INVALID_TABLE)
		{
			//变量定义
			WORD wEnumIndex=0;
			IServerUserItem * pIServerUserItem=NULL;

			//获取桌子
			CTableFrame * pTableFrame=m_TableFrameArray[wTableID];

			//枚举用户
			do
			{
				//获取用户
				pIServerUserItem=pTableFrame->EnumLookonUserItem(wEnumIndex++);
				if( pIServerUserItem==NULL) break;
				if( pIServerUserItem==pTargetUserItem )
				{
					//发送消息
					SendPropertyFailure(pISourceUserItem,TEXT("此道具不可赠送给旁观用户,购买失败！"), 0L,cbRequestArea);

					return false;
				}
			} while (true);
		}
	}

	//道具判断
	switch(wPropertyIndex)
	{
	case PROPERTY_ID_SCORE_CLEAR :			//负分清零
		{
			//变量定义
			SCORE lCurrScore = pTargetUserItem->GetUserScore();
			if( lCurrScore >= 0)
			{
				//变量定义
				TCHAR szMessage[128]=TEXT("");
				if ( pISourceUserItem==pTargetUserItem ) 
					_sntprintf(szMessage,CountArray(szMessage),TEXT("您现在的积分已经是非负数，不需要使用负分清零道具！"));
				else
					_sntprintf(szMessage,CountArray(szMessage),TEXT("[ %s ]现在的积分已经是非负数，不需要使用负分清零道具！"), pTargetUserItem->GetNickName());

				//发送消息
				SendPropertyFailure(pISourceUserItem,szMessage, 0L,cbRequestArea);

				return false;
			}
			break;
		}
	case PROPERTY_ID_ESCAPE_CLEAR :			 //逃跑清零
		{
			//变量定义
			DWORD dwCurrFleeCount = pTargetUserItem->GetUserInfo()->dwFleeCount;
			if ( dwCurrFleeCount==0 )
			{
				//变量定义
				TCHAR szMessage[128]=TEXT("");		
				if ( pISourceUserItem == pTargetUserItem ) 
					_sntprintf(szMessage,CountArray(szMessage),TEXT("您现在的逃跑率已经为0，不需要使用逃跑清零道具！"));
				else
					_sntprintf(szMessage,CountArray(szMessage),TEXT("[ %s ]现在的逃跑率已经为0，不需要使用逃跑清零道具！"), pTargetUserItem->GetNickName());

				//发送消息
				SendPropertyFailure(pISourceUserItem,szMessage,0L,cbRequestArea);

				return false;
			}
			break;
		}
	}

	return true;
}

//绑定用户
IServerUserItem * CAttemperEngineSink::GetBindUserItem(WORD wBindIndex)
{
	//获取参数
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	//获取用户
	if (pBindParameter!=NULL)
	{
		return pBindParameter->pIServerUserItem;
	}

	//错误断言
	ASSERT_ERR(FALSE);

	return NULL;
}

//绑定参数
tagBindParameter * CAttemperEngineSink::GetBindParameter(WORD wBindIndex)
{
	//无效连接
	if (wBindIndex==INVALID_WORD) return NULL;

	//常规连接
	if (wBindIndex<m_pGameServiceOption->wMaxPlayer)
	{
		return m_pNormalParameter+wBindIndex;
	}

	//机器连接
	if ((wBindIndex>=INDEX_ANDROID)&&(wBindIndex<(INDEX_ANDROID+MAX_ANDROID)))
	{
		return m_pAndroidParameter+(wBindIndex-INDEX_ANDROID);
	}

	//错误断言
	ASSERT_ERR(FALSE);

	return NULL;
}

//道具类型
WORD CAttemperEngineSink::GetPropertyType(WORD wPropertyIndex)
{
	switch(wPropertyIndex)
	{
	case PROPERTY_ID_CAR:	case PROPERTY_ID_EGG: 	case PROPERTY_ID_CLAP: 	case PROPERTY_ID_KISS: 	case PROPERTY_ID_BEER:
	case PROPERTY_ID_CAKE: 	case PROPERTY_ID_RING:  case PROPERTY_ID_BEAT: 	case PROPERTY_ID_BOMB:  case PROPERTY_ID_SMOKE:
	case PROPERTY_ID_VILLA: case PROPERTY_ID_BRICK: case PROPERTY_ID_FLOWER: 
		{
			return PT_TYPE_PRESENT;
		};
	case PROPERTY_ID_TWO_CARD: 	case PROPERTY_ID_FOUR_CARD:  case PROPERTY_ID_SCORE_CLEAR:     case PROPERTY_ID_ESCAPE_CLEAR:
	case PROPERTY_ID_TRUMPET:	case PROPERTY_ID_TYPHON:     case PROPERTY_ID_GUARDKICK_CARD:  case PROPERTY_ID_POSSESS:
	case PROPERTY_ID_BLUERING_CARD: case PROPERTY_ID_YELLOWRING_CARD: case PROPERTY_ID_WHITERING_CARD: case PROPERTY_ID_REDRING_CARD:
	case PROPERTY_ID_VIPROOM_CARD: 
		{
			return PT_TYPE_PROPERTY;
		};
	}

	ASSERT_ERR(false);

	return PT_TYPE_ERROR;
}

//配置机器
bool CAttemperEngineSink::InitAndroidUser()
{
	//机器参数
	tagAndroidUserParameter AndroidUserParameter;
	ZeroMemory(&AndroidUserParameter,sizeof(AndroidUserParameter));

	//配置参数
	AndroidUserParameter.pGameParameter=m_pGameParameter;
	AndroidUserParameter.pGameServiceAttrib=m_pGameServiceAttrib;
	AndroidUserParameter.pGameServiceOption=m_pGameServiceOption;

	//服务组件
	AndroidUserParameter.pITimerEngine=m_pITimerEngine;
	AndroidUserParameter.pIServerUserManager=&m_ServerUserManager;
	AndroidUserParameter.pIGameServiceManager=m_pIGameServiceManager;
	AndroidUserParameter.pITCPNetworkEngineEvent=QUERY_OBJECT_PTR_INTERFACE(m_pIAttemperEngine,ITCPNetworkEngineEvent);
	AndroidUserParameter.pIDabaBaseEngine=m_pIKernelDataBaseEngine;

	CopyMemory(AndroidUserParameter.guiRoom, m_guidRoom, 40 * sizeof WCHAR);

	//设置对象
	if (m_AndroidUserManager.InitAndroidUser(AndroidUserParameter)==false)
	{
		return false;
	}

	return true;
}

//配置桌子
bool CAttemperEngineSink::InitTableFrameArray()
{
	//桌子参数
	tagTableFrameParameter TableFrameParameter;
	ZeroMemory(&TableFrameParameter,sizeof(TableFrameParameter));

	//内核组件
	TableFrameParameter.pITimerEngine=m_pITimerEngine;
	TableFrameParameter.pIKernelDataBaseEngine=m_pIKernelDataBaseEngine;
	TableFrameParameter.pIRecordDataBaseEngine=m_pIRecordDataBaseEngine;

	//服务组件
	TableFrameParameter.pIMainServiceFrame=this;
	TableFrameParameter.pIAndroidUserManager=&m_AndroidUserManager;
	TableFrameParameter.pIGameServiceManager=m_pIGameServiceManager;

	//配置参数
	TableFrameParameter.pGameParameter=m_pGameParameter;
	TableFrameParameter.pGameServiceAttrib=m_pGameServiceAttrib;
	TableFrameParameter.pGameServiceOption=m_pGameServiceOption;

	if(m_pIGameMatchServiceManager!=NULL)
		TableFrameParameter.pIGameMatchServiceManager=QUERY_OBJECT_PTR_INTERFACE(m_pIGameMatchServiceManager,IUnknownEx);

	//桌子容器
	m_TableFrameArray.SetSize(m_pGameServiceOption->wTableCount);
	ZeroMemory(m_TableFrameArray.GetData(),m_pGameServiceOption->wTableCount*sizeof(CTableFrame *));

	//创建桌子
	for (WORD i=0;i<m_pGameServiceOption->wTableCount;i++)
	{
		//创建对象
		m_TableFrameArray[i]=new CTableFrame;

		//配置桌子
		if (m_TableFrameArray[i]->InitializationFrame(i,TableFrameParameter)==false)
		{
			return false;
		}

		if(m_pIGameMatchServiceManager!=NULL)
			m_pIGameMatchServiceManager->InitTableFrame(QUERY_OBJECT_PTR_INTERFACE((m_TableFrameArray[i]),ITableFrame),i);

		m_AndroidUserManager.InitTableFrame(QUERY_OBJECT_PTR_INTERFACE((m_TableFrameArray[i]),ITableFrame),i);
	}

	if(m_pIGameMatchServiceManager!=NULL)
	{
		if (m_pIGameMatchServiceManager->InitMatchInterface(QUERY_OBJECT_PTR_INTERFACE(m_pIAttemperEngine,ITCPNetworkEngineEvent),m_pIKernelDataBaseEngine,
			(IServerUserManager*)QUERY_OBJECT_INTERFACE(m_ServerUserManager,IServerUserManager),this,m_pITimerEngine,&m_AndroidUserManager, m_guidRoom)==false)
		{
			ASSERT_ERR(FALSE);
			return false;
		}
		// 		if (m_pIGameMatchServiceManager->InitServerUserManager()==false)
		// 		{
		// 			ASSERT_ERR(FALSE);
		// 			return false;
		// 		}
		// 
		// 		if(m_pIGameMatchServiceManager->InitMainServiceFrame(QUERY_ME_INTERFACE(IMainServiceFrame))==false)
		// 		{
		// 			ASSERT_ERR(FALSE);
		// 			return false;
		// 		}

	}

	return true;
}

bool CAttemperEngineSink::SendServerInfo()
{
	CP_ControlResult ControlResult;
	ControlResult.cbSuccess=ER_SUCCESS;
	SendUIControlPacket(UI_SERVICE_CONFIG_RESULT,&ControlResult,sizeof(ControlResult));

	return true;
}

bool CAttemperEngineSink::SendTaskComplete(DWORD dwUserID, TCHAR* szEventCode)
{
	//m_task.DoJob(dwUserID, szEventCode);
	return true;
}

//发送请求
bool CAttemperEngineSink::SendUIControlPacket(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	//发送数据
	CServiceUnits * pServiceUnits=CServiceUnits::g_pServiceUnits;
	pServiceUnits->PostControlRequest(wRequestID,pData,wDataSize);

	return true;
}

//插入分配
bool CAttemperEngineSink::InsertDistribute(IServerUserItem * pIServerUserItem)
{
	//效验参数
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//状态判断
	ASSERT_ERR(pIServerUserItem->GetUserStatus()<US_PLAYING);
	if (pIServerUserItem->GetUserStatus()>=US_PLAYING) return false;


	//变量定义
	tagDistributeInfo DistributeInfo;
	ZeroMemory(&DistributeInfo,sizeof(DistributeInfo));

	//设置变量
	DistributeInfo.pIServerUserItem=pIServerUserItem;
	DistributeInfo.wLastTableID=pIServerUserItem->GetTableID();
	DistributeInfo.pPertainNode=NULL;

	//加入数组
	if(m_DistributeManage.InsertDistributeNode(DistributeInfo)==false)
	{
		//PerformDistribute();
		return pIServerUserItem->IsAndroidUser()?false:true; 
	}

	//用户起立
	if (pIServerUserItem->GetTableID()!=INVALID_TABLE)
	{
		WORD wTableID=pIServerUserItem->GetTableID();
		m_TableFrameArray[wTableID]->PerformStandUpAction(pIServerUserItem);
	}

	//PerformDistribute(); //由定时器执行

	return true;
}

//删除分配
bool CAttemperEngineSink::DeleteDistribute(IServerUserItem * pIServerUserItem)
{
	m_DistributeManage.RemoveDistributeNode(pIServerUserItem);
	return true;
}

//执行分组
bool CAttemperEngineSink::PerformDistribute()
{
	//人数校验
	if(m_DistributeManage.GetCount()<1) return false;

	//分配用户
	while(true)
	{
		//人数校验
		if(m_DistributeManage.GetCount()<1) break;

		//变量定义
		CDistributeInfoArray DistributeInfoArray;

		//获取用户
		WORD wRandCount = __max(m_pGameServiceAttrib->wChairCount,1);
		WORD wChairCount = rand()%wRandCount;
		WORD wDistributeCount = m_DistributeManage.PerformDistribute(DistributeInfoArray,wChairCount);
		//if(wDistributeCount < m_pGameServiceOption->wMinDistributeUser) break;

		CString szFileName;
		WORD wTableCount=0;

		wTableCount = m_TableFrameArray.GetCount();
		//寻找位置
		CTableFrame * pCurrTableFrame=NULL;
		for (WORD i=0;i<wTableCount;i++)
		{
			//获取对象
			ASSERT_ERR(m_TableFrameArray[i]!=NULL);
			CTableFrame * pTableFrame=m_TableFrameArray[i];
			if ( pTableFrame == NULL ) continue;

			bool bDynamicJoin = true;
			if (m_pGameServiceAttrib->cbDynamicJoin == FALSE) bDynamicJoin = false;
			if (CServerRule::IsAllowDynamicJoin(m_pGameServiceOption->dwServerRule) == false) bDynamicJoin = false;
			if (pTableFrame->IsGameStarted() && bDynamicJoin == false) continue;

			//状态判断
			tagTableUserInfo userInfo = {0};
			DWORD userCount = pTableFrame->GetTableUserInfo(userInfo);

			if (userCount < m_pGameServiceAttrib->wChairCount && userInfo.wTableUserCount != 0)
			{
				pCurrTableFrame = pTableFrame;
				break;
			}
		}

		if (pCurrTableFrame == NULL) break;
		

		//玩家坐下
		bool bSitSuccess=true;
		INT_PTR nSitFailedIndex=INVALID_CHAIR;
		for(INT_PTR nIndex=0;nIndex<DistributeInfoArray.GetCount();nIndex++)
		{
			//变量定义
			WORD wChairID=pCurrTableFrame->GetNullChairID();

			//分配用户
			if (wChairID!=INVALID_CHAIR)
			{
				//获取用户
				IServerUserItem * pIServerUserItem=DistributeInfoArray[nIndex].pIServerUserItem;

				//用户坐下
				if(pCurrTableFrame->PerformSitDownAction(wChairID,pIServerUserItem)==false)
				{
					bSitSuccess=false;
					nSitFailedIndex=nIndex;
					break;
				}
			}
		}

		//坐下结果
		if(bSitSuccess)
		{
			//移除结点
			while(DistributeInfoArray.GetCount()>0)
			{
				m_DistributeManage.RemoveDistributeNode((tagDistributeNode *)DistributeInfoArray[0].pPertainNode);
				DistributeInfoArray.RemoveAt(0);
			}
		}
		else
		{
			for(INT_PTR nIndex=0;nIndex<DistributeInfoArray.GetCount();nIndex++)
			{
				if(nSitFailedIndex==nIndex)
				{
					m_DistributeManage.RemoveDistributeNode((tagDistributeNode *)DistributeInfoArray[nIndex].pPertainNode);
				}
				else
				{
					//用户起立
					if (DistributeInfoArray[nIndex].pIServerUserItem->GetTableID()!=INVALID_TABLE)
					{
						WORD wTableID=DistributeInfoArray[nIndex].pIServerUserItem->GetTableID();
						m_TableFrameArray[wTableID]->PerformStandUpAction(DistributeInfoArray[nIndex].pIServerUserItem);
					}
				}
			}
		}		
	}

	return true;
}

//敏感词过滤
void CAttemperEngineSink::SensitiveWordFilter(LPCTSTR pMsg, LPTSTR pszFiltered, int nMaxLen)
{
	m_WordsFilter.Filtrate(pMsg,pszFiltered,nMaxLen);
}

//设置参数
void CAttemperEngineSink::SetMobileUserParameter(IServerUserItem * pIServerUserItem,BYTE cbDeviceType,WORD wBehaviorFlags,WORD wPageTableCount)
{
	if(wPageTableCount > m_pGameServiceOption->wTableCount)wPageTableCount=m_pGameServiceOption->wTableCount;
	pIServerUserItem->SetMobileUserDeskCount(wPageTableCount);
	pIServerUserItem->SetMobileUserRule(wBehaviorFlags);
}

//群发数据
bool CAttemperEngineSink::SendDataBatchToMobileUser(WORD wCmdTable, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//枚举用户
	WORD wEnumIndex=0;
	while(wEnumIndex<m_ServerUserManager.GetUserItemCount())
	{
		//过滤用户
		IServerUserItem *pIServerUserItem=m_ServerUserManager.EnumUserItem(wEnumIndex++);
		if(pIServerUserItem==NULL) continue;
		if(!pIServerUserItem->IsMobileUser()) continue;

		//定义变量
		WORD wMobileUserRule = pIServerUserItem->GetMobileUserRule();
		WORD wTagerTableID = pIServerUserItem->GetTableID();
		bool bViewModeAll = ((wMobileUserRule&VIEW_MODE_ALL)!=0);
		bool bRecviceGameChat = ((wMobileUserRule&RECVICE_GAME_CHAT)!=0);
		bool bRecviceRoomChat = ((wMobileUserRule&RECVICE_ROOM_CHAT)!=0);
		bool bRecviceRoomWhisper = ((wMobileUserRule&RECVICE_ROOM_WHISPER)!=0);

		//状态过滤
		if(pIServerUserItem->GetUserStatus() >= US_SIT)
		{
			if(wTagerTableID != wCmdTable)continue;
		}

		//聊天过滤
		if(wSubCmdID==SUB_GR_USER_CHAT || wSubCmdID==SUB_GR_USER_EXPRESSION)
		{
			if(!bRecviceGameChat || !bRecviceRoomChat) continue;
		}
		if(wSubCmdID==SUB_GR_WISPER_CHAT || wSubCmdID==SUB_GR_WISPER_EXPRESSION)
		{
			if(!bRecviceRoomWhisper) continue;
		}

		//部分可视
		if(!bViewModeAll)
		{
			//消息过滤
			if(wSubCmdID==SUB_GR_USER_ENTER && wCmdTable==INVALID_TABLE) continue;
			if(wSubCmdID==SUB_GR_USER_SCORE && pIServerUserItem->GetUserStatus() < US_SIT) continue;

			//定义变量
			WORD wTagerDeskPos = pIServerUserItem->GetMobileUserDeskPos();
			WORD wTagerDeskCount = pIServerUserItem->GetMobileUserDeskCount();

			//状态消息过滤
			if(wCmdTable < wTagerDeskPos) continue;
			if(wCmdTable > (wTagerDeskPos+wTagerDeskCount-1)) continue;
		}

		//发送消息
		SendData(pIServerUserItem,wMainCmdID,wSubCmdID,pData,wDataSize);
	}

	return true;
}

//群发用户信息
bool CAttemperEngineSink::SendUserInfoPacketBatchToMobileUser(IServerUserItem * pIServerUserItem)
{
	//效验参数
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//变量定义
	BYTE cbBuffer[SOCKET_TCP_PACKET];
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();
	tagMobileUserInfoHead * pUserInfoHead=(tagMobileUserInfoHead *)cbBuffer;
	CSendPacketHelper SendPacket(cbBuffer+sizeof(tagMobileUserInfoHead),sizeof(cbBuffer)-sizeof(tagMobileUserInfoHead));

	//用户属性
	pUserInfoHead->wFaceID=pUserInfo->wFaceID;
	pUserInfoHead->dwGameID=pUserInfo->dwGameID;
	pUserInfoHead->dwUserID=pUserInfo->dwUserID;
	pUserInfoHead->dwCustomID=pUserInfo->dwCustomID;

	//用户属性
	pUserInfoHead->cbGender=pUserInfo->cbGender;
	pUserInfoHead->cbMemberOrder=pUserInfo->cbMemberOrder;

	//用户状态
	pUserInfoHead->wTableID=pUserInfo->wTableID;
	pUserInfoHead->wChairID=pUserInfo->wChairID;
	pUserInfoHead->cbUserStatus=pUserInfo->cbUserStatus;

	//用户局数
	pUserInfoHead->dwWinCount=pUserInfo->dwWinCount;
	pUserInfoHead->dwLostCount=pUserInfo->dwLostCount;
	pUserInfoHead->dwDrawCount=pUserInfo->dwDrawCount;
	pUserInfoHead->dwFleeCount=pUserInfo->dwFleeCount;
	pUserInfoHead->dwExperience=pUserInfo->dwExperience;

	//用户成绩
	pUserInfoHead->lScore=pUserInfo->lScore;
	pUserInfoHead->lScore+=pIServerUserItem->GetTrusteeScore();
	pUserInfoHead->lScore+=pIServerUserItem->GetFrozenedScore();

	//叠加信息
	SendPacket.AddPacket(pUserInfo->szNickName,DTP_GR_NICK_NAME);

	//发送数据
	WORD wHeadSize=sizeof(tagMobileUserInfoHead);
	SendDataBatchToMobileUser(pUserInfoHead->wTableID,MDM_GR_USER,SUB_GR_USER_ENTER,cbBuffer,wHeadSize+SendPacket.GetDataSize());

	return true;
}

//发可视用户信息
bool CAttemperEngineSink::SendViewTableUserInfoPacketToMobileUser(IServerUserItem * pIServerUserItem,DWORD dwUserIDReq)
{
	//变量定义
	BYTE cbBuffer[SOCKET_TCP_PACKET];
	tagMobileUserInfoHead * pUserInfoHead=(tagMobileUserInfoHead *)cbBuffer;
	WORD wMobileUserRule = pIServerUserItem->GetMobileUserRule();
	WORD wTagerTableID = pIServerUserItem->GetTableID();
	WORD wTagerDeskPos = pIServerUserItem->GetMobileUserDeskPos();
	WORD wTagerDeskCount = pIServerUserItem->GetMobileUserDeskCount();
	bool bViewModeAll = ((wMobileUserRule&VIEW_MODE_ALL)!=0);
	if(wTagerDeskCount==0) wTagerDeskCount=1;

	//枚举用户
	WORD wEnumIndex=0;
	while(wEnumIndex<m_ServerUserManager.GetUserItemCount())
	{
		//过滤用户
		IServerUserItem *pIUserItem=m_ServerUserManager.EnumUserItem(wEnumIndex++);
		if(pIUserItem==NULL || (dwUserIDReq==INVALID_CHAIR && pIUserItem==pIServerUserItem)) continue;
		if(dwUserIDReq != INVALID_CHAIR && pIUserItem->GetUserID() != dwUserIDReq) continue;

		//部分可视
		if(dwUserIDReq==INVALID_CHAIR && !bViewModeAll)
		{
			if(pIUserItem->GetTableID() < wTagerDeskPos) continue;
			if(pIUserItem->GetTableID() > (wTagerDeskPos+wTagerDeskCount-1)) continue;
		}

		//定义变量
		tagUserInfo * pUserInfo=pIUserItem->GetUserInfo();
		ZeroMemory(cbBuffer,sizeof(cbBuffer));
		CSendPacketHelper SendPacket(cbBuffer+sizeof(tagMobileUserInfoHead),sizeof(cbBuffer)-sizeof(tagMobileUserInfoHead));

		//用户属性
		pUserInfoHead->wFaceID=pUserInfo->wFaceID;
		pUserInfoHead->dwGameID=pUserInfo->dwGameID;
		pUserInfoHead->dwUserID=pUserInfo->dwUserID;
		pUserInfoHead->dwCustomID=pUserInfo->dwCustomID;

		//用户属性
		pUserInfoHead->cbGender=pUserInfo->cbGender;
		pUserInfoHead->cbMemberOrder=pUserInfo->cbMemberOrder;

		//用户状态
		pUserInfoHead->wTableID=pUserInfo->wTableID;
		pUserInfoHead->wChairID=pUserInfo->wChairID;
		pUserInfoHead->cbUserStatus=pUserInfo->cbUserStatus;

		//用户局数
		pUserInfoHead->dwWinCount=pUserInfo->dwWinCount;
		pUserInfoHead->dwLostCount=pUserInfo->dwLostCount;
		pUserInfoHead->dwDrawCount=pUserInfo->dwDrawCount;
		pUserInfoHead->dwFleeCount=pUserInfo->dwFleeCount;
		pUserInfoHead->dwExperience=pUserInfo->dwExperience;

		//用户成绩
		pUserInfoHead->lScore=pUserInfo->lScore;
		pUserInfoHead->lScore+=pIUserItem->GetTrusteeScore();
		pUserInfoHead->lScore+=pIUserItem->GetFrozenedScore();

		//叠加信息
		SendPacket.AddPacket(pUserInfo->szNickName,DTP_GR_NICK_NAME);

		//发送数据
		WORD wHeadSize=sizeof(tagMobileUserInfoHead);
		SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_USER_ENTER,cbBuffer,wHeadSize+SendPacket.GetDataSize());
	}

	return true;
}

//手机立即登录
bool CAttemperEngineSink::MobileUserImmediately(IServerUserItem * pIServerUserItem)
{
	//查找桌子
	for (INT_PTR i=0;i<(m_pGameServiceOption->wTableCount);i++)
	{
		//获取桌子
		CTableFrame * pTableFrame=m_TableFrameArray[i];
		if (pTableFrame->IsGameStarted()==true) continue;
		if(pTableFrame->IsTableLocked()) continue;

		//无效过滤
		WORD wChairID=pTableFrame->GetRandNullChairID();
		if (wChairID==INVALID_CHAIR) continue;

		//用户坐下
		pTableFrame->PerformSitDownAction(wChairID,pIServerUserItem);
		return true;
	}

	//失败
	m_TableFrameArray[0]->SendRequestFailure(pIServerUserItem,TEXT("没找到可进入的游戏桌！"),REQUEST_FAILURE_NORMAL);
	return true;
}

//发送系统消息
bool CAttemperEngineSink::SendSystemMessage(CMD_GR_SendMessage * pSendMessage, WORD wDataSize)
{
	//消息处理
	ASSERT_ERR(pSendMessage!=NULL);

	//效验数据
	ASSERT_ERR(wDataSize==sizeof(CMD_GR_SendMessage)-sizeof(pSendMessage->szSystemMessage)+sizeof(TCHAR)*pSendMessage->wChatLength);
	if (wDataSize!=sizeof(CMD_GR_SendMessage)-sizeof(pSendMessage->szSystemMessage)+sizeof(TCHAR)*pSendMessage->wChatLength) 
		return false;

	//所有房间
	if(pSendMessage->cbAllRoom == TRUE)
	{
		pSendMessage->cbAllRoom=FALSE;
		m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE,SUB_CS_C_SYSTEM_MESSAGE,pSendMessage,wDataSize);
	}
	else
	{
		//发送系统消息
		if(pSendMessage->cbGame == TRUE)
			SendGameMessage(pSendMessage->szSystemMessage,SMT_CHAT);
		if(pSendMessage->cbRoom == TRUE)
			SendRoomMessage(pSendMessage->szSystemMessage,SMT_CHAT);
	}

	return true;
}

//清除消息数据
void CAttemperEngineSink::ClearSystemMessageData()
{
	while(m_SystemMessageList.GetCount() > 0)
	{
		tagSystemMessage * pRqHead = m_SystemMessageList.RemoveHead();
		if(pRqHead)
		{
			delete [] ((BYTE*)pRqHead);
		}
	}
}

void CAttemperEngineSink::PostReleaseAllAndroid()
{
	DBR_GR_GameAndriodAll request = {0};
	CopyMemory(request.guidRoom, m_guidRoom, 40 * sizeof WCHAR);

	m_pIDBCorrespondManager->PostDataBaseRequest(0l, DBR_GR_ANDRIOD_ALL, 0l, (void *)&request, sizeof request);
}


void CAttemperEngineSink::ShutDownSocket(IServerUserItem* pIServerUserItem)
{
	//获取绑定
	if ( pIServerUserItem == NULL ) return ;
	WORD wBindIndex=pIServerUserItem->GetBindIndex();
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	//绑带处理
	if (pBindParameter!=NULL)
	{
		//中断网络
		if (pBindParameter->dwSocketID!=0L)
		{
			if (LOWORD(pBindParameter->dwSocketID)>=INDEX_ANDROID)
			{
				//m_AndroidUserManager.DeleteAndroidUserItem(pBindParameter->dwSocketID);
			}
			else
			{
				m_pITCPNetworkEngine->ShutDownSocket(pBindParameter->dwSocketID);
			}
		}
	}
}

// 操作结果
bool CAttemperEngineSink::OnDBPCOperateResult(DWORD dwContextID, VOID* pData, WORD wDataSize)
{
	CMD_GP_OperateResult OperateResult = { 0 };

	// 变量定义
	DBO_GP_OperateResult * pOperateResult = (DBO_GP_OperateResult *)pData;
	OperateResult.cbStringLength = (wDataSize - sizeof(WORD) - sizeof(DWORD)) / 2;
	OperateResult.wSubCommandID = pOperateResult->wSubCommandID;
	lstrcpyn(OperateResult.szDescribe, pOperateResult->szDescribeString, sizeof(OperateResult.szDescribe));

	int wSendSize = 1 + 2  + CountStringBuffer(OperateResult.szDescribe);

	// 发送数据
	if (pOperateResult->dwResultCode == 0)
		m_pITCPNetworkEngine->SendData(dwContextID, MDM_GF_FRAME, SUB_GF_OPERATE_SUCCESS, &OperateResult, wSendSize);
	else
		m_pITCPNetworkEngine->SendData(dwContextID, MDM_GF_FRAME, SUB_GF_OPERATE_FAILURE, &OperateResult, wSendSize);

	return true;
}

bool CAttemperEngineSink::DetectKickUser(IServerUserItem * pIServerUserItem, BYTE cbQuickKick)
{
	if (pIServerUserItem == NULL) return false;
	if ( m_pGameServiceOption->wKindID != KIND_FISH && pIServerUserItem->GetUserStatus() == US_PLAYING) return false;
	//被踢判断
	DWORD dwUserID = pIServerUserItem->GetUserID();
	WORD wTableID = pIServerUserItem->GetTableID();
	WORD wBindIndex = pIServerUserItem->GetBindIndex();
	DWORD dwInoutIndex = pIServerUserItem->GetInoutIndex();
	tagBindParameter* pBindParameter = GetBindParameter(wBindIndex);
	if (pBindParameter == NULL) return false;

	tagKickUser KickUser = { 0 };
	if ( cbQuickKick && (m_KickUserItemMap.Lookup(dwUserID, KickUser) == TRUE)) {
		//请离桌子
		if (wTableID != INVALID_TABLE)
		{
			CTableFrame * pTableFrame = m_TableFrameArray[wTableID];
			if (pTableFrame && pTableFrame->PerformStandUpAction(pIServerUserItem) == false) {}
		}

		SendLogonFailure(TEXT("您已被管理员请出房间,请稍后再进入！"), 0, pBindParameter->dwSocketID);
		I_TRACE(TEXT("管理员快速踢玩家[%d]"), dwUserID);
		return true;
	}

// 	DWORD dwKickTime;
// 	if (m_KickUserItemMap.Lookup(dwUserID, dwKickTime) == TRUE)
// 	{
// 		//时效判断
// 		DWORD dwCurrTime = (DWORD)time(NULL);
// 		if ( dwKickTime > dwCurrTime ) 
// 		{
// 			//请离桌子
// 			if (wTableID != INVALID_TABLE)
// 			{
// 				CTableFrame * pTableFrame = m_TableFrameArray[wTableID];
// 				if (pTableFrame && pTableFrame->PerformStandUpAction(pIServerUserItem) == false) {  }
// 			}
// 
// 			SendLogonFailure(TEXT("您已被管理员请出房间,请稍后再进入！"), 0, pBindParameter->dwSocketID);
// 
// 			//PerformUnlockScore(dwUserID, dwInoutIndex, LER_SERVER_CONDITIONS);
// 
// 			I_TRACE(TEXT("管理员踢玩家[%d]"), dwUserID);
// 
// 			return true;
// 		}
// 		else
// 		{
// 			//移除玩家
// 			m_KickUserItemMap.RemoveKey(dwUserID);		
// 		}
// 	}
	return false;
}

//心跳处理 作废
bool CAttemperEngineSink::OnTCPNetworkMainHeart(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//WORD wBindIndex=LOWORD(dwSocketID);
	//IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	//ASSERT_ERR(pIServerUserItem!=NULL);
	//if (pIServerUserItem==NULL) return false;

	if ( wSubCmdID == SUB_GR_HEART )
	{
		DWORD dwCurTime = (DWORD)time(NULL);
		//I_TRACE(TEXT("%s 接收心跳,%d"),pIServerUserItem->GetNickName(),dwCurTime);
		//pIServerUserItem->SetHeartTime(dwCurTime);

		GameServer::GSUserHeart gsUserHeart;
		gsUserHeart.set_userid(1);
		std::string serializeStr;
		gsUserHeart.SerializeToString(&serializeStr);
		SendData(dwSocketID,MDM_GR_HEART,SUB_GR_HEART, (void*)serializeStr.c_str(), (WORD)serializeStr.size());
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////////////
