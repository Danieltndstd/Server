#include "StdAfx.h"
#include "ServerUserManager.h"

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CServerUserItem::CServerUserItem()
{
	//���ӱ���
	m_dwUserRight=0L;
	m_dwMasterRight=0L;

	//�йܱ���
	m_lTrusteeScore=0L;
	m_lRestrictScore=0L;
	m_lFrozenedScore=0L;
	m_lFillUserScore =0L;
	m_IsAutoAddScore = 0;

	//��¼��Ϣ
	m_dwDBQuestID=0L;
	m_dwLogonTime=0L;
	m_dwInoutIndex=INVALID_DWORD;

	//�ͻ�����
	m_bMobileUser=false;
	m_bAndroidUser=false;

	//����ӿ�
	m_pIServerUserItemSink=NULL;

	//ϵͳ����
	m_wBindIndex=INVALID_WORD;
	m_dwClientAddr=INADDR_NONE;
	ZeroMemory(m_szMachineID,sizeof(m_szMachineID));

	//��������
	m_bClientReady=false;
	m_bModifyScore=false;
	ZeroMemory(m_szLogonPass,sizeof(m_szLogonPass));

	//���Ա���
	ZeroMemory(&m_UserInfo,sizeof(m_UserInfo));
	ZeroMemory(&m_UserRule,sizeof(m_UserRule));
	ZeroMemory(&m_ScoreFormer,sizeof(m_ScoreFormer));
	ZeroMemory(&m_UserProperty,sizeof(m_UserProperty));
	m_UserInfo.wTableID=INVALID_TABLE;
	m_UserInfo.wChairID=INVALID_CHAIR;

	//��¼��Ϣ
	ZeroMemory(&m_RecordInfo,sizeof(m_RecordInfo));
	ZeroMemory(&m_VariationInfo,sizeof(m_VariationInfo));

	//�ֻ�����
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

//��������
CServerUserItem::~CServerUserItem()
{
}

//�ӿڲ�ѯ
VOID * CServerUserItem::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IServerUserItem,Guid,dwQueryVer);
	QUERYINTERFACE(IServerUserService,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IServerUserItem,Guid,dwQueryVer);
	return NULL;
}

//�û�ʤ��
WORD CServerUserItem::GetUserWinRate()
{
	//����ʤ��
	DWORD dwPlayCount=GetUserPlayCount();
	if (dwPlayCount!=0L) return (WORD)(m_UserInfo.dwWinCount*10000L/dwPlayCount);

	return 0;
}

//�û�����
WORD CServerUserItem::GetUserLostRate()
{
	//��������
	DWORD dwPlayCount=GetUserPlayCount();
	if (dwPlayCount!=0L) return (WORD)(m_UserInfo.dwLostCount*10000L/dwPlayCount);

	return 0;
}

//�û�����
WORD CServerUserItem::GetUserDrawRate()
{
	//�������
	DWORD dwPlayCount=GetUserPlayCount();
	if (dwPlayCount!=0L) return (WORD)(m_UserInfo.dwDrawCount*10000L/dwPlayCount);

	return 0;
}

//�û�����
WORD CServerUserItem::GetUserFleeRate()
{
	//��������
	DWORD dwPlayCount=GetUserPlayCount();
	if (dwPlayCount!=0L) return (WORD)(m_UserInfo.dwFleeCount*10000L/dwPlayCount);

	return 0;
}

//�Ա��ʺ�
bool CServerUserItem::ContrastNickName(LPCTSTR pszNickName)
{
	//Ч�����
	ASSERT_ERR(pszNickName!=NULL);
	if (pszNickName==NULL) return false;

	//���ȶԱ�
	INT nContrastLen=lstrlen(pszNickName);
	INT nSourceLen=lstrlen(m_UserInfo.szNickName);

	//�ַ��Ա�
	if (nContrastLen!=nSourceLen) return false;
	if (CompareString(LOCALE_SYSTEM_DEFAULT,NORM_IGNORECASE,pszNickName,nContrastLen,m_UserInfo.szNickName,nSourceLen)!=CSTR_EQUAL) return false;

	return true;
}

//�Ա�����
bool CServerUserItem::ContrastLogonPass(LPCTSTR pszPassword)
{
	//Ч�����
	ASSERT_ERR(pszPassword!=NULL);
	if (pszPassword==NULL) return false;

	//���ȶԱ�
	INT nTargetLen=lstrlen(pszPassword);
	INT nSourceLen=lstrlen(m_szLogonPass);

	//����Ա�
	if (nTargetLen!=nSourceLen) return false;
	if (CompareString(LOCALE_SYSTEM_DEFAULT,NORM_IGNORECASE,pszPassword,nTargetLen,m_szLogonPass,nSourceLen)!=CSTR_EQUAL) return false;

	return true;
}

//����״̬
bool CServerUserItem::SetUserStatus(BYTE cbUserStatus, WORD wTableID, WORD wChairID, BYTE cbNotifyClient)
{
	//Ч��״̬
	//ASSERT_ERR(m_UserInfo.dwUserID!=0L);
	if (m_UserInfo.dwUserID==0L) return false;

	//��¼��Ϣ
	WORD wOldTableID=m_UserInfo.wTableID;
	WORD wOldChairID=m_UserInfo.wChairID;

	//���ñ���
	m_UserInfo.wTableID=wTableID;
	m_UserInfo.wChairID=wChairID;
	m_UserInfo.cbUserStatus=cbUserStatus;
	//��������
	if (cbUserStatus == US_FREE)
		m_cbEnterTable = FALSE;

	//��¼�������
	if ( cbUserStatus == US_SIT || cbUserStatus == US_PLAYING || cbUserStatus == US_OFFLINE || cbUserStatus == US_READY){
		m_UserInfo.wLastTableID = wTableID;
		m_UserInfo.wLastChairID = wChairID;
	}

	//����״̬
	ASSERT_ERR(m_pIServerUserItemSink!=NULL);
	if (m_pIServerUserItemSink!=NULL && (cbNotifyClient == TRUE || m_bAndroidUser)) m_pIServerUserItemSink->OnEventUserItemStatus(this,wOldTableID,wOldChairID, cbNotifyClient);

	return true;
}

//д����� lGrade ��Ϊ�����뷿���� 
bool CServerUserItem::WriteUserScore(SCORE lScore, SCORE lGrade, SCORE lRevenue, DWORD dwUserMedal, BYTE cbScoreType, DWORD dwPlayTimeCount)
{
	//Ч��״̬
	ASSERT_ERR((m_UserInfo.dwUserID!=0L)&&(cbScoreType!=SCORE_TYPE_NULL));
	if ((m_UserInfo.dwUserID==0L)||(cbScoreType==SCORE_TYPE_NULL)) return false;

	//�޸�ʤ��
	switch (cbScoreType)
	{
	case SCORE_TYPE_WIN:	//ʤ��
		{
			m_UserInfo.dwWinCount++;
			m_RecordInfo.dwWinCount++;
			m_VariationInfo.dwWinCount++;
			break;
		}
	case SCORE_TYPE_LOSE:	//���
		{
			m_UserInfo.dwLostCount++;
			m_RecordInfo.dwLostCount++;
			m_VariationInfo.dwLostCount++;
			break;
		}
	case SCORE_TYPE_DRAW:	//�;�
		{
			m_UserInfo.dwDrawCount++;
			m_RecordInfo.dwDrawCount++;
			m_VariationInfo.dwDrawCount++;
			break;
		}
	case SCORE_TYPE_FLEE:	//�Ӿ�
		{
			m_UserInfo.dwFleeCount++;
			m_RecordInfo.dwFleeCount++;
			m_VariationInfo.dwFleeCount++;
			break;
		}
	}

	//���ñ�־
	m_bModifyScore=true;

	//���û���
	m_UserInfo.lScore+=lScore;
	m_UserInfo.lGrade+=/*lGrade*/0;
	m_UserInfo.dwUserMedal+=0;
	m_UserInfo.dwExperience+=0;

	m_UserCardBox.wCardCount++;

	//��¼��Ϣ
	if (m_cbBloodRoom && m_wCashInRatio )
		m_RecordInfo.lScore+=lScore/m_wCashInRatio;
	else m_RecordInfo.lScore+=lScore;
	m_RecordInfo.lGrade+=/*lGrade*/0;
	m_RecordInfo.lRevenue+=lRevenue;
	m_RecordInfo.dwUserMedal+=0;
	m_RecordInfo.dwPlayTimeCount+=0;
	m_RecordInfo.dwExperience+=0;

	//�����Ϣ
	//���ƣ���Ϸʱ�������飬����д�ֽӿ�
	m_VariationInfo.dwUserMedal+=0;
	m_VariationInfo.dwPlayTimeCount+=0;
	m_VariationInfo.dwExperience+=0;

	if (IsAndroidUser()==false) //�����˻��ֲ�д���ݿ�
	{
		if (m_cbBloodRoom && m_wCashInRatio )
			m_VariationInfo.lScore+=lScore/m_wCashInRatio; //����һ����ʺ�۷�
		else m_VariationInfo.lScore+=lScore;
		m_VariationInfo.lGrade+=/*lGrade*/0;
		m_VariationInfo.lRevenue+=lRevenue;
	}

	//�йܻ���
	do
	{
		// ���û���ֶβ���
		if (!AutoFillScore()&& m_cbBloodRoom == 0)
		{
			// �����ҷ���С����ͽ���  �Զ�����
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
	

	//����״̬
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

// ����
bool CServerUserItem::AutoFillScore()
{
	
	//I_TRACE(TEXT("%s ��ǰ %I64d ���� %I64d ����� %I64d "), m_UserInfo.szNickName, m_UserInfo.lScore, m_lFillUserScore, m_lTrusteeScore);

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

	//I_TRACE(TEXT("%s ��ǰ %I64d ���� %I64d ����� %I64d "), m_UserInfo.szNickName, m_UserInfo.lScore, m_lFillUserScore, m_lTrusteeScore);

	return true;
}

bool CServerUserItem::WriteUserMedal(LONG lUserMedal,BYTE cbScoreType)
{
	//Ч��״̬
	ASSERT_ERR((m_UserInfo.dwUserID!=0L)&&(cbScoreType!=SCORE_TYPE_NULL));
	if ((m_UserInfo.dwUserID==0L)||(cbScoreType==SCORE_TYPE_NULL)) return false;

	m_bModifyScore=true;

	m_UserInfo.dwUserMedal+=lUserMedal;
	m_RecordInfo.dwUserMedal+=lUserMedal;
	m_VariationInfo.dwUserMedal+=lUserMedal;

	//����״̬
	ASSERT_ERR(m_pIServerUserItemSink!=NULL);
	if (m_pIServerUserItemSink!=NULL) m_pIServerUserItemSink->OnEventUserItemScore(this,SCORE_REASON_MEDAL);

	return true;
}


//��ѯ��¼
bool CServerUserItem::QueryRecordInfo(tagVariationInfo & RecordInfo)
{
	//Ч��״̬
	ASSERT_ERR(m_UserInfo.dwUserID!=0L);
	if (m_UserInfo.dwUserID==0L) return false;

	//���ñ���
	RecordInfo=m_RecordInfo;

	return true;
}

//��ȡ���
bool CServerUserItem::DistillVariation(tagVariationInfo & VariationInfo)
{
	//Ч��״̬
	ASSERT_ERR(m_UserInfo.dwUserID!=0L);
	if (m_UserInfo.dwUserID==0L) return false;

	//�޸��ж�
	if (m_bModifyScore==true)
	{
		//���ñ���
		VariationInfo=m_VariationInfo;
		m_cbIsAutoVariation = false;
	}
	else
	{
		//���ñ���
		ZeroMemory(&VariationInfo,sizeof(VariationInfo));
	}

	//��ԭ����
	m_bModifyScore=false;
	ZeroMemory(&m_VariationInfo,sizeof(m_VariationInfo));

	return true;
}

//�������
bool CServerUserItem::FrozenedUserScore(SCORE lScore)
{
	//Ч��״̬
	ASSERT_ERR(m_UserInfo.dwUserID!=0L);
	if (m_UserInfo.dwUserID==0L) return false;

	if ( m_cbBloodRoom ) return true;
	if ( m_cbAutoAddScore ) return true;

	//Ч�����
	ASSERT_ERR((lScore>=0L)&&((m_UserInfo.lScore+m_lTrusteeScore)>=lScore));
	if ((lScore<0L)||((m_UserInfo.lScore+m_lTrusteeScore)<lScore)) return false;

	//���ñ���
	m_lFrozenedScore+=lScore;

	//�����û�
	if (m_lTrusteeScore<lScore)
	{
		//��������
		lScore-=m_lTrusteeScore;
		m_UserInfo.lScore-=lScore;
		m_lTrusteeScore=0L;
	}
	else
	{
		//�й�����
		m_lTrusteeScore-=lScore;
	}

	return true;
}

//�ⶳ����
bool CServerUserItem::UnFrozenedUserScore(SCORE lScore)
{
	//Ч��״̬
	ASSERT_ERR(m_UserInfo.dwUserID!=0L);
	if (m_UserInfo.dwUserID==0L) return false;

	if ( m_cbBloodRoom ) return true;
	if ( m_cbAutoAddScore ) return true;

	//Ч�����
	ASSERT_ERR((lScore>=0L)&&(m_lFrozenedScore>=lScore));
	if ((lScore<0L)||(m_lFrozenedScore<lScore)) return false;

	//���ñ���
	m_lFrozenedScore-=lScore;
	//m_lTrusteeScore+=lScore;

	//�йܻ���
	if (m_lRestrictScore>0L)
	{
		//��������
		SCORE lTotalScore=m_UserInfo.lScore+m_lTrusteeScore;

		//�йܵ���
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

// ����
bool CServerUserItem::FillUserScore(SCORE lScore, BYTE IsAutoAddScore)
{
	//Ч��״̬
	ASSERT_ERR(m_UserInfo.dwUserID != 0L);
	if (m_UserInfo.dwUserID == 0L) return false;

	//Ч�����
	//ASSERT_ERR((lScore >= 0L) && (m_lTrusteeScore >= lScore));
	//if ((lScore < 0L) || (m_lTrusteeScore < lScore)) return false;

	//I_TRACE(TEXT("���� %s ��ǰ %I64d ���� %I64d ����� %I64d "), m_UserInfo.szNickName, m_UserInfo.lScore, lScore, m_lTrusteeScore);

	m_lFillUserScore = lScore;
	m_IsAutoAddScore = IsAutoAddScore;

	return true;
}

//�޸���Ϣ
bool CServerUserItem::ModifyUserProperty(SCORE lScore,LONG lLoveLiness)
{
	//Ч��״̬
	ASSERT_ERR(m_UserInfo.dwUserID!=0L);
	if (m_UserInfo.dwUserID==0L) return false;

	if ( m_cbBloodRoom ) return true;

	//���û���
	m_UserInfo.lScore+=lScore;
	m_UserInfo.lLoveLiness+=lLoveLiness;

	//��¼��Ϣ
	m_RecordInfo.lScore+=lScore;
	m_RecordInfo.lLoveLiness+=lLoveLiness;

	//�����Ϣ
	if (lLoveLiness!=0L)
	{
		m_bModifyScore=true;
//		m_VariationInfo.lLoveLiness+=lLoveLiness;
		m_VariationInfo.lLoveLiness+=0;
	}

	//�йܻ���
	if ((m_lRestrictScore>0L)&&(lScore!=0L))
	{
		//��������
		SCORE lTotalScore=m_UserInfo.lScore+m_lTrusteeScore;

		//�йܵ���
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

	//����״̬
	ASSERT_ERR(m_pIServerUserItemSink!=NULL);
	if (m_pIServerUserItemSink!=NULL) m_pIServerUserItemSink->OnEventUserItemScore(this, SCORE_REASON_PROPERTY);

	return true;
}

//��Ҷһ�Ѫ��
bool CServerUserItem::ScoreExchangeBlood(BYTE cbType,DWORD dwBloodPrice,DWORD dwBloodPoint)
{
	return true;
}

//�����
bool CServerUserItem::DetachBindStatus()
{
	//Ч��״̬
	ASSERT_ERR(m_UserInfo.dwUserID!=0L);
	if (m_UserInfo.dwUserID==0L) return false;

	//�û�����
	m_bClientReady=false;

	//��������
	m_wBindIndex=INVALID_WORD;

	return true;
}

//���в���
bool CServerUserItem::ModifyUserInsure(SCORE lScore, SCORE lInsure, SCORE lRevenue)
{
	//ƽ��Ч��
	/*ASSERT_ERR((m_bAndroidUser==true)||(((lScore+lInsure+lRevenue)==0L)&&(lRevenue>=0L)));
	if ((m_bAndroidUser==false)&&(((lScore+lInsure+lRevenue)!=0L)||(lRevenue<0L))) return false;*/

	//Ч��״̬
	/*ASSERT_ERR((m_UserInfo.dwUserID!=0L)&&((m_UserInfo.lScore+m_lTrusteeScore+lScore)>=0L));
	if ((m_UserInfo.dwUserID==0L)||((m_UserInfo.lScore+m_lTrusteeScore+lScore)<0L)) return false;*/

	if ( m_cbBloodRoom ) return true;
	//���û���
	m_UserInfo.lScore+=lScore;
	/*
	if(m_UserInfo.lScore < 0)
		m_UserInfo.lScore = 0;
	*/
	m_UserInfo.lInsure+=lInsure;

	//��¼��Ϣ
	m_RecordInfo.lScore+=lScore;
	m_RecordInfo.lInsure+=lInsure;
	m_RecordInfo.lRevenue+=lRevenue;

	//�йܻ���
	if (m_lRestrictScore>0L)
	{
		//��������
		SCORE lTotalScore=m_UserInfo.lScore+m_lTrusteeScore;

		//�йܵ���
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

	//����״̬
	ASSERT_ERR(m_pIServerUserItemSink!=NULL);
	if (m_pIServerUserItemSink!=NULL) m_pIServerUserItemSink->OnEventUserItemScore(this, SCORE_REASON_INSURE);

	return true;
}

//���ò���
bool CServerUserItem::SetUserParameter(DWORD dwClientAddr, WORD wBindIndex, TCHAR szMachineID[LEN_MACHINE_ID], bool bAndroidUser, bool bClientReady)
{
	//Ч��״̬
	ASSERT_ERR(m_UserInfo.dwUserID!=0L);
	if (m_UserInfo.dwUserID==0L) return false;

	//�û�����
	m_bAndroidUser=bAndroidUser;
	m_bClientReady=bClientReady;

	//��������
	m_wBindIndex=wBindIndex;
	m_dwClientAddr=dwClientAddr;
	lstrcpyn(m_szMachineID,szMachineID,CountArray(m_szMachineID));

	return true;
}

//�޸�Ȩ��
VOID CServerUserItem::ModifyUserRight( DWORD dwAddRight, DWORD dwRemoveRight, bool bGameRight)
{
	//���Ȩ��
	m_dwUserRight |= dwAddRight;

	//ɾ��Ȩ��
	m_dwUserRight &= ~dwRemoveRight;

	//����״̬
	ASSERT_ERR(m_pIServerUserItemSink!=NULL);
	if (m_pIServerUserItemSink!=NULL) m_pIServerUserItemSink->OnEventUserItemRight(this,dwAddRight,dwRemoveRight,bGameRight);

	return;
}

//��������
VOID CServerUserItem::ResetUserItem()
{
	//���ӱ���
	m_dwUserRight=0L;
	m_dwMasterRight=0L;

	//�йܱ���
	m_lTrusteeScore=0L;
	m_lRestrictScore=0L;
	m_lFrozenedScore=0L;
	m_lFillUserScore=0L;
	m_IsAutoAddScore = 0;

	//��¼��Ϣ
	m_dwDBQuestID=0L;
	m_dwLogonTime=0L;
	m_dwInoutIndex=INVALID_DWORD;

	//�ͻ�����
	m_bMobileUser=false;
	m_bAndroidUser=false;

	//����ӿ�
	m_pIServerUserItemSink=NULL;

	//ϵͳ����
	m_wBindIndex=INVALID_WORD;
	m_dwClientAddr=INADDR_NONE;
	ZeroMemory(m_szMachineID,sizeof(m_szMachineID));

	//��������
	m_bClientReady=false;
	m_bModifyScore=false;
	ZeroMemory(m_szLogonPass,sizeof(m_szLogonPass));

	//���Ա���
	ZeroMemory(&m_UserInfo,sizeof(m_UserInfo));
	ZeroMemory(&m_UserRule,sizeof(m_UserRule));
	ZeroMemory(&m_ScoreFormer,sizeof(m_ScoreFormer));
	ZeroMemory(&m_UserProperty,sizeof(m_UserProperty));
	m_UserInfo.wTableID=INVALID_TABLE;
	m_UserInfo.wChairID=INVALID_CHAIR;

	//��¼��Ϣ
	ZeroMemory(&m_RecordInfo,sizeof(m_RecordInfo));
	ZeroMemory(&m_VariationInfo,sizeof(m_VariationInfo));

	//�ֻ�����
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

//���캯��
CServerUserManager::CServerUserManager()
{
	//����ӿ�
	m_pIServerUserItemSink=NULL;

	//��������
	m_UserIDMap.InitHashTable(PRIME_SERVER_USER);

	return;
}

//��������
CServerUserManager::~CServerUserManager()
{
	//�ͷ��û�
	for (INT_PTR i=0;i<m_UserItemStore.GetCount();i++) m_UserItemStore[i]->Release();
	for (INT_PTR i=0;i<m_UserItemArray.GetCount();i++) m_UserItemArray[i]->Release();

	//ɾ������
	m_UserIDMap.RemoveAll();
	m_UserItemStore.RemoveAll();
	m_UserItemArray.RemoveAll();

	return;
}

//�ӿڲ�ѯ
VOID * CServerUserManager::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IServerUserManager,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IServerUserManager,Guid,dwQueryVer);
	return NULL;
}

//���ýӿ�
bool CServerUserManager::SetServerUserItemSink(IUnknownEx * pIUnknownEx)
{
	//���ýӿ�
	if (pIUnknownEx!=NULL)
	{
		//��ѯ�ӿ�
		ASSERT_ERR(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IServerUserItemSink)!=NULL);
		m_pIServerUserItemSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IServerUserItemSink);

		//�ɹ��ж�
		if (m_pIServerUserItemSink==NULL) return false;
	}
	else m_pIServerUserItemSink=NULL;

	return true;
}

//ö���û�
IServerUserItem * CServerUserManager::EnumUserItem(WORD wEnumIndex)
{
	if (wEnumIndex>=m_UserItemArray.GetCount()) return NULL;
	return m_UserItemArray[wEnumIndex];
}

//�����û�
IServerUserItem * CServerUserManager::SearchUserItem(DWORD dwUserID)
{
	return m_UserIDMap[dwUserID];
}

//�����û�
IServerUserItem * CServerUserManager::SearchUserItem(LPCTSTR pszNickName)
{
	//��������
	CServerUserItem * pServerUserItem=NULL;

	//�����û�
	for (INT_PTR i=0;i<m_UserItemArray.GetCount();i++)
	{
		pServerUserItem=m_UserItemArray[i];
		if (pServerUserItem->ContrastNickName(pszNickName)==true) return pServerUserItem;
	}

	return NULL;
}

//ɾ���û�
bool CServerUserManager::DeleteUserItem()
{
	//�洢����
	m_UserItemStore.Append(m_UserItemArray);

	//ɾ������
	m_UserIDMap.RemoveAll();
	m_UserItemArray.RemoveAll();

	return true;
}

//ɾ���û�
bool CServerUserManager::DeleteUserItem(IServerUserItem * pIServerUserItem)
{
	//Ч�����
	//ASSERT_ERR((pIServerUserItem!=NULL)&&(pIServerUserItem->GetUserStatus()==US_NULL));
	//if ((pIServerUserItem==NULL)||(pIServerUserItem->GetUserStatus()!=US_NULL)) return false;

	//��������
	CServerUserItem * pTempUserItem=NULL;
	DWORD dwUserID=pIServerUserItem->GetUserID();

	//Ѱ�Ҷ���
	for (INT_PTR i=0;i<m_UserItemArray.GetCount();i++)
	{
		//��ȡ�û�
		pTempUserItem=m_UserItemArray[i];
		if (pIServerUserItem!=pTempUserItem) continue;

		//���ö���
		pTempUserItem->ResetUserItem();

		//ɾ������
		m_UserItemArray.RemoveAt(i);
		m_UserIDMap.RemoveKey(dwUserID);
		m_UserItemStore.Add(pTempUserItem);

		return true;
	}

	return false;
}

//�����û�
bool CServerUserManager::InsertUserItem(IServerUserItem * * pIServerUserResult, tagUserInfo & UserInfo, tagUserInfoPlus & UserInfoPlus)
{
	//��������
	CServerUserItem * pServerUserItem=NULL;

	//��ȡָ��
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

	//ԭʼ����
	pServerUserItem->m_ScoreFormer.lScore=UserInfo.lScore;
	pServerUserItem->m_ScoreFormer.dwWinCount=UserInfo.dwWinCount;
	pServerUserItem->m_ScoreFormer.dwLostCount=UserInfo.dwLostCount;
	pServerUserItem->m_ScoreFormer.dwDrawCount=UserInfo.dwDrawCount;
	pServerUserItem->m_ScoreFormer.dwFleeCount=UserInfo.dwFleeCount;
	pServerUserItem->m_ScoreFormer.dwExperience=UserInfo.dwExperience;

	//���Ա���
	CopyMemory(&pServerUserItem->m_UserInfo,&UserInfo,sizeof(UserInfo));
	ZeroMemory(&pServerUserItem->m_RecordInfo,sizeof(pServerUserItem->m_RecordInfo));
	ZeroMemory(&pServerUserItem->m_VariationInfo,sizeof(pServerUserItem->m_VariationInfo));
	ZeroMemory(&pServerUserItem->m_UserCardBox,sizeof(pServerUserItem->m_UserCardBox));

	//��¼��Ϣ
	pServerUserItem->m_dwLogonTime=UserInfoPlus.dwLogonTime;
	pServerUserItem->m_dwInoutIndex=UserInfoPlus.dwInoutIndex;

	//�û�Ȩ��
	pServerUserItem->m_dwUserRight=UserInfoPlus.dwUserRight;
	pServerUserItem->m_dwMasterRight=UserInfoPlus.dwMasterRight;
	pServerUserItem->m_lRestrictScore=UserInfoPlus.lRestrictScore;

	//������Ϣ
	pServerUserItem->m_wBindIndex=UserInfoPlus.wBindIndex;
	pServerUserItem->m_dwClientAddr=UserInfoPlus.dwClientAddr;
	lstrcpyn(pServerUserItem->m_szMachineID,UserInfoPlus.szMachineID,CountArray(pServerUserItem->m_szMachineID));

	//��������
	pServerUserItem->m_bClientReady=false;
	pServerUserItem->m_bModifyScore=false;
	pServerUserItem->m_bMobileUser=UserInfoPlus.bMobileUser;
	pServerUserItem->m_bAndroidUser=UserInfoPlus.bAndroidUser;
	pServerUserItem->m_cbEnterTable = FALSE;
	lstrcpyn(pServerUserItem->m_szLogonPass,UserInfoPlus.szPassword,CountArray(pServerUserItem->m_szLogonPass));

	//��Ѫ
	pServerUserItem->m_cbBloodRoom		= UserInfoPlus.cbBloodRoom;
	pServerUserItem->m_cbAutoAddScore	= UserInfoPlus.cbAutoAddScore;
	
	if (UserInfoPlus.cbBloodRoom == 0 && UserInfo.lScore >= UserInfoPlus.lEnterScore)
	{
		if (UserInfo.lScore < UserInfoPlus.lInitBlood) UserInfoPlus.lInitBlood = UserInfo.lScore;
		
		pServerUserItem->m_UserInfo.lScore = UserInfoPlus.lInitBlood;   //���Ѫ��
		pServerUserItem->m_lTrusteeScore = UserInfo.lScore - pServerUserItem->m_UserInfo.lScore;
		pServerUserItem->m_wCashInRatio = UserInfoPlus.lEnterScore ? UserInfoPlus.lInitBlood / UserInfoPlus.lEnterScore : 0;
		pServerUserItem->m_cbRoomType = UserInfoPlus.cbRoomType;
		pServerUserItem->m_lRestrictScore = UserInfoPlus.lMaxEnterScore;
		pServerUserItem->m_cbAutoAddScore = 0;
	}

	if ( ( UserInfoPlus.cbBloodRoom==1 || UserInfoPlus.cbBloodRoom==2 ) && UserInfo.lScore >=UserInfoPlus.lEnterScore)
	{
		pServerUserItem->m_UserInfo.lScore = UserInfoPlus.lInitBlood;   //���Ѫ��
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

	//�йܵ���
	else if ((UserInfoPlus.lRestrictScore>0L)&&(UserInfo.lScore>UserInfoPlus.lRestrictScore))
	{
		pServerUserItem->m_UserInfo.lScore=UserInfoPlus.lRestrictScore;
		pServerUserItem->m_lTrusteeScore=UserInfo.lScore-UserInfoPlus.lRestrictScore;
	}

	//�����û�
	m_UserItemArray.Add(pServerUserItem);
	m_UserIDMap[UserInfo.dwUserID]=pServerUserItem;

	//���ñ���
	*pIServerUserResult=pServerUserItem;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
