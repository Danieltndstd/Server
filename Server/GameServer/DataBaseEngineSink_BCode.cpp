#include "StdAfx.h"
#include "ServiceUnits.h"
#include "DataBaseEngineSink.h"

void CDataBaseEngineSink::RegisterRequestEvent_BCode()
{
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_SCROLL_MESSAGE, CDataBaseEngineSink::OnLoadScrollMessageNew, this);		//滚动消息读取
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_LOAD_TIP_MESSAGE, CDataBaseEngineSink::OnLoadTipMessage, this);		//读取弹窗
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_LOAD_SCROLL_MESSAGE, CDataBaseEngineSink::OnLoadScrollMessage, this);		//读取滚动消息
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_CLEAR_SCROLL_MESSAGE, CDataBaseEngineSink::OnClearScrollMessage, this);		//清除滚动消息 
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_GET_FIRST_REWARD_STATUS, CDataBaseEngineSink::OnGetFirstRewardStatus, this);		//获取一局奖励状态 
}


//读取完成第一局游戏奖励
bool CDataBaseEngineSink::OnLoadFinishedFirstGameRreward(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	PROCESS_BEGIN()

	m_AccountsDBAide.ResetParameter();
	LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_GetFinishedFirstGameRreward"),true);

	if(DB_SUCCESS == lResultCode)
	{
		while(m_AccountsDBModule->IsRecordsetEnd() == false)
		{
			DWORD & dwFinishedFristGameReward = CDataCenter::Instance()->dwFinishedFristGameReward;

			dwFinishedFristGameReward = m_AccountsDBAide.GetValue_DWORD(TEXT("dwFinishedFirstGameRreward"));

			//移动记录
			m_AccountsDBModule->MoveToNext();
		}
	}

	PROCESS_END()
}
// 滚动消息读取
bool CDataBaseEngineSink::OnLoadScrollMessageNew(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	PROCESS_BEGIN()

	try
	{
	
		// 查询无限次数的滚动消息
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.AddParameter(TEXT("@dwKindID"), m_pGameServiceOption->wKindID);
		m_PlatformDBAide.AddParameter(TEXT("@dwServerID"), m_pGameServiceOption->wServerID);
		LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("QP_P_LoadScrollMessage_Time"), true);

		if (DB_SUCCESS == lResultCode)
		{
			while (m_PlatformDBModule->IsRecordsetEnd() == false)
			{
				DBO_GR_LoadScrollMessage ScrollMessage;
				ZeroMemory(&ScrollMessage, sizeof(ScrollMessage));
				ScrollMessage.dwID = m_PlatformDBAide.GetValue_DWORD(TEXT("ID"));
				ScrollMessage.dwMessageType = m_PlatformDBAide.GetValue_DWORD(TEXT("MessageType"));
				m_PlatformDBAide.GetValue_String(TEXT("MessageString"), ScrollMessage.szScrollMessage, CountArray(ScrollMessage.szScrollMessage));

			

				// 更改读取配置
				m_TreasureDBAide.ResetParameter();
				m_TreasureDBAide.AddParameter(TEXT("@dwID"), ScrollMessage.dwID);
				m_TreasureDBAide.AddParameter(TEXT("@dwServerID"), m_pGameServiceOption->wServerID);
				m_TreasureDBAide.ExecuteProcess(TEXT("QP_P_ReadScrollMessage_Time"), true);

				// 发送消息
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_SCROLL_MESSAGE, dwContextID, &ScrollMessage, sizeof(DBO_GR_LoadScrollMessage));

				// 移动记录
				m_PlatformDBModule->MoveToNext();
			}
		}

		// 查询次数滚动消息
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.AddParameter(TEXT("@dwKindID"), m_pGameServiceOption->wKindID);
		m_PlatformDBAide.AddParameter(TEXT("@dwServerID"), m_pGameServiceOption->wServerID);
		lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("QP_P_LoadScrollMessage_Num"), true);

		if (DB_SUCCESS == lResultCode)
		{
			while (m_PlatformDBModule->IsRecordsetEnd() == false)
			{
				DBO_GR_LoadScrollMessage ScrollMessage;
				ZeroMemory(&ScrollMessage, sizeof(ScrollMessage));
				ScrollMessage.dwID = m_PlatformDBAide.GetValue_DWORD(TEXT("ID"));
				ScrollMessage.dwMessageType = m_PlatformDBAide.GetValue_DWORD(TEXT("MessageType"));
				m_PlatformDBAide.GetValue_String(TEXT("MessageString"), ScrollMessage.szScrollMessage, CountArray(ScrollMessage.szScrollMessage));

				
				//更改读取配置
				m_TreasureDBAide.ResetParameter();
				m_TreasureDBAide.AddParameter(TEXT("@dwID"), ScrollMessage.dwID);
				m_TreasureDBAide.AddParameter(TEXT("@dwServerID"), m_pGameServiceOption->wServerID);
				m_TreasureDBAide.ExecuteProcess(TEXT("QP_P_ReadScrollMessage_Time"), true);

				// 发送消息
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_SCROLL_MESSAGE, dwContextID, &ScrollMessage, sizeof(DBO_GR_LoadScrollMessage));

				//移动记录
				m_PlatformDBModule->MoveToNext();
			}
		}
	}
	catch (IDataBaseException* pIException)
	{
		//输出错误
		TRACE_LOG(pIException->GetExceptionDescribe(), TraceLevel_Exception);
		TRACE_LOG(TEXT("OnLogonDisposeResult Error"), TraceLevel_Exception);

		OnLogonDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试或者选择另一服务器登录"), false);

		return true;
	}
	PROCESS_END()
}
//读取弹窗
bool CDataBaseEngineSink::OnLoadTipMessage(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	PROCESS_BEGIN()

	m_PlatformDBAide.ResetParameter();
	LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("QP_P_LoadTipMessage"),true);

	map<DWORD,DBO_GR_TipMessage> & mTipMessage = CDataCenter::Instance()->m_mTipMessage;

	//清空消息
	mTipMessage.clear();

	if(DB_SUCCESS == lResultCode)
	{
		while(m_PlatformDBModule->IsRecordsetEnd() == false)
		{
			DBO_GR_TipMessage tipMessage = {0};
			DWORD dwID = m_PlatformDBAide.GetValue_DWORD(TEXT("dwID"));

			m_PlatformDBAide.GetValue_SystemTime(TEXT("startTime"),tipMessage.startTime);
			m_PlatformDBAide.GetValue_SystemTime(TEXT("concludeTime"),tipMessage.endTime);

			m_PlatformDBAide.GetValue_String(TEXT("szMessageString"),tipMessage.szSystemMessage,CountArray(tipMessage.szSystemMessage));

			mTipMessage[dwID] = tipMessage;

			//移动记录
			m_PlatformDBModule->MoveToNext();
		}
	}

	PROCESS_END()
}

//读取滚动消息
bool CDataBaseEngineSink::OnLoadScrollMessage(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	PROCESS_BEGIN()
		
	DWORD dwScrollType;

	srand(time(NULL));
	dwScrollType = rand() % 4;
	if (dwScrollType == 0)
	{
		dwScrollType = 1;
	}

	m_PlatformDBAide.ResetParameter();
	m_PlatformDBAide.AddParameter(TEXT("@dwScrollType"),dwScrollType);
	m_PlatformDBAide.AddParameter(TEXT("@dwServerID"),m_pGameServiceOption->wServerID);
	LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("QP_P_LoadScrollMessage"),true);

	if(DB_SUCCESS == lResultCode)
	{
		while(m_PlatformDBModule->IsRecordsetEnd() == false)
		{
			DBO_GR_ScrollMessage pScrollMessage = {0};

			m_PlatformDBAide.GetValue_String(TEXT("szScrollMessage"),pScrollMessage.szScrollMessage,CountArray(pScrollMessage.szScrollMessage));

			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_LOAD_SCROLL_MESSAGE,dwContextID,&pScrollMessage,sizeof(DBO_GR_ScrollMessage));

			break;
		}
	}

	PROCESS_END()
}

//清除滚动消息
bool CDataBaseEngineSink::OnClearScrollMessage(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	PROCESS_BEGIN()

	m_PlatformDBAide.ResetParameter();
	LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("QP_P_ClearScrollMessage"),false);

	PROCESS_END()
}


//获取一局奖励状态
bool CDataBaseEngineSink::OnGetFirstRewardStatus(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	PROCESS_BEGIN()

	DWORD * dwUserID = (DWORD*)pData; 

	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),*dwUserID);
	LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_GetFirstRewardStatus"),false);
	
	if ( DB_SUCCESS == lResultCode )
	{
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_GET_FIRST_REWARD_STATUS,dwContextID,NULL,0);
	}

	PROCESS_END()
}

