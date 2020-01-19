#include "StdAfx.h"
#include "ServiceUnits.h"
#include "ControlPacket.h"
#include "AttemperEngineSink.h"
//////////////////////////////////////////////////////////////////////////
#define IDI_MESSAGE				10086
#define IDI_BACKUP				20086
#define IDI_BACKUP_RESET		30086
#define IDI_CHECK_EXPIRE_RED	40000		//检查过期红包
#define IDI_CHECK_RESIDUE_RED	40001		//检查还有剩余的红包

//
#define IDT_BACKUP_DAY_TIME		86400
#define IDT_BACKUP_RESET_TIME	60

//////////////////////////////////////////////////////////////////////////////////

//启动事件
bool CAttemperEngineSink::OnAttemperEngineBegin(IUnknownEx * pIUnknownEx)
{
	return true;
}

//控制事件
bool CAttemperEngineSink::OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize)
{
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
	case IDI_MESSAGE+1:
	case IDI_MESSAGE+2:
	case IDI_MESSAGE+3:
		{
			SendMsg(dwTimerID - IDI_MESSAGE);
			StartNextMsgTime(dwTimerID - IDI_MESSAGE);
		}
		return true;
	case IDI_BACKUP_RESET:
		{
			SLocalTime locTime;
			SGSYS_GetLocalTime(0, &locTime);

			if (locTime.m_nMday != gLocalTime.m_nMday || 
				locTime.m_nMon != gLocalTime.m_nMon ||
				locTime.m_nYear != gLocalTime.m_nYear)
			{
				BackupAndMoveFile();
				m_pITCPNetworkEngine->SendDataBatch(MDM_CS_MANAGER_SERVICE, SUB_CS_S_HIGH_SCORE_LIST_R, NULL, 0, 0);
			}

			SGSYS_GetLocalTime(0, &gLocalTime);
			int left = GetBackupTime();
			m_pITimerEngine->KillTimer(IDI_BACKUP);
			m_pITimerEngine->SetTimer(IDI_BACKUP, left * 1000, 1, 0);
			m_pITimerEngine->SetTimer(IDI_BACKUP_RESET, IDT_BACKUP_RESET_TIME * 1000, 1, 0);
			////I_TRACE(TEXT("%d秒后进行备份检查"), left);
			break;

		}
	case IDI_BACKUP:
		{
			SLocalTime locTime;
			SGSYS_GetLocalTime(0, &locTime);

			if (locTime.m_nMday != gLocalTime.m_nMday || 
				locTime.m_nMon != gLocalTime.m_nMon ||
				locTime.m_nYear != gLocalTime.m_nYear)
			{
				BackupAndMoveFile();
				m_pITCPNetworkEngine->SendDataBatch(MDM_CS_MANAGER_SERVICE, SUB_CS_S_HIGH_SCORE_LIST_R, NULL, 0, 0);
			}

			SGSYS_GetLocalTime(0, &gLocalTime);

			m_pITimerEngine->KillTimer(IDI_BACKUP);
			int left = GetBackupTime();
			m_pITimerEngine->SetTimer(IDI_BACKUP, left * 1000, 1, 0);
			//I_TRACE(TEXT("%d秒后进行捕鱼排行榜备份检查"), left);
		}
		return true;
	
	}
	return false;
}

//读取事件
bool CAttemperEngineSink::OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (Command.wMainCmdID)
	{
	case MDM_CS_REGISTER:		//服务注册
		{
			return OnTCPNetworkMainRegister(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_CS_SERVICE_INFO:	//服务信息
		{
			return OnTCPNetworkMainServiceInfo(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_CS_USER_COLLECT:	//用户命令
		{
			return OnTCPNetworkMainUserCollect(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_CS_REMOTE_SERVICE:	//远程服务
		{
			return OnTCPNetworkMainRemoteService(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_CS_MANAGER_SERVICE: //管理服务
		{
			return OnTCPNetworkMainManagerService(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_CS_MATCH_SERVICE:	//比赛服务
		{
			return true;
		}
	}

	return false;
}

//关闭事件
bool CAttemperEngineSink::OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID)
{
	//获取信息
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	bool isGameKind = false;

	//游戏服务
	if (pBindParameter->ServiceKind==ServiceKind_Game)
	{
		//变量定义
		WORD wBindIndex=LOWORD(dwSocketID);

		//汇总处理
		if (wBindIndex==m_wCollectItem)
		{
			//设置变量
			m_wCollectItem=INVALID_WORD;

			//汇总切换
			if (m_WaitCollectItemArray.GetCount()>0)
			{
				//提取变量
				INT_PTR nWaitCount=m_WaitCollectItemArray.GetCount();
				m_wCollectItem=m_WaitCollectItemArray[nWaitCount-1];

				//删除数组
				m_WaitCollectItemArray.RemoveAt(nWaitCount-1);

				//发送消息
				DWORD dwSocketID=(m_pBindParameter+m_wCollectItem)->dwSocketID;
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_USER_COLLECT,SUB_CS_S_COLLECT_REQUEST);
			}
		}
		else
		{
			//删除等待
			for (INT_PTR i=0;i<m_WaitCollectItemArray.GetCount();i++)
			{
				if (wBindIndex==m_WaitCollectItemArray[i])
				{
					m_WaitCollectItemArray.RemoveAt(i);
					break;
				}
			}
		}

		//变量定义
		CMD_CS_S_ServerRemove ServerRemove;
		ZeroMemory(&ServerRemove,sizeof(ServerRemove));

		//删除通知
		ServerRemove.wServerID=pBindParameter->wServiceID;
		m_pITCPNetworkEngine->SendDataBatch(MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_REMOVE,&ServerRemove,sizeof(ServerRemove),0L);

		//注销房间
		m_GlobalInfoManager.DeleteServerItem(pBindParameter->wServiceID);

		isGameKind = true;
	}

	//广场服务
	if (pBindParameter->ServiceKind==ServiceKind_Plaza)
	{
		//变量定义
		WORD wBindIndex=LOWORD(dwSocketID);
		tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

		//注销房间
		m_GlobalInfoManager.DeletePlazaItem(pBindParameter->wServiceID);
	}

	//清除信息
	ZeroMemory(pBindParameter,sizeof(tagBindParameter));


	BYTE * pClientAddr=(BYTE *)&dwClientAddr;
	//if (isGameKind)
	{
		//I_TRACE(TEXT("%d.%d.%d.%d %s断开链接"), pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3], isGameKind ? TEXT("房间服务器") : TEXT("登陆服务器"));
	}
	return false;
}

//注册服务
bool CAttemperEngineSink::OnTCPNetworkMainRegister(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_CS_C_REGISTER_PLAZA:	//注册广场
		{
			//效验数据
			ASSERT_ERR(wDataSize==sizeof(CMD_CS_C_RegisterPlaza));
			if (wDataSize!=sizeof(CMD_CS_C_RegisterPlaza)) return false;

			//消息定义
			CMD_CS_C_RegisterPlaza * pRegisterPlaza=(CMD_CS_C_RegisterPlaza *)pData;

			//有效判断
			if ((pRegisterPlaza->szServerName[0]==0)||(pRegisterPlaza->szServerAddr[0]==0))
			{
				//变量定义
				CMD_CS_S_RegisterFailure RegisterFailure;
				ZeroMemory(&RegisterFailure,sizeof(RegisterFailure));

				//设置变量
				RegisterFailure.lErrorCode=0L;
				lstrcpyn(RegisterFailure.szDescribeString,TEXT("服务器注册失败，“服务地址”与“服务器名”不合法！"),CountArray(RegisterFailure.szDescribeString));

				//发送消息
				WORD wStringSize=CountStringBuffer(RegisterFailure.szDescribeString);
				WORD wSendSize=sizeof(RegisterFailure)-sizeof(RegisterFailure.szDescribeString)+wStringSize;
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_REGISTER,SUB_CS_S_REGISTER_FAILURE,&RegisterFailure,wSendSize);

				//中断网络
				m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);

				return true;
			}

			//设置绑定
			WORD wBindIndex=LOWORD(dwSocketID);
			(m_pBindParameter+wBindIndex)->wServiceID=wBindIndex;
			(m_pBindParameter+wBindIndex)->ServiceKind=ServiceKind_Plaza;

			//变量定义
			tagGamePlaza GamePlaza;
			ZeroMemory(&GamePlaza,sizeof(GamePlaza));

			//构造数据
			GamePlaza.wPlazaID=wBindIndex;
			lstrcpyn(GamePlaza.szServerName,pRegisterPlaza->szServerName,CountArray(GamePlaza.szServerName));
			lstrcpyn(GamePlaza.szServerAddr,pRegisterPlaza->szServerAddr,CountArray(GamePlaza.szServerAddr));

			//注册房间
			m_GlobalInfoManager.ActivePlazaItem(wBindIndex,GamePlaza);

			//发送列表
			SendServerListItem(dwSocketID);

			//群发设置
			m_pITCPNetworkEngine->AllowBatchSend(dwSocketID,true,0L);

			//链接成功提示
			BYTE * pClientAddr=(BYTE *)&(m_pBindParameter+wBindIndex)->dwClientAddr;
			//I_TRACE(TEXT("%d.%d.%d.%d 登陆服务器注册成功"), pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);
			//g_Log.LogToFile(LOG_ROOT, TEXT("%d.%d.%d.%d 登陆服务器注册成功"), pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

			return true;
		}
	case SUB_CS_C_REGISTER_SERVER:	//注册房间
		{
			//效验数据
			ASSERT_ERR(wDataSize==sizeof(CMD_CS_C_RegisterServer));
			if (wDataSize!=sizeof(CMD_CS_C_RegisterServer)) return false;

			//消息定义
			CMD_CS_C_RegisterServer * pRegisterServer=(CMD_CS_C_RegisterServer *)pData;

			//查找房间
			if (m_GlobalInfoManager.SearchServerItem(pRegisterServer->wServerID)!=NULL)
			{
				//变量定义
				CMD_CS_S_RegisterFailure RegisterFailure;
				ZeroMemory(&RegisterFailure,sizeof(RegisterFailure));

				//设置变量
				RegisterFailure.lErrorCode=0L;
				lstrcpyn(RegisterFailure.szDescribeString,TEXT("已经存在相同标识的游戏房间服务，房间服务注册失败"),CountArray(RegisterFailure.szDescribeString));

				//发送消息
				WORD wStringSize=CountStringBuffer(RegisterFailure.szDescribeString);
				WORD wSendSize=sizeof(RegisterFailure)-sizeof(RegisterFailure.szDescribeString)+wStringSize;
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_REGISTER,SUB_CS_S_REGISTER_FAILURE,&RegisterFailure,wSendSize);

				//中断网络
				m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);

				return true;
			}

			//设置绑定
			WORD wBindIndex=LOWORD(dwSocketID);
			(m_pBindParameter+wBindIndex)->ServiceKind=ServiceKind_Game;
			(m_pBindParameter+wBindIndex)->wServiceID=pRegisterServer->wServerID;

			//变量定义
			tagGameServer GameServer;
			ZeroMemory(&GameServer,sizeof(GameServer));

			//构造数据
			GameServer.wKindID=pRegisterServer->wKindID;
			GameServer.wNodeID=pRegisterServer->wNodeID;
			GameServer.wSortID=pRegisterServer->wSortID;
			GameServer.wServerID=pRegisterServer->wServerID;
			GameServer.wServerPort=pRegisterServer->wServerPort;
			GameServer.dwOnLineCount=pRegisterServer->dwOnLineCount;
			GameServer.dwFullCount=pRegisterServer->dwFullCount;
			GameServer.wServerType=pRegisterServer->wServerType;
			lstrcpyn(GameServer.szServerName,pRegisterServer->szServerName,CountArray(GameServer.szServerName));
			lstrcpyn(GameServer.szServerAddr,pRegisterServer->szServerAddr,CountArray(GameServer.szServerAddr));
			GameServer.lCellScore = pRegisterServer->lCellScore;	//游戏基数
			GameServer.cbRoomType = pRegisterServer->cbRoomType;

			//限制配置
			GameServer.lRestrictScore	= pRegisterServer->lRestrictScore	;						//限制积分
			GameServer.lMinTableScore	= pRegisterServer->lMinTableScore	;						//最低积分
			GameServer.lMinEnterScore	= pRegisterServer->lMinEnterScore	;						//最低积分
			GameServer.lMaxEnterScore	= pRegisterServer->lMaxEnterScore	;						//最高积分
			GameServer.lMinEnterTiLi	= pRegisterServer->lMinEnterTiLi	;						//最低体力

			GameServer.wCardBoxIndex	= pRegisterServer->wCardBoxIndex	;						//宝箱索引

			//注册房间
			m_GlobalInfoManager.ActiveServerItem(wBindIndex,GameServer);

			//群发设置
			m_pITCPNetworkEngine->AllowBatchSend(dwSocketID,true,0L);

			//群发房间
			m_pITCPNetworkEngine->SendDataBatch(MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_INSERT,&GameServer,sizeof(GameServer),0L);

			//发送列表 
			SendServerListItem(dwSocketID);		

			//汇总通知
			if (m_wCollectItem==INVALID_WORD)
			{
				m_wCollectItem=wBindIndex;
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_USER_COLLECT,SUB_CS_S_COLLECT_REQUEST);
			}
			else m_WaitCollectItemArray.Add(wBindIndex);

			BYTE * pClientAddr=(BYTE *)&(m_pBindParameter+wBindIndex)->dwClientAddr;
			//I_TRACE(TEXT("%d.%d.%d.%d [%s]注册成功"), pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3],GameServer.szServerName);

			return true;
		}
	case SUB_CS_C_REGISTER_MATCH:
		{			
			//效验数据
			ASSERT_ERR(wDataSize==sizeof(CMD_CS_C_RegisterMatch));
			if (wDataSize!=sizeof(CMD_CS_C_RegisterMatch)) return false;

			//消息定义
			CMD_CS_C_RegisterMatch * pRegisterMatch=(CMD_CS_C_RegisterMatch *)pData;

			return true;
		}
	}

	return false;
}

//服务状态
bool CAttemperEngineSink::OnTCPNetworkMainServiceInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_CS_C_SERVER_ONLINE:	//房间人数
		{
			//效验数据
			ASSERT_ERR(wDataSize==sizeof(CMD_CS_C_ServerOnLine));
			if (wDataSize!=sizeof(CMD_CS_C_ServerOnLine)) return false;

			//消息定义
			CMD_CS_C_ServerOnLine * pServerOnLine=(CMD_CS_C_ServerOnLine *)pData;

			//获取参数
			WORD wBindIndex=LOWORD(dwSocketID);
			tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

			//连接效验
			ASSERT_ERR(pBindParameter->ServiceKind==ServiceKind_Game);
			if (pBindParameter->ServiceKind!=ServiceKind_Game) return false;

			//查找房间
			WORD wServerID=pBindParameter->wServiceID;
			CGlobalServerItem * pGlobalServerItem=m_GlobalInfoManager.SearchServerItem(wServerID);

			//设置人数
			if (pGlobalServerItem!=NULL)
			{
				//变量定义
				CMD_CS_S_ServerOnLine ServerOnLine;
				ZeroMemory(&ServerOnLine,sizeof(ServerOnLine));

				//设置变量
				pGlobalServerItem->m_GameServer.dwOnLineCount=pServerOnLine->dwOnLineCount;

				//设置变量
				ServerOnLine.wServerID=wServerID;
				ServerOnLine.dwOnLineCount=pServerOnLine->dwOnLineCount;

				//发送通知
				m_pITCPNetworkEngine->SendDataBatch(MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_ONLINE,&ServerOnLine,sizeof(ServerOnLine),0L);
			}

			return true;
		}
	case SUB_CS_C_SERVER_MODIFY:	//房间修改
		{
			//效验数据
			ASSERT_ERR(wDataSize==sizeof(CMD_CS_C_ServerModify));
			if (wDataSize!=sizeof(CMD_CS_C_ServerModify)) return false;

			//消息定义
			CMD_CS_C_ServerModify * pServerModify=(CMD_CS_C_ServerModify *)pData;

			//获取参数
			WORD wBindIndex=LOWORD(dwSocketID);
			tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

			//连接效验
			ASSERT_ERR(pBindParameter->ServiceKind==ServiceKind_Game);
			if (pBindParameter->ServiceKind!=ServiceKind_Game) return false;

			//查找房间
			ASSERT_ERR(m_GlobalInfoManager.SearchServerItem(pBindParameter->wServiceID)!=NULL);
			CGlobalServerItem * pGlobalServerItem=m_GlobalInfoManager.SearchServerItem(pBindParameter->wServiceID);

			//房间修改
			if (pGlobalServerItem!=NULL)
			{
				//设置变量
				pGlobalServerItem->m_GameServer.wSortID=pServerModify->wSortID;
				pGlobalServerItem->m_GameServer.wKindID=pServerModify->wKindID;
				pGlobalServerItem->m_GameServer.wNodeID=pServerModify->wNodeID;
				pGlobalServerItem->m_GameServer.wServerPort=pServerModify->wServerPort;
				pGlobalServerItem->m_GameServer.dwOnLineCount=pServerModify->dwOnLineCount;
				pGlobalServerItem->m_GameServer.dwFullCount=pServerModify->dwFullCount;
				lstrcpyn(pGlobalServerItem->m_GameServer.szServerName,pServerModify->szServerName,CountArray(pGlobalServerItem->m_GameServer.szServerName));
				lstrcpyn(pGlobalServerItem->m_GameServer.szServerAddr,pServerModify->szServerAddr,CountArray(pGlobalServerItem->m_GameServer.szServerAddr));

				//变量定义
				CMD_CS_S_ServerModify ServerModify;
				ZeroMemory(&ServerModify,sizeof(ServerModify));

				//设置变量
				ServerModify.wKindID=pServerModify->wKindID;
				ServerModify.wSortID=pServerModify->wSortID;
				ServerModify.wNodeID=pServerModify->wNodeID;
				ServerModify.wServerID=pBindParameter->wServiceID;
				ServerModify.wServerPort=pServerModify->wServerPort;
				ServerModify.dwOnLineCount=pServerModify->dwOnLineCount;
				ServerModify.dwFullCount=pServerModify->dwFullCount;
				lstrcpyn(ServerModify.szServerAddr,pServerModify->szServerAddr,CountArray(ServerModify.szServerAddr));
				lstrcpyn(ServerModify.szServerName,pServerModify->szServerName,CountArray(ServerModify.szServerName));

				//发送通知
				m_pITCPNetworkEngine->SendDataBatch(MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_MODIFY,&ServerModify,sizeof(ServerModify),0L);
			}

			return true;
		}
	}

	return false;
}

//用户处理
bool CAttemperEngineSink::OnTCPNetworkMainUserCollect(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_CS_C_USER_ENTER:		//用户进入
		{
			//效验数据
			ASSERT_ERR(wDataSize==sizeof(CMD_CS_C_UserEnter));
			if (wDataSize!=sizeof(CMD_CS_C_UserEnter)) return false;

			//消息处理
			CMD_CS_C_UserEnter * pUserEnter=(CMD_CS_C_UserEnter *)pData;
			pUserEnter->szNickName[CountArray(pUserEnter->szNickName)-1]=0;

			//获取参数
			WORD wBindIndex=LOWORD(dwSocketID);
			tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

			//连接效验
			ASSERT_ERR(pBindParameter->ServiceKind==ServiceKind_Game);
			if (pBindParameter->ServiceKind!=ServiceKind_Game) return false;

			//变量定义
			tagGlobalUserInfo GlobalUserInfo;
			ZeroMemory(&GlobalUserInfo,sizeof(GlobalUserInfo));

			//设置变量
			GlobalUserInfo.dwUserID=pUserEnter->dwUserID;
			GlobalUserInfo.dwGameID=pUserEnter->dwGameID;
			lstrcpyn(GlobalUserInfo.szNickName,pUserEnter->szNickName,CountArray(GlobalUserInfo.szNickName));

			//辅助信息
			GlobalUserInfo.cbGender=pUserEnter->cbGender;
			GlobalUserInfo.cbMemberOrder=pUserEnter->cbMemberOrder;
			GlobalUserInfo.cbMasterOrder=pUserEnter->cbMasterOrder;

			//激活用户
			m_GlobalInfoManager.ActiveUserItem(GlobalUserInfo,pBindParameter->wServiceID);
					
			return true;
		}
	case SUB_CS_C_USER_LEAVE:		//用户离开
		{
			//效验数据
			ASSERT_ERR(wDataSize==sizeof(CMD_CS_C_UserLeave));
			if (wDataSize!=sizeof(CMD_CS_C_UserLeave)) return false;

			//消息处理
			CMD_CS_C_UserLeave * pUserLeave=(CMD_CS_C_UserLeave *)pData;

			//获取参数
			WORD wBindIndex=LOWORD(dwSocketID);
			tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

			//连接效验
			ASSERT_ERR(pBindParameter->ServiceKind==ServiceKind_Game);
			if (pBindParameter->ServiceKind!=ServiceKind_Game) return false;

			//删除用户
			m_GlobalInfoManager.DeleteUserItem(pUserLeave->dwUserID,pBindParameter->wServiceID);

			return true;
		}
	case SUB_CS_C_USER_FINISH:		//用户完成
		{
			//获取参数
			WORD wBindIndex=LOWORD(dwSocketID);
			tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

			//连接效验
			ASSERT_ERR((m_wCollectItem==wBindIndex)&&(pBindParameter->ServiceKind==ServiceKind_Game));
			if ((m_wCollectItem!=wBindIndex)||(pBindParameter->ServiceKind!=ServiceKind_Game)) return false;

			//设置变量
			m_wCollectItem=INVALID_WORD;

			//汇总切换
			if (m_WaitCollectItemArray.GetCount()>0)
			{
				//切换汇总
				INT_PTR nWaitCount=m_WaitCollectItemArray.GetCount();
				m_wCollectItem=m_WaitCollectItemArray[nWaitCount-1];

				//删除数组
				m_WaitCollectItemArray.RemoveAt(nWaitCount-1);

				//发送消息
				DWORD dwSocketID=(m_pBindParameter+m_wCollectItem)->dwSocketID;
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_USER_COLLECT,SUB_CS_S_COLLECT_REQUEST);
			}

			return true;
		}
	}

	return false;
}

//远程服务
bool CAttemperEngineSink::OnTCPNetworkMainRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_CS_C_SEARCH_CORRESPOND:	//协调查找
		{
			//效验参数
			ASSERT_ERR(wDataSize==sizeof(CMD_CS_C_SearchCorrespond));
			if (wDataSize!=sizeof(CMD_CS_C_SearchCorrespond)) return false;

			//处理消息
			CMD_CS_C_SearchCorrespond * pSearchCorrespond=(CMD_CS_C_SearchCorrespond *)pData;
			pSearchCorrespond->szNickName[CountArray(pSearchCorrespond->szNickName)-1]=0;

			//变量定义
			CMD_CS_S_SearchCorrespond SearchCorrespond;
			ZeroMemory(&SearchCorrespond,sizeof(SearchCorrespond));

			//结果用户
			CGlobalUserItem * ResultUserItem[2];
			ZeroMemory(ResultUserItem,sizeof(ResultUserItem));

			//设置变量
			SearchCorrespond.dwSocketID=pSearchCorrespond->dwSocketID;
			SearchCorrespond.dwClientAddr=pSearchCorrespond->dwClientAddr;

			//查找用户
			if (pSearchCorrespond->dwGameID!=0L)
			{
				ResultUserItem[0]=m_GlobalInfoManager.SearchUserItemByGameID(pSearchCorrespond->dwGameID);
			}

			//查找用户
			if (pSearchCorrespond->szNickName[0]!=0)
			{
				_tcslwr(pSearchCorrespond->szNickName);
				ResultUserItem[1]=m_GlobalInfoManager.SearchUserItemByNickName(pSearchCorrespond->szNickName);
			}

			//设置结果
			for (BYTE i=0;i<CountArray(ResultUserItem);i++)
			{
				if (ResultUserItem[i]!=NULL)
				{
					//变量定义
					WORD wServerIndex=0;

					//查找房间
					do
					{
						//查找房间
						CGlobalServerItem * pGlobalServerItem=ResultUserItem[i]->EnumServerItem(wServerIndex++);

						//终止判断
						if (pGlobalServerItem==NULL) break;
						if (SearchCorrespond.wUserCount>=CountArray(SearchCorrespond.UserRemoteInfo)) break;

						//索引定义
						WORD wIndex=SearchCorrespond.wUserCount++;

						//辅助信息
						SearchCorrespond.UserRemoteInfo[wIndex].cbGender=ResultUserItem[i]->GetGender();
						SearchCorrespond.UserRemoteInfo[wIndex].cbMemberOrder=ResultUserItem[i]->GetMemberOrder();
						SearchCorrespond.UserRemoteInfo[wIndex].cbMasterOrder=ResultUserItem[i]->GetMasterOrder();

						//用户信息
						SearchCorrespond.UserRemoteInfo[wIndex].dwUserID=ResultUserItem[i]->GetUserID();
						SearchCorrespond.UserRemoteInfo[wIndex].dwGameID=ResultUserItem[i]->GetGameID();
						lstrcpyn(SearchCorrespond.UserRemoteInfo[wIndex].szNickName,ResultUserItem[i]->GetNickName(),LEN_NICKNAME);

						//房间信息
						SearchCorrespond.UserRemoteInfo[wIndex].wKindID=pGlobalServerItem->GetKindID();
						SearchCorrespond.UserRemoteInfo[wIndex].wServerID=pGlobalServerItem->GetServerID();
						lstrcpyn(SearchCorrespond.UserRemoteInfo[wIndex].szGameServer,pGlobalServerItem->m_GameServer.szServerName,LEN_SERVER);

					} while (true);
				}
			}

			//发送数据
			WORD wHeadSize=sizeof(SearchCorrespond)-sizeof(SearchCorrespond.UserRemoteInfo);
			WORD wItemSize=sizeof(SearchCorrespond.UserRemoteInfo[0])*SearchCorrespond.wUserCount;
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_REMOTE_SERVICE,SUB_CS_S_SEARCH_CORRESPOND,&SearchCorrespond,wHeadSize+wItemSize);

			return true;
		}
	default:
		break;
	}

	return false;
}

//管理服务
bool CAttemperEngineSink::OnTCPNetworkMainManagerService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_CS_C_SYSTEM_MESSAGE:	//系统消息
		{
			//发送通知
			m_pITCPNetworkEngine->SendDataBatch(MDM_CS_MANAGER_SERVICE,SUB_CS_S_SYSTEM_MESSAGE,pData,wDataSize,0L);
			return true;
		}
	case SUB_CS_C_PROPERTY_TRUMPET:  //喇叭消息
		{
			//发送通知
			m_pITCPNetworkEngine->SendDataBatch(MDM_CS_MANAGER_SERVICE,SUB_CS_S_PROPERTY_TRUMPET,pData,wDataSize,0L);
			return true;
		}
	}

	return false;
}


//发送列表
bool CAttemperEngineSink::SendServerListItem(DWORD dwSocketID)
{
	//变量定义
	WORD wPacketSize=0L;
	POSITION Position=NULL;
	BYTE cbBuffer[SOCKET_TCP_PACKET];

	//发送信息
	m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_INFO);

	//收集数据
	do
	{
		//发送数据
		if ((wPacketSize+sizeof(tagGameServer))>sizeof(cbBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_INSERT,cbBuffer,wPacketSize);
			wPacketSize=0;
		}

		//获取对象
		tagGameServer * pGameServer=(tagGameServer *)(cbBuffer+wPacketSize);
		CGlobalServerItem * pGlobalServerItem=m_GlobalInfoManager.EnumServerItem(Position);

		//设置数据
		if (pGlobalServerItem!=NULL)
		{
			wPacketSize+=sizeof(tagGameServer);
			CopyMemory(pGameServer,&pGlobalServerItem->m_GameServer,sizeof(tagGameServer));
		}

	} while (Position!=NULL);

	//发送数据
	if (wPacketSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_INSERT,cbBuffer,wPacketSize);

	SendMatchListItem(dwSocketID);

	//发送完成
	m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_FINISH);

	return true;
}

bool CAttemperEngineSink::SendMatchListItem(DWORD dwSocketID)
{
	//变量定义
	WORD wPacketSize=0L;
	POSITION Position=NULL;
	BYTE cbBuffer[SOCKET_TCP_PACKET] = {0};

	//发送信息
	//m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_INFO);

	//收集数据
	do
	{
		//发送数据
		if ((wPacketSize+sizeof(tagGameMatch))>sizeof(cbBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVICE_INFO,SUB_CS_S_MATCH_INSERT,cbBuffer,wPacketSize);
			wPacketSize=0;
		}

		//获取对象
		tagGameMatch * pGameServer=(tagGameMatch *)(cbBuffer+wPacketSize);
		CGlobalServerItem * pGlobalServerItem=m_GlobalInfoManager.EnumServerItem(Position);

		//设置数据
		if (pGlobalServerItem!=NULL)
		{
			wPacketSize+=sizeof(tagGameMatch);
			CopyMemory(pGameServer,&pGlobalServerItem->m_GameMatch,sizeof(tagGameMatch));
		}

	} while (Position!=NULL);

	//发送数据
	if (wPacketSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVICE_INFO,SUB_CS_S_MATCH_INSERT,cbBuffer,wPacketSize);

	//发送完成
	//m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_FINISH);

	return true;
}

//用户发送
bool CAttemperEngineSink::SendDataToUser(WORD wServerID, DWORD dwUserID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////////////
//定时消息发送
void CAttemperEngineSink::StartNextMsgTime(int index)
{
	if (m_pITimerEngine)
	{
		TCHAR szPath[MAX_PATH];
		GetCurrentDirectory(sizeof(szPath),szPath);
		_sntprintf(szPath,CountArray(szPath),TEXT("%s\\Settings\\Message%d.ini"), szPath, index);

		int minSec = GetPrivateProfileInt(TEXT("CONFIG"), TEXT("MIN_SEC"), 100, szPath);
		int maxSec = GetPrivateProfileInt(TEXT("CONFIG"), TEXT("MAX_SEC"), 200, szPath);
		
		int Sec = rand() % (maxSec - minSec + 1) + minSec;

		m_pITimerEngine->KillTimer(IDI_MESSAGE + index);
		m_pITimerEngine->SetTimer(IDI_MESSAGE + index, Sec * 1000, 1, 0);
	}
} // StartNextMsgTime

//发送消息
void CAttemperEngineSink::SendMsg(int iMsg)
{
	TCHAR szPath[MAX_PATH];
	GetCurrentDirectory(sizeof(szPath),szPath);
	_sntprintf(szPath,CountArray(szPath),TEXT("%s\\Settings\\Message%d.ini"), szPath, iMsg);

	int msgCnt = GetPrivateProfileInt(TEXT("MESSGE"), TEXT("COUNT"), 0, szPath);

	if (msgCnt == 0)
	{
		return;
	}

	int index = rand() % msgCnt + 1;
	TCHAR NameBuf[MAX_PATH] = {0};
	TCHAR OutBuf[MAX_PATH]  = {0};

	CMD_CS_S_SendGladMsg gladMsg;
	ZeroMemory(&gladMsg, sizeof(gladMsg));
	
	// 内容文本
	_sntprintf(NameBuf, CountArray(NameBuf), TEXT("MSG%d"), index);
	GetPrivateProfileString(TEXT("MESSGE"), NameBuf, TEXT(""), gladMsg.szContent, sizeof(gladMsg.szContent), szPath);
	// 内容颜色
	_sntprintf(NameBuf, CountArray(NameBuf), TEXT("COL_TEXT%d"), index);
	GetPrivateProfileString(TEXT("MESSGE"), NameBuf, TEXT("FFFFFF"), OutBuf, sizeof(OutBuf), szPath);
	_sntscanf(OutBuf, lstrlen(OutBuf), TEXT("%x"), &gladMsg.colText);
	gladMsg.colText = RGB(GetBValue(gladMsg.colText), GetGValue(gladMsg.colText), GetRValue(gladMsg.colText));
	// 名字颜色
	_sntprintf(NameBuf, CountArray(NameBuf), TEXT("COL_NAME%d"), index);
	GetPrivateProfileString(TEXT("MESSGE"), NameBuf, TEXT("FFFFFF"), OutBuf, sizeof(OutBuf), szPath);
	_sntscanf(OutBuf, lstrlen(OutBuf), TEXT("%x"), &gladMsg.colName);
	gladMsg.colName = RGB(GetBValue(gladMsg.colName), GetGValue(gladMsg.colName), GetRValue(gladMsg.colName));
	// 数字颜色
	_sntprintf(NameBuf, CountArray(NameBuf), TEXT("COL_NUM%d"), index);
	GetPrivateProfileString(TEXT("MESSGE"), NameBuf, TEXT("FFFFFF"), OutBuf, sizeof(OutBuf), szPath);
	_sntscanf(OutBuf, lstrlen(OutBuf), TEXT("%x"), &gladMsg.colNum);
	gladMsg.colNum = RGB(GetBValue(gladMsg.colNum), GetGValue(gladMsg.colNum), GetRValue(gladMsg.colNum));

	if (iMsg == 1)
	{
		// 数字文本
		_sntprintf(NameBuf, CountArray(NameBuf), TEXT("NUM%d"), index);
		GetPrivateProfileString(TEXT("MESSGE"), NameBuf, TEXT("0L"), gladMsg.szNum, sizeof(gladMsg.szNum), szPath);
		
		// 名字文本
		int nameCnt = GetPrivateProfileInt(TEXT("NAME"), TEXT("COUNT"), 0, szPath);

		if (nameCnt == 0)
		{
			return;
		}

		_sntprintf(NameBuf, CountArray(NameBuf), TEXT("NAME%d"), (rand() % nameCnt + 1));
		GetPrivateProfileString(TEXT("NAME"), NameBuf, TEXT("Ni***MK"), gladMsg.szNickName, sizeof(gladMsg.szNickName), szPath);
	}

	if (lstrlen(gladMsg.szContent) == 0)
	{
		return;
	}
	
	//m_pITCPNetworkEngine->SendDataBatch(MDM_CS_MANAGER_SERVICE,SUB_CS_S_GLAD_MESSAGE,&gladMsg, sizeof(gladMsg),0L);
} // SendMsg
