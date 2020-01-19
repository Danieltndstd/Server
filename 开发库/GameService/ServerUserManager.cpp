#include "StdAfx.h"
#include "ServerUserManager.h"

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CServerUserItem::CServerUserItem()
{
	//附加变量
	m_dwUserRight=0L;
	m_dwMasterRight=0L;

	//托管变量
	m_lTrusteeScore=0L;
	m_lRestrictScore=0L;
	m_lFrozenedScore=0L;
	m_lFillUserScore =0L;
	m_IsAutoAddScore = 0;

	//登录信息
	m_dwDBQuestID=0L;
	m_dwLogonTime=0L;
	m_dwInoutIndex=INVALID_DWORD;

	//客户类型
	m_bMobileUser=false;
	m_bAndroidUser=false;

	//组件接口
	m_pIServerUserItemSink=NULL;

	//系统属性
	m_wBindIndex=INVALID_WORD;
	m_dwClientAddr=INADDR_NONE;
	ZeroMemory(m_szMachineID,sizeof(m_szMachineID));

	//辅助变量
	m_bClientReady=false;
	m_bModifyScore=false;
	ZeroMemory(m_szLogonPass,sizeof(m_szLogonPass));

	//属性变量
	ZeroMemory(&m_UserInfo,sizeof(m_UserInfo));
	ZeroMemory(&m_UserRule,sizeof(m_UserRule));
	ZeroMemory(&m_ScoreFormer,sizeof(m_ScoreFormer));
	ZeroMemory(&m_UserProperty,sizeof(m_UserProperty));
	m_UserInfo.wTableID=INVALID_TABLE;
	m_UserInfo.wChairID=INVALID_CHAIR;

	//记录信息
	ZeroMemory(&m_RecordInfo,sizeof(m_RecordInfo));
	ZeroMemory(&m_VariationInfo,sizeof(m_VariationInfo));

	//手机定义
	m_wMobileUserRule=0;
	m_wMobileUserRule |= VIEW_MODE_PART;
	m_wMobileUserRule |= VIEW_INFO_LEVEL_1;
	m_wDeskPos=0;
	m_wDeskCount=1;

	m_dwDrawStartTime=0;
	m_wCashInRatio = 0;
	m_cbRoomType = 0;
	m_cbBloodRoom = 0;
	m_cbAutoAddScore = 0;
	m_dwSitDownTime = 0;
	m_cbIsAutoVariation=false;
	m_cbBlackUser = 0;
	m_dwHeartTime= 0;

	return;
}

//析构函数
CServerUserItem::~CServerUserItem()
{
}

//接口查询
VOID * CServerUserItem::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IServerUserItem,Guid,dwQueryVer);
	QUERYINTERFACE(IServerUserService,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IServerUserItem,Guid,dwQueryVer);
	return NULL;
}

//用户胜率
WORD CServerUserItem::GetUserWinRate()
{
	//计算胜率
	DWORD dwPlayCount=GetUserPlayCount();
	if (dwPlayCount!=0L) return (WORD)(m_UserInfo.dwWinCount*10000L/dwPlayCount);

	return 0;
}

//用户输率
WORD CServerUserItem::GetUserLostRate()
{
	//计算输率
	DWORD dwPlayCount=GetUserPlayCount();
	if (dwPlayCount!=0L) return (WORD)(m_UserInfo.dwLostCount*10000L/dwPlayCount);

	return 0;
}

//用户和率
WORD CServerUserItem::GetUserDrawRate()
{
	//计算和率
	DWORD dwPlayCount=GetUserPlayCount();
	if (dwPlayCount!=0L) return (WORD)(m_UserInfo.dwDrawCount*10000L/dwPlayCount);

	return 0;
}

//用户逃率
WORD CServerUserItem::GetUserFleeRate()
{
	//计算逃率
	DWORD dwPlayCount=GetUserPlayCount();
	if (dwPlayCount!=0L) return (WORD)(m_UserInfo.dwFleeCount*10000L/dwPlayCount);

	return 0;
}

//对比帐号
bool CServerUserItem::ContrastNickName(LPCTSTR pszNickName)
{
	//效验参数
	ASSERT_ERR(pszNickName!=NULL);
	if (pszNickName==NULL) return false;

	//长度对比
	INT nContrastLen=lstrlen(pszNickName);
	INT nSourceLen=lstrlen(m_UserInfo.szNickName);

	//字符对比
	if (nContrastLen!=nSourceLen) return false;
	if (CompareString(LOCALE_SYSTEM_DEFAULT,NORM_IGNORECASE,pszNickName,nContrastLen,m_UserInfo.szNickName,nSourceLen)!=CSTR_EQUAL) return false;

	return true;
}

//对比密码
bool CServerUserItem::ContrastLogonPass(LPCTSTR pszPassword)
{
	//效验参数
	ASSERT_ERR(pszPassword!=NULL);
	if (pszPassword==NULL) return false;

	//长度对比
	INT nTargetLen=lstrlen(pszPassword);
	INT nSourceLen=lstrlen(m_szLogonPass);

	//密码对比
	if (nTargetLen!=nSourceLen) return false;
	if (CompareString(LOCALE_SYSTEM_DEFAULT,NORM_IGNORECASE,pszPassword,nTargetLen,m_szLogonPass,nSourceLen)!=CSTR_EQUAL) return false;

	return true;
}

//设置状态
bool CServerUserItem::SetUserStatus(BYTE cbUserStatus, WORD wTableID, WORD wChairID, BYTE cbNotifyClient)
{
	//效验状态
	//ASSERT_ERR(m_UserInfo.dwUserID!=0L);
	if (m_UserInfo.dwUserID==0L) return false;

	//记录信息
	WORD wOldTableID=m_UserInfo.wTableID;
	WORD wOldChairID=m_UserInfo.wChairID;

	//设置变量
	m_UserInfo.wTableID=wTableID;
	m_UserInfo.wChairID=wChairID;
	m_UserInfo.cbUserStatus=cbUserStatus;
	//进桌重置
	if (cbUserStatus == US_FREE)
		m_cbEnterTable = FALSE;

	//记录最近桌椅
	if ( cbUserStatus == US_SIT || cbUserStatus == US_PLAYING || cbUserStatus == US_OFFLINE || cbUserStatus == US_READY){
		m_UserInfo.wLastTableID = wTableID;
		m_UserInfo.wLastChairID = wChairID;
	}

	//发送状态
	ASSERT_ERR(m_pIServerUserItemSink!=NULL);
	if (m_pIServerUserItemSink!=NULL && (cbNotifyClient == TRUE || m_bAndroidUser)) m_pIServerUserItemSink->OnEventUserItemStatus(this,wOldTableID,wOldChairID, cbNotifyClient);

	return true;
}

//写入积分 lGrade 改为传入入房限制 
bool CServerUserItem::WriteUserScore(SCORE lScore, SCORE lGrade, SCORE lRevenue, DWORD dwUserMedal, BYTE cbScoreType, DWORD dwPlayTimeCount)
{
	//效验状态
	ASSERT_ERR((m_UserInfo.dwUserID!=0L)&&(cbScoreType!=SCORE_TYPE_NULL));
	if ((m_UserInfo.dwUserID==0L)||(cbScoreType==SCORE_TYPE_NULL)) return false;

	//修改胜负
	switch (cbScoreType)
	{
	case SCORE_TYPE_WIN:	//胜局
		{
			m_UserInfo.dwWinCount++;
			m_RecordInfo.dwWinCount++;
			m_VariationInfo.dwWinCount++;
			break;
		}
	case SCORE_TYPE_LOSE:	//输局
		{
			m_UserInfo.dwLostCount++;
			m_RecordInfo.dwLostCount++;
			m_VariationInfo.dwLostCount++;
			break;
		}
	case SCORE_TYPE_DRAW:	//和局
		{
			m_UserInfo.dwDrawCount++;
			m_RecordInfo.dwDrawCount++;
			m_VariationInfo.dwDrawCount++;
			break;
		}
	case SCORE_TYPE_FLEE:	//逃局
		{
			m_UserInfo.dwFleeCount++;
			m_RecordInfo.dwFleeCount++;
			m_VariationInfo.dwFleeCount++;
			break;
		}
	}

	//设置标志
	m_bModifyScore=true;

	//设置积分
	m_UserInfo.lScore+=lScore;
	m_UserInfo.lGrade+=/*lGrade*/0;
	m_UserInfo.dwUserMedal+=0;
	m_UserInfo.dwExperience+=0;

	m_UserCardBox.wCardCount++;

	//记录信息
	if (m_cbBloodRoom && m_wCashInRatio )
		m_RecordInfo.lScore+=lScore/m_wCashInRatio;
	else m_RecordInfo.lScore+=lScore;
	m_RecordInfo.lGrade+=/*lGrade*/0;
	m_RecordInfo.lRevenue+=lRevenue;
	m_RecordInfo.dwUserMedal+=0;
	m_RecordInfo.dwPlayTimeCount+=0;
	m_RecordInfo.dwExperience+=0;

	//变更信息
	//奖牌，游戏时长，经验，不走写分接口
	m_VariationInfo.dwUserMedal+=0;
	m_VariationInfo.dwPlayTimeCount+=0;
	m_VariationInfo.dwExperience+=0;

	if (IsAndroidUser()==false) //机器人积分不写数据库
	{
		if (m_cbBloodRoom && m_wCashInRatio )
			m_VariationInfo.lScore+=lScore/m_wCashInRatio; //计算兑换比率后扣分
		else m_VariationInfo.lScore+=lScore;
		m_VariationInfo.lGrade+=/*lGrade*/0;
		m_VariationInfo.lRevenue+=lRevenue;
	}

	//托管积分
	do
	{
		// 如果没有字段补分
		if (!AutoFillScore()&& m_cbBloodRoom == 0)
		{
			// 如果玩家分数小于最低进入  自动补分
			if (m_UserInfo.lScore<lGrade)
			{
				
				SCORE lTotalScore = m_UserInfo.lScore + m_lTrusteeScore;

				if (lTotalScore > lGrade)
				{
					m_UserInfo.lScore = lGrade;
					m_lTrusteeScore = lTotalScore - lGrade;
				}
				else
				{
					m_lTrusteeScore = 0L;
					m_UserInfo.lScore = lTotalScore;
				}
				
			}
		}
		


		if (m_cbBloodRoom != 0)
		{

			AutoAddScore();
		}

	} while (0);
	

	//发送状态
	ASSERT_ERR(m_pIServerUserItemSink!=NULL);
	if (m_pIServerUserItemSink!=NULL) m_pIServerUserItemSink->OnEventUserItemScore(this,SCORE_REASON_WRITE);

	return true;
}

bool CServerUserItem::AutoAddScore()
{
	if ( !m_cbAutoAddScore ) return false;
	if ( m_cbAutoAddScore && m_UserInfo.lScore<m_lRestrictScore)
	{
		SCORE lTotalScore=m_UserInfo.lScore+m_lTrusteeScore;

		if (lTotalScore>m_lRestrictScore)
		{
			m_bModifyScore = true;
			m_cbIsAutoVariation = true;
			m_UserInfo.lScore=m_lRestrictScore;
			m_lTrusteeScore=lTotalScore-m_lRestrictScore;
		}
		else
		{
			m_lTrusteeScore=0L;
			m_UserInfo.lScore=lTotalScore;

			return false;
		}
	}
	
	return true ;
}

// 补分
bool CServerUserItem::AutoFillScore()
{
	
	//I_TRACE(TEXT("%s 当前 %I64d 补分 %I64d 冻结分 %I64d "), m_UserInfo.szNickName, m_UserInfo.lScore, m_lFillUserScore, m_lTrusteeScore);

	if (m_lFillUserScore == 0)return false;


	SCORE AllUserScore = m_UserInfo.lScore + m_lTrusteeScore;
	SCORE lFillScore = m_lFillUserScore;
	if (AllUserScore < lFillScore) lFillScore = AllUserScore;

	m_lTrusteeScore = AllUserScore - lFillScore;

	m_UserInfo.lScore = lFillScore;

	if (!m_IsAutoAddScore)
	{
		m_lFillUserScore = 0;
	}

	//I_TRACE(TEXT("%s 当前 %I64d 补分 %I64d 冻结分 %I64d "), m_UserInfo.szNickName, m_UserInfo.lScore, m_lFillUserScore, m_lTrusteeScore);

	return true;
}

bool CServerUserItem::WriteUserMedal(LONG lUserMedal,BYTE cbScoreType)
{
	//效验状态
	ASSERT_ERR((m_UserInfo.dwUserID!=0L)&&(cbScoreType!=SCORE_TYPE_NULL));
	if ((m_UserInfo.dwUserID==0L)||(cbScoreType==SCORE_TYPE_NULL)) return false;

	m_bModifyScore=true;

	m_UserInfo.dwUserMedal+=lUserMedal;
	m_RecordInfo.dwUserMedal+=lUserMedal;
	m_VariationInfo.dwUserMedal+=lUserMedal;

	//发送状态
	ASSERT_ERR(m_pIServerUserItemSink!=NULL);
	if (m_pIServerUserItemSink!=NULL) m_pIServerUserItemSink->OnEventUserItemScore(this,SCORE_REASON_MEDAL);

	return true;
}


//查询记录
bool CServerUserItem::QueryRecordInfo(tagVariationInfo & RecordInfo)
{
	//效验状态
	ASSERT_ERR(m_UserInfo.dwUserID!=0L);
	if (m_UserInfo.dwUserID==0L) return false;

	//设置变量
	RecordInfo=m_RecordInfo;

	return true;
}

//提取变更
bool CServerUserItem::DistillVariation(tagVariationInfo & VariationInfo)
{
	//效验状态
	ASSERT_ERR(m_UserInfo.dwUserID!=0L);
	if (m_UserInfo.dwUserID==0L) return false;

	//修改判断
	if (m_bModifyScore==true)
	{
		//设置变量
		VariationInfo=m_VariationInfo;
		m_cbIsAutoVariation = false;
	}
	else
	{
		//设置变量
		ZeroMemory(&VariationInfo,sizeof(VariationInfo));
	}

	//还原变量
	m_bModifyScore=false;
	ZeroMemory(&m_VariationInfo,sizeof(m_VariationInfo));

	return true;
}

//冻结积分
bool CServerUserItem::FrozenedUserScore(SCORE lScore)
{
	//效验状态
	ASSERT_ERR(m_UserInfo.dwUserID!=0L);
	if (m_UserInfo.dwUserID==0L) return false;

	if ( m_cbBloodRoom ) return true;
	if ( m_cbAutoAddScore ) return true;

	//效验积分
	ASSERT_ERR((lScore>=0L)&&((m_UserInfo.lScore+m_lTrusteeScore)>=lScore));
	if ((lScore<0L)||((m_UserInfo.lScore+m_lTrusteeScore)<lScore)) return false;

	//设置变量
	m_lFrozenedScore+=lScore;

	//设置用户
	if (m_lTrusteeScore<lScore)
	{
		//积分锁定
		lScore-=m_lTrusteeScore;
		m_UserInfo.lScore-=lScore;
		m_lTrusteeScore=0L;
	}
	else
	{
		//托管锁定
		m_lTrusteeScore-=lScore;
	}

	return true;
}

//解冻积分
bool CServerUserItem::UnFrozenedUserScore(SCORE lScore)
{
	//效验状态
	ASSERT_ERR(m_UserInfo.dwUserID!=0L);
	if (m_UserInfo.dwUserID==0L) return false;

	if ( m_cbBloodRoom ) return true;
	if ( m_cbAutoAddScore ) return true;

	//效验积分
	ASSERT_ERR((lScore>=0L)&&(m_lFrozenedScore>=lScore));
	if ((lScore<0L)||(m_lFrozenedScore<lScore)) return false;

	//设置变量
	m_lFrozenedScore-=lScore;
	//m_lTrusteeScore+=lScore;

	//托管积分
	if (m_lRestrictScore>0L)
	{
		//变量定义
		SCORE lTotalScore=m_UserInfo.lScore+m_lTrusteeScore;

		//托管调整
		if (lTotalScore>m_lRestrictScore)
		{
			m_UserInfo.lScore=m_lRestrictScore;
			m_lTrusteeScore=lTotalScore-m_lRestrictScore;
		}
		else
		{
			m_lTrusteeScore=0L;
			m_UserInfo.lScore=lTotalScore;
		}
	}

	return true;
}

// 补分
bool CServerUserItem::FillUserScore(SCORE lScore, BYTE IsAutoAddScore)
{
	//效验状态
	ASSERT_ERR(m_UserInfo.dwUserID != 0L);
	if (m_UserInfo.dwUserID == 0L) return false;

	//效验积分
	//ASSERT_ERR((lScore >= 0L) && (m_lTrusteeScore >= lScore));
	//if ((lScore < 0L) || (m_lTrusteeScore < lScore)) return false;

	//I_TRACE(TEXT("补分 %s 当前 %I64d 补分 %I64d 冻结分 %I64d "), m_UserInfo.szNickName, m_UserInfo.lScore, lScore, m_lTrusteeScore);

	m_lFillUserScore = lScore;
	m_IsAutoAddScore = IsAutoAddScore;

	return true;
}

//修改信息
bool CServerUserItem::ModifyUserProperty(SCORE lScore,LONG lLoveLiness)
{
	//效验状态
	ASSERT_ERR(m_UserInfo.dwUserID!=0L);
	if (m_UserInfo.dwUserID==0L) return false;

	if ( m_cbBloodRoom ) return true;

	//设置积分
	m_UserInfo.lScore+=lScore;
	m_UserInfo.lLoveLiness+=lLoveLiness;

	//记录信息
	m_RecordInfo.lScore+=lScore;
	m_RecordInfo.lLoveLiness+=lLoveLiness;

	//变更信息
	if (lLoveLiness!=0L)
	{
		m_bModifyScore=true;
//		m_VariationInfo.lLoveLiness+=lLoveLiness;
		m_VariationInfo.lLoveLiness+=0;
	}

	//托管积分
	if ((m_lRestrictScore>0L)&&(lScore!=0L))
	{
		//变量定义
		SCORE lTotalScore=m_UserInfo.lScore+m_lTrusteeScore;

		//托管调整
		if (lTotalScore>m_lRestrictScore)
		{
			m_UserInfo.lScore=m_lRestrictScore;
			m_lTrusteeScore=lTotalScore-m_lRestrictScore;
		}
		else
		{
			m_lTrusteeScore=0L;
			m_UserInfo.lScore=lTotalScore;
		}
	}

	//发送状态
	ASSERT_ERR(m_pIServerUserItemSink!=NULL);
	if (m_pIServerUserItemSink!=NULL) m_pIServerUserItemSink->OnEventUserItemScore(this, SCORE_REASON_PROPERTY);

	return true;
}

//金币兑换血量
bool CServerUserItem::ScoreExchangeBlood(BYTE cbType,DWORD dwBloodPrice,DWORD dwBloodPoint)
{
	return true;
}

//解除绑定
bool CServerUserItem::DetachBindStatus()
{
	//效验状态
	ASSERT_ERR(m_UserInfo.dwUserID!=0L);
	if (m_UserInfo.dwUserID==0L) return false;

	//用户属性
	m_bClientReady=false;

	//连接属性
	m_wBindIndex=INVALID_WORD;

	return true;
}

//银行操作
bool CServerUserItem::ModifyUserInsure(SCORE lScore, SCORE lInsure, SCORE lRevenue)
{
	//平衡效验
	/*ASSERT_ERR((m_bAndroidUser==true)||(((lScore+lInsure+lRevenue)==0L)&&(lRevenue>=0L)));
	if ((m_bAndroidUser==false)&&(((lScore+lInsure+lRevenue)!=0L)||(lRevenue<0L))) return false;*/

	//效验状态
	/*ASSERT_ERR((m_UserInfo.dwUserID!=0L)&&((m_UserInfo.lScore+m_lTrusteeScore+lScore)>=0L));
	if ((m_UserInfo.dwUserID==0L)||((m_UserInfo.lScore+m_lTrusteeScore+lScore)<0L)) return false;*/

	if ( m_cbBloodRoom ) return true;
	//设置积分
	m_UserInfo.lScore+=lScore;
	/*
	if(m_UserInfo.lScore < 0)
		m_UserInfo.lScore = 0;
	*/
	m_UserInfo.lInsure+=lInsure;

	//记录信息
	m_RecordInfo.lScore+=lScore;
	m_RecordInfo.lInsure+=lInsure;
	m_RecordInfo.lRevenue+=lRevenue;

	//托管积分
	if (m_lRestrictScore>0L)
	{
		//变量定义
		SCORE lTotalScore=m_UserInfo.lScore+m_lTrusteeScore;

		//托管调整
		if (lTotalScore>m_lRestrictScore)
		{
			m_UserInfo.lScore=m_lRestrictScore;
			m_lTrusteeScore=lTotalScore-m_lRestrictScore;
		}
		else
		{
			m_lTrusteeScore=0L;
			m_UserInfo.lScore=lTotalScore;
		}
	}

	//发送状态
	ASSERT_ERR(m_pIServerUserItemSink!=NULL);
	if (m_pIServerUserItemSink!=NULL) m_pIServerUserItemSink->OnEventUserItemScore(this, SCORE_REASON_INSURE);

	return true;
}

//设置参数
bool CServerUserItem::SetUserParameter(DWORD dwClientAddr, WORD wBindIndex, TCHAR szMachineID[LEN_MACHINE_ID], bool bAndroidUser, bool bClientReady)
{
	//效验状态
	ASSERT_ERR(m_UserInfo.dwUserID!=0L);
	if (m_UserInfo.dwUserID==0L) return false;

	//用户属性
	m_bAndroidUser=bAndroidUser;
	m_bClientReady=bClientReady;

	//连接属性
	m_wBindIndex=wBindIndex;
	m_dwClientAddr=dwClientAddr;
	lstrcpyn(m_szMachineID,szMachineID,CountArray(m_szMachineID));

	return true;
}

//修改权限
VOID CServerUserItem::ModifyUserRight( DWORD dwAddRight, DWORD dwRemoveRight, bool bGameRight)
{
	//添加权限
	m_dwUserRight |= dwAddRight;

	//删除权限
	m_dwUserRight &= ~dwRemoveRight;

	//发送状态
	ASSERT_ERR(m_pIServerUserItemSink!=NULL);
	if (m_pIServerUserItemSink!=NULL) m_pIServerUserItemSink->OnEventUserItemRight(this,dwAddRight,dwRemoveRight,bGameRight);

	return;
}

//重置数据
VOID CServerUserItem::ResetUserItem()
{
	//附加变量
	m_dwUserRight=0L;
	m_dwMasterRight=0L;

	//托管变量
	m_lTrusteeScore=0L;
	m_lRestrictScore=0L;
	m_lFrozenedScore=0L;
	m_lFillUserScore=0L;
	m_IsAutoAddScore = 0;

	//登录信息
	m_dwDBQuestID=0L;
	m_dwLogonTime=0L;
	m_dwInoutIndex=INVALID_DWORD;

	//客户类型
	m_bMobileUser=false;
	m_bAndroidUser=false;

	//组件接口
	m_pIServerUserItemSink=NULL;

	//系统属性
	m_wBindIndex=INVALID_WORD;
	m_dwClientAddr=INADDR_NONE;
	ZeroMemory(m_szMachineID,sizeof(m_szMachineID));

	//辅助变量
	m_bClientReady=false;
	m_bModifyScore=false;
	ZeroMemory(m_szLogonPass,sizeof(m_szLogonPass));

	//属性变量
	ZeroMemory(&m_UserInfo,sizeof(m_UserInfo));
	ZeroMemory(&m_UserRule,sizeof(m_UserRule));
	ZeroMemory(&m_ScoreFormer,sizeof(m_ScoreFormer));
	ZeroMemory(&m_UserProperty,sizeof(m_UserProperty));
	m_UserInfo.wTableID=INVALID_TABLE;
	m_UserInfo.wChairID=INVALID_CHAIR;

	//记录信息
	ZeroMemory(&m_RecordInfo,sizeof(m_RecordInfo));
	ZeroMemory(&m_VariationInfo,sizeof(m_VariationInfo));

	//手机定义
	m_wMobileUserRule=0;
	m_wMobileUserRule |= VIEW_MODE_PART;
	m_wMobileUserRule |= VIEW_INFO_LEVEL_1;
	m_wDeskPos=0;
	m_wDeskCount=1;

	m_dwDrawStartTime=0;
	m_wCashInRatio = 0;
	m_cbRoomType = 0;
	m_cbBloodRoom = 0;
	m_cbAutoAddScore = 0;
	m_dwSitDownTime = 0;
	m_cbIsAutoVariation=false;
	m_cbBlackUser = 0;
	m_dwHeartTime = 0;

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CServerUserManager::CServerUserManager()
{
	//组件接口
	m_pIServerUserItemSink=NULL;

	//设置质数
	m_UserIDMap.InitHashTable(PRIME_SERVER_USER);

	return;
}

//析构函数
CServerUserManager::~CServerUserManager()
{
	//释放用户
	for (INT_PTR i=0;i<m_UserItemStore.GetCount();i++) m_UserItemStore[i]->Release();
	for (INT_PTR i=0;i<m_UserItemArray.GetCount();i++) m_UserItemArray[i]->Release();

	//删除数据
	m_UserIDMap.RemoveAll();
	m_UserItemStore.RemoveAll();
	m_UserItemArray.RemoveAll();

	return;
}

//接口查询
VOID * CServerUserManager::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IServerUserManager,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IServerUserManager,Guid,dwQueryVer);
	return NULL;
}

//设置接口
bool CServerUserManager::SetServerUserItemSink(IUnknownEx * pIUnknownEx)
{
	//设置接口
	if (pIUnknownEx!=NULL)
	{
		//查询接口
		ASSERT_ERR(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IServerUserItemSink)!=NULL);
		m_pIServerUserItemSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IServerUserItemSink);

		//成功判断
		if (m_pIServerUserItemSink==NULL) return false;
	}
	else m_pIServerUserItemSink=NULL;

	return true;
}

//枚举用户
IServerUserItem * CServerUserManager::EnumUserItem(WORD wEnumIndex)
{
	if (wEnumIndex>=m_UserItemArray.GetCount()) return NULL;
	return m_UserItemArray[wEnumIndex];
}

//查找用户
IServerUserItem * CServerUserManager::SearchUserItem(DWORD dwUserID)
{
	return m_UserIDMap[dwUserID];
}

//查找用户
IServerUserItem * CServerUserManager::SearchUserItem(LPCTSTR pszNickName)
{
	//变量定义
	CServerUserItem * pServerUserItem=NULL;

	//搜索用户
	for (INT_PTR i=0;i<m_UserItemArray.GetCount();i++)
	{
		pServerUserItem=m_UserItemArray[i];
		if (pServerUserItem->ContrastNickName(pszNickName)==true) return pServerUserItem;
	}

	return NULL;
}

//删除用户
bool CServerUserManager::DeleteUserItem()
{
	//存储对象
	m_UserItemStore.Append(m_UserItemArray);

	//删除对象
	m_UserIDMap.RemoveAll();
	m_UserItemArray.RemoveAll();

	return true;
}

//删除用户
bool CServerUserManager::DeleteUserItem(IServerUserItem * pIServerUserItem)
{
	//效验参数
	//ASSERT_ERR((pIServerUserItem!=NULL)&&(pIServerUserItem->GetUserStatus()==US_NULL));
	//if ((pIServerUserItem==NULL)||(pIServerUserItem->GetUserStatus()!=US_NULL)) return false;

	//变量定义
	CServerUserItem * pTempUserItem=NULL;
	DWORD dwUserID=pIServerUserItem->GetUserID();

	//寻找对象
	for (INT_PTR i=0;i<m_UserItemArray.GetCount();i++)
	{
		//获取用户
		pTempUserItem=m_UserItemArray[i];
		if (pIServerUserItem!=pTempUserItem) continue;

		//重置对象
		pTempUserItem->ResetUserItem();

		//删除对象
		m_UserItemArray.RemoveAt(i);
		m_UserIDMap.RemoveKey(dwUserID);
		m_UserItemStore.Add(pTempUserItem);

		return true;
	}

	return false;
}

//插入用户
bool CServerUserManager::InsertUserItem(IServerUserItem * * pIServerUserResult, tagUserInfo & UserInfo, tagUserInfoPlus & UserInfoPlus)
{
	//变量定义
	CServerUserItem * pServerUserItem=NULL;

	//获取指针
	if (m_UserItemStore.GetCount()>0)
	{
		INT_PTR nItemPostion=m_UserItemStore.GetCount()-1;
		pServerUserItem=m_UserItemStore[nItemPostion];
		pServerUserItem->m_pIServerUserItemSink=m_pIServerUserItemSink;
		m_UserItemStore.RemoveAt(nItemPostion);
	}
	else
	{
		try
		{
			pServerUserItem=new CServerUserItem;
			pServerUserItem->m_pIServerUserItemSink=m_pIServerUserItemSink;
		}
		catch (...)
		{
			ASSERT_ERR(FALSE);
			return false;
		}
	}

	//原始变量
	pServerUserItem->m_ScoreFormer.lScore=UserInfo.lScore;
	pServerUserItem->m_ScoreFormer.dwWinCount=UserInfo.dwWinCount;
	pServerUserItem->m_ScoreFormer.dwLostCount=UserInfo.dwLostCount;
	pServerUserItem->m_ScoreFormer.dwDrawCount=UserInfo.dwDrawCount;
	pServerUserItem->m_ScoreFormer.dwFleeCount=UserInfo.dwFleeCount;
	pServerUserItem->m_ScoreFormer.dwExperience=UserInfo.dwExperience;

	//属性变量
	CopyMemory(&pServerUserItem->m_UserInfo,&UserInfo,sizeof(UserInfo));
	ZeroMemory(&pServerUserItem->m_RecordInfo,sizeof(pServerUserItem->m_RecordInfo));
	ZeroMemory(&pServerUserItem->m_VariationInfo,sizeof(pServerUserItem->m_VariationInfo));
	ZeroMemory(&pServerUserItem->m_UserCardBox,sizeof(pServerUserItem->m_UserCardBox));

	//登录信息
	pServerUserItem->m_dwLogonTime=UserInfoPlus.dwLogonTime;
	pServerUserItem->m_dwInoutIndex=UserInfoPlus.dwInoutIndex;

	//用户权限
	pServerUserItem->m_dwUserRight=UserInfoPlus.dwUserRight;
	pServerUserItem->m_dwMasterRight=UserInfoPlus.dwMasterRight;
	pServerUserItem->m_lRestrictScore=UserInfoPlus.lRestrictScore;

	//连接信息
	pServerUserItem->m_wBindIndex=UserInfoPlus.wBindIndex;
	pServerUserItem->m_dwClientAddr=UserInfoPlus.dwClientAddr;
	lstrcpyn(pServerUserItem->m_szMachineID,UserInfoPlus.szMachineID,CountArray(pServerUserItem->m_szMachineID));

	//辅助变量
	pServerUserItem->m_bClientReady=false;
	pServerUserItem->m_bModifyScore=false;
	pServerUserItem->m_bMobileUser=UserInfoPlus.bMobileUser;
	pServerUserItem->m_bAndroidUser=UserInfoPlus.bAndroidUser;
	pServerUserItem->m_cbEnterTable = FALSE;
	lstrcpyn(pServerUserItem->m_szLogonPass,UserInfoPlus.szPassword,CountArray(pServerUserItem->m_szLogonPass));

	//补血
	pServerUserItem->m_cbBloodRoom		= UserInfoPlus.cbBloodRoom;
	pServerUserItem->m_cbAutoAddScore	= UserInfoPlus.cbAutoAddScore;
	
	if (UserInfoPlus.cbBloodRoom == 0 && UserInfo.lScore >= UserInfoPlus.lEnterScore)
	{
		if (UserInfo.lScore < UserInfoPlus.lInitBlood) UserInfoPlus.lInitBlood = UserInfo.lScore;
		
		pServerUserItem->m_UserInfo.lScore = UserInfoPlus.lInitBlood;   //获得血量
		pServerUserItem->m_lTrusteeScore = UserInfo.lScore - pServerUserItem->m_UserInfo.lScore;
		pServerUserItem->m_wCashInRatio = UserInfoPlus.lEnterScore ? UserInfoPlus.lInitBlood / UserInfoPlus.lEnterScore : 0;
		pServerUserItem->m_cbRoomType = UserInfoPlus.cbRoomType;
		pServerUserItem->m_lRestrictScore = UserInfoPlus.lMaxEnterScore;
		pServerUserItem->m_cbAutoAddScore = 0;
	}

	if ( ( UserInfoPlus.cbBloodRoom==1 || UserInfoPlus.cbBloodRoom==2 ) && UserInfo.lScore >=UserInfoPlus.lEnterScore)
	{
		pServerUserItem->m_UserInfo.lScore = UserInfoPlus.lInitBlood;   //获得血量
		pServerUserItem->m_lTrusteeScore = UserInfo.lScore - UserInfoPlus.lEnterScore; 
		pServerUserItem->m_wCashInRatio = UserInfoPlus.lEnterScore?UserInfoPlus.lInitBlood / UserInfoPlus.lEnterScore:0;
		pServerUserItem->m_cbRoomType = UserInfoPlus.cbRoomType;
		pServerUserItem->m_lRestrictScore = UserInfoPlus.lInitBlood;
	}

	if ( UserInfoPlus.cbBloodRoom==3 ) 
	{
		pServerUserItem->m_UserInfo.lScore = UserInfo.lScore;   
		pServerUserItem->m_lTrusteeScore = 0; 
		pServerUserItem->m_wCashInRatio = 1;
		pServerUserItem->m_cbRoomType = UserInfoPlus.cbRoomType;
		pServerUserItem->m_lRestrictScore = 0;
		pServerUserItem->m_cbAutoAddScore=1;
		if ( pServerUserItem->m_UserInfo.dwGroupID == 100 ) 
			pServerUserItem->m_cbBlackUser = 1;


	}

	//托管调整
	else if ((UserInfoPlus.lRestrictScore>0L)&&(UserInfo.lScore>UserInfoPlus.lRestrictScore))
	{
		pServerUserItem->m_UserInfo.lScore=UserInfoPlus.lRestrictScore;
		pServerUserItem->m_lTrusteeScore=UserInfo.lScore-UserInfoPlus.lRestrictScore;
	}

	//插入用户
	m_UserItemArray.Add(pServerUserItem);
	m_UserIDMap[UserInfo.dwUserID]=pServerUserItem;

	//设置变量
	*pIServerUserResult=pServerUserItem;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
