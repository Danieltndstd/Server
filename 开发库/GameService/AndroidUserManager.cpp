#include "StdAfx.h"
#include "AndroidUserManager.h"
#include "../../Server/GameServer/DataBasePacket.h"

//////////////////////////////////////////////////////////////////////////////////

//常量定义
#define ADNDROID_PLAY_DRAW          10                                  //游戏局数  

//时间标识
#define IDI_ANDROID_INOUT			(IDI_REBOT_MODULE_START+0)			//进出时间
#define IDI_ANDROID_PULSE			(IDI_REBOT_MODULE_START+1)			//脉冲时间

#define IDI_ANDROID_CHECK			(IDI_REBOT_MODULE_START+3)			//检测房间

//机器时间
#define TIME_ANDROID_INOUT			5L									//登录时间
#define TIME_ANDROID_PULSE			1L									//脉冲时间
#define TIME_LOAD_ANDROID_INFO		900L								//加载用户
#define TIME_ANDROID_REPOSE_TIME	1800L								//服务时间
#define TIME_ANDROID_CHECK			20L									//服务时间


//////////////////////////////////////////////////////////////////////////////////

//构造函数
CAndroidUserManager::CAndroidUserManager()
{
	//系统参数
	m_pGameParameter=NULL;
	m_pGameServiceAttrib=NULL;
	m_pGameServiceOption=NULL;

	//组件接口
	m_pITimerEngine=NULL;
	m_pIServerUserManager=NULL;
	m_pIGameServiceManager=NULL;
	m_pITCPNetworkEngineEvent=NULL;

	//配置变量
	m_wAutoAndroidCount=0;

	//库存变量
	m_wStockCount=0;
	ZeroMemory(&m_AndroidParameter,sizeof(m_AndroidParameter));

	m_ppITableFrame = NULL;

	//设置索引 奇数:容量的120%适合
	m_AndroidUserItemMap.InitHashTable(PRIME_ANDROID_USER);

	//m_bQuickTimer	= false;
	//m_dwRepeat		= 0;
	//m_dwcrtTimes	= 0;
	return;
}

//析构函数
CAndroidUserManager::~CAndroidUserManager()
{
	//清理断言
	ASSERT_ERR(m_AndroidUserItemMap.GetCount()==0L);
	ASSERT_ERR(m_AndroidUserItemArray.GetCount()==0L);
	ASSERT_ERR(m_AndroidUserItemBuffer.GetCount()==0L);

	return;
}

//接口查询
VOID * CAndroidUserManager::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserManager,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserManager,Guid,dwQueryVer);
	return NULL;
}

//启动服务
bool CAndroidUserManager::StartService()
{
	m_wAutoAndroidCount = m_pGameServiceOption->wAndroidCount;

	//机器人创建定时器,比赛方式不用
	if (m_pGameServiceOption->wServerType != GAME_GENRE_MATCH)
		m_pITimerEngine->SetTimer(IDI_ANDROID_INOUT,TIME_ANDROID_INOUT*1000L,TIMES_INFINITY,0);
	
	//机器人脉冲定时器	
	m_pITimerEngine->SetTimer(IDI_ANDROID_PULSE,TIME_ANDROID_PULSE*1000L,TIMES_INFINITY,0);

	//机器人检测房间(于基准线的波动) 普通房间
	//if (m_pGameServiceOption->wServerType != GAME_GENRE_MATCH)
	//	m_pITimerEngine->SetTimer(IDI_ANDROID_CHECK, TIME_ANDROID_CHECK * 1000l, TIMES_INFINITY, NULL);
	
	return true;
}


//停止服务
bool CAndroidUserManager::ConcludeService()
{
	//删除存储
	for (INT_PTR i=0;i<m_AndroidUserItemArray.GetCount();i++)
	{
		SafeRelease(m_AndroidUserItemArray[i]);
	}

	SafeDeleteArray(m_ppITableFrame);

	//清理数组
	m_AndroidUserItemMap.RemoveAll();
	m_AndroidUserItemArray.RemoveAll();
	m_AndroidUserItemBuffer.RemoveAll();

	//库存用户
	m_wStockCount=0;
	ZeroMemory(&m_AndroidParameter,sizeof(m_AndroidParameter));

	return true;
}

//VOID CAndroidUserManager::SetQuickJionTimer(DWORD dwRepeat)
//{
//	m_pITimerEngine->KillTimer(IDI_ANDROID_INOUT);
//	m_pITimerEngine->SetTimer(IDI_ANDROID_INOUT, TIME_ANDROID_INOUT, dwRepeat, 0);
//
//	m_dwcrtTimes	= 0;
//	m_dwRepeat		= dwRepeat;
//	m_bQuickTimer	= true;
//}
//
//VOID CAndroidUserManager::ResetJoinTimer()
//{
//	m_dwcrtTimes	= 0;
//	m_dwRepeat		= 0;
//	m_bQuickTimer	= false;
//	m_pITimerEngine->KillTimer(IDI_ANDROID_INOUT);
//	m_pITimerEngine->SetTimer(IDI_ANDROID_INOUT, TIME_ANDROID_INOUT*1000, TIMES_INFINITY, 0);
//}

//配置组件
bool CAndroidUserManager::InitAndroidUser(tagAndroidUserParameter & AndroidUserParameter)
{
	//设置变量
	m_pGameParameter=AndroidUserParameter.pGameParameter;
	m_pGameServiceAttrib=AndroidUserParameter.pGameServiceAttrib;
	m_pGameServiceOption=AndroidUserParameter.pGameServiceOption;

	//组件接口
	m_pITimerEngine=AndroidUserParameter.pITimerEngine;
	m_pIServerUserManager=AndroidUserParameter.pIServerUserManager;
	m_pIGameServiceManager=AndroidUserParameter.pIGameServiceManager;
	m_pITCPNetworkEngineEvent=AndroidUserParameter.pITCPNetworkEngineEvent;
	m_pIDataBaseEngine=AndroidUserParameter.pIDabaBaseEngine;

	CopyMemory(m_guiRoom, AndroidUserParameter.guiRoom, 40 * sizeof WCHAR);

	return true;
}

//设置库存
bool CAndroidUserManager::SetAndroidStock(tagAndroidParameter AndroidParameter[], WORD wStockCount)
{
	//设置变量
	//m_wStockCount=0;
	//ZeroMemory(m_AndroidParameter,sizeof(m_AndroidParameter));

	//设置用户
	for (WORD i=0;i<wStockCount;i++)
	{
		//数目效验
		//ASSERT_ERR(m_wStockCount<CountArray(m_AndroidParameter));
		if (m_wStockCount>=CountArray(m_AndroidParameter)) break;

		//设置变量
		CAndroidUserItem * pAndroidUserItem=NULL;
		m_AndroidUserItemMap.Lookup(AndroidParameter[i].dwUserID,pAndroidUserItem);

		//设置变量
		if (pAndroidUserItem==NULL)
		{
			m_AndroidParameter[m_wStockCount++]=AndroidParameter[i];
		}
	}

	return true;
}

//删除机器
bool CAndroidUserManager::DeleteAndroidUserItem(DWORD dwAndroidID)

{
	//获取对象
	WORD wIndex=LOWORD(dwAndroidID);
	CAndroidUserItem * pAndroidUserItem=GetAndroidUserItem(wIndex);


	//对象效验
	//ASSERT_ERR((pAndroidUserItem!=NULL)&&(pAndroidUserItem->m_wRoundID==HIWORD(dwAndroidID)));
	if ((pAndroidUserItem==NULL)||(pAndroidUserItem->m_wRoundID!=HIWORD(dwAndroidID))) return false;

	//关闭事件
	try
	{
		
		m_pITCPNetworkEngineEvent->OnEventTCPNetworkShut(dwAndroidID,0,0L);
	}
	catch (...)
	{
		//错误断言
		ASSERT_ERR(FALSE);
	}

	//删除对象
	FreeAndroidUserItem(pAndroidUserItem->GetUserID());

	return true;
}

//查找机器
IAndroidUserItem * CAndroidUserManager::SearchAndroidUserItem(DWORD dwUserID, DWORD dwContextID)
{
	//查找机器
	CAndroidUserItem * pAndroidUserItem=m_AndroidUserItemMap[dwUserID];

	//机器判断
	if (pAndroidUserItem!=NULL)
	{
		WORD wRoundID=pAndroidUserItem->m_wRoundID;
		WORD wAndroidIndex=pAndroidUserItem->m_wAndroidIndex;
		if (MAKELONG(wAndroidIndex,wRoundID)==dwContextID) return pAndroidUserItem;
	}

	return NULL;
}

//创建机器人
void CAndroidUserManager::CreateAndroidUserItem(DWORD dwCount)
{
	for (DWORD i = 0; i < dwCount; ++i)
	{
		OnEventTimerPulse(IDI_ANDROID_INOUT, NULL);
	}
}

//创建机器
IAndroidUserItem * CAndroidUserManager::CreateAndroidUserItem(tagAndroidParameter & AndroidParameter)
{
	//效验参数
	ASSERT_ERR(AndroidParameter.dwUserID!=0L);
	if (AndroidParameter.dwUserID==0L) return NULL;

	//创建对象
	CAndroidUserItem * pAndroidUserItem=ActiveAndroidUserItem(AndroidParameter);
	if (pAndroidUserItem==NULL) return NULL;

	//属性变量
	WORD wRoundID=pAndroidUserItem->m_wRoundID;
	WORD wAndroidIndex=pAndroidUserItem->m_wAndroidIndex;

	//连接模拟
	try
	{
		if (m_pITCPNetworkEngineEvent->OnEventTCPNetworkBind(MAKELONG(wAndroidIndex,wRoundID),0L)==false)
		{
			throw 0;
		}
	}
	catch (...)
	{
		//错误断言
		ASSERT_ERR(FALSE);

		//释放用户
		FreeAndroidUserItem(AndroidParameter.dwUserID);

		return NULL;
	}

	//变量定义
	CMD_GR_LogonUserID LogonUserID;
	ZeroMemory(&LogonUserID,sizeof(LogonUserID));

	//版本信息
	LogonUserID.dwPlazaVersion=VERSION_PLAZA;
	LogonUserID.dwFrameVersion=VERSION_FRAME;
	LogonUserID.dwProcessVersion=m_pGameServiceAttrib->dwClientVersion;

	//用户信息
	LogonUserID.dwUserID=AndroidParameter.dwUserID;
	lstrcpyn(LogonUserID.szPassword,TEXT(""),CountArray(LogonUserID.szPassword));
	LogonUserID.wKindID=m_pGameServiceOption->wKindID;

	//构造数据
	TCP_Command Command;
	Command.wMainCmdID=MDM_GR_LOGON;
	Command.wSubCmdID=SUB_GR_LOGON_USERID;

	//消息处理
	try
	{
		//发送数据
		if (m_pITCPNetworkEngineEvent->OnEventTCPNetworkRead(MAKELONG(wAndroidIndex,wRoundID),Command,&LogonUserID,sizeof(LogonUserID))==false)
		{
			//throw 0;
		}
	}
	catch (...) 
	{
		//错误断言
		ASSERT_ERR(FALSE);
		DISK_LOG(TEXT("游戏服务 释放机器人 2"));
		//删除机器
		DeleteAndroidUserItem(MAKELONG(wAndroidIndex,wRoundID));

		return NULL;
	}

	return pAndroidUserItem;
}

//脉冲事件
bool CAndroidUserManager::OnEventTimerPulse(DWORD dwTimerID, WPARAM dwBindParameter)
{
	switch (dwTimerID)
	{
	case IDI_ANDROID_INOUT:			//进出处理
		{
			//变量定义
			DWORD dwUserID=0L;
			INT_PTR nActiveCount=m_AndroidUserItemMap.GetCount();
			POSITION Position=m_AndroidUserItemMap.GetStartPosition();

			//获取时间
			SYSTEMTIME SystemTime;
			GetLocalTime(&SystemTime);
			DWORD dwTimeMask=(1L<<SystemTime.wHour);

			//变量定义
			bool bAllowAndroidAttend=CServerRule::IsAllowAndroidAttend(m_pGameServiceOption->dwServerRule);
			bool bAllowAndroidSimulate=CServerRule::IsAllowAndroidSimulate(m_pGameServiceOption->dwServerRule);

			//登录处理
			if ((bAllowAndroidAttend==true)||(bAllowAndroidSimulate==true))
			{
				//寻找机器
				for (WORD i=0;i<m_wStockCount;i++)
				{
					if ((m_AndroidParameter[i].dwServiceTime&dwTimeMask)!=0L)
					{
						//创建机器
						if (CreateAndroidUserItem(m_AndroidParameter[i])!=NULL)
						{
							//删除存存
							m_wStockCount--;
							m_AndroidParameter[i]=m_AndroidParameter[m_wStockCount];

							break;
						}
					}
				}

			}

			//变量定义
			DWORD dwCurrentTime=(DWORD)time(NULL);
			DWORD dwServerRule=m_pGameServiceOption->dwServerRule;

			//退出处理
			while (Position!=NULL)
			{
				//获取对象
				CAndroidUserItem * pAndroidUserItem=NULL;
				m_AndroidUserItemMap.GetNextAssoc(Position,dwUserID,pAndroidUserItem);

				//退出判断
				if (pAndroidUserItem!=NULL && pAndroidUserItem->m_pIServerUserItem!=NULL)
				{
					//变量定义
					IServerUserItem * pIServerUserItem=pAndroidUserItem->m_pIServerUserItem;
					tagAndroidParameter * pAndroidParameter=pAndroidUserItem->GetAndroidParameter();

					//服务状态
					BYTE cbUserStatus=pIServerUserItem->GetUserStatus();
					if ((cbUserStatus!=US_FREE)&&(cbUserStatus!=US_SIT)) continue;

					//服务时间
					DWORD dwLogonTime=pIServerUserItem->GetLogonTime();
					DWORD dwReposeTime=pAndroidUserItem->GetAndroidService()->dwReposeTime;

					//离开判断
					bool bLeaveServer=false;
					if ((dwLogonTime+dwReposeTime)<dwCurrentTime) bLeaveServer=true;
					if ((pAndroidParameter->dwServiceTime&dwTimeMask)==0L) bLeaveServer=true;
					if ((bAllowAndroidAttend==false)&&(bAllowAndroidSimulate==false)) bLeaveServer=true;

					//删除用户
					if (bLeaveServer==true)
					{
						//删除用户
						WORD wRoundID=pAndroidUserItem->m_wRoundID;
						WORD wAndroidIndex=pAndroidUserItem->m_wAndroidIndex;
						DeleteAndroidUserItem(MAKELONG(wAndroidIndex,wRoundID));

						DISK_LOG(TEXT("游戏服务 释放机器人 3"));
						return true;
					}
				}
			}

			return true;
		}
	case IDI_ANDROID_PULSE:		//用户脉冲
		{
			//变量定义
			DWORD dwUserID=0L;
			CAndroidUserItem * pAndroidUserItem=NULL;
			POSITION Position=m_AndroidUserItemMap.GetStartPosition();

			//用户处理
			while (Position!=NULL)
			{
				//获取对象
				m_AndroidUserItemMap.GetNextAssoc(Position,dwUserID,pAndroidUserItem);

				//时间处理
				try
				{
					ASSERT_ERR(pAndroidUserItem!=NULL);
					if ( pAndroidUserItem!=NULL && pAndroidUserItem->m_pIServerUserItem!=NULL) 
						pAndroidUserItem->OnTimerPulse(dwTimerID,dwBindParameter);
				}
				catch (...)
				{
					ASSERT_ERR(FALSE);
					DISK_LOG(TEXT("游戏服务 释放机器人 4"));
					DeleteAndroidUserItem(MAKELONG(pAndroidUserItem->m_wAndroidIndex,pAndroidUserItem->m_wRoundID));
				}
			}

			return true;
		}

	case IDI_ANDROID_CHECK:
		{
			int dwUserCount = 0;
			
			for (DWORD dwCount = 0; dwCount < m_pIServerUserManager->GetUserItemCount(); dwCount++)
			{
				IServerUserItem* pSerUserItem = m_pIServerUserManager->EnumUserItem(dwCount);

				if (!pSerUserItem->IsAndroidUser())
					dwUserCount++;
			}
			//需要的机器人数目
			dwUserCount = m_wAutoAndroidCount - dwUserCount;

			int nTotal = m_AndroidUserItemMap.GetCount() + m_wStockCount; //活动机器人+库存值(未创建机器人)


			if (dwUserCount < 0) dwUserCount = 0;

			if (nTotal > dwUserCount) //移除多余机器人
			{
				DWORD dwNeedMove = nTotal - dwUserCount;
				CString UserArray, temp;

				UserArray.Empty();

				for (int i = 0; i < dwNeedMove; ++i)
				{
					if (m_wStockCount == 0 || dwNeedMove == 0)
						break;

					tagAndroidParameter parameter = m_AndroidParameter[0];

					m_wStockCount--;
					m_AndroidParameter[0] = m_AndroidParameter[m_wStockCount];

					if (UserArray.IsEmpty())
						temp.Format(TEXT("%d"), parameter.dwUserID);
					else
						temp.Format(TEXT(",%d"), parameter.dwUserID);

					UserArray += temp;
					dwNeedMove--;
				}

				CAndroidUserItem * pAndroidUserItem=NULL;
				POSITION Position=m_AndroidUserItemMap.GetStartPosition();
				DWORD dwUserID = 0l;


				while (Position != NULL) //移除空闲机器人
				{
					m_AndroidUserItemMap.GetNextAssoc(Position, dwUserID, pAndroidUserItem);

					if (dwNeedMove == 0) break;

					IServerUserItem* pUser = pAndroidUserItem->GetMeUserItem();

					if (pAndroidUserItem != NULL && pUser != NULL && pUser->GetUserStatus()!= US_PLAYING)
					{
						WORD wRoundID=pAndroidUserItem->m_wRoundID;
						WORD wAndroidIndex=pAndroidUserItem->m_wAndroidIndex;
						if (pAndroidUserItem->GetTableID() != INVALID_TABLE)
							m_ppITableFrame[pAndroidUserItem->GetTableID()]->PerformStandUpAction(pAndroidUserItem->GetMeUserItem());

						DISK_LOG(TEXT("游戏服务 释放机器人 5"));
						DeleteAndroidUserItem(MAKELONG(wAndroidIndex,wRoundID));
						dwNeedMove--;
					}
				}

				if (UserArray.IsEmpty()) return true;

				DBR_GR_GameAndriodArray request = {0};
				CopyMemory(request.guidRoom, m_guiRoom, 40 * sizeof WCHAR);
				CopyMemory(request.szUserArray, UserArray.GetBuffer(), UserArray.GetLength());
				m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_ANDRIOD_ARRAY, 0l, (void*)&request, sizeof request);
			}
			else if (nTotal < dwUserCount) //申请少的机器人
			{
				DBR_GR_GameAndroidApply request = {0};
				CopyMemory(&request.guidRoom, m_guiRoom, 40 * sizeof WCHAR);
				request.dwCount = dwUserCount - nTotal;
				request.dwRoomID = m_pGameServiceOption->wServerID;

				m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_ANDRIOD_APPLY, 0l, &request, sizeof request);
			}
			
			return true;
		}

	}

	return false;
}

//用户状况
WORD CAndroidUserManager::GetAndroidUserInfo(tagAndroidUserInfo & AndroidUserInfo, DWORD dwServiceGender)
{
	//获取时间
	SYSTEMTIME SystemTime;
	GetLocalTime(&SystemTime);

	//设置变量
	ZeroMemory(&AndroidUserInfo,sizeof(AndroidUserInfo));

	//变量定义
	DWORD dwTimeMask=(1L<<SystemTime.wHour);
	POSITION Position=m_AndroidUserItemMap.GetStartPosition();

	//枚举对象
	while (Position!=NULL)
	{
		//获取对象
		DWORD dwUserID=0L;
		CAndroidUserItem * pAndroidUserItem=NULL;
		m_AndroidUserItemMap.GetNextAssoc(Position,dwUserID,pAndroidUserItem);

		//效验参数
		ASSERT_ERR((dwUserID!=0L)&&(pAndroidUserItem!=NULL));
		if ((dwUserID==0L)||(pAndroidUserItem==NULL)) break;

		//绑定判断
		if(pAndroidUserItem->m_pIServerUserItem==NULL) continue;

		//局数判断
		if(pAndroidUserItem->m_AndroidService.dwResidualPlayDraw==0) continue;

		//变量定义
		IServerUserItem * pIServerUserItem=pAndroidUserItem->m_pIServerUserItem;
		tagAndroidParameter * pAndroidParameter=pAndroidUserItem->GetAndroidParameter();

		//服务判断
		if ((pAndroidParameter->dwServiceTime&dwTimeMask)==0L) continue;
		if ((pAndroidParameter->dwServiceGender&dwServiceGender)==0L) continue;

		//状态判断
		switch (pIServerUserItem->GetUserStatus())
		{
		case US_FREE:
			{
				ASSERT_ERR(AndroidUserInfo.wFreeUserCount<CountArray(AndroidUserInfo.pIAndroidUserFree));
				AndroidUserInfo.pIAndroidUserFree[AndroidUserInfo.wFreeUserCount++]=pAndroidUserItem;
				//I_TRACE(TEXT("空闲机器人:%d"),AndroidUserInfo.wFreeUserCount);
				break;
			}
		case US_SIT:
		case US_READY:
			{
				ASSERT_ERR(AndroidUserInfo.wSitdownUserCount<CountArray(AndroidUserInfo.pIAndroidUserSitdown));
				AndroidUserInfo.pIAndroidUserSitdown[AndroidUserInfo.wSitdownUserCount++]=pAndroidUserItem;
				//I_TRACE(TEXT("坐下机器人:%d"),AndroidUserInfo.wFreeUserCount);
				break;
			}
		case US_PLAYING:
		case US_OFFLINE:
			{
				ASSERT_ERR(AndroidUserInfo.wPlayUserCount<CountArray(AndroidUserInfo.pIAndroidUserPlay));
				AndroidUserInfo.pIAndroidUserPlay[AndroidUserInfo.wPlayUserCount++]=pAndroidUserItem;
				//I_TRACE(TEXT("游戏机器人:%d"),AndroidUserInfo.wFreeUserCount);
				break;
			}
		}
	}

	return AndroidUserInfo.wFreeUserCount+AndroidUserInfo.wPlayUserCount+AndroidUserInfo.wSitdownUserCount;
}

//发送数据
bool CAndroidUserManager::SendDataToClient(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//发送数据
	for (INT_PTR i=0;i<m_AndroidUserItemArray.GetCount();i++)
	{
		//获取对象
		CAndroidUserItem * pAndroidUserItem=m_AndroidUserItemArray[i];

		//状态判断
		if (pAndroidUserItem->m_pIServerUserItem==NULL) continue;
		if (pAndroidUserItem->m_AndroidParameter.dwUserID==0L) continue;

		//消息处理
		try
		{
			if (pAndroidUserItem->OnSocketRead(wMainCmdID,wSubCmdID,pData,wDataSize)==false)
			{
				//throw 0;
			}
		}
		catch (...) 
		{
			//错误断言
			ASSERT_ERR(FALSE);

			DISK_LOG(TEXT("游戏服务 释放机器人 6"));
			//断开用户
			WORD wRoundID=pAndroidUserItem->m_wRoundID;
			WORD wAndroidIndex=pAndroidUserItem->m_wAndroidIndex;
			DeleteAndroidUserItem(MAKELONG(wAndroidIndex,wRoundID));
		}
	}

	return true;
}

//发送数据
bool CAndroidUserManager::SendDataToClient(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{

	//获取对象
	WORD wIndex=LOWORD(dwAndroidID);
	CAndroidUserItem * pAndroidUserItem=GetAndroidUserItem(wIndex);

	//对象效验
	//ASSERT_ERR((pAndroidUserItem!=NULL)&&(pAndroidUserItem->m_wRoundID==HIWORD(dwAndroidID)));
	if ((pAndroidUserItem==NULL)||(pAndroidUserItem->m_wRoundID!=HIWORD(dwAndroidID))) return false;

	//消息处理
	try
	{
		if (pAndroidUserItem->OnSocketRead(wMainCmdID,wSubCmdID,pData,wDataSize)==false)
		{
			//ASSERT_ERR(FALSE);
			//throw 0;
		}
	}
	catch (...) 
	{
		ASSERT_ERR(FALSE);
		DISK_LOG(TEXT("游戏服务 释放机器人 7"));
		DeleteAndroidUserItem(dwAndroidID);
	}

	return true;
}

//发送数据
bool CAndroidUserManager::SendDataToServer(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//构造数据
	TCP_Command Command;
	Command.wSubCmdID=wSubCmdID;
	Command.wMainCmdID=wMainCmdID;

	//消息处理
	try
	{
		if (m_pITCPNetworkEngineEvent->OnEventTCPNetworkRead(dwAndroidID,Command,pData,wDataSize)==false)
		{
			//throw 0;
		}
	}
	catch (...) 
	{
		ASSERT_ERR(FALSE);
		DISK_LOG(TEXT("游戏服务 释放机器人 8"));
		DeleteAndroidUserItem(dwAndroidID);
	}

	return true;
}

//获取对象
CAndroidUserItem * CAndroidUserManager::GetAndroidUserItem(WORD wIndex)
{
	//效验索引
	ASSERT_ERR(wIndex>=INDEX_ANDROID);
	if (wIndex<INDEX_ANDROID) return NULL;

	//效验索引
	ASSERT_ERR((wIndex-INDEX_ANDROID)<m_AndroidUserItemArray.GetCount());
	if (((wIndex-INDEX_ANDROID)>=m_AndroidUserItemArray.GetCount())) return NULL;

	//获取对象
	WORD wBufferIndex=wIndex-INDEX_ANDROID;
	CAndroidUserItem * pAndroidUserItem=m_AndroidUserItemArray[wBufferIndex];

	return pAndroidUserItem;
}


//释放对象
VOID CAndroidUserManager::FreeAndroidUserItem(DWORD dwUserID)
{
	//效验参数
	ASSERT_ERR(dwUserID!=0);
	if (dwUserID==0) return;

	//变量定义
	CAndroidUserItem * pAndroidUserItem=NULL;
	m_AndroidUserItemMap.Lookup(dwUserID,pAndroidUserItem);

	//对象判断
	if (pAndroidUserItem==NULL)
	{
		ASSERT_ERR(FALSE);
		return;
	}

	//复位对象
	pAndroidUserItem->RepositUserItem();

	//设置索引
	m_AndroidUserItemMap.RemoveKey(dwUserID);
	m_AndroidUserItemBuffer.Add(pAndroidUserItem);


	//if (m_AndroidUserItemArray[pAndroidUserItem->m_wAndroidIndex-INDEX_ANDROID]->GetUserID() == dwUserID)
	//	m_AndroidUserItemArray.RemoveAt(pAndroidUserItem->m_wAndroidIndex-INDEX_ANDROID);

	DBR_GR_GameAndriodRelease request;
	CopyMemory(&request.guidRoom, &m_guiRoom, 40 * sizeof WCHAR);
	request.dwUserId = dwUserID;

	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_ANDRIOD_RELEASE, 0l, (void*)&request, sizeof request);

	return;
}

//激活对象
CAndroidUserItem * CAndroidUserManager::ActiveAndroidUserItem(tagAndroidParameter & AndroidParameter)
{
	//变量定义
	CAndroidUserItem * pAndroidUserItem=NULL;
	INT_PTR nFreeItemCount=m_AndroidUserItemBuffer.GetCount();

	//获取对象
	if (nFreeItemCount>0)
	{
		//获取对象
		INT_PTR nItemPostion=nFreeItemCount-1;
		pAndroidUserItem=m_AndroidUserItemBuffer[nItemPostion];

		//删除数组
		m_AndroidUserItemBuffer.RemoveAt(nItemPostion);
		//m_AndroidUserItemArray.Add(pAndroidUserItem);
	}

	//创建对象
	if (pAndroidUserItem==NULL)
	{
		//数目判断
		if (m_AndroidUserItemArray.GetCount()>=MAX_ANDROID)
		{
			ASSERT_ERR(FALSE);
			return NULL;
		}

		try
		{
			//变量定义
			IAndroidUserItemSink * pIAndroidUserItemSink=NULL;
			
			//创建陪玩
			if (CServerRule::IsAllowAndroidAttend(m_pGameServiceOption->dwServerRule)==true)
			{
				pIAndroidUserItemSink=(IAndroidUserItemSink *)m_pIGameServiceManager->CreateAndroidUserItemSink(IID_IAndroidUserItemSink,VER_IAndroidUserItemSink);
			}

			//创建对象
			try
			{
				pAndroidUserItem=new CAndroidUserItem;
			}
			catch (...)
			{
				//错误断言
				ASSERT_ERR(FALSE);

				//删除对象
				SafeRelease(pIAndroidUserItemSink);
			}

			//错误判断
			if (pAndroidUserItem==NULL)
			{
				//错误断言
				ASSERT_ERR(FALSE);

				//删除对象
				SafeRelease(pIAndroidUserItemSink);

				return NULL;
			}

			//设置用户
			if ((pIAndroidUserItemSink!=NULL)&&(pIAndroidUserItemSink->Initialization(pAndroidUserItem)==false))
			{
				//错误断言
				ASSERT_ERR(FALSE);

				//删除对象
				SafeRelease(pAndroidUserItem);
				SafeRelease(pIAndroidUserItemSink);

				return NULL;
			}

			//设置数组
			WORD wCurrentIndex=(WORD)m_AndroidUserItemArray.Add(pAndroidUserItem);

			//设置接口
			pAndroidUserItem->m_wAndroidIndex=wCurrentIndex+INDEX_ANDROID;
			pAndroidUserItem->m_pIServerUserManager=m_pIServerUserManager;
			pAndroidUserItem->m_pIAndroidUserItemSink=pIAndroidUserItemSink;
			pAndroidUserItem->m_pIAndroidUserManager=QUERY_ME_INTERFACE(IAndroidUserManager);
		}
		catch (...) 
		{ 
			ASSERT_ERR(FALSE);
			return NULL; 
		}
	}

	//服务时间
	if ((AndroidParameter.dwMaxReposeTime!=0L)&&(AndroidParameter.dwMinReposeTime!=0L))
	{
		//变量定义
		DWORD dwMaxReposeTime=AndroidParameter.dwMaxReposeTime;
		DWORD dwMinReposeTime=AndroidParameter.dwMinReposeTime;

		//调整时间
		if ((dwMaxReposeTime-dwMinReposeTime)>0L)
			pAndroidUserItem->m_AndroidService.dwReposeTime=dwMinReposeTime+rand()%(dwMaxReposeTime-dwMinReposeTime);
		else
			pAndroidUserItem->m_AndroidService.dwReposeTime=dwMinReposeTime;
	}
	else
		pAndroidUserItem->m_AndroidService.dwReposeTime=TIME_ANDROID_REPOSE_TIME;

	//游戏局数
	if((AndroidParameter.dwMinPlayDraw!=0L)&&(AndroidParameter.dwMaxPlayDraw!=0L))
	{
		//变量定义
		DWORD dwMaxPlayDraw=AndroidParameter.dwMaxPlayDraw;
		DWORD dwMinPlayDraw=AndroidParameter.dwMinPlayDraw;

		//调整局数
		if ((dwMaxPlayDraw-dwMinPlayDraw)>0L)
			pAndroidUserItem->m_AndroidService.dwResidualPlayDraw=dwMinPlayDraw+rand()%(dwMaxPlayDraw-dwMinPlayDraw);
		else
			pAndroidUserItem->m_AndroidService.dwResidualPlayDraw=dwMinPlayDraw;
	}
	else
		pAndroidUserItem->m_AndroidService.dwResidualPlayDraw=ADNDROID_PLAY_DRAW;

	//服务类型
	if (pAndroidUserItem->m_pIAndroidUserItemSink == NULL && (AndroidParameter.dwServiceGender&ANDROID_SIMULATE) != 0)
		AndroidParameter.dwServiceGender = ANDROID_SIMULATE;
		

	//设置变量
	pAndroidUserItem->m_AndroidParameter=AndroidParameter;

	//设置索引
	m_AndroidUserItemMap[AndroidParameter.dwUserID]=pAndroidUserItem;

	return pAndroidUserItem;
}

//初始化桌子
bool CAndroidUserManager::InitTableFrame(ITableFrame * pTableFrame,WORD wTableID)
{
	if (m_ppITableFrame==NULL) 
	{
		m_ppITableFrame = new ITableFrame*[m_pGameServiceOption->wTableCount];
		ZeroMemory(m_ppITableFrame, sizeof(ITableFrame*)*m_pGameServiceOption->wTableCount);
	}

	if(pTableFrame==NULL || m_ppITableFrame==NULL ||
		wTableID>m_pGameServiceOption->wTableCount)
	{
		ASSERT_ERR(false);
		return false;
	}
	m_ppITableFrame[wTableID]=pTableFrame;

	return true;
}


//
//获取机器人用户
//IAndroidUserItem* CAndroidUserManager::GetAndroidUser(DWORD dwUserID)
//{
//	CAndroidUserItem* pUser = nullptr;
//	m_AndroidUserItemMap.Lookup(dwUserID, pUser);
//
//	return pUser;
//}


//////////////////////////////////////////////////////////////////////////////////
