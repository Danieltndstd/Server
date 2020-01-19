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

	InitMatchType();		//初始化比赛类型
	InitMatchSolution();	//初始化比赛方案
	InitMatchUserCount();	//初始化比赛人数

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



//接口查询
VOID* CChampionshipMatch::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameEventSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameEventSink,Guid,dwQueryVer);
	return NULL;
}

//用户报名
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
		pUserScore->lGrade=0L; //用于节分瑞士
		pUserScore->lScore=(SCORE)m_dwInitalScore;

		//发送成绩
		m_pMatchSink->SendGroupUserScore(pUserItem,this);
	}
	
	if ( pUserItem->IsAndroidUser() ) ++m_wAndroidUserCount;
	m_OnMatchUserMap[pUserItem->GetUserID()]=pUserItem;
	m_pMatchSink->AddMatchUserItem(pUserItem);

	m_pMatchSink->SendMatchStatus(pUserItem,MS_SIGNUP);

	return true;
}

//用户退赛
BOOL CChampionshipMatch::OnUserQuitMatch(IServerUserItem *pUserItem,bool bMatchNotOpen)
{
	//用户判断
	ASSERT(pUserItem!=NULL);
	if (pUserItem==NULL) return false;

	//查找用户
	IServerUserItem * pIServerUserExist=NULL;
	m_OnMatchUserMap.Lookup(pUserItem->GetUserID(),pIServerUserExist);

	//用户判断
	if (pIServerUserExist==NULL) return FALSE;

		if (pUserItem->IsAndroidUser())
			 m_wAndroidUserCount--;

		//玩家退赛，从比赛数组中删除
		m_OnMatchUserMap.RemoveKey(pUserItem->GetUserID());
		//发送退赛状态
		m_pMatchSink->SendMatchStatus(pUserItem,MS_NULL);

		//比赛没有开始 直接返回
		if ( bMatchNotOpen ) return TRUE;

		if ( MatchStandUpAction(pUserItem,true) == true ) return TRUE;


	return FALSE;
}

//分配桌子
void CChampionshipMatch::SetMatchTable(ITableFrame *pTable[], WORD wTableCount)
{
	if(pTable==NULL || wTableCount==0)return ;

	m_pAssignTable=new ITableFramEx[wTableCount];
	ZeroMemory(m_pAssignTable,sizeof(ITableFramEx)*wTableCount);
	for (WORD i=0;i<wTableCount;i++)
		m_pAssignTable[i].pTableFrame=pTable[i];
	m_wTableCount=wTableCount;
	m_bAssignTable=true;
	//分好桌设置报名状态
	SetCurMatchStatus(MS_MATCH_JOIN);
}

//获取空座位
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
	D_TRACE(TEXT("桌子都有人"));
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

//游戏开始
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
//游戏结束
bool CChampionshipMatch::OnEventGameEnd(ITableFrame *pITableFrame,WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	m_bGameEnd=true;

	//一局结束判断比赛赛制
	if ( m_dwCurStageType == StageRule_Dali ) DaLi_OnEventGameEnd(pITableFrame);
	else if (m_dwCurStageType == StageRule_ASSDali )		AssDaLi_OnEventGameEnd(pITableFrame);
	else if ( m_dwCurStageType == StageRule_CountScore	)	CountScore_OnEventGameEnd(pITableFrame);
	else if ( m_dwCurStageType == StageRule_SwissShift )	SwissShift_OnEventGameEnd(pITableFrame);
	else if ( m_dwCurStageType == StageRule_SwissScore )	 SwissScore_OnEventGameEnd(pITableFrame);

	//游戏结束发送排名
	//游戏开始，游戏结束都是桌子统计的，一桌结束只发送给这桌的玩家
	SendUserRank(pITableFrame);

	m_bGameEnd=false;
	return true;
}
//定时器
bool CChampionshipMatch::OnTimeMessage(DWORD dwTimerID, WPARAM dwBindParameter)
{
	//状态校验
	if(m_enMatchStatus==MS_NULL_STATUS) return true;
	//定时器是全局定时器，一个定时消息会走到所有比赛实例
	switch(dwTimerID)
	{
	case IDI_CHECK_MATCH_START: //人满赛开赛定时器
		{
			if ( m_cbMatchType != MatchType_FullUser ) return true;
			if ( m_bStarted ) break;

			if ( m_OnMatchUserMap.GetCount() < m_dwMatchUserCount ) return true;

			m_pMatchSink->StartMatchCheck();
			
			return true;
		}
	case  IDI_MATCH_TIME_START: //定时赛开赛定时器
		{
			if ( m_cbMatchType != MatchType_Timing ) return true;
			if ( m_bStarted ) break;
			//比赛前要确定，方案，类型，人数，低分,分配桌子
			//1.
			m_dwMatchUserCount = m_OnMatchUserMap.GetCount();
			if ( m_dwMatchUserCount < m_pMatchGlobal->Solution.dwMinJoinCount )
			{
				m_pMatchSink->SendGameMessage(NULL,TEXT("很抱歉，参赛人数比晋级人数少，比赛无法正常进行，比赛已结束，请关闭比赛"),SMT_CHAT|SMT_TABLE_ROLL,this);
				MatchOver();
				return true;
			}
			//2.根据人数多少确定采取哪套方案 3.确定类型
			InitMatchSolutionForTime();
			
			//4.确定低分 基分
			InitMatchBaseScore();
			//5.分配桌子  11/3=3
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
			//比赛开始
			m_pMatchSink->OnEventMatchStart(this);
			SetCurMatchStatus(MS_MATCH_MATCHING);
			
			return true;
		}
	case IDI_CHECK_MATCH_GROUP: //3s一次轮询
		{		
			if ( m_bStarted == false ) break;
			if ( m_bGameEnd == true ) break;
			if ( m_cbWait == false) 
				m_cbWait=true;  
			else break;
			//在这里计算比较完成桌子数，来决定是否进入下一轮
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

	//参赛人数比晋级人数少，直接结束比赛
	if(nCount< dwCount )
	{
		m_pMatchSink->SendGameMessage(NULL,TEXT("很抱歉，参赛人数比晋级人数少，比赛无法正常进行，比赛已结束，请关闭比赛"),SMT_CHAT|SMT_TABLE_ROLL,this);
		MatchOver();
		return false;
	}

	tagMatchScore *pScore=new tagMatchScore[nCount];
	ZeroMemory(pScore,sizeof(tagMatchScore)*nCount);

	//排名按分数高低排序，从高到低, Array[0]>Array[1]
	MatchScoreSortAction(pScore);
	//赛事压缩系数
	m_fRatio = m_fMeanValue/pScore[0].lScore;
	INT_PTR nNextRoundUserCount=dwCount;
	for (INT_PTR i=0;i<nCount;i++)
	{
		if(i>=nNextRoundUserCount)
		{
			m_OnMatchUserMap.RemoveKey(pScore[i].dwUserID); //淘汰玩家已经踢出参赛数组，但没有起立
			m_pMatchSink->WriteUserAward(pScore[i].pUserItem, (WORD)i+1, m_dwMatchID, NULL,this);
		}
		else m_pMatchSink->SendGameMessage(pScore[i].pUserItem, TEXT("恭喜您晋级成功，请稍候，下一轮比赛即将开始！"), SMT_CHAT|SMT_TABLE_ROLL);

	}
	SafeDeleteArray(pScore);

	return true;
}

//大分晋级
bool CChampionshipMatch::MatchGradePromotionAction(DWORD dwCount)
{
	INT_PTR nCount=m_OnMatchUserMap.GetCount();

	//参赛人数比晋级人数少，直接结束比赛
	if(nCount< dwCount )
	{
		m_pMatchSink->SendGameMessage(NULL,TEXT("很抱歉，参赛人数比晋级人数少，比赛无法正常进行，比赛已结束，请关闭比赛"),SMT_CHAT|SMT_TABLE_ROLL,this);
		MatchOver();
		return false;
	}

	tagMatchScore *pScore=new tagMatchScore[nCount];
	ZeroMemory(pScore,sizeof(tagMatchScore)*nCount);

	//大分排序
	MatchGradeSortAction(pScore);
	//赛事压缩系数
	m_fRatio = m_fMeanValue/pScore[0].lScore;
	INT_PTR nNextRoundUserCount=dwCount;
	for (INT_PTR i=0;i<nCount;i++)
	{
		if(i>=nNextRoundUserCount)
		{
			m_OnMatchUserMap.RemoveKey(pScore[i].dwUserID); //淘汰玩家已经踢出参赛数组，但没有起立
			m_pMatchSink->WriteUserAward(pScore[i].pUserItem, (WORD)i, m_dwMatchID, NULL,this);
		}
		else m_pMatchSink->SendGameMessage(pScore[i].pUserItem, TEXT("恭喜您晋级成功，请稍候，下一轮比赛即将开始！"), SMT_CHAT|SMT_TABLE_ROLL);

	}
	SafeDeleteArray(pScore);

	return true;
}

//积分排序
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
			else if ( score[i].lGrade == score[mid].lGrade && score[i].lScore > score[mid].lScore ) right = mid - 1; //大分相同是，按积分排序
			else left = mid + 1;
		}
		for(INT_PTR j = i - 1;j >= left;j--)
			score[j+1] = score[j];
		score[left] = Temp;
	}
	return nCount;
}

//大分排序


//玩家排序
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

//获取名次
WORD CChampionshipMatch::GetUserRank(IServerUserItem *pUserItem, ITableFrame *pITableFrame)
{
	//参数效验
	ASSERT(pUserItem!=NULL);
	if(pUserItem==NULL) return INVALID_WORD;

	//定义变量
	SCORE lUserScore=pUserItem->GetUserScore();
	WORD wRank=1;
	DWORD dwUserID=0;
	IServerUserItem *pLoopUserItem=NULL;

	if(pITableFrame==NULL) //统计玩家在所有桌的排名
	{
		//遍历容器
		POSITION pos=m_OnMatchUserMap.GetStartPosition();
		while(pos!=NULL)
		{
			m_OnMatchUserMap.GetNextAssoc(pos,dwUserID, pLoopUserItem);
			if (pLoopUserItem==NULL) continue;
			if(pLoopUserItem!=pUserItem && pLoopUserItem->GetUserScore()>=lUserScore)
				++wRank;
		}
		if ( m_NextRoundUserMap.IsEmpty() == FALSE)  //莫要忘记这个map
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
	else  //统计玩家在当前桌的排名
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

//用户坐下
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
				str.Format(TEXT("该玩家不在这场比赛中，昵称[%s],桌子号：%d(%d)，第:%d轮"),
					pIServerUserItem->GetNickName(),pIServerUserItem->GetTableID(),wTableID,m_cbCurRound);

				DISK_LOG((LPCTSTR)str);
			}
		}

	}
	return true;
}

//比赛结束
void CChampionshipMatch::MatchOver()
{
	for (WORD i=0; i<m_wTableCount; i++)
	{
		if(m_pAssignTable[i].pTableFrame!=NULL && m_pAssignTable[i].pTableFrame->IsGameStarted())
			m_pAssignTable[i].pTableFrame->DismissGame();
	}

	//删除用户
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

//打立出局游戏结束
void CChampionshipMatch::DaLi_OnEventGameEnd(ITableFrame *pTableFrame)
{
	DaLi_AddBaseScore();
	if ( m_enMatchStatus == MS_STOP_OUT )
	{
		m_pMatchSink->SendGameMessage(NULL,TEXT("淘汰人数已到，请等待其他桌比赛结束，即将进入复赛"),SMT_CHAT|SMT_TABLE_ROLL,this);
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

			//打立出局晋级判断
			if(nCount<=m_MatchStageInfo.Dali.dwRemainUserCount && m_enMatchStatus!=MS_STOP_OUT)
			{
				m_enMatchStatus=MS_STOP_OUT;

				TCHAR szMsg[256]=TEXT("");
				_sntprintf(szMsg,CountArray(szMsg),TEXT("已经达到淘汰人数，大约%d秒后即将结束初赛排名，确定晋级玩家！"),m_MatchStageInfo.Dali.dwWaitTime);
				m_pMatchSink->SendGameMessage(NULL,szMsg,SMT_CHAT|SMT_TABLE_ROLL,this);					
			}					
		}
		else m_pMatchSink->SendGameMessage(pUserItem,TEXT("本局已结束，请稍候，系统配桌中..."), SMT_CHAT|SMT_TABLE_ROLL);

	}
	 m_wFinishTable++;						//每完成一局都要换桌
}

void CChampionshipMatch::AssDaLi_OnEventGameEnd(ITableFrame *pTableFrame)
{
	DaLi_AddBaseScore();
	if ( m_enMatchStatus == MS_STOP_OUT )
	{
		m_pMatchSink->SendGameMessage(NULL,TEXT("淘汰人数已到，请等待其他桌比赛结束，即将进入复赛"),SMT_CHAT|SMT_TABLE_ROLL,this);
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

			//打立出局晋级判断
			if(nCount<=m_MatchStageInfo.Dali.dwRemainUserCount && m_enMatchStatus!=MS_STOP_OUT)
			{
				m_enMatchStatus=MS_STOP_OUT;

				TCHAR szMsg[256]=TEXT("");
				_sntprintf(szMsg,CountArray(szMsg),TEXT("已经达到淘汰人数，大约%d秒后即将结束初赛排名，确定晋级玩家！"),m_MatchStageInfo.AssDali.dwWaitTime);
				m_pMatchSink->SendGameMessage(NULL,szMsg,SMT_CHAT|SMT_TABLE_ROLL,this);					
			}					
		}
		else m_pMatchSink->SendGameMessage(pUserItem,TEXT("本局已结束，请稍候，系统配桌中..."), SMT_CHAT|SMT_TABLE_ROLL);
	}
	m_wFinishTable++;						//每完成一局都要换桌
}

//用户数目
WORD CChampionshipMatch::GetSitUserCount(ITableFrame *pTableFrame)
{
	//参数效验
	ASSERT(pTableFrame!=NULL);
	if(pTableFrame==NULL) return 0;

	if(pTableFrame->GetGameStatus()>=GAME_STATUS_PLAY) return m_wChairCount;

	//变量定义
	WORD wUserCount=0;

	//数目统计
	for (WORD i=0;i<pTableFrame->GetChairCount();i++)
	{
		IServerUserItem *pUserItem=pTableFrame->GetTableUserItem(i);
		if (pUserItem!=NULL && pUserItem->GetUserStatus()<US_PLAYING) wUserCount++;
	}

	return wUserCount;
}



//获取空椅
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

//发送排名到当前桌的玩家
void CChampionshipMatch::SendUserRank(ITableFrame *pTableFrame)
{
	
	DWORD dwMatchUser = GetMatchUserCount();

	CMD_GR_MatchCurProc CurProc = {0};
	CurProc.cbCurStage		 = m_cbCurStage;
	CurProc.cbCurRule		 = m_dwCurStageType;	//当前赛制									
	CurProc.dwCurBase		 = m_dwCurBase;			//当前基数
	CurProc.dwMatchUser		 = dwMatchUser;			//当前剩余人数

	//打立出局 ASS打立出局
	if ( m_dwCurStageType == StageRule_ASSDali )
	{
		CurProc.dwOutScore		 = m_MatchStageInfo.AssDali.dwLeastScore;			//最少积分
		CurProc.dwPromotionCount = m_MatchStageInfo.AssDali.dwNextRoundUserCount;	//晋级人数
		CurProc.dwRemainCount	 = m_MatchStageInfo.AssDali.dwRemainUserCount;		//截止人数
	}
	else if ( m_dwCurStageType == StageRule_Dali )
	{
		CurProc.dwOutScore		 = m_MatchStageInfo.Dali.dwLeastScore;			//最少积分
		CurProc.dwPromotionCount = m_MatchStageInfo.Dali.dwNextRoundUserCount;	//晋级人数
		CurProc.dwRemainCount	 = m_MatchStageInfo.Dali.dwRemainUserCount;		//截止人数
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
			DISK_LOG(TEXT("游戏有人掉线,桌子%d"),pTableFrame->GetTableID());
			continue;
		}
		CurProc.dwCurRank		= GetUserRank(pIServerUserItem); 
		pTableFrame->SendTableData(i, SUB_GR_MATCH_INFO, &CurProc, sizeof(CurProc), MDM_GR_MATCH);//发送排名到当前桌的玩家
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
	//增加基数 单位是秒

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
			//决赛积分清零，根据最终排名给予奖励
			if (m_MatchStageInfo.Stage.wCount == m_cbCurStage+1)
			pUserScore->lScore = 0;
			else
				pUserScore->lScore = m_fRatio*pUserScore->lScore;

			m_pMatchSink->SendGroupUserScore(pUserItem,this);
		}
	}
	
}



//晋级规则：1起立，2排名 改分 3坐下
bool CChampionshipMatch::DaLi_MatchProc()
{
	//停止淘汰，先全部起立，然后排名，分开处理，避开发包拥塞
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

		//起立
		MatchStandUpAction(NULL,false,true);
		//排名
		if (MatchPromotionAction(m_MatchStageInfo.Dali.dwNextRoundUserCount) == false) return false;
		//积分变更
		//MatchScoreUpdateAction();

		SetCurMatchStatus(MS_SIT_DOWN);
		m_wFinishTable=0;
		return true;
	}
	else if ( m_enMatchStatus == MS_SIT_DOWN )			//晋级时候坐下
	{
		//晋级把下轮人数保存，阶段状态变更
		m_dwCurRoundUserCount = m_MatchStageInfo.Dali.dwNextRoundUserCount;
		//阶段递增
		if (MatchAddStageAction() == false) return false; //必须放这里
		MatchSitDownAction();
		SetCurMatchStatus(MS_MATCH_MATCHING);
		return true;
	}													
		//打立出局分桌规则：完成桌子数大于预定桌子数，或者超过预定等待时间，可以分桌
		if(m_wFinishTable>m_MatchStageInfo.Dali.dwFreeTable /*|| DaLi_WaitTable() == true*/ )
			
		{
			MatchStandUpAction(NULL,false,true); //先起立立马坐下这样换桌快
			MatchSitDownAction();
									
			//完成桌子数复位
			m_wFinishTable=0;
		}
	return true;
}

//ASS
bool CChampionshipMatch::AssDaLi_MatchProc()
{
	//停止淘汰，先全部起立，然后排名，分开处理，避开发包拥塞
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

		//起立
		MatchStandUpAction(NULL,false,true);
		//排名
		if (MatchPromotionAction(m_MatchStageInfo.AssDali.dwNextRoundUserCount) == false) return false;
		//积分变更
		//MatchScoreUpdateAction();

		SetCurMatchStatus(MS_SIT_DOWN);
		m_wFinishTable=0;
		return true;
	}
	else if ( m_enMatchStatus == MS_SIT_DOWN )			//晋级时候坐下
	{
		m_dwCurRoundUserCount = m_MatchStageInfo.AssDali.dwNextRoundUserCount;
		//阶段递增
		if (MatchAddStageAction() == false) return false; //必须放这里
		MatchSitDownAction();
		SetCurMatchStatus(MS_MATCH_MATCHING);
		return true;
	}													
	//打立出局分桌规则：完成桌子数大于预定桌子数，或者超过预定等待时间，可以分桌
	if(m_wFinishTable>m_MatchStageInfo.AssDali.dwFreeTable /*|| DaLi_WaitTable() == true*/ )

	{
		MatchStandUpAction(NULL,false,true);
		MatchSitDownAction(true);	 //取积分相近的组桌

		//完成桌子数复位
		m_wFinishTable=0;
	}
	return true;
}

//瑞士移位
bool CChampionshipMatch::SwissShift_MatchProc()
{
	
	if ( m_enMatchStatus == MS_NEXT_ROUND)
	{
		MatchStandUpAction(NULL,false,true);

		//检测是否最后一轮
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

	//完成桌数等于预定桌数
	WORD wTableCount = m_dwCurRoundUserCount/m_wChairCount;
	if ( m_wFinishTable < wTableCount ) return true;

	//当前局数+1
	++m_cbCurCount;
	if ( m_cbCurCount != m_MatchStageInfo.SwissShift.wGameCount )
	{
		//一轮多局，不排序，先起立再坐下继续打
		MatchStandUpAction(NULL,false,true);
		SetCurMatchStatus(MS_SIT_DOWN);
		m_wFinishTable = 0;
		return true;
	}

	m_wFinishTable=0; //完成桌子复位
	++m_cbCurRound;	 //当前轮数+1
	m_cbCurCount=0;	 //当前局数复位
	m_dwCurRoundUserCount =  (m_dwCurRoundUserCount * m_MatchStageInfo.SwissShift.wOutPercent)/100;  //计算下一轮人数
	SetCurMatchStatus(MS_NEXT_ROUND); //所有桌子已经空闲，进入下一轮
	
	return true;
}

//节分瑞士
bool CChampionshipMatch::SwissScore_MatchProc()
{
	
	if ( m_enMatchStatus == MS_NEXT_ROUND)
	{
		MatchStandUpAction(NULL,false,true);

		//检测是否最后一轮
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

	//完成桌数等于预定桌数
	WORD wTableCount = m_dwCurRoundUserCount /m_wChairCount;
	if ( m_wFinishTable < wTableCount ) return true;

	++m_cbCurCount;
	if ( m_cbCurCount != m_MatchStageInfo.SwissScore.wGameCount )
	{
		//一轮多局，不排序，先起立再坐下继续打
		MatchStandUpAction(NULL,false,true);
		SetCurMatchStatus(MS_SIT_DOWN);
		m_wFinishTable = 0;
		return true;
	}

	//复位
	m_wFinishTable=0;
	++m_cbCurRound;
	m_cbCurCount=0;
	m_dwCurRoundUserCount =  (m_dwCurRoundUserCount * m_MatchStageInfo.SwissScore.wOutPercent)/100;  //计算下一轮人数
	SetCurMatchStatus(MS_NEXT_ROUND);
	
	return true;
}

//定局积分，每轮只有一副，多副再说
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
// 			//获取用户
// 			m_OnMatchUserMap.GetNextAssoc(pos,dwUserID,pIServerUserItem);
// 			
// 			dwUserID;
// 			pIServerUserItem;
// 		}


		//检测是否最后一轮
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

	//完成桌数等于预定桌数 进入下一轮或者下一局
	WORD wTableCount = m_dwCurRoundUserCount/m_wChairCount;
	if ( m_wFinishTable < wTableCount ) return true;

	//一轮多局，不排序，先起立再坐下继续打 ,不在一局结束判断，在此判断
 	++m_cbCurCount;
 	if ( m_cbCurCount != m_MatchStageInfo.Round.wGameCount )
 	{
 		//一轮有多局，则全部起立，全部坐下,要特别注意
		MatchStandUpAction(NULL,false,true);
		SetCurMatchStatus(MS_SIT_DOWN);
		m_wFinishTable = 0;
		m_ReadyOutUserMap.RemoveAll();
		m_NextRoundUserMap.RemoveAll();
		return true;
 	}

 	BYTE cbCurRound = m_cbCurRound+1;
 	do 
 	{	//最后一轮可以计算奖励了
 		if ( cbCurRound >= m_MatchStageInfo.Round.wRoundCount )
 			break;
 
 		CountScore_Func();
 
 	} while (0);
	

	m_wFinishTable=0;
	++m_cbCurRound;
	m_cbCurCount=0;
	m_dwCurRoundUserCount =  (m_dwCurRoundUserCount * m_MatchStageInfo.Round.wOutPercent)/100;  //计算下一轮人数
	SetCurMatchStatus(MS_NEXT_ROUND);
	
	return true;
}

bool CChampionshipMatch::CountScore_NextRoundCheck()
{
	//判断最后玩家人数是否是桌子的倍数

	INT_PTR nCount = m_OnMatchUserMap.GetCount();
	DWORD m = nCount%m_wChairCount; // 5%3
	DWORD n = nCount/m_wChairCount; // 5/3
	if ( m > 0 ) //相模为0说明是桌子的倍数
	{
		//排序
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
		DISK_LOG(TEXT("定局积分人数排序后 :%d"),m_OnMatchUserMap.GetCount());
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
	//末位玩家排名,并淘汰
	nCount = m_ReadyOutUserMap.GetCount();
	tagMatchScore *Score = new tagMatchScore[nCount];
	ZeroMemory(Score,sizeof(tagMatchScore)*nCount);
	nCount = OutUserSortAction(Score);
	//淘汰
	for ( WORD i=0;i<nCount;++i )
	{
		//m_ReadyOutUser.RemoveAt(i);
		m_pMatchSink->WriteUserAward(Score[i].pUserItem,dwPromotion+i+1,m_dwMatchID,NULL,this);  //淘汰排名=晋级人数+1
	}
	SafeDeleteArray(Score);
	//////////////////////////////////////////////////////////////////////////
	//中间名次玩家排名
	nCount = m_NextRoundUserMap.GetCount();
	Score = new tagMatchScore[nCount];
	ZeroMemory(Score,sizeof(tagMatchScore)*nCount);
	nCount = NextRoundUserSortAction(Score);
	//第二名总人数/2人晋级
	WORD wTempCount = nCount/2;
	DWORD dwRank = m_OnMatchUserMap.GetCount()+wTempCount;
	for( WORD i = wTempCount;i<nCount;++i )
	{
		m_OnMatchUserMap.RemoveKey(Score[i].dwUserID);
		m_pMatchSink->WriteUserAward(Score[i].pUserItem,dwRank+i+1,m_dwMatchID,NULL,this); // 根据人数决定排名
	}
	//未淘汰玩家重新加入参赛map
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
节分瑞士：
每桌结束，根据积分排序，然后统计大分lGrade
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
	//排序
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
	//大分统计
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
定局积分：先排序，最末名出局，中间名次排序，其他处理定时器
每桌结束，最末位淘汰，中间名次加入可能淘汰数组
如果局数不等于预设局数，定时器那边全部起立，全部坐下
*/
void CChampionshipMatch::CountScore_OnEventGameEnd(ITableFrame *pTableFrame)
{
		++m_wFinishTable;
 	do 
 	{
 		// 一轮多副的情况
 		if ( m_cbCurCount+1 != m_MatchStageInfo.Round.wGameCount ) break;
		// 最后一轮
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
 		//排序
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
 		//移除最后一名 出局
 		m_OnMatchUserMap.RemoveKey(pScore[nCount].dwUserID);
 		//加入淘汰数组 所有桌结束后，排名写奖励
		m_ReadyOutUserMap[pScore[nCount].dwUserID] = pScore[nCount].pUserItem;
 
 		//从第二名开始
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
	if ( bAllUser ) //群发
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
	if ( bLog ) //写日志
	{
		CString strFormat;
		strFormat.Format(TEXT("<----玩家退赛%s,分数:%ld,轮数:%d"),pIServerUserItem->GetNickName(),pIServerUserItem->GetUserScore(),m_cbCurRound);
		DISK_LOG(strFormat);
		strFormat.Format(TEXT("状态:%d,桌号:%d----->"),pIServerUserItem->GetUserStatus(),pIServerUserItem->GetTableID());
		DISK_LOG(strFormat);
	}
	ITableFrame* pTableFrame = m_pMatchSink->GetTableFrame(pIServerUserItem);
	if ( pTableFrame == NULL ) return false;
	return pTableFrame->PerformStandUpAction(pIServerUserItem);
}

bool CChampionshipMatch::MatchSitDownAction(bool bSort,bool bLog)
{
	if ( bSort ) //排序
	{
		if ( m_bSnakeSort )//蛇形排位标识
		{
			//前三分之一坐一号位,中三分之一坐二号位,后三分之一坐三号位
			tagMatchScore *pScore=new tagMatchScore[m_OnMatchUserMap.GetCount()];
			ZeroMemory(pScore,sizeof(tagMatchScore)*m_OnMatchUserMap.GetCount());
			WORD wCount=MatchScoreSortAction(pScore);
			WORD FirstChair=wCount/3;
			WORD SecondChair=FirstChair*2;

			//前三分之一坐下
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
			//中三分之一坐
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
			//后三分之一坐
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
		//获取用户
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
	
	//比赛阶段 递增
	++m_cbCurStage;

	if ( m_cbCurStage >= m_MatchStageInfo.Stage.wCount) 
	{
		MatchEndAndReward();
		MatchOver();
		return false;
	}
	m_dwCurStageType = m_MatchStageInfo.Stage.wStageType[m_cbCurStage];

	//切换赛制时，确定赛制类型，低分，基分
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

//比赛类型
bool CChampionshipMatch::InitMatchType()
{
	m_cbMatchType=(BYTE)m_pMatchGlobal->Base.dwMatchType; // 1人满赛 2定时赛

	return true;
}

bool CChampionshipMatch::InitMatchUserCount()
{
	if ( m_cbMatchType == MatchType_FullUser )
	{
		m_dwMatchUserCount = m_pMatchGlobal->Solution.dwMaxJoinCount;
		m_dwCurRoundUserCount = m_pMatchGlobal->Solution.dwMaxJoinCount; //瑞士移位最初轮人数
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
		m_MatchStageInfo	= m_pMatchGlobal->StageArray[0]; //人满赛采取方案1
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
	//根据开赛类型获取游戏低分，初始积分
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
//比赛结束
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