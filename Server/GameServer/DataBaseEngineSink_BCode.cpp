#include "StdAfx.h"
#include "ServiceUnits.h"
#include "DataBaseEngineSink.h"

void CDataBaseEngineSink::RegisterRequestEvent_BCode()
{
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_SCROLL_MESSAGE, CDataBaseEngineSink::OnLoadScrollMessageNew, this);		//������Ϣ��ȡ
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_LOAD_TIP_MESSAGE, CDataBaseEngineSink::OnLoadTipMessage, this);		//��ȡ����
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_LOAD_SCROLL_MESSAGE, CDataBaseEngineSink::OnLoadScrollMessage, this);		//��ȡ������Ϣ
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_CLEAR_SCROLL_MESSAGE, CDataBaseEngineSink::OnClearScrollMessage, this);		//���������Ϣ 
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_GET_FIRST_REWARD_STATUS, CDataBaseEngineSink::OnGetFirstRewardStatus, this);		//��ȡһ�ֽ���״̬ 
}


//��ȡ��ɵ�һ����Ϸ����
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

			//�ƶ���¼
			m_AccountsDBModule->MoveToNext();
		}
	}

	PROCESS_END()
}
// ������Ϣ��ȡ
bool CDataBaseEngineSink::OnLoadScrollMessageNew(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	PROCESS_BEGIN()

	try
	{
	
		// ��ѯ���޴����Ĺ�����Ϣ
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

			

				// ���Ķ�ȡ����
				m_TreasureDBAide.ResetParameter();
				m_TreasureDBAide.AddParameter(TEXT("@dwID"), ScrollMessage.dwID);
				m_TreasureDBAide.AddParameter(TEXT("@dwServerID"), m_pGameServiceOption->wServerID);
				m_TreasureDBAide.ExecuteProcess(TEXT("QP_P_ReadScrollMessage_Time"), true);

				// ������Ϣ
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_SCROLL_MESSAGE, dwContextID, &ScrollMessage, sizeof(DBO_GR_LoadScrollMessage));

				// �ƶ���¼
				m_PlatformDBModule->MoveToNext();
			}
		}

		// ��ѯ����������Ϣ
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

				
				//���Ķ�ȡ����
				m_TreasureDBAide.ResetParameter();
				m_TreasureDBAide.AddParameter(TEXT("@dwID"), ScrollMessage.dwID);
				m_TreasureDBAide.AddParameter(TEXT("@dwServerID"), m_pGameServiceOption->wServerID);
				m_TreasureDBAide.ExecuteProcess(TEXT("QP_P_ReadScrollMessage_Time"), true);

				// ������Ϣ
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_SCROLL_MESSAGE, dwContextID, &ScrollMessage, sizeof(DBO_GR_LoadScrollMessage));

				//�ƶ���¼
				m_PlatformDBModule->MoveToNext();
			}
		}
	}
	catch (IDataBaseException* pIException)
	{
		//�������
		TRACE_LOG(pIException->GetExceptionDescribe(), TraceLevel_Exception);
		TRACE_LOG(TEXT("OnLogonDisposeResult Error"), TraceLevel_Exception);

		OnLogonDisposeResult(dwContextID, DB_ERROR, TEXT("�������ݿ�����쳣�������Ժ����Ի���ѡ����һ��������¼"), false);

		return true;
	}
	PROCESS_END()
}
//��ȡ����
bool CDataBaseEngineSink::OnLoadTipMessage(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	PROCESS_BEGIN()

	m_PlatformDBAide.ResetParameter();
	LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("QP_P_LoadTipMessage"),true);

	map<DWORD,DBO_GR_TipMessage> & mTipMessage = CDataCenter::Instance()->m_mTipMessage;

	//�����Ϣ
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

			//�ƶ���¼
			m_PlatformDBModule->MoveToNext();
		}
	}

	PROCESS_END()
}

//��ȡ������Ϣ
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

//���������Ϣ
bool CDataBaseEngineSink::OnClearScrollMessage(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	PROCESS_BEGIN()

	m_PlatformDBAide.ResetParameter();
	LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("QP_P_ClearScrollMessage"),false);

	PROCESS_END()
}


//��ȡһ�ֽ���״̬
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

