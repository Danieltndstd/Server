#include "StdAfx.h"
#include "ServiceUnits.h"
#include "ControlPacket.h"
#include "AttemperEngineSink.h"
//////////////////////////////////////////////////////////////////////////
#define IDI_MESSAGE				10086
#define IDI_BACKUP				20086
#define IDI_BACKUP_RESET		30086
#define IDI_CHECK_EXPIRE_RED	40000		//�����ں��
#define IDI_CHECK_RESIDUE_RED	40001		//��黹��ʣ��ĺ��

//
#define IDT_BACKUP_DAY_TIME		86400
#define IDT_BACKUP_RESET_TIME	60

//////////////////////////////////////////////////////////////////////////////////

//�����¼�
bool CAttemperEngineSink::OnAttemperEngineBegin(IUnknownEx * pIUnknownEx)
{
	return true;
}

//�����¼�
bool CAttemperEngineSink::OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize)
{
	return false;
}

//�����¼�
bool CAttemperEngineSink::OnEventAttemperData(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}

//ʱ���¼�
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
			////I_TRACE(TEXT("%d�����б��ݼ��"), left);
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
			//I_TRACE(TEXT("%d�����в������а񱸷ݼ��"), left);
		}
		return true;
	
	}
	return false;
}

//��ȡ�¼�
bool CAttemperEngineSink::OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (Command.wMainCmdID)
	{
	case MDM_CS_REGISTER:		//����ע��
		{
			return OnTCPNetworkMainRegister(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_CS_SERVICE_INFO:	//������Ϣ
		{
			return OnTCPNetworkMainServiceInfo(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_CS_USER_COLLECT:	//�û�����
		{
			return OnTCPNetworkMainUserCollect(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_CS_REMOTE_SERVICE:	//Զ�̷���
		{
			return OnTCPNetworkMainRemoteService(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_CS_MANAGER_SERVICE: //�������
		{
			return OnTCPNetworkMainManagerService(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_CS_MATCH_SERVICE:	//��������
		{
			return true;
		}
	}

	return false;
}

//�ر��¼�
bool CAttemperEngineSink::OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID)
{
	//��ȡ��Ϣ
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	bool isGameKind = false;

	//��Ϸ����
	if (pBindParameter->ServiceKind==ServiceKind_Game)
	{
		//��������
		WORD wBindIndex=LOWORD(dwSocketID);

		//���ܴ���
		if (wBindIndex==m_wCollectItem)
		{
			//���ñ���
			m_wCollectItem=INVALID_WORD;

			//�����л�
			if (m_WaitCollectItemArray.GetCount()>0)
			{
				//��ȡ����
				INT_PTR nWaitCount=m_WaitCollectItemArray.GetCount();
				m_wCollectItem=m_WaitCollectItemArray[nWaitCount-1];

				//ɾ������
				m_WaitCollectItemArray.RemoveAt(nWaitCount-1);

				//������Ϣ
				DWORD dwSocketID=(m_pBindParameter+m_wCollectItem)->dwSocketID;
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_USER_COLLECT,SUB_CS_S_COLLECT_REQUEST);
			}
		}
		else
		{
			//ɾ���ȴ�
			for (INT_PTR i=0;i<m_WaitCollectItemArray.GetCount();i++)
			{
				if (wBindIndex==m_WaitCollectItemArray[i])
				{
					m_WaitCollectItemArray.RemoveAt(i);
					break;
				}
			}
		}

		//��������
		CMD_CS_S_ServerRemove ServerRemove;
		ZeroMemory(&ServerRemove,sizeof(ServerRemove));

		//ɾ��֪ͨ
		ServerRemove.wServerID=pBindParameter->wServiceID;
		m_pITCPNetworkEngine->SendDataBatch(MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_REMOVE,&ServerRemove,sizeof(ServerRemove),0L);

		//ע������
		m_GlobalInfoManager.DeleteServerItem(pBindParameter->wServiceID);

		isGameKind = true;
	}

	//�㳡����
	if (pBindParameter->ServiceKind==ServiceKind_Plaza)
	{
		//��������
		WORD wBindIndex=LOWORD(dwSocketID);
		tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

		//ע������
		m_GlobalInfoManager.DeletePlazaItem(pBindParameter->wServiceID);
	}

	//�����Ϣ
	ZeroMemory(pBindParameter,sizeof(tagBindParameter));


	BYTE * pClientAddr=(BYTE *)&dwClientAddr;
	//if (isGameKind)
	{
		//I_TRACE(TEXT("%d.%d.%d.%d %s�Ͽ�����"), pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3], isGameKind ? TEXT("���������") : TEXT("��½������"));
	}
	return false;
}

//ע�����
bool CAttemperEngineSink::OnTCPNetworkMainRegister(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_CS_C_REGISTER_PLAZA:	//ע��㳡
		{
			//Ч������
			ASSERT_ERR(wDataSize==sizeof(CMD_CS_C_RegisterPlaza));
			if (wDataSize!=sizeof(CMD_CS_C_RegisterPlaza)) return false;

			//��Ϣ����
			CMD_CS_C_RegisterPlaza * pRegisterPlaza=(CMD_CS_C_RegisterPlaza *)pData;

			//��Ч�ж�
			if ((pRegisterPlaza->szServerName[0]==0)||(pRegisterPlaza->szServerAddr[0]==0))
			{
				//��������
				CMD_CS_S_RegisterFailure RegisterFailure;
				ZeroMemory(&RegisterFailure,sizeof(RegisterFailure));

				//���ñ���
				RegisterFailure.lErrorCode=0L;
				lstrcpyn(RegisterFailure.szDescribeString,TEXT("������ע��ʧ�ܣ��������ַ���롰�������������Ϸ���"),CountArray(RegisterFailure.szDescribeString));

				//������Ϣ
				WORD wStringSize=CountStringBuffer(RegisterFailure.szDescribeString);
				WORD wSendSize=sizeof(RegisterFailure)-sizeof(RegisterFailure.szDescribeString)+wStringSize;
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_REGISTER,SUB_CS_S_REGISTER_FAILURE,&RegisterFailure,wSendSize);

				//�ж�����
				m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);

				return true;
			}

			//���ð�
			WORD wBindIndex=LOWORD(dwSocketID);
			(m_pBindParameter+wBindIndex)->wServiceID=wBindIndex;
			(m_pBindParameter+wBindIndex)->ServiceKind=ServiceKind_Plaza;

			//��������
			tagGamePlaza GamePlaza;
			ZeroMemory(&GamePlaza,sizeof(GamePlaza));

			//��������
			GamePlaza.wPlazaID=wBindIndex;
			lstrcpyn(GamePlaza.szServerName,pRegisterPlaza->szServerName,CountArray(GamePlaza.szServerName));
			lstrcpyn(GamePlaza.szServerAddr,pRegisterPlaza->szServerAddr,CountArray(GamePlaza.szServerAddr));

			//ע�᷿��
			m_GlobalInfoManager.ActivePlazaItem(wBindIndex,GamePlaza);

			//�����б�
			SendServerListItem(dwSocketID);

			//Ⱥ������
			m_pITCPNetworkEngine->AllowBatchSend(dwSocketID,true,0L);

			//���ӳɹ���ʾ
			BYTE * pClientAddr=(BYTE *)&(m_pBindParameter+wBindIndex)->dwClientAddr;
			//I_TRACE(TEXT("%d.%d.%d.%d ��½������ע��ɹ�"), pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);
			//g_Log.LogToFile(LOG_ROOT, TEXT("%d.%d.%d.%d ��½������ע��ɹ�"), pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

			return true;
		}
	case SUB_CS_C_REGISTER_SERVER:	//ע�᷿��
		{
			//Ч������
			ASSERT_ERR(wDataSize==sizeof(CMD_CS_C_RegisterServer));
			if (wDataSize!=sizeof(CMD_CS_C_RegisterServer)) return false;

			//��Ϣ����
			CMD_CS_C_RegisterServer * pRegisterServer=(CMD_CS_C_RegisterServer *)pData;

			//���ҷ���
			if (m_GlobalInfoManager.SearchServerItem(pRegisterServer->wServerID)!=NULL)
			{
				//��������
				CMD_CS_S_RegisterFailure RegisterFailure;
				ZeroMemory(&RegisterFailure,sizeof(RegisterFailure));

				//���ñ���
				RegisterFailure.lErrorCode=0L;
				lstrcpyn(RegisterFailure.szDescribeString,TEXT("�Ѿ�������ͬ��ʶ����Ϸ������񣬷������ע��ʧ��"),CountArray(RegisterFailure.szDescribeString));

				//������Ϣ
				WORD wStringSize=CountStringBuffer(RegisterFailure.szDescribeString);
				WORD wSendSize=sizeof(RegisterFailure)-sizeof(RegisterFailure.szDescribeString)+wStringSize;
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_REGISTER,SUB_CS_S_REGISTER_FAILURE,&RegisterFailure,wSendSize);

				//�ж�����
				m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);

				return true;
			}

			//���ð�
			WORD wBindIndex=LOWORD(dwSocketID);
			(m_pBindParameter+wBindIndex)->ServiceKind=ServiceKind_Game;
			(m_pBindParameter+wBindIndex)->wServiceID=pRegisterServer->wServerID;

			//��������
			tagGameServer GameServer;
			ZeroMemory(&GameServer,sizeof(GameServer));

			//��������
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
			GameServer.lCellScore = pRegisterServer->lCellScore;	//��Ϸ����
			GameServer.cbRoomType = pRegisterServer->cbRoomType;

			//��������
			GameServer.lRestrictScore	= pRegisterServer->lRestrictScore	;						//���ƻ���
			GameServer.lMinTableScore	= pRegisterServer->lMinTableScore	;						//��ͻ���
			GameServer.lMinEnterScore	= pRegisterServer->lMinEnterScore	;						//��ͻ���
			GameServer.lMaxEnterScore	= pRegisterServer->lMaxEnterScore	;						//��߻���
			GameServer.lMinEnterTiLi	= pRegisterServer->lMinEnterTiLi	;						//�������

			GameServer.wCardBoxIndex	= pRegisterServer->wCardBoxIndex	;						//��������

			//ע�᷿��
			m_GlobalInfoManager.ActiveServerItem(wBindIndex,GameServer);

			//Ⱥ������
			m_pITCPNetworkEngine->AllowBatchSend(dwSocketID,true,0L);

			//Ⱥ������
			m_pITCPNetworkEngine->SendDataBatch(MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_INSERT,&GameServer,sizeof(GameServer),0L);

			//�����б� 
			SendServerListItem(dwSocketID);		

			//����֪ͨ
			if (m_wCollectItem==INVALID_WORD)
			{
				m_wCollectItem=wBindIndex;
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_USER_COLLECT,SUB_CS_S_COLLECT_REQUEST);
			}
			else m_WaitCollectItemArray.Add(wBindIndex);

			BYTE * pClientAddr=(BYTE *)&(m_pBindParameter+wBindIndex)->dwClientAddr;
			//I_TRACE(TEXT("%d.%d.%d.%d [%s]ע��ɹ�"), pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3],GameServer.szServerName);

			return true;
		}
	case SUB_CS_C_REGISTER_MATCH:
		{			
			//Ч������
			ASSERT_ERR(wDataSize==sizeof(CMD_CS_C_RegisterMatch));
			if (wDataSize!=sizeof(CMD_CS_C_RegisterMatch)) return false;

			//��Ϣ����
			CMD_CS_C_RegisterMatch * pRegisterMatch=(CMD_CS_C_RegisterMatch *)pData;

			return true;
		}
	}

	return false;
}

//����״̬
bool CAttemperEngineSink::OnTCPNetworkMainServiceInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_CS_C_SERVER_ONLINE:	//��������
		{
			//Ч������
			ASSERT_ERR(wDataSize==sizeof(CMD_CS_C_ServerOnLine));
			if (wDataSize!=sizeof(CMD_CS_C_ServerOnLine)) return false;

			//��Ϣ����
			CMD_CS_C_ServerOnLine * pServerOnLine=(CMD_CS_C_ServerOnLine *)pData;

			//��ȡ����
			WORD wBindIndex=LOWORD(dwSocketID);
			tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

			//����Ч��
			ASSERT_ERR(pBindParameter->ServiceKind==ServiceKind_Game);
			if (pBindParameter->ServiceKind!=ServiceKind_Game) return false;

			//���ҷ���
			WORD wServerID=pBindParameter->wServiceID;
			CGlobalServerItem * pGlobalServerItem=m_GlobalInfoManager.SearchServerItem(wServerID);

			//��������
			if (pGlobalServerItem!=NULL)
			{
				//��������
				CMD_CS_S_ServerOnLine ServerOnLine;
				ZeroMemory(&ServerOnLine,sizeof(ServerOnLine));

				//���ñ���
				pGlobalServerItem->m_GameServer.dwOnLineCount=pServerOnLine->dwOnLineCount;

				//���ñ���
				ServerOnLine.wServerID=wServerID;
				ServerOnLine.dwOnLineCount=pServerOnLine->dwOnLineCount;

				//����֪ͨ
				m_pITCPNetworkEngine->SendDataBatch(MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_ONLINE,&ServerOnLine,sizeof(ServerOnLine),0L);
			}

			return true;
		}
	case SUB_CS_C_SERVER_MODIFY:	//�����޸�
		{
			//Ч������
			ASSERT_ERR(wDataSize==sizeof(CMD_CS_C_ServerModify));
			if (wDataSize!=sizeof(CMD_CS_C_ServerModify)) return false;

			//��Ϣ����
			CMD_CS_C_ServerModify * pServerModify=(CMD_CS_C_ServerModify *)pData;

			//��ȡ����
			WORD wBindIndex=LOWORD(dwSocketID);
			tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

			//����Ч��
			ASSERT_ERR(pBindParameter->ServiceKind==ServiceKind_Game);
			if (pBindParameter->ServiceKind!=ServiceKind_Game) return false;

			//���ҷ���
			ASSERT_ERR(m_GlobalInfoManager.SearchServerItem(pBindParameter->wServiceID)!=NULL);
			CGlobalServerItem * pGlobalServerItem=m_GlobalInfoManager.SearchServerItem(pBindParameter->wServiceID);

			//�����޸�
			if (pGlobalServerItem!=NULL)
			{
				//���ñ���
				pGlobalServerItem->m_GameServer.wSortID=pServerModify->wSortID;
				pGlobalServerItem->m_GameServer.wKindID=pServerModify->wKindID;
				pGlobalServerItem->m_GameServer.wNodeID=pServerModify->wNodeID;
				pGlobalServerItem->m_GameServer.wServerPort=pServerModify->wServerPort;
				pGlobalServerItem->m_GameServer.dwOnLineCount=pServerModify->dwOnLineCount;
				pGlobalServerItem->m_GameServer.dwFullCount=pServerModify->dwFullCount;
				lstrcpyn(pGlobalServerItem->m_GameServer.szServerName,pServerModify->szServerName,CountArray(pGlobalServerItem->m_GameServer.szServerName));
				lstrcpyn(pGlobalServerItem->m_GameServer.szServerAddr,pServerModify->szServerAddr,CountArray(pGlobalServerItem->m_GameServer.szServerAddr));

				//��������
				CMD_CS_S_ServerModify ServerModify;
				ZeroMemory(&ServerModify,sizeof(ServerModify));

				//���ñ���
				ServerModify.wKindID=pServerModify->wKindID;
				ServerModify.wSortID=pServerModify->wSortID;
				ServerModify.wNodeID=pServerModify->wNodeID;
				ServerModify.wServerID=pBindParameter->wServiceID;
				ServerModify.wServerPort=pServerModify->wServerPort;
				ServerModify.dwOnLineCount=pServerModify->dwOnLineCount;
				ServerModify.dwFullCount=pServerModify->dwFullCount;
				lstrcpyn(ServerModify.szServerAddr,pServerModify->szServerAddr,CountArray(ServerModify.szServerAddr));
				lstrcpyn(ServerModify.szServerName,pServerModify->szServerName,CountArray(ServerModify.szServerName));

				//����֪ͨ
				m_pITCPNetworkEngine->SendDataBatch(MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_MODIFY,&ServerModify,sizeof(ServerModify),0L);
			}

			return true;
		}
	}

	return false;
}

//�û�����
bool CAttemperEngineSink::OnTCPNetworkMainUserCollect(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_CS_C_USER_ENTER:		//�û�����
		{
			//Ч������
			ASSERT_ERR(wDataSize==sizeof(CMD_CS_C_UserEnter));
			if (wDataSize!=sizeof(CMD_CS_C_UserEnter)) return false;

			//��Ϣ����
			CMD_CS_C_UserEnter * pUserEnter=(CMD_CS_C_UserEnter *)pData;
			pUserEnter->szNickName[CountArray(pUserEnter->szNickName)-1]=0;

			//��ȡ����
			WORD wBindIndex=LOWORD(dwSocketID);
			tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

			//����Ч��
			ASSERT_ERR(pBindParameter->ServiceKind==ServiceKind_Game);
			if (pBindParameter->ServiceKind!=ServiceKind_Game) return false;

			//��������
			tagGlobalUserInfo GlobalUserInfo;
			ZeroMemory(&GlobalUserInfo,sizeof(GlobalUserInfo));

			//���ñ���
			GlobalUserInfo.dwUserID=pUserEnter->dwUserID;
			GlobalUserInfo.dwGameID=pUserEnter->dwGameID;
			lstrcpyn(GlobalUserInfo.szNickName,pUserEnter->szNickName,CountArray(GlobalUserInfo.szNickName));

			//������Ϣ
			GlobalUserInfo.cbGender=pUserEnter->cbGender;
			GlobalUserInfo.cbMemberOrder=pUserEnter->cbMemberOrder;
			GlobalUserInfo.cbMasterOrder=pUserEnter->cbMasterOrder;

			//�����û�
			m_GlobalInfoManager.ActiveUserItem(GlobalUserInfo,pBindParameter->wServiceID);
					
			return true;
		}
	case SUB_CS_C_USER_LEAVE:		//�û��뿪
		{
			//Ч������
			ASSERT_ERR(wDataSize==sizeof(CMD_CS_C_UserLeave));
			if (wDataSize!=sizeof(CMD_CS_C_UserLeave)) return false;

			//��Ϣ����
			CMD_CS_C_UserLeave * pUserLeave=(CMD_CS_C_UserLeave *)pData;

			//��ȡ����
			WORD wBindIndex=LOWORD(dwSocketID);
			tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

			//����Ч��
			ASSERT_ERR(pBindParameter->ServiceKind==ServiceKind_Game);
			if (pBindParameter->ServiceKind!=ServiceKind_Game) return false;

			//ɾ���û�
			m_GlobalInfoManager.DeleteUserItem(pUserLeave->dwUserID,pBindParameter->wServiceID);

			return true;
		}
	case SUB_CS_C_USER_FINISH:		//�û����
		{
			//��ȡ����
			WORD wBindIndex=LOWORD(dwSocketID);
			tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

			//����Ч��
			ASSERT_ERR((m_wCollectItem==wBindIndex)&&(pBindParameter->ServiceKind==ServiceKind_Game));
			if ((m_wCollectItem!=wBindIndex)||(pBindParameter->ServiceKind!=ServiceKind_Game)) return false;

			//���ñ���
			m_wCollectItem=INVALID_WORD;

			//�����л�
			if (m_WaitCollectItemArray.GetCount()>0)
			{
				//�л�����
				INT_PTR nWaitCount=m_WaitCollectItemArray.GetCount();
				m_wCollectItem=m_WaitCollectItemArray[nWaitCount-1];

				//ɾ������
				m_WaitCollectItemArray.RemoveAt(nWaitCount-1);

				//������Ϣ
				DWORD dwSocketID=(m_pBindParameter+m_wCollectItem)->dwSocketID;
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_USER_COLLECT,SUB_CS_S_COLLECT_REQUEST);
			}

			return true;
		}
	}

	return false;
}

//Զ�̷���
bool CAttemperEngineSink::OnTCPNetworkMainRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_CS_C_SEARCH_CORRESPOND:	//Э������
		{
			//Ч�����
			ASSERT_ERR(wDataSize==sizeof(CMD_CS_C_SearchCorrespond));
			if (wDataSize!=sizeof(CMD_CS_C_SearchCorrespond)) return false;

			//������Ϣ
			CMD_CS_C_SearchCorrespond * pSearchCorrespond=(CMD_CS_C_SearchCorrespond *)pData;
			pSearchCorrespond->szNickName[CountArray(pSearchCorrespond->szNickName)-1]=0;

			//��������
			CMD_CS_S_SearchCorrespond SearchCorrespond;
			ZeroMemory(&SearchCorrespond,sizeof(SearchCorrespond));

			//����û�
			CGlobalUserItem * ResultUserItem[2];
			ZeroMemory(ResultUserItem,sizeof(ResultUserItem));

			//���ñ���
			SearchCorrespond.dwSocketID=pSearchCorrespond->dwSocketID;
			SearchCorrespond.dwClientAddr=pSearchCorrespond->dwClientAddr;

			//�����û�
			if (pSearchCorrespond->dwGameID!=0L)
			{
				ResultUserItem[0]=m_GlobalInfoManager.SearchUserItemByGameID(pSearchCorrespond->dwGameID);
			}

			//�����û�
			if (pSearchCorrespond->szNickName[0]!=0)
			{
				_tcslwr(pSearchCorrespond->szNickName);
				ResultUserItem[1]=m_GlobalInfoManager.SearchUserItemByNickName(pSearchCorrespond->szNickName);
			}

			//���ý��
			for (BYTE i=0;i<CountArray(ResultUserItem);i++)
			{
				if (ResultUserItem[i]!=NULL)
				{
					//��������
					WORD wServerIndex=0;

					//���ҷ���
					do
					{
						//���ҷ���
						CGlobalServerItem * pGlobalServerItem=ResultUserItem[i]->EnumServerItem(wServerIndex++);

						//��ֹ�ж�
						if (pGlobalServerItem==NULL) break;
						if (SearchCorrespond.wUserCount>=CountArray(SearchCorrespond.UserRemoteInfo)) break;

						//��������
						WORD wIndex=SearchCorrespond.wUserCount++;

						//������Ϣ
						SearchCorrespond.UserRemoteInfo[wIndex].cbGender=ResultUserItem[i]->GetGender();
						SearchCorrespond.UserRemoteInfo[wIndex].cbMemberOrder=ResultUserItem[i]->GetMemberOrder();
						SearchCorrespond.UserRemoteInfo[wIndex].cbMasterOrder=ResultUserItem[i]->GetMasterOrder();

						//�û���Ϣ
						SearchCorrespond.UserRemoteInfo[wIndex].dwUserID=ResultUserItem[i]->GetUserID();
						SearchCorrespond.UserRemoteInfo[wIndex].dwGameID=ResultUserItem[i]->GetGameID();
						lstrcpyn(SearchCorrespond.UserRemoteInfo[wIndex].szNickName,ResultUserItem[i]->GetNickName(),LEN_NICKNAME);

						//������Ϣ
						SearchCorrespond.UserRemoteInfo[wIndex].wKindID=pGlobalServerItem->GetKindID();
						SearchCorrespond.UserRemoteInfo[wIndex].wServerID=pGlobalServerItem->GetServerID();
						lstrcpyn(SearchCorrespond.UserRemoteInfo[wIndex].szGameServer,pGlobalServerItem->m_GameServer.szServerName,LEN_SERVER);

					} while (true);
				}
			}

			//��������
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

//�������
bool CAttemperEngineSink::OnTCPNetworkMainManagerService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_CS_C_SYSTEM_MESSAGE:	//ϵͳ��Ϣ
		{
			//����֪ͨ
			m_pITCPNetworkEngine->SendDataBatch(MDM_CS_MANAGER_SERVICE,SUB_CS_S_SYSTEM_MESSAGE,pData,wDataSize,0L);
			return true;
		}
	case SUB_CS_C_PROPERTY_TRUMPET:  //������Ϣ
		{
			//����֪ͨ
			m_pITCPNetworkEngine->SendDataBatch(MDM_CS_MANAGER_SERVICE,SUB_CS_S_PROPERTY_TRUMPET,pData,wDataSize,0L);
			return true;
		}
	}

	return false;
}


//�����б�
bool CAttemperEngineSink::SendServerListItem(DWORD dwSocketID)
{
	//��������
	WORD wPacketSize=0L;
	POSITION Position=NULL;
	BYTE cbBuffer[SOCKET_TCP_PACKET];

	//������Ϣ
	m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_INFO);

	//�ռ�����
	do
	{
		//��������
		if ((wPacketSize+sizeof(tagGameServer))>sizeof(cbBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_INSERT,cbBuffer,wPacketSize);
			wPacketSize=0;
		}

		//��ȡ����
		tagGameServer * pGameServer=(tagGameServer *)(cbBuffer+wPacketSize);
		CGlobalServerItem * pGlobalServerItem=m_GlobalInfoManager.EnumServerItem(Position);

		//��������
		if (pGlobalServerItem!=NULL)
		{
			wPacketSize+=sizeof(tagGameServer);
			CopyMemory(pGameServer,&pGlobalServerItem->m_GameServer,sizeof(tagGameServer));
		}

	} while (Position!=NULL);

	//��������
	if (wPacketSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_INSERT,cbBuffer,wPacketSize);

	SendMatchListItem(dwSocketID);

	//�������
	m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_FINISH);

	return true;
}

bool CAttemperEngineSink::SendMatchListItem(DWORD dwSocketID)
{
	//��������
	WORD wPacketSize=0L;
	POSITION Position=NULL;
	BYTE cbBuffer[SOCKET_TCP_PACKET] = {0};

	//������Ϣ
	//m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_INFO);

	//�ռ�����
	do
	{
		//��������
		if ((wPacketSize+sizeof(tagGameMatch))>sizeof(cbBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVICE_INFO,SUB_CS_S_MATCH_INSERT,cbBuffer,wPacketSize);
			wPacketSize=0;
		}

		//��ȡ����
		tagGameMatch * pGameServer=(tagGameMatch *)(cbBuffer+wPacketSize);
		CGlobalServerItem * pGlobalServerItem=m_GlobalInfoManager.EnumServerItem(Position);

		//��������
		if (pGlobalServerItem!=NULL)
		{
			wPacketSize+=sizeof(tagGameMatch);
			CopyMemory(pGameServer,&pGlobalServerItem->m_GameMatch,sizeof(tagGameMatch));
		}

	} while (Position!=NULL);

	//��������
	if (wPacketSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVICE_INFO,SUB_CS_S_MATCH_INSERT,cbBuffer,wPacketSize);

	//�������
	//m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_FINISH);

	return true;
}

//�û�����
bool CAttemperEngineSink::SendDataToUser(WORD wServerID, DWORD dwUserID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////////////
//��ʱ��Ϣ����
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

//������Ϣ
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
	
	// �����ı�
	_sntprintf(NameBuf, CountArray(NameBuf), TEXT("MSG%d"), index);
	GetPrivateProfileString(TEXT("MESSGE"), NameBuf, TEXT(""), gladMsg.szContent, sizeof(gladMsg.szContent), szPath);
	// ������ɫ
	_sntprintf(NameBuf, CountArray(NameBuf), TEXT("COL_TEXT%d"), index);
	GetPrivateProfileString(TEXT("MESSGE"), NameBuf, TEXT("FFFFFF"), OutBuf, sizeof(OutBuf), szPath);
	_sntscanf(OutBuf, lstrlen(OutBuf), TEXT("%x"), &gladMsg.colText);
	gladMsg.colText = RGB(GetBValue(gladMsg.colText), GetGValue(gladMsg.colText), GetRValue(gladMsg.colText));
	// ������ɫ
	_sntprintf(NameBuf, CountArray(NameBuf), TEXT("COL_NAME%d"), index);
	GetPrivateProfileString(TEXT("MESSGE"), NameBuf, TEXT("FFFFFF"), OutBuf, sizeof(OutBuf), szPath);
	_sntscanf(OutBuf, lstrlen(OutBuf), TEXT("%x"), &gladMsg.colName);
	gladMsg.colName = RGB(GetBValue(gladMsg.colName), GetGValue(gladMsg.colName), GetRValue(gladMsg.colName));
	// ������ɫ
	_sntprintf(NameBuf, CountArray(NameBuf), TEXT("COL_NUM%d"), index);
	GetPrivateProfileString(TEXT("MESSGE"), NameBuf, TEXT("FFFFFF"), OutBuf, sizeof(OutBuf), szPath);
	_sntscanf(OutBuf, lstrlen(OutBuf), TEXT("%x"), &gladMsg.colNum);
	gladMsg.colNum = RGB(GetBValue(gladMsg.colNum), GetGValue(gladMsg.colNum), GetRValue(gladMsg.colNum));

	if (iMsg == 1)
	{
		// �����ı�
		_sntprintf(NameBuf, CountArray(NameBuf), TEXT("NUM%d"), index);
		GetPrivateProfileString(TEXT("MESSGE"), NameBuf, TEXT("0L"), gladMsg.szNum, sizeof(gladMsg.szNum), szPath);
		
		// �����ı�
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
