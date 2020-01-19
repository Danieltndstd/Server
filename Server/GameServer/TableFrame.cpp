#include "StdAfx.h"
#include "TableFrame.h"
#include "StockManager.h"
#include "DataBasePacket.h"
#include "AttemperEngineSink.h"

//////////////////////////////////////////////////////////////////////////////////

//���߶���
#define IDI_OFF_LINE				(TIME_TABLE_SINK_RANGE+1)			//���߱�ʶ

#define IDI_USER_READY_DETECT		(TIME_TABLE_SINK_RANGE+3)			//׼�����
#define MAX_OFF_LINE				3									//���ߴ���
#define TIME_OFF_LINE				60000L								//����ʱ��

#define IDI_ANDROID_STAND_UP		(TIME_TABLE_SINK_RANGE+4)			//�������뿪

//////////////////////////////////////////////////////////////////////////////////

//�������
CStockManager						g_StockManager;						//������

//��Ϸ��¼
CGameScoreRecordArray				CTableFrame::m_GameScoreRecordBuffer;
CFishRecordArray					CTableFrame::m_FishRecordBuffer;

TCHAR	g_szFilePath[MAX_PATH];
//////////////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrame::CTableFrame()
{
	//��������
	m_wTableID=0;
	m_wChairCount=0;
	m_cbStartMode=START_MODE_ALL_READY;

	//��־����
	m_bGameStarted=false;
	m_bDrawStarted=false;
	m_bTableStarted=false;
	ZeroMemory(m_bAllowLookon,sizeof(m_bAllowLookon));
	ZeroMemory(m_lFrozenedScore,sizeof(m_lFrozenedScore));
	ZeroMemory(&m_RankTopList,sizeof(m_RankTopList));

	//��Ϸ����
	m_lCellScore=0L;
	m_cbGameStatus=GAME_STATUS_FREE;

	//ʱ�����
	m_dwDrawStartTime=0L;
	ZeroMemory(&m_SystemTimeStart,sizeof(m_SystemTimeStart));

	m_wDrawCount=0;

	//��̬����
	m_dwTableOwnerID=0L;
	ZeroMemory(m_szEnterPassword,sizeof(m_szEnterPassword));

	//���߱���
	ZeroMemory(m_wOffLineCount,sizeof(m_wOffLineCount));
	ZeroMemory(m_dwOffLineTime,sizeof(m_dwOffLineTime));

	//������Ϣ
	m_pGameParameter=NULL;
	m_pGameServiceAttrib=NULL;
	m_pGameServiceOption=NULL;

	//����ӿ�
	m_pITimerEngine=NULL;
	m_pITableFrameSink=NULL;
	m_pIMainServiceFrame=NULL;
	m_pIAndroidUserManager=NULL;

	//���Žӿ�
	m_pITableUserAction=NULL;
	m_pITableUserRequest=NULL;
	m_pIMatchTableAction=NULL;

	//���ݽӿ�
	m_pIKernelDataBaseEngine=NULL;
	m_pIRecordDataBaseEngine=NULL;

	//�����ӿ�
	m_pIGameMatchSink=NULL;

	//�û�����
	ZeroMemory(m_TableUserItemArray,sizeof(m_TableUserItemArray));

	//��־�趨
	m_fileLog.SetRoot(TEXT("./SLog/Ox/����%d/����%d/"), 11111, 22222);
}

//��������
CTableFrame::~CTableFrame()
{
	//�ͷŶ���
	SafeRelease(m_pITableFrameSink);
	SafeRelease(m_pIMatchTableAction);

	if (m_pIGameMatchSink!=NULL)
	{
		SafeDelete(m_pIGameMatchSink);
	}

	return;
}

//�ӿڲ�ѯ
VOID * CTableFrame::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrame,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrame,Guid,dwQueryVer);
	return NULL;
}

//��ʼ��Ϸ
bool CTableFrame::StartGame()
{
	//��Ϸ״̬
	ASSERT(m_bDrawStarted==false);
	if (m_bDrawStarted==true) return false;

	//�������
	bool bGameStarted=m_bGameStarted;
	bool bTableStarted=m_bTableStarted;

	//����״̬
	m_bGameStarted=true;
	m_bDrawStarted=true;
	m_bTableStarted=true;

	//��ʼʱ��
	GetLocalTime(&m_SystemTimeStart);
	m_dwDrawStartTime=(DWORD)time(NULL);

	//I_TRACE(TEXT("����[%d],��ʼ��Ϸ"),m_wTableID);

	//��ʼ����
	if (bGameStarted==false)
	{
		//״̬����
		ZeroMemory(m_wOffLineCount,sizeof(m_wOffLineCount));
		ZeroMemory(m_dwOffLineTime,sizeof(m_dwOffLineTime));

		//�����û�
		for (WORD i=0;i<m_wChairCount;i++)
		{
			//��ȡ�û�
			IServerUserItem * pIServerUserItem=GetTableUserItem(i);

			//�����û�
			if (pIServerUserItem!=NULL)
			{
				//����Ԫ��
				if (m_pGameServiceOption->lServiceScore>0L)
				{
					m_lFrozenedScore[i]=m_pGameServiceOption->lServiceScore;

					pIServerUserItem->FrozenedUserScore(m_pGameServiceOption->lServiceScore);
				}

				//����״̬
				BYTE cbUserStatus=pIServerUserItem->GetUserStatus();
				if ((cbUserStatus!=US_OFFLINE)&&(cbUserStatus!=US_PLAYING)) pIServerUserItem->SetUserStatus(US_PLAYING,m_wTableID,i);

				I_TRACE(TEXT("��Ϸ��ʼ ����[%d] �û�[%d] ����[%d-%d] ״̬[%d]"),pIServerUserItem->IsAndroidUser(),pIServerUserItem->GetUserID(),pIServerUserItem->GetTableID(),pIServerUserItem->GetChairID(),pIServerUserItem->GetUserStatus());

			}
		}

		//����״̬
		if (bTableStarted!=m_bTableStarted) SendTableStatus();

	}

	//֪ͨ�¼�
	ASSERT_ERR(m_pITableFrameSink!=NULL);
	if (m_pITableFrameSink!=NULL) m_pITableFrameSink->OnEventGameStart();

	//����֪ͨ
	if(m_pIGameMatchSink!=NULL) m_pIGameMatchSink->OnEventGameStart(this, m_wChairCount);

	return true;
}

//��ɢ��Ϸ
bool CTableFrame::DismissGame()
{
	ConcludeGame(0);
 	IServerUserItem* pIServerUserItem=NULL;
 	BYTE cbUserStatus=US_NULL;
 	for (WORD i=0;i<m_wChairCount;i++)
 	{
 		 pIServerUserItem=GetTableUserItem(i);
 		if ( pIServerUserItem != NULL )
 		{
			
 			PerformStandUpAction(pIServerUserItem);
			if ( pIServerUserItem->IsAndroidUser() )
			{
				CAttemperEngineSink * pAttemperEngineSink = (CAttemperEngineSink *)m_pIMainServiceFrame;
				tagBindParameter * pBindParameter = pAttemperEngineSink->GetBindParameter(pIServerUserItem->GetBindIndex());
				if ( pBindParameter )
				m_pIAndroidUserManager->DeleteAndroidUserItem(pBindParameter->dwSocketID);
				continue;
			}
			
 			pIServerUserItem->SetUserStatus(US_NULL,INVALID_TABLE,INVALID_CHAIR);
 		}
 	}

	return false;
}

//������Ϸ
bool CTableFrame::ConcludeGame(BYTE cbGameStatus)
{
	//Ч��״̬
	ASSERT(m_bGameStarted==true);
	if (m_bGameStarted==false) return false;

	//�������
	bool bDrawStarted=m_bDrawStarted;

	//����״̬
	m_bDrawStarted=false;
	m_cbGameStatus=cbGameStatus;
	m_bGameStarted=(cbGameStatus>=GAME_STATUS_PLAY)?true:false;


	m_wDrawCount++;

	//֪ͨ����
	if(m_pIGameMatchSink!=NULL) m_pIGameMatchSink->OnEventGameEnd(this,0, NULL, cbGameStatus);

	//��Ϸ��¼
	if (bDrawStarted==true)
	{
		//д���¼
		if (CServerRule::IsRecordGameScore(m_pGameServiceOption->dwServerRule)==true)
		{
			//��������
			DBR_GR_GameScoreRecord GameScoreRecord;
			ZeroMemory(&GameScoreRecord,sizeof(GameScoreRecord));

			//���ñ���
			GameScoreRecord.wTableID=m_wTableID;
			GameScoreRecord.dwPlayTimeCount=(bDrawStarted==true)?(DWORD)time(NULL)-m_dwDrawStartTime:0; //������Ϸʱ��

			//��Ϸʱ��
			GameScoreRecord.SystemTimeStart=m_SystemTimeStart;
			GetLocalTime(&GameScoreRecord.SystemTimeConclude);

			//�û�����
			for (INT_PTR i=0;i<m_GameScoreRecordActive.GetCount();i++)
			{
				//��ȡ����
				ASSERT_ERR(m_GameScoreRecordActive[i]!=NULL);
				tagGameScoreRecord * pGameScoreRecord=m_GameScoreRecordActive[i];

				//�û���Ŀ
				if (pGameScoreRecord->cbAndroid==FALSE)
				{
					GameScoreRecord.wUserCount++;
				}
				else
				{
					GameScoreRecord.wAndroidCount++;
				}

				//����ͳ��
				GameScoreRecord.dwUserMemal+=pGameScoreRecord->dwUserMemal;

				//ͳ����Ϣ
				if (pGameScoreRecord->cbAndroid==FALSE) //�����˲����
				{
					//GameScoreRecord.lWasteCount-=(pGameScoreRecord->lScore+pGameScoreRecord->lRevenue);
					//������˰��
					GameScoreRecord.lWasteCount -= pGameScoreRecord->lScore;
					GameScoreRecord.lRevenueCount+=pGameScoreRecord->lRevenue;
				}

				//�ɼ���Ϣ
				if (GameScoreRecord.wRecordCount<CountArray(GameScoreRecord.GameScoreRecord))
				{
					WORD wIndex=GameScoreRecord.wRecordCount++;
					CopyMemory(&GameScoreRecord.GameScoreRecord[wIndex],pGameScoreRecord,sizeof(tagGameScoreRecord));
				}
			}

			//Ͷ������
			if(GameScoreRecord.wUserCount > 0)
			{
				WORD wHeadSize=sizeof(GameScoreRecord)-sizeof(GameScoreRecord.GameScoreRecord);
				WORD wDataSize=sizeof(GameScoreRecord.GameScoreRecord[0])*GameScoreRecord.wRecordCount;
				m_pIRecordDataBaseEngine->PostDataBaseRequest(DBR_GR_GAME_SCORE_RECORD,0,&GameScoreRecord,wHeadSize+wDataSize);
			}
		}

		//�����¼
		if (m_GameScoreRecordActive.GetCount()>0L)
		{
			m_GameScoreRecordBuffer.Append(m_GameScoreRecordActive);
			m_GameScoreRecordActive.RemoveAll();
		}
	}

	//��������
	if (m_bGameStarted==false)
	{
		//��������
		bool bOffLineWait=false;

		//�����û�
		for (WORD i=0;i<m_wChairCount;i++)
		{
			//��ȡ�û�
			IServerUserItem * pIServerUserItem=GetTableUserItem(i);

			if ( pIServerUserItem == NULL ) continue;

			//����Ԫ��
			if (m_lFrozenedScore[i] != 0L)
			{
				pIServerUserItem->UnFrozenedUserScore(m_lFrozenedScore[i]);

				m_lFrozenedScore[i] = 0L;
			}

			//�жϵ���
			if (pIServerUserItem->GetUserStatus() == US_OFFLINE || pIServerUserItem->IsClientReady() == false)
			{
				//���ߴ���
				bOffLineWait = true;
				I_TRACE(TEXT("��Ϸ���� ��ҵ������뷿�� %s, TableID %d,Status %d"), pIServerUserItem->GetNickName(), pIServerUserItem->GetTableID(), pIServerUserItem->GetUserStatus());
				PerformStandUpAction(pIServerUserItem,FALSE);
				pIServerUserItem->SetUserStatus(US_NULL, INVALID_TABLE, INVALID_CHAIR);
			}
			else
			{
				bool bAllowAvertCheatMode = (CServerRule::IsAllowAvertCheatMode(m_pGameServiceOption->dwServerRule) && (m_pGameServiceAttrib->wChairCount < MAX_CHAIR));

				//��������
				if (pIServerUserItem->IsAndroidUser() == true)
				{
					if (m_pGameServiceAttrib->wChairCount < MAX_CHAIR)
					{
						if (bAllowAvertCheatMode)
						{
							ToSaveScore(pIServerUserItem);
							PerformStandUpAction(pIServerUserItem, FALSE);  //������������֪ͨ
						}
						else
							pIServerUserItem->SetUserStatus(US_SIT, m_wTableID, i); //������Ϸ����������
					}
					else pIServerUserItem->SetUserStatus(US_SIT, m_wTableID, i); //������Ϸ����������
				}
				else
				{
					if (m_pGameServiceOption->wKindID != KIND_FISH)
					{
						if (bAllowAvertCheatMode)
							PerformStandUpAction(pIServerUserItem, FALSE);  //����������֪ͨ
						else
							pIServerUserItem->SetUserStatus(US_SIT, m_wTableID, i);
					}
				}
			}
		}

		//ɾ��ʱ��
		//if (bOffLineWait==true) KillGameTimer(IDI_OFF_LINE);
	}

	//��������
	ASSERT_ERR(m_pITableFrameSink!=NULL);
	if (m_pITableFrameSink!=NULL) m_pITableFrameSink->RepositionSink();

	//�߳����  ������Ϸ�����Ż�
	if (m_bGameStarted==false && m_pGameServiceOption->wServerType != GAME_GENRE_MATCH && m_pGameServiceAttrib->wChairCount >= MAX_CHAIR) //������Ϸ��׼������Ϸ�������
	{
 		for (WORD i=0;i<m_wChairCount;i++)
 		{
 			//��ȡ�û�
 			if (m_TableUserItemArray[i]==NULL) continue;
 			IServerUserItem * pIServerUserItem=m_TableUserItemArray[i];

			if (pIServerUserItem->IsAndroidUser()==true && 
				pIServerUserItem->GetUserStatus()>=US_SIT &&
				pIServerUserItem->GetUserScore() <10000000)
			{
				//CAttemperEngineSink * pAttemperEngineSink = (CAttemperEngineSink *)m_pIMainServiceFrame;
				//tagBindParameter * pBindParameter = pAttemperEngineSink->GetBindParameter(pIServerUserItem->GetBindIndex());

				//��ȡǮ������
				ToSaveScore(pIServerUserItem);
				//������Ǯ����ɾ��������
				PerformStandUpAction(pIServerUserItem);

				//m_pIAndroidUserManager->DeleteAndroidUserItem(pBindParameter->dwSocketID);

				continue;
			}

 				if ((m_pGameServiceOption->lMinEnterScore!=0L)&&(pIServerUserItem->GetUserScore()<m_pGameServiceOption->lMinEnterScore))
 				{
 					//������ʾ
 					TCHAR szDescribe[128]=TEXT("");
 					if (m_pGameServiceOption->wServerType&(GAME_GENRE_GOLD|GAME_GENRE_SCORE))
 					{
 						_sntprintf(szDescribe,CountArray(szDescribe),TEXT("�װ�����ң�����Ϸ����Ҫ����%0.2f��ң���Я���Ľ�Ҳ�����������Ϸ���뵽����ȡ����ٽ�����Ϸ��"),double(m_pGameServiceOption->lMinEnterScore)/1000);
 
 					}		
 
//  					if (pIServerUserItem->IsAndroidUser()==true && pIServerUserItem->GetUserStatus()>=US_SIT)
//  					{
// 						CAttemperEngineSink * pAttemperEngineSink = (CAttemperEngineSink *)m_pIMainServiceFrame;
// 						tagBindParameter * pBindParameter = pAttemperEngineSink->GetBindParameter(pIServerUserItem->GetBindIndex());
// 
//  						//������Ǯ����ɾ��������
//  						PerformStandUpAction(pIServerUserItem);
//  						
//  						m_pIAndroidUserManager->DeleteAndroidUserItem(pBindParameter->dwSocketID);
//  						
//  					}
//  					else 
 					{
							SendGameMessage(pIServerUserItem,szDescribe,SMT_CHAT|SMT_EJECT|SMT_CLOSE_GAME);
 					}
 
 					continue;
 				}
 			
		}
	}

	//��������
	ConcludeTable();

	//����״̬
	SendTableStatus();

	//WORD wIndex = 3;
	//if (m_pGameServiceOption->wKindID == 215) wIndex = 8;
	//SetGameTimer(IDI_ANDROID_STAND_UP, wIndex *1000,1,0);

	return true;
}
//�����˴�ȡ��
void CTableFrame::ToSaveScore(IServerUserItem* pIServerUserItem)
{	
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();
	if (pUserInfo == NULL) return;
	if (pIServerUserItem->IsAndroidUser() == false) return;
	if (  m_pGameServiceOption->wRoomType == 4  ) //����
	{
		pUserInfo->lScore = 500000;
	}
	else 
	{
		if (pUserInfo->lScore <= m_pGameServiceOption->lMinEnterScore)
		{
			pUserInfo->lScore += (long long)((float)rand() / 1000.0f + (float)(m_pGameServiceOption->lMinEnterScore) * 4.5);
			//pUserInfo->lScore = 1255000;
			I_TRACE(TEXT("[ȡ��] ������[%d],UserID[%d],����[%I64d],׼��[%I64d]"), pIServerUserItem->IsAndroidUser(), pIServerUserItem->GetUserID(), pIServerUserItem->GetUserScore(), m_pGameServiceOption->lMinEnterScore);
		}
	}
	
}
//��������
bool CTableFrame::ConcludeTable()
{
	//��������
	if ((m_bGameStarted==false)&&(m_bTableStarted==true))
	{
		//�����ж�
		WORD wTableUserCount=GetSitUserCount();
		if (wTableUserCount==0) m_bTableStarted=false;
		if (m_pGameServiceAttrib->wChairCount==MAX_CHAIR) m_bTableStarted=false;

		//ģʽ�ж�
		if (m_cbStartMode==START_MODE_FULL_READY) m_bTableStarted=false;
		if (m_cbStartMode==START_MODE_PAIR_READY) m_bTableStarted=false;
		if (m_cbStartMode==START_MODE_ALL_READY) m_bTableStarted=false;
		if (m_cbStartMode==START_MODE_SYSTRM_BANKER) m_bTableStarted=false;		// ��Ϸ����������״̬Ҳ����  ZZL
	}

	return true;
}
// ����д��¼
bool CTableFrame::WriteFishRecord(WORD wChairID, tagFishRecordInfo & FishRecordInfo, BYTE WriteScore)
{
	//Ч�����
	ASSERT_ERR((wChairID < m_wChairCount));
	if ((wChairID >= m_wChairCount)) return false;

	//��ȡ�û�
	ASSERT_ERR(GetTableUserItem(wChairID) != NULL);
	IServerUserItem * pIServerUserItem = GetTableUserItem(wChairID);

	//д����� 
	if (pIServerUserItem != NULL)
	{
		//��Ϸ��¼
		if (CServerRule::IsRecordGameScore(m_pGameServiceOption->dwServerRule) == true)
		{
			//��������
			tagFishRecordInfo * pFishRecord = NULL;

			//��ѯ���
			if (m_FishRecordBuffer.GetCount() > 0L)
			{
				//��ȡ����
				INT_PTR nCount = m_FishRecordBuffer.GetCount();
				pFishRecord = m_FishRecordBuffer[nCount - 1];

				//ɾ������
				m_FishRecordBuffer.RemoveAt(nCount - 1);
			}

			//��������
			if (pFishRecord == NULL)
			{
				try
				{
					//��������
					pFishRecord = new tagFishRecordInfo;
					if (pFishRecord == NULL) throw TEXT("��Ϸ��¼���󴴽�ʧ��");
				}
				catch (...)
				{
					ASSERT_ERR(FALSE);
				}
			}

			//��¼����
			if (pFishRecord != NULL)
			{
				pFishRecord->dwUserID = FishRecordInfo.dwUserID;
				pFishRecord->dwRoomRatio = FishRecordInfo.dwRoomRatio;
				pFishRecord->dwBulletID = FishRecordInfo.dwBulletID;
				pFishRecord->dwBulletScore = FishRecordInfo.dwBulletScore;
				pFishRecord->dwFishID = FishRecordInfo.dwFishID;
				pFishRecord->dwFishType = FishRecordInfo.dwFishType;
				pFishRecord->dwFishScore = FishRecordInfo.dwFishScore;
				pFishRecord->dwRevenueScore = FishRecordInfo.dwRevenueScore;

				pFishRecord->dwSceneType = FishRecordInfo.dwSceneType;
				pFishRecord->dwScoreType = FishRecordInfo.dwScoreType;
				pFishRecord->dwBulletSum = FishRecordInfo.dwBulletSum;
				pFishRecord->dwBulletAllScore = FishRecordInfo.dwBulletAllScore;
				pFishRecord->dwCheckFishAllScore = FishRecordInfo.dwCheckFishAllScore;
				pFishRecord->dwScore = FishRecordInfo.dwScore;
				pFishRecord->dwStartScore = FishRecordInfo.dwStartScore;
				pFishRecord->dwEndScore = FishRecordInfo.dwEndScore;

				pFishRecord->stStartTiem = FishRecordInfo.stStartTiem;
				pFishRecord->stEndTiem = FishRecordInfo.stEndTiem;

				_sntprintf(pFishRecord->szGameDetails, CountArray(pFishRecord->szGameDetails),
					TEXT("%s"),FishRecordInfo.szGameDetails);

				//��������
				m_FishRecordActive.Add(pFishRecord);
			}


			//������Ϸ ��ʱд��
			if (WriteScore == 1 || m_FishRecordActive.GetCount() > 50)
			{
				//��������
				DBR_GP_GameFishRecord GameFishRecord;
				ZeroMemory(&GameFishRecord, sizeof(GameFishRecord));

				GameFishRecord.wTableID = m_wTableID;

				//�û�����
				for (INT_PTR i = 0; i < m_FishRecordActive.GetCount(); i++)
				{
					//��ȡ����
					ASSERT_ERR(m_FishRecordActive[i] != NULL);
					tagFishRecordInfo * pFishRecord = m_FishRecordActive[i];

					//�ɼ���Ϣ
					if (GameFishRecord.wRecordCount < CountArray(GameFishRecord.FishRecord))
					{
						WORD wIndex = GameFishRecord.wRecordCount++;
						CopyMemory(&GameFishRecord.FishRecord[wIndex], pFishRecord, sizeof(tagFishRecordInfo));
					}
				}

				//Ͷ������
				if (GameFishRecord.wRecordCount> 0)
				{
					WORD wHeadSize = sizeof(GameFishRecord) - sizeof(GameFishRecord.FishRecord);
					WORD wDataSize = sizeof(GameFishRecord.FishRecord[0])*GameFishRecord.wRecordCount;
					m_pIRecordDataBaseEngine->PostDataBaseRequest(DBR_GP_GAME_FISH_RECORD, 0, &GameFishRecord, wHeadSize + wDataSize);
				}
				//�����¼
				if (m_FishRecordActive.GetCount() > 0L)
				{
					m_FishRecordBuffer.Append(m_FishRecordActive);
					m_FishRecordActive.RemoveAll();
				}
			}
		}
	}
	

	return true;
}

// ����Ϸ ������Ϣ����
bool CTableFrame::GameScrollMessage(VOID * pData, WORD wDataSize)
{
	return m_pIRecordDataBaseEngine->PostDataBaseRequest(DBR_GP_GAME_SCROLL_MESSAGE, 0, pData, wDataSize);
}

//д�����
bool CTableFrame::WriteUserScore(WORD wChairID, tagScoreInfo & ScoreInfo, BYTE WriteScore)
{
	//Ч�����
	ASSERT_ERR((wChairID < m_wChairCount) && (ScoreInfo.cbType != SCORE_TYPE_NULL));
	if ((wChairID >= m_wChairCount) && (ScoreInfo.cbType == SCORE_TYPE_NULL)) return false;

	//��ȡ�û�
	ASSERT_ERR(GetTableUserItem(wChairID) != NULL);
	IServerUserItem * pIServerUserItem = GetTableUserItem(wChairID);
	TCHAR szMessage[128] = TEXT("");

	//д����� 
	if (pIServerUserItem != NULL)
	{

		//��Ϸʱ��
		DWORD dwCurrentTime = (DWORD)time(NULL);
		DWORD dwPlayTimeCount = (m_bDrawStarted == true) ? dwCurrentTime - m_dwDrawStartTime : 0L;


		//д�����
		if (WriteScore == 0)
			pIServerUserItem->WriteUserScore(ScoreInfo.lScore, m_pGameServiceOption->lMinEnterScore, ScoreInfo.lRevenue, 0, ScoreInfo.cbType, dwPlayTimeCount);

		//��Ϸ��¼
		if (m_pGameServiceOption->wKindID != 2016 && CServerRule::IsRecordGameScore(m_pGameServiceOption->dwServerRule) == true)
		{
			//��������
			tagGameScoreRecord * pGameScoreRecord = NULL;

			//��ѯ���
			if (m_GameScoreRecordBuffer.GetCount() > 0L)
			{
				//��ȡ����
				INT_PTR nCount = m_GameScoreRecordBuffer.GetCount();
				pGameScoreRecord = m_GameScoreRecordBuffer[nCount - 1];

				//ɾ������
				m_GameScoreRecordBuffer.RemoveAt(nCount - 1);
			}

			//��������
			if (pGameScoreRecord == NULL)
			{
				try
				{
					//��������
					pGameScoreRecord = new tagGameScoreRecord;
					if (pGameScoreRecord == NULL) throw TEXT("��Ϸ��¼���󴴽�ʧ��");
				}
				catch (...)
				{
					ASSERT_ERR(FALSE);
				}
			}

			//��¼����
			if (pGameScoreRecord != NULL)
			{
				//�û���Ϣ
				pGameScoreRecord->wChairID = wChairID;
				pGameScoreRecord->dwUserID = pIServerUserItem->GetUserID();
				pGameScoreRecord->cbAndroid = (pIServerUserItem->IsAndroidUser() ? TRUE : FALSE);

				//�û���Ϣ
				pGameScoreRecord->dwDBQuestID = pIServerUserItem->GetDBQuestID();
				pGameScoreRecord->dwInoutIndex = pIServerUserItem->GetInoutIndex();

				//�ɼ���Ϣ
				pGameScoreRecord->lScore = ScoreInfo.lScore;
				//lGrade��Ϊ��ǰ����
				pGameScoreRecord->lGrade = pIServerUserItem->GetUserScore();
				pGameScoreRecord->lRevenue = ScoreInfo.lRevenue;

				//������Ϣ
				pGameScoreRecord->dwUserMemal = pIServerUserItem->GetUserMedal();
				pGameScoreRecord->dwPlayTimeCount = dwPlayTimeCount;
				//SYSTEMTIME SystemTime;
				//GetLocalTime(&pGameScoreRecord->RecordDate);

				pGameScoreRecord->lAddScore = ScoreInfo.lAddScore;
				_sntprintf_s(pGameScoreRecord->TCardDate, MAX_PATH, TEXT("%s"), ScoreInfo.TCardDate);
				_sntprintf_s(pGameScoreRecord->TGameProce, MAX_PATH, TEXT("%s"), ScoreInfo.TGameProce);

				//��������˰
				// 				if(pIServerUserItem->IsAndroidUser())
				// 				{
				// 					pGameScoreRecord->lScore += pGameScoreRecord->lRevenue;
				// 					pGameScoreRecord->lRevenue = 0L;
				// 				}

				//��������
				m_GameScoreRecordActive.Add(pGameScoreRecord);
			}


			//������Ϸ ��ʱд��
			if (m_pGameServiceOption->wKindID == 2016 &&
				(WriteScore == 0 || m_GameScoreRecordActive.GetCount() > 12))
			{
				//��������
				DBR_GR_GameScoreRecord GameScoreRecord;
				ZeroMemory(&GameScoreRecord, sizeof(GameScoreRecord));

				//���ñ���
				GameScoreRecord.wTableID = m_wTableID;
				GameScoreRecord.dwPlayTimeCount = 0; //������Ϸʱ��

													 //��Ϸʱ��
				GameScoreRecord.SystemTimeStart = m_SystemTimeStart;
				GetLocalTime(&GameScoreRecord.SystemTimeConclude);

				//�û�����
				for (INT_PTR i = 0; i < m_GameScoreRecordActive.GetCount(); i++)
				{
					//��ȡ����
					ASSERT_ERR(m_GameScoreRecordActive[i] != NULL);
					tagGameScoreRecord * pGameScoreRecord = m_GameScoreRecordActive[i];

					//�û���Ŀ
					if (pGameScoreRecord->cbAndroid == FALSE)
					{
						GameScoreRecord.wUserCount++;
					}
					else
					{
						GameScoreRecord.wAndroidCount++;
					}

					//����ͳ��
					GameScoreRecord.dwUserMemal += pGameScoreRecord->dwUserMemal;

					//ͳ����Ϣ
					if (pGameScoreRecord->cbAndroid==FALSE)
					{
						GameScoreRecord.lWasteCount -= (pGameScoreRecord->lScore + pGameScoreRecord->lRevenue);
						GameScoreRecord.lRevenueCount += pGameScoreRecord->lRevenue;
					}

					//�ɼ���Ϣ
					if (GameScoreRecord.wRecordCount < CountArray(GameScoreRecord.GameScoreRecord))
					{
						WORD wIndex = GameScoreRecord.wRecordCount++;
						CopyMemory(&GameScoreRecord.GameScoreRecord[wIndex], pGameScoreRecord, sizeof(tagGameScoreRecord));
					}
				}

				//Ͷ������
				if (GameScoreRecord.wUserCount > 0)
				{
					WORD wHeadSize = sizeof(GameScoreRecord) - sizeof(GameScoreRecord.GameScoreRecord);
					WORD wDataSize = sizeof(GameScoreRecord.GameScoreRecord[0])*GameScoreRecord.wRecordCount;
					m_pIRecordDataBaseEngine->PostDataBaseRequest(DBR_GR_GAME_SCORE_RECORD, 0, &GameScoreRecord, wHeadSize + wDataSize);
				}
				//�����¼
				if (m_GameScoreRecordActive.GetCount() > 0L)
				{
					m_GameScoreRecordBuffer.Append(m_GameScoreRecordActive);
					m_GameScoreRecordActive.RemoveAll();
				}
			}
		}
	}
	else
	{
		//�뿪�û�
		I_TRACE(TEXT("����ڴ������Ϣ��ʧ bug!!!"));

		return false;
	}


	//�㲥��Ϣ
	if (szMessage[0] != 0)
	{
		//��������
		IServerUserItem * pISendUserItem = NULL;
		WORD wEnumIndex = 0;

		//��Ϸ���
		for (WORD i = 0; i < m_wChairCount; i++)
		{
			//��ȡ�û�
			pISendUserItem = GetTableUserItem(i);
			if (pISendUserItem == NULL) continue;

			//������Ϣ
			SendGameMessage(pISendUserItem, szMessage, SMT_CHAT);
		}

		//�Թ��û�
		do
		{
			pISendUserItem = EnumLookonUserItem(wEnumIndex++);
			if (pISendUserItem != NULL)
			{
				//������Ϣ
				SendGameMessage(pISendUserItem, szMessage, SMT_CHAT);
			}
		} while (pISendUserItem != NULL);
	}
	return true;
}

//д�����
bool CTableFrame::WriteTableScore(tagScoreInfo ScoreInfoArray[], WORD wScoreCount)
{
	//Ч�����
	ASSERT_ERR(wScoreCount==m_wChairCount);
	if (wScoreCount!=m_wChairCount) return false;

	//д�����
	for (WORD i=0;i<m_wChairCount;i++)
	{
		if (ScoreInfoArray[i].cbType!=SCORE_TYPE_NULL)
		{
			WriteUserScore(i,ScoreInfoArray[i]);
		}
	}

	return true;
}

//д�뽱��
bool CTableFrame::WriteTableMedal(tagMedalInfo MedalInfoArray[], WORD wCount)
{
	//Ч�����
	ASSERT(wCount==m_wChairCount);
	if (wCount!=m_wChairCount) return false;

	//д�����
	for (WORD i=0;i<m_wChairCount;i++)
	{
		//if (MedalInfoArray[i].cbType!=SCORE_TYPE_NULL)
		{
			WriteUserMedal(i,MedalInfoArray[i]);
		}
	}

	return true;
}

//д�뽱��
bool CTableFrame::WriteUserMedal(WORD wChairID, tagMedalInfo & MedalInfo)
{
	//Ч�����
	ASSERT((wChairID<m_wChairCount));
	if ((wChairID>=m_wChairCount)) return false;

	//��ȡ�û�
	ASSERT(GetTableUserItem(wChairID)!=NULL);
	IServerUserItem * pIServerUserItem=GetTableUserItem(wChairID);

	if (pIServerUserItem!=NULL)
	{
		//д�뽱��
		pIServerUserItem->WriteUserMedal(MedalInfo.lMedal,MedalInfo.cbType);
	}

	return true;
}

bool CTableFrame:: WriteGameEvent(WORD wChairID,void* pData)
{
	//Ч�����
	ASSERT((wChairID<m_wChairCount));
	if ((wChairID>=m_wChairCount)) return false;

	//��ȡ�û�
	ASSERT(GetTableUserItem(wChairID)!=NULL);
	IServerUserItem * pIServerUserItem=GetTableUserItem(wChairID);

	if (pIServerUserItem!=NULL)
	{
		m_pIMainServiceFrame->PostGameEvent(pIServerUserItem,pData);
	}
	
	return true;
}

//����˰��
SCORE CTableFrame::CalculateRevenue(WORD wChairID, SCORE lScore)
{
	//Ч�����
	ASSERT_ERR(wChairID<m_wChairCount);
	if (wChairID>=m_wChairCount) return 0L;

	//����˰��
	if ((m_pGameServiceOption->wRevenueRatio>0)&&(lScore>=REVENUE_BENCHMARK))
	{
		//��ȡ�û�
		ASSERT_ERR(GetTableUserItem(wChairID)!=NULL);
		IServerUserItem * pIServerUserItem=GetTableUserItem(wChairID);

		//����˰��
		SCORE lRevenue=lScore*m_pGameServiceOption->wRevenueRatio/REVENUE_DENOMINATOR;

		return lRevenue;
	}

	return 0L;
}

//�����޶�
SCORE CTableFrame::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{
	//�û�Ч��
	ASSERT_ERR(pIServerUserItem->GetTableID()==m_wTableID);
	if (pIServerUserItem->GetTableID()!=m_wTableID) return 0L;

	//��ѯ���
	SCORE lTrusteeScore=pIServerUserItem->GetTrusteeScore();
	SCORE lMinEnterScore=m_pGameServiceOption->lMinTableScore;
	SCORE lUserConsumeQuota=m_pITableFrameSink->QueryConsumeQuota(pIServerUserItem);

	//Ч����
	ASSERT_ERR((lUserConsumeQuota>=0L)&&(lUserConsumeQuota<=pIServerUserItem->GetUserScore()-lMinEnterScore));
	if ((lUserConsumeQuota<0L)||(lUserConsumeQuota>pIServerUserItem->GetUserScore()-lMinEnterScore)) return 0L;

	return lUserConsumeQuota+lTrusteeScore;
}

//Ѱ���û�
IServerUserItem * CTableFrame::SearchUserItem(DWORD dwUserID)
{
	//��������
	WORD wEnumIndex=0;
	IServerUserItem * pIServerUserItem=NULL;

	//�����û�
	for (WORD i=0;i<m_wChairCount;i++)
	{
		pIServerUserItem=GetTableUserItem(i);
		if ((pIServerUserItem!=NULL)&&(pIServerUserItem->GetUserID()==dwUserID)) return pIServerUserItem;
	}

	//�Թ��û�
	do
	{
		pIServerUserItem=EnumLookonUserItem(wEnumIndex++);
		if ((pIServerUserItem!=NULL)&&(pIServerUserItem->GetUserID()==dwUserID)) return pIServerUserItem;
	} while (pIServerUserItem!=NULL);

	return NULL;
}

//��Ϸ�û�
IServerUserItem * CTableFrame::GetTableUserItem(WORD wChairID)
{
	//Ч�����
	if (wChairID>=m_wChairCount)
		return NULL;

	//��ȡ�û�
	IServerUserItem* pUserItem = m_TableUserItemArray[wChairID];
	if ( pUserItem != NULL)
	{
		WORD wTempTableID = pUserItem->GetTableID();
		WORD wTempChairID = pUserItem->GetChairID();

		//�����쳣��
		if ( wTempTableID == INVALID_TABLE && wTempChairID == INVALID_CHAIR )
		{
			CString cs;
			cs.Format(TEXT("�쳣����[%d-%d] ����[%d] ���[%d] ״̬[%d] ������[%d]"),wTempTableID,wTempChairID,m_wTableID,pUserItem->GetUserID(),pUserItem->GetUserStatus(),pUserItem->IsAndroidUser());
			I_TRACE(cs.GetBuffer(0));
			m_ServerFileLog.Log(cs.GetBuffer(0));

			m_TableUserItemArray[wChairID]=NULL;
			return NULL;
		}
	}
	return m_TableUserItemArray[wChairID];

}

//�Թ��û�
IServerUserItem * CTableFrame::EnumLookonUserItem(WORD wEnumIndex)
{
	if (wEnumIndex>=m_LookonUserItemArray.GetCount()) return NULL;
	return m_LookonUserItemArray[wEnumIndex];
}

//����ʱ��
bool CTableFrame::SetGameTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter)
{
	//Ч�����
	ASSERT_ERR((dwTimerID>0)&&(dwTimerID<TIME_TABLE_MODULE_RANGE));
	if ((dwTimerID<=0)||(dwTimerID>=TIME_TABLE_MODULE_RANGE)) return false;

	//����ʱ��
	DWORD dwEngineTimerID=IDI_TABLE_MODULE_START+m_wTableID*TIME_TABLE_MODULE_RANGE;
	if (m_pITimerEngine!=NULL) m_pITimerEngine->SetTimer(dwEngineTimerID+dwTimerID,dwElapse,dwRepeat,dwBindParameter);

	return true;
}

//ɾ��ʱ��
bool CTableFrame::KillGameTimer(DWORD dwTimerID)
{
	//Ч�����
	ASSERT_ERR((dwTimerID>0)&&(dwTimerID<=TIME_TABLE_MODULE_RANGE));
	if ((dwTimerID<=0)||(dwTimerID>TIME_TABLE_MODULE_RANGE)) return false;

	//ɾ��ʱ��
	DWORD dwEngineTimerID=IDI_TABLE_MODULE_START+m_wTableID*TIME_TABLE_MODULE_RANGE;
	if (m_pITimerEngine!=NULL) m_pITimerEngine->KillTimer(dwEngineTimerID+dwTimerID);

	return true;
}

//��������
bool CTableFrame::SendUserItemData(IServerUserItem * pIServerUserItem, WORD wSubCmdID)
{
	//״̬Ч��
	ASSERT_ERR((pIServerUserItem!=NULL)&&(pIServerUserItem->IsClientReady()==true));
	if ((pIServerUserItem==NULL)&&(pIServerUserItem->IsClientReady()==false)) return false;

	//��������
	m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,NULL,0);

	return true;
}

//��������
bool CTableFrame::SendUserItemData(IServerUserItem * pIServerUserItem, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//״̬Ч��
	ASSERT_ERR((pIServerUserItem!=NULL)&&(pIServerUserItem->IsClientReady()==true));
	if ((pIServerUserItem==NULL)&&(pIServerUserItem->IsClientReady()==false)) return false;

	//��������
	m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,pData,wDataSize);

	return true;
}

//��������
bool CTableFrame::SendTableData(WORD wChairID, WORD wSubCmdID)
{
	//�û�Ⱥ��
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<m_wChairCount;i++)
		{
			//��ȡ�û�
			IServerUserItem * pIServerUserItem=GetTableUserItem(i);
			if (pIServerUserItem==NULL) continue;

			//Ч��״̬
			//ASSERT(pIServerUserItem->IsClientReady()==true);
			//if (pIServerUserItem->IsClientReady()==false) continue;

			//��������
			m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,NULL,0);
		}

		return true;
	}
	else
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) return false;

		//Ч��״̬
		//ASSERT(pIServerUserItem->IsClientReady()==true);
		//if (pIServerUserItem->IsClientReady()==false) return false;

		//��������
		m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,NULL,0);

		return true;
	}

	return false;
}

//��������
bool CTableFrame::SendTableData(WORD wChairID, WORD wSubCmdID, VOID * pData, WORD wDataSize,WORD wMainCmdID)
{
	//�û�Ⱥ��
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<m_wChairCount;i++)
		{
			//��ȡ�û�
			IServerUserItem * pIServerUserItem=GetTableUserItem(i);
			if ((pIServerUserItem==NULL)||(pIServerUserItem->IsClientReady()==false)) continue;

			//��������
			m_pIMainServiceFrame->SendData(pIServerUserItem,wMainCmdID,wSubCmdID,pData,wDataSize);
		}

		return true;
	}
	else
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=GetTableUserItem(wChairID);
		if ((pIServerUserItem==NULL)||(pIServerUserItem->IsClientReady()==false)) return false;

		//��������
		m_pIMainServiceFrame->SendData(pIServerUserItem,wMainCmdID,wSubCmdID,pData,wDataSize);

		return true;
	}

	return false;
}

//��������
bool CTableFrame::SendLookonData(WORD wChairID, WORD wSubCmdID)
{
	//��������
	WORD wEnumIndex=0;
	IServerUserItem * pIServerUserItem=NULL;

	//ö���û�
	do
	{
		//��ȡ�û�
		pIServerUserItem=EnumLookonUserItem(wEnumIndex++);
		if (pIServerUserItem==NULL) break;

		//Ч��״̬
		ASSERT_ERR(pIServerUserItem->IsClientReady()==true);
		if (pIServerUserItem->IsClientReady()==false) return false;

		//��������
		if ((wChairID==INVALID_CHAIR)||(pIServerUserItem->GetChairID()==wChairID))
		{
			m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,NULL,0);
		}

	} while (true);

	return true;
}

//��������
bool CTableFrame::SendLookonData(WORD wChairID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//��������
	WORD wEnumIndex=0;
	IServerUserItem * pIServerUserItem=NULL;

	//ö���û�
	do
	{
		//��ȡ�û�
		pIServerUserItem=EnumLookonUserItem(wEnumIndex++);
		if (pIServerUserItem==NULL) break;

		//Ч��״̬
		//ASSERT_ERR(pIServerUserItem->IsClientReady()==true);
		if (pIServerUserItem->IsClientReady()==false) return false;

		//��������
		if ((wChairID==INVALID_CHAIR)||(pIServerUserItem->GetChairID()==wChairID))
		{
			m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,pData,wDataSize);
		}

	} while (true);

	return true;
}

//������Ϣ
bool CTableFrame::SendGameMessage(LPCTSTR lpszMessage, WORD wType)
{
	//��������
	WORD wEnumIndex=0;

	//������Ϣ
	for (WORD i=0;i<m_wChairCount;i++)
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=GetTableUserItem(i);
		if ((pIServerUserItem==NULL)||(pIServerUserItem->IsClientReady()==false)) continue;

		//������Ϣ
		m_pIMainServiceFrame->SendGameMessage(pIServerUserItem,lpszMessage,wType);
	}

	//ö���û�
	do
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=EnumLookonUserItem(wEnumIndex++);
		if (pIServerUserItem==NULL) break;

		//Ч��״̬
		ASSERT_ERR(pIServerUserItem->IsClientReady()==true);
		if (pIServerUserItem->IsClientReady()==false) return false;

		//������Ϣ
		m_pIMainServiceFrame->SendGameMessage(pIServerUserItem,lpszMessage,wType);

	} while (true);

	return true;
}

//������Ϣ
bool CTableFrame::SendRoomMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType)
{
	//�û�Ч��
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//������Ϣ
	m_pIMainServiceFrame->SendRoomMessage(pIServerUserItem,lpszMessage,wType);

	return true;
}

//��Ϸ��Ϣ
bool CTableFrame::SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType)
{
	//�û�Ч��
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//������Ϣ
	return m_pIMainServiceFrame->SendGameMessage(pIServerUserItem,lpszMessage,wType);
}

//���ͳ���
bool CTableFrame::SendGameScene(IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
{
	//�û�Ч��
	ASSERT_ERR((pIServerUserItem!=NULL)&&(pIServerUserItem->IsClientReady()==true));
	if ((pIServerUserItem==NULL)||(pIServerUserItem->IsClientReady()==false)) return false;

	//���ͳ���
	ASSERT_ERR(m_pIMainServiceFrame!=NULL);
	m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_GAME_SCENE,pData,wDataSize);

	return true;
}

//�����¼�
bool CTableFrame::OnEventUserOffLine(IServerUserItem * pIServerUserItem)
{
	//����Ч��
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//�û�����
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();
	IServerUserItem * pITableUserItem=m_TableUserItemArray[pUserInfo->wChairID];

	//�û�����
	WORD wChairID=pIServerUserItem->GetChairID();
	BYTE cbUserStatus=pIServerUserItem->GetUserStatus();

	//��Ϸ�û�
	if (cbUserStatus!=US_LOOKON)
	{
		//Ч���û�
		ASSERT_ERR(pIServerUserItem==GetTableUserItem(wChairID));
		if (pIServerUserItem!=GetTableUserItem(wChairID)) return false;

		bool bFlag = false;

		//���ߴ���
		if ((cbUserStatus==US_PLAYING)/*&&(m_wOffLineCount[wChairID]<MAX_OFF_LINE)*/)
			bFlag = true;
		if ( m_pGameServiceOption->wServerType&GAME_GENRE_MATCH )
			bFlag = true;
		//���ߴ���
		if ( bFlag )
		{
			//�û�����
			pIServerUserItem->SetClientReady(false);
			pIServerUserItem->SetUserStatus(US_OFFLINE,m_wTableID,wChairID);
			m_pITableFrameSink->OnFrameMessage(SUB_GF_USER_OFFLINE,NULL,NULL,pIServerUserItem);

			//���ߴ���
			//if (m_dwOffLineTime[wChairID]==0L)
			//{
			//	//���ñ���
			//	m_wOffLineCount[wChairID]++;
			//	m_dwOffLineTime[wChairID]=(DWORD)time(NULL);

			//	//ʱ������
			//	//WORD wOffLineCount=GetOffLineUserCount();
			//	//if (wOffLineCount==1)

			//}

			//��ʱ����ID�ܽ��ͣ�һ�����Ӳ��ܳ���50������Ȼ��������һ������ȥ
			//DWORD dwEngineTimerID=IDI_TABLE_MODULE_START+m_wTableID*TIME_TABLE_MODULE_RANGE;
			//if (m_pITimerEngine!=NULL) m_pITimerEngine->SetTimer(dwEngineTimerID+IDI_OFF_LINE,120*1000,1,wChairID);

			if ( !pIServerUserItem->IsAndroidUser() )
				I_TRACE(TEXT("��� %s �ѵ���,Status:%d,TableID:%d,ChairID:%d"),pIServerUserItem->GetNickName(),pIServerUserItem->GetUserStatus(),m_wTableID,wChairID);

			//DWORD UserID = pIServerUserItem->GetUserID();
			//m_pIRecordDataBaseEngine->PostDataBaseRequest(DBR_GR_SET_USER_OFFLINE_RECORD,0,&UserID,sizeof(DWORD));

			return true;
		}
	}

	//��Ϸû��ʼ���������ˣ��ͻ����������
	I_TRACE(TEXT("OnEventUserOffLine %s, %d,%d"),pIServerUserItem->GetNickName(),pIServerUserItem->GetTableID(),pIServerUserItem->GetUserStatus());

	if ( m_pGameServiceOption->wServerType != GAME_GENRE_MATCH  )
	{
		PerformStandUpAction(pIServerUserItem);

		pIServerUserItem->SetUserStatus(US_NULL,INVALID_TABLE,INVALID_CHAIR);
	}

	return true;
}

//�����¼�
bool CTableFrame::OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	//֪ͨ��Ϸ
	return m_pITableFrameSink->OnUserScroeNotify(wChairID,pIServerUserItem,cbReason);
}

//ʱ���¼�
bool CTableFrame::OnEventTimer(DWORD dwTimerID, WPARAM dwBindParameter)
{
	//�ص��¼�
	if ((dwTimerID>=0)&&(dwTimerID<TIME_TABLE_SINK_RANGE))
	{
		ASSERT_ERR(m_pITableFrameSink!=NULL);
		m_pITableFrameSink->OnTimerMessage(dwTimerID,dwBindParameter);
	}

	//�¼�����
	switch (dwTimerID)
	{
	case IDI_OFF_LINE:	//�����¼�
		{
			return true;
			//Ч��״̬
			ASSERT_ERR(m_bGameStarted==true);
			if (m_bGameStarted==false) return false;

			//��������
			DWORD dwOffLineTime=0L;
			WORD wOffLineChair=INVALID_CHAIR;

			DISK_LOG(TEXT("���߶�ʱ��ִ��"));
			wOffLineChair = (WORD)dwBindParameter;
			IServerUserItem * pUserItem=GetTableUserItem(wOffLineChair);
			if ( pUserItem != NULL && 
				pUserItem->GetUserID() != 0 && 
				pUserItem->GetTableID() != INVALID_TABLE &&
				pUserItem->GetUserStatus() == US_OFFLINE &&
				//czg bug ���ߵ����������˻ᵼ�� ĳ�ֽ���ʱ����Ϸ��ʼ�Ʒ��Ҳ�����λ�ϵ���
				(GetGameStatus()<GAME_STATUS_PLAY)	//��ǰ���Ӳ�������Ϸ״̬
				)
			{
				//���ñ���
				m_dwOffLineTime[wOffLineChair]=0L;

				//�û�����
				DISK_LOG(TEXT("����ʱ���ѵ�������� %s, ���� %d,״̬ %d"),pUserItem->GetNickName(),pUserItem->GetTableID(),pUserItem->GetUserStatus());
				PerformStandUpAction(pUserItem);
				return true;
			}

			//Ѱ���û�
			for (WORD i=0;i<m_wChairCount;i++)
			{
				if ((m_dwOffLineTime[i]!=0L)&&((m_dwOffLineTime[i]<dwOffLineTime)||(wOffLineChair==INVALID_CHAIR)))
				{
					wOffLineChair=i;
					dwOffLineTime=m_dwOffLineTime[i];
				}
			}

			//λ���ж�
			ASSERT_ERR(wOffLineChair!=INVALID_CHAIR);
			if (wOffLineChair==INVALID_CHAIR) return false;

			//�û��ж�
			ASSERT_ERR(dwBindParameter<m_wChairCount);
			if (wOffLineChair!=(WORD)dwBindParameter)
			{
				//ʱ�����
				DWORD dwCurrentTime=(DWORD)time(NULL);
				DWORD dwLapseTime=dwCurrentTime-m_dwOffLineTime[wOffLineChair];

				//����ʱ��
				dwLapseTime=__min(dwLapseTime,TIME_OFF_LINE-2000L);
				SetGameTimer(IDI_OFF_LINE,TIME_OFF_LINE-dwLapseTime,1,wOffLineChair);

				return true;
			}

			//czg bug ���Ӵ�����Ϸ״̬ ���ߵ����������˻ᵼ�� ĳ�ֽ���ʱ����Ϸ��ʼ�Ʒ��Ҳ�����λ�ϵ���
			if (m_bGameStarted==false)
			{
				//��ȡ�û�
				ASSERT_ERR(GetTableUserItem(wOffLineChair)!=NULL);
				IServerUserItem * pIServerUserItem=GetTableUserItem(wOffLineChair);

				//������Ϸ
				if (pIServerUserItem!=NULL)
				{
					//���ñ���
					m_dwOffLineTime[wOffLineChair]=0L;

					//�û�����
					DISK_LOG(TEXT("IDI_OFF_LINE %s, %d,%d"),pIServerUserItem->GetNickName(),pIServerUserItem->GetTableID(),pIServerUserItem->GetUserStatus());
					PerformStandUpAction(pIServerUserItem);
				}
			}
			else
			//����ʱ��
			{
				//��������
				DWORD dwOffLineTime=0L;
				WORD wOffLineChair=INVALID_CHAIR;

				//Ѱ���û�
				for (WORD i=0;i<m_wChairCount;i++)
				{
					if ((m_dwOffLineTime[i]!=0L)&&((m_dwOffLineTime[i]<dwOffLineTime)||(wOffLineChair==INVALID_CHAIR)))
					{
						wOffLineChair=i;
						dwOffLineTime=m_dwOffLineTime[i];
					}
				}

				//����ʱ��
				if (wOffLineChair!=INVALID_CHAIR)
				{
					//ʱ�����
					DWORD dwCurrentTime=(DWORD)time(NULL);
					DWORD dwLapseTime=dwCurrentTime-m_dwOffLineTime[wOffLineChair];

					//����ʱ��
					dwLapseTime=__min(dwLapseTime,TIME_OFF_LINE-2000L);
					SetGameTimer(IDI_OFF_LINE,TIME_OFF_LINE-dwLapseTime,1,wOffLineChair);
				}
			}

			return true;
		}
 		case IDI_GAME_OPTION:
			{
				for (WORD i=0;i<m_wChairCount;i++)
				{
					//��ȡ�û�
					IServerUserItem * pIServerUserItem=GetTableUserItem(i);

					//��ȡ����
					WORD wChairID=pIServerUserItem->GetChairID();
					BYTE cbUserStatus=pIServerUserItem->GetUserStatus();

					//��������
					if ((cbUserStatus!=US_LOOKON)&&((m_dwOffLineTime[wChairID]!=0L)))
					{
						//���ñ���
						//m_dwOffLineTime[wChairID]=0L;

						//ɾ��ʱ��
						//WORD wOffLineCount=GetOffLineUserCount();
						//if (wOffLineCount==0) KillGameTimer(IDI_OFF_LINE);
					}

					//����״̬
					pIServerUserItem->SetClientReady(true);
					m_bAllowLookon[wChairID]=false;

					//����״̬
					CMD_GF_GameStatus GameStatus;
					GameStatus.cbGameStatus=m_cbGameStatus;
					GameStatus.cbAllowLookon=FALSE;
					m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_GAME_STATUS,&GameStatus,sizeof(GameStatus));

					//���ͳ���
					bool bSendSecret= false;
					m_pITableFrameSink->OnEventSendGameScene(wChairID,pIServerUserItem,m_cbGameStatus,bSendSecret);

					//��ʼ�ж�
					if ((cbUserStatus==US_READY || cbUserStatus==US_PLAYING || cbUserStatus== US_OFFLINE)&&(EfficacyStartGame(wChairID)==true))
					{
						if ( m_pGameServiceOption->wServerType&GAME_GENRE_MATCH )
							DISK_LOG(TEXT("��Ϸ��ʱ��ʼ �� %d ��"),m_wTableID);
						StartGame(); 
					}
				}
				return true;
			}
		case IDI_ANDROID_STAND_UP:
			{
				for (WORD i=0;i<m_wChairCount;i++)
				{
					IServerUserItem * pIServerUserItem=GetTableUserItem(i);
					if ( pIServerUserItem == NULL ) continue;
					if ( pIServerUserItem->IsAndroidUser() == false ) continue;
					if( pIServerUserItem->GetUserStatus() != US_PLAYING ) continue;

					CAttemperEngineSink * pAttemperEngineSink = (CAttemperEngineSink *)m_pIMainServiceFrame;
					tagBindParameter * pBindParameter = pAttemperEngineSink->GetBindParameter(pIServerUserItem->GetBindIndex());

					if( m_pGameServiceOption->wKindID == 220 )
					{
						I_TRACE(TEXT("������ȡǮ������ %s, TableID %d, Status %d"),pIServerUserItem->GetNickName(),pIServerUserItem->GetTableID(),pIServerUserItem->GetUserStatus());
						ToSaveScore(pIServerUserItem);
						PerformStandUpAction(pIServerUserItem);
						continue;
					}
					I_TRACE(TEXT("������������ %s, TableID %d, Status %d"),pIServerUserItem->GetNickName(),pIServerUserItem->GetTableID(),pIServerUserItem->GetUserStatus());
					PerformStandUpAction(pIServerUserItem);

					//������ɾ��
					//m_pIAndroidUserManager->DeleteAndroidUserItem(pBindParameter->dwSocketID);
				}

				return true;
			}
		case IDI_USER_READY_DETECT:
			{
				//return true;
				I_TRACE(TEXT("δ׼����Ҽ��"));
				for ( WORD i=0;i<m_wChairCount;++i )
				{
					IServerUserItem * pIServerUserItem=GetTableUserItem(i);

					if ( pIServerUserItem == NULL) continue;
					WORD wChairID=pIServerUserItem->GetChairID();
					BYTE cbUserStatus=pIServerUserItem->GetUserStatus();
					DWORD dwSitDownTime = pIServerUserItem->GetSitDownTime();

					//�ų�������״̬
					if ( cbUserStatus == US_FREE ||
						cbUserStatus == US_PLAYING ||
						cbUserStatus == US_LOOKON ||
						cbUserStatus == US_READY ||
						cbUserStatus == US_NULL 
						) continue;
					if ( dwSitDownTime == 0 ) continue;
					if ( IsGameStarted() ) continue;
					DWORD dwCurTime = (DWORD)time(NULL);
					if ( dwCurTime - dwSitDownTime >=5 )  //���¼��5�룬�߳�
					{
						//I_TRACE(TEXT("δ׼������߳�"));
						SendGameMessage(pIServerUserItem,TEXT("��������ʱ��û׼��,ϵͳ�Ѱ����߳�����"),SMT_CHAT|SMT_EJECT|SMT_CLOSE_GAME);
						PerformStandUpAction(pIServerUserItem);
						m_pIMainServiceFrame->ShutDownSocket(pIServerUserItem);
						


					}
				}
				return true;
			}
	}

	return false;
}

//��Ϸ�¼�
bool CTableFrame::OnEventSocketGame(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	//Ч�����
	ASSERT_ERR(pIServerUserItem!=NULL);
	ASSERT_ERR(m_pITableFrameSink!=NULL);

	//��Ϣ����
	bool bRet = false;
	bRet = m_pITableFrameSink->OnGameMessage(wSubCmdID,pData,wDataSize,pIServerUserItem);
	if ( bRet == false)
		I_TRACE(TEXT("��Ϸ�¼�S_cmd:%d,����:%d,���:%d"),wSubCmdID,m_wTableID,pIServerUserItem->GetUserID());
	return bRet;
}

//����¼�
bool CTableFrame::OnEventSocketFrame(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	//��Ϸ����
	if (m_pITableFrameSink->OnFrameMessage(wSubCmdID,pData,wDataSize,pIServerUserItem)==true) return true;

	//��������
	if(m_pIGameMatchSink!=NULL && m_pIGameMatchSink->OnFrameMessage(wSubCmdID,pData,wDataSize,pIServerUserItem)==true) return true;

	//Ĭ�ϴ���
	switch (wSubCmdID)
	{
	case SUB_GF_GAME_OPTION_H5 :
		{
		//GameServer::GSGameOption gsGameOption;
		//gsGameOption.ParseFromArray(pData, wDataSize);

		//������Ϣֻ����һ��
		if ( pIServerUserItem->IsClientReady() )
		{
			m_fileLog.Log(TEXT("������Ϣֻ����һ�� %d"), pIServerUserItem->IsClientReady());
			return true;
		}

		//��ȡ����
		WORD wChairID = pIServerUserItem->GetChairID();
		BYTE cbUserStatus = pIServerUserItem->GetUserStatus();

		//����״̬
		pIServerUserItem->SetClientReady(true);

		//����״̬
		/*CMD_GF_GameStatus GameStatus;
		GameStatus.cbGameStatus = m_cbGameStatus;
		GameStatus.cbAllowLookon = FALSE;*/

		GameServer::Proto_GameStatus ProtoGameStarus;
		ProtoGameStarus.set_cbgamestatus(m_cbGameStatus);
		std::string serializeStr;
		ProtoGameStarus.SerializeToString(&serializeStr);
		m_pIMainServiceFrame->SendData(pIServerUserItem, MDM_GF_FRAME, SUB_GF_GAME_STATUS, (void*)serializeStr.c_str(), (WORD)serializeStr.size());

		//���ͳ���
		bool bSendSecret = ((cbUserStatus != US_LOOKON) || (m_bAllowLookon[wChairID] == true));
		m_pITableFrameSink->OnEventSendGameScene(wChairID, pIServerUserItem, m_cbGameStatus, bSendSecret);
		I_TRACE(TEXT("���ͳ�����Ϣ %s,%d-%d,Status:%d"),pIServerUserItem->GetNickName(),pIServerUserItem->GetTableID(),pIServerUserItem->GetChairID(),pIServerUserItem->GetUserStatus());

		//��ʼ�ж�
		if ((cbUserStatus == US_READY || cbUserStatus == US_PLAYING) && (EfficacyStartGame(wChairID) == true))
		{
			if (m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)
			{
				KillGameTimer(IDI_GAME_OPTION);
			}
			//I_TRACE(TEXT("����׼������,��Ϸ��ʼ"));
			StartGame();
		}

			return true;
		}
	case SUB_GF_USER_READY_H5:
		{
		GameServer::GSUserReady gsUserReady;
		gsUserReady.ParseFromArray(pData, wDataSize);
		if (pIServerUserItem->GetUserID() != gsUserReady.userid())
			return false;

		//��ȡ����
		WORD wChairID = pIServerUserItem->GetChairID();
		BYTE cbUserStatus = pIServerUserItem->GetUserStatus();

		//Ч��״̬
		ASSERT_ERR(GetTableUserItem(wChairID) == pIServerUserItem);
		if (GetTableUserItem(wChairID) != pIServerUserItem) return false;


		if (m_pGameServiceOption->wServerType == GAME_GENRE_SCORE && pIServerUserItem->GetUserScore() <= GetCellScore() * 5)
		{
			SendGameMessage(pIServerUserItem, TEXT("Я���������㣬��ȡ����ٽ�����Ϸ!"), SMT_CHAT | SMT_EJECT | SMT_CLOSE_GAME);
			return true;
		}

		if (pIServerUserItem->GetUserScore() <= m_pGameServiceOption->lMinEnterScore)
		{
			//������ʾ
			TCHAR szDescribe[128] = TEXT("");
			if (m_pGameServiceOption->wServerType&(GAME_GENRE_GOLD | GAME_GENRE_SCORE))
				_sntprintf(szDescribe, CountArray(szDescribe), TEXT("�װ�����ң�����Ϸ����Ҫ����%0.2f��ң���Я���Ľ�Ҳ�����������Ϸ���뵽����ȡ����ٽ�����Ϸ��"), double(m_pGameServiceOption->lMinEnterScore) / 1000);

			//������Ϣ
			if (pIServerUserItem->IsAndroidUser() == true)
			{
				//DISK_LOG(TEXT("AndroidUser not enough score %s, %d,%d"),pIServerUserItem->GetNickName(),pIServerUserItem->GetTableID(),pIServerUserItem->GetUserStatus());
				PerformStandUpAction(pIServerUserItem);

				//CAttemperEngineSink * pAttemperEngineSink = (CAttemperEngineSink *)m_pIMainServiceFrame;
				//tagBindParameter * pBindParameter = pAttemperEngineSink->GetBindParameter(pIServerUserItem->GetBindIndex());

				//m_pIAndroidUserManager->DeleteAndroidUserItem(pBindParameter->dwSocketID);		 
			}
			else
				SendGameMessage(pIServerUserItem, szDescribe, SMT_CHAT | SMT_EJECT | SMT_CLOSE_GAME);

			return true;
		}

		if (cbUserStatus != US_SIT) return true;

		//�����ж�
		if (m_wDrawCount >= __max(rand() % 6, 3) && pIServerUserItem->IsAndroidUser() && CServerRule::IsAllowAvertCheatMode(m_pGameServiceOption->dwServerRule) && (m_pGameServiceAttrib->wChairCount < MAX_CHAIR))
		{
			m_pIMainServiceFrame->InsertDistribute(pIServerUserItem);
			return true;
		}

		//�¼�֪ͨ
		if (m_pITableUserAction != NULL)
		{
			m_pITableUserAction->OnActionUserOnReady(wChairID, pIServerUserItem, pData, wDataSize);
		}

		//�¼�֪ͨ
		if (m_pIMatchTableAction != NULL && !m_pIMatchTableAction->OnActionUserOnReady(wChairID, pIServerUserItem, pData, wDataSize))
			return true;

		//��ʼ�ж�
		if (EfficacyStartGame(wChairID) == false)
		{
			pIServerUserItem->SetUserStatus(US_READY, m_wTableID, wChairID);
		}
		else
		{
			StartGame();
		}

		return true;
			return true;
		}
	case SUB_GF_LOOKON_CONFIG_H5:
		{
			return true;
		}
	case SUB_GF_GAME_OPTION:	//��Ϸ����
		{
			//Ч�����
			//ASSERT_ERR(wDataSize==sizeof(CMD_GF_GameOption));
			//if (wDataSize!=sizeof(CMD_GF_GameOption)) return false;

			//��������
			//CMD_GF_GameOption * pGameOption=(CMD_GF_GameOption *)pData;

			//��ȡ����
			WORD wChairID=pIServerUserItem->GetChairID();
			BYTE cbUserStatus=pIServerUserItem->GetUserStatus();

			//����״̬
			pIServerUserItem->SetClientReady(true);
			//if (cbUserStatus!=US_LOOKON) m_bAllowLookon[wChairID]=pGameOption->cbAllowLookon?true:false;

			//����״̬
			CMD_GF_GameStatus GameStatus;
			GameStatus.cbGameStatus=m_cbGameStatus;
			GameStatus.cbAllowLookon=m_bAllowLookon[wChairID]?TRUE:FALSE;
			m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_GAME_STATUS,&GameStatus,sizeof(GameStatus));

// 			if ((m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)==0)
// 			{
// 				//������Ϣ
// 				TCHAR szMessage[128]=TEXT("");
// 				_sntprintf(szMessage,CountArray(szMessage),TEXT("��ӭ�����롰%s����Ϸ��ף����Ϸ��죡"),m_pGameServiceAttrib->szGameName);
// 				m_pIMainServiceFrame->SendGameMessage(pIServerUserItem,szMessage,SMT_CHAT);
// 			}

			//���ͳ���
			bool bSendSecret=((cbUserStatus!=US_LOOKON)||(m_bAllowLookon[wChairID]==true));
			m_pITableFrameSink->OnEventSendGameScene(wChairID,pIServerUserItem,m_cbGameStatus,bSendSecret);

			//m_pIMainServiceFrame->SendUserCardBox(pIServerUserItem);
			//I_TRACE(TEXT("����׼�� %s %d-%d Status:%d"), pIServerUserItem->GetNickName(), pIServerUserItem->GetTableID(), pIServerUserItem->GetChairID(), pIServerUserItem->GetUserStatus());
			//��ʼ�ж�
			if ((cbUserStatus==US_READY || cbUserStatus == US_PLAYING)&&(EfficacyStartGame(wChairID)==true))
			{
				if ( m_pGameServiceOption->wServerType&GAME_GENRE_MATCH )
				{
					KillGameTimer(IDI_GAME_OPTION);
				}
				StartGame(); 
			}

			return true;
		}
	case SUB_GF_USER_READY:		//�û�׼��
		{
			//��ȡ����
			WORD wChairID=pIServerUserItem->GetChairID();
			BYTE cbUserStatus=pIServerUserItem->GetUserStatus();

			//Ч��״̬
			ASSERT_ERR(GetTableUserItem(wChairID)==pIServerUserItem);
			if (GetTableUserItem(wChairID)!=pIServerUserItem) return false;

			if ( m_pGameServiceOption->wServerType&GAME_GENRE_MATCH ) return true;

			if ( m_pGameServiceOption->wServerType==GAME_GENRE_SCORE && pIServerUserItem->GetUserScore() <= GetCellScore()*5 )
			{
				SendGameMessage(pIServerUserItem,TEXT("Я���������㣬��ȡ����ٽ�����Ϸ!"),SMT_CHAT|SMT_EJECT|SMT_CLOSE_GAME);
				return true;
			}

				if (pIServerUserItem->GetUserScore()<=m_pGameServiceOption->lMinEnterScore)
				{
					//������ʾ
					TCHAR szDescribe[128]=TEXT("");
					if (m_pGameServiceOption->wServerType&(GAME_GENRE_GOLD|GAME_GENRE_SCORE))
						_sntprintf(szDescribe, CountArray(szDescribe), TEXT("�װ�����ң�����Ϸ����Ҫ����%0.2f��ң���Я���Ľ�Ҳ�����������Ϸ���뵽����ȡ����ٽ�����Ϸ��"), double(m_pGameServiceOption->lMinEnterScore) / 1000);

					//������Ϣ
					if (pIServerUserItem->IsAndroidUser()==true)
					{
						//DISK_LOG(TEXT("AndroidUser not enough score %s, %d,%d"),pIServerUserItem->GetNickName(),pIServerUserItem->GetTableID(),pIServerUserItem->GetUserStatus());
						PerformStandUpAction(pIServerUserItem);

						//CAttemperEngineSink * pAttemperEngineSink = (CAttemperEngineSink *)m_pIMainServiceFrame;
						//tagBindParameter * pBindParameter = pAttemperEngineSink->GetBindParameter(pIServerUserItem->GetBindIndex());

						//m_pIAndroidUserManager->DeleteAndroidUserItem(pBindParameter->dwSocketID);		 
					}
					else
						SendGameMessage(pIServerUserItem, szDescribe, SMT_CHAT | SMT_EJECT | SMT_CLOSE_GAME);

					return true;
				}

//				if ( m_pGameServiceOption->wRoomType!=3 && (m_pGameServiceOption->lMaxEnterScore!=0L)&&(pIServerUserItem->GetUserScore()>=m_pGameServiceOption->lMaxEnterScore*3) )
//				{
					//������ʾ
// 					TCHAR szDescribe[128]=TEXT("");
// 					if (m_pGameServiceOption->wServerType&(GAME_GENRE_GOLD|GAME_GENRE_SCORE))
// 					{
// 						_sntprintf(szDescribe,CountArray(szDescribe),TEXT("�װ�����ң����ķ���̫�ߣ����ܽ�����Ϸ���뵽�߱�����Ϸ��"));
// 					}

					//������Ϣ
//					if (pIServerUserItem->IsAndroidUser()==true)
//					{
//						//��ȡǮ������
//						ToSaveScore(pIServerUserItem);
//						PerformStandUpAction(pIServerUserItem);
// 						CAttemperEngineSink * pAttemperEngineSink = (CAttemperEngineSink *)m_pIMainServiceFrame;
// 						tagBindParameter * pBindParameter = pAttemperEngineSink->GetBindParameter(pIServerUserItem->GetBindIndex());
// 						m_pIAndroidUserManager->DeleteAndroidUserItem(pBindParameter->dwSocketID);

//					}
//					else SendGameMessage(pIServerUserItem,szDescribe,SMT_CHAT|SMT_EJECT|SMT_CLOSE_GAME);

//				}
			

			//Ч��״̬
			//ASSERT_ERR(cbUserStatus==US_SIT);
			if (cbUserStatus!=US_SIT) return true;

			//�����ж�
 			if (m_wDrawCount >= __max(rand()%6, 3) && pIServerUserItem->IsAndroidUser() && CServerRule::IsAllowAvertCheatMode(m_pGameServiceOption->dwServerRule)&&(m_pGameServiceAttrib->wChairCount < MAX_CHAIR))
 			{
 				m_pIMainServiceFrame->InsertDistribute(pIServerUserItem);
 				return true;
 			}

			//�¼�֪ͨ
			if (m_pITableUserAction!=NULL)
			{
				m_pITableUserAction->OnActionUserOnReady(wChairID,pIServerUserItem,pData,wDataSize);
			}

			//�¼�֪ͨ
			if(m_pIMatchTableAction!=NULL && !m_pIMatchTableAction->OnActionUserOnReady(wChairID,pIServerUserItem, pData,wDataSize))
				return true;

			//��ʼ�ж�
			if (EfficacyStartGame(wChairID)==false)
			{
				pIServerUserItem->SetUserStatus(US_READY,m_wTableID,wChairID);
			}
			else
			{
				I_TRACE(TEXT("������׼������Ϸ��ʼ!"));
				StartGame(); 
			}

			return true;
		}
	case SUB_GF_USER_CHAT:		//�û�����
		{
			//��������
			CMD_GF_C_UserChat * pUserChat=(CMD_GF_C_UserChat *)pData;

			//Ч�����
			ASSERT_ERR(wDataSize<=sizeof(CMD_GF_C_UserChat));
			ASSERT_ERR(wDataSize>=(sizeof(CMD_GF_C_UserChat)-sizeof(pUserChat->szChatString)));
			ASSERT_ERR(wDataSize==(sizeof(CMD_GF_C_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0])));

			//Ч�����
			if (wDataSize>sizeof(CMD_GF_C_UserChat)) return false;
			if (wDataSize<(sizeof(CMD_GF_C_UserChat)-sizeof(pUserChat->szChatString))) return false;
			if (wDataSize!=(sizeof(CMD_GF_C_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0]))) return false;

			//Ŀ���û�
			if ((pUserChat->dwTargetUserID!=0)&&(SearchUserItem(pUserChat->dwTargetUserID)==NULL))
			{
				ASSERT_ERR(FALSE);
				return true;
			}

			//״̬�ж�
			if ((CServerRule::IsForfendGameChat(m_pGameServiceOption->dwServerRule)==true)&&(pIServerUserItem->GetMasterOrder()==0L))
			{
				SendGameMessage(pIServerUserItem,TEXT("��Ǹ����ǰ��Ϸ�����ֹ��Ϸ���죡"),SMT_CHAT);
				return true;
			}

			//Ȩ���ж�
			if (CUserRight::CanRoomChat(pIServerUserItem->GetUserRight())==false)
			{
				SendGameMessage(pIServerUserItem,TEXT("��Ǹ����û����Ϸ�����Ȩ�ޣ�����Ҫ����������ϵ��Ϸ�ͷ���ѯ��"),SMT_EJECT|SMT_CHAT);
				return true;
			}

			//������Ϣ
			CMD_GF_S_UserChat UserChat;
			ZeroMemory(&UserChat,sizeof(UserChat));

			//�ַ�����
			m_pIMainServiceFrame->SensitiveWordFilter(pUserChat->szChatString,UserChat.szChatString,CountArray(UserChat.szChatString));

			//��������
			UserChat.dwChatColor=pUserChat->dwChatColor;
			UserChat.wChatLength=pUserChat->wChatLength;
			UserChat.dwTargetUserID=pUserChat->dwTargetUserID;
			UserChat.dwSendUserID=pIServerUserItem->GetUserID();
			UserChat.wChatLength=CountStringBuffer(UserChat.szChatString);

			//��������
			WORD wHeadSize=sizeof(UserChat)-sizeof(UserChat.szChatString);
			WORD wSendSize=wHeadSize+UserChat.wChatLength*sizeof(UserChat.szChatString[0]);

			//��Ϸ�û�
			for (WORD i=0;i<m_wChairCount;i++)
			{
				//��ȡ�û�
				IServerUserItem * pIServerUserItem=GetTableUserItem(i);
				if ((pIServerUserItem==NULL)||(pIServerUserItem->IsClientReady()==false)) continue;

				m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_USER_CHAT,&UserChat,wSendSize);
			}

			//�Թ��û�
			WORD wEnumIndex=0;
			IServerUserItem * pIServerUserItem=NULL;

			//ö���û�
			do
			{
				//��ȡ�û�
				pIServerUserItem=EnumLookonUserItem(wEnumIndex++);
				if (pIServerUserItem==NULL) break;

				//��������
				if (pIServerUserItem->IsClientReady()==true)
				{
					m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_USER_CHAT,&UserChat,wSendSize);
				}
			} while (true);

			return true;
		}
	case SUB_GF_USER_EXPRESSION:	//�û�����
		{
			//Ч�����
			ASSERT_ERR(wDataSize==sizeof(CMD_GF_C_UserExpression));
			if (wDataSize!=sizeof(CMD_GF_C_UserExpression)) return false;

			//��������
			CMD_GF_C_UserExpression * pUserExpression=(CMD_GF_C_UserExpression *)pData;

			//Ŀ���û�
			if ((pUserExpression->dwTargetUserID!=0)&&(SearchUserItem(pUserExpression->dwTargetUserID)==NULL))
			{
				ASSERT_ERR(FALSE);
				return true;
			}

			//״̬�ж�
			if ((CServerRule::IsForfendGameChat(m_pGameServiceOption->dwServerRule)==true)&&(pIServerUserItem->GetMasterOrder()==0L))
			{
				SendGameMessage(pIServerUserItem,TEXT("��Ǹ����ǰ��Ϸ�����ֹ��Ϸ���죡"),SMT_CHAT);
				return true;
			}

			//Ȩ���ж�
			if (CUserRight::CanRoomChat(pIServerUserItem->GetUserRight())==false)
			{
				SendGameMessage(pIServerUserItem,TEXT("��Ǹ����û����Ϸ�����Ȩ�ޣ�����Ҫ����������ϵ��Ϸ�ͷ���ѯ��"),SMT_EJECT|SMT_CHAT);
				return true;
			}

			//������Ϣ
			CMD_GR_S_UserExpression UserExpression;
			ZeroMemory(&UserExpression,sizeof(UserExpression));

			//��������
			UserExpression.wItemIndex=pUserExpression->wItemIndex;
			UserExpression.dwSendUserID=pIServerUserItem->GetUserID();
			UserExpression.dwTargetUserID=pUserExpression->dwTargetUserID;

			//��Ϸ�û�
			for (WORD i=0;i<m_wChairCount;i++)
			{
				//��ȡ�û�
				IServerUserItem * pIServerUserItem=GetTableUserItem(i);
				if ((pIServerUserItem==NULL)||(pIServerUserItem->IsClientReady()==false)) continue;

				//��������
				m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_USER_EXPRESSION,&UserExpression,sizeof(UserExpression));
			}

			//�Թ��û�
			WORD wEnumIndex=0;
			IServerUserItem * pIServerUserItem=NULL;

			//ö���û�
			do
			{
				//��ȡ�û�
				pIServerUserItem=EnumLookonUserItem(wEnumIndex++);
				if (pIServerUserItem==NULL) break;

				//��������
				if (pIServerUserItem->IsClientReady()==true)
				{
					m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_USER_EXPRESSION,&UserExpression,sizeof(UserExpression));
				}
			} while (true);

			return true;
		}
	case SUB_GF_LOOKON_CONFIG:		//�Թ�����
		{
			//Ч�����
			ASSERT_ERR(wDataSize==sizeof(CMD_GF_LookonConfig));
			if (wDataSize<sizeof(CMD_GF_LookonConfig)) return false;

			//��������
			CMD_GF_LookonConfig * pLookonConfig=(CMD_GF_LookonConfig *)pData;

			//Ŀ���û�
			if ((pLookonConfig->dwUserID!=0)&&(SearchUserItem(pLookonConfig->dwUserID)==NULL))
			{
				ASSERT_ERR(FALSE);
				return true;
			}

			//�û�Ч��
			ASSERT_ERR(pIServerUserItem->GetUserStatus()!=US_LOOKON);
			if (pIServerUserItem->GetUserStatus()==US_LOOKON) return false;

			//�Թ۴���
			if (pLookonConfig->dwUserID!=0L)
			{
				for (INT_PTR i=0;i<m_LookonUserItemArray.GetCount();i++)
				{
					//��ȡ�û�
					IServerUserItem * pILookonUserItem=m_LookonUserItemArray[i];
					if (pILookonUserItem->GetUserID()!=pLookonConfig->dwUserID) continue;
					if (pILookonUserItem->GetChairID()!=pIServerUserItem->GetChairID()) continue;

					//������Ϣ
					CMD_GF_LookonStatus LookonStatus;
					LookonStatus.cbAllowLookon=pLookonConfig->cbAllowLookon;

					//������Ϣ
					ASSERT_ERR(m_pIMainServiceFrame!=NULL);
					m_pIMainServiceFrame->SendData(pILookonUserItem,MDM_GF_FRAME,SUB_GF_LOOKON_STATUS,&LookonStatus,sizeof(LookonStatus));

					break;
				}
			}
			else
			{
				//�����ж�
				bool bAllowLookon=(pLookonConfig->cbAllowLookon==TRUE)?true:false;
				if (bAllowLookon==m_bAllowLookon[pIServerUserItem->GetChairID()]) return true;

				//���ñ���
				m_bAllowLookon[pIServerUserItem->GetChairID()]=bAllowLookon;

				//������Ϣ
				CMD_GF_LookonStatus LookonStatus;
				LookonStatus.cbAllowLookon=pLookonConfig->cbAllowLookon;

				//������Ϣ
				for (INT_PTR i=0;i<m_LookonUserItemArray.GetCount();i++)
				{
					//��ȡ�û�
					IServerUserItem * pILookonUserItem=m_LookonUserItemArray[i];
					if (pILookonUserItem->GetChairID()!=pIServerUserItem->GetChairID()) continue;

					//������Ϣ
					ASSERT_ERR(m_pIMainServiceFrame!=NULL);
					m_pIMainServiceFrame->SendData(pILookonUserItem,MDM_GF_FRAME,SUB_GF_LOOKON_STATUS,&LookonStatus,sizeof(LookonStatus));
				}
			}

			return true;
		}
	}

	return false;
}

//��ȡ��λ
WORD CTableFrame::GetNullChairID()
{
	//��������
	for (WORD i=0;i<m_wChairCount;i++)
	{
		if (m_TableUserItemArray[i]==NULL)
		{
			return i;
		}
	}

	return INVALID_CHAIR;
}

//�����λ
WORD CTableFrame::GetRandNullChairID()
{
	//��������
	WORD wIndex = 0;
	if ( m_pGameServiceOption->wKindID == GAME_CALLBAN_OX)
	{
		wIndex = 0;
	}
	else
	{
		wIndex = rand() % m_wChairCount;
	}
	for (WORD i=wIndex;i<m_wChairCount+wIndex;i++)
	{
		if (m_TableUserItemArray[i%m_wChairCount]==NULL)
		{
			return i%m_wChairCount;
		}
	}

	return INVALID_CHAIR;
}

//�û���Ŀ
WORD CTableFrame::GetSitUserCount()
{
	//��������
	WORD wUserCount=0;

	//��Ŀͳ��
	for (WORD i=0;i<m_wChairCount;i++)
	{
		if (GetTableUserItem(i)!=NULL)
		{
			wUserCount++;
		}
	}

	return wUserCount;
}

//�Թ���Ŀ
WORD CTableFrame::GetLookonUserCount()
{
	//��ȡ��Ŀ
	INT_PTR nLookonCount=m_LookonUserItemArray.GetCount();

	return (WORD)(nLookonCount);
}

//������Ŀ
WORD CTableFrame::GetOffLineUserCount()
{
	//��������
	WORD wOffLineCount=0;

	//��������
	for (WORD i=0;i<m_wChairCount;i++)
	{
		if (m_dwOffLineTime[i]!=0L)
		{
			wOffLineCount++;
		}
	}

	return wOffLineCount;
}


//����״��
WORD CTableFrame::GetTableUserInfo(tagTableUserInfo & TableUserInfo)
{
	//���ñ���
	ZeroMemory(&TableUserInfo,sizeof(TableUserInfo));

	//�û�����
	for (WORD i=0;i<m_pGameServiceAttrib->wChairCount;i++)
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=GetTableUserItem(i);
		if (pIServerUserItem==NULL) continue;

		//�û�����
		if (pIServerUserItem->IsAndroidUser()==false)
		{
			TableUserInfo.wTableUserCount++;
		}
		else
		{
			TableUserInfo.wTableAndroidCount++;
		}

		//׼���ж�
		if (pIServerUserItem->GetUserStatus()==US_READY)
		{
			TableUserInfo.wTableReadyCount++;
		}
	}

	//������Ŀ
	switch (m_cbStartMode)
	{
	case START_MODE_ALL_READY:		//����׼��
		{
			TableUserInfo.wMinUserCount=2;
			break;
		}
	case START_MODE_PAIR_READY:		//��Կ�ʼ
		{
			TableUserInfo.wMinUserCount=2;
			break;
		}
	case START_MODE_TIME_CONTROL:	//ʱ�����
		{
			TableUserInfo.wMinUserCount=1;
			break;
		}
	case START_MODE_SYSTRM_BANKER:	//ϵͳ��ׯ����С��ʼ����			ZZL
		{
			TableUserInfo.wMinUserCount=1;
			break;
		}
	default:						//Ĭ��ģʽ
		{
			TableUserInfo.wMinUserCount=m_pGameServiceAttrib->wChairCount;
			break;
		}
	}

	return TableUserInfo.wTableAndroidCount+TableUserInfo.wTableUserCount;
}

//��������
bool CTableFrame::InitializationFrame(WORD wTableID, tagTableFrameParameter & TableFrameParameter)
{
	//���ñ���
	m_wTableID=wTableID;
	m_wChairCount=TableFrameParameter.pGameServiceAttrib->wChairCount;

	//���ò���
	m_pGameParameter=TableFrameParameter.pGameParameter;
	m_pGameServiceAttrib=TableFrameParameter.pGameServiceAttrib;
	m_pGameServiceOption=TableFrameParameter.pGameServiceOption;

	//����ӿ�
	m_pITimerEngine=TableFrameParameter.pITimerEngine;
	m_pIMainServiceFrame=TableFrameParameter.pIMainServiceFrame;
	m_pIAndroidUserManager=TableFrameParameter.pIAndroidUserManager;
	m_pIKernelDataBaseEngine=TableFrameParameter.pIKernelDataBaseEngine;
	m_pIRecordDataBaseEngine=TableFrameParameter.pIRecordDataBaseEngine;

	//��������
	IGameServiceManager * pIGameServiceManager=TableFrameParameter.pIGameServiceManager;
	m_pITableFrameSink=(ITableFrameSink *)pIGameServiceManager->CreateTableFrameSink(IID_ITableFrameSink,VER_ITableFrameSink);

	//�����ж�
	if (m_pITableFrameSink==NULL)
	{
		ASSERT_ERR(FALSE);
		return false;
	}

	//��������
	IUnknownEx * pITableFrame=QUERY_ME_INTERFACE(IUnknownEx);
	if (m_pITableFrameSink->Initialization(pITableFrame)==false) return false;

	//���ñ���
	m_lCellScore=m_pGameServiceOption->lCellScore;

	g_szFilePath;
	ZeroMemory(g_szFilePath,CountArray(g_szFilePath));
	GetCurrentDirectory(sizeof(g_szFilePath), g_szFilePath);

	//��չ�ӿ�
	m_pITableUserAction=QUERY_OBJECT_PTR_INTERFACE(m_pITableFrameSink,ITableUserAction);
	m_pITableUserRequest=QUERY_OBJECT_PTR_INTERFACE(m_pITableFrameSink,ITableUserRequest);

	m_ServerFileLog.SetRoot(TEXT("./SLog/UserAction/%s(Server)"), m_pGameServiceOption->szServerName);

	//��������ģʽ
	if((TableFrameParameter.pGameServiceOption->wServerType&GAME_GENRE_MATCH)!=0 && TableFrameParameter.pIGameMatchServiceManager!=NULL)
	{
		IUnknownEx * pIUnknownEx=QUERY_ME_INTERFACE(IUnknownEx);
		IGameMatchServiceManager * pIGameMatchServiceManager=QUERY_OBJECT_PTR_INTERFACE(TableFrameParameter.pIGameMatchServiceManager,IGameMatchServiceManager);
		if (pIGameMatchServiceManager==NULL)
		{
			ASSERT_ERR(FALSE);
			return false;
		}
		m_pIGameMatchSink=(IGameMatchSink *)pIGameMatchServiceManager->CreateGameMatchSink(IID_IGameMatchSink,VER_IGameMatchSink);

		//�����ж�
		if (m_pIGameMatchSink==NULL)
		{
			ASSERT_ERR(FALSE);
			return false;
		}

		//��չ�ӿ�
		m_pIMatchTableAction=QUERY_OBJECT_PTR_INTERFACE(m_pIGameMatchSink,ITableUserAction);
		if (m_pIGameMatchSink->InitTableFrameSink(pIUnknownEx)==false) 
		{
			return false;
		}
	}

	return true;
}

//��������
bool CTableFrame::PerformStandUpActionH5(IServerUserItem * pIServerUserItem, BYTE cbNotifyClient)
{
	//Ч�����
	ASSERT_ERR(pIServerUserItem != NULL);
	ASSERT_ERR(pIServerUserItem->GetTableID() == m_wTableID);
	ASSERT_ERR(pIServerUserItem->GetChairID() <= m_wChairCount);
	//I_TRACE(TEXT("��������"));
	//�û�����
	WORD wChairID = pIServerUserItem->GetChairID();
	WORD wTableID = pIServerUserItem->GetTableID();
	BYTE cbUserStatus = pIServerUserItem->GetUserStatus();
	IServerUserItem * pITableUserItem = GetTableUserItem(wChairID);

	//��Ϸ�û�
	if ((m_bGameStarted == true)/*&& m_cbStartMode!=START_MODE_TIME_CONTROL*/ && ((cbUserStatus == US_PLAYING) || (cbUserStatus == US_OFFLINE)))
	{
		if (m_pGameServiceOption->wRoomType == 4 || m_pGameServiceAttrib->wChairCount == MAX_CHAIR)
		{
			BYTE cbConcludeReason = (cbUserStatus == US_OFFLINE) ? GER_NETWORK_ERROR : GER_USER_LEAVE;
			m_pITableFrameSink->OnEventGameConclude(wChairID, pIServerUserItem, cbConcludeReason);
		}
	}

	I_TRACE(TEXT("��ǰ��[%d] �������[%s][%d-%d]  �������[%s][%d-%d]"),m_wTableID,pIServerUserItem->GetNickName(),pIServerUserItem->GetTableID(),pIServerUserItem->GetChairID(),
		pITableUserItem->GetNickName(),pITableUserItem->GetTableID(),pITableUserItem->GetChairID());

	//���ñ���
	if (pIServerUserItem == pITableUserItem)
	{
		//����Ԫ��
		if (m_lFrozenedScore[wChairID] != 0L)
		{
			pIServerUserItem->UnFrozenedUserScore(m_lFrozenedScore[wChairID]);
			m_lFrozenedScore[wChairID] = 0L;
		}

		//�¼�֪ͨ
		if (m_pITableUserAction != NULL)
		{
			m_pITableUserAction->OnActionUserStandUp(wChairID, pIServerUserItem, false); //������Ϸ������ᴥ����Ϸ�������ı�״̬
		}

		//��������
		ConcludeTable();

		//�û�״̬
		pIServerUserItem->SetClientReady(false);
		
		if ( pIServerUserItem->GetUserStatus() != US_FREE) //������Ϸʱ�иı�״̬
		{
			if (pIServerUserItem->GetUserStatus() == US_OFFLINE)
				pIServerUserItem->SetUserStatus(US_NULL, INVALID_TABLE, INVALID_CHAIR,cbNotifyClient);
			else {
// 				CString cs;
// 				cs.Format(TEXT("�������������������[%d] ���[%d] ����[%d-%d] ״̬[%d] ֪ͨ[%d]"), m_wTableID, pIServerUserItem->GetUserID(), pIServerUserItem->GetTableID(), pIServerUserItem->GetChairID(), pIServerUserItem->GetUserStatus(), cbNotifyClient);
// 				m_ServerFileLog.Log(cs.GetBuffer(0));
				pIServerUserItem->SetUserStatus(US_FREE, INVALID_TABLE, INVALID_CHAIR, cbNotifyClient);
			}
		}

		//���ñ���
		m_TableUserItemArray[wChairID] = NULL;

		return true;
	}
	else
	{
		//��������
		for (INT_PTR i = 0; i < m_LookonUserItemArray.GetCount(); i++)
		{
			if (pIServerUserItem == m_LookonUserItemArray[i])
			{
				//ɾ������
				m_LookonUserItemArray.RemoveAt(i);

				//�¼�֪ͨ
				if (m_pITableUserAction != NULL)
				{
					m_pITableUserAction->OnActionUserStandUp(wChairID, pIServerUserItem, true);
				}

				//�û�״̬
				pIServerUserItem->SetClientReady(false);
				pIServerUserItem->SetUserStatus(US_FREE, INVALID_TABLE, INVALID_CHAIR,cbNotifyClient);

				return true;
			}
		}

	}

	return true;
}

//��������
bool CTableFrame::PerformStandUpAction(IServerUserItem * pIServerUserItem, BYTE cbNotifyClient)
{
	//Ч�����
	ASSERT_ERR(pIServerUserItem!=NULL);
	ASSERT_ERR(pIServerUserItem->GetTableID()==m_wTableID);
	ASSERT_ERR(pIServerUserItem->GetChairID()<=m_wChairCount);

	tagUserInfo* pUserInfo = pIServerUserItem->GetUserInfo();
	if ( pUserInfo->cbClientKind == CLIENT_KIND_WEBSOCKET )
	{
		//I_TRACE(TEXT("�������� cbNotifyClient[%d]"),cbNotifyClient);
		return PerformStandUpActionH5(pIServerUserItem,cbNotifyClient);
	}

	//�û�����
	WORD wChairID=pIServerUserItem->GetChairID();
	WORD wTableID=pIServerUserItem->GetTableID();
	BYTE cbUserStatus=pIServerUserItem->GetUserStatus();
	IServerUserItem * pITableUserItem=GetTableUserItem(wChairID);

	//��Ϸ�û�
	if ((m_bGameStarted==true)/*&& m_cbStartMode!=START_MODE_TIME_CONTROL*/ &&((cbUserStatus==US_PLAYING)||(cbUserStatus==US_OFFLINE)))
	{
		if ( m_pGameServiceOption->wRoomType == 4 ) //���泡ǿ�˽�ɢ��Ϸ
		{
			BYTE cbConcludeReason=(cbUserStatus==US_OFFLINE)?GER_NETWORK_ERROR:GER_USER_LEAVE;
			m_pITableFrameSink->OnEventGameConclude(wChairID,pIServerUserItem,cbConcludeReason);
		}
		else if ( m_pGameServiceOption->wKindID == 301 ) //�����齫
		{
			BYTE cbConcludeReason=(cbUserStatus==US_OFFLINE)?GER_NETWORK_ERROR:GER_USER_LEAVE;
			m_pITableFrameSink->OnEventGameConclude(wChairID,pIServerUserItem,cbConcludeReason);
		}
		else
		{
			//�뿪�ж�
 			BYTE cbConcludeReason=(cbUserStatus==US_OFFLINE)?GER_NETWORK_ERROR:GER_USER_LEAVE;
 
 			if ( m_pGameServiceAttrib->wChairCount == MAX_CHAIR ) //����������
 				m_pITableFrameSink->OnEventGameConclude(wChairID,pIServerUserItem,cbConcludeReason);
 			
		}
	}


	//���ñ���
	if (pIServerUserItem==pITableUserItem)
	{
		//����Ԫ��
		if (m_lFrozenedScore[wChairID]!=0L)
		{
			pIServerUserItem->UnFrozenedUserScore(m_lFrozenedScore[wChairID]);
			m_lFrozenedScore[wChairID]=0L;
		}

		//�¼�֪ͨ
		if (m_pITableUserAction!=NULL)
		{
			m_pITableUserAction->OnActionUserStandUp(wChairID,pIServerUserItem,false);
		}

		//�¼�֪ͨ
		if(m_pIMatchTableAction!=NULL) m_pIMatchTableAction->OnActionUserStandUp(wChairID,pIServerUserItem,false);

		//��������
		ConcludeTable();

		//�û�״̬
		pIServerUserItem->SetClientReady(false);
		//I_TRACE(TEXT("����������:%d"),pIServerUserItem->GetUserID());
		pIServerUserItem->SetUserStatus((cbUserStatus==US_OFFLINE)?US_NULL:US_FREE,INVALID_TABLE,INVALID_CHAIR,cbNotifyClient);

		//���ñ���
		m_TableUserItemArray[wChairID] = NULL;

		//��ʼ�ж�
		if (EfficacyStartGame(INVALID_CHAIR)==true)
		{
			StartGame();
		}

		//��������
		bool bTableLocked=IsTableLocked();
		bool bTableStarted=IsTableStarted();
		WORD wTableUserCount=GetSitUserCount();

		//������Ϣ
		if (wTableUserCount==0)
		{
			m_dwTableOwnerID=0L;
			m_szEnterPassword[0]=0;
		}

		//�����Թ�
		if (wTableUserCount==0)
		{
			for (INT_PTR i=0;i<m_LookonUserItemArray.GetCount();i++)
			{
				SendGameMessage(m_LookonUserItemArray[i],TEXT("����Ϸ������������Ѿ��뿪�ˣ�"),SMT_CLOSE_GAME|SMT_EJECT);
			}
		}

		//����״̬
		if ((bTableLocked!=IsTableLocked())||(bTableStarted!=IsTableStarted()))
		{
			SendTableStatus();
		}

		return true;
	}
	else
	{
		//��������
		for (INT_PTR i=0;i<m_LookonUserItemArray.GetCount();i++)
		{
			if (pIServerUserItem==m_LookonUserItemArray[i])
			{
				//ɾ������
				m_LookonUserItemArray.RemoveAt(i);

				//�¼�֪ͨ
				if (m_pITableUserAction!=NULL)
				{
					m_pITableUserAction->OnActionUserStandUp(wChairID,pIServerUserItem,true);
				}

				//�¼�֪ͨ
				if(m_pIMatchTableAction!=NULL) m_pIMatchTableAction->OnActionUserStandUp(wChairID,pIServerUserItem,true);

				//�û�״̬
				pIServerUserItem->SetClientReady(false);
				pIServerUserItem->SetUserStatus(US_FREE,INVALID_TABLE,INVALID_CHAIR, cbNotifyClient);

				return true;
			}
		}
		
		//�������
		ASSERT_ERR(FALSE);
	}

	return true;
}

//�Թ۶���
bool CTableFrame::PerformLookonAction(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	//Ч�����
	ASSERT_ERR((pIServerUserItem!=NULL)&&(wChairID<m_wChairCount));
	ASSERT_ERR((pIServerUserItem->GetTableID()==INVALID_TABLE)&&(pIServerUserItem->GetChairID()==INVALID_CHAIR));

	//��������
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();
	tagUserRule * pUserRule=pIServerUserItem->GetUserRule();
	IServerUserItem * pITableUserItem=GetTableUserItem(wChairID);

	//��Ϸ״̬
	if ((m_bGameStarted==false)&&(pIServerUserItem->GetMasterOrder()==0L))
	{
		SendRequestFailure(pIServerUserItem,TEXT("��Ϸ��û�п�ʼ�������Թ۴���Ϸ����"),REQUEST_FAILURE_NORMAL);
		return false;
	}

	//ģ�⴦��
	if (m_pGameServiceAttrib->wChairCount < MAX_CHAIR && pIServerUserItem->IsAndroidUser()==false)
	{
		//�������
		CAttemperEngineSink * pAttemperEngineSink=(CAttemperEngineSink *)m_pIMainServiceFrame;

		//���һ���
		for (WORD i=0; i<m_pGameServiceAttrib->wChairCount; i++)
		{
			//��ȡ�û�
			IServerUserItem *pIUserItem=m_TableUserItemArray[i];
			if(pIUserItem==NULL) continue;
			if(pIUserItem->IsAndroidUser()==false)break;

			//��ȡ����
			tagBindParameter * pBindParameter=pAttemperEngineSink->GetBindParameter(pIUserItem->GetBindIndex());
			IAndroidUserItem * pIAndroidUserItem=m_pIAndroidUserManager->SearchAndroidUserItem(pIUserItem->GetUserID(),pBindParameter->dwSocketID);
			tagAndroidParameter * pAndroidParameter=pIAndroidUserItem->GetAndroidParameter();

			//ģ���ж�
			if((pAndroidParameter->dwServiceGender&ANDROID_SIMULATE)!=0
				&& (pAndroidParameter->dwServiceGender&ANDROID_PASSIVITY)==0
				&& (pAndroidParameter->dwServiceGender&ANDROID_INITIATIVE)==0)
			{
				SendRequestFailure(pIServerUserItem,TEXT("��Ǹ����ǰ��Ϸ���ӽ�ֹ�û��Թۣ�"),REQUEST_FAILURE_NORMAL);
				return false;
			}

			break;
		}
	}


	//�Թ��ж�
	if (CServerRule::IsAllowAndroidSimulate(m_pGameServiceOption->dwServerRule)==true
		&& (CServerRule::IsAllowAndroidAttend(m_pGameServiceOption->dwServerRule)==false))
	{
		if ((pITableUserItem!=NULL)&&(pITableUserItem->IsAndroidUser()==true))
		{
			SendRequestFailure(pIServerUserItem,TEXT("��Ǹ����ǰ��Ϸ�����ֹ�û��Թۣ�"),REQUEST_FAILURE_NORMAL);
			return false;
		}
	}

	//״̬�ж�
	if ((CServerRule::IsForfendGameLookon(m_pGameServiceOption->dwServerRule)==true)&&(pIServerUserItem->GetMasterOrder()==0))
	{
		SendRequestFailure(pIServerUserItem,TEXT("��Ǹ����ǰ��Ϸ�����ֹ�û��Թۣ�"),REQUEST_FAILURE_NORMAL);
		return false;
	}

	//�����ж�
	if ((pITableUserItem==NULL)&&(pIServerUserItem->GetMasterOrder()==0L))
	{
		SendRequestFailure(pIServerUserItem,TEXT("���������λ��û����Ϸ��ң��޷��Թ۴���Ϸ��"),REQUEST_FAILURE_NORMAL);
		return false;
	}

	//����Ч��
	if ((IsTableLocked()==true)&&(pIServerUserItem->GetMasterOrder()==0L)&&(lstrcmp(pUserRule->szPassword,m_szEnterPassword)!=0))
	{
		SendRequestFailure(pIServerUserItem,TEXT("��Ϸ���������벻��ȷ�������Թ���Ϸ��"),REQUEST_FAILURE_PASSWORD);
		return false;
	}

	//��չЧ��
	if (m_pITableUserRequest!=NULL)
	{
		//��������
		tagRequestResult RequestResult;
		ZeroMemory(&RequestResult,sizeof(RequestResult));

		//����Ч��
		if (m_pITableUserRequest->OnUserRequestLookon(wChairID,pIServerUserItem,RequestResult)==false)
		{
			//������Ϣ
			SendRequestFailure(pIServerUserItem,RequestResult.szFailureReason,RequestResult.cbFailureCode);

			return false;
		}
	}

	//�����û�
	m_LookonUserItemArray.Add(pIServerUserItem);

	//�û�״̬
	pIServerUserItem->SetClientReady(false);
	pIServerUserItem->SetUserStatus(US_LOOKON,m_wTableID,wChairID);

	//�¼�֪ͨ
	if (m_pITableUserAction!=NULL)
	{
		m_pITableUserAction->OnActionUserSitDown(wChairID,pIServerUserItem,true);
	}

	//�¼�֪ͨ
	if(m_pIMatchTableAction!=NULL) m_pIMatchTableAction->OnActionUserSitDown(wChairID,pIServerUserItem,true);
	return true;
}

//���¶���
bool CTableFrame::PerformSitDownActionH5(WORD wChairID, IServerUserItem * pIServerUserItem, LPCTSTR lpszPassword)
{
	//Ч�����
	ASSERT_ERR((pIServerUserItem != NULL) && (wChairID < m_wChairCount));
	ASSERT_ERR((pIServerUserItem->GetTableID() == INVALID_TABLE) && (pIServerUserItem->GetChairID() == INVALID_CHAIR));

	//��������
	tagUserInfo * pUserInfo = pIServerUserItem->GetUserInfo();
	tagUserRule * pUserRule = pIServerUserItem->GetUserRule();
	IServerUserItem * pITableUserItem = GetTableUserItem(wChairID);

	//���ֱ���
	SCORE lUserScore = pIServerUserItem->GetUserScore();
	SCORE lMinTableScore = m_pGameServiceOption->lMinTableScore;  //�����
	SCORE lLessEnterScore = m_pITableFrameSink->QueryLessEnterScore(wChairID, pIServerUserItem);

	CAttemperEngineSink * pAttemperEngineSink = (CAttemperEngineSink *)m_pIMainServiceFrame;
	tagBindParameter * pBindParameter = pAttemperEngineSink->GetBindParameter(pIServerUserItem->GetBindIndex());
	if (pBindParameter == NULL) return false;
	//������Ϣ
	TCHAR szDescribe[128] = TEXT("");

	//��̬����
	bool bDynamicJoin = true;
	if (m_pGameServiceAttrib->cbDynamicJoin == FALSE) bDynamicJoin = false;
	if (CServerRule::IsAllowDynamicJoin(m_pGameServiceOption->dwServerRule) == false) bDynamicJoin = false;

	//��Ϸ״̬
	if ((m_bGameStarted == true) && (bDynamicJoin == false))
	{
		_sntprintf(szDescribe,CountArray(szDescribe),TEXT("��Ϸ�Ѿ���ʼ�ˣ����ڲ��ܽ�����Ϸ��!"));
		m_pIMainServiceFrame->SendLogonFailure(szDescribe, 0, pBindParameter->dwSocketID);
		I_TRACE(szDescribe);
		return false;
	}

	//�����ж�
	if (pITableUserItem != NULL)
	{
		_sntprintf(szDescribe, CountArray(szDescribe), TEXT("�����Ѿ��� [ %s ] ռ���ˣ��´ζ���Ҫ���Ӵ��"), pITableUserItem->GetNickName());
		I_TRACE(TEXT("%s"), szDescribe);
		//������Ϣ
		m_pIMainServiceFrame->SendLogonFailure(szDescribe, 0, pBindParameter->dwSocketID);
		return false;
	}

	//����Ч��
	if (((IsTableLocked() == true) && (pIServerUserItem->GetMasterOrder() == 0L))
		&& ((lpszPassword == NULL) || (lstrcmp(lpszPassword, m_szEnterPassword) != 0)))
	{
		m_pIMainServiceFrame->SendLogonFailure(TEXT("��Ϸ���������벻��ȷ�����ܼ�����Ϸ��"), 0, pBindParameter->dwSocketID);
		I_TRACE(TEXT("��Ϸ���������벻��ȷ�����ܼ�����Ϸ��"));
		return false;
	}

	//��������
	if ((m_pGameServiceOption->lMinEnterScore != 0L) && (lUserScore < m_pGameServiceOption->lMinEnterScore))
	{
		_sntprintf(szDescribe, CountArray(szDescribe), TEXT("������Ҫ%0.2f�Ľ�ң����Ľ�Ҳ��������ܼ��룡"), (float)m_pGameServiceOption->lMinEnterScore / 1000);
		I_TRACE(TEXT("%s"), szDescribe);
		//������Ϣ
		m_pIMainServiceFrame->SendLogonFailure(szDescribe, 0, pBindParameter->dwSocketID);

		return false;
	}


	//���ñ���
	m_TableUserItemArray[wChairID] = pIServerUserItem;

	//�û�״̬
	if ((IsGameStarted() == false) || (m_cbStartMode != START_MODE_TIME_CONTROL))
	{
		pIServerUserItem->SetClientReady(false);
		//pIServerUserItem->SetUserStatus(US_SIT, m_wTableID, wChairID);
		I_TRACE(TEXT("%s��ʼ���� %d-%d"),pIServerUserItem->GetNickName(),m_wTableID,wChairID);
		pIServerUserItem->SetUserStatus(US_READY, m_wTableID, wChairID);
	}
	else
	{
		//���ñ���
		m_wOffLineCount[wChairID] = 0L;
		m_dwOffLineTime[wChairID] = 0L;

		//����Ԫ��
		if (m_pGameServiceOption->lServiceScore > 0L)
		{
			m_lFrozenedScore[wChairID] = m_pGameServiceOption->lServiceScore;

			pIServerUserItem->FrozenedUserScore(m_pGameServiceOption->lServiceScore);
		}

		//����״̬
		pIServerUserItem->SetClientReady(false);
		pIServerUserItem->SetUserStatus(US_PLAYING, m_wTableID, wChairID);
	}

	//�¼�֪ͨ
	if (m_pITableUserAction != NULL)
	{
		m_pITableUserAction->OnActionUserSitDown(wChairID, pIServerUserItem, false);
	}

	return true;
}

//���¶���
bool CTableFrame::PerformSitDownAction(WORD wChairID, IServerUserItem * pIServerUserItem, LPCTSTR lpszPassword)
{
	//Ч�����
	ASSERT_ERR((pIServerUserItem!=NULL)&&(wChairID<m_wChairCount));
	ASSERT_ERR((pIServerUserItem->GetTableID()==INVALID_TABLE)&&(pIServerUserItem->GetChairID()==INVALID_CHAIR));

	//��������
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();

	if (pUserInfo && pUserInfo->cbClientKind == CLIENT_KIND_WEBSOCKET )
	{
		return PerformSitDownActionH5(wChairID, pIServerUserItem, lpszPassword);
	}

	tagUserRule * pUserRule=pIServerUserItem->GetUserRule();
	IServerUserItem * pITableUserItem=GetTableUserItem(wChairID);

	//���ֱ���
	SCORE lUserScore=pIServerUserItem->GetUserScore();
	SCORE lMinTableScore=m_pGameServiceOption->lMinTableScore;  //�����
	SCORE lLessEnterScore=m_pITableFrameSink->QueryLessEnterScore(wChairID,pIServerUserItem);

	CAttemperEngineSink * pAttemperEngineSink = (CAttemperEngineSink *)m_pIMainServiceFrame;
	tagBindParameter * pBindParameter = pAttemperEngineSink->GetBindParameter(pIServerUserItem->GetBindIndex());

	m_wDrawCount = 0;

// 	//״̬�ж�
// 	if ((CServerRule::IsForfendGameEnter(m_pGameServiceOption->dwServerRule)==true)&&(pIServerUserItem->GetMasterOrder()==0))
// 	{
// 		SendRequestFailure(pIServerUserItem,TEXT("��Ǹ����ǰ��Ϸ���ӽ�ֹ�û����룡"),REQUEST_FAILURE_NORMAL);
// 		return false;
// 	}

	//ģ�⴦��
// 	if (m_pGameServiceAttrib->wChairCount < MAX_CHAIR && pIServerUserItem->IsAndroidUser()==false)
// 	{
// 		//�������
// 		CAttemperEngineSink * pAttemperEngineSink=(CAttemperEngineSink *)m_pIMainServiceFrame;
// 
// 		//���һ���
// 		for (WORD i=0; i<m_pGameServiceAttrib->wChairCount; i++)
// 		{
// 			//��ȡ�û�
// 			IServerUserItem *pIUserItem=m_TableUserItemArray[i];
// 			if(pIUserItem==NULL) continue;
// 			if(pIUserItem->IsAndroidUser()==false)break;
// 
// 			//��ȡ����
// 			tagBindParameter * pBindParameter=pAttemperEngineSink->GetBindParameter(pIUserItem->GetBindIndex());
// 			IAndroidUserItem * pIAndroidUserItem=m_pIAndroidUserManager->SearchAndroidUserItem(pIUserItem->GetUserID(),pBindParameter->dwSocketID);
// 			tagAndroidParameter * pAndroidParameter=pIAndroidUserItem->GetAndroidParameter();
// 
// 			//ģ���ж�
// 			if((pAndroidParameter->dwServiceGender&ANDROID_SIMULATE)!=0
// 				&& (pAndroidParameter->dwServiceGender&ANDROID_PASSIVITY)==0
// 				&& (pAndroidParameter->dwServiceGender&ANDROID_INITIATIVE)==0)
// 			{
// 				SendRequestFailure(pIServerUserItem,TEXT("��Ǹ����ǰ��Ϸ���ӽ�ֹ�û����룡"),REQUEST_FAILURE_NORMAL);
// 				return false;
// 			}
// 
// 			break;
// 		}
// 	}

	//��̬����
	bool bDynamicJoin=true;
	if (m_pGameServiceAttrib->cbDynamicJoin==FALSE) bDynamicJoin=false;
	if (CServerRule::IsAllowDynamicJoin(m_pGameServiceOption->dwServerRule)==false) bDynamicJoin=false;

	//��Ϸ״̬
	if ((m_bGameStarted==true)&&(bDynamicJoin==false))
	{
		m_pIMainServiceFrame->SendLogonFailure(TEXT("��Ϸ�Ѿ���ʼ�ˣ����ڲ��ܽ�����Ϸ����"),0,pBindParameter->dwSocketID);
		I_TRACE(TEXT("��Ϸ�Ѿ���ʼ�ˣ����ڲ��ܽ�����Ϸ��"));
		return false;
	}

	//�����ж�
	if (pITableUserItem!=NULL)
	{
		//������Ϣ
		TCHAR szDescribe[128]=TEXT("");
		_sntprintf(szDescribe,CountArray(szDescribe),TEXT("�����Ѿ��� [ %s ] ռ���ˣ��´ζ���Ҫ���Ӵ��"),pITableUserItem->GetNickName());
			I_TRACE(TEXT("%s"),szDescribe);
		//������Ϣ
		m_pIMainServiceFrame->SendLogonFailure(szDescribe,0,pBindParameter->dwSocketID);
		

		return false;
	}

	//����Ч��
	if(((IsTableLocked()==true)&&(pIServerUserItem->GetMasterOrder()==0L))
		&&((lpszPassword==NULL)||(lstrcmp(lpszPassword,m_szEnterPassword)!=0)))
	{
		m_pIMainServiceFrame->SendLogonFailure(TEXT("��Ϸ���������벻��ȷ�����ܼ�����Ϸ��"),0,pBindParameter->dwSocketID);
		I_TRACE(TEXT("��Ϸ���������벻��ȷ�����ܼ�����Ϸ��"));
		return false;
	}
	
	//��������
	if ((m_pGameServiceOption->lMinEnterScore != 0L) && (lUserScore < m_pGameServiceOption->lMinEnterScore))
	{
		//������Ϣ
		TCHAR szDescribe[128]=TEXT("");
		_sntprintf(szDescribe,CountArray(szDescribe),TEXT("������Ϸ������Ҫ%0.2f�Ľ�ң����Ľ�Ҳ��������ܼ��룡"),lMinTableScore/1000);
		I_TRACE(TEXT("%s"),szDescribe);
		//������Ϣ
		m_pIMainServiceFrame->SendLogonFailure(szDescribe,0,pBindParameter->dwSocketID);
		ToSaveScore(pIServerUserItem);

		return false;
	}

	//����Ч��
	//if (EfficacyIPAddress(pIServerUserItem)==false) return false;
	//if (EfficacyScoreRule(pIServerUserItem)==false) return false;

	//��չЧ��
	if (m_pITableUserRequest!=NULL)
	{
		//��������
		tagRequestResult RequestResult;
		ZeroMemory(&RequestResult,sizeof(RequestResult));

		//����Ч��
		if (m_pITableUserRequest->OnUserRequestSitDown(wChairID,pIServerUserItem,RequestResult)==false)
		{
			//������Ϣ
			m_pIMainServiceFrame->SendLogonFailure(RequestResult.szFailureReason,RequestResult.cbFailureCode,pBindParameter->dwSocketID);
			I_TRACE(TEXT("%s"),RequestResult.szFailureReason);
			return false;
		}
	}

	//���ñ���
	m_TableUserItemArray[wChairID]=pIServerUserItem;

	//�û�״̬
	if ((IsGameStarted()==false)||(m_cbStartMode!=START_MODE_TIME_CONTROL))
	{
// 		if ((m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)==0)
// 		{
// 			pIServerUserItem->SetClientReady(false);
// 			pIServerUserItem->SetUserStatus(US_SIT,m_wTableID,wChairID);
// 		}
//  		else
 		{
 			pIServerUserItem->SetClientReady(false);
 			pIServerUserItem->SetUserStatus(US_READY,m_wTableID,wChairID);
 		}
	}
	else
	{
		//���ñ���
		m_wOffLineCount[wChairID]=0L;
		m_dwOffLineTime[wChairID]=0L;

		//����Ԫ��
		if (m_pGameServiceOption->lServiceScore>0L)
		{
			m_lFrozenedScore[wChairID]=m_pGameServiceOption->lServiceScore;

			pIServerUserItem->FrozenedUserScore(m_pGameServiceOption->lServiceScore);
		}

		//����״̬
		pIServerUserItem->SetClientReady(false);
		pIServerUserItem->SetUserStatus(US_PLAYING,m_wTableID,wChairID);
	}

	//������Ϣ
	if (GetSitUserCount()==1)
	{
		//״̬����
		bool bTableLocked=IsTableLocked();

		//���ñ���
		m_dwTableOwnerID=pIServerUserItem->GetUserID();
		lstrcpyn(m_szEnterPassword,pUserRule->szPassword,CountArray(m_szEnterPassword));

		//����״̬
		if (bTableLocked!=IsTableLocked()) SendTableStatus();
	}	

	//�¼�֪ͨ
	if (m_pITableUserAction!=NULL)
	{
		m_pITableUserAction->OnActionUserSitDown(wChairID,pIServerUserItem,false);
	}

	//�¼�֪ͨ
	if(m_pIMatchTableAction!=NULL) m_pIMatchTableAction->OnActionUserSitDown(wChairID,pIServerUserItem,false);

	//SetGameTimer(IDI_USER_READY_DETECT,32*1000,1,0);

	return true;
}

//����״̬
bool CTableFrame::SendTableStatus()
{
	//�ͻ��˲��������Ϣ
	return true;
	//��������
	CMD_GR_TableStatus TableStatus;
	ZeroMemory(&TableStatus,sizeof(TableStatus));

	//��������
	TableStatus.wTableID=m_wTableID;
	TableStatus.TableStatus.cbTableLock=IsTableLocked()?TRUE:FALSE;
	TableStatus.TableStatus.cbPlayStatus=IsTableStarted()?TRUE:FALSE;

	

	//��������
	m_pIMainServiceFrame->SendData(BG_COMPUTER,MDM_GR_STATUS,SUB_GR_TABLE_STATUS,&TableStatus,sizeof(TableStatus));

	//�ֻ�����

	return true;
}

//����ʧ��
bool CTableFrame::SendRequestFailure(IServerUserItem * pIServerUserItem, LPCTSTR pszDescribe, LONG lErrorCode)
{
	//��������
	CMD_GR_RequestFailure RequestFailure;
	ZeroMemory(&RequestFailure,sizeof(RequestFailure));

	//��������
	RequestFailure.lErrorCode=lErrorCode;
	lstrcpyn(RequestFailure.szDescribeString,pszDescribe,CountArray(RequestFailure.szDescribeString));

	//��������
	WORD wDataSize=CountStringBuffer(RequestFailure.szDescribeString);
	WORD wHeadSize=sizeof(RequestFailure)-sizeof(RequestFailure.szDescribeString);
	m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_REQUEST_FAILURE,&RequestFailure,wHeadSize+wDataSize);

	return true;
}

//��ʼЧ��
bool CTableFrame::EfficacyStartGame(WORD wReadyChairID)
{
	//״̬�ж�
	if (m_bGameStarted==true) return false;

	//ģʽ����
	if (m_cbStartMode==START_MODE_TIME_CONTROL) return false;
	if (m_cbStartMode==START_MODE_MASTER_CONTROL) return false;

	//׼������
	WORD wReadyUserCount=0;
	for (WORD i=0;i<m_wChairCount;i++)
	{
		//��ȡ�û�
		IServerUserItem * pITableUserItem=GetTableUserItem(i);
		if (pITableUserItem==NULL) continue;

		//�û�ͳ��
		if (pITableUserItem!=NULL)
		{
			//״̬�ж�
			if (pITableUserItem->IsClientReady()==false) return false;
			BYTE cbUserStatus = pITableUserItem->GetUserStatus();
			//�������������û������ж�״̬�����û������Ѿ�����
			if ((wReadyChairID!=i)&&(cbUserStatus!=US_READY)) return false;

			//�û�����
			wReadyUserCount++;
		}
	}

	//��ʼ����
	switch (m_cbStartMode)
	{
	case START_MODE_ALL_READY:			//����׼��
		{
			//��Ŀ�ж�
			if ( m_pGameServiceOption->wKindID==GAME_CALLBAN_OX)
			{
				if (wReadyUserCount >= 3L) return true;
			}
			else
			{
				if (wReadyUserCount >= 2L) return true;
			}

			return false;
		}
	case START_MODE_FULL_READY:			//���˿�ʼ
		{
			//�����ж�
			if (wReadyUserCount==m_wChairCount) return true;

			return false;
		}
	case START_MODE_PAIR_READY:			//��Կ�ʼ
		{
			//��Ŀ�ж�
			if (wReadyUserCount==m_wChairCount) return true;
			if ((wReadyUserCount<2L)||(wReadyUserCount%2)!=0) return false;

			//λ���ж�
			for (WORD i=0;i<m_wChairCount/2;i++)
			{
				//��ȡ�û�
				IServerUserItem * pICurrentUserItem=GetTableUserItem(i);
				IServerUserItem * pITowardsUserItem=GetTableUserItem(i+m_wChairCount/2);

				//λ�ù���
				if ((pICurrentUserItem==NULL)&&(pITowardsUserItem!=NULL)) return false;
				if ((pICurrentUserItem!=NULL)&&(pITowardsUserItem==NULL)) return false;
			}

			return true;
		}
	case START_MODE_SYSTRM_BANKER:		// ϵͳ��ׯ   ZZL
		{
			if(wReadyUserCount >= 1L)
				return true;
			return false;
		}
	default:
		{
			ASSERT_ERR(FALSE);
			return false;
		}
	}

	return false;
}

//��ַЧ��
bool CTableFrame::EfficacyIPAddress(IServerUserItem * pIServerUserItem)
{
	//����Ա��������
	if(pIServerUserItem->GetMasterOrder()!=0) return true;

	//�����ж�
	if (CServerRule::IsForfendGameRule(m_pGameServiceOption->dwServerRule)==true) return true;

	//��ַЧ��
	const tagUserRule * pUserRule=pIServerUserItem->GetUserRule(),*pTableUserRule=NULL;
	bool bCheckSameIP=pUserRule->bLimitSameIP;
	for (WORD i=0;i<m_wChairCount;i++)
	{
		//��ȡ�û�
		IServerUserItem * pITableUserItem=GetTableUserItem(i);
		if (pITableUserItem!=NULL && (!pITableUserItem->IsAndroidUser()) && (pITableUserItem->GetMasterOrder()==0))
		{
			pTableUserRule=pITableUserItem->GetUserRule();
			if (pTableUserRule->bLimitSameIP==true) 
			{
				bCheckSameIP=true;
				break;
			}
		}
	}

	//��ַЧ��
	if (bCheckSameIP==true)
	{
		DWORD dwUserIP=pIServerUserItem->GetClientAddr();
		for (WORD i=0;i<m_wChairCount;i++)
		{
			//��ȡ�û�
			IServerUserItem * pITableUserItem=GetTableUserItem(i);
			if ((pITableUserItem!=NULL)&&(pITableUserItem != pIServerUserItem)&&(!pITableUserItem->IsAndroidUser())&&(pITableUserItem->GetMasterOrder()==0)&&(pITableUserItem->GetClientAddr()==dwUserIP))
			{
				if (!pUserRule->bLimitSameIP)
				{
					//������Ϣ
					LPCTSTR pszDescribe=TEXT("����Ϸ����������˲�����ͬ IP ��ַ�������Ϸ���� IP ��ַ�����ҵ� IP ��ַ��ͬ�����ܼ�����Ϸ��");
					SendRequestFailure(pIServerUserItem,pszDescribe,REQUEST_FAILURE_QUIT);
					return false;
				}
				else
				{
					//������Ϣ
					LPCTSTR pszDescribe=TEXT("�������˲�����ͬ IP ��ַ�������Ϸ������Ϸ���������� IP ��ַ��ͬ����ң����ܼ�����Ϸ��");
					SendRequestFailure(pIServerUserItem,pszDescribe,REQUEST_FAILURE_QUIT);
					return false;
				}
			}
		}
		for (WORD i=0;i<m_wChairCount-1;i++)
		{
			//��ȡ�û�
			IServerUserItem * pITableUserItem=GetTableUserItem(i);
			if (pITableUserItem!=NULL && (!pITableUserItem->IsAndroidUser()) && (pITableUserItem->GetMasterOrder()==0))
			{
				for (WORD j=i+1;j<m_wChairCount;j++)
				{
					//��ȡ�û�
					IServerUserItem * pITableNextUserItem=GetTableUserItem(j);
					if ((pITableNextUserItem!=NULL) && (!pITableNextUserItem->IsAndroidUser()) && (pITableNextUserItem->GetMasterOrder()==0)&&(pITableUserItem->GetClientAddr()==pITableNextUserItem->GetClientAddr()))
					{
						LPCTSTR pszDescribe=TEXT("�������˲�����ͬ IP ��ַ�������Ϸ������Ϸ������ IP ��ַ��ͬ����ң����ܼ�����Ϸ��");
						SendRequestFailure(pIServerUserItem,pszDescribe,REQUEST_FAILURE_QUIT);
						return false;
					}
				}
			}
		}
	}
	return true;
}

//����Ч��
bool CTableFrame::EfficacyScoreRule(IServerUserItem * pIServerUserItem)
{
	//����Ա��������
	if(pIServerUserItem->GetMasterOrder()!=0) return true;

	//�����ж�
	if (CServerRule::IsForfendGameRule(m_pGameServiceOption->dwServerRule)==true) return true;

	//��������
	WORD wWinRate=pIServerUserItem->GetUserWinRate();
	WORD wFleeRate=pIServerUserItem->GetUserFleeRate();

	//���ַ�Χ
	for (WORD i=0;i<m_wChairCount;i++)
	{
		//��ȡ�û�
		IServerUserItem * pITableUserItem=GetTableUserItem(i);

		//����Ч��
		if (pITableUserItem!=NULL)
		{
			//��ȡ����
			tagUserRule * pTableUserRule=pITableUserItem->GetUserRule();

			//����Ч��
			if ((pTableUserRule->bLimitFleeRate)&&(wFleeRate>pTableUserRule->wMaxFleeRate))
			{
				//������Ϣ
				TCHAR szDescribe[128]=TEXT("");
				_sntprintf(szDescribe,CountArray(szDescribe),TEXT("����������̫�ߣ��� %s ���õ����ò��������ܼ�����Ϸ��"),pITableUserItem->GetNickName());

				//������Ϣ
				SendRequestFailure(pIServerUserItem,szDescribe,REQUEST_FAILURE_QUIT);

				return false;
			}

			//ʤ��Ч��
			if ((pTableUserRule->bLimitWinRate)&&(wWinRate<pTableUserRule->wMinWinRate))
			{
				//������Ϣ
				TCHAR szDescribe[128]=TEXT("");
				_sntprintf(szDescribe,CountArray(szDescribe),TEXT("����ʤ��̫�ͣ��� %s ���õ����ò��������ܼ�����Ϸ��"),pITableUserItem->GetNickName());

				//������Ϣ
				SendRequestFailure(pIServerUserItem,szDescribe,REQUEST_FAILURE_NORMAL);

				return false;
			}

			//����Ч��
			if (pTableUserRule->bLimitGameScore==true)
			{
				//��߻���
				if (pIServerUserItem->GetUserScore()>pTableUserRule->lMaxGameScore)
				{
					//������Ϣ
					TCHAR szDescribe[128]=TEXT("");
					_sntprintf(szDescribe,CountArray(szDescribe),TEXT("���Ļ���̫�ߣ��� %s ���õ����ò��������ܼ�����Ϸ��"),pITableUserItem->GetNickName());

					//������Ϣ
					SendRequestFailure(pIServerUserItem,szDescribe,REQUEST_FAILURE_QUIT);

					return false;
				}

				//��ͻ���
				if (pIServerUserItem->GetUserScore()<pTableUserRule->lMinGameScore)
				{
					//������Ϣ
					TCHAR szDescribe[128]=TEXT("");
					_sntprintf(szDescribe,CountArray(szDescribe),TEXT("���Ļ���̫�ͣ��� %s ���õ����ò��������ܼ�����Ϸ��"),pITableUserItem->GetNickName());

					//������Ϣ
					SendRequestFailure(pIServerUserItem,szDescribe,REQUEST_FAILURE_QUIT);

					return false;
				}
			}
		}
	}

	return true;
}
