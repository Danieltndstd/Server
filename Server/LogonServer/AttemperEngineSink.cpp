#include "StdAfx.h"
#include "ServiceUnits.h"
#include "ControlPacket.h"
#include "AttemperEngineSink.h"
//#include "ParseAddr.h"
//#include "boost/date_time/posix_time/posix_time.hpp"
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
#define IDI_TIME_CACHE_RANK			7									//缓存排行榜
#define IDI_LOAD_SCROLL_MESSAGE		8									//读取滚动信息
#define IDI_CLEAR_SCROLL_MESSAGE	9									//清除滚动消息

//时间定义
#define TIME_LOAD_SCROLL_MESSAGE		25L								//读取滚动消息
#define TIME_CLEAR_SCROLL_MESSAGE		300L							//清除滚动消息

//////////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////


//启动事件
bool CAttemperEngineSink::OnAttemperEngineBegin(IUnknownEx * pIUnknownEx)
{

	//事件注册
	{
		// 主命令注册
		{
			REGISTER_FUNCTION_4(m_mMainFunctions, MDM_GP_LOGON, CAttemperEngineSink::OnTCPNetworkMainPCLogon, this);	// 登录命令
			REGISTER_FUNCTION_4(m_mMainFunctions, MDM_GP_SERVER_LIST, CAttemperEngineSink::OnTCPNetworkMainPCServerList, this);	// 列表命令
			REGISTER_FUNCTION_4(m_mMainFunctions, MDM_GP_USER_SERVICE, CAttemperEngineSink::OnTCPNetworkMainPCUserService, this);	// 服务命令
			REGISTER_FUNCTION_4(m_mMainFunctions, MDM_GP_REMOTE_SERVICE, CAttemperEngineSink::OnTCPNetworkMainPCRemoteService, this);	// 远程服务

			REGISTER_FUNCTION_4(m_mMainFunctions, MDM_MB_LOGON, CAttemperEngineSink::OnTCPNetworkMainMBLogon, this);	// 登录命令
			REGISTER_FUNCTION_4(m_mMainFunctions, MDM_MB_SERVER_LIST, CAttemperEngineSink::OnTCPNetworkMainMBServerList, this);	// 列表命令
		}

		// 数据库事件注册
		{
			//登录
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_LOGON_SUCCESS, CAttemperEngineSink::OnDBPCLogonSuccess, this);	// 登录成功
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_WEB_LOGON_SUCCESS, CAttemperEngineSink::OnDBWebLogonSuccess, this);	// 登录成功
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_LOGON_FAILURE, CAttemperEngineSink::OnDBPCLogonFailure, this);	// 登录失败
			REGISTER_FUNCTION_3(m_mDbFunctions, DBR_GP_VALIDATE_MBCARD, CAttemperEngineSink::OnDBEventDeal, this, MDM_GP_LOGON, SUB_GP_VALIDATE_MBCARD);
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_MB_LOGON_SUCCESS, CAttemperEngineSink::OnDBMBLogonSuccess, this);	// 登录成功
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_MB_LOGON_FAILURE, CAttemperEngineSink::OnDBMBLogonFailure, this);	// 登录失败
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_THIRD_REGISTER, CAttemperEngineSink::OnDBThirdPartyRegister, this);	// 三方注册
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_THIRD_LOGON, CAttemperEngineSink::OnDBThirdPartySuccess, this);	// 第三方登录成功
			//银行
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_USER_INSURE_INFO, CAttemperEngineSink::OnDBPCUserInsureInfo, this);	// 银行资料
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_USER_INSURE_SUCCESS, CAttemperEngineSink::OnDBPCUserInsureSuccess, this);	// 银行成功
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_USER_INSURE_FAILURE, CAttemperEngineSink::OnDBPCUserInsureFailure, this);	// 银行失败
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_USER_INSURE_USER_INFO, CAttemperEngineSink::OnDBEventDeal, this, MDM_GP_USER_SERVICE, SUB_GP_QUERY_USER_INFO_RESULT);	// 用户信息
			//用户

			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_AVATAR_INFO, CAttemperEngineSink::OnDBEventDeal, this, MDM_GP_USER_SERVICE, SUB_GP_GET_AVATAR_INFO);	// 头像
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_IMAGE_INFO, CAttemperEngineSink::OnDBEventDeal, this, MDM_GP_USER_SERVICE, SUB_GP_GET_IMAGE_INFO);	// 形象
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_MODIFY_AVATAR, CAttemperEngineSink::OnDBEventDeal, this, MDM_GP_USER_SERVICE, SUB_GP_MODIFY_AVATAR);	// 修改头像结果
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_MODIFY_IMAGE, CAttemperEngineSink::OnDBEventDeal, this, MDM_GP_USER_SERVICE, SUB_GP_MODIFY_IMAGE);	// 修改形象结果

			//列表结果
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_GAME_TYPE_ITEM, CAttemperEngineSink::OnDBPCGameTypeItem, this);	// 类型子项
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_GAME_KIND_ITEM, CAttemperEngineSink::OnDBPCGameKindItem, this);	// 类型子项
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_GAME_NODE_ITEM, CAttemperEngineSink::OnDBPCGameNodeItem, this);	// 节点子项
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_GAME_PAGE_ITEM, CAttemperEngineSink::OnDBPCGamePageItem, this);	// 定制子项
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_GAME_LIST_RESULT, CAttemperEngineSink::OnDBPCGameListResult, this);	// 加载结果
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_GAME_COLLECTION, CAttemperEngineSink::OnDBPCGameCollection, this);
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_BROADCAST_MSG, CAttemperEngineSink::OnDBBroadcastMsg, this);	// 广播消息
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_BROADCAST_TYPHON, CAttemperEngineSink::OnDBBroadcastTyphon, this);	// 发送喇叭
			//通用属性
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_UPDATE_ATTRIBUTE, CAttemperEngineSink::OnDBUpdateAttribute, this);	// 刷新通用属性


			
			//比赛
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_GET_MATCHDES,CAttemperEngineSink::OnDBEventDeal, this, MDM_MB_MATCH_INFO, MATCH_MB_DESCRIPTION);	//获取比赛概述
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_GET_MATCHRULE,CAttemperEngineSink::OnDBEventDeal, this, MDM_MB_MATCH_INFO, MATCH_MB_RULE);	//获取比赛规则
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_GET_MATCHSOLUTION,CAttemperEngineSink::OnDBEventDeal, this, MDM_MB_MATCH_INFO, MATCH_MB_SOLUTION);	//获取比赛解决方案
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_GET_MATCHREWARD,CAttemperEngineSink::OnDBEventDeal, this, MDM_MB_MATCH_INFO, MATCH_MB_REWARD);	//获取比赛奖励
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_GET_MATCHINFOEND,CAttemperEngineSink::OnDBEventDeal, this, MDM_MB_MATCH_INFO, MATCH_MB_END);	//获取比赛结果结束
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_GET_MATCH_APPLY_STATUS,CAttemperEngineSink::OnDBEventDeal, this, MDM_MB_MATCH_INFO, MATCH_MB_APPLY_STATUS);	//获取比赛报名状态
			

			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_OPERATE_RESULT, CAttemperEngineSink::OnDBPCOperateResult, this);	// 操作结果
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_USER_INDIVIDUAL, CAttemperEngineSink::OnDBEventDeal, this, MDM_GP_USER_SERVICE, SUB_GP_USER_INDIVIDUAL);	// 用户信息


			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_RELOAD_SCORE, CAttemperEngineSink::OnDBEventDeal, this, MDM_GP_USER_SERVICE, SUB_GP_RELOAD_SCORE);	//刷新积分


			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_GET_GAME_RECORDS,CAttemperEngineSink::OnDBEventDeal, this, MDM_GP_USER_SERVICE, SUB_GP_GET_GAME_RECORDS);	//用户比赛记录


		}
		
	}

	//设置接口
	if (m_ServerUserManager.SetServerUserItemSink(QUERY_ME_INTERFACE(IServerUserItemSink))==false)
	{
		ASSERT_ERR(FALSE);
		return false;
	}

	//设置时间
	m_pITimerEngine->SetTimer(IDI_CHECK_VERIFICATION, 60 * 100lL, TIMES_INFINITY, 0);
	//m_pITimerEngine->SetTimer(IDI_COLLECT_ONLINE_INFO,m_pInitParameter->m_wCollectTime*1000L,TIMES_INFINITY,0);



	//读取滚动消息
	m_pITimerEngine->SetTimer(IDI_LOAD_SCROLL_MESSAGE,TIME_LOAD_SCROLL_MESSAGE * 1000L,TIMES_INFINITY,0);

	//清除滚动消息
	m_pITimerEngine->SetTimer(IDI_CLEAR_SCROLL_MESSAGE,TIME_CLEAR_SCROLL_MESSAGE * 1000L,TIMES_INFINITY,0);

	return true;
}

//控制事件
bool CAttemperEngineSink::OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize)
{
	switch (wIdentifier)
	{
	case CT_LOAD_DB_GAME_LIST:		//加载列表
		{
			//加载列表
			m_ServerListManager.DisuseKernelItem();
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_GAME_LIST,0,NULL,0);

			return true;
		}
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
	}

	return false;
}

//调度事件
bool CAttemperEngineSink::OnEventAttemperData(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}

//时间事件
bool CAttemperEngineSink::OnEventTimer(DWORD dwTimerID, WPARAM wBindParam)
{
	switch (dwTimerID)
	{
	case IDI_LOAD_GAME_LIST:		//加载列表
		{
			//加载列表
			m_ServerListManager.DisuseKernelItem();
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_GAME_LIST,0,NULL,0);

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
			TRACE_LOG(szString,TraceLevel_Normal);

			return true;
		}
	case IDI_COLLECT_ONLINE_INFO:	//统计在线
		{
			return true;
			//变量定义
			DBR_GP_OnLineCountInfo OnLineCountInfo;
			ZeroMemory(&OnLineCountInfo,sizeof(OnLineCountInfo));

			//获取总数
			OnLineCountInfo.dwOnLineCountSum=m_ServerListManager.CollectOnlineInfo();

			//获取类型
			POSITION KindPosition=NULL;
			do
			{
				//获取类型
				CGameKindItem * pGameKindItem=m_ServerListManager.EmunGameKindItem(KindPosition);

				//设置变量
				if (pGameKindItem!=NULL)
				{
					WORD wKindIndex=OnLineCountInfo.wKindCount++;
					OnLineCountInfo.OnLineCountKind[wKindIndex].wKindID=pGameKindItem->m_GameKind.wKindID;
					OnLineCountInfo.OnLineCountKind[wKindIndex].dwOnLineCount=pGameKindItem->m_GameKind.dwOnLineCount;
				}

				//溢出判断
				if (OnLineCountInfo.wKindCount>=CountArray(OnLineCountInfo.OnLineCountKind))
				{
					ASSERT_ERR(FALSE);
					break;
				}

			} while (KindPosition!=NULL);

			//发送请求
			WORD wHeadSize=sizeof(OnLineCountInfo)-sizeof(OnLineCountInfo.OnLineCountKind);
			WORD wSendSize=wHeadSize+OnLineCountInfo.wKindCount*sizeof(OnLineCountInfo.OnLineCountKind[0]);
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_ONLINE_COUNT_INFO,0,&OnLineCountInfo,wSendSize);

			return true;
		}
	case IDI_BROADCAST_MSG:
		{
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_BROADCASE_MSG,0,NULL,0);
			return true;
		}
		//读取滚动条
	case IDI_LOAD_SCROLL_MESSAGE:
		{
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_LOAD_SCROLL_MESSAGE,0,NULL,0);
			return true;
		}
		//清除滚动记录
	case IDI_CLEAR_SCROLL_MESSAGE:
		{
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_CLEAR_SCROLL_MESSAGE,0,NULL,0);
			return true;
		}
	}

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

	return bResult;
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
		case MDM_CS_REMOTE_SERVICE:	//远程服务
			{
				return OnTCPSocketMainRemoteService(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_MANAGER_SERVICE: //管理服务
			{
				return true;
			}
		case MDM_CS_MATCH_SERVICE:	//比赛服务
			{
				return true;
			}
		}
	}

	//错误断言
	ASSERT_ERR(FALSE);

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
			LPCTSTR pszDescribeString=pRegisterFailure->szDescribeString;
			if (lstrlen(pszDescribeString)>0) TRACE_LOG(pszDescribeString,TraceLevel_Exception);

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
			return true;
			//效验参数
			ASSERT_ERR(wDataSize==sizeof(CMD_CS_S_ServerOnLine));
			if (wDataSize!=sizeof(CMD_CS_S_ServerOnLine)) return false;

			//变量定义
			CMD_CS_S_ServerOnLine * pServerOnLine=(CMD_CS_S_ServerOnLine *)pData;

			//查找房间
			CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(pServerOnLine->wServerID);
			if(pGameServerItem == NULL) return true;

			//设置人数
			DWORD dwOldOnlineCount=0;
			dwOldOnlineCount = pGameServerItem->m_GameServer.dwOnLineCount;
			pGameServerItem->m_GameServer.dwOnLineCount=pServerOnLine->dwOnLineCount;

			//目录人数
			CGameKindItem * pGameKindItem=m_ServerListManager.SearchGameKind(pGameServerItem->m_GameServer.wKindID);
			if (pGameKindItem!=NULL)
			{
				tagGameServer * pGameServer=&pGameServerItem->m_GameServer;
				pGameKindItem->m_GameKind.dwOnLineCount -= dwOldOnlineCount;
				pGameKindItem->m_GameKind.dwOnLineCount += pGameServer->dwOnLineCount;
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

			DBR_GP_ControlRoomStauts ControlRoom = { 0 };
			//更新数据
			for (WORD i=0;i<wItemCount;i++)
			{
				//I_TRACE(TEXT("SUB_CS_S_SERVER_INSERT ServerID:%d"),pGameServer->wServerID);

				m_ServerListManager.InsertGameServer(pGameServer++);
			}

			return true;
		}
	case SUB_CS_S_MATCH_INSERT:		//比赛插入
		{
			return true;
			ASSERT_ERR(wDataSize%sizeof(tagGameMatch)==0);
			if (wDataSize%sizeof(tagGameMatch)!=0) return false;

			//变量定义
			WORD wItemCount=wDataSize/sizeof(tagGameMatch);
			tagGameMatch * pGameMatch=(tagGameMatch *)pData;

			for( WORD i=0;i<wItemCount;++i )
			{
				m_ServerListManager.InsertGameMatch(pGameMatch);	
				pGameMatch++;
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
			ASSERT_ERR(m_ServerListManager.SearchGameServer(pServerModify->wServerID));
			CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(pServerModify->wServerID);

			//设置房间
			if (pGameServerItem!=NULL)
			{
				//设置人数
				DWORD dwOldOnlineCount=0, dwOldFullCount=0;
				dwOldOnlineCount = pGameServerItem->m_GameServer.dwOnLineCount;
				dwOldFullCount   = pGameServerItem->m_GameServer.dwFullCount;

				//修改房间信息
				pGameServerItem->m_GameServer.wKindID=pServerModify->wKindID;
				pGameServerItem->m_GameServer.wNodeID=pServerModify->wNodeID;
				pGameServerItem->m_GameServer.wSortID=pServerModify->wSortID;
				pGameServerItem->m_GameServer.wServerPort=pServerModify->wServerPort;
				pGameServerItem->m_GameServer.dwOnLineCount=pServerModify->dwOnLineCount;
				pGameServerItem->m_GameServer.dwFullCount=pServerModify->dwFullCount;
				lstrcpyn(pGameServerItem->m_GameServer.szServerName,pServerModify->szServerName,CountArray(pGameServerItem->m_GameServer.szServerName));
				lstrcpyn(pGameServerItem->m_GameServer.szServerAddr,pServerModify->szServerAddr,CountArray(pGameServerItem->m_GameServer.szServerAddr));

				//目录人数
				CGameKindItem * pGameKindItem=m_ServerListManager.SearchGameKind(pGameServerItem->m_GameServer.wKindID);
				if (pGameKindItem!=NULL)
				{
					tagGameServer * pGameServer=&pGameServerItem->m_GameServer;
					pGameKindItem->m_GameKind.dwOnLineCount -= dwOldOnlineCount;
					pGameKindItem->m_GameKind.dwOnLineCount += pGameServer->dwOnLineCount;

					pGameKindItem->m_GameKind.dwFullCount -= dwOldFullCount;
					pGameKindItem->m_GameKind.dwFullCount += pGameServer->dwFullCount;
				}
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

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_KILL_LOCK,0,pServerRemove,sizeof(CMD_CS_S_ServerRemove));

			DBR_GP_ControlRoomStauts ControlRoom = { 0 };
			ControlRoom.wServerID = pServerRemove->wServerID;
			ControlRoom.cbIsOpen = 0;
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_CONTROL_ROOM_STATUS, 0, &ControlRoom, sizeof(ControlRoom));
			//I_TRACE(TEXT("SUB_CS_S_SERVER_REMOVE wServerID:%d"),ControlRoom.wServerID);

			m_ServerListManager.DeleteGameServer(pServerRemove->wServerID);
			return true;
		}
	case SUB_CS_S_SERVER_FINISH:	//房间完成
		{
			//清理列表
			//m_ServerListManager.CleanServerItem();

			//事件处理
			CP_ControlResult ControlResult;
			ControlResult.cbSuccess=ER_SUCCESS;
			SendUIControlPacket(UI_CORRESPOND_RESULT,&ControlResult,sizeof(ControlResult));


			return true;
		}

	}

	return true;
}

//远程服务
bool CAttemperEngineSink::OnTCPSocketMainRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_SEARCH_CORRESPOND:	//协调查找
		{
			//变量定义
			CMD_CS_S_SearchCorrespond * pSearchCorrespond=(CMD_CS_S_SearchCorrespond *)pData;

			//效验参数
			ASSERT_ERR(wDataSize<=sizeof(CMD_CS_S_SearchCorrespond));
			ASSERT_ERR(wDataSize>=(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)));
			ASSERT_ERR(wDataSize==(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)+pSearchCorrespond->wUserCount*sizeof(pSearchCorrespond->UserRemoteInfo[0])));

			//效验参数
			if (wDataSize>sizeof(CMD_CS_S_SearchCorrespond)) return false;
			if (wDataSize<(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo))) return false;
			if (wDataSize!=(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)+pSearchCorrespond->wUserCount*sizeof(pSearchCorrespond->UserRemoteInfo[0]))) return false;

			//判断在线
			ASSERT_ERR(LOWORD(pSearchCorrespond->dwSocketID)<m_pInitParameter->m_wMaxConnect);
			if ((m_pBindParameter+LOWORD(pSearchCorrespond->dwSocketID))->dwSocketID!=pSearchCorrespond->dwSocketID) return true;

			//变量定义
			CMD_GP_S_SearchCorrespond SearchCorrespond;
			ZeroMemory(&SearchCorrespond,sizeof(SearchCorrespond));

			//设置变量
			for (WORD i=0;i<pSearchCorrespond->wUserCount;i++)
			{
				//数据效验
				ASSERT_ERR(SearchCorrespond.wUserCount<CountArray(SearchCorrespond.UserRemoteInfo));
				if (SearchCorrespond.wUserCount>=CountArray(SearchCorrespond.UserRemoteInfo)) break;

				//设置变量
				WORD wIndex=SearchCorrespond.wUserCount++;
				CopyMemory(&SearchCorrespond.UserRemoteInfo[wIndex],&pSearchCorrespond->UserRemoteInfo[i],sizeof(SearchCorrespond.UserRemoteInfo[wIndex]));
			}

			//发送数据
			WORD wHeadSize=sizeof(SearchCorrespond)-sizeof(SearchCorrespond.UserRemoteInfo);
			WORD wItemSize=sizeof(SearchCorrespond.UserRemoteInfo[0])*SearchCorrespond.wUserCount;
			m_pITCPNetworkEngine->SendData(pSearchCorrespond->dwSocketID,MDM_GP_REMOTE_SERVICE,SUB_GP_S_SEARCH_CORRESPOND,&SearchCorrespond,wHeadSize+wItemSize);

			return true;
		}
	}



	return true;
}

//登录处理
bool CAttemperEngineSink::OnTCPNetworkMainPCLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_LOGON_GAMEID:		//I D 登录
		{
			return OnTCPNetworkSubPCLogonGameID(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_LOGON_ACCOUNTS:		//帐号登录
		{
			return OnTCPNetworkSubPCLogonAccounts(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_LOGON_THIRD:		//三方登录
		{
			return OnTCPNetworkSubPCLogonThird(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_LOGON_THIRDPARTY:	//三方登录
		{
			return OnTCPNetworkSubPCLogonThirdparty(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_LOGON_91SDK:		//91登录
		{
			return OnTCPNetworkSubPCLogon91Sdk(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_REGISTER_ACCOUNTS:	//帐号注册
		{
			return OnTCPNetworkSubPCRegisterAccounts(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_REGISTER_ACCOUNTS_MB: //手机帐号注册
		{
			OnTCPNetworkSubPCRegisterAccountsMB(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_REGISTER_ACCOUNTS_THIRD: //第三方账号注册
		{
			OnTCPNetworkSubPCRegisterAccountsThird(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_GET_VALIDATE_CODE:  //获取验证码
		{
			return OnTCPNetworkSubPCGetValidateCode(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_WEB_REGISTER_ACCOUNTS:		// Web消息登录
		{
			return OnTcpNetworkSumWebRegisterAccounts(pData, wDataSize, dwSocketID);
		}
	}

	return false;
}

//列表处理
bool CAttemperEngineSink::OnTCPNetworkMainPCServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_GET_LIST:			//获取列表
		{
			//发送列表
			SendGameTypeInfo(dwSocketID);
			SendGameKindInfo(dwSocketID);

			//发送列表
			if (m_pInitParameter->m_cbDelayList==TRUE)
			{
				//发送列表
				//SendGamePageInfo(dwSocketID,INVALID_WORD);
				//SendGameNodeInfo(dwSocketID,INVALID_WORD);
				SendGameServerInfo(dwSocketID,INVALID_WORD);
			}
			else
			{
				//发送页面
				//SendGamePageInfo(dwSocketID,0);
			}

			//发送完成
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_FINISH);

			return true;
		}
	case SUB_GP_GET_SERVER:			//获取房间
		{
			//效验数据
			ASSERT_ERR(wDataSize%sizeof(WORD)==0);
			if (wDataSize%sizeof(WORD)!=0) return false;

			//发送列表
			UINT nKindCount=wDataSize/sizeof(WORD);
			for (UINT i=0;i<nKindCount;i++)
			{
				SendGameNodeInfo(dwSocketID,((WORD *)pData)[i]);
				//SendGamePageInfo(dwSocketID,((WORD *)pData)[i]);
				SendGameServerInfo(dwSocketID,((WORD *)pData)[i]);
			}

			//发送完成
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_SERVER_FINISH,pData,wDataSize);

			return true;
		}
	case SUB_GP_GET_ONLINE:			//获取在线
		{
			//变量定义
			CMD_GP_GetOnline * pGetOnline=(CMD_GP_GetOnline *)pData;
			WORD wHeadSize=(sizeof(CMD_GP_GetOnline)-sizeof(pGetOnline->wOnLineServerID));

			//效验数据
			ASSERT_ERR((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pGetOnline->wServerCount*sizeof(WORD))));
			if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pGetOnline->wServerCount*sizeof(WORD)))) return false;

			//变量定义
			CMD_GP_KindOnline KindOnline;
			CMD_GP_ServerOnline ServerOnline;
			ZeroMemory(&KindOnline,sizeof(KindOnline));
			ZeroMemory(&ServerOnline,sizeof(ServerOnline));

			//获取类型
			POSITION KindPosition=NULL;
			do
			{
				//获取类型
				CGameKindItem * pGameKindItem=m_ServerListManager.EmunGameKindItem(KindPosition);

				//设置变量
				if (pGameKindItem!=NULL)
				{
					WORD wKindIndex=KindOnline.wKindCount++;
					KindOnline.OnLineInfoKind[wKindIndex].wKindID=pGameKindItem->m_GameKind.wKindID;
					KindOnline.OnLineInfoKind[wKindIndex].dwOnLineCount=pGameKindItem->m_GameKind.dwOnLineCount;
				}

				//溢出判断
				if (KindOnline.wKindCount>=CountArray(KindOnline.OnLineInfoKind))
				{
					ASSERT_ERR(FALSE);
					break;
				}

			} while (KindPosition!=NULL);

			//获取房间
			for (WORD i=0;i<pGetOnline->wServerCount;i++)
			{
				//获取房间
				WORD wServerID=pGetOnline->wOnLineServerID[i];
				CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(wServerID);

				//设置变量
				if (pGameServerItem!=NULL)
				{
					WORD wServerIndex=ServerOnline.wServerCount++;
					ServerOnline.OnLineInfoServer[wServerIndex].wServerID=wServerID;
					ServerOnline.OnLineInfoServer[wServerIndex].dwOnLineCount=pGameServerItem->m_GameServer.dwOnLineCount;
				}

				//溢出判断
				if (ServerOnline.wServerCount>=CountArray(ServerOnline.OnLineInfoServer))
				{
					ASSERT_ERR(FALSE);
					break;
				}
			}

			//类型在线
			if (KindOnline.wKindCount>0)
			{
				WORD wHeadSize=sizeof(KindOnline)-sizeof(KindOnline.OnLineInfoKind);
				WORD wSendSize=wHeadSize+KindOnline.wKindCount*sizeof(KindOnline.OnLineInfoKind[0]);
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GR_KINE_ONLINE,&KindOnline,wSendSize);
			}

			//房间在线
			if (ServerOnline.wServerCount>0)
			{
				WORD wHeadSize=sizeof(ServerOnline)-sizeof(ServerOnline.OnLineInfoServer);
				WORD wSendSize=wHeadSize+ServerOnline.wServerCount*sizeof(ServerOnline.OnLineInfoServer[0]);
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GR_SERVER_ONLINE,&ServerOnline,wSendSize);
			}

			return true;
		}
	case SUB_GP_GET_COLLECTION:		//获取收藏
		{
			CMD_GP_Collection* collection = (CMD_GP_Collection *)pData;
			SendCollectionInfo(dwSocketID,*collection);
			return true;
		}
	}

	return false;
}

//服务处理
bool CAttemperEngineSink::OnTCPNetworkMainPCUserService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_CESHI:				//测试代码
		{
			LogonServer::PROTO_CESHI parseCeShi;
			parseCeShi.ParseFromArray(pData, wDataSize);
			
			DWORD Ceshi1 = parseCeShi.buserid();
			DWORD Ceshi2 = parseCeShi.cbplaystatus();
			DWORD Ceshi3 = parseCeShi.cbmaxcall();

			LogonServer::PROTO_CESHI parseCeShiFH;
			parseCeShiFH.set_buserid(3);
			parseCeShiFH.set_cbplaystatus(2);
			parseCeShiFH.set_cbmaxcall(1);

			std::string serializeStr;
			parseCeShiFH.SerializeToString(&serializeStr);
			m_pITCPNetworkEngine->SendData(dwSocketID, 3, 99, (void*)serializeStr.c_str(), (WORD)serializeStr.size());

			return true;
		}
	case SUB_GP_MODIFY_MACHINE:		//绑定机器
		{
			//效验参数
			ASSERT_ERR(wDataSize==sizeof(CMD_GP_ModifyMachine));
			if (wDataSize!=sizeof(CMD_GP_ModifyMachine)) return false;

			//处理消息
			CMD_GP_ModifyMachine * pModifyMachine=(CMD_GP_ModifyMachine *)pData;
			pModifyMachine->szPassword[CountArray(pModifyMachine->szPassword)-1]=0;

			//变量定义
			DBR_GP_ModifyMachine ModifyMachine;
			ZeroMemory(&ModifyMachine,sizeof(ModifyMachine));

			//构造数据
			ModifyMachine.cbBind=pModifyMachine->cbBind;
			ModifyMachine.dwUserID=pModifyMachine->dwUserID;
			ModifyMachine.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyMachine.szPassword,pModifyMachine->szPassword,CountArray(ModifyMachine.szPassword));
			lstrcpyn(ModifyMachine.szMachineID,pModifyMachine->szMachineID,CountArray(ModifyMachine.szMachineID));

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_MACHINE,dwSocketID,&ModifyMachine,sizeof(ModifyMachine));

			return true;
		}
	case SUB_GP_MODIFY_LOGON_PASS:	//修改密码
		{
			//效验参数
			//ASSERT_ERR(wDataSize==sizeof(CMD_GP_ModifyLogonPass));
			//if (wDataSize != sizeof(CMD_GP_ModifyLogonPass)) return false;

			//处理消息
			//CMD_GP_ModifyLogonPass * pModifyLogonPass=(CMD_GP_ModifyLogonPass *)pData;
			//pModifyLogonPass->szAccounts[CountArray(pModifyLogonPass->szAccounts) - 1] = 0;
			//pModifyLogonPass->szScrPassword[CountArray(pModifyLogonPass->szScrPassword) - 1] = 0;
			//pModifyLogonPass->szDesPassword[CountArray(pModifyLogonPass->szDesPassword) - 1] = 0;
			
			LogonServer::PROTO_ModifyLogonPass ProtoModifyLogonPass;
			ProtoModifyLogonPass.ParseFromArray(pData, wDataSize);
			
			//变量定义
			DBR_GP_ModifyLogonPass ModifyLogonPass;
			ZeroMemory(&ModifyLogonPass,sizeof(ModifyLogonPass));

			//构造数据
			ModifyLogonPass.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyLogonPass.szAccounts, Utils::ConvertA2W(ProtoModifyLogonPass.szaccounts()).c_str(), CountArray(ModifyLogonPass.szAccounts));
			lstrcpyn(ModifyLogonPass.szDesPassword, Utils::ConvertA2W(ProtoModifyLogonPass.szdespassword()).c_str(), CountArray(ModifyLogonPass.szDesPassword));
			lstrcpyn(ModifyLogonPass.szScrPassword, Utils::ConvertA2W(ProtoModifyLogonPass.szscrpassword()).c_str(), CountArray(ModifyLogonPass.szScrPassword));

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_LOGON_PASS,dwSocketID,&ModifyLogonPass,sizeof(ModifyLogonPass));

			return true;
		}
	case SUB_GP_MODIFY_INSURE_PASS:	//修改密码
		{
			//效验参数
			ASSERT_ERR(wDataSize==sizeof(CMD_GP_ModifyInsurePass));
			if (wDataSize!=sizeof(CMD_GP_ModifyInsurePass)) return false;

			//处理消息
			CMD_GP_ModifyInsurePass * pModifyInsurePass=(CMD_GP_ModifyInsurePass *)pData;
			pModifyInsurePass->szDesPassword[CountArray(pModifyInsurePass->szDesPassword)-1]=0;
			pModifyInsurePass->szScrPassword[CountArray(pModifyInsurePass->szScrPassword)-1]=0;

			//变量定义
			DBR_GP_ModifyInsurePass ModifyInsurePass;
			ZeroMemory(&ModifyInsurePass,sizeof(ModifyInsurePass));

			//构造数据
			ModifyInsurePass.dwUserID=pModifyInsurePass->dwUserID;
			ModifyInsurePass.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyInsurePass.szDesPassword,pModifyInsurePass->szDesPassword,CountArray(ModifyInsurePass.szDesPassword));
			lstrcpyn(ModifyInsurePass.szScrPassword,pModifyInsurePass->szScrPassword,CountArray(ModifyInsurePass.szScrPassword));

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_INSURE_PASS,dwSocketID,&ModifyInsurePass,sizeof(ModifyInsurePass));

			return true;
		}
		//修改签名
	case SUB_GP_MODIFY_UNDER_WRITE: return PostDataBaseRequest<DBR_GP_MODIFY_UNDER_WRITE, CMD_GP_ModifyUnderWrite>(dwSocketID, pData, wDataSize);
		//修改昵称
	case SUB_GP_MODIFY_NICKNAME: 
		{
			LogonServer::PROTO_ModifyNickName ProtoModifyNickName;
			ProtoModifyNickName.ParseFromArray(pData, wDataSize);

			//变量定义
			CMD_GP_ModifyNickName ModifyNickName;
			ZeroMemory(&ModifyNickName, sizeof(ModifyNickName));

			//构造数据
			ModifyNickName.dwUserID = ProtoModifyNickName.dwuserid();
			lstrcpyn(ModifyNickName.szNickName, Utils::ConvertA2W(ProtoModifyNickName.sznickname()).c_str(), CountArray(ModifyNickName.szNickName));
			
			//投递请求
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_NICKNAME, dwSocketID, &ModifyNickName, sizeof(ModifyNickName));

			//return PostDataBaseRequest<DBR_GP_MODIFY_NICKNAME, CMD_GP_ModifyNickName>(dwSocketID, pData, wDataSize);
		}
		//找回密码
	case SUB_GP_PW_RECOVERY: return PostDataBaseRequest<DBR_GP_PW_RECOVERY, CMD_GP_ModifyPass>(dwSocketID, pData, wDataSize);
		//获取头像
	case SUB_GP_GET_AVATAR_INFO: return PostDataBaseRequest<DBR_GP_GET_AVATAR_INFO, DWORD>(dwSocketID, pData, wDataSize);
		//获取形象
	case SUB_GP_GET_IMAGE_INFO: return PostDataBaseRequest<DBR_GP_GET_IMAGE_INFO, DWORD>(dwSocketID, pData, wDataSize);
		//修改头像
	case SUB_GP_MODIFY_AVATAR: return PostDataBaseRequest<DBR_GP_MODIFY_AVATAR, CMD_GP_ModifyAvatar>(dwSocketID, pData, wDataSize);
		//修改形象
	case SUB_GP_MODIFY_IMAGE: return PostDataBaseRequest<DBR_GP_MODIFY_IMAGE, CMD_GP_ModifyImage>(dwSocketID, pData, wDataSize);
		//查询信息
	case SUB_GP_QUERY_INDIVIDUAL: return PostDataBaseRequest<DBR_GP_QUERY_INDIVIDUAL, DWORD>(dwSocketID, pData, wDataSize);
	case SUB_GP_MODIFY_INDIVIDUAL:	//修改资料 实名认证
		{
			//效验参数
			ASSERT_ERR(wDataSize>=sizeof(CMD_GP_ModifyIndividual));
			if (wDataSize<sizeof(CMD_GP_ModifyIndividual)) return false;

			//处理消息
			CMD_GP_ModifyIndividual * pModifyIndividual=(CMD_GP_ModifyIndividual *)pData;
			pModifyIndividual->szVerificationCode[CountArray(pModifyIndividual->szVerificationCode) - 1] = 0;
			pModifyIndividual->szCompellation[CountArray(pModifyIndividual->szCompellation) - 1] = 0;
			pModifyIndividual->szMobilePhone[CountArray(pModifyIndividual->szMobilePhone) - 1] = 0;
			pModifyIndividual->szPassportID[CountArray(pModifyIndividual->szPassportID) - 1] = 0;
			pModifyIndividual->szPassword[CountArray(pModifyIndividual->szPassword) - 1] = 0;

			//变量定义
			DBR_GP_ModifyIndividual ModifyIndividual = { 0 };
			ModifyIndividual.dwUserID = pModifyIndividual->dwUserID;
			lstrcpyn(ModifyIndividual.szCompellation, pModifyIndividual->szCompellation, CountArray(ModifyIndividual.szCompellation));
			lstrcpyn(ModifyIndividual.szMobilePhone, pModifyIndividual->szMobilePhone, CountArray(ModifyIndividual.szMobilePhone));
			lstrcpyn(ModifyIndividual.szPassword, pModifyIndividual->szPassword, CountArray(ModifyIndividual.szPassword));
			lstrcpyn(ModifyIndividual.szPassportID, pModifyIndividual->szPassportID, CountArray(ModifyIndividual.szPassportID));

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_INDIVIDUAL, dwSocketID, &ModifyIndividual, sizeof(ModifyIndividual));

			return true;
		}
	case SUB_GP_USER_SAVE_SCORE:	//存入元宝
		{
			//效验参数
			//ASSERT_ERR(wDataSize==sizeof(CMD_GP_UserSaveScore));
			//if (wDataSize!=sizeof(CMD_GP_UserSaveScore)) return false;

			//处理消息
			//CMD_GP_UserSaveScore * pUserSaveScore=(CMD_GP_UserSaveScore *)pData;
			//pUserSaveScore->szMachineID[CountArray(pUserSaveScore->szMachineID)-1]=0;

			LogonServer::PROTO_UserSaveScore ProtoUserSaveScore;
			ProtoUserSaveScore.ParseFromArray(pData, wDataSize);

			//变量定义
			DBR_GP_UserSaveScore UserSaveScore;
			ZeroMemory(&UserSaveScore,sizeof(UserSaveScore));

			//构造数据
			UserSaveScore.dwUserID= ProtoUserSaveScore.dwuserid();
			UserSaveScore.lSaveScore= ProtoUserSaveScore.lsavescore();
			UserSaveScore.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(UserSaveScore.szMachineID, Utils::ConvertA2W(ProtoUserSaveScore.szmachineid()).c_str(),CountArray(UserSaveScore.szMachineID));

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_SAVE_SCORE,dwSocketID,&UserSaveScore,sizeof(UserSaveScore));

			return true;
		}
	case SUB_GP_USER_TAKE_SCORE:	//取出元宝
		{
			//效验参数
			//ASSERT_ERR(wDataSize==sizeof(CMD_GP_UserTakeScore));
			//if (wDataSize!=sizeof(CMD_GP_UserTakeScore)) return false;

			//处理消息
			//CMD_GP_UserTakeScore * pUserTakeScore=(CMD_GP_UserTakeScore *)pData;
			//pUserTakeScore->szPassword[CountArray(pUserTakeScore->szPassword)-1]=0;
			//pUserTakeScore->szMachineID[CountArray(pUserTakeScore->szMachineID)-1]=0;

			// 
			LogonServer::PROTO_UserTakeScore ProtoUserTakeScore;
			ProtoUserTakeScore.ParseFromArray(pData, wDataSize);

			//变量定义
			DBR_GP_UserTakeScore UserTakeScore;
			ZeroMemory(&UserTakeScore,sizeof(UserTakeScore));

			//构造数据
			UserTakeScore.dwUserID = ProtoUserTakeScore.dwuserid();
			UserTakeScore.lTakeScore = ProtoUserTakeScore.ltakescore();
			UserTakeScore.dwClientAddr = (m_pBindParameter + LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(UserTakeScore.szPassword, Utils::ConvertA2W(ProtoUserTakeScore.szpassword()).c_str(), CountArray(UserTakeScore.szPassword));
			lstrcpyn(UserTakeScore.szMachineID, Utils::ConvertA2W(ProtoUserTakeScore.szmachineid()).c_str(), CountArray(UserTakeScore.szMachineID));

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_TAKE_SCORE,dwSocketID,&UserTakeScore,sizeof(UserTakeScore));

			return true;
		}
	case SUB_GP_USER_TRANSFER_SCORE://转账元宝
		{
			//效验参数
			ASSERT_ERR(wDataSize==sizeof(CMD_GP_UserTransferScore));
			if (wDataSize!=sizeof(CMD_GP_UserTransferScore)) return false;

			//处理消息
			CMD_GP_UserTransferScore * pUserTransferScore=(CMD_GP_UserTransferScore *)pData;
			pUserTransferScore->szNickName[CountArray(pUserTransferScore->szNickName)-1]=0;
			pUserTransferScore->szPassword[CountArray(pUserTransferScore->szPassword)-1]=0;
			pUserTransferScore->szMachineID[CountArray(pUserTransferScore->szMachineID)-1]=0;

			//变量定义
			DBR_GP_UserTransferScore UserTransferScore;
			ZeroMemory(&UserTransferScore,sizeof(UserTransferScore));

			//构造数据
			UserTransferScore.dwUserID=pUserTransferScore->dwUserID;
			UserTransferScore.cbByNickName=pUserTransferScore->cbByNickName;
			UserTransferScore.lTransferScore=pUserTransferScore->lTransferScore;
			UserTransferScore.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(UserTransferScore.szNickName,pUserTransferScore->szNickName,CountArray(UserTransferScore.szNickName));
			lstrcpyn(UserTransferScore.szPassword,pUserTransferScore->szPassword,CountArray(UserTransferScore.szPassword));
			lstrcpyn(UserTransferScore.szMachineID,pUserTransferScore->szMachineID,CountArray(UserTransferScore.szMachineID));

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_TRANSFER_SCORE,dwSocketID,&UserTransferScore,sizeof(UserTransferScore));

			return true;
		}
	case SUB_GP_QUERY_INSURE_INFO:	//查询银行
		{
			//效验参数
			//ASSERT_ERR(wDataSize == sizeof(DWORD));
			//if (wDataSize != sizeof(DWORD)) return false;

			//处理消息
			//DWORD* pQueryInsureInfo=(DWORD*)pData;
			LogonServer::PROTO_QueryInsureInfo ProtoQueryInsureInfo;
			ProtoQueryInsureInfo.ParseFromArray(pData, wDataSize);

			//变量定义
			DBR_GP_QueryInsureInfo QueryInsureInfo;
			ZeroMemory(&QueryInsureInfo,sizeof(QueryInsureInfo));

			//构造数据
			QueryInsureInfo.dwUserID = ProtoQueryInsureInfo.dwuserid();
			QueryInsureInfo.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_INSURE_INFO,dwSocketID,&QueryInsureInfo,sizeof(QueryInsureInfo));

			return true;
		}
		//查询用户
	case SUB_GP_QUERY_USER_INFO_REQUEST: return PostDataBaseRequest<DBR_GP_QUERY_USER_INFO, CMD_GP_QueryUserInfoRequest>(dwSocketID, pData, wDataSize);
		//重新获取属性
	case SUB_GP_RELOAD_SCORE: return PostDataBaseRequest<DBR_GP_RELOAD_SCORE, DWORD>( dwSocketID, pData, wDataSize);

	case SUB_GP_BIND_VISITOR_ACCOUNTS:	//游客转正
		{
			//投递请求
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GP_BindVisitorAccount));
			if (wDataSize!=sizeof(CMD_GP_BindVisitorAccount)) return false;

			//处理消息
			CMD_GP_BindVisitorAccount * pSystemFaceInfo=(CMD_GP_BindVisitorAccount *)pData;

			//变量定义
			DBR_GP_BindVisitorAccount BindVisitorAccount;
			ZeroMemory(&BindVisitorAccount,sizeof(BindVisitorAccount));

			//构造数据
			BindVisitorAccount.dwUserID=pSystemFaceInfo->dwUserID;
			BindVisitorAccount.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;

			lstrcpyn(BindVisitorAccount.szAccounts,pSystemFaceInfo->szAccounts,CountArray(BindVisitorAccount.szAccounts));
			lstrcpyn(BindVisitorAccount.szNickName,pSystemFaceInfo->szNickName,CountArray(BindVisitorAccount.szNickName));
			lstrcpyn(BindVisitorAccount.szMachineID,pSystemFaceInfo->szMachineID,CountArray(BindVisitorAccount.szMachineID));
			lstrcpyn(BindVisitorAccount.szPassword,pSystemFaceInfo->szPassword,CountArray(BindVisitorAccount.szMachineID));
			lstrcpyn(BindVisitorAccount.szMobilePhone,pSystemFaceInfo->szMobilePhone,CountArray(BindVisitorAccount.szMobilePhone));

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_BIND_VISITOR_ACCOUNT,dwSocketID,&BindVisitorAccount,sizeof(BindVisitorAccount));

			return true;
		}

	case SUB_GP_BIND_MOBILE_PHONE:	// 绑定手机
		{
			//校验参数
			ASSERT(wDataSize == sizeof(CMD_GP_BindMobilePhone));
			if (wDataSize != sizeof(CMD_GP_BindMobilePhone)) return false;

			//处理消息
			CMD_GP_BindMobilePhone * pBindMobilePhone = (CMD_GP_BindMobilePhone *)pData;

			DBR_GP_BindMobile BindMobile = { 0 };
			BindMobile.dwUserID = pBindMobilePhone->dwUserID;
			lstrcpyn(BindMobile.szMobliePhone, pBindMobilePhone->szMobilePhone, CountArray(BindMobile.szMobliePhone));
			lstrcpyn(BindMobile.szPassword, pBindMobilePhone->szPassword, CountArray(BindMobile.szPassword));

			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_BIND_MOBILE, dwSocketID, &BindMobile, sizeof(BindMobile));

			return true;
		}

		//新找回密码
	case SUB_GP_RETRIEVE_PASSWORD: 
		{
			//校验参数
			ASSERT(wDataSize == sizeof(CMD_GP_RetrievePassword));
			if (wDataSize != sizeof(CMD_GP_RetrievePassword)) return false;

			CMD_GP_RetrievePassword * pRetrievePasswrod = (CMD_GP_RetrievePassword*)pData;

			return PostDataBaseRequest<DBR_GP_RETRIEVE_PASSWORD, CMD_GP_RetrievePassword>(dwSocketID, pData, wDataSize);
		}
		//////////////////////////////////////////////////////////////////////////
		//房间基本信息
		//获取签到
	case SUB_GP_GET_USER_SIGNIN: return OnTCPNetworkMainPCUserService_GetUserSignIn(pData,wDataSize,dwSocketID);
		//签到
	case SUB_GP_SET_USER_SIGNIN: return OnTCPNetworkMainPCUserService_SetUserSignIn(pData,wDataSize,dwSocketID);
		// 转盘
	case SUB_GP_GET_DIAL_INFO:
		{
			LogonServer::PROTO_GetDialInfo ProtoGetDialInfo;
			ProtoGetDialInfo.ParseFromArray(pData, wDataSize);

			//变量定义
			CMD_GP_GetDailInfo GetDailInfo;
			ZeroMemory(&GetDailInfo, sizeof(GetDailInfo));

			//构造数据
			GetDailInfo.dwUserID = ProtoGetDialInfo.dwuserid();
			GetDailInfo.dwClientAddr = (m_pBindParameter + LOWORD(dwSocketID))->dwClientAddr;
		
			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GETDAILINFO, dwSocketID, &GetDailInfo, sizeof(GetDailInfo));

			return true;
		}
		// 转动转盘领取奖励
	case SUB_GP_ROTATE_DAIL_AWAED:
		{
			LogonServer::PROTO_RotateDailAward ProtoRotateDailAward;
			ProtoRotateDailAward.ParseFromArray(pData, wDataSize);

			//变量定义
			CMD_GP_RotateDailAward RotateDailAward;
			ZeroMemory(&RotateDailAward, sizeof(RotateDailAward));

			//构造数据
			RotateDailAward.dwUserID = ProtoRotateDailAward.dwuserid();
			RotateDailAward.dwDailType = ProtoRotateDailAward.dwdailtype();
			RotateDailAward.dwClientAddr = (m_pBindParameter + LOWORD(dwSocketID))->dwClientAddr;

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_ROTATEDAILAWARD, dwSocketID, &RotateDailAward, sizeof(RotateDailAward));

			return true;
		}
		//查询绑定银行卡
	case SUB_GP_CHICK_BANKCARD:
		{
			LogonServer::PROTO_ChickBindBank ProtoChickBindBank;
			ProtoChickBindBank.ParseFromArray(pData, wDataSize);

			CMD_GP_ChickBindBank ChickBindBank;
			ZeroMemory(&ChickBindBank, sizeof(ChickBindBank));

			//构造数据
			ChickBindBank.dwUserID = ProtoChickBindBank.dwuserid();
			ChickBindBank.wSubCommandID = SUB_GP_CHICK_BANKCARD;
			ChickBindBank.dwClientAddr = (m_pBindParameter + LOWORD(dwSocketID))->dwClientAddr;
			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_CHICKBINDBANK, dwSocketID, &ChickBindBank, sizeof(ChickBindBank));


			return true;
		}
		// 绑定银行卡
	case SUB_GP_BIND_BANKCARD:
		{
			LogonServer::PROTO_GP_BindBankCard ProtoBindBank;
			ProtoBindBank.ParseFromArray(pData, wDataSize);

			CMD_GP_BindBankCard BindBank;
			ZeroMemory(&BindBank, sizeof(BindBank));
			BindBank.dwClientAddr = (m_pBindParameter + LOWORD(dwSocketID))->dwClientAddr;
			BindBank.wSubCommandID = SUB_GP_BIND_BANKCARD;
			BindBank.dwUserID = ProtoBindBank.dwuserid();
			lstrcpyn(BindBank.szBankName, Utils::ConvertA2W(ProtoBindBank.szbankname()).c_str(), CountArray(BindBank.szBankName));
			lstrcpyn(BindBank.szBankCardNum, Utils::ConvertA2W(ProtoBindBank.szbankcardnum()).c_str(), CountArray(BindBank.szBankCardNum));
			lstrcpyn(BindBank.szAccountHolder, Utils::ConvertA2W(ProtoBindBank.szaccountholder()).c_str(), CountArray(BindBank.szAccountHolder));
			
			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_BINDBANK, dwSocketID, &BindBank, sizeof(BindBank));
			return true;
		}
		// 查询绑定支付宝
	case SUB_GP_CHICK_BIND_ZFB:
		{
			LogonServer::PROTO_ChickBindZFB ProtoChickBindZFB;
			ProtoChickBindZFB.ParseFromArray(pData, wDataSize);

			CMD_GP_ChickBindZFB ChickBindZFB;
			ZeroMemory(&ChickBindZFB, sizeof(ChickBindZFB));

			//构造数据
			ChickBindZFB.dwUserID = ProtoChickBindZFB.dwuserid();
			ChickBindZFB.wSubCommandID = SUB_GP_CHICK_BIND_ZFB;
			ChickBindZFB.dwClientAddr = (m_pBindParameter + LOWORD(dwSocketID))->dwClientAddr;
			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_CHICKBINDZFB, dwSocketID, &ChickBindZFB, sizeof(ChickBindZFB));


			return true;
		}
		// 绑定支付宝
	case SUB_GP_BIND_ZFB:
		{
			LogonServer::PROTO_BindZFB ProtoBindZFB;
			ProtoBindZFB.ParseFromArray(pData, wDataSize);

			CMD_GP_BindZFB BindZFB;
			ZeroMemory(&BindZFB, sizeof(BindZFB));
			BindZFB.wSubCommandID = SUB_GP_BIND_ZFB;
			BindZFB.dwUserID = ProtoBindZFB.dwuserid();
			BindZFB.dwClientAddr = (m_pBindParameter + LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(BindZFB.szZFBName, Utils::ConvertA2W(ProtoBindZFB.szzfbname()).c_str(), CountArray(BindZFB.szZFBName));
			lstrcpyn(BindZFB.szNickName, Utils::ConvertA2W(ProtoBindZFB.sznickname()).c_str(), CountArray(BindZFB.szNickName));

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_BINDZFB, dwSocketID, &BindZFB, sizeof(BindZFB));
			return true;
		}
		// 请求邮件
	case SUB_GP_REQUEST_MAIL:
		{
			LogonServer::PROTO_RequestMail ProtoRequestMail;
			ProtoRequestMail.ParseFromArray(pData, wDataSize);

			CMD_GP_RequestMail RequestMail;
			ZeroMemory(&RequestMail, sizeof(RequestMail));
			RequestMail.dwUserID = ProtoRequestMail.dwuserid();
			RequestMail.dwClientAddr = (m_pBindParameter + LOWORD(dwSocketID))->dwClientAddr;

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_REQUEST_MAIL, dwSocketID, &RequestMail, sizeof(RequestMail));
			return true;
		}
		// 读取删除邮件
	case SUB_GP_READ_MAIL:
		{
			LogonServer::PROTO_ReadDeleteMail ProtoReadDeleteMail;
			ProtoReadDeleteMail.ParseFromArray(pData, wDataSize);

			CMD_GP_ReadDeleteMail ReadDeleteMail;
			ZeroMemory(&ReadDeleteMail, sizeof(ReadDeleteMail));
			ReadDeleteMail.dwUserID = ProtoReadDeleteMail.dwuserid();
			ReadDeleteMail.dwClientAddr = (m_pBindParameter + LOWORD(dwSocketID))->dwClientAddr;

			ReadDeleteMail.dwOpenType = ProtoReadDeleteMail.dwopentype();
			ReadDeleteMail.dwMailIndex = ProtoReadDeleteMail.dwmailindex();

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_READ_DELETE_MAIL, dwSocketID, &ReadDeleteMail, sizeof(ReadDeleteMail));
			return true;
		}
		//获取用户完善信息
	case SUB_GP_GET_USER_PERFECT_INFORMATION: return OnTCPNetworkMainPCUserService_GetUserPerfectInformation(pData,wDataSize,dwSocketID);
		//设置用户完善信息
	case SUB_GP_SET_USER_PERFECT_INFORMATION: return OnTCPNetworkMainPCUserService_SetUserPerfectInformation(pData,wDataSize,dwSocketID);
		//客户端登录完成
	case SUB_GP_CLIENT_LOGIN_SUCCESS : return OnTCPNetworkMainPCUserService_ClientLoginSuccess(pData,wDataSize,dwSocketID);
		//客户端退出房间
	case SUB_GP_CLIENT_EXIT_ROOM : return OnTCPNetworkMainPCUserService_ClientExitRoom(pData,wDataSize,dwSocketID);
		//绑定用户推广
	case SUB_GP_BIND_USER_SPREAD : return OnTCPNetworkMainPCUserService_BindUserSpread(pData,wDataSize,dwSocketID);
		//是否已经绑定推广
	case SUB_GP_IS_BIND_SPREAD : return OnTCPNetworkMainPCUserService_IsBindSpread(pData,wDataSize,dwSocketID);
		//是否已经修改过昵称
	case SUB_GP_IS_ALREADY_MODIFY_NICKNAME : return OnTCPNetworkMainPCUserService_IsAlreadyModifyNickName(pData,wDataSize,dwSocketID);
		
		

	default: break;
	}


	return false;
}

//远程处理
bool CAttemperEngineSink::OnTCPNetworkMainPCRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_C_SEARCH_CORRESPOND:	//协调查找
		{
			//效验参数
			ASSERT_ERR(wDataSize==sizeof(CMD_GP_C_SearchCorrespond));
			if (wDataSize!=sizeof(CMD_GP_C_SearchCorrespond)) return false;

			//处理消息
			CMD_GP_C_SearchCorrespond * pSearchCorrespond=(CMD_GP_C_SearchCorrespond *)pData;
			pSearchCorrespond->szNickName[CountArray(pSearchCorrespond->szNickName)-1]=0;

			//变量定义
			CMD_CS_C_SearchCorrespond SearchCorrespond;
			ZeroMemory(&SearchCorrespond,sizeof(SearchCorrespond));

			//连接变量
			SearchCorrespond.dwSocketID=dwSocketID;
			SearchCorrespond.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;

			//查找变量
			SearchCorrespond.dwGameID=pSearchCorrespond->dwGameID;
			lstrcpyn(SearchCorrespond.szNickName,pSearchCorrespond->szNickName,CountArray(SearchCorrespond.szNickName));

			//发送数据
			m_pITCPSocketService->SendData(MDM_CS_REMOTE_SERVICE,SUB_CS_C_SEARCH_CORRESPOND,&SearchCorrespond,sizeof(SearchCorrespond));

			return true;
		}
	}

	return false;
}

//登录处理
bool CAttemperEngineSink::OnTCPNetworkMainMBLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_LOGON_GAMEID:		//I D 登录
		{
			return OnTCPNetworkSubMBLogonGameID(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_LOGON_ACCOUNTS:		//帐号登录
		{
			return OnTCPNetworkSubMBLogonAccounts(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_REGISTER_ACCOUNTS:	//帐号注册
		{
			return OnTCPNetworkSubMBRegisterAccounts(pData,wDataSize,dwSocketID);
		}
	case SUB_MB_VISITOR_ACCOUNTS: //游客身份
		{
			return OnTCPNetworkSubMBVisitorAccounts(pData,wDataSize,dwSocketID);
		}
	case SUB_MB_REGISTER_MOBILE: //手机注册
		{
			return OnTCPNetworkSubMBRegisterMobile(pData, wDataSize, dwSocketID);
		}
	}

	return false;
}

//列表处理
bool CAttemperEngineSink::OnTCPNetworkMainMBServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_GET_SERVER:
		{
			//效验数据
			ASSERT_ERR(wDataSize%sizeof(WORD) == 0);
			if (wDataSize%sizeof(WORD) != 0) return false;

			//游戏ID
			WORD wKindID = *(WORD*)pData;

			//发送房间列表
			SendMobileServerInfo(dwSocketID, wKindID);

			return true;
		}
	case SUB_GP_GET_MATCH:
		{
			//效验数据
			ASSERT_ERR(wDataSize%sizeof(WORD) == 0);
			if (wDataSize%sizeof(WORD) != 0) return false;

			//游戏ID
			WORD wKindID = *(WORD*)pData;

			//发送比赛房间列表
			SendMobileMatchInfo(dwSocketID,wKindID);

			return true;
		}
	case SUB_MB_KINE_ONLINE:
		{
			WORD wSendSize=0;
			BYTE cbDataBuffer[SOCKET_TCP_PACKET];

			//枚举数据
			POSITION Position=NULL;
			CGameServerItem * pGameServerItem=NULL;
			CMD_GP_UserOnline UserOnline = {0};
			DWORD dwRand = 0;
			DWORD dwRand_ = 0;
			for (DWORD i = 0; i<m_ServerListManager.GetGameServerCount(); i++)
			{

				//获取数据
				pGameServerItem = m_ServerListManager.EmunGameServerItem(Position);
				if (pGameServerItem == NULL) break;


				if ( pGameServerItem->m_GameServer.wServerType != GAME_GENRE_MATCH)
				{
					dwRand_ = rand() % 800 + 300;
					dwRand = rand() % 100 + dwRand_;
					UserOnline.wRoomOnline += pGameServerItem->m_GameServer.dwOnLineCount+dwRand;
				}
				else 
				{
					dwRand_ = rand() % 800 + 300;
					dwRand = rand() % 100 + dwRand_;
					UserOnline.wMatchOnline += pGameServerItem->m_GameServer.dwOnLineCount+dwRand;
				}
			}
			UserOnline.wActiveOnline = 0;

			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_KINE_ONLINE,&UserOnline,sizeof(UserOnline));
			return true;
		}
	}

	return false;
}

//服务处理
bool CAttemperEngineSink::OnTCPNetworkMainMBUserService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch( wSubCmdID )
	{
	case SUB_GP_USER_DAILYGET: return PostDataBaseRequest<DBR_GP_USER_DAILYGET, DWORD>(dwSocketID, pData, wDataSize);
	}

	return false;
}

//发送房间
VOID CAttemperEngineSink::SendMobileServerInfo(DWORD dwSocketID, WORD wModuleID)
{
	//网络数据
	WORD wSendSize = 0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//枚举数据
	POSITION Position = NULL;
	CGameServerItem * pGameServerItem = NULL;
	DWORD dwRand = 0;
	DWORD dwRand_ = 0;
	//枚举数据
	for (DWORD i = 0; i<m_ServerListManager.GetGameServerCount(); i++)
	{
		//发送数据
		if ((wSendSize + sizeof(tagGameServer_))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID, MDM_MB_SERVER_LIST, SUB_MB_LIST_SERVER, cbDataBuffer, wSendSize);
			wSendSize = 0;
		}

		//获取数据
		pGameServerItem = m_ServerListManager.EmunGameServerItem(Position);
		if (pGameServerItem == NULL) break;

		dwRand_ = rand() % 800 + 300;
		dwRand = rand() % 100 + dwRand_;
		//拷贝数据（过滤比赛房间）
		if (pGameServerItem->m_GameServer.wKindID == wModuleID && pGameServerItem->m_GameServer.wServerType != GAME_GENRE_MATCH)
		{
			tagGameServer_ GameServer;
			memset(GameServer.szServerName, 0, sizeof GameServer.szServerName);

			GameServer.dwFullCount = pGameServerItem->m_GameServer.dwFullCount;
			GameServer.dwOnLineCount = pGameServerItem->m_GameServer.dwOnLineCount + dwRand;
			GameServer.lCellScore = pGameServerItem->m_GameServer.lCellScore;
			_tcscpy_s(GameServer.szServerAddr, pGameServerItem->m_GameServer.szServerAddr);
			_tcscpy_s(GameServer.szServerName, pGameServerItem->m_GameServer.szServerName);
			GameServer.wServerID = pGameServerItem->m_GameServer.wServerID;
			GameServer.wServerPort = pGameServerItem->m_GameServer.wServerPort;
			GameServer.wServerType = pGameServerItem->m_GameServer.wServerType;
			GameServer.cbRoomType = pGameServerItem->m_GameServer.cbRoomType;

			//限制配置
			GameServer.lMinEnterScore	= pGameServerItem->m_GameServer.lMinEnterScore	;		//获得血量
			GameServer.lMinTableScore	= pGameServerItem->m_GameServer.lMinTableScore	;		//最低积分
			GameServer.lMaxEnterScore	= pGameServerItem->m_GameServer.lMaxEnterScore	;		//最低体力

			GameServer.wKindID			= pGameServerItem->m_GameServer.wKindID;

			CopyMemory(cbDataBuffer + wSendSize, &GameServer, sizeof(tagGameServer_));
			wSendSize += sizeof(tagGameServer_);
		}
	}

	//发送剩余
	if (wSendSize > 0) m_pITCPNetworkEngine->SendData(dwSocketID, MDM_MB_SERVER_LIST, SUB_MB_LIST_SERVER, cbDataBuffer, wSendSize);

	m_pITCPNetworkEngine->SendData(dwSocketID, MDM_MB_SERVER_LIST, SUB_MB_LIST_FINISH, &wModuleID, sizeof(WORD));

	return;
}

VOID CAttemperEngineSink::SendMobileMatchInfo(DWORD dwSocketID, WORD wModuleID)
{
	return;
}

//I D 登录
bool CAttemperEngineSink::OnTCPNetworkSubPCLogonGameID(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT_ERR(wDataSize>=sizeof(CMD_GP_LogonGameID));
	if (wDataSize<sizeof(CMD_GP_LogonGameID))
	{
		if (wDataSize<sizeof(CMD_GP_LogonGameID)-sizeof(BYTE))
			return false;
	}

	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//处理消息
	CMD_GP_LogonGameID * pLogonGameID=(CMD_GP_LogonGameID *)pData;
	pLogonGameID->szPassword[CountArray(pLogonGameID->szPassword)-1]=0;
	pLogonGameID->szMachineID[CountArray(pLogonGameID->szMachineID)-1]=0;

	//设置连接
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pLogonGameID->dwPlazaVersion;

	//效验版本
	if (CheckPlazaVersion(DEVICE_TYPE_PC,pLogonGameID->dwPlazaVersion,dwSocketID,((pLogonGameID->cbValidateFlags&LOW_VER_VALIDATE_FLAGS)!=0))==false)
	{
		return true;
	}

	//变量定义
	DBR_GP_LogonGameID LogonGameID;
	ZeroMemory(&LogonGameID,sizeof(LogonGameID));

	//附加信息
	LogonGameID.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//构造数据
	LogonGameID.dwGameID=pLogonGameID->dwGameID;
	LogonGameID.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(LogonGameID.szPassword,pLogonGameID->szPassword,CountArray(LogonGameID.szPassword));
	lstrcpyn(LogonGameID.szMachineID,pLogonGameID->szMachineID,CountArray(LogonGameID.szMachineID));
	LogonGameID.cbNeeValidateMBCard=(pLogonGameID->cbValidateFlags&MB_VALIDATE_FLAGS);

	//投递请求
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOGON_GAMEID,dwSocketID,&LogonGameID,sizeof(LogonGameID));

	return true;
}

//QQ,微博登录
bool CAttemperEngineSink::OnTCPNetworkSubPCLogonThird(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT_ERR(wDataSize>=sizeof(CMD_GP_LogonThird));


	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//处理消息
	CMD_GP_LogonThird * pLogonAccounts=(CMD_GP_LogonThird *)pData;

	pLogonAccounts->szMachineID[CountArray(pLogonAccounts->szMachineID)-1]=0;

	//设置连接
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pLogonAccounts->dwPlazaVersion;

	//版本判断
	if (CheckPlazaVersion(DEVICE_TYPE_PC,pLogonAccounts->dwPlazaVersion,dwSocketID,((pLogonAccounts->cbValidateFlags&LOW_VER_VALIDATE_FLAGS)!=0))==false)
	{
		return true;
	}

	//变量定义
	DBR_GP_LogonThird LogonAccounts;
	ZeroMemory(&LogonAccounts,sizeof(LogonAccounts));

	//附加信息
	LogonAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	LogonAccounts.dwUserID = pLogonAccounts->dwUserID;
	//构造数据
	LogonAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(LogonAccounts.szMachineID,pLogonAccounts->szMachineID,CountArray(LogonAccounts.szMachineID));
	LogonAccounts.cbNeeValidateMBCard=(pLogonAccounts->cbValidateFlags&MB_VALIDATE_FLAGS);

	//投递请求
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOGON_Third,dwSocketID,&LogonAccounts,sizeof(LogonAccounts));

	return true;
}

//帐号登录
bool CAttemperEngineSink::OnTCPNetworkSubPCLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT_ERR(wDataSize>=sizeof(CMD_GP_LogonAccounts));
	if (wDataSize<sizeof(CMD_GP_LogonAccounts))
	{
		if (wDataSize<sizeof(CMD_GP_LogonAccounts)-sizeof(BYTE))
			return false;
	}

	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//处理消息
	CMD_GP_LogonAccounts * pLogonAccounts=(CMD_GP_LogonAccounts *)pData;
	pLogonAccounts->szAccounts[CountArray(pLogonAccounts->szAccounts)-1]=0;
	pLogonAccounts->szPassword[CountArray(pLogonAccounts->szPassword)-1]=0;
	pLogonAccounts->szMachineID[CountArray(pLogonAccounts->szMachineID)-1]=0;

	//设置连接
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pLogonAccounts->dwPlazaVersion;

	//版本判断
	if (CheckPlazaVersion(DEVICE_TYPE_PC,pLogonAccounts->dwPlazaVersion,dwSocketID,((pLogonAccounts->cbValidateFlags&LOW_VER_VALIDATE_FLAGS)!=0))==false)
	{
		return true;
	}

	//变量定义
	DBR_GP_LogonAccounts LogonAccounts;
	ZeroMemory(&LogonAccounts,sizeof(LogonAccounts));

	//附加信息
	LogonAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//构造数据
	LogonAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(LogonAccounts.szAccounts,pLogonAccounts->szAccounts,CountArray(LogonAccounts.szAccounts));
	lstrcpyn(LogonAccounts.szPassword,pLogonAccounts->szPassword,CountArray(LogonAccounts.szPassword));
	lstrcpyn(LogonAccounts.szMachineID,pLogonAccounts->szMachineID,CountArray(LogonAccounts.szMachineID));
	LogonAccounts.cbNeeValidateMBCard=(pLogonAccounts->cbValidateFlags&MB_VALIDATE_FLAGS);

	//投递请求
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOGON_ACCOUNTS,dwSocketID,&LogonAccounts,sizeof(LogonAccounts));

	//I_TRACE(TEXT("OnTCPNetworkSubPCLogonAccounts %s"),pLogonAccounts->szAccounts);
	return true;
}

//三方登录
bool CAttemperEngineSink::OnTCPNetworkSubPCLogonThirdparty(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT_ERR(wDataSize>=sizeof(CMD_GP_LogonThirdParty));
	if (wDataSize<sizeof(CMD_GP_LogonThirdParty))
	{
		if (wDataSize<sizeof(CMD_GP_LogonThirdParty)-sizeof(BYTE))
			return false;
	}

	//处理消息
	CMD_GP_LogonThirdParty * pLogonThirdParty=(CMD_GP_LogonThirdParty *)pData;
	pLogonThirdParty->szAccounts[CountArray(pLogonThirdParty->szAccounts)-1]=0;
	pLogonThirdParty->szPassword[CountArray(pLogonThirdParty->szPassword)-1]=0;

	CInternetSession session;
	CHttpConnection *p_http_connection = NULL;
	CHttpFile *p_http_file = NULL;

	CString url_server;	// 服务器名称
	CString url_name;	// 文件名
	INTERNET_PORT port; // 端口号
	DWORD server_type;	// 服务类型。

	TCHAR PassWord[LEN_MD5] = {L"119781010"};
	CWHEncrypt::MD5Encrypt(PassWord,PassWord);
	CWHEncrypt::MD5Encrypt(pLogonThirdParty->szPassword,pLogonThirdParty->szPassword);


	TCHAR szNavigation[256]=TEXT("");																	
	_sntprintf(szNavigation,CountArray(szNavigation),TEXT("http://www.gxhc365.com/api_login/%s/%s"),pLogonThirdParty->szAccounts,pLogonThirdParty->szPassword);
	if (!AfxParseURL(szNavigation, server_type, url_server, url_name, port))
	{
		return 1;
	}

	try
	{
		// 连接到 Http 服务器
		session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);		//重试次数
		session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 2000);	//重试之间的等待延时
		p_http_connection = session.GetHttpConnection(url_server, port);

		// 打开 Http 请求
		p_http_file = p_http_connection->OpenRequest(CHttpConnection::HTTP_VERB_GET, url_name);

		if (!p_http_file)
			throw 0;

		// 发送 Http 请求
		if (!p_http_file->SendRequest())
			throw 0;

		DWORD state_code;	// 获取状态码
		p_http_file->QueryInfoStatusCode(state_code);
		char save_buffer;
		if (state_code == HTTP_STATUS_OK) // 请求完成
		{
			UINT read_size;	// 实际读入的字节数。
			read_size = p_http_file->Read(&save_buffer, 1);
		}

		if (p_http_file)
			p_http_file->Close();
		if (p_http_connection)
			p_http_connection->Close();
		session.Close();

		int isRegister = save_buffer;
		if (isRegister == 49) //授权成功
		{
			DBR_GP_LogonThirdParty LogonThirdParty;
			lstrcpyn(LogonThirdParty.szAccounts,pLogonThirdParty->szAccounts,CountArray(LogonThirdParty.szAccounts));
			lstrcpyn(LogonThirdParty.szPassword,pLogonThirdParty->szPassword,CountArray(LogonThirdParty.szPassword));

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOGON_ThirdParty,dwSocketID,&LogonThirdParty,sizeof(LogonThirdParty));
		}
		else //授权失败
		{
			DBO_GP_OperateResult logonFailure;
			logonFailure.dwResultCode = -1;
			lstrcpyn(logonFailure.szDescribeString,L"第三方授权失败，账号密码有误", sizeof(logonFailure.szDescribeString));
			OnDBPCLogonFailure(dwSocketID,&logonFailure,sizeof(logonFailure));
		}
	}
	catch (CInternetException* ex)
	{
		wchar_t message[1024];
		ex->GetErrorMessage(message, 1024);
		TRACE(message);
	}

	return true;
}

bool CAttemperEngineSink::OnTCPNetworkSubPCLogon91Sdk( VOID * pData, WORD wDataSize, DWORD dwSocketID )
{
	//效验参数
	ASSERT_ERR(wDataSize>=sizeof(CMD_GP_Logon91Sdk));
	if (wDataSize<sizeof(CMD_GP_Logon91Sdk))
	{
		if (wDataSize<sizeof(CMD_GP_Logon91Sdk)-sizeof(BYTE))
			return false;
	}

	CMD_GP_Logon91Sdk * pLogon91Sdk=(CMD_GP_Logon91Sdk *)pData;
	pLogon91Sdk->szAccounts[CountArray(pLogon91Sdk->szAccounts)-1]=0;

	TCHAR  md5pwdout[LEN_MD5] = L"D07F3DC914A02043B82B0743AC8D58B5";


	DBR_GP_LogonThirdParty LogonThirdParty;
	lstrcpyn(LogonThirdParty.szAccounts,pLogon91Sdk->szAccounts,CountArray(LogonThirdParty.szAccounts));
	lstrcpyn(LogonThirdParty.szPassword,md5pwdout,CountArray(LogonThirdParty.szPassword));

	//投递请求
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOGON_ThirdParty,dwSocketID,&LogonThirdParty,sizeof(LogonThirdParty));

	return true;
}

//获取验证码
bool CAttemperEngineSink::OnTCPNetworkSubPCGetValidateCode(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	INT nWidth=77;
	INT nHeight=18;
	//随机数字验证码
	TCHAR vc[5]={0};
	INT nTmp=0;
	for(int i=0;i<4;i++)
	{
		if(rand()%2==0)//数字
		{
			vc[i]=TEXT('0')+rand()%9+1;
		}
		else//字母
		{
			nTmp=rand()%26;
			while(nTmp==14)
			{
				nTmp=rand()%26;
			}			
			vc[i]=nTmp+65;	
			//if(rand()%2==0)//大写字母
			//{
			//	vc[i]=nTmp+65;	
			//}
			//else//小写字母
			//{
			//	vc[i]=nTmp+97;				
			//}
		}
	}
	//生成验证码图片
	//构造图片
	CImage imagevc;
	imagevc.Create(nWidth,nHeight,32);

	//创建 DC
	CImageDC BufferDC(imagevc);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	//填充背景
	pBufferDC->FillSolidRect(1,1,nWidth-2,nHeight-2,RGB(255,255,255));
	pBufferDC->Draw3dRect(0,0,nWidth,nHeight,RGB(255,255,255),RGB(125,125,125));

	//随机背景
	for (INT nXPos=1;nXPos<nWidth-1;nXPos+=2)
	{
		for (INT nYPos=1;nYPos<nHeight-1;nYPos+=2)
		{
			pBufferDC->SetPixel(nXPos,nYPos,RGB(rand()%255,rand()%255,rand()%255));
		}
	}

	//设置 DC
	pBufferDC->SetBkMode(TRANSPARENT);
	pBufferDC->SetTextColor(RGB(0,0,0));

	//变量定义
	INT nXSpace=nWidth/CountArray(vc);

	//显示内容
	for (BYTE i=0;i<CountArray(vc);i++)
	{
		//位置定义
		CRect rcDraw;
		rcDraw.SetRect(i*nXSpace+rand()%10,0,(i+1)*nXSpace+rand()%10,nHeight);

		//创建字体
		CFont DrawFont;
		DrawFont.CreateFont(-14,0,0,0,800,0,0,0,134,3,2,ANTIALIASED_QUALITY,2,TEXT("宋体"));

		//显示内容
		pBufferDC->SelectObject(DrawFont);
		pBufferDC->DrawText(&vc[i],1,&rcDraw,DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	}	
	//绘制随机线
	CPen pen;
	pen.CreatePen(PS_SOLID,1,0xFF000000);
	pBufferDC->SelectObject(&pen);
	for(int i=0;i<2;i++)
	{
		pBufferDC->MoveTo(0,rand()%18);
		pBufferDC->LineTo(76,rand()%18);
	}

	DWORD buf[77*18];
	INT t=0;
	for(int x=0;x<nWidth;x++)
	{
		for(int y=0;y<nHeight;y++)
		{
			buf[t]=pBufferDC->GetPixel(x,y);
			t++;
		}
	}
	//把验证码存入map字典
	std::wstring key=(TCHAR*)pData;
	std::wstring val=vc;
	if(m_ValidateCodeMap.find(key)!=m_ValidateCodeMap.end()) m_ValidateCodeMap.erase(key);
	m_ValidateCodeMap.insert(std::make_pair(key,val));
	//发送图片
	m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_LOGON,SUB_GP_VALIDATE_CODE,(void*)buf,nWidth*nHeight*4);
	return true;
}

//帐号注册
bool CAttemperEngineSink::OnTCPNetworkSubPCRegisterAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT_ERR(wDataSize>=sizeof(CMD_GP_RegisterAccounts));
	if (wDataSize<sizeof(CMD_GP_RegisterAccounts))
	{
		if (wDataSize<sizeof(CMD_GP_RegisterAccounts)-sizeof(BYTE))
			return false;
	}

	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//处理消息
	CMD_GP_RegisterAccounts * pRegisterAccounts=(CMD_GP_RegisterAccounts *)pData;
	pRegisterAccounts->szAccounts[CountArray(pRegisterAccounts->szAccounts)-1]=0;
	pRegisterAccounts->szNickName[CountArray(pRegisterAccounts->szNickName)-1]=0;
	pRegisterAccounts->szSpreader[CountArray(pRegisterAccounts->szSpreader)-1]=0;
	pRegisterAccounts->szMachineID[CountArray(pRegisterAccounts->szMachineID)-1]=0;
	pRegisterAccounts->szLogonPass[CountArray(pRegisterAccounts->szLogonPass)-1]=0;
	pRegisterAccounts->szInsurePass[CountArray(pRegisterAccounts->szInsurePass)-1]=0;
	pRegisterAccounts->szPassPortID[CountArray(pRegisterAccounts->szPassPortID)-1]=0;
	pRegisterAccounts->szCompellation[CountArray(pRegisterAccounts->szCompellation)-1]=0;
	//TRACE_LOG(pRegisterAccounts->szValidateCode,enTraceLevel::TraceLevel_Info);
	//验证验证码
	bool bIsOK=true;
	std::wstring key=pRegisterAccounts->szMachineID;
	wstring val;
	val=pRegisterAccounts->szValidateCode;
	if(m_ValidateCodeMap.find(key)==m_ValidateCodeMap.end()) 
	{
		bIsOK=false;
	}
	else
	{
		if(m_ValidateCodeMap[key] != val) bIsOK=false;
		m_ValidateCodeMap.erase(key);
	}

	if(bIsOK==false)
	{
		CMD_GP_LogonFailure LogonFailure;
		ZeroMemory(&LogonFailure,sizeof(LogonFailure));
		LogonFailure.lResultCode=7;
		TCHAR* szDescriptionString=TEXT("效验码效验验证失败，请重新输入！");
		lstrcpyn(LogonFailure.szDescribeString,szDescriptionString,17);
		WORD wStringSize=CountStringBuffer(LogonFailure.szDescribeString);
		WORD wSendSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString)+wStringSize;
		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_LOGON,SUB_GP_LOGON_FAILURE,&LogonFailure,wSendSize);
		return true;
	}
	//设置连接
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pRegisterAccounts->dwPlazaVersion;

	//效验版本
	if (CheckPlazaVersion(DEVICE_TYPE_PC,pRegisterAccounts->dwPlazaVersion,dwSocketID,((pRegisterAccounts->cbValidateFlags&LOW_VER_VALIDATE_FLAGS)!=0))==false)
	{
		return true;
	}

	//变量定义
	DBR_GP_RegisterAccounts RegisterAccounts;
	ZeroMemory(&RegisterAccounts,sizeof(RegisterAccounts));

	//附加信息
	RegisterAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//构造数据
	RegisterAccounts.wFaceID=pRegisterAccounts->wFaceID;
	RegisterAccounts.cbGender=pRegisterAccounts->cbGender;
	RegisterAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(RegisterAccounts.szAccounts,pRegisterAccounts->szAccounts,CountArray(RegisterAccounts.szAccounts));
	lstrcpyn(RegisterAccounts.szNickName,pRegisterAccounts->szNickName,CountArray(RegisterAccounts.szNickName));
	lstrcpyn(RegisterAccounts.szSpreader,pRegisterAccounts->szSpreader,CountArray(RegisterAccounts.szSpreader));
	lstrcpyn(RegisterAccounts.szMachineID,pRegisterAccounts->szMachineID,CountArray(RegisterAccounts.szMachineID));
	lstrcpyn(RegisterAccounts.szLogonPass,pRegisterAccounts->szLogonPass,CountArray(RegisterAccounts.szLogonPass));
	lstrcpyn(RegisterAccounts.szInsurePass,pRegisterAccounts->szInsurePass,CountArray(RegisterAccounts.szInsurePass));
	lstrcpyn(RegisterAccounts.szPassPortID,pRegisterAccounts->szPassPortID,CountArray(RegisterAccounts.szPassPortID));
	lstrcpyn(RegisterAccounts.szCompellation,pRegisterAccounts->szCompellation,CountArray(RegisterAccounts.szCompellation));

	//投递请求
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_REGISTER_ACCOUNTS,dwSocketID,&RegisterAccounts,sizeof(RegisterAccounts));

	//I_TRACE(TEXT("OnTCPNetworkSubPCRegisterAccounts %s"),pRegisterAccounts->szAccounts);
	return true;
}

//手机注册账号
bool CAttemperEngineSink::OnTCPNetworkSubPCRegisterAccountsMB(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT_ERR(wDataSize>=sizeof(CMD_GP_RegisterAccounts));
	if (wDataSize<sizeof(CMD_GP_RegisterAccounts))
	{
		if (wDataSize<sizeof(CMD_GP_RegisterAccounts)-sizeof(BYTE))
			return false;
	}

	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//处理消息
	CMD_GP_RegisterAccounts * pRegisterAccounts=(CMD_GP_RegisterAccounts *)pData;
	pRegisterAccounts->szAccounts[CountArray(pRegisterAccounts->szAccounts)-1]=0;
	pRegisterAccounts->szNickName[CountArray(pRegisterAccounts->szNickName)-1]=0;
	pRegisterAccounts->szSpreader[CountArray(pRegisterAccounts->szSpreader)-1]=0;
	pRegisterAccounts->szMachineID[CountArray(pRegisterAccounts->szMachineID)-1]=0;
	pRegisterAccounts->szLogonPass[CountArray(pRegisterAccounts->szLogonPass)-1]=0;
	pRegisterAccounts->szInsurePass[CountArray(pRegisterAccounts->szInsurePass)-1]=0;
	pRegisterAccounts->szPassPortID[CountArray(pRegisterAccounts->szPassPortID)-1]=0;
	pRegisterAccounts->szCompellation[CountArray(pRegisterAccounts->szCompellation)-1]=0;

	//验证验证码
	bool bIsOK=true;
	std::wstring key=pRegisterAccounts->szMachineID;
	wstring val;
	val=pRegisterAccounts->szValidateCode;

	if(bIsOK==false)
	{
		CMD_GP_LogonFailure LogonFailure;
		ZeroMemory(&LogonFailure,sizeof(LogonFailure));
		LogonFailure.lResultCode=7;
		TCHAR* szDescriptionString=TEXT("效验码效验验证失败，请重新输入！");
		lstrcpyn(LogonFailure.szDescribeString,szDescriptionString,17);
		WORD wStringSize=CountStringBuffer(LogonFailure.szDescribeString);
		WORD wSendSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString)+wStringSize;
		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_LOGON,SUB_GP_LOGON_FAILURE,&LogonFailure,wSendSize);
		return true;
	}
	//设置连接
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pRegisterAccounts->dwPlazaVersion;

	//效验版本
	if (CheckPlazaVersion(DEVICE_TYPE_PC,pRegisterAccounts->dwPlazaVersion,dwSocketID,((pRegisterAccounts->cbValidateFlags&LOW_VER_VALIDATE_FLAGS)!=0))==false)
	{
		return true;
	}

	//变量定义
	DBR_GP_RegisterAccounts RegisterAccounts;
	ZeroMemory(&RegisterAccounts,sizeof(RegisterAccounts));

	//附加信息
	RegisterAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//构造数据
	RegisterAccounts.wFaceID=pRegisterAccounts->wFaceID;
	RegisterAccounts.cbGender=pRegisterAccounts->cbGender;
	RegisterAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(RegisterAccounts.szAccounts,pRegisterAccounts->szAccounts,CountArray(RegisterAccounts.szAccounts));
	lstrcpyn(RegisterAccounts.szNickName,pRegisterAccounts->szNickName,CountArray(RegisterAccounts.szNickName));
	lstrcpyn(RegisterAccounts.szSpreader,pRegisterAccounts->szSpreader,CountArray(RegisterAccounts.szSpreader));
	lstrcpyn(RegisterAccounts.szMachineID,pRegisterAccounts->szMachineID,CountArray(RegisterAccounts.szMachineID));
	lstrcpyn(RegisterAccounts.szLogonPass,pRegisterAccounts->szLogonPass,CountArray(RegisterAccounts.szLogonPass));
	lstrcpyn(RegisterAccounts.szInsurePass,pRegisterAccounts->szInsurePass,CountArray(RegisterAccounts.szInsurePass));
	lstrcpyn(RegisterAccounts.szPassPortID,pRegisterAccounts->szPassPortID,CountArray(RegisterAccounts.szPassPortID));
	lstrcpyn(RegisterAccounts.szCompellation,pRegisterAccounts->szCompellation,CountArray(RegisterAccounts.szCompellation));

	//投递请求
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_REGISTER_ACCOUNTS,dwSocketID,&RegisterAccounts,sizeof(RegisterAccounts));

	return true;
}

//第三方账号注册
bool CAttemperEngineSink::OnTCPNetworkSubPCRegisterAccountsThird(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT_ERR(wDataSize>=sizeof(CMD_GP_RegisterThird));
	if (wDataSize<sizeof(CMD_GP_RegisterThird))
	{
		if (wDataSize<sizeof(CMD_GP_RegisterThird)-sizeof(BYTE))
			return false;
	}

	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//处理消息
	CMD_GP_RegisterThird * pRegisterAccounts=(CMD_GP_RegisterThird *)pData;
	pRegisterAccounts->szAccounts[CountArray(pRegisterAccounts->szAccounts)-1]=0;
	pRegisterAccounts->szNickName[CountArray(pRegisterAccounts->szNickName)-1]=0;
	pRegisterAccounts->szSpreader[CountArray(pRegisterAccounts->szSpreader)-1]=0;
	pRegisterAccounts->szMachineID[CountArray(pRegisterAccounts->szMachineID)-1]=0;
	pRegisterAccounts->szLogonPass[CountArray(pRegisterAccounts->szLogonPass)-1]=0;
	pRegisterAccounts->szInsurePass[CountArray(pRegisterAccounts->szInsurePass)-1]=0;
	pRegisterAccounts->szPassPortID[CountArray(pRegisterAccounts->szPassPortID)-1]=0;
	pRegisterAccounts->szCompellation[CountArray(pRegisterAccounts->szCompellation)-1]=0;
	pRegisterAccounts->szThirdParty[CountArray(pRegisterAccounts->szThirdParty)-1]=0;

	//验证验证码
	bool bIsOK=true;
	std::wstring key=pRegisterAccounts->szMachineID;
	wstring val;
	val=pRegisterAccounts->szValidateCode;

	if(bIsOK==false)
	{
		CMD_GP_LogonFailure LogonFailure;
		ZeroMemory(&LogonFailure,sizeof(LogonFailure));
		LogonFailure.lResultCode=7;
		TCHAR* szDescriptionString=TEXT("效验码效验验证失败，请重新输入！");
		lstrcpyn(LogonFailure.szDescribeString,szDescriptionString,17);
		WORD wStringSize=CountStringBuffer(LogonFailure.szDescribeString);
		WORD wSendSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString)+wStringSize;
		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_LOGON,SUB_GP_LOGON_FAILURE,&LogonFailure,wSendSize);
		return true;
	}
	//设置连接
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pRegisterAccounts->dwPlazaVersion;

	//效验版本
	if (CheckPlazaVersion(DEVICE_TYPE_PC,pRegisterAccounts->dwPlazaVersion,dwSocketID,((pRegisterAccounts->cbValidateFlags&LOW_VER_VALIDATE_FLAGS)!=0))==false)
	{
		return true;
	}

	//变量定义
	DBR_GP_RegisterThird RegisterAccounts;
	ZeroMemory(&RegisterAccounts,sizeof(RegisterAccounts));

	//附加信息
	RegisterAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//构造数据
	RegisterAccounts.wFaceID=pRegisterAccounts->wFaceID;
	RegisterAccounts.cbGender=pRegisterAccounts->cbGender;
	RegisterAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(RegisterAccounts.szAccounts,pRegisterAccounts->szAccounts,CountArray(RegisterAccounts.szAccounts));
	lstrcpyn(RegisterAccounts.szNickName,pRegisterAccounts->szNickName,CountArray(RegisterAccounts.szNickName));
	lstrcpyn(RegisterAccounts.szSpreader,pRegisterAccounts->szSpreader,CountArray(RegisterAccounts.szSpreader));
	lstrcpyn(RegisterAccounts.szMachineID,pRegisterAccounts->szMachineID,CountArray(RegisterAccounts.szMachineID));
	lstrcpyn(RegisterAccounts.szLogonPass,pRegisterAccounts->szLogonPass,CountArray(RegisterAccounts.szLogonPass));
	lstrcpyn(RegisterAccounts.szInsurePass,pRegisterAccounts->szInsurePass,CountArray(RegisterAccounts.szInsurePass));
	lstrcpyn(RegisterAccounts.szPassPortID,pRegisterAccounts->szPassPortID,CountArray(RegisterAccounts.szPassPortID));
	lstrcpyn(RegisterAccounts.szCompellation,pRegisterAccounts->szCompellation,CountArray(RegisterAccounts.szCompellation));
	lstrcpyn(RegisterAccounts.szThirdParty,pRegisterAccounts->szThirdParty,CountArray(RegisterAccounts.szThirdParty));

	//投递请求
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_REGISTER_THIRD,dwSocketID,&RegisterAccounts,sizeof(RegisterAccounts));

	return true;
}

//Web 登录
bool CAttemperEngineSink::OnTcpNetworkSumWebRegisterAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{

	//变量定义
	WORD wBindIndex = LOWORD(dwSocketID);
	tagBindParameter * pBindParameter = (m_pBindParameter + wBindIndex);
	//设置连接
	pBindParameter->cbClientKind = CLIENT_KIND_COMPUTER;
	
	LogonServer::PROTO_RegisterAccounts ProtoRegisterAccounts;
	ProtoRegisterAccounts.ParseFromArray(pData, wDataSize);


	//变量定义
	DBR_GP_Web_RegisterAccounts RegisterAccounts;
	ZeroMemory(&RegisterAccounts, sizeof(RegisterAccounts));

	//附加信息
	RegisterAccounts.pBindParameter = (m_pBindParameter + LOWORD(dwSocketID));

	//构造数据
	RegisterAccounts.cbGender = ProtoRegisterAccounts.cbgender();
	RegisterAccounts.dwClientAddr = (m_pBindParameter + LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(RegisterAccounts.szAccounts, Utils::ConvertA2W(ProtoRegisterAccounts.szaccounts()).c_str(), CountArray(RegisterAccounts.szAccounts));
	lstrcpyn(RegisterAccounts.szNickName, Utils::ConvertA2W(ProtoRegisterAccounts.sznickname()).c_str(), CountArray(RegisterAccounts.szNickName));
	lstrcpyn(RegisterAccounts.szLogonPass, Utils::ConvertA2W(ProtoRegisterAccounts.szlogonpass()).c_str(), CountArray(RegisterAccounts.szLogonPass));
	

	//投递请求
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_WEB_REGISTER_ACCOUNTS, dwSocketID, &RegisterAccounts, sizeof(RegisterAccounts));

	return true;
}

//I D 登录
bool CAttemperEngineSink::OnTCPNetworkSubMBLogonGameID(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT_ERR(wDataSize>=sizeof(CMD_MB_LogonGameID));
	if (wDataSize<sizeof(CMD_MB_LogonGameID)) return false;

	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//处理消息
	CMD_MB_LogonGameID * pLogonGameID=(CMD_MB_LogonGameID *)pData;
	pLogonGameID->szPassword[CountArray(pLogonGameID->szPassword)-1]=0;
	pLogonGameID->szMachineID[CountArray(pLogonGameID->szMachineID)-1]=0;
	pLogonGameID->szMobilePhone[CountArray(pLogonGameID->szMobilePhone)-1]=0;

	//设置连接
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->dwPlazaVersion=pLogonGameID->dwPlazaVersion;

	//效验版本
	//if (CheckMBPlazaVersion(pLogonGameID->cbDeviceType, pLogonGameID->dwPlazaVersion, pLogonGameID->wModuleID, dwSocketID) == false) return true;

	//变量定义
	DBR_MB_LogonGameID LogonGameID;
	ZeroMemory(&LogonGameID,sizeof(LogonGameID));

	//附加信息
	LogonGameID.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//构造数据
	LogonGameID.dwGameID=pLogonGameID->dwGameID;
	LogonGameID.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(LogonGameID.szPassword,pLogonGameID->szPassword,CountArray(LogonGameID.szPassword));
	lstrcpyn(LogonGameID.szMachineID,pLogonGameID->szMachineID,CountArray(LogonGameID.szMachineID));
	lstrcpyn(LogonGameID.szMobilePhone,pLogonGameID->szMobilePhone,CountArray(LogonGameID.szMobilePhone));

	//投递请求
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_LOGON_GAMEID,dwSocketID,&LogonGameID,sizeof(LogonGameID));

	return true;
}

//帐号登录
bool CAttemperEngineSink::OnTCPNetworkSubMBLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT_ERR(wDataSize>=sizeof(CMD_MB_LogonAccounts));
	if (wDataSize<sizeof(CMD_MB_LogonAccounts)) return false; 

	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//处理消息
	CMD_MB_LogonAccounts * pLogonAccounts=(CMD_MB_LogonAccounts *)pData;
	pLogonAccounts->szAccounts[CountArray(pLogonAccounts->szAccounts)-1]=0;
	pLogonAccounts->szPassword[CountArray(pLogonAccounts->szPassword)-1]=0;
	pLogonAccounts->szMachineID[CountArray(pLogonAccounts->szMachineID)-1]=0;
	pLogonAccounts->szMobilePhone[CountArray(pLogonAccounts->szMobilePhone)-1]=0;

	//设置连接
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->dwPlazaVersion=pLogonAccounts->dwPlazaVersion;

	//版本判断
	//if (CheckMBPlazaVersion(pLogonAccounts->cbDeviceType, pLogonAccounts->dwPlazaVersion, pLogonAccounts->wModuleID, dwSocketID) == false) return true;

	//变量定义
	DBR_MB_LogonAccounts LogonAccounts;
	ZeroMemory(&LogonAccounts,sizeof(LogonAccounts));

	//附加信息
	LogonAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//构造数据
	LogonAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(LogonAccounts.szAccounts,pLogonAccounts->szAccounts,CountArray(LogonAccounts.szAccounts));
	lstrcpyn(LogonAccounts.szPassword,pLogonAccounts->szPassword,CountArray(LogonAccounts.szPassword));
	lstrcpyn(LogonAccounts.szMachineID,pLogonAccounts->szMachineID,CountArray(LogonAccounts.szMachineID));
	lstrcpyn(LogonAccounts.szMobilePhone,pLogonAccounts->szMobilePhone,CountArray(LogonAccounts.szMobilePhone));

	//投递请求
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_LOGON_ACCOUNTS,dwSocketID,&LogonAccounts,sizeof(LogonAccounts));

	//I_TRACE(TEXT("OnTCPNetworkSubMBLogonAccounts %s"),pLogonAccounts->szAccounts);
	return true;
}

//帐号注册
bool CAttemperEngineSink::OnTCPNetworkSubMBRegisterAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT_ERR(wDataSize>=sizeof(CMD_MB_RegisterAccounts));
	if (wDataSize<sizeof(CMD_MB_RegisterAccounts)) return false;

	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);


	{
		TCHAR szFilePath[MAX_PATH] = L"";
		GetCurrentDirectory(sizeof(szFilePath), szFilePath);

		TCHAR szFullPath[MAX_PATH] = L"";
		TCHAR szDescribeString[128] = L"";
		DWORD dwIsOpenRegister = 1;
		swprintf(szFullPath, sizeof(szFullPath), TEXT("%s/Settings/policy.ini"), szFilePath);
		dwIsOpenRegister=GetPrivateProfileInt(TEXT("Parameter"),TEXT("IsOpenRegister"),1,(LPCTSTR)szFullPath);
		if ( dwIsOpenRegister == 0 )
		{
			GetPrivateProfileString(TEXT("Parameter"),TEXT("Desc"),TEXT("平台即将转向，请联系客服获取新平台地址"),szDescribeString,CountArray(szDescribeString),szFullPath);

			CMD_MB_LogonFailure LogonFailure;
			ZeroMemory(&LogonFailure,sizeof(LogonFailure));

			LogonFailure.lResultCode = 1;
			_tcscpy_s(LogonFailure.szDescribeString, szDescribeString);

			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_LOGON_BCode,SUB_MB_LOGON_FAILURE,&LogonFailure,sizeof(CMD_MB_LogonFailure));

			return true;
		}
	}

	//处理消息
	CMD_MB_RegisterAccounts * pRegisterAccounts=(CMD_MB_RegisterAccounts *)pData;
	pRegisterAccounts->szAccounts[CountArray(pRegisterAccounts->szAccounts)-1]=0;
	pRegisterAccounts->szNickName[CountArray(pRegisterAccounts->szNickName)-1]=0;
	pRegisterAccounts->szMachineID[CountArray(pRegisterAccounts->szMachineID)-1]=0;
	pRegisterAccounts->szLogonPass[CountArray(pRegisterAccounts->szLogonPass)-1]=0;
	pRegisterAccounts->szInsurePass[CountArray(pRegisterAccounts->szInsurePass)-1]=0;
	pRegisterAccounts->szMobilePhone[CountArray(pRegisterAccounts->szMobilePhone)-1]=0;

	//设置连接
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;

	//变量定义
	DBR_MB_RegisterAccounts RegisterAccounts;
	ZeroMemory(&RegisterAccounts,sizeof(RegisterAccounts));

	//附加信息
	RegisterAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//构造数据
	RegisterAccounts.wFaceID=pRegisterAccounts->wFaceID;
	RegisterAccounts.cbGender=pRegisterAccounts->cbGender;
	RegisterAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(RegisterAccounts.szAccounts,pRegisterAccounts->szAccounts,CountArray(RegisterAccounts.szAccounts));
	lstrcpyn(RegisterAccounts.szNickName,pRegisterAccounts->szNickName,CountArray(RegisterAccounts.szNickName));
	lstrcpyn(RegisterAccounts.szMachineID,pRegisterAccounts->szMachineID,CountArray(RegisterAccounts.szMachineID));
	lstrcpyn(RegisterAccounts.szLogonPass,pRegisterAccounts->szLogonPass,CountArray(RegisterAccounts.szLogonPass));
	lstrcpyn(RegisterAccounts.szInsurePass,pRegisterAccounts->szInsurePass,CountArray(RegisterAccounts.szInsurePass));
	lstrcpyn(RegisterAccounts.szMobilePhone,pRegisterAccounts->szMobilePhone,CountArray(RegisterAccounts.szMobilePhone));

	//投递请求
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_REGISTER_ACCOUNTS,dwSocketID,&RegisterAccounts,sizeof(RegisterAccounts));

	//I_TRACE(TEXT("OnTCPNetworkSubMBRegisterAccounts %s"),pRegisterAccounts->szAccounts);
	return true;
}

//游客登录
bool CAttemperEngineSink::OnTCPNetworkSubMBVisitorAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize>=sizeof(CMD_MB_LogonVisitor));
	if (wDataSize<sizeof(CMD_MB_LogonVisitor)) return false;

	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//处理消息
	CMD_MB_LogonVisitor * pRegisterAccounts=(CMD_MB_LogonVisitor *)pData;
	pRegisterAccounts->szMachineID[CountArray(pRegisterAccounts->szMachineID)-1]=0;
	pRegisterAccounts->szSpreader[CountArray(pRegisterAccounts->szSpreader)-1]=0;

	//设置连接
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->dwPlazaVersion=pRegisterAccounts->dwPlazaVersion;

	//变量定义
	DBR_MB_VisitorAccounts RegisterAccounts;
	ZeroMemory(&RegisterAccounts,sizeof(RegisterAccounts));

	//附加信息
	RegisterAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//构造数据
	RegisterAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(RegisterAccounts.szSpreader,pRegisterAccounts->szSpreader,CountArray(RegisterAccounts.szSpreader));
	lstrcpyn(RegisterAccounts.szMachineID,pRegisterAccounts->szMachineID,CountArray(RegisterAccounts.szMachineID));

	//投递请求
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_VISITOR_ACCOUNTS,dwSocketID,&RegisterAccounts,sizeof(RegisterAccounts));

	//I_TRACE(TEXT("OnTCPNetworkSubMBVisitorAccounts %s"),pRegisterAccounts->szMachineID);
	return true;
}

// 手机注册账号
bool CAttemperEngineSink::OnTCPNetworkSubMBRegisterMobile(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	// 效验参数
	ASSERT_ERR(wDataSize == sizeof(CMD_MB_RegisterMobile));
	if (wDataSize != sizeof(CMD_MB_RegisterMobile))
		return false;

	// 变量定义
	DWORD wBindIndex = LOWORD(dwSocketID);
	tagBindParameter* pBindParameter = (m_pBindParameter + wBindIndex);

	// 处理消息
	CMD_MB_RegisterMobile* pRegisterMobile = (CMD_MB_RegisterMobile*)pData;
	pRegisterMobile->szMachineID[CountArray(pRegisterMobile->szMachineID) - 1] = 0;
	pRegisterMobile->szLogonPass[CountArray(pRegisterMobile->szLogonPass) - 1] = 0;
	pRegisterMobile->szMobilePhone[CountArray(pRegisterMobile->szMobilePhone) - 1] = 0;
	pRegisterMobile->szVerificationCode[CountArray(pRegisterMobile->szVerificationCode) - 1] = 0;

	// 设置连接
	pBindParameter->cbClientKind = CLIENT_KIND_MOBILE;

	// 变量定义
	DBR_MB_RegisterMobile RegisterMobile = { 0 };

	// 附加信息
	RegisterMobile.pBindParameter = (m_pBindParameter + LOWORD(dwSocketID));

	// 构造数据
	RegisterMobile.dwClientAddr = (m_pBindParameter + LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(RegisterMobile.szMobile, pRegisterMobile->szMobilePhone, CountArray(RegisterMobile.szMobile));
	lstrcpyn(RegisterMobile.szMachineID, pRegisterMobile->szMachineID, CountArray(RegisterMobile.szMachineID));
	lstrcpyn(RegisterMobile.szLogonPass, pRegisterMobile->szLogonPass, CountArray(RegisterMobile.szLogonPass));

	//投递请求
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_REGISTER_MOBILE, dwSocketID, &RegisterMobile, sizeof(RegisterMobile));

	return true;
}

//登录成功
bool CAttemperEngineSink::OnDBPCLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT_ERR(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//变量定义
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];
	DBO_GP_LogonSuccess * pDBOLogonSuccess=(DBO_GP_LogonSuccess *)pData;
	CMD_GP_LogonSuccess * pCMDLogonSuccess=(CMD_GP_LogonSuccess *)cbDataBuffer;

	//发送定义
	WORD wHeadSize=sizeof(CMD_GP_LogonSuccess);
	CSendPacketHelper SendPacket(cbDataBuffer+wHeadSize,sizeof(cbDataBuffer)-wHeadSize);

	//设置变量
	ZeroMemory(pCMDLogonSuccess,sizeof(CMD_GP_LogonSuccess));

	// add 2015.12.14
	WORD wBindIndex=LOWORD(dwContextID);
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
	bool bMobileUser=(pBindParameter->cbClientKind==CLIENT_KIND_MOBILE);

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
	lstrcpyn(UserInfo.szNickName,pDBOLogonSuccess->szNickName,CountArray(UserInfo.szNickName));

	//用户资料
	UserInfo.cbGender=pDBOLogonSuccess->cbGender;
	UserInfo.cbMemberOrder=pDBOLogonSuccess->cbMemberOrder;
	UserInfo.cbMasterOrder=0;
	lstrcpyn(UserInfo.szGroupName,pDBOLogonSuccess->szGroupName,CountArray(UserInfo.szGroupName));
	lstrcpyn(UserInfo.szUnderWrite,pDBOLogonSuccess->szUnderWrite,CountArray(UserInfo.szUnderWrite));

	//状态设置
	UserInfo.cbUserStatus=US_FREE;
	UserInfo.wTableID=INVALID_TABLE;
	UserInfo.wChairID=INVALID_CHAIR;

	//积分信息
	UserInfo.lScore=pDBOLogonSuccess->lUserScore;
	UserInfo.lGrade=0;
	UserInfo.lInsure=pDBOLogonSuccess->lUserInsure;
	UserInfo.dwWinCount=0;
	UserInfo.dwLostCount=0;
	UserInfo.dwDrawCount=0;
	UserInfo.dwFleeCount=0;
	UserInfo.dwUserMedal=pDBOLogonSuccess->dwUserMedal;
	UserInfo.dwExperience=pDBOLogonSuccess->dwExperience;
	UserInfo.lLoveLiness=0;

	//登录信息
	UserInfoPlus.dwLogonTime=(DWORD)time(NULL);
	UserInfoPlus.dwInoutIndex=0;

	//用户权限
	UserInfoPlus.dwUserRight=0;
	UserInfoPlus.dwMasterRight=0;

	//辅助变量
	UserInfoPlus.bMobileUser=bMobileUser;
	UserInfoPlus.bAndroidUser=false;
	UserInfoPlus.lRestrictScore=0;
	lstrcpyn(UserInfoPlus.szPassword,pDBOLogonSuccess->szPassword,CountArray(UserInfoPlus.szPassword));

	//连接信息
	UserInfoPlus.wBindIndex=LOWORD(dwContextID);
	UserInfoPlus.dwClientAddr=pBindParameter->dwClientAddr;
	ZeroMemory(UserInfoPlus.szMachineID,CountArray(UserInfoPlus.szMachineID));

	IServerUserItem * pIServerUserItem=m_ServerUserManager.SearchUserItem(pDBOLogonSuccess->dwUserID);
	if ( pIServerUserItem != NULL ) m_ServerUserManager.DeleteUserItem(pIServerUserItem);
	//激活用户
	m_ServerUserManager.InsertUserItem(&pIServerUserItem,UserInfo,UserInfoPlus);
	//设置用户
	pBindParameter->pIServerUserItem=pIServerUserItem;
	// end 2015.12.14

	//构造数据
	pCMDLogonSuccess->wFaceID=pDBOLogonSuccess->wFaceID;
	pCMDLogonSuccess->cbGender=pDBOLogonSuccess->cbGender;
	pCMDLogonSuccess->dwGameID=pDBOLogonSuccess->dwGameID;
	pCMDLogonSuccess->dwUserID=pDBOLogonSuccess->dwUserID;
	pCMDLogonSuccess->dwCustomID=pDBOLogonSuccess->dwCustomID;
	pCMDLogonSuccess->dwUserMedal=pDBOLogonSuccess->dwUserMedal;
	pCMDLogonSuccess->dwExperience=pDBOLogonSuccess->dwExperience;
	pCMDLogonSuccess->dwLoveLiness=pDBOLogonSuccess->dwLoveLiness;
	pCMDLogonSuccess->cbMoorMachine=pDBOLogonSuccess->cbMoorMachine;
	lstrcpyn(pCMDLogonSuccess->szAccounts,pDBOLogonSuccess->szAccounts,CountArray(pCMDLogonSuccess->szAccounts));
	lstrcpyn(pCMDLogonSuccess->szNickName,pDBOLogonSuccess->szNickName,CountArray(pCMDLogonSuccess->szNickName));

	//拓展信息
	lstrcpyn(pCMDLogonSuccess->szCompellation,pDBOLogonSuccess->szCompellation,CountArray(pCMDLogonSuccess->szCompellation));
	lstrcpyn(pCMDLogonSuccess->szQQ,pDBOLogonSuccess->szQQ,CountArray(pCMDLogonSuccess->szQQ));
	lstrcpyn(pCMDLogonSuccess->szEMail,pDBOLogonSuccess->szEMail,CountArray(pCMDLogonSuccess->szEMail));
	lstrcpyn(pCMDLogonSuccess->szSeatPhone,pDBOLogonSuccess->szSeatPhone,CountArray(pCMDLogonSuccess->szSeatPhone));
	lstrcpyn(pCMDLogonSuccess->szMobilePhone,pDBOLogonSuccess->szMobilePhone,CountArray(pCMDLogonSuccess->szMobilePhone));
	lstrcpyn(pCMDLogonSuccess->szDwellingPlace,pDBOLogonSuccess->szDwellingPlace,CountArray(pCMDLogonSuccess->szDwellingPlace));
	lstrcpyn(pCMDLogonSuccess->szUserNote,pDBOLogonSuccess->szUserNote,CountArray(pCMDLogonSuccess->szUserNote));

	//用户成绩
	pCMDLogonSuccess->lUserScore=pDBOLogonSuccess->lUserScore;
	pCMDLogonSuccess->lUserInsure=pDBOLogonSuccess->lUserInsure;

	//配置信息
	pCMDLogonSuccess->cbShowServerStatus=m_bShowServerStatus?1:0;

	//会员信息
	if (pDBOLogonSuccess->cbMemberOrder!=0)
	{
		DTP_GP_MemberInfo MemberInfo;
		ZeroMemory(&MemberInfo,sizeof(MemberInfo));
		MemberInfo.cbMemberOrder=pDBOLogonSuccess->cbMemberOrder;
		MemberInfo.MemberOverDate=pDBOLogonSuccess->MemberOverDate;
		SendPacket.AddPacket(&MemberInfo,sizeof(MemberInfo),DTP_GP_MEMBER_INFO);
	}

	//个性签名
	if (pDBOLogonSuccess->szUnderWrite[0]!=0)
	{
		SendPacket.AddPacket(pDBOLogonSuccess->szUnderWrite,CountStringBuffer(pDBOLogonSuccess->szUnderWrite),DTP_GP_UNDER_WRITE);
	}

	if ( pDBOLogonSuccess->szLastClientAddr[0]!=0 )
	{
		DTP_GP_AddressInfo AddressInfo;
		ZeroMemory(&AddressInfo,sizeof(AddressInfo));
		AddressInfo.LastLogonDate = pDBOLogonSuccess->LastLogonDate;
		lstrcpyn(AddressInfo.szMsgInfo,pDBOLogonSuccess->szDescribeString,CountArray(pDBOLogonSuccess->szDescribeString));
		
		SendPacket.AddPacket(&AddressInfo,sizeof(AddressInfo),DTP_GP_ADDRESS_INFO);
	}

	//网络设置

	m_pITCPNetworkEngine->AllowBatchSend(dwContextID,true,BG_COMPUTER);

	//登录成功
	WORD wSendSize=SendPacket.GetDataSize()+sizeof(CMD_GP_LogonSuccess);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_LOGON_SUCCESS,cbDataBuffer,wSendSize);

	//发送列表
	if (m_pInitParameter->m_cbDelayList==TRUE)
	{
		//发送列表
		SendGameTypeInfo(dwContextID);
		SendGameKindInfo(dwContextID);
		//SendGamePageInfo(dwContextID,INVALID_WORD);
		//SendGameNodeInfo(dwContextID,INVALID_WORD);
		SendGameServerInfo(dwContextID,INVALID_WORD);
		m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_SERVER_LIST,SUB_GP_LIST_FINISH);
	}
	else
	{
		SendGameTypeInfo(dwContextID);
		SendGameKindInfo(dwContextID);
		SendGamePageInfo(dwContextID,0);
		m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_SERVER_LIST,SUB_GP_LIST_FINISH);
	}

	////登录完成
	CMD_GP_LogonFinish LogonFinish;
	ZeroMemory(&LogonFinish,sizeof(LogonFinish));
	LogonFinish.wIntermitTime=m_pInitParameter->m_wIntermitTime;
	LogonFinish.wOnLineCountTime=m_pInitParameter->m_wOnLineCountTime;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_LOGON_FINISH,&LogonFinish,sizeof(LogonFinish));

	//I_TRACE(TEXT("OnDBPCLogonSuccess %s"),pDBOLogonSuccess->szAccounts);
	return true;
}

//登录成功
bool CAttemperEngineSink::OnDBWebLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT_ERR(LOWORD(dwContextID) < m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter + LOWORD(dwContextID))->dwSocketID != dwContextID) return true;

	//变量定义
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];
	DBO_GP_LogonSuccess * pDBOLogonSuccess = (DBO_GP_LogonSuccess *)pData;
	CMD_GP_LogonSuccess * pCMDLogonSuccess = (CMD_GP_LogonSuccess *)cbDataBuffer;

	//发送定义
	WORD wHeadSize = sizeof(CMD_GP_LogonSuccess);
	CSendPacketHelper SendPacket(cbDataBuffer + wHeadSize, sizeof(cbDataBuffer) - wHeadSize);

	//设置变量
	ZeroMemory(pCMDLogonSuccess, sizeof(CMD_GP_LogonSuccess));

	// add 2015.12.14
	WORD wBindIndex = LOWORD(dwContextID);
	tagBindParameter * pBindParameter = GetBindParameter(wBindIndex);
	bool bMobileUser = (pBindParameter->cbClientKind == CLIENT_KIND_MOBILE);

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
	UserInfo.cbMasterOrder = 0;
	lstrcpyn(UserInfo.szGroupName, pDBOLogonSuccess->szGroupName, CountArray(UserInfo.szGroupName));
	lstrcpyn(UserInfo.szUnderWrite, pDBOLogonSuccess->szUnderWrite, CountArray(UserInfo.szUnderWrite));

	//状态设置
	UserInfo.cbUserStatus = US_FREE;
	UserInfo.wTableID = INVALID_TABLE;
	UserInfo.wChairID = INVALID_CHAIR;

	//积分信息
	UserInfo.lScore = pDBOLogonSuccess->lUserScore;
	UserInfo.lGrade = 0;
	UserInfo.lInsure = pDBOLogonSuccess->lUserInsure;
	UserInfo.dwWinCount = 0;
	UserInfo.dwLostCount = 0;
	UserInfo.dwDrawCount = 0;
	UserInfo.dwFleeCount = 0;
	UserInfo.dwUserMedal = pDBOLogonSuccess->dwUserMedal;
	UserInfo.dwExperience = pDBOLogonSuccess->dwExperience;
	UserInfo.lLoveLiness = 0;

	//登录信息
	UserInfoPlus.dwLogonTime = (DWORD)time(NULL);
	UserInfoPlus.dwInoutIndex = 0;

	//用户权限
	UserInfoPlus.dwUserRight = 0;
	UserInfoPlus.dwMasterRight = 0;

	//辅助变量
	UserInfoPlus.bMobileUser = bMobileUser;
	UserInfoPlus.bAndroidUser = false;
	UserInfoPlus.lRestrictScore = 0;
	lstrcpyn(UserInfoPlus.szPassword, pDBOLogonSuccess->szPassword, CountArray(UserInfoPlus.szPassword));

	//连接信息
	UserInfoPlus.wBindIndex = LOWORD(dwContextID);
	UserInfoPlus.dwClientAddr = pBindParameter->dwClientAddr;
	ZeroMemory(UserInfoPlus.szMachineID, CountArray(UserInfoPlus.szMachineID));

	IServerUserItem * pIServerUserItem = m_ServerUserManager.SearchUserItem(pDBOLogonSuccess->dwUserID);
	if (pIServerUserItem != NULL) m_ServerUserManager.DeleteUserItem(pIServerUserItem);
	//激活用户
	m_ServerUserManager.InsertUserItem(&pIServerUserItem, UserInfo, UserInfoPlus);
	//设置用户
	pBindParameter->pIServerUserItem = pIServerUserItem;
	// end 2015.12.14

	//构造数据
	pCMDLogonSuccess->wFaceID = pDBOLogonSuccess->wFaceID;
	pCMDLogonSuccess->cbGender = pDBOLogonSuccess->cbGender;
	pCMDLogonSuccess->dwGameID = pDBOLogonSuccess->dwGameID;
	pCMDLogonSuccess->dwUserID = pDBOLogonSuccess->dwUserID;
	pCMDLogonSuccess->dwCustomID = pDBOLogonSuccess->dwCustomID;
	pCMDLogonSuccess->dwUserMedal = pDBOLogonSuccess->dwUserMedal;
	pCMDLogonSuccess->dwExperience = pDBOLogonSuccess->dwExperience;
	pCMDLogonSuccess->dwLoveLiness = pDBOLogonSuccess->dwLoveLiness;
	pCMDLogonSuccess->cbMoorMachine = pDBOLogonSuccess->cbMoorMachine;
	lstrcpyn(pCMDLogonSuccess->szAccounts, pDBOLogonSuccess->szAccounts, CountArray(pCMDLogonSuccess->szAccounts));
	lstrcpyn(pCMDLogonSuccess->szNickName, pDBOLogonSuccess->szNickName, CountArray(pCMDLogonSuccess->szNickName));

	//拓展信息
	lstrcpyn(pCMDLogonSuccess->szCompellation, pDBOLogonSuccess->szCompellation, CountArray(pCMDLogonSuccess->szCompellation));
	lstrcpyn(pCMDLogonSuccess->szQQ, pDBOLogonSuccess->szQQ, CountArray(pCMDLogonSuccess->szQQ));
	lstrcpyn(pCMDLogonSuccess->szEMail, pDBOLogonSuccess->szEMail, CountArray(pCMDLogonSuccess->szEMail));
	lstrcpyn(pCMDLogonSuccess->szSeatPhone, pDBOLogonSuccess->szSeatPhone, CountArray(pCMDLogonSuccess->szSeatPhone));
	lstrcpyn(pCMDLogonSuccess->szMobilePhone, pDBOLogonSuccess->szMobilePhone, CountArray(pCMDLogonSuccess->szMobilePhone));
	lstrcpyn(pCMDLogonSuccess->szDwellingPlace, pDBOLogonSuccess->szDwellingPlace, CountArray(pCMDLogonSuccess->szDwellingPlace));
	lstrcpyn(pCMDLogonSuccess->szUserNote, pDBOLogonSuccess->szUserNote, CountArray(pCMDLogonSuccess->szUserNote));

	//用户成绩
	pCMDLogonSuccess->lUserScore = pDBOLogonSuccess->lUserScore;
	pCMDLogonSuccess->lUserInsure = pDBOLogonSuccess->lUserInsure;

	//配置信息
	pCMDLogonSuccess->cbShowServerStatus = m_bShowServerStatus ? 1 : 0;

	//会员信息
	if (pDBOLogonSuccess->cbMemberOrder != 0)
	{
		DTP_GP_MemberInfo MemberInfo;
		ZeroMemory(&MemberInfo, sizeof(MemberInfo));
		MemberInfo.cbMemberOrder = pDBOLogonSuccess->cbMemberOrder;
		MemberInfo.MemberOverDate = pDBOLogonSuccess->MemberOverDate;
		SendPacket.AddPacket(&MemberInfo, sizeof(MemberInfo), DTP_GP_MEMBER_INFO);
	}

	//个性签名
	if (pDBOLogonSuccess->szUnderWrite[0] != 0)
	{
		SendPacket.AddPacket(pDBOLogonSuccess->szUnderWrite, CountStringBuffer(pDBOLogonSuccess->szUnderWrite), DTP_GP_UNDER_WRITE);
	}

	if (pDBOLogonSuccess->szLastClientAddr[0] != 0)
	{
		DTP_GP_AddressInfo AddressInfo;
		ZeroMemory(&AddressInfo, sizeof(AddressInfo));
		AddressInfo.LastLogonDate = pDBOLogonSuccess->LastLogonDate;
		lstrcpyn(AddressInfo.szMsgInfo, pDBOLogonSuccess->szDescribeString, CountArray(pDBOLogonSuccess->szDescribeString));

		SendPacket.AddPacket(&AddressInfo, sizeof(AddressInfo), DTP_GP_ADDRESS_INFO);
	}

	//网络设置

	m_pITCPNetworkEngine->AllowBatchSend(dwContextID, true, BG_COMPUTER);

	//登录成功
	WORD wSendSize = SendPacket.GetDataSize() + sizeof(CMD_GP_LogonSuccess);
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_LOGON, SUB_GP_LOGON_SUCCESS, cbDataBuffer, wSendSize);


	// 游戏房间暂时不发送
	//发送列表
// 	if (m_pInitParameter->m_cbDelayList == TRUE)
// 	{
// 		//发送列表
// 		SendGameTypeInfo(dwContextID);
// 		SendGameKindInfo(dwContextID);
// 		//SendGamePageInfo(dwContextID,INVALID_WORD);
// 		//SendGameNodeInfo(dwContextID,INVALID_WORD);
// 		SendGameServerInfo(dwContextID, INVALID_WORD);
// 		m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_SERVER_LIST, SUB_GP_LIST_FINISH);
// 	}
// 	else
// 	{
// 		SendGameTypeInfo(dwContextID);
// 		SendGameKindInfo(dwContextID);
// 		SendGamePageInfo(dwContextID, 0);
// 		m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_SERVER_LIST, SUB_GP_LIST_FINISH);
// 	}

	//登录成功
	LogonServer::PROTO_LogonFinish ProtoLogonFinish;
	ProtoLogonFinish.set_cblogonfinish(1);
	std::string serializeStr;
	ProtoLogonFinish.SerializeToString(&serializeStr);

	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_LOGON, SUB_GP_WEB_LOGON_FINISH, (void*)serializeStr.c_str(), (WORD)serializeStr.size());

	////登录完成
	//CMD_GP_LogonFinish LogonFinish;
	//ZeroMemory(&LogonFinish, sizeof(LogonFinish));
	//LogonFinish.wIntermitTime = m_pInitParameter->m_wIntermitTime;
	//LogonFinish.wOnLineCountTime = m_pInitParameter->m_wOnLineCountTime;
	//m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_LOGON, SUB_GP_LOGON_FINISH, &LogonFinish, sizeof(LogonFinish));

	//I_TRACE(TEXT("OnDBPCLogonSuccess %s"),pDBOLogonSuccess->szAccounts);
	return true;
}

//登录失败
bool CAttemperEngineSink::OnDBPCLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT_ERR(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//变量定义
	CMD_GP_LogonFailure LogonFailure;
	ZeroMemory(&LogonFailure,sizeof(LogonFailure));
	DBO_GP_OperateResult * pLogonFailure = (DBO_GP_OperateResult *)pData;

	//构造数据
	LogonFailure.lResultCode = pLogonFailure->dwResultCode;
	lstrcpyn(LogonFailure.szDescribeString,pLogonFailure->szDescribeString,CountArray(LogonFailure.szDescribeString));

	//发送数据
	WORD wStringSize=CountStringBuffer(LogonFailure.szDescribeString);
	WORD wSendSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString)+wStringSize;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_LOGON_FAILURE,&LogonFailure,wSendSize);

	//关闭连接
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}
// 查询银行
bool CAttemperEngineSink::OnDBPCUserInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT_ERR(LOWORD(dwContextID) < m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter + LOWORD(dwContextID))->dwSocketID != dwContextID) return true;

	//变量定义
	DBO_GP_UserInsureInfo * pUserInsureInfo = (DBO_GP_UserInsureInfo *)pData;

	
	LogonServer::PROTO_UserInsureSuccess ProtoUserInsureSuccess;
	ProtoUserInsureSuccess.set_dwuserid(pUserInsureInfo->dwUserID);
	ProtoUserInsureSuccess.set_luserscore(pUserInsureInfo->lUserScore);
	ProtoUserInsureSuccess.set_luserinsure(pUserInsureInfo->lUserInsure);
	ProtoUserInsureSuccess.set_szdescribestring(Utils::ConvertFromUtf16ToUtf8(TEXT("123456")));

	std::string serializeStr;
	ProtoUserInsureSuccess.SerializeToString(&serializeStr);

	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GP_USER_INSURE_SUCCESS, (void*)serializeStr.c_str(), (WORD)serializeStr.size());

	return true;
}

//银行成功
bool CAttemperEngineSink::OnDBPCUserInsureSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT_ERR(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//变量定义
	DBO_GP_UserInsureSuccess * pUserInsureSuccess=(DBO_GP_UserInsureSuccess *)pData;

	//变量定义
	//CMD_GP_UserInsureSuccess UserInsureSuccess;
	//ZeroMemory(&UserInsureSuccess,sizeof(UserInsureSuccess));

	//设置变量
	//UserInsureSuccess.dwUserID=pUserInsureSuccess->dwUserID;
	//UserInsureSuccess.lUserScore=pUserInsureSuccess->lSourceScore+pUserInsureSuccess->lVariationScore;
	//UserInsureSuccess.lUserInsure=pUserInsureSuccess->lSourceInsure+pUserInsureSuccess->lVariationInsure;
	//lstrcpyn(UserInsureSuccess.szDescribeString,pUserInsureSuccess->szDescribeString,CountArray(UserInsureSuccess.szDescribeString));

	//发送消息
	//WORD wDescribe=CountStringBuffer(UserInsureSuccess.szDescribeString);
	//WORD wHeadSize=sizeof(UserInsureSuccess)-sizeof(UserInsureSuccess.szDescribeString);
	//m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INSURE_SUCCESS,&UserInsureSuccess,wHeadSize+wDescribe);

	LogonServer::PROTO_UserInsureSuccess ProtoUserInsureSuccess;
	ProtoUserInsureSuccess.set_dwuserid(pUserInsureSuccess->dwUserID);
	ProtoUserInsureSuccess.set_luserscore(pUserInsureSuccess->lSourceScore + pUserInsureSuccess->lVariationScore);
	ProtoUserInsureSuccess.set_luserinsure(pUserInsureSuccess->lSourceInsure + pUserInsureSuccess->lVariationInsure);
	ProtoUserInsureSuccess.set_szdescribestring(Utils::ConvertFromUtf16ToUtf8(pUserInsureSuccess->szDescribeString));

	std::string serializeStr;
	ProtoUserInsureSuccess.SerializeToString(&serializeStr);

	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GP_USER_INSURE_SUCCESS, (void*)serializeStr.c_str(), (WORD)serializeStr.size());

	return true;
}

//操作失败
bool CAttemperEngineSink::OnDBPCUserInsureFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT_ERR(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//变量定义
	//CMD_GP_UserInsureFailure UserInsureFailure;
	//ZeroMemory(&UserInsureFailure,sizeof(UserInsureFailure));

	//变量定义
	DBO_GP_OperateResult * pUserInsureFailure = (DBO_GP_OperateResult *)pData;

	//构造数据
	//UserInsureFailure.lResultCode = pUserInsureFailure->dwResultCode;
	//lstrcpyn(UserInsureFailure.szDescribeString,pUserInsureFailure->szDescribeString,CountArray(UserInsureFailure.szDescribeString));

	//发送数据
	//WORD wDescribe=CountStringBuffer(UserInsureFailure.szDescribeString);
	//WORD wHeadSize=sizeof(UserInsureFailure)-sizeof(UserInsureFailure.szDescribeString);
	//m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INSURE_FAILURE,&UserInsureFailure,wHeadSize+wDescribe);


	LogonServer::PROTO_UserInsureFailure ProtoUserInsureFailure;
	ProtoUserInsureFailure.set_lresultcode(pUserInsureFailure->dwResultCode);
	ProtoUserInsureFailure.set_szdescribestring(Utils::ConvertFromUtf16ToUtf8(pUserInsureFailure->szDescribeString));

	std::string serializeStr;
	ProtoUserInsureFailure.SerializeToString(&serializeStr);

	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GP_USER_INSURE_FAILURE, (void*)serializeStr.c_str(), (WORD)serializeStr.size());


	return true;
}

// 操作结果
bool CAttemperEngineSink::OnDBPCOperateResult(DWORD dwContextID, VOID* pData, WORD wDataSize)
{
	// 判断在线
	ASSERT_ERR(LOWORD(dwContextID) < m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter + LOWORD(dwContextID))->dwSocketID != dwContextID) return true;

	CMD_GP_OperateResult OperateResult = { 0 };

	// 变量定义
	DBO_GP_OperateResult * pOperateResult = (DBO_GP_OperateResult *)pData;
	OperateResult.cbStringLength = (wDataSize - sizeof(WORD) - sizeof(DWORD)) / 2;
	OperateResult.wSubCommandID = pOperateResult->wSubCommandID;
	lstrcpyn(OperateResult.szDescribe, pOperateResult->szDescribeString, sizeof(OperateResult.szDescribe));

	int wSendSize = 1 + 2  + CountStringBuffer(OperateResult.szDescribe);

	LogonServer::PROTO_ModifyResult ProtoModifyResult;
	ProtoModifyResult.set_cbresultcode(pOperateResult->dwResultCode);
	ProtoModifyResult.set_wsubcommandid(OperateResult.wSubCommandID);
	ProtoModifyResult.set_szdescribe(Utils::ConvertFromUtf16ToUtf8(OperateResult.szDescribe));
	
	std::string serializeStr;
	ProtoModifyResult.SerializeToString(&serializeStr);
	// 发送数据
	//if (pOperateResult->dwResultCode == 0)
		m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GP_OPERATE_SUCCESS, (void*)serializeStr.c_str(), (WORD)serializeStr.size());
	//else
	//	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GP_OPERATE_FAILURE, (void*)serializeStr.c_str(), (WORD)serializeStr.size());

	return true;
}

//登录成功
bool CAttemperEngineSink::OnDBMBLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	// 判断在线
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID)
		return true;


	DBO_MB_LogonSuccess* pDBOLogonSuccess = (DBO_MB_LogonSuccess*)pData;
	{


		// 登录成功数据
		auto pDBOLogonSuccess = (DBO_MB_LogonSuccess*)pData;
		auto wBindIndex = LOWORD(dwContextID);
		auto pBindParameter = GetBindParameter(wBindIndex);
		auto bMobileUser = (pBindParameter->cbClientKind == CLIENT_KIND_MOBILE);

		// 数据填充
		tagUserInfo UserInfo = { 0 };
		tagUserInfoPlus UserInfoPlus = { 0 };

		//属性资料
		UserInfo.wFaceID = pDBOLogonSuccess->wFaceID;
		UserInfo.dwUserID = pDBOLogonSuccess->dwUserID;
		UserInfo.dwGameID = pDBOLogonSuccess->dwGameID;
		lstrcpyn(UserInfo.szNickName, pDBOLogonSuccess->szNickName, CountArray(UserInfo.szNickName));

		//用户资料
		UserInfo.cbGender = pDBOLogonSuccess->cbGender;
		UserInfo.cbMemberOrder = pDBOLogonSuccess->cbMemberOrder;
		UserInfo.cbMasterOrder = 0;
		lstrcpyn(UserInfo.szUnderWrite, pDBOLogonSuccess->szUnderWrite, CountArray(UserInfo.szUnderWrite));

		//状态设置
		UserInfo.cbUserStatus = US_FREE;
		UserInfo.wTableID = INVALID_TABLE;
		UserInfo.wChairID = INVALID_CHAIR;

		//积分信息
		UserInfo.lScore = pDBOLogonSuccess->lUserScore;
		UserInfo.lGrade = 0;
		UserInfo.lInsure = pDBOLogonSuccess->lUserInsure;
		UserInfo.dwWinCount = 0;
		UserInfo.dwLostCount = 0;
		UserInfo.dwDrawCount = 0;
		UserInfo.dwFleeCount = 0;
		UserInfo.dwUserMedal = pDBOLogonSuccess->dwUserMedal;
		UserInfo.dwExperience = pDBOLogonSuccess->dwExperience;
		UserInfo.lLoveLiness = pDBOLogonSuccess->dwLoveLiness;

		UserInfo.wAvatarID = pDBOLogonSuccess->wAvatarID;

		//登录信息
		UserInfoPlus.dwLogonTime = (DWORD)time(NULL);
		UserInfoPlus.dwInoutIndex = pDBOLogonSuccess->dwInoutIndex;

		//用户权限
		UserInfoPlus.dwUserRight = 0;
		UserInfoPlus.dwMasterRight = 0;

		//辅助变量
		UserInfoPlus.bMobileUser = bMobileUser;
		UserInfoPlus.bAndroidUser = false;
		UserInfoPlus.lRestrictScore = 0;

		//连接信息
		UserInfoPlus.wBindIndex = LOWORD(dwContextID);
		UserInfoPlus.dwClientAddr = pBindParameter->dwClientAddr;
		ZeroMemory(UserInfoPlus.szMachineID, CountArray(UserInfoPlus.szMachineID));

		// 切换用户
		auto pIServerUserItem = m_ServerUserManager.SearchUserItem(pDBOLogonSuccess->dwUserID);
		if (pIServerUserItem)
		{
			WORD wSourceIndex = pIServerUserItem->GetBindIndex();
			tagBindParameter * pSourceParameter = GetBindParameter(wSourceIndex);
			if ( pSourceParameter->dwSocketID != dwContextID ) 
			SwitchUserItemConnect(pIServerUserItem,wBindIndex);
		}

		//激活用户
		m_ServerUserManager.InsertUserItem(&pIServerUserItem, UserInfo, UserInfoPlus);

		//设置用户
		pBindParameter->pIServerUserItem = pIServerUserItem;

		m_pITCPNetworkEngine->AllowBatchSend(dwContextID,true,BG_MOBILE);
	}

	//发送登录成功数据
	OnDBEventDeal(dwContextID, pData, wDataSize, MDM_MB_LOGON, SUB_MB_LOGON_SUCCESS);

	//发送房间
	WORD wIndex = LOWORD(dwContextID);

	//发送游戏信息
	SendMobileKindInfo(dwContextID, (m_pBindParameter + wIndex)->wModuleID);

	//发送房间完成
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_MB_LOGON, SUB_MB_LOGON_FINISH);

	//I_TRACE(TEXT("OnDBPCLogonSuccess %s"),pDBOLogonSuccess->szAccounts);
	return true;
}

//登录失败
bool CAttemperEngineSink::OnDBMBLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT_ERR(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//变量定义
	CMD_MB_LogonFailure LogonFailure;
	ZeroMemory(&LogonFailure,sizeof(LogonFailure));
	DBO_GP_OperateResult * pLogonFailure = (DBO_GP_OperateResult *)pData;

	//构造数据
	LogonFailure.lResultCode = pLogonFailure->dwResultCode;
	_tcscpy_s(LogonFailure.szDescribeString, pLogonFailure->szDescribeString);

	//发送数据
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_MB_LOGON,SUB_MB_LOGON_FAILURE,&LogonFailure,sizeof(CMD_MB_LogonFailure));

	//关闭连接
	//m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//第三方注册
bool CAttemperEngineSink::OnDBThirdPartyRegister(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT_ERR(wDataSize%sizeof(CMD_GP_RegisterThird)==0);
	if (wDataSize%sizeof(CMD_GP_RegisterThird)!=0) return false;

	CMD_GP_RegisterThird *thirdPartyRegister = (CMD_GP_RegisterThird*)pData;
	OnTCPNetworkSubPCRegisterAccountsThird(thirdPartyRegister, wDataSize, dwContextID);
	return true;
}

//第三方直接登录
bool CAttemperEngineSink::OnDBThirdPartySuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT_ERR(wDataSize%sizeof(CMD_GP_LogonAccounts)==0);
	if (wDataSize%sizeof(CMD_GP_LogonAccounts)!=0) return false;

	CMD_GP_LogonAccounts * logonAccounts = (CMD_GP_LogonAccounts *)pData;
	OnTCPNetworkSubPCLogonAccounts(logonAccounts, wDataSize, dwContextID);
	return true;
}

//游戏种类
bool CAttemperEngineSink::OnDBPCGameTypeItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT_ERR(wDataSize%sizeof(DBO_GP_GameType)==0);
	if (wDataSize%sizeof(DBO_GP_GameType)!=0) return false;

	//变量定义
	WORD wItemCount=wDataSize/sizeof(DBO_GP_GameType);
	DBO_GP_GameType * pGameType=(DBO_GP_GameType *)pData;

	//更新数据
	for (WORD i=0;i<wItemCount;i++)
	{
		//变量定义
		tagGameType GameType;
		ZeroMemory(&GameType,sizeof(GameType));

		//构造数据
		GameType.wTypeID=(pGameType+i)->wTypeID;
		GameType.wJoinID=(pGameType+i)->wJoinID;
		GameType.wSortID=(pGameType+i)->wSortID;
		lstrcpyn(GameType.szTypeName,(pGameType+i)->szTypeName,CountArray(GameType.szTypeName));

		//插入列表
		m_ServerListManager.InsertGameType(&GameType);
	}

	return true;

}

//游戏类型
bool CAttemperEngineSink::OnDBPCGameKindItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	auto wItemCount = wDataSize / sizeof(DBO_GP_GameKind);
	auto pGameKind = (DBO_GP_GameKind*)pData;

	//更新数据
	for (auto i = 0; i < wItemCount; i++, pGameKind++)
	{
		//变量定义
		tagGameKind GameKind = { 0 };

		//构造数据
		GameKind.wTypeID = pGameKind->wTypeID;
		GameKind.wJoinID = pGameKind->wJoinID;
		GameKind.wSortID = pGameKind->wSortID;
		GameKind.wKindID = pGameKind->wKindID;
		GameKind.wGameID = pGameKind->wGameID;
		GameKind.dwOnLineCount = m_ServerListManager.CollectOnlineInfo(pGameKind->wKindID);
		lstrcpyn(GameKind.szKindName, pGameKind->szKindName, CountArray(GameKind.szKindName));
		lstrcpyn(GameKind.szProcessName, pGameKind->szProcessName, CountArray(GameKind.szProcessName));
		lstrcpyn(GameKind.szDownLoadUrl, pGameKind->szDownLoadUrl, CountArray(GameKind.szDownLoadUrl));

		//插入列表
		m_ServerListManager.InsertGameKind(&GameKind);
	}

	return true;
}

//游戏节点
bool CAttemperEngineSink::OnDBPCGameNodeItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	auto wItemCount = wDataSize / sizeof(DBO_GP_GameNode);
	auto pGameNode = (DBO_GP_GameNode*)pData;

	//更新数据
	for (auto i = 0; i < wItemCount; i++, pGameNode++)
	{
		//变量定义
		tagGameNode GameNode = { 0 };

		//构造数据
		GameNode.wKindID = pGameNode->wKindID;
		GameNode.wJoinID = pGameNode->wJoinID;
		GameNode.wSortID = pGameNode->wSortID;
		GameNode.wNodeID = pGameNode->wNodeID;
		lstrcpyn(GameNode.szNodeName, pGameNode->szNodeName, CountArray(GameNode.szNodeName));

		//插入列表
		m_ServerListManager.InsertGameNode(&GameNode);
	}

	return true;
}

//游戏定制
bool CAttemperEngineSink::OnDBPCGamePageItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	auto wItemCount = wDataSize / sizeof(DBO_GP_GamePage);
	auto pGamePage = (DBO_GP_GamePage*)pData;

	//更新数据
	for (auto i = 0; i < wItemCount; i++)
	{
		//变量定义
		tagGamePage GamePage = { 0 };

		//构造数据
		GamePage.wKindID = pGamePage->wKindID;
		GamePage.wNodeID = pGamePage->wNodeID;
		GamePage.wSortID = pGamePage->wSortID;
		GamePage.wPageID = pGamePage->wPageID;
		GamePage.wOperateType = pGamePage->wOperateType;
		lstrcpyn(GamePage.szDisplayName, pGamePage->szDisplayName, CountArray(GamePage.szDisplayName));

		//插入列表
		m_ServerListManager.InsertGamePage(&GamePage);
	}

	return true;
}

//游戏列表
bool CAttemperEngineSink::OnDBPCGameListResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT_ERR(wDataSize==sizeof(DBO_GP_GameListResult));
	if (wDataSize!=sizeof(DBO_GP_GameListResult)) return false;

	//变量定义
	DBO_GP_GameListResult * pGameListResult=(DBO_GP_GameListResult *)pData;

	//消息处理
	if (pGameListResult->cbSuccess==TRUE)
	{
		//清理列表
		m_ServerListManager.CleanKernelItem();

		//事件通知
		CP_ControlResult ControlResult;
		ControlResult.cbSuccess=ER_SUCCESS;
		SendUIControlPacket(UI_LOAD_DB_LIST_RESULT,&ControlResult,sizeof(ControlResult));

		//设置时间
		ASSERT_ERR(m_pITimerEngine!=NULL);
		m_pITimerEngine->SetTimer(IDI_LOAD_GAME_LIST,m_pInitParameter->m_wLoadListTime*1000L,1,0);
	}
	else
	{
		//构造提示
		TCHAR szDescribe[128]=TEXT("");
		_sntprintf(szDescribe,CountArray(szDescribe),TEXT("服务器列表加载失败，%ld 秒后将重新加载"),m_pInitParameter->m_wReLoadListTime);

		//提示消息
		TRACE_LOG(szDescribe,TraceLevel_Warning);

		//设置时间
		ASSERT_ERR(m_pITimerEngine!=NULL);
		m_pITimerEngine->SetTimer(IDI_LOAD_GAME_LIST,m_pInitParameter->m_wReLoadListTime*1000L,1,0);
	}

	return true;
}

//收藏列表
bool CAttemperEngineSink::OnDBPCGameCollection(DWORD dwContextID,VOID* pData,WORD wDataSize)
{
	DBO_GP_GameCollection *pGameCollection = (DBO_GP_GameCollection*)pData;

	CMD_GP_Collection CollectionInfo;
	CollectionInfo.wFlag = pGameCollection->wFlag;
	CollectionInfo.wUserID = pGameCollection->wUserID;
	CollectionInfo.wSortID = pGameCollection->wSortID;
	CollectionInfo.wCollectionCount = pGameCollection->wCount;
	lstrcpyn(CollectionInfo.DisplayName,pGameCollection->szDisplayName,CountArray(CollectionInfo.DisplayName));
	CopyMemory(CollectionInfo.Collection,pGameCollection->Collection,CountArray(CollectionInfo.Collection) * sizeof(tagGameCollection));

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_SERVER_LIST,SUB_GP_LIST_COLLECTION,&CollectionInfo,sizeof(CMD_GP_Collection));

	if(pGameCollection->wFlag == COLLECTION_QUERY)
	{
		//登录完成
		CMD_GP_LogonFinish LogonFinish;
		ZeroMemory(&LogonFinish,sizeof(LogonFinish));
		LogonFinish.wIntermitTime=m_pInitParameter->m_wIntermitTime;
		LogonFinish.wOnLineCountTime=m_pInitParameter->m_wOnLineCountTime;
		m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_LOGON_FINISH,&LogonFinish,sizeof(LogonFinish));
	}

	return true;
}

//版本检测
bool CAttemperEngineSink::CheckPlazaVersion(BYTE cbDeviceType, DWORD dwPlazaVersion, DWORD dwSocketID, bool bCheckLowVer)
{
	//变量定义
	bool bMustUpdate=false;
	bool bAdviceUpdate=false;
	DWORD dwVersion=VERSION_PLAZA;

	//手机版本
	if(cbDeviceType >= DEVICE_TYPE_IPAD) dwVersion=VERSION_MOBILE_IOS;
	else if(cbDeviceType >= DEVICE_TYPE_IPHONE) dwVersion=VERSION_MOBILE_IOS;
	else if(cbDeviceType >= DEVICE_TYPE_ITOUCH) dwVersion=VERSION_MOBILE_IOS;
	else if(cbDeviceType >= DEVICE_TYPE_ANDROID) dwVersion=VERSION_MOBILE_ANDROID;
	else if(cbDeviceType == DEVICE_TYPE_PC) dwVersion=VERSION_PLAZA;

	//版本判断
	if (bCheckLowVer && GetSubVer(dwPlazaVersion)<GetSubVer(dwVersion)) bAdviceUpdate=true;
	if (GetMainVer(dwPlazaVersion)!=GetMainVer(dwVersion)) bMustUpdate=true;
	if (GetProductVer(dwPlazaVersion)!=GetProductVer(dwVersion)) bMustUpdate=true;

	//升级判断
	if ((bMustUpdate==true)||(bAdviceUpdate==true))
	{
		//变量定义
		CMD_GP_UpdateNotify UpdateNotify;
		ZeroMemory(&UpdateNotify,sizeof(UpdateNotify));

		//变量定义
		UpdateNotify.cbMustUpdate=bMustUpdate;
		UpdateNotify.cbAdviceUpdate=bAdviceUpdate;
		UpdateNotify.dwCurrentVersion=dwVersion;

		//发送消息
		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_LOGON,SUB_GP_UPDATE_NOTIFY,&UpdateNotify,sizeof(UpdateNotify));

		//中断判断
		if (bMustUpdate==true) 
		{
			m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);
			return false;
		}
	}

	return true;
}

void CAttemperEngineSink::GetDeviceVersion(BYTE cbDeviceType, WORD wModuleID, BYTE* cbArray)
{
	//当前路径
	TCHAR szFilePath[MAX_PATH] = L"";
	GetCurrentDirectory(sizeof(szFilePath), szFilePath);

	//ini
	TCHAR szFullPath[MAX_PATH] = L"";
	swprintf(szFullPath, sizeof(szFullPath), TEXT("%s\\PhoneVersion.ini"), szFilePath);

	CString StrPlatform = L"Pf_Android";
	switch (cbDeviceType)
	{
	case DEVICE_TYPE_ANDROID: StrPlatform = L"Pf_Android"; break;
	case DEVICE_TYPE_ITOUCH: StrPlatform = L"Pf_Touch"; break;
	case DEVICE_TYPE_IPHONE: StrPlatform = L"Pf_Phone"; break;
	case DEVICE_TYPE_IPAD: StrPlatform = L"Pf_IPad"; break;
	default:
		break;
	}

	CString StrName = L"";
	StrName.Format(L"%d", wModuleID);

	TCHAR szOutBuffer[MAX_PATH] = L"";
	GetPrivateProfileString(StrName, StrPlatform, L"1.0.0", szOutBuffer, sizeof(TCHAR)*MAX_PATH, szFullPath);

	int nIndex = 0;
	TCHAR* szToken = L"";

	szToken = _tcstok(szOutBuffer, L".");
	cbArray[nIndex++] = _tstoi(szToken);
	while (true)
	{
		szToken = _tcstok(NULL, L".");
		cbArray[nIndex++] = _tstoi(szToken);

		if (nIndex == 3)
			break;
	};

	return;
}

//版本检测
bool CAttemperEngineSink::CheckMBPlazaVersion(BYTE cbDeviceType, DWORD dwPlazaVersion, WORD wModuleID, DWORD dwSocketID)
{
	BYTE cbVersion[3] = { 0 };
	GetDeviceVersion(cbDeviceType, wModuleID, cbVersion);

	DWORD dwVersion = PROCESS_VERSION(cbVersion[0], cbVersion[1], cbVersion[2]);//获取版本号

	if (dwPlazaVersion != dwVersion)
	{
		//变量定义
		CMD_GP_UpdateNotify UpdateNotify;
		ZeroMemory(&UpdateNotify, sizeof(UpdateNotify));

		//变量定义
		UpdateNotify.cbMustUpdate = true;
		UpdateNotify.cbAdviceUpdate = true;
		UpdateNotify.dwCurrentVersion = dwVersion;

		//发送消息
		m_pITCPNetworkEngine->SendData(dwSocketID, MDM_MB_LOGON, SUB_MB_UPDATE_NOTIFY, &UpdateNotify, sizeof(UpdateNotify));

		//中断
		m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);
		return false;
	}

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

// 切换连接
bool CAttemperEngineSink::SwitchUserItemConnect(IServerUserItem * pIServerUserItem, WORD wTargetIndex)
{
	// 效验参数
	if (pIServerUserItem == NULL) return false;

	//断开用户
	if (pIServerUserItem->GetBindIndex() != INVALID_WORD)
	{
		WORD wSourceIndex = pIServerUserItem->GetBindIndex();
		tagBindParameter * pSourceParameter = GetBindParameter(wSourceIndex);

		//I_TRACE(TEXT("SwitchUserItemConnect 用户%s,同一个服务器多次登录"),pIServerUserItem->GetNickName());
		//通知上一个连接的玩家断开连接
		LPCTSTR pszMessage = TEXT("请注意，您的帐号在另一地方登录游戏，您被迫离开！");
		if (pIServerUserItem->IsAndroidUser() == false)
			m_pITCPNetworkEngine->SendData(pSourceParameter->dwSocketID, MDM_GP_USER_SERVICE, SUB_GP_FORCE_CLOSE, (void*)pszMessage, CountStringBuffer(pszMessage));

		if ((pSourceParameter != NULL) && (pSourceParameter->pIServerUserItem == pIServerUserItem)) pSourceParameter->pIServerUserItem = NULL;

		//m_pITCPNetworkEngine->ShutDownSocket(pSourceParameter->dwSocketID);

		//并删除玩家资源
		//m_ServerUserManager.DeleteUserItem(pIServerUserItem);
		pIServerUserItem->SetUserStatus(US_NULL,INVALID_TABLE,INVALID_CHAIR);
	}

	return true;
}

//发送类型
VOID CAttemperEngineSink::SendGameTypeInfo(DWORD dwSocketID)
{
	//网络数据
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//枚举数据
	POSITION Position=NULL;
	CGameTypeItem * pGameTypeItem=NULL;

	//枚举数据
	for (DWORD i=0;i<m_ServerListManager.GetGameTypeCount();i++)
	{
		//发送数据
		if ((wSendSize+sizeof(tagGameType))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_TYPE,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//获取数据
		pGameTypeItem=m_ServerListManager.EmunGameTypeItem(Position);
		if (pGameTypeItem==NULL) break;

		//拷贝数据
		CopyMemory(cbDataBuffer+wSendSize,&pGameTypeItem->m_GameType,sizeof(tagGameType));
		wSendSize+=sizeof(tagGameType);
	}

	//发送剩余
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_TYPE,cbDataBuffer,wSendSize);

	return;
}

//发送种类
VOID CAttemperEngineSink::SendGameKindInfo(DWORD dwSocketID)
{
	//网络数据
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//枚举数据
	POSITION Position=NULL;
	CGameKindItem * pGameKindItem=NULL;

	//枚举数据
	for (DWORD i=0;i<m_ServerListManager.GetGameKindCount();i++)
	{
		//发送数据
		if ((wSendSize+sizeof(tagGameKind))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_KIND,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//获取数据
		pGameKindItem=m_ServerListManager.EmunGameKindItem(Position);
		if (pGameKindItem==NULL) break;

		//拷贝数据
		CopyMemory(cbDataBuffer+wSendSize,&pGameKindItem->m_GameKind,sizeof(tagGameKind));
		wSendSize+=sizeof(tagGameKind);
	}

	//发送剩余
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_KIND,cbDataBuffer,wSendSize);

	return;
}

//发送节点
VOID CAttemperEngineSink::SendGameNodeInfo(DWORD dwSocketID, WORD wKindID)
{
	//网络数据
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//枚举数据
	POSITION Position=NULL;
	CGameNodeItem * pGameNodeItem=NULL;

	//枚举数据
	for (DWORD i=0;i<m_ServerListManager.GetGameNodeCount();i++)
	{
		//发送数据
		if ((wSendSize+sizeof(tagGameNode))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_NODE,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//获取数据
		pGameNodeItem=m_ServerListManager.EmunGameNodeItem(Position);
		if (pGameNodeItem==NULL) break;

		//拷贝数据
		if ((wKindID==INVALID_WORD)||(pGameNodeItem->m_GameNode.wKindID==wKindID))
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGameNodeItem->m_GameNode,sizeof(tagGameNode));
			wSendSize+=sizeof(tagGameNode);
		}
	}

	//发送剩余
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_NODE,cbDataBuffer,wSendSize);

	return;
}

//发送定制
VOID CAttemperEngineSink::SendGamePageInfo(DWORD dwSocketID, WORD wKindID)
{
	//网络数据
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//枚举数据
	POSITION Position=NULL;
	CGamePageItem * pGamePageItem=NULL;

	//枚举数据
	for (DWORD i=0;i<m_ServerListManager.GetGamePageCount();i++)
	{
		//发送数据
		if ((wSendSize+sizeof(tagGamePage))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_PAGE,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//获取数据
		pGamePageItem=m_ServerListManager.EmunGamePageItem(Position);
		if (pGamePageItem==NULL) break;

		//拷贝数据
		if ((wKindID==INVALID_WORD)||(pGamePageItem->m_GamePage.wKindID==wKindID))
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGamePageItem->m_GamePage,sizeof(tagGamePage));
			wSendSize+=sizeof(tagGamePage);
		}
	}

	//发送剩余
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_PAGE,cbDataBuffer,wSendSize);

	return;
}

//发送房间
VOID CAttemperEngineSink::SendGameServerInfo(DWORD dwSocketID, WORD wKindID)
{
	//网络数据
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//枚举数据
	POSITION Position=NULL;
	CGameServerItem * pGameServerItem=NULL;

	//枚举数据
	for (DWORD i=0;i<m_ServerListManager.GetGameServerCount();i++)
	{
		//发送数据
		if ((wSendSize+sizeof(tagGameServer))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_SERVER,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//获取数据
		pGameServerItem=m_ServerListManager.EmunGameServerItem(Position);
		if (pGameServerItem==NULL) break;

		//拷贝数据
		if ((wKindID==INVALID_WORD)||(pGameServerItem->m_GameServer.wKindID==wKindID))
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGameServerItem->m_GameServer,sizeof(tagGameServer));
			wSendSize+=sizeof(tagGameServer);
		}
	}

	//发送剩余
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_SERVER,cbDataBuffer,wSendSize);

	return;
}

//发送房间
VOID CAttemperEngineSink::SendGameServerMBInfo(DWORD dwSocketID, WORD wKindID)
{
	//网络数据
	WORD wSendSize = 0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//枚举数据
	POSITION Position = NULL;
	CGameServerItem * pGameServerItem = NULL;
	DWORD dwRand = 0;
	DWORD dwRand_ = 0;
	//枚举数据
	for (DWORD i = 0; i<m_ServerListManager.GetGameServerCount(); i++)
	{
		//发送数据
		if ((wSendSize + sizeof(tagGameServer_))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID, MDM_MB_SERVER_LIST, SUB_MB_LIST_SERVER, cbDataBuffer, wSendSize);
			wSendSize = 0;
		}

		//获取数据
		pGameServerItem = m_ServerListManager.EmunGameServerItem(Position);
		if (pGameServerItem == NULL) break;

		dwRand_ = rand() % 800 + 300;
		dwRand = rand() % 100 + dwRand_;
		//拷贝数据（过滤比赛房间）
		if (wKindID == INVALID_WORD && pGameServerItem->m_GameServer.wServerType != GAME_GENRE_MATCH)
		{
			tagGameServer_ GameServer;
			memset(GameServer.szServerName, 0, sizeof GameServer.szServerName);

			GameServer.dwFullCount = pGameServerItem->m_GameServer.dwFullCount;	
			GameServer.dwOnLineCount = pGameServerItem->m_GameServer.dwOnLineCount + dwRand;
			GameServer.lCellScore = pGameServerItem->m_GameServer.lCellScore;
			_tcscpy_s(GameServer.szServerAddr, pGameServerItem->m_GameServer.szServerAddr);
			_tcscpy_s(GameServer.szServerName, pGameServerItem->m_GameServer.szServerName);
			GameServer.wServerID = pGameServerItem->m_GameServer.wServerID;
			GameServer.wServerPort = pGameServerItem->m_GameServer.wServerPort;
			GameServer.wServerType = pGameServerItem->m_GameServer.wServerType;
			GameServer.cbRoomType = pGameServerItem->m_GameServer.cbRoomType;

			//限制配置
			GameServer.lMinEnterScore	= pGameServerItem->m_GameServer.lMinEnterScore	;		//获得血量
			GameServer.lMinTableScore	= pGameServerItem->m_GameServer.lMinTableScore	;		//最低积分
			GameServer.lMaxEnterScore	= pGameServerItem->m_GameServer.lMaxEnterScore	;		//最低体力

			GameServer.wKindID			= pGameServerItem->m_GameServer.wKindID;

			CopyMemory(cbDataBuffer + wSendSize, &GameServer, sizeof(tagGameServer_));
			wSendSize += sizeof(tagGameServer_);
		}
	}

	//发送剩余
	if (wSendSize > 0) m_pITCPNetworkEngine->SendData(dwSocketID, MDM_MB_SERVER_LIST, SUB_MB_LIST_SERVER, cbDataBuffer, wSendSize);

	//m_pITCPNetworkEngine->SendData(dwSocketID, MDM_MB_SERVER_LIST, SUB_MB_LIST_FINISH, &wModuleID, sizeof(WORD));

	return;
}

//发送收藏
VOID CAttemperEngineSink::SendCollectionInfo(DWORD dwSocketID, CMD_GP_Collection collection)
{
	//投递请求
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_COLLECTION,dwSocketID,&collection,sizeof(CMD_GP_Collection));
}

//发送类型
VOID CAttemperEngineSink::SendMobileKindInfo(DWORD dwSocketID, WORD wModuleID)
{
	//网络数据
	WORD wSendSize = 0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//枚举数据
	POSITION Position = NULL;
	CGameKindItem * pGameKindItem = NULL;

	//枚举数据
	for (DWORD i = 0; i<m_ServerListManager.GetGameKindCount(); i++)
	{
		//发送数据
		if ((wSendSize + sizeof(tagGameKind_))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID, MDM_MB_SERVER_LIST, SUB_MB_LIST_KIND, cbDataBuffer, wSendSize);
			wSendSize = 0;
		}

		//获取数据
		pGameKindItem = m_ServerListManager.EmunGameKindItem(Position);
		if (pGameKindItem == NULL) break;

		//手机这边要做宽窄变换
		tagGameKind_ GameKind;
		ZeroMemory(&GameKind, sizeof(GameKind));
		GameKind.dwFullCount = pGameKindItem->m_GameKind.dwFullCount;
		GameKind.dwOnLineCount = pGameKindItem->m_GameKind.dwOnLineCount;
		GameKind.wGameID = pGameKindItem->m_GameKind.wGameID;
		GameKind.wIsRoomOnline = pGameKindItem->m_GameKind.wIsRoomOnline;
		GameKind.wKindID = pGameKindItem->m_GameKind.wKindID;
		_tcscpy_s(GameKind.szKindName, pGameKindItem->m_GameKind.szKindName);

		CopyMemory(cbDataBuffer + wSendSize, &GameKind, sizeof(tagGameKind_));
		wSendSize += sizeof(tagGameKind_);
	}

	//发送剩余
	if (wSendSize > 0) m_pITCPNetworkEngine->SendData(dwSocketID, MDM_MB_SERVER_LIST, SUB_MB_LIST_KIND, cbDataBuffer, wSendSize);

	return;
}

bool CAttemperEngineSink::OnDBBroadcastMsg(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	DBO_GP_BroadcastMsg* pBroadcastMsg = (DBO_GP_BroadcastMsg*)pData;
	CMD_GP_BroadcastMsg ClientBroadcast;
	ZeroMemory(&ClientBroadcast,sizeof(ClientBroadcast));
	for ( WORD i =0 ;i<pBroadcastMsg->wCount ;++i )
	{
		CopyMemory(&ClientBroadcast.ArrayBroadcastMsg[i],&pBroadcastMsg->ArrayBroadcastMsg[i],sizeof(tagBroadcastMsg));
	}
	ClientBroadcast.wCount = pBroadcastMsg->wCount;
	WORD wPackSize = ClientBroadcast.wCount * sizeof(ClientBroadcast.ArrayBroadcastMsg[0]);
	WORD wHeadSize = sizeof(ClientBroadcast) - sizeof(ClientBroadcast.ArrayBroadcastMsg);
	SendData(BG_COMPUTER,MDM_GP_USER_SERVICE,SUB_GP_BROADCAST_MSG,&ClientBroadcast,wHeadSize+wPackSize);

	return true;
}

bool CAttemperEngineSink::OnDBBroadcastTyphon(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	DBO_GP_BroadcastTyphon* pTyphon = (DBO_GP_BroadcastTyphon*)pData;
	CMD_GP_BroadcastTyphon ClientTyhon ={0};

	ClientTyhon.cbType = pTyphon->cbType;
	ClientTyhon.dwUserID = pTyphon->dwUserID;
	ClientTyhon.cbSuccess = pTyphon->cbSuccess;
	lstrcpyn(ClientTyhon.szNickName,pTyphon->szNickName,CountArray(ClientTyhon.szNickName));
	WORD wPackSize = CountStringBuffer(ClientTyhon.szContent);
	lstrcpyn(ClientTyhon.szContent,pTyphon->szContent,wPackSize);
	WORD wHeadSize = sizeof(ClientTyhon) - sizeof(ClientTyhon.szContent);

	if ( ClientTyhon.cbSuccess )
		SendData(BG_COMPUTER,MDM_GP_USER_SERVICE,SUB_GP_BROADCAST_TYPHON,&ClientTyhon,wHeadSize+wPackSize);
	else
		m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_BROADCAST_TYPHON,&ClientTyhon,wHeadSize+wPackSize);

	return true;
}

// 签到
bool CAttemperEngineSink::OnDBTodayIsSignin(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	DBO_GP_GetSignIn* pTyphon = (DBO_GP_GetSignIn*)pData;
	
	LogonServer::PROTO_LogonFinish ProtoLogonFinish;
	ProtoLogonFinish.set_cblogonfinish(1);
	std::string serializeStr;
	ProtoLogonFinish.SerializeToString(&serializeStr);

	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_LOGON, SUB_GP_WEB_LOGON_FINISH, (void*)serializeStr.c_str(), (WORD)serializeStr.size());

	return true;
}

bool CAttemperEngineSink::OnDBEventDeal(DWORD dwContextID, VOID* pData, WORD wDataSize, WORD wMainCmdID, WORD wSubCmdID)
{
	//判断在线
	ASSERT_ERR(LOWORD(dwContextID) < m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter + LOWORD(dwContextID))->dwSocketID != dwContextID) return true;


	// 登录成功数据
	auto pDBOLogonSuccess = (DBO_MB_LogonSuccess_BCode*)pData;

	LogonServer::PROTO_LogonSuccessBCode ProtoLogonSuccessBCode;
	ProtoLogonSuccessBCode.set_wfaceid(pDBOLogonSuccess->wFaceID);
	ProtoLogonSuccessBCode.set_cbgender(pDBOLogonSuccess->cbGender);
	ProtoLogonSuccessBCode.set_dwuserid(pDBOLogonSuccess->dwUserID);
	ProtoLogonSuccessBCode.set_dwgameid(pDBOLogonSuccess->dwGameID);
	ProtoLogonSuccessBCode.set_dwexperience(pDBOLogonSuccess->dwExperience);
	ProtoLogonSuccessBCode.set_cbmemberorder(pDBOLogonSuccess->cbMemberOrder);

	ProtoLogonSuccessBCode.set_dwusermedal(pDBOLogonSuccess->dwUserMedal);
	ProtoLogonSuccessBCode.set_luserscore(pDBOLogonSuccess->lUserScore);
	ProtoLogonSuccessBCode.set_luserinsure(pDBOLogonSuccess->lUserInsure);

	ProtoLogonSuccessBCode.set_dwinoutindex(pDBOLogonSuccess->dwInoutIndex);
	ProtoLogonSuccessBCode.set_lrevenue(pDBOLogonSuccess->lRevenue);
	ProtoLogonSuccessBCode.set_llobbylv(pDBOLogonSuccess->lLobbyLV);
	ProtoLogonSuccessBCode.set_cbvisitorclient(pDBOLogonSuccess->cbVisitorClient);

	ProtoLogonSuccessBCode.set_sznickname(Utils::ConvertFromUtf16ToUtf8(pDBOLogonSuccess->szNickName));
	ProtoLogonSuccessBCode.set_szaccounts(Utils::ConvertFromUtf16ToUtf8(pDBOLogonSuccess->szAccounts));
	ProtoLogonSuccessBCode.set_szunderwrite(Utils::ConvertFromUtf16ToUtf8(pDBOLogonSuccess->szUnderWrite));
	
	ProtoLogonSuccessBCode.set_dwserverid(pDBOLogonSuccess->dwServerID);

	std::string serializeStr;
	ProtoLogonSuccessBCode.SerializeToString(&serializeStr);

	//发送数据
	m_pITCPNetworkEngine->SendData(dwContextID, wMainCmdID, wSubCmdID, (void*)serializeStr.c_str(), (WORD)serializeStr.size());

	return true;
}

bool CAttemperEngineSink::OnDBCorrespondEventDeal(DWORD dwContextID, VOID* pData, WORD wDataSize, WORD wMainCmdID, WORD wSubCmdID)
{
	//发送数据
	m_pITCPSocketService->SendData( wMainCmdID, wSubCmdID, pData, wDataSize);

	return true;
}


//刷新通用属性
bool CAttemperEngineSink::OnDBUpdateAttribute(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_UPDATE_ATTRIBUTE,pData,wDataSize);

	return true;
}


template<WORD wRequestID, typename StructType>
bool CAttemperEngineSink::PostDataBaseRequest(DWORD dwContextID, VOID* pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize == sizeof(StructType));
	if (wDataSize != sizeof(StructType)) return false;

	//投递数据库事件
	m_pIDataBaseEngine->PostDataBaseRequest(wRequestID, dwContextID, pData, wDataSize);

	return true;
}


//发送数据
bool CAttemperEngineSink::SendData(BYTE cbSendMask, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//用户数据
	m_pITCPNetworkEngine->SendDataBatch(wMainCmdID,wSubCmdID,pData,wDataSize,cbSendMask);

	return true;
}

//绑定用户
IServerUserItem * CAttemperEngineSink::GetBindUserItem(DWORD dwSocketID)
{
	WORD wBindIndex=LOWORD(dwSocketID);
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
	if (wBindIndex<m_pInitParameter->m_wMaxConnect)
	{
		return m_pBindParameter+wBindIndex;
	}

	//错误断言
	ASSERT_ERR(FALSE);

	return NULL;
}
bool CAttemperEngineSink::OnEventUserItemStatus(IServerUserItem * pIServerUserItem, WORD wOldTableID, WORD wOldChairID, BYTE cbNotifyClient)
{
	//离开判断
	if (pIServerUserItem->GetUserStatus()==US_NULL)
	{
		//获取绑定
		WORD wBindIndex=pIServerUserItem->GetBindIndex();
		tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

		//绑带处理
		if (pBindParameter!=NULL)
		{
			//绑定处理
			//if (pBindParameter->pIServerUserItem==pIServerUserItem)
			//	pBindParameter->pIServerUserItem=NULL;

			//中断网络
			if (pBindParameter->dwSocketID!=0L)
				m_pITCPNetworkEngine->ShutDownSocket(pBindParameter->dwSocketID);
		}

		//删除用户
		//DBR_GP_LogonOut LogonOut = { 0 };
		//LogonOut.dwUserID = pIServerUserItem->GetUserID();
		//LogonOut.dwInOutIndex = pIServerUserItem->GetInoutIndex();

		m_ServerUserManager.DeleteUserItem(pIServerUserItem);
		//m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOGON_OUT, 0, &LogonOut, sizeof(LogonOut));
	}
	return true;
}

//用户聊天
bool CAttemperEngineSink::OnTCPNetworkSubUserChat(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	CMD_GR_C_UserChat * pUserChat=(CMD_GR_C_UserChat *)pData;

	ASSERT_ERR(wDataSize<=sizeof(CMD_GR_C_UserChat));
	if (wDataSize>sizeof(CMD_GR_C_UserChat)) return false;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pISendUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT_ERR(pISendUserItem!=NULL);
	if (pISendUserItem==NULL) return false;

	//寻找用户
	// 	IServerUserItem * pIRecvUserItem=NULL;
	// 	if (pUserChat->dwTargetUserID!=0)
	// 	{
	// 		pIRecvUserItem=m_ServerUserManager.SearchUserItem(pUserChat->dwTargetUserID);
	// 		if (pIRecvUserItem==NULL) return true;
	// 	}

	//构造消息
	CMD_GR_S_UserChat UserChat;
	ZeroMemory(&UserChat,sizeof(UserChat));

	//字符过滤
	SensitiveWordFilter(pUserChat->szChatString,UserChat.szChatString,CountArray(UserChat.szChatString));

	//构造数据
	UserChat.dwChatColor=pUserChat->dwChatColor;
	UserChat.wChatLength=pUserChat->wChatLength;
	UserChat.dwSendUserID=pISendUserItem->GetUserID();
	UserChat.dwTargetUserID=pUserChat->dwTargetUserID;
	UserChat.wChatLength=CountStringBuffer(UserChat.szChatString);

	//转发消息
	WORD wHeadSize=sizeof(UserChat)-sizeof(UserChat.szChatString);
	m_pITCPSocketService->SendData(MDM_CS_SERVICE_INFO,SUB_CS_C_USER_CHAT,&UserChat,wHeadSize+UserChat.wChatLength*sizeof(UserChat.szChatString[0]));

	return true;
}

//敏感词过滤
void CAttemperEngineSink::SensitiveWordFilter(LPCTSTR pMsg, LPTSTR pszFiltered, int nMaxLen)
{
	m_WordsFilter.Filtrate(pMsg,pszFiltered,nMaxLen);
}

//////////////////////////////////////////////////////////////////////////////////
