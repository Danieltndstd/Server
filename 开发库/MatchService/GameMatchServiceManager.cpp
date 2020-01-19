#include "StdAfx.h"
#include "AfxTempl.h"
#include "GameMatchSink.h"
#include "GameMatchServiceManager.h"

#include "..\..\Server\GameServer\DataBasePacket.h"
#include "..\..\Server\GameServer\AttemperEngineSink.h"
//////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////

int ReturnWeekDay( unsigned int iYear, unsigned int iMonth, unsigned int iDay )
{
	int iWeek = 0;
	unsigned int y = 0, c = 0, m = 0, d = 0;

	if ( iMonth == 1 || iMonth == 2 )
	{
		c = ( iYear - 1 ) / 100;
		y = ( iYear - 1 ) % 100;
		m = iMonth + 12;
		d = iDay;
	}
	else
	{
		c = iYear / 100;
		y = iYear % 100;
		m = iMonth;
		d = iDay;
	}

	iWeek = y + y / 4 + c / 4 - 2 * c + 26 * ( m + 1 ) / 10 + d - 1;    //���չ�ʽ
	iWeek = iWeek >= 0 ? ( iWeek % 7 ) : ( iWeek % 7 + 7 );    //iWeekΪ��ʱȡģ
	if ( iWeek == 0 )    //�����ղ���Ϊһ�ܵĵ�һ��
	{
		iWeek = 7;
	}

	return iWeek;
}

//���캯��
CGameMatchServiceManager::CGameMatchServiceManager(void)
{
	m_pIGameServiceFrame=NULL;
	m_pITCPNetworkEngineEvent=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITimerEngine=NULL;
	m_pIServerUserManager=NULL;
	m_pGameServiceOption=NULL;
	m_pGameServiceAttrib=NULL;
	m_ppITableFrame=NULL;
	m_pAndroidUserManager=NULL;
	m_bTableUsed=NULL;
	m_pCurMatch=NULL;
	ZeroMemory(&m_guidRoom, sizeof(m_guidRoom));
	m_bMatchOver = false;
	m_cbRealUserJoin = 0;

	memset(&m_MatchGlobal,0,sizeof(m_MatchGlobal));
	return;
}

//��������
CGameMatchServiceManager::~CGameMatchServiceManager(void)
{
	//�������
	//SafeDelete(m_pGameMatchSink);
	SafeDeleteArray(m_ppITableFrame);
	SafeDeleteArray(m_bTableUsed);
	INT_PTR nCount=m_TimerItemFree.GetCount();
	for (INT_PTR i=0;i<nCount;i++)
		SafeDelete(m_TimerItemFree.GetAt(i));
	m_TimerItemFree.RemoveAll();

	nCount=m_TimerItemActive.GetCount();
	for (INT_PTR i=0;i<nCount;i++)
		SafeDelete(m_TimerItemActive.GetAt(i));
	m_TimerItemActive.RemoveAll();
	m_UnRecovery.RemoveAll();
	
}
//ֹͣ����
bool  CGameMatchServiceManager::StopService()
{
	m_OnMatchUserItem.RemoveAll();
	
	SafeDeleteArray(m_ppITableFrame);
	SafeDeleteArray(m_bTableUsed);
	INT_PTR nCount=m_TimerItemFree.GetCount();
	for (INT_PTR i=0;i<nCount;i++)
		SafeDelete(m_TimerItemFree.GetAt(i));
	m_TimerItemFree.RemoveAll();

	nCount=m_TimerItemActive.GetCount();
	for (INT_PTR i=0;i<nCount;i++)
		SafeDelete(m_TimerItemActive.GetAt(i));
	m_TimerItemActive.RemoveAll();

	for(INT_PTR i=0; i<m_MatchGroup.GetCount();i++)
		SafeDelete(m_MatchGroup.GetAt(i));
	m_MatchGroup.RemoveAll();

	return true;
}
//��������
bool  CGameMatchServiceManager::StartService()
{
	ReadMatchRule();
	//CTraceService::szLogRoot.Format(TEXT("./SLog/GameServer/������־/%s"),m_pGameServiceOption->szServerName);
	return true;
}
//�ӿڲ�ѯ
void *  CGameMatchServiceManager::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameMatchServiceManager,Guid,dwQueryVer);
	QUERYINTERFACE(IChampionshipMatchSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameMatchServiceManager,Guid,dwQueryVer);
	return NULL;
}

//��ʼ���ӿ�
bool  CGameMatchServiceManager::InitMatchInterface(ITCPNetworkEngineEvent *pTCPNetworkEngine,IDataBaseEngine *pIDataBaseEngine, IServerUserManager *pIServerUserManager,
								 IMainServiceFrame *pIMainServiceFrame,ITimerEngine *pITimerEngine,IAndroidUserManager *pAndroidUserManager, WCHAR guidRoom[])
{
	if(pTCPNetworkEngine==NULL || pIDataBaseEngine==NULL || pIServerUserManager==NULL || pIMainServiceFrame==NULL || pITimerEngine==NULL) return false;

	m_pITCPNetworkEngineEvent=pTCPNetworkEngine;
	m_pIDataBaseEngine=pIDataBaseEngine;
	m_pIServerUserManager=pIServerUserManager;
	m_pIGameServiceFrame=pIMainServiceFrame;
	m_pITimerEngine=pITimerEngine;
	m_pAndroidUserManager=pAndroidUserManager;

	CopyMemory(m_guidRoom, guidRoom, 40 * sizeof WCHAR);

	return true;
}


//��������������˾������ݿ�
bool  CGameMatchServiceManager::OnUserJoinGame(IServerUserItem * pIServerUserItem)
{
	if ( MatchJoinCheck(pIServerUserItem) == false ) return false;
	//if ( MatchJoinTimeCheck(pIServerUserItem) == false ) return false;
	if(m_pCurMatch->OnUserSignUp(pIServerUserItem) == false ) return false;
	SendMatchInfo(NULL);
	return true;
}

//�û��˳�����
bool  CGameMatchServiceManager::OnUserQuitGame(IServerUserItem * pIServerUserItem, BYTE cbReason,WORD *pBestRank, DWORD dwContextID)
{	
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem == NULL) return false;

	bool bJoin=false;
	//�ж�����Ƿ�����̭������
	for(INT_PTR i=0;i<m_OutUserItem.GetCount();++i)
	{
		if ( pIServerUserItem == m_OutUserItem.GetAt(i) )
		{
			DISK_LOG(TEXT("��� %s ����̭ ״̬ %d ����:%d"),pIServerUserItem->GetNickName(),pIServerUserItem->GetUserStatus(),pIServerUserItem->GetTableID());
			return false;
		}
	}

	for (INT_PTR i=0;i<m_OnMatchUserItem.GetCount();i++)
	{
		IServerUserItem * pOnLineUserItem=m_OnMatchUserItem[i];
		//�ܱ�������������쳣�������
		if (pOnLineUserItem==pIServerUserItem)
		{
			ASSERT(pOnLineUserItem->GetUserID()==pIServerUserItem->GetUserID());
			m_OnMatchUserItem.RemoveAt(i);
			bJoin=true;
			break;
		}
	}

	if(bJoin == false) return true;

	//��ǰ�����飬����û�п�ʼ�����˻�������
	if(m_pCurMatch->OnUserQuitMatch(pIServerUserItem,true))
	{
		if(!pIServerUserItem->IsAndroidUser())
		{
			DBR_GR_UserMatchJoin MatchFee;
			MatchFee.dwUserID		=	pIServerUserItem->GetUserID();
			MatchFee.wServerID		=	m_pGameServiceOption->wServerID;
			MatchFee.dwMatchID		=	m_pCurMatch->m_dwMatchID;
			MatchFee.cbQuitMatch	=	1;	//������ʶ

			MatchFee.dwJoinType	=	m_MatchGlobal.Fee.Fee[0].dwFeeType;
			MatchFee.dwJoinFee	=	m_MatchGlobal.Fee.Fee[0].dwFeeCount;
			MatchFee.dwJoinTypeTwo	=	m_MatchGlobal.Fee.Fee[1].dwFeeType;
			MatchFee.dwJoinFeeTwo	=	m_MatchGlobal.Fee.Fee[1].dwFeeCount;
			MatchFee.dwJoinTypeThree	=	m_MatchGlobal.Fee.Fee[2].dwFeeType;
			MatchFee.dwJoinFeeThree	=	m_MatchGlobal.Fee.Fee[2].dwFeeCount;
			MatchFee.dwJoinTypeFour	=	m_MatchGlobal.Fee.Fee[3].dwFeeType;
			MatchFee.dwJoinFeeFour	=	m_MatchGlobal.Fee.Fee[3].dwFeeCount;
			MatchFee.dwJoinTypeFive	=	m_MatchGlobal.Fee.Fee[4].dwFeeType;
			MatchFee.dwJoinFeeFive	=	m_MatchGlobal.Fee.Fee[4].dwFeeCount;
	

			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_MATCH_QUIT,dwContextID,&MatchFee,sizeof(MatchFee));
		}

		return true;
	}

	//���ǵ�ǰ������,�����˻�������
	for (INT_PTR i=0;i<m_MatchGroup.GetCount();i++)
	{
		CChampionshipMatch *pMatch=m_MatchGroup.GetAt(i);
		if ( pMatch == NULL ) continue;
		if(pMatch->OnUserQuitMatch(pIServerUserItem,false))
		{
			//�������������������ø��±�����Ϣ
			break;;
		}
	}

	return false;
}


//ʱ���¼�
bool  CGameMatchServiceManager::OnEventTimer(DWORD dwTimerID, WPARAM dwBindParameter)
{
	if(dwTimerID>=IDI_GROUP_START)
	{
		INT_PTR nCount=m_TimerItemActive.GetCount();
		tagMatchTimerItem *pTimerItem=NULL;
		for (INT_PTR i=0;i<nCount;i++)
		{
			pTimerItem=m_TimerItemActive.GetAt(i);
			if(pTimerItem->dwTimerID==dwTimerID)
			{
				CChampionshipMatch *pMatch=(CChampionshipMatch*)pTimerItem->wBindParam;
				if(pMatch!=NULL)
				{
					pMatch->OnTimeMessage(pTimerItem->dwRealID,dwBindParameter);					
				}
				else
				{
					for (INT_PTR i=0;i<m_MatchGroup.GetCount();i++)
					{
						CChampionshipMatch *pMatch=m_MatchGroup.GetAt(i);
						if ( pMatch == NULL ) continue;
						pMatch->OnTimeMessage(dwTimerID,dwBindParameter);
					}
					m_pCurMatch->OnTimeMessage(dwTimerID,dwBindParameter);
				}
				//���ô���
				if (pTimerItem->dwRepeatTimes!=TIMES_INFINITY)
				{
					ASSERT(pTimerItem->dwRepeatTimes>0);
					if (pTimerItem->dwRepeatTimes==1L)
					{
						m_TimerItemActive.RemoveAt(i);
						m_TimerItemFree.Add(pTimerItem);
					}
					else pTimerItem->dwRepeatTimes--;
				}
				return true;
			}
		}
		for (INT_PTR i=0;i<m_MatchGroup.GetCount();i++)
		{
			CChampionshipMatch *pMatch=m_MatchGroup.GetAt(i);
			if ( pMatch == NULL ) continue;
			pMatch->OnTimeMessage(dwTimerID,dwBindParameter);
		}
		m_pCurMatch->OnTimeMessage(dwTimerID,dwBindParameter);
		return true;
	}

	switch (dwTimerID)
	{
	case IDI_DELETE_OUT_USER:
		{
			if ( m_bMatchOver == true) break;
 			for (INT_PTR i=0;i<m_OutUserItem.GetCount();i++)
 			{
 				IServerUserItem *pUserItem=m_OutUserItem.GetAt(i);
				if(pUserItem == NULL ) continue;
					if(pUserItem->GetTableID()!=INVALID_TABLE)
					{
						DISK_LOG(TEXT("%s ����,����:%d"),pUserItem->GetNickName(),pUserItem->GetTableID());
						m_ppITableFrame[pUserItem->GetTableID()]->PerformStandUpAction(pUserItem);
					}
					if ( pUserItem->IsAndroidUser() )
						PostReleaseAndDeleteAndroid(pUserItem->GetUserID(), pUserItem->GetBindIndex());			
 			}
 			m_OutUserItem.RemoveAll();			
			
			return true;
		}		
	case IDI_DELETE_OVER_MATCH:
		{
			CChampionshipMatch *pOverMatch=NULL;
			for(INT_PTR nOverCount=0;nOverCount<m_OverGroup.GetCount();nOverCount++)
			{
				m_bMatchOver = true;
				pOverMatch=m_OverGroup.GetAt(nOverCount);

				for (WORD j=0;j<pOverMatch->m_wTableCount;j++)
				{
					WORD wTableID=pOverMatch->m_pAssignTable[j].pTableFrame->GetTableID();
					if(wTableID==INVALID_TABLE) continue;
					m_bTableUsed[wTableID]=false;
					IGameMatchSink *pSink=m_ppITableFrame[wTableID]->GetGameMatchSink();
					pSink->SetGameEventSink(NULL);
				}
				SafeDelete(pOverMatch);
				m_OverGroup.RemoveAt(nOverCount);		
			}
			
			if ( m_bMatchOver == true )
			{
				//��ʱ���ڱ�����������ʵ��,�����㹻
				CreateMatchInstanceEnd();
			
				m_bMatchOver = false;
			}
			
			return true;
		}
	case IDI_MATCH_POST_ANDROID:
		{
			tagAndroidUserInfo tangInfo = {0};
			m_pAndroidUserManager->GetAndroidUserInfo(tangInfo, ANDROID_SIMULATE|ANDROID_PASSIVITY|ANDROID_INITIATIVE);
			if ( m_pAndroidUserManager->GetAndroidCount()>=m_MatchGlobal.Base.dwAndroidUserCount ) return true;
			PostAndroidApply(6);
			return true; 
		}
	case IDI_ANDROID_SIGN_UP:
		{
			//if ( m_pAndroidUserManager->GetAndroidCount()>=m_MatchGlobal.Base.dwAndroidUserCount ) return true;
			IAndroidUserItem *pAndroidUserItem=GetFreeAndroidUserItem();
			if(pAndroidUserItem!=NULL )
				OnUserJoinGame(pAndroidUserItem->GetMeUserItem());
			break;
		}
	default:
		break;
	}

	return true;
}

bool CGameMatchServiceManager::StartMatchCheck()
{
	//û�����ˣ��������ÿ�ʼ
	if (m_cbRealUserJoin == 0)
	{
		DWORD dwUserID=0L;
		IServerUserItem *pIServerUserItem = NULL;
		POSITION pos = m_pCurMatch->m_OnMatchUserMap.GetStartPosition();
		while (pos != NULL)
		{
			m_pCurMatch->m_OnMatchUserMap.GetNextAssoc(pos, dwUserID, pIServerUserItem);
			OnUserQuitGame(pIServerUserItem, 0, 0);
		}
		return false;
	}

	if ( m_MatchGlobal.Base.dwMatchType == MatchType_Timing )
	{
		IServerUserItem *pUserItem=NULL;
		DWORD dwUserID;	

		POSITION pos=m_pCurMatch->m_OnMatchUserMap.GetStartPosition();
		while(pos!=NULL)
		{
			m_pCurMatch->m_OnMatchUserMap.GetNextAssoc(pos, dwUserID, pUserItem);
			if(pUserItem!=NULL)
			{
				tagUserInfo * pUserScore=pUserItem->GetUserInfo();
				pUserScore->dwWinCount=0L;
				pUserScore->dwLostCount=0L;
				pUserScore->dwFleeCount=0L;
				pUserScore->dwDrawCount=0L;
				pUserScore->lGrade=0L; 
				pUserScore->lScore = m_pCurMatch->m_dwInitalScore;

				SendGroupUserScore(pUserItem,m_pCurMatch);
			}
		}
	}
	
	m_pCurMatch->m_bStarted = true;
	m_pCurMatch->SetCurMatchStatus(MS_MATCH_START);
	
// 	POSITION pos=m_pCurMatch->m_OnMatchUserMap.GetStartPosition();
// 	IServerUserItem *pUserItem=NULL;
// 	DWORD dwUserID=0;
// 	while(pos!=NULL)
// 	{
// 		m_pCurMatch->m_OnMatchUserMap.GetNextAssoc(pos,dwUserID, pUserItem);
// 		DISK_LOG(TEXT("������ʼ֪ͨ��� %s ���� %d ״̬ %d"),pUserItem->GetNickName(),pUserItem->GetTableID(),pUserItem->GetUserStatus());
// 	}
// 
// 	DWORD dwCount = m_pCurMatch->m_OnMatchUserMap.GetCount();


	SendMatchStatus(NULL,MS_MATCHING,m_pCurMatch);

	m_pITimerEngine->SetTimer(IDI_START_MATCH_START,3000L,1,NULL);

	return true;
}

//�����޸�
bool  CGameMatchServiceManager::RectifyServiceOption(tagGameServiceOption * pGameServiceOption,tagGameServiceAttrib *pGameServiceAttrib)
{
	ASSERT(pGameServiceOption!=NULL);
	if (pGameServiceOption==NULL) return false;
	m_pGameServiceOption = pGameServiceOption;
	m_pGameServiceAttrib = pGameServiceAttrib;

	//�����������Ӷ���
	if (m_ppITableFrame==NULL)
	{
		m_ppITableFrame=new ITableFrame*[pGameServiceOption->wTableCount];
		m_bTableUsed=new bool[pGameServiceOption->wTableCount];
	}

	return true;
}

//��������ģʽ
void *  CGameMatchServiceManager::CreateGameMatchSink(const IID & Guid, DWORD dwQueryVer)
{
	//��������
	CGameMatchSink *pGameMatchSink=NULL;
	//��������
	try
	{
		pGameMatchSink=new CGameMatchSink();
		if (pGameMatchSink==NULL) throw TEXT("����ʧ��");
		void * pObject=pGameMatchSink->QueryInterface(Guid,dwQueryVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");
		
		return pObject;
	}
	catch (...) {}

	return NULL;
}

//��������
bool  CGameMatchServiceManager::SendDataToServer(DWORD dwUserIndex, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	ASSERT(FALSE);
	return true;
	//��������
	TCP_Command Command;
	Command.wSubCmdID=wSubCmdID;
	Command.wMainCmdID=wMainCmdID;

	//��Ϣ����
	try
	{
		if (m_pITCPNetworkEngineEvent == NULL)
		{
			throw 0;
		}
		if (dwUserIndex>=INDEX_ANDROID)
		{
		
		}
		else
			if (m_pITCPNetworkEngineEvent->OnEventTCPNetworkRead(dwUserIndex,Command,pData,wDataSize)==false)
			{
				throw 0;
			}
	}
	catch (...) 
	{
		ASSERT(FALSE);
		DeleteUserItem(dwUserIndex);
		return false;
	}

	return true;
}

//��������� 
bool CGameMatchServiceManager::PostAndroidApply(DWORD dwCount)
{
	DBR_GR_GameAndroidApply dbrAndriond = {0};
	dbrAndriond.dwCount = dwCount;
	CopyMemory(dbrAndriond.guidRoom, m_guidRoom, 40 * sizeof WCHAR);
	dbrAndriond.dwRoomID = m_pGameServiceOption->wServerID;
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_ANDRIOD_APPLY, 0l, (void*)&dbrAndriond, sizeof(DBR_GR_GameAndroidApply));

	return true;
}

//�ͷŵ���������
bool CGameMatchServiceManager::PostReleaseSingleAndroid(DWORD dwUserID)
{
	DBR_GR_GameAndriodRelease request = {0};
	request.dwUserId = dwUserID;
	CopyMemory(request.guidRoom, m_guidRoom, 40 * sizeof WCHAR);
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_ANDRIOD_RELEASE, 0l, (void*)&request, sizeof request);

	return true;
}
//�ͷ����л�����
bool CGameMatchServiceManager::PostReleaseAllAdroid()
{
	DBR_GR_GameAndriodAll request = {0};
	CopyMemory(request.guidRoom, m_guidRoom, 40 * sizeof WCHAR);
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_ANDRIOD_ALL, 0l, (void *)&request, sizeof request);

	return true;
}

//�����ͷŻ�����
bool CGameMatchServiceManager::PostReleaseArrayAndroid(CString androids)
{
	DBR_GR_GameAndriodArray request = {0};
	CopyMemory(request.guidRoom, m_guidRoom, 40 * sizeof WCHAR);
	CopyMemory(request.szUserArray, androids.GetBuffer(),androids.GetLength());
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_ANDRIOD_ARRAY, 0l, (void*)&request, sizeof request);

	return true;
}

bool CGameMatchServiceManager::PostReleaseAndDeleteAndroid(DWORD dwUserID, WORD dwBind)
{
	DBR_GR_GameAndriodReleaseAndDelete request = {0};
	CopyMemory(request.guidRoom, m_guidRoom, 40 * sizeof WCHAR);
	request.dwUserId = dwUserID;
	request.wBingId = dwBind;
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_ANDRIOD_RELEASE_DELETE, 0l, (void*)&request, sizeof request);


	return true;
}

//�û���̭
bool CGameMatchServiceManager::OnUserOut(IServerUserItem *pUserItem,CChampionshipMatch *pMatch)

{
	if(pUserItem==NULL)
		return true;

	SendMatchStatus(pUserItem,MS_OUT);

	for (INT_PTR i=0;i<m_OnMatchUserItem.GetCount();i++)
	{
		//��ȡ�û�
		IServerUserItem * pOnLineUserItem=m_OnMatchUserItem[i];

		//�û��ж�
		if (pOnLineUserItem==pUserItem)
		{
			m_OnMatchUserItem.RemoveAt(i);

			//�ӳ���������Ȼ�ͻ��˿�����Ч��
			m_OutUserItem.Add(pUserItem);
			break;
		}
	}


	m_pITimerEngine->SetTimer(IDI_DELETE_OUT_USER, 3000,1,0);

	return true;
}

//ɾ���û�
bool CGameMatchServiceManager::DeleteUserItem(DWORD dwUserIndex)
{
	try
	{
		if (m_pITCPNetworkEngineEvent == NULL)
		{
			throw 0;
		}
		m_pITCPNetworkEngineEvent->OnEventTCPNetworkShut(dwUserIndex,0,0L);
	}
	catch (...)
	{
		//�������
		ASSERT(FALSE);
		return false;
	}
	return true;
}

//������Ϣ
bool CGameMatchServiceManager::SendRoomMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType,CChampionshipMatch *pMatch)
{
	do 
	{
		if ( pMatch == NULL ) break;

		POSITION pos=pMatch->m_OnMatchUserMap.GetStartPosition();
		IServerUserItem *pUserItem=NULL;
		DWORD dwUserID=0;
		while(pos!=NULL)
		{
			pMatch->m_OnMatchUserMap.GetNextAssoc(pos,dwUserID, pUserItem);
			m_pIGameServiceFrame->SendRoomMessage(pUserItem,lpszMessage,wMessageType);
		}


		return true;

	} while (0);

	if(pIServerUserItem==NULL)
		return false;

	m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,lpszMessage,wMessageType);

	return true;
}

//������Ϸ��Ϣ
bool CGameMatchServiceManager::SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType,CChampionshipMatch *pMatch)
{
	do 
	{
		if ( pMatch == NULL ) break;

		POSITION pos=pMatch->m_OnMatchUserMap.GetStartPosition();
		IServerUserItem *pUserItem=NULL;
		DWORD dwUserID=0;
		while(pos!=NULL)
		{
			pMatch->m_OnMatchUserMap.GetNextAssoc(pos,dwUserID, pUserItem);
			m_pIGameServiceFrame->SendGameMessage(pUserItem,lpszMessage,wMessageType);
		}

		return true;

	} while (0);

	if (pIServerUserItem == NULL)
		return false;

	m_pIGameServiceFrame->SendGameMessage(pIServerUserItem,lpszMessage,wMessageType);

	return true;
}

//��ʼ�����ӿ��
bool CGameMatchServiceManager::InitTableFrame(ITableFrame * pTableFrame,WORD wTableID)
{
	if(pTableFrame==NULL || m_ppITableFrame==NULL ||
		wTableID>m_pGameServiceOption->wTableCount)
	{
		ASSERT(false);
		return false;
	}
	m_ppITableFrame[wTableID]=pTableFrame;
	m_bTableUsed[wTableID]=false;
	return true;
}

bool CGameMatchServiceManager::AssignTableForFullUser(CChampionshipMatch *pMatch, WORD wTableCount)
{

	ITableFrame **pTable=new ITableFrame*[wTableCount];
	ZeroMemory(pTable,sizeof(ITableFrame*)*wTableCount);
	WORD n=0,wCount=0;
	while((wCount<wTableCount)&&n<m_pGameServiceOption->wTableCount)
	{
		if(!m_bTableUsed[n])
		{
			for(WORD i=0; i<m_pGameServiceAttrib->wChairCount; i++)
			{
				IServerUserItem *pTableUserItem=m_ppITableFrame[n]->GetTableUserItem(i);
				if(pTableUserItem!=NULL && pTableUserItem->GetUserID() != 0)
				{
					//�������ӣ����������������
					DISK_LOG(TEXT("��������ʱ����û���� %s,tableID:%d,UserStatus:%d"),pTableUserItem->GetNickName(),pTableUserItem->GetTableID(),pTableUserItem->GetUserStatus());
					m_ppITableFrame[n]->PerformStandUpAction(pTableUserItem);
				}
			}

			pTable[wCount]=m_ppITableFrame[n];
			pTable[wCount]->SetCellScore(pMatch->m_dwCurBase);
			IGameMatchSink *pSink=m_ppITableFrame[n]->GetGameMatchSink();
			pSink->SetGameEventSink(QUERY_OBJECT_PTR_INTERFACE(m_pCurMatch,IUnknownEx));
			m_bTableUsed[n]=true;
			wCount++;
		}
		n++;
	}
	if(n>m_pGameServiceOption->wTableCount || wCount<wTableCount)
	{
		DISK_LOG(TEXT("�������ӷ���ʧ�ܣ�������:%d,�ѷ���:%d"),wTableCount,wCount);
		for(WORD i=0;i<wCount;i++)
		{
			if(pTable[i]!=NULL)
				m_bTableUsed[pTable[i]->GetTableID()]=false;
		}
		SafeDeleteArray(pTable);
		return false;
	}
	m_pCurMatch->SetMatchTable(pTable, wTableCount);
	SafeDeleteArray(pTable);

	return true;
}

//��������
bool CGameMatchServiceManager::AssignTable(CChampionshipMatch *pMatch, WORD wTableCount)
{
	
	ITableFrame **pTable=new ITableFrame*[wTableCount];
	ZeroMemory(pTable,sizeof(ITableFrame*)*wTableCount);
	WORD n=0,wCount=0;
	while((wCount<wTableCount)&&n<m_pGameServiceOption->wTableCount)
	{
		if(!m_bTableUsed[n])
		{
			for(WORD i=0; i<m_pGameServiceAttrib->wChairCount; i++)
			{
				IServerUserItem *pTableUserItem=m_ppITableFrame[n]->GetTableUserItem(i);
				if(pTableUserItem!=NULL && pTableUserItem->GetUserID() != 0)
				{
					//�������ӣ����������������
					DISK_LOG(TEXT("��������ʱ����û���� %s,tableID:%d,UserStatus:%d"),pTableUserItem->GetNickName(),pTableUserItem->GetTableID(),pTableUserItem->GetUserStatus());
					m_ppITableFrame[n]->PerformStandUpAction(pTableUserItem);
				}
			}

			pTable[wCount]=m_ppITableFrame[n];
			pTable[wCount]->SetCellScore(pMatch->m_dwCurBase);
 			IGameMatchSink *pSink=m_ppITableFrame[n]->GetGameMatchSink();
 			pSink->SetGameEventSink(QUERY_OBJECT_PTR_INTERFACE(m_pCurMatch,IUnknownEx));
			m_bTableUsed[n]=true;
			wCount++;
		}
		n++;
	}
	if(n>m_pGameServiceOption->wTableCount || wCount<wTableCount)
	{
		DISK_LOG(TEXT("�������ӷ���ʧ�ܣ�������:%d,�ѷ���:%d"),wTableCount,wCount);
		for(WORD i=0;i<wCount;i++)
		{
			if(pTable[i]!=NULL)
			m_bTableUsed[pTable[i]->GetTableID()]=false;
		}
		SafeDeleteArray(pTable);
		return false;
	}
	m_pCurMatch->SetMatchTable(pTable, wTableCount);
	SafeDeleteArray(pTable);
	
	return true;
}

//��ȡ����������Ŀ
int CGameMatchServiceManager::GetFreeTableCount()
{
	int nCount = 0;
	
	for (int i = 0; i < m_pGameServiceOption->wTableCount; ++i)
	{
		if (!m_bTableUsed[i])
			nCount++;
	}
	
	return nCount;
}


//��������
bool CGameMatchServiceManager::SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	if(pIServerUserItem!=NULL)
		return m_pIGameServiceFrame->SendData(pIServerUserItem,wMainCmdID,wSubCmdID,pData,wDataSize);
	else
		return m_pIGameServiceFrame->SendData(BG_COMPUTER, wMainCmdID,wSubCmdID, pData, wDataSize);
    
	return true;
}

bool CGameMatchServiceManager::ReadMatchRule()
{
	return	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_MATCH_BASE, 0, NULL, 0);
}

//��ʱ��
bool CGameMatchServiceManager::SetGameTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter,CChampionshipMatch *pMatch)
{
	tagMatchTimerItem * pTimerItem=NULL;
	INT_PTR nFreeCount=m_TimerItemFree.GetCount();
	if (nFreeCount>0)
	{
		pTimerItem=m_TimerItemFree[nFreeCount-1];
		ASSERT(pTimerItem!=NULL);
		m_TimerItemFree.RemoveAt(nFreeCount-1);
	}

	pTimerItem->dwRealID=dwTimerID;
	pTimerItem->wBindParam=(WPARAM)pMatch;
	pTimerItem->dwRepeatTimes=dwRepeat;
	m_TimerItemActive.Add(pTimerItem);
	return m_pITimerEngine->SetTimer(pTimerItem->dwTimerID,dwElapse,dwRepeat,dwBindParameter);
}

bool CGameMatchServiceManager::KillGameTimer(DWORD dwTimerID)
{
	return m_pITimerEngine->KillTimer(dwTimerID);
}

//д���齱��
bool CGameMatchServiceManager::WriteExpAward(IServerUserItem *pIServerUserItem, DWORD dwRank,DWORD dwMatchNO,LPCTSTR szDescribe,CChampionshipMatch *pMatch)
{
	DWORD dwOrderID		= 0;
	DWORD dwExpValue	= 0;
	
	do 
	{
		if ( m_ExpReward.dwSolutionID != pMatch->m_dwCurSolutionID ) break;

		for( WORD j =0;j<m_ExpReward.dwCount;++j )
		{
			if ( dwRank >= m_ExpReward.Exp[j].dwRankUpper && dwRank <= m_ExpReward.Exp[j].dwRankLower )
			{
				dwOrderID = m_ExpReward.Exp[j].dwOrderID;
				dwExpValue = m_ExpReward.Exp[j].dwExpValue;
			}
		}

	} while (0);

	if(dwOrderID) 
	{
		DBR_GR_MatchExpReward ExpReward = {0};

		ExpReward.wServerID		= m_pGameServiceOption->wServerID;
		ExpReward.dwMatchID		= dwMatchNO;
		ExpReward.dwUserID		= pIServerUserItem->GetUserID();
		ExpReward.dwRank		= dwRank;
		ExpReward.dwExpValue	= dwExpValue;

		m_pIGameServiceFrame->PostDataBaseRequest(pIServerUserItem, DBR_GR_MATCH_EXP_REWARD, &ExpReward, sizeof ExpReward);
	}

	return true;
}

//д�뽱��
bool CGameMatchServiceManager::WriteUserAward(IServerUserItem *pIServerUserItem, DWORD dwRank,DWORD dwMatchNO,LPCTSTR szDescribe,CChampionshipMatch *pMatch)
{
	ASSERT(pIServerUserItem!=NULL);
	if(pIServerUserItem==NULL) return false; 

	WriteExpAward(pIServerUserItem,dwRank,dwMatchNO,szDescribe,pMatch);

	DWORD dwOrderID = 0;
	tagRewardItem Item[5] = {0};

	//�������� ���ݿ�֪ͨ���ʼ�
	WORD i = pMatch->m_cbSolutionIndex;
	WORD nCount = m_Reward[i].dwCount;
	WORD m = 0;
	for( WORD j =0;j<nCount;++j )
	{
		//�ҵ��������䣬ȡ���������ͺͽ�������
		if ( dwRank >= m_Reward[i].Reward[j].dwRankUpper && dwRank <= m_Reward[i].Reward[j].dwRankLower)
		{
			dwOrderID = m_Reward[i].Reward[j].dwOrderID;
			Item[m].dwRewardType = m_Reward[i].Reward[j].dwRewardType;
			Item[m].dwCount = m_Reward[i].Reward[j].dwRewardCount;
			++m;
		}
	}

// 	for ( WORD i=0;i<3;++i )
// 	{
// 		if ( m_Reward[i].dwSolutionID != pMatch->m_dwCurSolutionID ) continue;
// 		WORD nCount = m_Reward[i].dwCount;
// 		WORD m = 0;
// 		for( WORD j =0;j<nCount;++j )
// 		{
// 			//�ҵ��������䣬ȡ���������ͺͽ�������
// 			if ( dwRank >= m_Reward[i].Reward[j].dwRankUpper && dwRank <= m_Reward[i].Reward[j].dwRankLower)
// 			{
// 				dwOrderID = m_Reward[i].Reward[j].dwOrderID;
// 				Item[m].dwRewardType = m_Reward[i].Reward[j].dwRewardType;
// 				Item[m].dwCount = m_Reward[i].Reward[j].dwRewardCount;
// 				++m;
// 			}
// 		}
// 	}

	if(dwOrderID) 
	{
		//�����˻�ͬ��д����
		DBR_GR_MatchReward MatchReward = {0};

		MatchReward.wServerID		= m_pGameServiceOption->wServerID;
		MatchReward.dwMatchID		= dwMatchNO;
		MatchReward.dwUserID		= pIServerUserItem->GetUserID();
		MatchReward.dwRank			= dwRank;
		MatchReward.lMatchScore		= pIServerUserItem->GetUserScore();
		CopyMemory(MatchReward.Item,Item,sizeof(tagRewardItem)*5);

		m_pIGameServiceFrame->PostDataBaseRequest(pIServerUserItem, DBR_GR_MATCH_REWARD, &MatchReward, sizeof MatchReward);
	}
	else
	{
		CMD_GR_MatchResult MatchRewardResult = {0};
		MatchRewardResult.dwRank			= dwRank;
		m_pIGameServiceFrame->SendData(pIServerUserItem, MDM_GR_MATCH, SUB_GR_MATCH_RESULT, (VOID*)&MatchRewardResult, sizeof(MatchRewardResult));
		OnUserOut(pIServerUserItem,pMatch);
	}

	return true;
}

//�����û�����
bool CGameMatchServiceManager::SendGroupUserScore(IServerUserItem * pIServerUserItem,CChampionshipMatch *pMatch)
{
	if (pIServerUserItem == NULL) return false;

	//��������
	CMD_GR_UserScore UserScore;
	
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();

	//��������
	UserScore.dwUserID=pUserInfo->dwUserID;
	UserScore.UserScore.dwWinCount=pUserInfo->dwWinCount;
	UserScore.UserScore.dwLostCount=pUserInfo->dwLostCount;
	UserScore.UserScore.dwDrawCount=pUserInfo->dwDrawCount;
	UserScore.UserScore.dwFleeCount=pUserInfo->dwFleeCount;
	UserScore.UserScore.dwUserMedal=pUserInfo->dwUserMedal;
	UserScore.UserScore.dwExperience=pUserInfo->dwExperience;
	UserScore.UserScore.lLoveLiness=pUserInfo->lLoveLiness;

	//�������
	UserScore.UserScore.lGrade=pUserInfo->lGrade;
	UserScore.UserScore.lInsure=pUserInfo->lInsure;

	//�������
	UserScore.UserScore.lScore=pUserInfo->lScore;

	//�㲥�����ҵ���Ϣ���������
	if(pMatch!=NULL)
	{
		POSITION pos=pMatch->m_OnMatchUserMap.GetStartPosition();
		IServerUserItem *pUserItem=NULL;
		DWORD dwUserID=0;
		while(pos!=NULL)
		{
			pMatch->m_OnMatchUserMap.GetNextAssoc(pos,dwUserID, pUserItem);
			m_pIGameServiceFrame->SendData(pUserItem,MDM_GR_USER,SUB_GR_USER_SCORE,&UserScore,sizeof(UserScore));	
		}
	}
	else
		m_pIGameServiceFrame->SendData(BG_COMPUTER,MDM_GR_USER,SUB_GR_USER_SCORE,&UserScore,sizeof(UserScore));
	

	return true;
}

//����״̬
bool  CGameMatchServiceManager::SendGroupUserStatus(IServerUserItem * pIServerUserItem,CChampionshipMatch *pMatch)
{
	ASSERT(pIServerUserItem!=NULL);
	ASSERT(pIServerUserItem->IsClientReady()==true);
	if (pIServerUserItem == NULL) return false;
	if (pIServerUserItem->IsClientReady()==false) return false;

	//��������
	CMD_GR_UserStatus UserStatus;
	memset(&UserStatus,0,sizeof(UserStatus));
	tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();

	//��������
	UserStatus.dwUserID=pUserData->dwUserID;
	UserStatus.UserStatus.wTableID=pUserData->wTableID;
	UserStatus.UserStatus.wChairID=pUserData->wChairID;
	UserStatus.UserStatus.cbUserStatus=pUserData->cbUserStatus;

	POSITION pos=pMatch->m_OnMatchUserMap.GetStartPosition();
	IServerUserItem *pUserItem=NULL;
	DWORD dwUserID=0;
	while(pos!=NULL)
	{
		pMatch->m_OnMatchUserMap.GetNextAssoc(pos,dwUserID, pUserItem);
		if(pUserItem->IsClientReady())
			m_pIGameServiceFrame->SendData(pUserItem,MDM_GR_USER,SUB_GR_USER_STATUS,&UserStatus,sizeof(UserStatus));	
	}

	return true;
}

bool CGameMatchServiceManager::SendMatchStatus(IServerUserItem * pIServerUserItem,BYTE cbStatus,CChampionshipMatch *pMatch)
{
	CMD_GR_MatchStatus MatchStatus;
	MatchStatus.cbMatchStatus = cbStatus;
	if ( pMatch != NULL )
	{
		SendGroupData(MDM_GR_MATCH,SUB_GR_MATCH_STATUS,&MatchStatus,sizeof(MatchStatus),pMatch);
		return true;
	}
	if ( pIServerUserItem == NULL || pIServerUserItem->IsAndroidUser() == true ) return false;
	//�����ˣ�ֻ��������
	SendData(pIServerUserItem, MDM_GR_MATCH, SUB_GR_MATCH_STATUS, &MatchStatus, sizeof(MatchStatus));
	return true;
}

//�������ݵ�������
bool CGameMatchServiceManager::SendDataToGameServer(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	return SendDataToServer(dwAndroidID,wMainCmdID,wSubCmdID,pData,wDataSize);
}

//�������ݵ�һ���û�
bool CGameMatchServiceManager::SendGroupData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize,CChampionshipMatch *pMatch)
{
	POSITION pos=pMatch->m_OnMatchUserMap.GetStartPosition();
	IServerUserItem *pUserItem=NULL;
	DWORD dwUserID=0;
	while(pos!=NULL)
	{
		pMatch->m_OnMatchUserMap.GetNextAssoc(pos,dwUserID, pUserItem);
		m_pIGameServiceFrame->SendData(pUserItem,wMainCmdID,wSubCmdID,pData,wDataSize);	
	}

    return true;
}

//��ȡһ�����еĻ�����
IAndroidUserItem * CGameMatchServiceManager::GetFreeAndroidUserItem()
{
	if(m_pAndroidUserManager->GetAndroidCount()==0) return NULL;

	//����״̬
	tagAndroidUserInfo AndroidSimulate;
	m_pAndroidUserManager->GetAndroidUserInfo(AndroidSimulate,ANDROID_SIMULATE|ANDROID_PASSIVITY|ANDROID_INITIATIVE);

	if(AndroidSimulate.wFreeUserCount==0) 
		return NULL;
	else
	{
		//�����ѱ����Ļ������ظ�����
		for(WORD i=0; i<AndroidSimulate.wFreeUserCount;i++)
		{
			bool bInMatch=false;
			for (INT_PTR j=0;j<m_OnMatchUserItem.GetCount();j++)
			{
				//���ڶ�ʱ����Ӱ�죬�����������˸պñ���̭����ûɾ�����򱣳�����̭����
				if(m_OnMatchUserItem.GetAt(j)==AndroidSimulate.pIAndroidUserFree[i]->GetMeUserItem())
				{
					bInMatch=true;
					break;
				}
				
			}
			for (INT_PTR j=0;j<m_OutUserItem.GetCount();j++)
			{
				if ( m_OutUserItem.GetAt(j) == AndroidSimulate.pIAndroidUserFree[i]->GetMeUserItem() )
				{
					bInMatch = true;
					break;
				}
			}
			if(!bInMatch)
			{
				ASSERT(AndroidSimulate.pIAndroidUserFree[i]->GetUserID()==AndroidSimulate.pIAndroidUserFree[i]->GetMeUserItem()->GetUserID());
				return AndroidSimulate.pIAndroidUserFree[i];
			}
		}
	}
    
	return NULL;
}

//�����油������
bool CGameMatchServiceManager::SetReplaceAndroidItem(IServerUserItem *pIServerUserItem)
{
	//����Ч��
	ASSERT(pIServerUserItem!=NULL);
	if(pIServerUserItem==NULL) return false;

	for (INT_PTR i=0;i<m_OnMatchUserItem.GetCount();i++)
	{
		if(m_OnMatchUserItem.GetAt(i)==pIServerUserItem)
			return false;
	}

	m_OnMatchUserItem.Add(pIServerUserItem);
	return true;
}

//��������
bool CGameMatchServiceManager::SendMatchInfo(IServerUserItem * pIServerUserItem)
{
	//��¼���������·�
	/*
	BYTE cbDataBuffer[SOCKET_TCP_PACKET]={0};
	WORD wDataSize = 0;
	WORD wHeadSize = 0;
	WORD wTempSize = 0;
	do 
	{
		if ( pIServerUserItem && pIServerUserItem->IsAndroidUser() == true ) break;

		//��������
		do 
		{	
			if ( m_MatchReward.wCount == 0 ) break;

			CopyMemory(cbDataBuffer+wDataSize,&m_MatchReward.wCount,sizeof(WORD));						
			wDataSize+=sizeof(WORD);
			for(WORD i=0;i<m_MatchReward.wCount;++i)
			{
				CopyMemory(cbDataBuffer+wDataSize,&m_MatchReward.MatchReward[i].wLength,sizeof(WORD));
				wDataSize+=sizeof(WORD);
				CopyMemory(cbDataBuffer+wDataSize,m_MatchReward.MatchReward[i].szRewardDesc,m_MatchReward.MatchReward[i].wLength);
				wDataSize+=wTempSize;
			}

			m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GR_MATCH,SUB_GR_MATCH_REWARD,&cbDataBuffer,wDataSize);
		} while (0);
		//////////////////////////////////////////////////////////////////////////
		//��������
		ZeroMemory(cbDataBuffer,sizeof(cbDataBuffer));
		wDataSize=0;
		wTempSize=0;
		CopyMemory(cbDataBuffer+wDataSize,&m_MatchRule.wCount,sizeof(m_MatchRule.wCount));							//�������
		wDataSize+=sizeof(m_MatchRule.wCount);	
		for (WORD i=0;i<m_MatchRule.wCount;++i)
		{
			CopyMemory(cbDataBuffer+wDataSize,&m_MatchRule.MatchRule[i].wLength,sizeof(WORD));			//��������
			wDataSize += sizeof(WORD);
			CopyMemory(cbDataBuffer+wDataSize,m_MatchRule.MatchRule[i].szRuleDesc,m_MatchRule.MatchRule[i].wLength);		//��������
			wDataSize += wTempSize;
		}
		m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GR_MATCH,SUB_GR_MATCH_RULE,&cbDataBuffer,wDataSize);
		//////////////////////////////////////////////////////////////////////////

		//��������
		wDataSize = m_MatchJoin.wCount * sizeof(m_MatchJoin.MatchJoin[0]);
		wHeadSize = sizeof(WORD);
		m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GR_MATCH,SUB_GR_MATCH_JOIN,&m_MatchJoin,wHeadSize+wDataSize);
		//////////////////////////////////////////////////////////////////////////

		ZeroMemory(cbDataBuffer,sizeof(cbDataBuffer));
		wDataSize=0;
		wTempSize=0;
		CopyMemory(cbDataBuffer+wDataSize,&m_MatchInfo.wMatchType,sizeof(WORD));
		wDataSize+=sizeof(WORD);
		CopyMemory(cbDataBuffer+wDataSize,&m_MatchInfo.dwInitScore,sizeof(DWORD));
		wDataSize+=sizeof(DWORD);
		CopyMemory(cbDataBuffer+wDataSize,&m_MatchInfo.dwJoinUserCountLower,sizeof(DWORD));
		wDataSize+=sizeof(DWORD);
		CopyMemory(cbDataBuffer+wDataSize,&m_MatchInfo.dwJoinUserCountUpper,sizeof(DWORD));
		wDataSize+=sizeof(DWORD);
		if ( m_MatchInfo.wMatchType == MatchType_Timing )
		{
			CopyMemory(cbDataBuffer+wDataSize,&m_MatchInfo.sysMatchStartTime,sizeof(SYSTEMTIME));
			wDataSize+=sizeof(SYSTEMTIME);
		}
		CopyMemory(cbDataBuffer+wDataSize,&m_MatchInfo.sysMatchJoinTime,sizeof(SYSTEMTIME));
		wDataSize+=sizeof(SYSTEMTIME);
		CopyMemory(cbDataBuffer+wDataSize,&m_MatchInfo.wURLLength,sizeof(WORD));
		wDataSize+=sizeof(WORD);
		CopyMemory(cbDataBuffer+wDataSize,m_MatchInfo.szActiveURL,m_MatchInfo.wURLLength);
		wDataSize+=m_MatchInfo.wURLLength;
		CopyMemory(cbDataBuffer+wDataSize,&m_MatchInfo.wNameLength,sizeof(WORD));
		wDataSize+=sizeof(WORD);
		CopyMemory(cbDataBuffer+wDataSize,m_MatchInfo.szMatchName,m_MatchInfo.wNameLength);
		wDataSize+=m_MatchInfo.wNameLength;
		m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GR_MATCH,SUB_GR_MATCH_TYPE,cbDataBuffer,wDataSize);
	} while (0);
	*/
	CMD_GR_Match_User MaterUser = {0};
	MaterUser.dwJoinCount			= (DWORD)m_pCurMatch->m_OnMatchUserMap.GetCount();  //��������
	MaterUser.dwTotalJoinCount		= (DWORD)m_OnMatchUserItem.GetCount();				//�ۼƱ�����
	MaterUser.dwMatchStartCount		= (DWORD)m_MatchGlobal.Solution.dwMaxJoinCount; //��������

	SendGroupData(MDM_GR_MATCH,SUB_GR_MATCH_USER,&MaterUser,sizeof(MaterUser),m_pCurMatch);
	
	return true;
}

//���ݿ��¼�
bool CGameMatchServiceManager::OnEventDataBase(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
{
	switch (wRequestID)
	{
	case DBO_GR_MATCH_JOIN:
		{
			if ( pIServerUserItem==NULL ) return true;
			if ( wDataSize!=sizeof(DBO_GR_UserMatchJoin) ) return false;

			DBO_GR_UserMatchJoin* pMatchJoin = (DBO_GR_UserMatchJoin*)pData;

			if ( pMatchJoin->wResultCode !=0 )
			{
				m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,pMatchJoin->szMsg,SMT_CHAT|SMT_EJECT);
				break;
			}

			if ( m_pCurMatch->OnUserSignUp(pIServerUserItem) )
				++m_cbRealUserJoin;

			DISK_LOG(TEXT("�����ɹ� %s ����%d"),pIServerUserItem->GetNickName(),pIServerUserItem->GetTableID());
			//m_OnMatchUserItem.Add(pIServerUserItem); //��OnUserSignUp()�ڲ��Ѿ����

			m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,pMatchJoin->szMsg,SMT_EJECT);//������Ϣ
			SendMatchInfo(NULL);//Ⱥ����������

			break;
		}
	case DBO_GR_MATCH_QUIT:		//�������
		{
			if ( pIServerUserItem==NULL ) return true;
			if ( wDataSize!=sizeof(DBO_GR_UserMatchJoin) ) return false;

			DBO_GR_UserMatchJoin* pMatchJoin = (DBO_GR_UserMatchJoin*)pData;

			m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,pMatchJoin->szMsg,SMT_EJECT);
			
			CMD_GR_Match_User MaterUser = {0};
			MaterUser.dwJoinCount			= (DWORD)m_pCurMatch->m_OnMatchUserMap.GetCount();  //��������
			MaterUser.dwTotalJoinCount		= (DWORD)m_OnMatchUserItem.GetCount();				//�ۼƱ�����
			MaterUser.dwMatchStartCount		= (DWORD)m_MatchGlobal.Solution.dwMaxJoinCount; //��������

			SendGroupData(MDM_GR_MATCH,SUB_GR_MATCH_USER,&MaterUser,sizeof(MaterUser),m_pCurMatch);
			SendData(pIServerUserItem, MDM_GR_MATCH, SUB_GR_MATCH_USER, &MaterUser, sizeof(MaterUser));
			--m_cbRealUserJoin ;
			break;
		}	
	case DBO_GR_MATCH_RESULT:
		{
			if (pIServerUserItem == NULL) return true;

			CMD_GR_MatchResult* pMatchResult = (CMD_GR_MatchResult*)pData;
			CMD_GR_MatchResult MatchResult = {0};
			MatchResult.dwRank = pMatchResult->dwRank;

			m_pIGameServiceFrame->SendData(pIServerUserItem, MDM_GR_MATCH, SUB_GR_MATCH_RESULT, (VOID*)&MatchResult, sizeof(MatchResult));
			OnUserOut(pIServerUserItem,NULL);

			break;
		}
	case DBO_GR_MATCH_APPLY_ANDROID:
		{
			DBO_GR_GameAndroidApply * pGameAndroidInfo=(DBO_GR_GameAndroidApply *)pData;
			WORD wHeadSize=sizeof(DBO_GR_GameAndroidApply)-sizeof(pGameAndroidInfo->AndroidParameter);

			ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pGameAndroidInfo->wAndroidCount*sizeof(pGameAndroidInfo->AndroidParameter[0]))));
			if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pGameAndroidInfo->wAndroidCount*sizeof(pGameAndroidInfo->AndroidParameter[0])))) return false;

			if (pGameAndroidInfo->lResultCode == DB_SUCCESS)
			{
				m_pAndroidUserManager->SetAndroidStock(pGameAndroidInfo->AndroidParameter,pGameAndroidInfo->wAndroidCount);
				m_pAndroidUserManager->CreateAndroidUserItem(m_pAndroidUserManager->GetStockCount());
			}

			break;
		}
	case DBO_GR_MATCH_FEE: //�������ã���������
		{
			if (wDataSize%sizeof(CMD_GR_MatchFeeInfo)!=0) return false;

			CMD_GR_MatchFeeInfo* pFeeInfo = (CMD_GR_MatchFeeInfo*)pData;

			CopyMemory(&m_MatchGlobal.Fee,&pFeeInfo->MatchFee,sizeof(pFeeInfo->MatchFee));
			CopyMemory(&m_MatchGlobal.Require,&pFeeInfo->MatchRequire,sizeof(pFeeInfo->MatchRequire));
			break;
		}
	case DBO_GR_MATCH_SOLUTION: //������Ϣ
		{
			if (wDataSize%sizeof(CMD_GR_MatchSolution)!=0) return false;

			CMD_GR_MatchSolution* pSolution=(CMD_GR_MatchSolution*)pData;

			CopyMemory(&m_MatchGlobal.Solution,pSolution,sizeof(CMD_GR_MatchSolution));

			break;
		}
	case DBO_GR_MATCH_STAGE:  //��������
		{
			if ( wDataSize%sizeof(CMD_GR_StageArray) !=0 ) return false;

			CMD_GR_StageArray* pArray = (CMD_GR_StageArray*)pData;

			WORD wSolutionCount = wDataSize/sizeof(CMD_GR_StageArray);
			WORD wStageCount = 0;
			WORD wStageType = 0;

			//////////////////////////////////////////////////////////////////////////
//  			m_MatchGlobal.StageArray[0].Stage.wCount = 1;
//  			m_MatchGlobal.StageArray[0].Stage.wStageType[0] = (WORD)StageRule_CountScore; 
//  			CopyMemory(&m_MatchGlobal.StageArray[0].Round,&pArray[0].Round,sizeof(CMD_GR_MatchRound));
//  			m_MatchGlobal.Solution.dwMaxJoinCount=6;
//  			break;
			//////////////////////////////////////////////////////////////////////////
			//��ȡ���Ƹ���������
			for ( WORD j=0;j<wSolutionCount;++j )
			{
				//��ȡÿ�׷��������Ƹ���
				wStageCount = pArray[j].Stage.wCount;
				m_MatchGlobal.StageArray[j].Stage.wCount = wStageCount;
				
				for( WORD i=0;i<wStageCount;++i )
				{
					//��ȡÿ�׷�������������
					wStageType = pArray[j].Stage.wStageType[i];
					m_MatchGlobal.StageArray[j].Stage.wStageType[i] = wStageType; 
					//��ȡ����������Ϣ
					switch( wStageType )
					{
					case StageRule_Dali:
						{
							CopyMemory(&m_MatchGlobal.StageArray[j].Dali,&pArray[j].Dali,sizeof(CMD_GR_MatchDaLi));
							break;
						}
					case  StageRule_ASSDali:
						{
							CopyMemory(&m_MatchGlobal.StageArray[j].AssDali,&pArray[j].AssDali,sizeof(CMD_GR_MatchDaLi));
							break;
						}
					case StageRule_CountScore:
						{
							CopyMemory(&m_MatchGlobal.StageArray[j].Round,&pArray[j].Round,sizeof(CMD_GR_MatchRound));
							break;
						}
					case StageRule_SwissScore:
						{
							CopyMemory(&m_MatchGlobal.StageArray[j].SwissScore,&pArray[j].SwissScore,sizeof(CMD_GR_MatchRound));
							break;
						}
					case  StageRule_SwissShift:
						{
							CopyMemory(&m_MatchGlobal.StageArray[j].SwissShift,&pArray[j].SwissShift,sizeof(CMD_GR_MatchRound));
							break;
						}
					}
				}
			}
		
			break;
		}
	case DBO_GR_MATCH_TAGOUT://��������
		{
			if (wDataSize%sizeof(tagMatchTagout)!=0) return false;
			tagMatchTagout* pTagout = (tagMatchTagout*)pData;

			CopyMemory(&m_MatchGlobal.Tagout,pTagout,sizeof(tagMatchTagout));
			break;
		}
	case DBO_GR_MATCH_BASE: //������Ϣ
		{
			if (wDataSize%sizeof(CMD_GR_MatchBase)!=0) return false;
			CMD_GR_MatchBase* pBase = (CMD_GR_MatchBase*)pData;

			CopyMemory(&m_MatchGlobal.Base,pBase,sizeof(CMD_GR_MatchBase));
			break;
		}
	case DBO_GR_MATCH_DATE:	//����ʱ��
		{
			if (wDataSize%sizeof(CMD_GR_MatchDateTime)!=0) return false;
			CMD_GR_MatchDateTime* pTime = (CMD_GR_MatchDateTime*)pData;

			CopyMemory(&m_MatchGlobal.DateTime,pTime,sizeof(CMD_GR_MatchDateTime));
			break;
		}
	case DBO_GR_MATCH_REVIVE://��������
		{
			if (wDataSize%sizeof(tagMatchReviveInfo)!=0) return false;
			tagMatchReviveInfo* pRevive = (tagMatchReviveInfo*)pData;

			CopyMemory(&m_MatchGlobal.MatchRevive,pRevive,sizeof(tagMatchReviveInfo));
			break;
		}
	case DBO_GR_MATCH_REWARD: //��������
		{
 			if ( wDataSize%sizeof(CMD_GR_RewardArray) !=0 ) return false;
 
 			CMD_GR_RewardArray* pArray = (CMD_GR_RewardArray*)pData;
 
 			WORD wSolutionCount = wDataSize/sizeof(CMD_GR_RewardArray);
 
 			for ( WORD j=0;j<wSolutionCount;++j )
 			{
 				CopyMemory(&m_Reward[j].Reward,&pArray[j].Reward,sizeof(tagRewardInfo)*pArray[j].dwCount);
 				m_Reward[j].dwCount = pArray[j].dwCount;
				m_Reward[j].dwSolutionID = pArray[j].dwSolutionID;
 			}
			break;
		}
	case DBO_GR_MATCH_EXP_REWARD: //���齱��
		{
			if ( wDataSize % sizeof(CMD_GR_ExpReward) !=0 ) return false;

			CMD_GR_ExpReward* pExp = (CMD_GR_ExpReward*)pData;
			memset(&m_ExpReward,0,sizeof(m_ExpReward));

			CopyMemory(&m_ExpReward.Exp,&pExp->Exp,sizeof(tagExpReward)*pExp->dwCount);
			m_ExpReward.dwSolutionID = pExp->dwSolutionID;
			m_ExpReward.dwCount = pExp->dwCount;

			break;
		}
	case DBO_GR_MATCH_FINISH: //�������
		{
			InitMatchConfig();
			break;
		}
	default:
		break;
	}
	return true;
}

//������Ϣ
bool CGameMatchServiceManager::OnEventSocketMatch(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GR_MATCH_JOIN:
		{
			if ( MatchJoinCheck(pIServerUserItem) == false ) break;
			if ( MatchJoinTimeCheck(pIServerUserItem) == false ) break;
			//if ( MatchJoinRequireCheck(pIServerUserItem) == false ) break;
			//if ( MatchJoinFeeCheck(pIServerUserItem) == false ) break;

			MatchJoinPostDB(pData,wDataSize,pIServerUserItem,dwSocketID);
		}
		break;
	case SUB_GR_LEAVE_MATCH:	//�˳����� ����������
		{
			return OnUserQuitGame(pIServerUserItem,0,0,dwSocketID);
		}
	}
	return true;
}

void CGameMatchServiceManager::GetMatchGlobal(CMD_GR_MatchGlobal& MatchGlobal)
{
	ZeroMemory(&MatchGlobal,sizeof(MatchGlobal));

	CopyMemory(&MatchGlobal.Base,	&m_MatchGlobal.Base,	sizeof(CMD_GR_MatchBase));
	CopyMemory(&MatchGlobal.Solution,	&m_MatchGlobal.Solution,	sizeof(MatchGlobal.Solution));
	CopyMemory(&MatchGlobal.StageArray,	&m_MatchGlobal.StageArray,	sizeof(CMD_GR_StageArray));
	CopyMemory(&MatchGlobal.Require,&m_MatchGlobal.Require,	sizeof(MatchGlobal.Require));
	CopyMemory(&MatchGlobal.Fee,	&m_MatchGlobal.Fee,	sizeof(MatchGlobal.Fee));
	CopyMemory(&MatchGlobal.DateTime,	&m_MatchGlobal.DateTime,	sizeof(MatchGlobal.DateTime));
	CopyMemory(&MatchGlobal.MatchRevive,	&m_MatchGlobal.MatchRevive,	sizeof(MatchGlobal.MatchRevive));
	CopyMemory(&MatchGlobal.Tagout,	&m_MatchGlobal.Tagout,	sizeof(MatchGlobal.Tagout));
}

ITableFrame* CGameMatchServiceManager::GetTableFrame(IServerUserItem *pIServerUserItem)
{
	if ( pIServerUserItem == NULL ) return NULL;
	WORD wTableID = pIServerUserItem->GetTableID();
	return m_ppITableFrame[wTableID];
}

void CGameMatchServiceManager::InitMatchTime()
{
	m_WeekDay[0] = 0;
	m_WeekDay[1] = m_MatchGlobal.DateTime.cbMonIndex;
	m_WeekDay[2] = m_MatchGlobal.DateTime.cbTuesIndex;
	m_WeekDay[3] = m_MatchGlobal.DateTime.cbWednesIndex;
	m_WeekDay[4] = m_MatchGlobal.DateTime.cbThursIndex;
	m_WeekDay[5] = m_MatchGlobal.DateTime.cbFriIndex;
	m_WeekDay[6] = m_MatchGlobal.DateTime.cbSaturIndex;
	m_WeekDay[7] = m_MatchGlobal.DateTime.cbSunIndex;
}

void CGameMatchServiceManager::InitTimer()
{
	//������ʱ��
	{
		//����������ⶨʱ��
		m_pITimerEngine->SetTimer(IDI_DELETE_OVER_MATCH, 8000,TIMES_INFINITY,0);
		//��������ѯ��ʱ��
		m_pITimerEngine->SetTimer(IDI_CHECK_MATCH_GROUP,5000L,TIMES_INFINITY,0);
		//��ӻ����˶�ʱ��
		m_pITimerEngine->SetTimer(IDI_MATCH_POST_ANDROID, 6000, TIMES_INFINITY, 0);
		//�����˱�����ʱ��
		m_pITimerEngine->SetTimer(IDI_ANDROID_SIGN_UP,1000,TIMES_INFINITY,0);

		//������ʱ��
		if ( m_MatchGlobal.Base.dwMatchType == MatchType_FullUser )
			m_pITimerEngine->SetTimer(IDI_CHECK_MATCH_START, 3000,TIMES_INFINITY,0);
		if ( m_MatchGlobal.Base.dwMatchType == MatchType_Timing )
		{
// 			SYSTEMTIME CurTime;
// 			GetLocalTime(&CurTime);
// 			SYSTEMTIME StartTime =  m_MatchGlobal.DateTime.sysStartTime;
// 			DWORD dwLocalTime = CurTime.wHour*3600 + CurTime.wMinute*60 + CurTime.wSecond;
// 			StartTime.wHour = 14;
// 			StartTime.wMinute = 42;
// 			StartTime.wSecond = 59;
// 			DWORD dwStartTime = StartTime.wHour*3600 + StartTime.wMinute*60 + StartTime.wSecond;
// 			int dwInterval = dwStartTime - dwLocalTime + 5;
// 			if ( dwInterval <= 0 ) 
// 			{
// 				D_TRACE(TEXT("����ʱ���ѹ����޷�����������ʱ��!"));
// 				return ;
// 			}
// 			m_pITimerEngine->SetTimer(IDI_MATCH_TIME_START,dwInterval*1000,1,0);

			m_pITimerEngine->SetTimer(IDI_MATCH_TIME_START,60*1000,1,0);
		}			

	}
}

bool CGameMatchServiceManager::MatchJoinTimeCheck(IServerUserItem * pIServerUserItem)
{
	//1.�����������ڣ��ر�����

	//2.������������
	SYSTEMTIME sysToDay;
	GetLocalTime(&sysToDay);
	BYTE w = ReturnWeekDay(sysToDay.wYear,sysToDay.wMonth,sysToDay.wDay);
	if ( m_WeekDay[w] == 0 ) 
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("���첻��������������������!"),SMT_EJECT);
		return false;
	}

	//3. ��ʱ�����ڿ���ʱ�䣬��������
	if ( m_MatchGlobal.Base.dwMatchType == MatchType_Timing )
	{
		SYSTEMTIME CurTime;
		GetLocalTime(&CurTime);
		SYSTEMTIME StartTime =  m_MatchGlobal.DateTime.sysStartTime;
		DWORD dwLocalTime = CurTime.wHour*3600 + CurTime.wMinute*60 + CurTime.wSecond;
		DWORD dwStartTime = StartTime.wHour*3600 + StartTime.wMinute*60 + StartTime.wSecond;
		if ( dwLocalTime >dwStartTime )
		{
			if ( pIServerUserItem->IsAndroidUser() == false )
				m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("�����Ѿ���ʼ�����Ժ���!"),SMT_EJECT);
			return false;
		}
	}

	//4.������ǰ����ʱ�� 0�賿��������0��ǰ���ٷ��ӱ���
	DWORD dwJoinTime = m_MatchGlobal.DateTime.dwJoinTime;
	if ( dwJoinTime == 0 ) return true;
	else 
	{
		SYSTEMTIME CurTime;
		GetLocalTime(&CurTime);
		SYSTEMTIME StartTime =  m_MatchGlobal.DateTime.sysStartTime;

		DWORD dwLocalTime = CurTime.wHour*3600 + CurTime.wMinute*60 + CurTime.wSecond;
		DWORD dwStartTime = StartTime.wHour*3600 + StartTime.wMinute*60 + StartTime.wSecond;
		if ( dwLocalTime < dwStartTime - dwJoinTime )
		{
			if ( pIServerUserItem->IsAndroidUser() == false )
				m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("����ʱ�仹û��ʼ�����Ժ���!"),SMT_EJECT);
			return false;
		}
	}

	return true;
}

bool CGameMatchServiceManager::MatchJoinCheck(IServerUserItem * pIServerUserItem)
{
	if ( pIServerUserItem == NULL ) return false;

	//����Ƕ�ʱ����m_pCurMatchָ��ǰ�����������������ָ����һ������
	if(  m_MatchGlobal.Base.dwMatchType == MatchType_Timing && ( m_pCurMatch==NULL || m_pCurMatch->m_bAssignTable==true) )
	{
		SendRoomMessage(pIServerUserItem, TEXT("��Ǹ�������Ѿ���ʼ�����Ժ��ٱ���"),SMT_CHAT|SMT_EJECT);
		return false;;
	}
	//if ( m_pCurMatch == NULL ) return false;

	if(  m_MatchGlobal.Base.dwMatchType == MatchType_FullUser && ( m_pCurMatch==NULL || m_pCurMatch->m_bAssignTable==false) )
	{
		SendRoomMessage(pIServerUserItem, TEXT("��Ǹ���÷�������������������Ժ��ٱ������߽�����һ���������"),SMT_CHAT|SMT_EJECT);
		return false;;
	}

	if ( m_pCurMatch->m_OnMatchUserMap.GetCount() >= m_MatchGlobal.Solution.dwMaxJoinCount )
	{
		SendRoomMessage(pIServerUserItem, TEXT("����������������ȴ���һ��������"),SMT_CHAT|SMT_EJECT);
		return false;
	}

	for (INT_PTR i=0;i<m_OnMatchUserItem.GetCount();i++)
	{
		if(m_OnMatchUserItem.GetAt(i)==pIServerUserItem)
		{
			SendRoomMessage(pIServerUserItem, TEXT("���Ѿ��ɹ������������ظ�������"),SMT_CHAT|SMT_EJECT);
			return false;
		}
	}

	//������̭������
	for (INT_PTR i=0;i<m_OutUserItem.GetCount();i++)
	{
		if(m_OutUserItem.GetAt(i)==pIServerUserItem)
		{
			SendRoomMessage(pIServerUserItem, TEXT("��ȴ�5����ٽ��б�����лл������ϣ�ף��������죡"),SMT_CHAT|SMT_EJECT);
			return false;
		}
	}

	if(pIServerUserItem->GetUserStatus()!=US_FREE || pIServerUserItem->GetTableID()!=INVALID_TABLE)
	{
		SendRoomMessage(pIServerUserItem, TEXT("�Բ�������ǰ��״̬������μӱ�����"),SMT_CHAT|SMT_EJECT);
		return false;
	}

	return true;
}

bool CGameMatchServiceManager::MatchJoinRequireCheck(IServerUserItem * pIServerUserItem)
{
	//У���û���������
	for (int i=0;i<m_MatchGlobal.Require.wCount;i++)
	{
		//������
		if (m_MatchGlobal.Require.Require[i].dwRequreType==0)
		{
			break;
		}
		//��ʦ��
		else if (m_MatchGlobal.Require.Require[i].dwRequreType==ScoreType_Master)
		{
			if (pIServerUserItem->GetUserInfo()->lMasterScore<m_MatchGlobal.Require.Require[i].dwRequreCount)
			{
				SendRoomMessage(pIServerUserItem, TEXT("���Ĵ�ʦ�ֲ����㱨��������"),SMT_CHAT|SMT_EJECT);
				return false;
			}
		}
		//VIP
		else if (m_MatchGlobal.Require.Require[i].dwRequreType==ScoreType_VIP)
		{
			if (pIServerUserItem->GetUserInfo()->cbMemberOrder <m_MatchGlobal.Require.Require[i].dwRequreCount)
			{
				SendRoomMessage(pIServerUserItem, TEXT("���Ļ�Ա�ȼ������㱨��������"),SMT_CHAT|SMT_EJECT);
				return false;
			}
		}
		//����������
		else if (m_MatchGlobal.Require.Require[i].dwRequreType==ScoreType_GameExp)
		{
			if (pIServerUserItem->GetUserInfo()->lLandLV <m_MatchGlobal.Require.Require[i].dwRequreCount)
			{
				SendRoomMessage(pIServerUserItem, TEXT("���Ķ��������鲻���㱨��������"),SMT_CHAT|SMT_EJECT);
				return false;
			}
		}
	}
	return true;
}

//У���û�������
bool CGameMatchServiceManager::MatchJoinFeeCheck(IServerUserItem * pIServerUserItem)
{
	for (int i=0;i<m_MatchGlobal.Fee.wCount;i++)
	{
		//������
		if (m_MatchGlobal.Fee.Fee[i].dwFeeType==0)
		{
			return true;
		}
		//���
		else if (m_MatchGlobal.Fee.Fee[i].dwFeeType==ScoreType_Score)
		{
			if (pIServerUserItem->GetUserInfo()->lScore <m_MatchGlobal.Fee.Fee[i].dwFeeCount)
			{
				SendRoomMessage(pIServerUserItem, TEXT("���Ľ�Ҳ����㱨�����ã�"),SMT_CHAT|SMT_EJECT);
				return false;
			}
		}
		//������
		else if (m_MatchGlobal.Fee.Fee[i].dwFeeType==ScoreType_Race)
		{
			if (pIServerUserItem->GetUserInfo()->lRaceScore <m_MatchGlobal.Fee.Fee[i].dwFeeCount)
			{
				SendRoomMessage(pIServerUserItem, TEXT("���ľ����ֲ����㱨�����ã�"),SMT_CHAT|SMT_EJECT);
				return false;
			}
		}
		//����
		else if ( m_MatchGlobal.Fee.Fee[i].dwFeeType == ScoreType_Power )
		{
			if ( pIServerUserItem->GetUserInfo()->lPower < m_MatchGlobal.Fee.Fee[i].dwFeeCount)
			{
				SendRoomMessage(pIServerUserItem,TEXT("��������ֵ�����㱨�����ã�"),SMT_CHAT|SMT_EJECT);
				return false;
			}
		}

		//PKȯ�ڴ洢�������ж�	
	}
	return true;
}

void CGameMatchServiceManager::AddMatchUserItem(IServerUserItem *pIServerUserItem)
{
	if ( pIServerUserItem == NULL ) return ;
	m_OnMatchUserItem.Add(pIServerUserItem);
}

void CGameMatchServiceManager::MatchJoinPostDB(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
	//��������,�������ã��ڴ洢���̴���
	CMD_GR_UserMatchJoin* pMatchJoin = (CMD_GR_UserMatchJoin*)pData;
	DBR_GR_UserMatchJoin MatchFee;
	MatchFee.dwUserID	=	pIServerUserItem->GetUserID();
	MatchFee.wServerID	=	m_pGameServiceOption->wServerID;
	MatchFee.dwJoinType	=	m_MatchGlobal.Fee.Fee[0].dwFeeType;
	MatchFee.dwJoinFee	=	m_MatchGlobal.Fee.Fee[0].dwFeeCount;
	MatchFee.dwJoinTypeTwo	=	m_MatchGlobal.Fee.Fee[1].dwFeeType;
	MatchFee.dwJoinFeeTwo	=	m_MatchGlobal.Fee.Fee[1].dwFeeCount;
	MatchFee.dwJoinTypeThree	=	m_MatchGlobal.Fee.Fee[2].dwFeeType;
	MatchFee.dwJoinFeeThree	=	m_MatchGlobal.Fee.Fee[2].dwFeeCount;
	MatchFee.dwJoinTypeFour	=	m_MatchGlobal.Fee.Fee[3].dwFeeType;
	MatchFee.dwJoinFeeFour	=	m_MatchGlobal.Fee.Fee[3].dwFeeCount;
	MatchFee.dwJoinTypeFive	=	m_MatchGlobal.Fee.Fee[4].dwFeeType;
	MatchFee.dwJoinFeeFive	=	m_MatchGlobal.Fee.Fee[4].dwFeeCount;
	MatchFee.cbQuitMatch=	0;	
	MatchFee.dwMatchID	=	m_pCurMatch->m_dwMatchID;
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_MATCH_JOIN,dwSocketID,&MatchFee,sizeof(MatchFee));

}

void CGameMatchServiceManager::InitMatchConfig()
{
	//��ʼ����ʱ������ ������30����ʱ��
	for (DWORD i=0;i<30;i++)
	{
		tagMatchTimerItem *pTimeItem=new tagMatchTimerItem;
		pTimeItem->dwTimerID=IDI_GROUP_START+i;
		m_TimerItemFree.Add(pTimeItem);
	}

	//��������ʵ��
	CreateMatchInstance();
	//���ñ���ʱ��
	InitMatchTime();

	InitTimer();

	//֪ͨ������
	m_pIGameServiceFrame->SendServerInfo();
}

void CGameMatchServiceManager::CreateMatchInstance()
{
	//��������������֪��������֪
	if ( m_MatchGlobal.Base.dwMatchType == MatchType_FullUser )
	{	
		m_pCurMatch=new CChampionshipMatch(&m_MatchGlobal, (DWORD)time(NULL),m_pGameServiceAttrib->wChairCount, this);
		m_pCurMatch->InitMatchBaseScore();
		AssignTableForFullUser(m_pCurMatch, m_MatchGlobal.Solution.dwMaxJoinCount/m_pGameServiceAttrib->wChairCount); 
	}
	else if ( m_MatchGlobal.Base.dwMatchType == MatchType_Timing )
	{
		m_pCurMatch=new CChampionshipMatch(&m_MatchGlobal, (DWORD)time(NULL),m_pGameServiceAttrib->wChairCount, this);
		if ( m_pCurMatch != NULL ) m_pCurMatch->SetCurMatchStatus(MS_MATCH_JOIN);
	}
}

void CGameMatchServiceManager::CreateMatchInstanceAfter()
{
	m_pCurMatch == NULL;
	if ( m_MatchGlobal.Base.dwMatchType == MatchType_FullUser )
	{	
		m_pCurMatch=new CChampionshipMatch(&m_MatchGlobal, (DWORD)time(NULL),m_pGameServiceAttrib->wChairCount, this);
		m_pCurMatch->InitMatchBaseScore();
		AssignTableForFullUser(m_pCurMatch, m_MatchGlobal.Solution.dwMaxJoinCount/m_pGameServiceAttrib->wChairCount); 
	}
	else if ( m_MatchGlobal.Base.dwMatchType == MatchType_Timing )
	{
		m_pCurMatch = new CChampionshipMatch(&m_MatchGlobal, (DWORD)time(NULL),m_pGameServiceAttrib->wChairCount, this);	
	}

}

void CGameMatchServiceManager::CreateMatchInstanceEnd()
{
// 	if ( m_MatchGlobal.Base.dwMatchType == MatchType_Timing )
// 	{
// 		m_pCurMatch = new CChampionshipMatch(&m_MatchGlobal, (DWORD)time(NULL),m_pGameServiceAttrib->wChairCount, this);	
// 	}
	if ( m_MatchGlobal.Base.dwMatchType == MatchType_FullUser )
	{
		if ( m_pCurMatch != NULL && m_pCurMatch->m_bAssignTable == false )
			AssignTableForFullUser(m_pCurMatch, m_MatchGlobal.Solution.dwMaxJoinCount/m_pGameServiceAttrib->wChairCount);	
	}
}

bool CGameMatchServiceManager::OnEventMatchStart(CChampionshipMatch *pMatch)
{
	m_MatchGroup.Add(pMatch);

	//��һ�������Ѿ�������������һ������,��ʱ���ڱ�������������
	CreateMatchInstanceAfter();
	return true;
}

//��������
bool CGameMatchServiceManager::OnEventMatchOver(CChampionshipMatch *pMatch)
{
	//�������߳��ѽ�������
	for (INT_PTR i=0;i<m_MatchGroup.GetCount();i++)
	{
		CChampionshipMatch *pTempMatch=m_MatchGroup.GetAt(i);
		if (pTempMatch==pMatch)
		{
			m_MatchGroup.RemoveAt(i);
			break; 
		}
	}
	//�����������
	m_OverGroup.Add(pMatch);

	return true;
}

IServerUserItem * CGameMatchServiceManager::SearchMatchUser(DWORD dwUserID)
{
	if ( dwUserID == 0 ) return NULL;
	return m_pIServerUserManager->SearchUserItem(dwUserID);
}
//////////////////////////////////////////////////////////////////////////////////

//�����������
DECLARE_CREATE_MODULE(GameMatchServiceManager);

//////////////////////////////////////////////////////////////////////////
