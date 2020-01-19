#include "StdAfx.h"
#include ".\championshipmatch.h"
#include <cmath>
#include <ctime>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CChampionshipMatch::CChampionshipMatch(CMD_GR_MatchGlobal* pMatchGlobal, DWORD dwMatchID,WORD wChairCount,IChampionshipMatchSink *pSink)
	:m_pMatchGlobal(pMatchGlobal),m_dwMatchID(dwMatchID),m_wChairCount(wChairCount),m_pMatchSink(pSink)
{
	m_pAssignTable=NULL;
	m_wTableCount=0;
	m_dwAddBaseTime=0;
	m_bGameEnd=false;
	m_enMatchStatus=MS_NULL_STATUS;
	m_wFinishTable=0;
	m_bAssignTable=false;
	m_wAndroidUserCount=0;

	m_cbCurCount=0;
	m_cbCurRound=0;

	m_dwInitalScore	= 0;
	m_dwCurBase		= 0;

	InitMatchType();		//��ʼ����������
	InitMatchSolution();	//��ʼ����������
	InitMatchUserCount();	//��ʼ����������

	m_dwAddBaseTime=0;
	m_dwWaitTableTime=0;
	m_bStarted = false;

	m_bSnakeSort = false;
	m_cbWait = false;
}

CChampionshipMatch::~CChampionshipMatch(void)
{
	SafeDeleteArray(m_pAssignTable);
	m_pAssignTable=NULL;
	m_wTableCount=0;
	m_dwCurBase=0;
	m_dwAddBaseTime=0;
	m_bGameEnd=false;
	m_enMatchStatus=MS_NULL_STATUS;
	m_wFinishTable=0;
	m_dwCurBase=0;
	m_bAssignTable=false;
	m_wAndroidUserCount=0;
	m_dwInitalScore=0;

}



//�ӿڲ�ѯ
VOID* CChampionshipMatch::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameEventSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameEventSink,Guid,dwQueryVer);
	return NULL;
}

//�û�����
bool CChampionshipMatch::OnUserSignUp(IServerUserItem * pUserItem)
{
	if (pUserItem==NULL) return false;
	IServerUserItem * pIServerUserExist=NULL;
	m_OnMatchUserMap.Lookup(pUserItem->GetUserID(),pIServerUserExist);
	if (pIServerUserExist!=NULL) return false;

	if ( m_cbMatchType == MatchType_FullUser )
	{
		tagUserInfo *pUserScore=pUserItem->GetUserInfo();
		pUserScore->dwWinCount=0L;
		pUserScore->dwLostCount=0L;
		pUserScore->dwFleeCount=0L;
		pUserScore->dwDrawCount=0L;
		pUserScore->lGrade=0L; //���ڽڷ���ʿ
		pUserScore->lScore=(SCORE)m_dwInitalScore;

		//���ͳɼ�
		m_pMatchSink->SendGroupUserScore(pUserItem,this);
	}
	
	if ( pUserItem->IsAndroidUser() ) ++m_wAndroidUserCount;
	m_OnMatchUserMap[pUserItem->GetUserID()]=pUserItem;
	m_pMatchSink->AddMatchUserItem(pUserItem);

	m_pMatchSink->SendMatchStatus(pUserItem,MS_SIGNUP);

	return true;
}

//�û�����
BOOL CChampionshipMatch::OnUserQuitMatch(IServerUserItem *pUserItem,bool bMatchNotOpen)
{
	//�û��ж�
	ASSERT(pUserItem!=NULL);
	if (pUserItem==NULL) return false;

	//�����û�
	IServerUserItem * pIServerUserExist=NULL;
	m_OnMatchUserMap.Lookup(pUserItem->GetUserID(),pIServerUserExist);

	//�û��ж�
	if (pIServerUserExist==NULL) return FALSE;

		if (pUserItem->IsAndroidUser())
			 m_wAndroidUserCount--;

		//����������ӱ���������ɾ��
		m_OnMatchUserMap.RemoveKey(pUserItem->GetUserID());
		//��������״̬
		m_pMatchSink->SendMatchStatus(pUserItem,MS_NULL);

		//����û�п�ʼ ֱ�ӷ���
		if ( bMatchNotOpen ) return TRUE;

		if ( MatchStandUpAction(pUserItem,true) == true ) return TRUE;


	return FALSE;
}

//��������
void CChampionshipMatch::SetMatchTable(ITableFrame *pTable[], WORD wTableCount)
{
	if(pTable==NULL || wTableCount==0)return ;

	m_pAssignTable=new ITableFramEx[wTableCount];
	ZeroMemory(m_pAssignTable,sizeof(ITableFramEx)*wTableCount);
	for (WORD i=0;i<wTableCount;i++)
		m_pAssignTable[i].pTableFrame=pTable[i];
	m_wTableCount=wTableCount;
	m_bAssignTable=true;
	//�ֺ������ñ���״̬
	SetCurMatchStatus(MS_MATCH_JOIN);
}

//��ȡ����λ
ITableFramEx* CChampionshipMatch::GetEmptyChair(WORD &wTableID, WORD &wChairID)
{
	for (WORD i=0;i<m_wTableCount;i++)
	{
		ITableFramEx & pTable=m_pAssignTable[i];
		if ( pTable.pTableFrame == NULL ) continue;
		if(pTable.pTableFrame->IsGameStarted()) continue;

		for(WORD j=0;j<m_wChairCount;j++)
		{
			if(pTable.pTableFrame->GetTableUserItem(j)==NULL)
			{
				wTableID=i;
				wChairID=j;
				return &m_pAssignTable[i];
			}
		}
	}
	D_TRACE(TEXT("���Ӷ�����"));
	return NULL;
}

ITableFramEx* CChampionshipMatch::GetSnakeEmptyChair(WORD &wTableID, WORD &wChairID)
{
	for (WORD i=0;i<m_wTableCount;i++)
	{
		ITableFramEx & pTable=m_pAssignTable[i];
		if ( pTable.pTableFrame == NULL ) continue;
		if(pTable.pTableFrame->IsGameStarted()) continue;

		if ( pTable.pTableFrame->GetTableUserItem(wChairID)==NULL )
		{
			wTableID=i;
			return &m_pAssignTable[i];
		}
	}
	return NULL;
}

//��Ϸ��ʼ
bool CChampionshipMatch::OnEventGameStart(ITableFrame *pITableFrame, WORD wChairCount)
{
	//if ( m_dwCurStageType == StageRule_Dali || 
	//	m_dwCurStageType == StageRule_ASSDali )
	{
		pITableFrame->SetCellScore(m_dwCurBase);
	}

	SendUserRank(pITableFrame);

	return true;
}
//��Ϸ����
bool CChampionshipMatch::OnEventGameEnd(ITableFrame *pITableFrame,WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	m_bGameEnd=true;

	//һ�ֽ����жϱ�������
	if ( m_dwCurStageType == StageRule_Dali ) DaLi_OnEventGameEnd(pITableFrame);
	else if (m_dwCurStageType == StageRule_ASSDali )		AssDaLi_OnEventGameEnd(pITableFrame);
	else if ( m_dwCurStageType == StageRule_CountScore	)	CountScore_OnEventGameEnd(pITableFrame);
	else if ( m_dwCurStageType == StageRule_SwissShift )	SwissShift_OnEventGameEnd(pITableFrame);
	else if ( m_dwCurStageType == StageRule_SwissScore )	 SwissScore_OnEventGameEnd(pITableFrame);

	//��Ϸ������������
	//��Ϸ��ʼ����Ϸ������������ͳ�Ƶģ�һ������ֻ���͸����������
	SendUserRank(pITableFrame);

	m_bGameEnd=false;
	return true;
}
//��ʱ��
bool CChampionshipMatch::OnTimeMessage(DWORD dwTimerID, WPARAM dwBindParameter)
{
	//״̬У��
	if(m_enMatchStatus==MS_NULL_STATUS) return true;
	//��ʱ����ȫ�ֶ�ʱ����һ����ʱ��Ϣ���ߵ����б���ʵ��
	switch(dwTimerID)
	{
	case IDI_CHECK_MATCH_START: //������������ʱ��
		{
			if ( m_cbMatchType != MatchType_FullUser ) return true;
			if ( m_bStarted ) break;

			if ( m_OnMatchUserMap.GetCount() < m_dwMatchUserCount ) return true;

			m_pMatchSink->StartMatchCheck();
			
			return true;
		}
	case  IDI_MATCH_TIME_START: //��ʱ��������ʱ��
		{
			if ( m_cbMatchType != MatchType_Timing ) return true;
			if ( m_bStarted ) break;
			//����ǰҪȷ�������������ͣ��������ͷ�,��������
			//1.
			m_dwMatchUserCount = m_OnMatchUserMap.GetCount();
			if ( m_dwMatchUserCount < m_pMatchGlobal->Solution.dwMinJoinCount )
			{
				m_pMatchSink->SendGameMessage(NULL,TEXT("�ܱ�Ǹ�����������Ƚ��������٣������޷��������У������ѽ�������رձ���"),SMT_CHAT|SMT_TABLE_ROLL,this);
				MatchOver();
				return true;
			}
			//2.������������ȷ����ȡ���׷��� 3.ȷ������
			InitMatchSolutionForTime();
			
			//4.ȷ���ͷ� ����
			InitMatchBaseScore();
			//5.��������  11/3=3
			WORD wTableCount = m_dwMatchUserCount/m_wChairCount;
			if ( m_dwMatchUserCount%m_wChairCount != 0 ) wTableCount++;
			m_pMatchSink->AssignTable(this,wTableCount);
			
			
			m_pMatchSink->StartMatchCheck();

			return true;
		}
	case IDI_START_MATCH_START:
		{
			if ( m_enMatchStatus != MS_MATCH_START ) break;
			MatchSitDownAction();
			//������ʼ
			m_pMatchSink->OnEventMatchStart(this);
			SetCurMatchStatus(MS_MATCH_MATCHING);
			
			return true;
		}
	case IDI_CHECK_MATCH_GROUP: //3sһ����ѯ
		{		
			if ( m_bStarted == false ) break;
			if ( m_bGameEnd == true ) break;
			if ( m_cbWait == false) 
				m_cbWait=true;  
			else break;
			//���������Ƚ�������������������Ƿ������һ��
 			if( m_dwCurStageType == StageRule_Dali )				 DaLi_MatchProc();
			else if ( m_dwCurStageType == StageRule_ASSDali )		 AssDaLi_MatchProc();
 			else if ( m_dwCurStageType == StageRule_CountScore	)	 CountScore_MatchProc();
 			else if ( m_dwCurStageType == StageRule_SwissShift )	 SwissShift_MatchProc();
 			else if ( m_dwCurStageType == StageRule_SwissScore )	 SwissScore_MatchProc();

			m_cbWait=false;
		}
		return true;
	}
	return true;
}

bool CChampionshipMatch::MatchPromotionAction(DWORD dwCount)
{
	INT_PTR nCount=m_OnMatchUserMap.GetCount();

	//���������Ƚ��������٣�ֱ�ӽ�������
	if(nCount< dwCount )
	{
		m_pMatchSink->SendGameMessage(NULL,TEXT("�ܱ�Ǹ�����������Ƚ��������٣������޷��������У������ѽ�������رձ���"),SMT_CHAT|SMT_TABLE_ROLL,this);
		MatchOver();
		return false;
	}

	tagMatchScore *pScore=new tagMatchScore[nCount];
	ZeroMemory(pScore,sizeof(tagMatchScore)*nCount);

	//�����������ߵ����򣬴Ӹߵ���, Array[0]>Array[1]
	MatchScoreSortAction(pScore);
	//����ѹ��ϵ��
	m_fRatio = m_fMeanValue/pScore[0].lScore;
	INT_PTR nNextRoundUserCount=dwCount;
	for (INT_PTR i=0;i<nCount;i++)
	{
		if(i>=nNextRoundUserCount)
		{
			m_OnMatchUserMap.RemoveKey(pScore[i].dwUserID); //��̭����Ѿ��߳��������飬��û������
			m_pMatchSink->WriteUserAward(pScore[i].pUserItem, (WORD)i+1, m_dwMatchID, NULL,this);
		}
		else m_pMatchSink->SendGameMessage(pScore[i].pUserItem, TEXT("��ϲ�������ɹ������Ժ���һ�ֱ���������ʼ��"), SMT_CHAT|SMT_TABLE_ROLL);

	}
	SafeDeleteArray(pScore);

	return true;
}

//��ֽ���
bool CChampionshipMatch::MatchGradePromotionAction(DWORD dwCount)
{
	INT_PTR nCount=m_OnMatchUserMap.GetCount();

	//���������Ƚ��������٣�ֱ�ӽ�������
	if(nCount< dwCount )
	{
		m_pMatchSink->SendGameMessage(NULL,TEXT("�ܱ�Ǹ�����������Ƚ��������٣������޷��������У������ѽ�������رձ���"),SMT_CHAT|SMT_TABLE_ROLL,this);
		MatchOver();
		return false;
	}

	tagMatchScore *pScore=new tagMatchScore[nCount];
	ZeroMemory(pScore,sizeof(tagMatchScore)*nCount);

	//�������
	MatchGradeSortAction(pScore);
	//����ѹ��ϵ��
	m_fRatio = m_fMeanValue/pScore[0].lScore;
	INT_PTR nNextRoundUserCount=dwCount;
	for (INT_PTR i=0;i<nCount;i++)
	{
		if(i>=nNextRoundUserCount)
		{
			m_OnMatchUserMap.RemoveKey(pScore[i].dwUserID); //��̭����Ѿ��߳��������飬��û������
			m_pMatchSink->WriteUserAward(pScore[i].pUserItem, (WORD)i, m_dwMatchID, NULL,this);
		}
		else m_pMatchSink->SendGameMessage(pScore[i].pUserItem, TEXT("��ϲ�������ɹ������Ժ���һ�ֱ���������ʼ��"), SMT_CHAT|SMT_TABLE_ROLL);

	}
	SafeDeleteArray(pScore);

	return true;
}

//��������
DWORD CChampionshipMatch::MatchScoreSortAction(tagMatchScore score[])
{
	INT_PTR nCount=0;
	POSITION pos=m_OnMatchUserMap.GetStartPosition();

	while(pos!=NULL)
	{
		m_OnMatchUserMap.GetNextAssoc(pos,score[nCount].dwUserID, score[nCount].pUserItem);
		score[nCount].lScore=score[nCount].pUserItem->GetUserScore();

		nCount++;
	}

	for(INT_PTR i = 1;i < nCount;i++)
	{
		INT_PTR left = 0,right = i - 1;
		tagMatchScore  Temp = score[i];
		while(left <= right)
		{
			INT_PTR mid = (left + right)/2;
			if(score[i].lScore > score[mid].lScore)right = mid - 1;
			else left = mid + 1;
		}
		for(INT_PTR j = i - 1;j >= left;j--)
			score[j+1] = score[j];
		score[left] = Temp;
	}
	return nCount;
}

DWORD CChampionshipMatch::OutUserSortAction(tagMatchScore score[])
{
	INT_PTR nCount=0;
	POSITION pos=m_ReadyOutUserMap.GetStartPosition();

	while(pos!=NULL)
	{
		m_ReadyOutUserMap.GetNextAssoc(pos,score[nCount].dwUserID, score[nCount].pUserItem);
		score[nCount].lScore=score[nCount].pUserItem->GetUserScore();

		nCount++;
	}

	for(INT_PTR i = 1;i < nCount;i++)
	{
		INT_PTR left = 0,right = i - 1;
		tagMatchScore  Temp = score[i];
		while(left <= right)
		{
			INT_PTR mid = (left + right)/2;
			if(score[i].lScore > score[mid].lScore)right = mid - 1;
			else left = mid + 1;
		}
		for(INT_PTR j = i - 1;j >= left;j--)
			score[j+1] = score[j];
		score[left] = Temp;
	}
	return nCount;
}

DWORD CChampionshipMatch::NextRoundUserSortAction(tagMatchScore score[])
{
	INT_PTR nCount=0;
	POSITION pos=m_NextRoundUserMap.GetStartPosition();

	while(pos!=NULL)
	{
		m_NextRoundUserMap.GetNextAssoc(pos,score[nCount].dwUserID, score[nCount].pUserItem);
		score[nCount].lScore=score[nCount].pUserItem->GetUserScore();

		nCount++;
	}

	for(INT_PTR i = 1;i < nCount;i++)
	{
		INT_PTR left = 0,right = i - 1;
		tagMatchScore  Temp = score[i];
		while(left <= right)
		{
			INT_PTR mid = (left + right)/2;
			if(score[i].lScore > score[mid].lScore)right = mid - 1;
			else left = mid + 1;
		}
		for(INT_PTR j = i - 1;j >= left;j--)
			score[j+1] = score[j];
		score[left] = Temp;
	}
	return nCount;
}

DWORD CChampionshipMatch::MatchGradeSortAction(tagMatchScore score[])
{
	INT_PTR nCount=0;
	POSITION pos=m_OnMatchUserMap.GetStartPosition();

	while(pos!=NULL)
	{
		m_OnMatchUserMap.GetNextAssoc(pos,score[nCount].dwUserID, score[nCount].pUserItem);
		score[nCount].lGrade=score[nCount].pUserItem->GetUserGrade();
		score[nCount].lScore=score[nCount].pUserItem->GetUserScore();

		nCount++;
	}

	for(INT_PTR i = 1;i < nCount;i++)
	{
		INT_PTR left = 0,right = i - 1;
		tagMatchScore  Temp = score[i];
		while(left <= right)
		{
			INT_PTR mid = (left + right)/2;
			if(score[i].lGrade > score[mid].lGrade)right = mid - 1; 
			else if ( score[i].lGrade == score[mid].lGrade && score[i].lScore > score[mid].lScore ) right = mid - 1; //�����ͬ�ǣ�����������
			else left = mid + 1;
		}
		for(INT_PTR j = i - 1;j >= left;j--)
			score[j+1] = score[j];
		score[left] = Temp;
	}
	return nCount;
}

//�������


//�������
WORD CChampionshipMatch::SortUserItemArray(tagMatchScore score[], CMatchUserItemArray &MatchUserItemArray)
{
	INT_PTR nCount=MatchUserItemArray.GetCount();

	for(INT_PTR i=0;i<nCount;i++)
	{
		score[i].pUserItem=MatchUserItemArray.GetAt(i);
		score[i].lScore=score[i].pUserItem->GetUserScore();
	}	
	for(INT_PTR i = 1;i < nCount;i++)
	{
		INT_PTR left = 0,right = i - 1;
		tagMatchScore  Temp = score[i];
		while(left <= right)
		{
			INT_PTR mid = (left + right)/2;
			if(score[i].lScore > score[mid].lScore)right = mid - 1;
			else left = mid + 1;
		}
		for(INT_PTR j = i - 1;j >= left;j--)
			score[j+1] = score[j];
		score[left] = Temp;
	}
	return (WORD)nCount;
}

//��ȡ����
WORD CChampionshipMatch::GetUserRank(IServerUserItem *pUserItem, ITableFrame *pITableFrame)
{
	//����Ч��
	ASSERT(pUserItem!=NULL);
	if(pUserItem==NULL) return INVALID_WORD;

	//�������
	SCORE lUserScore=pUserItem->GetUserScore();
	WORD wRank=1;
	DWORD dwUserID=0;
	IServerUserItem *pLoopUserItem=NULL;

	if(pITableFrame==NULL) //ͳ�������������������
	{
		//��������
		POSITION pos=m_OnMatchUserMap.GetStartPosition();
		while(pos!=NULL)
		{
			m_OnMatchUserMap.GetNextAssoc(pos,dwUserID, pLoopUserItem);
			if (pLoopUserItem==NULL) continue;
			if(pLoopUserItem!=pUserItem && pLoopUserItem->GetUserScore()>=lUserScore)
				++wRank;
		}
		if ( m_NextRoundUserMap.IsEmpty() == FALSE)  //ĪҪ�������map
		{
			pos = m_NextRoundUserMap.GetStartPosition();
			dwUserID=0;
			pLoopUserItem=NULL;
			while(pos!=NULL)
			{
				m_NextRoundUserMap.GetNextAssoc(pos,dwUserID, pLoopUserItem);
				if (pLoopUserItem==NULL) continue;
				if(pLoopUserItem!=pUserItem && pLoopUserItem->GetUserScore()>=lUserScore)
					++wRank;
			}
		}
	}
	else  //ͳ������ڵ�ǰ��������
	{
		ASSERT(pUserItem->GetTableID()==pITableFrame->GetTableID());
		if(pUserItem->GetTableID()!=pITableFrame->GetTableID()) return INVALID_WORD;

		for(WORD i=0; i<m_wChairCount; i++)
		{
			pLoopUserItem=pITableFrame->GetTableUserItem(i);
			if(pLoopUserItem==NULL)continue;

			if(pLoopUserItem!=pUserItem && pLoopUserItem->GetUserScore()>=lUserScore)
				++wRank;
		}
	}

	return wRank;
}

//�û�����
bool CChampionshipMatch::OnActionUserSitDown(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if(!bLookonUser && pIServerUserItem != NULL)
	{
		IServerUserItem *pUserItem=NULL;
		if(!m_OnMatchUserMap.Lookup(pIServerUserItem->GetUserID(),pUserItem))
		{
			if ( pIServerUserItem->GetUserID() != 0 )
			{
				CString str;
				str.Format(TEXT("����Ҳ����ⳡ�����У��ǳ�[%s],���Ӻţ�%d(%d)����:%d��"),
					pIServerUserItem->GetNickName(),pIServerUserItem->GetTableID(),wTableID,m_cbCurRound);

				DISK_LOG((LPCTSTR)str);
			}
		}

	}
	return true;
}

//��������
void CChampionshipMatch::MatchOver()
{
	for (WORD i=0; i<m_wTableCount; i++)
	{
		if(m_pAssignTable[i].pTableFrame!=NULL && m_pAssignTable[i].pTableFrame->IsGameStarted())
			m_pAssignTable[i].pTableFrame->DismissGame();
	}

	//ɾ���û�
	IServerUserItem *pLastUserItem=NULL;
	DWORD dwUserID;	

 	POSITION pos=m_OnMatchUserMap.GetStartPosition();
 	while(pos!=NULL)
 	{
 		m_OnMatchUserMap.GetNextAssoc(pos, dwUserID, pLastUserItem);
 		if(pLastUserItem!=NULL)
 			m_pMatchSink->OnUserOut(pLastUserItem,this);
 	}
	m_OnMatchUserMap.RemoveAll();

	m_enMatchStatus=MS_NULL_STATUS;
	m_pMatchSink->OnEventMatchOver(this);

}

//����������Ϸ����
void CChampionshipMatch::DaLi_OnEventGameEnd(ITableFrame *pTableFrame)
{
	DaLi_AddBaseScore();
	if ( m_enMatchStatus == MS_STOP_OUT )
	{
		m_pMatchSink->SendGameMessage(NULL,TEXT("��̭�����ѵ�����ȴ������������������������븴��"),SMT_CHAT|SMT_TABLE_ROLL,this);
		return ;
	}
	for (WORD j=0;j<m_wChairCount;j++)
	{
		IServerUserItem *pUserItem=pTableFrame->GetTableUserItem(j);
		if ( pUserItem == NULL ) continue;
		if ( pUserItem->GetUserStatus() == US_OFFLINE || pUserItem->GetUserScore() <= m_MatchStageInfo.Dali.dwLeastScore)
		{
			m_OnMatchUserMap.RemoveKey(pUserItem->GetUserID());
			m_pMatchSink->WriteUserAward(pUserItem, GetUserRank(pUserItem), m_dwMatchID, NULL,this);

			INT_PTR nCount=m_OnMatchUserMap.GetCount();

			//�������ֽ����ж�
			if(nCount<=m_MatchStageInfo.Dali.dwRemainUserCount && m_enMatchStatus!=MS_STOP_OUT)
			{
				m_enMatchStatus=MS_STOP_OUT;

				TCHAR szMsg[256]=TEXT("");
				_sntprintf(szMsg,CountArray(szMsg),TEXT("�Ѿ��ﵽ��̭��������Լ%d��󼴽���������������ȷ��������ң�"),m_MatchStageInfo.Dali.dwWaitTime);
				m_pMatchSink->SendGameMessage(NULL,szMsg,SMT_CHAT|SMT_TABLE_ROLL,this);					
			}					
		}
		else m_pMatchSink->SendGameMessage(pUserItem,TEXT("�����ѽ��������Ժ�ϵͳ������..."), SMT_CHAT|SMT_TABLE_ROLL);

	}
	 m_wFinishTable++;						//ÿ���һ�ֶ�Ҫ����
}

void CChampionshipMatch::AssDaLi_OnEventGameEnd(ITableFrame *pTableFrame)
{
	DaLi_AddBaseScore();
	if ( m_enMatchStatus == MS_STOP_OUT )
	{
		m_pMatchSink->SendGameMessage(NULL,TEXT("��̭�����ѵ�����ȴ������������������������븴��"),SMT_CHAT|SMT_TABLE_ROLL,this);
		return ;
	}
	for (WORD j=0;j<m_wChairCount;j++)
	{
		IServerUserItem *pUserItem=pTableFrame->GetTableUserItem(j);
		if ( pUserItem == NULL ) continue;
		if ( pUserItem->GetUserStatus() == US_OFFLINE || pUserItem->GetUserScore() <= m_MatchStageInfo.AssDali.dwLeastScore)
		{
			m_OnMatchUserMap.RemoveKey(pUserItem->GetUserID());
			m_pMatchSink->WriteUserAward(pUserItem, GetUserRank(pUserItem), m_dwMatchID, NULL,this);

			INT_PTR nCount=m_OnMatchUserMap.GetCount();

			//�������ֽ����ж�
			if(nCount<=m_MatchStageInfo.Dali.dwRemainUserCount && m_enMatchStatus!=MS_STOP_OUT)
			{
				m_enMatchStatus=MS_STOP_OUT;

				TCHAR szMsg[256]=TEXT("");
				_sntprintf(szMsg,CountArray(szMsg),TEXT("�Ѿ��ﵽ��̭��������Լ%d��󼴽���������������ȷ��������ң�"),m_MatchStageInfo.AssDali.dwWaitTime);
				m_pMatchSink->SendGameMessage(NULL,szMsg,SMT_CHAT|SMT_TABLE_ROLL,this);					
			}					
		}
		else m_pMatchSink->SendGameMessage(pUserItem,TEXT("�����ѽ��������Ժ�ϵͳ������..."), SMT_CHAT|SMT_TABLE_ROLL);
	}
	m_wFinishTable++;						//ÿ���һ�ֶ�Ҫ����
}

//�û���Ŀ
WORD CChampionshipMatch::GetSitUserCount(ITableFrame *pTableFrame)
{
	//����Ч��
	ASSERT(pTableFrame!=NULL);
	if(pTableFrame==NULL) return 0;

	if(pTableFrame->GetGameStatus()>=GAME_STATUS_PLAY) return m_wChairCount;

	//��������
	WORD wUserCount=0;

	//��Ŀͳ��
	for (WORD i=0;i<pTableFrame->GetChairCount();i++)
	{
		IServerUserItem *pUserItem=pTableFrame->GetTableUserItem(i);
		if (pUserItem!=NULL && pUserItem->GetUserStatus()<US_PLAYING) wUserCount++;
	}

	return wUserCount;
}



//��ȡ����
WORD CChampionshipMatch::GetNullChairID(ITableFrame *pTableFrame)
{
	ASSERT(pTableFrame!=NULL);
	if(pTableFrame==NULL) return INVALID_CHAIR;

	for(WORD i=0; i<pTableFrame->GetChairCount(); i++)
	{
		if(pTableFrame->GetTableUserItem(i)==NULL)
			return i;
	}
	return INVALID_CHAIR;
}

//������������ǰ�������
void CChampionshipMatch::SendUserRank(ITableFrame *pTableFrame)
{
	
	DWORD dwMatchUser = GetMatchUserCount();

	CMD_GR_MatchCurProc CurProc = {0};
	CurProc.cbCurStage		 = m_cbCurStage;
	CurProc.cbCurRule		 = m_dwCurStageType;	//��ǰ����									
	CurProc.dwCurBase		 = m_dwCurBase;			//��ǰ����
	CurProc.dwMatchUser		 = dwMatchUser;			//��ǰʣ������

	//�������� ASS��������
	if ( m_dwCurStageType == StageRule_ASSDali )
	{
		CurProc.dwOutScore		 = m_MatchStageInfo.AssDali.dwLeastScore;			//���ٻ���
		CurProc.dwPromotionCount = m_MatchStageInfo.AssDali.dwNextRoundUserCount;	//��������
		CurProc.dwRemainCount	 = m_MatchStageInfo.AssDali.dwRemainUserCount;		//��ֹ����
	}
	else if ( m_dwCurStageType == StageRule_Dali )
	{
		CurProc.dwOutScore		 = m_MatchStageInfo.Dali.dwLeastScore;			//���ٻ���
		CurProc.dwPromotionCount = m_MatchStageInfo.Dali.dwNextRoundUserCount;	//��������
		CurProc.dwRemainCount	 = m_MatchStageInfo.Dali.dwRemainUserCount;		//��ֹ����
	}
	else if ( m_dwCurStageType == StageRule_SwissShift)
	{
		CurProc.cbRoundCount	= m_MatchStageInfo.SwissShift.wRoundCount ;
		CurProc.cbGameCount		= m_MatchStageInfo.SwissShift.wGameCount;
		CurProc.cbCurRound		= m_cbCurRound;	
		CurProc.cbCurCount		= m_cbCurCount;
	}
	else if (m_dwCurStageType == StageRule_CountScore)
	{
		CurProc.cbRoundCount	= m_MatchStageInfo.Round.wRoundCount ;
		CurProc.cbGameCount		= m_MatchStageInfo.Round.wGameCount;
		CurProc.cbCurRound		= m_cbCurRound;	
		CurProc.cbCurCount		= m_cbCurCount;
	}
	else if (m_dwCurStageType == StageRule_SwissScore)
	{
		CurProc.cbRoundCount	= m_MatchStageInfo.SwissScore.wRoundCount ;
		CurProc.cbGameCount		= m_MatchStageInfo.SwissScore.wGameCount;
		CurProc.cbCurRound		= m_cbCurRound;	
		CurProc.cbCurCount		= m_cbCurCount;
	}

	for(WORD i=0; i<m_wChairCount; i++)
	{
		IServerUserItem *pIServerUserItem=pTableFrame->GetTableUserItem(i);
		if (pIServerUserItem == nullptr )
		{
			DISK_LOG(TEXT("��Ϸ���˵���,����%d"),pTableFrame->GetTableID());
			continue;
		}
		CurProc.dwCurRank		= GetUserRank(pIServerUserItem); 
		pTableFrame->SendTableData(i, SUB_GR_MATCH_INFO, &CurProc, sizeof(CurProc), MDM_GR_MATCH);//������������ǰ�������
	}
	
}

DWORD CChampionshipMatch::GetMatchUserCount()
{
	DWORD dwUserCount = 0;
	if ( m_dwCurStageType == StageRule_SwissShift)
		dwUserCount = m_OnMatchUserMap.GetCount();
	else if (m_dwCurStageType == StageRule_CountScore)
	{
		dwUserCount = m_OnMatchUserMap.GetCount()+m_NextRoundUserMap.GetCount();
	}
	else if (m_dwCurStageType == StageRule_SwissScore)
	{
		dwUserCount = m_OnMatchUserMap.GetCount();
	}	
	else
	{
		dwUserCount = m_OnMatchUserMap.GetCount();
	}
	return	dwUserCount;
}

WORD CChampionshipMatch::GetPlayingTable()
{
	WORD wPlaying=0;
	for(WORD i=0;i<m_wTableCount;++i)
	{
		if(m_pAssignTable[i].pTableFrame->GetGameStatus()>=GAME_STATUS_PLAY)
			++wPlaying;
	}
	return wPlaying;
}

void CChampionshipMatch::DaLi_AddBaseScore()
{
	DWORD dwTimeCount=(DWORD)time(NULL)-m_dwAddBaseTime;
	//���ӻ��� ��λ����

	DWORD dwGrowthTime = 0;
	DWORD dwGrowthRange = 0;
	if ( m_dwCurStageType == StageRule_Dali )
	{
		dwGrowthTime = m_MatchStageInfo.Dali.dwGrowthTime;
		dwGrowthRange = m_MatchStageInfo.Dali.dwGrowthRange;
	}
	else if ( m_dwCurStageType == StageRule_ASSDali )
	{
		dwGrowthTime = m_MatchStageInfo.AssDali.dwGrowthTime;
		dwGrowthRange = m_MatchStageInfo.AssDali.dwGrowthRange;
	}

	if(dwTimeCount>dwGrowthTime)
	{
		m_dwCurBase*=HundredPercent(dwGrowthRange);
		m_dwAddBaseTime=(DWORD)time(NULL);
	}
	
}

bool CChampionshipMatch::DaLi_WaitTable()
{
	DWORD dwWaitTime = 0;;
	if ( m_dwCurStageType == StageRule_Dali )
	{
		dwWaitTime = m_MatchStageInfo.Dali.dwWaitTime;
	}
	else if ( m_dwCurStageType == StageRule_ASSDali )
	{
		dwWaitTime = m_MatchStageInfo.AssDali.dwWaitTime;
	}

	DWORD dwWaitTable = (DWORD)time(NULL)-m_dwWaitTableTime;
	if ( dwWaitTable>dwWaitTime )
	{
		m_dwWaitTableTime = (DWORD)time(NULL);
		return true;
	}
	
	return false;
}

void CChampionshipMatch::MatchScoreUpdateAction()
{
	
	IServerUserItem *pUserItem=NULL;
	DWORD dwUserID;	

	POSITION pos=m_OnMatchUserMap.GetStartPosition();
	while(pos!=NULL)
	{
		m_OnMatchUserMap.GetNextAssoc(pos, dwUserID, pUserItem);
		if(pUserItem!=NULL)
		{
			tagUserInfo * pUserScore=pUserItem->GetUserInfo();
			//�����������㣬���������������轱��
			if (m_MatchStageInfo.Stage.wCount == m_cbCurStage+1)
			pUserScore->lScore = 0;
			else
				pUserScore->lScore = m_fRatio*pUserScore->lScore;

			m_pMatchSink->SendGroupUserScore(pUserItem,this);
		}
	}
	
}



//��������1������2���� �ķ� 3����
bool CChampionshipMatch::DaLi_MatchProc()
{
	//ֹͣ��̭����ȫ��������Ȼ���������ֿ������ܿ�����ӵ��
	if(m_enMatchStatus==MS_STOP_OUT)
	{
		bool bPlaying=false;
		for (WORD i=0;i<m_wTableCount;i++)
		{
			if ( m_pAssignTable[i].pTableFrame == NULL) continue;
			if(m_pAssignTable[i].pTableFrame->GetGameStatus()>=GAME_STATUS_PLAY)
				bPlaying=true;
		}
		if(bPlaying) return true;

		//����
		MatchStandUpAction(NULL,false,true);
		//����
		if (MatchPromotionAction(m_MatchStageInfo.Dali.dwNextRoundUserCount) == false) return false;
		//���ֱ��
		//MatchScoreUpdateAction();

		SetCurMatchStatus(MS_SIT_DOWN);
		m_wFinishTable=0;
		return true;
	}
	else if ( m_enMatchStatus == MS_SIT_DOWN )			//����ʱ������
	{
		//�����������������棬�׶�״̬���
		m_dwCurRoundUserCount = m_MatchStageInfo.Dali.dwNextRoundUserCount;
		//�׶ε���
		if (MatchAddStageAction() == false) return false; //���������
		MatchSitDownAction();
		SetCurMatchStatus(MS_MATCH_MATCHING);
		return true;
	}													
		//�������ַ��������������������Ԥ�������������߳���Ԥ���ȴ�ʱ�䣬���Է���
		if(m_wFinishTable>m_MatchStageInfo.Dali.dwFreeTable /*|| DaLi_WaitTable() == true*/ )
			
		{
			MatchStandUpAction(NULL,false,true); //������������������������
			MatchSitDownAction();
									
			//�����������λ
			m_wFinishTable=0;
		}
	return true;
}

//ASS
bool CChampionshipMatch::AssDaLi_MatchProc()
{
	//ֹͣ��̭����ȫ��������Ȼ���������ֿ������ܿ�����ӵ��
	if(m_enMatchStatus==MS_STOP_OUT)
	{
		bool bPlaying=false;
		for (WORD i=0;i<m_wTableCount;i++)
		{
			if ( m_pAssignTable[i].pTableFrame == NULL) continue;
			if(m_pAssignTable[i].pTableFrame->GetGameStatus()>=GAME_STATUS_PLAY)
				bPlaying=true;
		}
		if(bPlaying) return true;

		//����
		MatchStandUpAction(NULL,false,true);
		//����
		if (MatchPromotionAction(m_MatchStageInfo.AssDali.dwNextRoundUserCount) == false) return false;
		//���ֱ��
		//MatchScoreUpdateAction();

		SetCurMatchStatus(MS_SIT_DOWN);
		m_wFinishTable=0;
		return true;
	}
	else if ( m_enMatchStatus == MS_SIT_DOWN )			//����ʱ������
	{
		m_dwCurRoundUserCount = m_MatchStageInfo.AssDali.dwNextRoundUserCount;
		//�׶ε���
		if (MatchAddStageAction() == false) return false; //���������
		MatchSitDownAction();
		SetCurMatchStatus(MS_MATCH_MATCHING);
		return true;
	}													
	//�������ַ��������������������Ԥ�������������߳���Ԥ���ȴ�ʱ�䣬���Է���
	if(m_wFinishTable>m_MatchStageInfo.AssDali.dwFreeTable /*|| DaLi_WaitTable() == true*/ )

	{
		MatchStandUpAction(NULL,false,true);
		MatchSitDownAction(true);	 //ȡ�������������

		//�����������λ
		m_wFinishTable=0;
	}
	return true;
}

//��ʿ��λ
bool CChampionshipMatch::SwissShift_MatchProc()
{
	
	if ( m_enMatchStatus == MS_NEXT_ROUND)
	{
		MatchStandUpAction(NULL,false,true);

		//����Ƿ����һ��
		if ( m_cbCurRound >= m_MatchStageInfo.SwissShift.wRoundCount)
		{
			if (MatchAddStageAction() == false) return true;
		}
		if (MatchPromotionAction(m_dwCurRoundUserCount) == false) return false;

		//MatchScoreUpdateAction();
		SetCurMatchStatus(MS_SIT_DOWN);
		return true;
	}
	else if ( m_enMatchStatus == MS_SIT_DOWN )
	{
		MatchSitDownAction(true);
		SetCurMatchStatus(MS_MATCH_MATCHING);
		return true;
	}

	//�����������Ԥ������
	WORD wTableCount = m_dwCurRoundUserCount/m_wChairCount;
	if ( m_wFinishTable < wTableCount ) return true;

	//��ǰ����+1
	++m_cbCurCount;
	if ( m_cbCurCount != m_MatchStageInfo.SwissShift.wGameCount )
	{
		//һ�ֶ�֣������������������¼�����
		MatchStandUpAction(NULL,false,true);
		SetCurMatchStatus(MS_SIT_DOWN);
		m_wFinishTable = 0;
		return true;
	}

	m_wFinishTable=0; //������Ӹ�λ
	++m_cbCurRound;	 //��ǰ����+1
	m_cbCurCount=0;	 //��ǰ������λ
	m_dwCurRoundUserCount =  (m_dwCurRoundUserCount * m_MatchStageInfo.SwissShift.wOutPercent)/100;  //������һ������
	SetCurMatchStatus(MS_NEXT_ROUND); //���������Ѿ����У�������һ��
	
	return true;
}

//�ڷ���ʿ
bool CChampionshipMatch::SwissScore_MatchProc()
{
	
	if ( m_enMatchStatus == MS_NEXT_ROUND)
	{
		MatchStandUpAction(NULL,false,true);

		//����Ƿ����һ��
		if ( m_cbCurRound >= m_MatchStageInfo.SwissScore.wRoundCount )
		{
			if (MatchAddStageAction() == false) return true;
		}
		if (MatchGradePromotionAction(m_MatchStageInfo.SwissScore.dwNextRoundUser) == false) return false;

		//MatchScoreUpdateAction();
		SetCurMatchStatus(MS_SIT_DOWN);
		return true;
	}
	else if ( m_enMatchStatus == MS_SIT_DOWN )
	{
		MatchSitDownAction(true);
		SetCurMatchStatus(MS_MATCH_MATCHING);
		return true;
	}

	//�����������Ԥ������
	WORD wTableCount = m_dwCurRoundUserCount /m_wChairCount;
	if ( m_wFinishTable < wTableCount ) return true;

	++m_cbCurCount;
	if ( m_cbCurCount != m_MatchStageInfo.SwissScore.wGameCount )
	{
		//һ�ֶ�֣������������������¼�����
		MatchStandUpAction(NULL,false,true);
		SetCurMatchStatus(MS_SIT_DOWN);
		m_wFinishTable = 0;
		return true;
	}

	//��λ
	m_wFinishTable=0;
	++m_cbCurRound;
	m_cbCurCount=0;
	m_dwCurRoundUserCount =  (m_dwCurRoundUserCount * m_MatchStageInfo.SwissScore.wOutPercent)/100;  //������һ������
	SetCurMatchStatus(MS_NEXT_ROUND);
	
	return true;
}

//���ֻ��֣�ÿ��ֻ��һ�����ั��˵
bool CChampionshipMatch::CountScore_MatchProc()
{	
	if ( m_enMatchStatus == MS_NEXT_ROUND)
	{
		MatchStandUpAction(NULL,false,true);

// 		DWORD dwUserID=0L;
// 		IServerUserItem *pIServerUserItem=NULL;
// 		POSITION pos=m_OnMatchUserMap.GetStartPosition();
// 		while (pos!=NULL)
// 		{
// 			//��ȡ�û�
// 			m_OnMatchUserMap.GetNextAssoc(pos,dwUserID,pIServerUserItem);
// 			
// 			dwUserID;
// 			pIServerUserItem;
// 		}


		//����Ƿ����һ��
		if ( m_cbCurRound >= m_MatchStageInfo.Round.wRoundCount )
		{
			if (MatchAddStageAction() == false)return true;
		}
		//MatchScoreUpdateAction();
		SetCurMatchStatus(MS_SIT_DOWN);

		return true;
	}
	else if ( m_enMatchStatus == MS_SIT_DOWN )
	{
		DWORD dwCount = m_OnMatchUserMap.GetCount();
		MatchSitDownAction(true);
		SetCurMatchStatus(MS_MATCH_MATCHING);
		return true;
	}

	//�����������Ԥ������ ������һ�ֻ�����һ��
	WORD wTableCount = m_dwCurRoundUserCount/m_wChairCount;
	if ( m_wFinishTable < wTableCount ) return true;

	//һ�ֶ�֣������������������¼����� ,����һ�ֽ����жϣ��ڴ��ж�
 	++m_cbCurCount;
 	if ( m_cbCurCount != m_MatchStageInfo.Round.wGameCount )
 	{
 		//һ���ж�֣���ȫ��������ȫ������,Ҫ�ر�ע��
		MatchStandUpAction(NULL,false,true);
		SetCurMatchStatus(MS_SIT_DOWN);
		m_wFinishTable = 0;
		m_ReadyOutUserMap.RemoveAll();
		m_NextRoundUserMap.RemoveAll();
		return true;
 	}

 	BYTE cbCurRound = m_cbCurRound+1;
 	do 
 	{	//���һ�ֿ��Լ��㽱����
 		if ( cbCurRound >= m_MatchStageInfo.Round.wRoundCount )
 			break;
 
 		CountScore_Func();
 
 	} while (0);
	

	m_wFinishTable=0;
	++m_cbCurRound;
	m_cbCurCount=0;
	m_dwCurRoundUserCount =  (m_dwCurRoundUserCount * m_MatchStageInfo.Round.wOutPercent)/100;  //������һ������
	SetCurMatchStatus(MS_NEXT_ROUND);
	
	return true;
}

bool CChampionshipMatch::CountScore_NextRoundCheck()
{
	//�ж������������Ƿ������ӵı���

	INT_PTR nCount = m_OnMatchUserMap.GetCount();
	DWORD m = nCount%m_wChairCount; // 5%3
	DWORD n = nCount/m_wChairCount; // 5/3
	if ( m > 0 ) //��ģΪ0˵�������ӵı���
	{
		//����
		tagMatchScore *Score = new tagMatchScore[nCount];
		ZeroMemory(Score,sizeof(tagMatchScore)*nCount);
		MatchScoreSortAction(Score);
		DWORD nNextRoundUserCount=n*m_wChairCount;
		for (DWORD i=0;i<nCount;i++)
		{
			if(i>=nNextRoundUserCount)  // >3
			{
				m_OnMatchUserMap.RemoveKey(Score[i].dwUserID); 
				m_pMatchSink->WriteUserAward(Score[i].pUserItem, (WORD)i, m_dwMatchID, NULL,this);
			}

		}
		DISK_LOG(TEXT("���ֻ������������ :%d"),m_OnMatchUserMap.GetCount());
		SafeDeleteArray(Score);
	}
	return true;
}

void CChampionshipMatch::CountScore_Func()
{
	INT_PTR nCount =0;
	DWORD dwPromotion = m_OnMatchUserMap.GetCount();
	dwPromotion += m_NextRoundUserMap.GetCount();
	//////////////////////////////////////////////////////////////////////////
	//ĩλ�������,����̭
	nCount = m_ReadyOutUserMap.GetCount();
	tagMatchScore *Score = new tagMatchScore[nCount];
	ZeroMemory(Score,sizeof(tagMatchScore)*nCount);
	nCount = OutUserSortAction(Score);
	//��̭
	for ( WORD i=0;i<nCount;++i )
	{
		//m_ReadyOutUser.RemoveAt(i);
		m_pMatchSink->WriteUserAward(Score[i].pUserItem,dwPromotion+i+1,m_dwMatchID,NULL,this);  //��̭����=��������+1
	}
	SafeDeleteArray(Score);
	//////////////////////////////////////////////////////////////////////////
	//�м������������
	nCount = m_NextRoundUserMap.GetCount();
	Score = new tagMatchScore[nCount];
	ZeroMemory(Score,sizeof(tagMatchScore)*nCount);
	nCount = NextRoundUserSortAction(Score);
	//�ڶ���������/2�˽���
	WORD wTempCount = nCount/2;
	DWORD dwRank = m_OnMatchUserMap.GetCount()+wTempCount;
	for( WORD i = wTempCount;i<nCount;++i )
	{
		m_OnMatchUserMap.RemoveKey(Score[i].dwUserID);
		m_pMatchSink->WriteUserAward(Score[i].pUserItem,dwRank+i+1,m_dwMatchID,NULL,this); // ����������������
	}
	//δ��̭������¼������map
	for( WORD i =0;i<wTempCount;++i )
	{
		m_OnMatchUserMap[Score[i].dwUserID] = Score[i].pUserItem;
	}
	SafeDeleteArray(Score);
	//////////////////////////////////////////////////////////////////////////
	CountScore_NextRoundCheck();
	
	m_ReadyOutUserMap.RemoveAll();
	m_NextRoundUserMap.RemoveAll();
}

void CChampionshipMatch::SwissShift_OnEventGameEnd(ITableFrame* pTableFrame)
{
	++m_wFinishTable;
}

/*
�ڷ���ʿ��
ÿ�����������ݻ�������Ȼ��ͳ�ƴ��lGrade
*/
void CChampionshipMatch::SwissScore_OnEventGameEnd(ITableFrame* pTableFrame)
{

	WORD nCount = m_wChairCount;
	tagMatchScore* pScore = new tagMatchScore[nCount];
	ZeroMemory(pScore,sizeof(tagMatchScore)*nCount);
	for( WORD i= 0;i<nCount;++i )
	{
		pScore[i].pUserItem	= pTableFrame->GetTableUserItem(i);
		pScore[i].lScore		= pScore[i].pUserItem->GetUserScore();
		pScore[i].dwUserID	= pScore[i].pUserItem->GetUserID();
	}
	//����
	for(INT_PTR i = 1;i < nCount;i++)
	{
		INT_PTR left = 0,right = i - 1;
		tagMatchScore  Temp = pScore[i];
		while(left <= right)
		{
			INT_PTR mid = (left + right)/2;
			if(pScore[i].lScore > pScore[mid].lScore)right = mid - 1;
			else left = mid + 1;
		}
		for(INT_PTR j = i - 1;j >= left;j--)
			pScore[j+1] = pScore[j];
		pScore[left] = Temp;
	}
	//���ͳ��
	WORD wScore = m_wChairCount;
	for(WORD i=0;i<nCount;++i)
	{
		tagUserInfo *pUserScore= pScore[i].pUserItem->GetUserInfo();
		pUserScore->lGrade = wScore;
		pScore[i].lGrade = wScore;
		--wScore;
	}

	++m_wFinishTable;
	SafeDeleteArray(pScore);
}

/*
���ֻ��֣���������ĩ�����֣��м�����������������ʱ��
ÿ����������ĩλ��̭���м����μ��������̭����
�������������Ԥ���������ʱ���Ǳ�ȫ��������ȫ������
*/
void CChampionshipMatch::CountScore_OnEventGameEnd(ITableFrame *pTableFrame)
{
		++m_wFinishTable;
 	do 
 	{
 		// һ�ֶั�����
 		if ( m_cbCurCount+1 != m_MatchStageInfo.Round.wGameCount ) break;
		// ���һ��
		if ( m_cbCurRound+1 >= m_MatchStageInfo.Round.wRoundCount ) break;
 
 		WORD nCount = m_wChairCount;
 		tagMatchScore* pScore = new tagMatchScore[nCount];
 		ZeroMemory(pScore,sizeof(tagMatchScore)*nCount);
 		for( WORD i= 0;i<nCount;++i )
 		{
 			pScore[i].pUserItem	= pTableFrame->GetTableUserItem(i);
 			pScore[i].lScore		= pScore[i].pUserItem->GetUserScore();
 			pScore[i].dwUserID	= pScore[i].pUserItem->GetUserID();
 		}
 		//����
 		for(INT_PTR i = 1;i < nCount;i++)
 		{
 			INT_PTR left = 0,right = i - 1;
 			tagMatchScore  Temp = pScore[i];
 			while(left <= right)
 			{
 				INT_PTR mid = (left + right)/2;
 				if(pScore[i].lScore > pScore[mid].lScore)right = mid - 1;
 				else left = mid + 1;
 			}
 			for(INT_PTR j = i - 1;j >= left;j--)
 				pScore[j+1] = pScore[j];
 			pScore[left] = Temp;
 		}
 		--nCount;
 		//�Ƴ����һ�� ����
 		m_OnMatchUserMap.RemoveKey(pScore[nCount].dwUserID);
 		//������̭���� ����������������д����
		m_ReadyOutUserMap[pScore[nCount].dwUserID] = pScore[nCount].pUserItem;
 
 		//�ӵڶ�����ʼ
 		for ( WORD i= 1;i<nCount;++i )
		{
			m_NextRoundUserMap[pScore[i].dwUserID] = pScore[i].pUserItem;
			m_OnMatchUserMap.RemoveKey(pScore[i].dwUserID);
		}
 
 		SafeDeleteArray(pScore);
 	} while (0);
	
}

bool CChampionshipMatch::MatchStandUpAction(IServerUserItem * pIServerUserItem,bool bLog,bool bAllUser)
{
	if ( bAllUser ) //Ⱥ��
	{
		for (WORD i=0;i<m_wTableCount;i++)
		{
			ITableFrame * pITableFrame=m_pAssignTable[i].pTableFrame;
			if ( pITableFrame == NULL ) continue;
			if (pITableFrame->GetGameStatus()>=GAME_STATUS_PLAY) continue;
			for (WORD j=0;j<m_wChairCount;j++)
			{
				IServerUserItem * pIServerUserItem=pITableFrame->GetTableUserItem(j);
				if (pIServerUserItem==NULL				|| 
					pIServerUserItem->GetUserID() == 0	||
					pIServerUserItem->GetTableID()==INVALID_TABLE ) continue;
				pITableFrame->PerformStandUpAction(pIServerUserItem);
			}
		}
		return true;
	}
	if ( bLog ) //д��־
	{
		CString strFormat;
		strFormat.Format(TEXT("<----�������%s,����:%ld,����:%d"),pIServerUserItem->GetNickName(),pIServerUserItem->GetUserScore(),m_cbCurRound);
		DISK_LOG(strFormat);
		strFormat.Format(TEXT("״̬:%d,����:%d----->"),pIServerUserItem->GetUserStatus(),pIServerUserItem->GetTableID());
		DISK_LOG(strFormat);
	}
	ITableFrame* pTableFrame = m_pMatchSink->GetTableFrame(pIServerUserItem);
	if ( pTableFrame == NULL ) return false;
	return pTableFrame->PerformStandUpAction(pIServerUserItem);
}

bool CChampionshipMatch::MatchSitDownAction(bool bSort,bool bLog)
{
	if ( bSort ) //����
	{
		if ( m_bSnakeSort )//������λ��ʶ
		{
			//ǰ����֮һ��һ��λ,������֮һ������λ,������֮һ������λ
			tagMatchScore *pScore=new tagMatchScore[m_OnMatchUserMap.GetCount()];
			ZeroMemory(pScore,sizeof(tagMatchScore)*m_OnMatchUserMap.GetCount());
			WORD wCount=MatchScoreSortAction(pScore);
			WORD FirstChair=wCount/3;
			WORD SecondChair=FirstChair*2;

			//ǰ����֮һ����
			for (WORD i=0;i<FirstChair;++i)
			{
				IServerUserItem * pIServerUserItem=pScore[i].pUserItem;
				if ( pIServerUserItem == NULL || pIServerUserItem->GetUserID() == 0 ) continue;
				if (pIServerUserItem->GetUserStatus()>=US_PLAYING) continue;

				WORD wChairID=0;
				WORD wTableID=INVALID_CHAIR;
				ITableFramEx * pITableFramEx=GetSnakeEmptyChair(wTableID,wChairID);
				if (pITableFramEx==NULL) continue;
				pITableFramEx->pTableFrame->PerformSitDownAction(wChairID,pIServerUserItem);
			}
			//������֮һ��
			for (WORD i=FirstChair;i<SecondChair;++i)
			{
				IServerUserItem * pIServerUserItem=pScore[i].pUserItem;
				if ( pIServerUserItem == NULL || pIServerUserItem->GetUserID() == 0 ) continue;
				if (pIServerUserItem->GetUserStatus()>=US_PLAYING) continue;

				WORD wChairID=1;
				WORD wTableID=INVALID_CHAIR;
				ITableFramEx * pITableFramEx=GetSnakeEmptyChair(wTableID,wChairID);
				if (pITableFramEx==NULL) continue;
				pITableFramEx->pTableFrame->PerformSitDownAction(wChairID,pIServerUserItem);
			}
			//������֮һ��
			for (WORD i=SecondChair;i<wCount;++i)
			{
				IServerUserItem * pIServerUserItem=pScore[i].pUserItem;
				if ( pIServerUserItem == NULL || pIServerUserItem->GetUserID() == 0 ) continue;
				if (pIServerUserItem->GetUserStatus()>=US_PLAYING) continue;

				WORD wChairID=2;
				WORD wTableID=INVALID_CHAIR;
				ITableFramEx * pITableFramEx=GetSnakeEmptyChair(wTableID,wChairID);
				if (pITableFramEx==NULL) continue;
				pITableFramEx->pTableFrame->PerformSitDownAction(wChairID,pIServerUserItem);
			}

			return true;
		}

		INT_PTR nCount=m_OnMatchUserMap.GetCount();
		tagMatchScore *pScore=new tagMatchScore[nCount];
		ZeroMemory(pScore,sizeof(tagMatchScore)*nCount);
		WORD wCount=MatchScoreSortAction(pScore);

		for (WORD i=0;i<wCount;i++)
		{
			IServerUserItem * pIServerUserItem=pScore[i].pUserItem;
			if ( pIServerUserItem == NULL || pIServerUserItem->GetUserID() == 0 ) continue;
			if (pIServerUserItem->GetUserStatus()>=US_PLAYING) continue;

			WORD wChairID=INVALID_TABLE;
			WORD wTableID=INVALID_CHAIR;
			ITableFramEx * pITableFramEx=GetEmptyChair(wTableID,wChairID);
			if (pITableFramEx==NULL) continue;
			pITableFramEx->pTableFrame->PerformSitDownAction(wChairID,pIServerUserItem);
		}
		SafeDeleteArray(pScore);

		return true;
	}
	if (bLog){}

	WORD wTableID=INVALID_TABLE;
	WORD wChairID=INVALID_CHAIR;
	DWORD dwUserID=0L;
	IServerUserItem *pIServerUserItem=NULL;
	POSITION pos=m_OnMatchUserMap.GetStartPosition();
	while (pos!=NULL)
	{
		//��ȡ�û�
		m_OnMatchUserMap.GetNextAssoc(pos,dwUserID,pIServerUserItem);
		if ( pIServerUserItem == NULL || dwUserID == 0 ) continue;

		if ( pIServerUserItem->GetUserStatus() >= US_SIT) continue;

		ITableFramEx * pTableEx=GetEmptyChair(wTableID,wChairID);

		if ( pTableEx == NULL || pTableEx->pTableFrame == NULL ) continue;

		pTableEx->pTableFrame->PerformSitDownAction(wChairID,pIServerUserItem);
	}

	return true;
}

bool CChampionshipMatch::MatchAddStageAction()
{
	
	//�����׶� ����
	++m_cbCurStage;

	if ( m_cbCurStage >= m_MatchStageInfo.Stage.wCount) 
	{
		MatchEndAndReward();
		MatchOver();
		return false;
	}
	m_dwCurStageType = m_MatchStageInfo.Stage.wStageType[m_cbCurStage];

	//�л�����ʱ��ȷ���������ͣ��ͷ֣�����
	InitMatchBaseScore();

	IServerUserItem *pUserItem=NULL;
	DWORD dwUserID;	

	POSITION pos=m_OnMatchUserMap.GetStartPosition();
	while(pos!=NULL)
	{
		m_OnMatchUserMap.GetNextAssoc(pos, dwUserID, pUserItem);
		if(pUserItem!=NULL)
		{
			tagUserInfo * pUserScore=pUserItem->GetUserInfo();
	
			pUserScore->lScore = m_dwInitalScore;

			//m_pMatchSink->SendGroupUserScore(pUserItem,this);
		}
	}
	
	return true;
}

//��������
bool CChampionshipMatch::InitMatchType()
{
	m_cbMatchType=(BYTE)m_pMatchGlobal->Base.dwMatchType; // 1������ 2��ʱ��

	return true;
}

bool CChampionshipMatch::InitMatchUserCount()
{
	if ( m_cbMatchType == MatchType_FullUser )
	{
		m_dwMatchUserCount = m_pMatchGlobal->Solution.dwMaxJoinCount;
		m_dwCurRoundUserCount = m_pMatchGlobal->Solution.dwMaxJoinCount; //��ʿ��λ���������
	}

	return true;
}

bool CChampionshipMatch::InitMatchSolution()
{
	memset(&m_MatchStageInfo,0,sizeof(m_MatchStageInfo));
	m_cbCurStage=0;
	m_dwSolutionCount[0] = m_pMatchGlobal->Solution.dwSolutionFirstCount;
	m_dwSolutionCount[1] = m_pMatchGlobal->Solution.dwSolutionSecondCount;
	m_dwSolutionCount[2] = m_pMatchGlobal->Solution.dwSolutionThirdCount;

	m_dwSolutionID[0]	 = m_pMatchGlobal->Solution.dwSolutionFirstID;
	m_dwSolutionID[1]	 = m_pMatchGlobal->Solution.dwSolutionSecondID;
	m_dwSolutionID[2]	 = m_pMatchGlobal->Solution.dwSolutionThirdID;

	if ( m_cbMatchType == MatchType_FullUser )
	{
		m_MatchStageInfo	= m_pMatchGlobal->StageArray[0]; //��������ȡ����1
		m_dwCurStageType	= m_MatchStageInfo.Stage.wStageType[m_cbCurStage];
		m_dwCurSolutionID	= m_pMatchGlobal->Solution.dwSolutionFirstID;
		m_cbSolutionIndex	= 0;
	}
	return 0;
}

bool CChampionshipMatch::InitMatchSolutionForTime()
{
	if ( m_dwSolutionCount[0] == 0 ) 
	{
		m_MatchStageInfo = m_pMatchGlobal->StageArray[0];
	}
	else
	{
		for(BYTE i=0;i<3;++i)
		{
			if ( m_dwMatchUserCount <= m_dwSolutionCount[i] )
			{
				m_dwCurSolutionID = m_dwSolutionID[i];
				m_cbSolutionIndex = i;
				m_MatchStageInfo = m_pMatchGlobal->StageArray[i];
				break;;
			}
		}
	}

	m_dwCurStageType = m_MatchStageInfo.Stage.wStageType[m_cbCurStage];
	return true;
}

bool CChampionshipMatch::InitMatchBaseScore()
{
	DWORD dwBaseScore = 0;
	DWORD dwInitScore = 0;
	//���ݿ������ͻ�ȡ��Ϸ�ͷ֣���ʼ����
	{
		if (m_dwCurStageType==StageRule_Dali)
		{
			dwBaseScore=m_MatchStageInfo.Dali.dwinitBase;
			dwInitScore=m_MatchStageInfo.Dali.dwInitScore;
		}
		else if(m_dwCurStageType==StageRule_ASSDali)
		{
			dwBaseScore=m_MatchStageInfo.AssDali.dwinitBase;
			dwInitScore=m_MatchStageInfo.AssDali.dwInitScore;
		}
		else if (m_dwCurStageType==StageRule_CountScore)
		{
			dwBaseScore=m_MatchStageInfo.Round.dwInitBase;
			dwInitScore=m_MatchStageInfo.Round.dwInitScore;
		}
		else if (m_dwCurStageType==StageRule_SwissScore)
		{
			dwBaseScore=m_MatchStageInfo.SwissScore.dwInitBase;
			dwInitScore=m_MatchStageInfo.SwissScore.dwInitScore;
		}
		else if (m_dwCurStageType==StageRule_SwissShift)
		{
			dwBaseScore=m_MatchStageInfo.SwissShift.dwInitBase;
			dwInitScore=m_MatchStageInfo.SwissShift.dwInitScore;
		}
	}

	m_dwCurBase = dwBaseScore;
	m_dwInitalScore = dwInitScore;

	return true;
}
//��������
bool CChampionshipMatch::MatchEndAndReward()
{
	DWORD nCount = m_OnMatchUserMap.GetCount();
	IServerUserItem *pUserItem=NULL;
	tagMatchScore *pScore = new tagMatchScore[nCount];
	ZeroMemory(pScore,sizeof(tagMatchScore)*nCount);
	WORD wCount=MatchScoreSortAction(pScore);
	for (WORD i=0;i<wCount;i++)
	{
		if (pScore[i].pUserItem == nullptr) continue;
		m_OnMatchUserMap.RemoveKey(pScore[i].pUserItem->GetUserID());
		m_pMatchSink->WriteUserAward(pScore[i].pUserItem, i+1,m_dwMatchID,NULL,this);
	}

	SafeDeleteArray(pScore);
	m_OnMatchUserMap.RemoveAll();

	return true;
}