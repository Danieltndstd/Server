#include "StdAfx.h"
#include "AndroidUserManager.h"
#include "../../Server/GameServer/DataBasePacket.h"

//////////////////////////////////////////////////////////////////////////////////

//��������
#define ADNDROID_PLAY_DRAW          10                                  //��Ϸ����  

//ʱ���ʶ
#define IDI_ANDROID_INOUT			(IDI_REBOT_MODULE_START+0)			//����ʱ��
#define IDI_ANDROID_PULSE			(IDI_REBOT_MODULE_START+1)			//����ʱ��

#define IDI_ANDROID_CHECK			(IDI_REBOT_MODULE_START+3)			//��ⷿ��

//����ʱ��
#define TIME_ANDROID_INOUT			5L									//��¼ʱ��
#define TIME_ANDROID_PULSE			1L									//����ʱ��
#define TIME_LOAD_ANDROID_INFO		900L								//�����û�
#define TIME_ANDROID_REPOSE_TIME	1800L								//����ʱ��
#define TIME_ANDROID_CHECK			20L									//����ʱ��


//////////////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserManager::CAndroidUserManager()
{
	//ϵͳ����
	m_pGameParameter=NULL;
	m_pGameServiceAttrib=NULL;
	m_pGameServiceOption=NULL;

	//����ӿ�
	m_pITimerEngine=NULL;
	m_pIServerUserManager=NULL;
	m_pIGameServiceManager=NULL;
	m_pITCPNetworkEngineEvent=NULL;

	//���ñ���
	m_wAutoAndroidCount=0;

	//������
	m_wStockCount=0;
	ZeroMemory(&m_AndroidParameter,sizeof(m_AndroidParameter));

	m_ppITableFrame = NULL;

	//�������� ����:������120%�ʺ�
	m_AndroidUserItemMap.InitHashTable(PRIME_ANDROID_USER);

	//m_bQuickTimer	= false;
	//m_dwRepeat		= 0;
	//m_dwcrtTimes	= 0;
	return;
}

//��������
CAndroidUserManager::~CAndroidUserManager()
{
	//�������
	ASSERT_ERR(m_AndroidUserItemMap.GetCount()==0L);
	ASSERT_ERR(m_AndroidUserItemArray.GetCount()==0L);
	ASSERT_ERR(m_AndroidUserItemBuffer.GetCount()==0L);

	return;
}

//�ӿڲ�ѯ
VOID * CAndroidUserManager::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserManager,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserManager,Guid,dwQueryVer);
	return NULL;
}

//��������
bool CAndroidUserManager::StartService()
{
	m_wAutoAndroidCount = m_pGameServiceOption->wAndroidCount;

	//�����˴�����ʱ��,������ʽ����
	if (m_pGameServiceOption->wServerType != GAME_GENRE_MATCH)
		m_pITimerEngine->SetTimer(IDI_ANDROID_INOUT,TIME_ANDROID_INOUT*1000L,TIMES_INFINITY,0);
	
	//���������嶨ʱ��	
	m_pITimerEngine->SetTimer(IDI_ANDROID_PULSE,TIME_ANDROID_PULSE*1000L,TIMES_INFINITY,0);

	//�����˼�ⷿ��(�ڻ�׼�ߵĲ���) ��ͨ����
	//if (m_pGameServiceOption->wServerType != GAME_GENRE_MATCH)
	//	m_pITimerEngine->SetTimer(IDI_ANDROID_CHECK, TIME_ANDROID_CHECK * 1000l, TIMES_INFINITY, NULL);
	
	return true;
}


//ֹͣ����
bool CAndroidUserManager::ConcludeService()
{
	//ɾ���洢
	for (INT_PTR i=0;i<m_AndroidUserItemArray.GetCount();i++)
	{
		SafeRelease(m_AndroidUserItemArray[i]);
	}

	SafeDeleteArray(m_ppITableFrame);

	//��������
	m_AndroidUserItemMap.RemoveAll();
	m_AndroidUserItemArray.RemoveAll();
	m_AndroidUserItemBuffer.RemoveAll();

	//����û�
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

//�������
bool CAndroidUserManager::InitAndroidUser(tagAndroidUserParameter & AndroidUserParameter)
{
	//���ñ���
	m_pGameParameter=AndroidUserParameter.pGameParameter;
	m_pGameServiceAttrib=AndroidUserParameter.pGameServiceAttrib;
	m_pGameServiceOption=AndroidUserParameter.pGameServiceOption;

	//����ӿ�
	m_pITimerEngine=AndroidUserParameter.pITimerEngine;
	m_pIServerUserManager=AndroidUserParameter.pIServerUserManager;
	m_pIGameServiceManager=AndroidUserParameter.pIGameServiceManager;
	m_pITCPNetworkEngineEvent=AndroidUserParameter.pITCPNetworkEngineEvent;
	m_pIDataBaseEngine=AndroidUserParameter.pIDabaBaseEngine;

	CopyMemory(m_guiRoom, AndroidUserParameter.guiRoom, 40 * sizeof WCHAR);

	return true;
}

//���ÿ��
bool CAndroidUserManager::SetAndroidStock(tagAndroidParameter AndroidParameter[], WORD wStockCount)
{
	//���ñ���
	//m_wStockCount=0;
	//ZeroMemory(m_AndroidParameter,sizeof(m_AndroidParameter));

	//�����û�
	for (WORD i=0;i<wStockCount;i++)
	{
		//��ĿЧ��
		//ASSERT_ERR(m_wStockCount<CountArray(m_AndroidParameter));
		if (m_wStockCount>=CountArray(m_AndroidParameter)) break;

		//���ñ���
		CAndroidUserItem * pAndroidUserItem=NULL;
		m_AndroidUserItemMap.Lookup(AndroidParameter[i].dwUserID,pAndroidUserItem);

		//���ñ���
		if (pAndroidUserItem==NULL)
		{
			m_AndroidParameter[m_wStockCount++]=AndroidParameter[i];
		}
	}

	return true;
}

//ɾ������
bool CAndroidUserManager::DeleteAndroidUserItem(DWORD dwAndroidID)

{
	//��ȡ����
	WORD wIndex=LOWORD(dwAndroidID);
	CAndroidUserItem * pAndroidUserItem=GetAndroidUserItem(wIndex);


	//����Ч��
	//ASSERT_ERR((pAndroidUserItem!=NULL)&&(pAndroidUserItem->m_wRoundID==HIWORD(dwAndroidID)));
	if ((pAndroidUserItem==NULL)||(pAndroidUserItem->m_wRoundID!=HIWORD(dwAndroidID))) return false;

	//�ر��¼�
	try
	{
		
		m_pITCPNetworkEngineEvent->OnEventTCPNetworkShut(dwAndroidID,0,0L);
	}
	catch (...)
	{
		//�������
		ASSERT_ERR(FALSE);
	}

	//ɾ������
	FreeAndroidUserItem(pAndroidUserItem->GetUserID());

	return true;
}

//���һ���
IAndroidUserItem * CAndroidUserManager::SearchAndroidUserItem(DWORD dwUserID, DWORD dwContextID)
{
	//���һ���
	CAndroidUserItem * pAndroidUserItem=m_AndroidUserItemMap[dwUserID];

	//�����ж�
	if (pAndroidUserItem!=NULL)
	{
		WORD wRoundID=pAndroidUserItem->m_wRoundID;
		WORD wAndroidIndex=pAndroidUserItem->m_wAndroidIndex;
		if (MAKELONG(wAndroidIndex,wRoundID)==dwContextID) return pAndroidUserItem;
	}

	return NULL;
}

//����������
void CAndroidUserManager::CreateAndroidUserItem(DWORD dwCount)
{
	for (DWORD i = 0; i < dwCount; ++i)
	{
		OnEventTimerPulse(IDI_ANDROID_INOUT, NULL);
	}
}

//��������
IAndroidUserItem * CAndroidUserManager::CreateAndroidUserItem(tagAndroidParameter & AndroidParameter)
{
	//Ч�����
	ASSERT_ERR(AndroidParameter.dwUserID!=0L);
	if (AndroidParameter.dwUserID==0L) return NULL;

	//��������
	CAndroidUserItem * pAndroidUserItem=ActiveAndroidUserItem(AndroidParameter);
	if (pAndroidUserItem==NULL) return NULL;

	//���Ա���
	WORD wRoundID=pAndroidUserItem->m_wRoundID;
	WORD wAndroidIndex=pAndroidUserItem->m_wAndroidIndex;

	//����ģ��
	try
	{
		if (m_pITCPNetworkEngineEvent->OnEventTCPNetworkBind(MAKELONG(wAndroidIndex,wRoundID),0L)==false)
		{
			throw 0;
		}
	}
	catch (...)
	{
		//�������
		ASSERT_ERR(FALSE);

		//�ͷ��û�
		FreeAndroidUserItem(AndroidParameter.dwUserID);

		return NULL;
	}

	//��������
	CMD_GR_LogonUserID LogonUserID;
	ZeroMemory(&LogonUserID,sizeof(LogonUserID));

	//�汾��Ϣ
	LogonUserID.dwPlazaVersion=VERSION_PLAZA;
	LogonUserID.dwFrameVersion=VERSION_FRAME;
	LogonUserID.dwProcessVersion=m_pGameServiceAttrib->dwClientVersion;

	//�û���Ϣ
	LogonUserID.dwUserID=AndroidParameter.dwUserID;
	lstrcpyn(LogonUserID.szPassword,TEXT(""),CountArray(LogonUserID.szPassword));
	LogonUserID.wKindID=m_pGameServiceOption->wKindID;

	//��������
	TCP_Command Command;
	Command.wMainCmdID=MDM_GR_LOGON;
	Command.wSubCmdID=SUB_GR_LOGON_USERID;

	//��Ϣ����
	try
	{
		//��������
		if (m_pITCPNetworkEngineEvent->OnEventTCPNetworkRead(MAKELONG(wAndroidIndex,wRoundID),Command,&LogonUserID,sizeof(LogonUserID))==false)
		{
			//throw 0;
		}
	}
	catch (...) 
	{
		//�������
		ASSERT_ERR(FALSE);
		DISK_LOG(TEXT("��Ϸ���� �ͷŻ����� 2"));
		//ɾ������
		DeleteAndroidUserItem(MAKELONG(wAndroidIndex,wRoundID));

		return NULL;
	}

	return pAndroidUserItem;
}

//�����¼�
bool CAndroidUserManager::OnEventTimerPulse(DWORD dwTimerID, WPARAM dwBindParameter)
{
	switch (dwTimerID)
	{
	case IDI_ANDROID_INOUT:			//��������
		{
			//��������
			DWORD dwUserID=0L;
			INT_PTR nActiveCount=m_AndroidUserItemMap.GetCount();
			POSITION Position=m_AndroidUserItemMap.GetStartPosition();

			//��ȡʱ��
			SYSTEMTIME SystemTime;
			GetLocalTime(&SystemTime);
			DWORD dwTimeMask=(1L<<SystemTime.wHour);

			//��������
			bool bAllowAndroidAttend=CServerRule::IsAllowAndroidAttend(m_pGameServiceOption->dwServerRule);
			bool bAllowAndroidSimulate=CServerRule::IsAllowAndroidSimulate(m_pGameServiceOption->dwServerRule);

			//��¼����
			if ((bAllowAndroidAttend==true)||(bAllowAndroidSimulate==true))
			{
				//Ѱ�һ���
				for (WORD i=0;i<m_wStockCount;i++)
				{
					if ((m_AndroidParameter[i].dwServiceTime&dwTimeMask)!=0L)
					{
						//��������
						if (CreateAndroidUserItem(m_AndroidParameter[i])!=NULL)
						{
							//ɾ�����
							m_wStockCount--;
							m_AndroidParameter[i]=m_AndroidParameter[m_wStockCount];

							break;
						}
					}
				}

			}

			//��������
			DWORD dwCurrentTime=(DWORD)time(NULL);
			DWORD dwServerRule=m_pGameServiceOption->dwServerRule;

			//�˳�����
			while (Position!=NULL)
			{
				//��ȡ����
				CAndroidUserItem * pAndroidUserItem=NULL;
				m_AndroidUserItemMap.GetNextAssoc(Position,dwUserID,pAndroidUserItem);

				//�˳��ж�
				if (pAndroidUserItem!=NULL && pAndroidUserItem->m_pIServerUserItem!=NULL)
				{
					//��������
					IServerUserItem * pIServerUserItem=pAndroidUserItem->m_pIServerUserItem;
					tagAndroidParameter * pAndroidParameter=pAndroidUserItem->GetAndroidParameter();

					//����״̬
					BYTE cbUserStatus=pIServerUserItem->GetUserStatus();
					if ((cbUserStatus!=US_FREE)&&(cbUserStatus!=US_SIT)) continue;

					//����ʱ��
					DWORD dwLogonTime=pIServerUserItem->GetLogonTime();
					DWORD dwReposeTime=pAndroidUserItem->GetAndroidService()->dwReposeTime;

					//�뿪�ж�
					bool bLeaveServer=false;
					if ((dwLogonTime+dwReposeTime)<dwCurrentTime) bLeaveServer=true;
					if ((pAndroidParameter->dwServiceTime&dwTimeMask)==0L) bLeaveServer=true;
					if ((bAllowAndroidAttend==false)&&(bAllowAndroidSimulate==false)) bLeaveServer=true;

					//ɾ���û�
					if (bLeaveServer==true)
					{
						//ɾ���û�
						WORD wRoundID=pAndroidUserItem->m_wRoundID;
						WORD wAndroidIndex=pAndroidUserItem->m_wAndroidIndex;
						DeleteAndroidUserItem(MAKELONG(wAndroidIndex,wRoundID));

						DISK_LOG(TEXT("��Ϸ���� �ͷŻ����� 3"));
						return true;
					}
				}
			}

			return true;
		}
	case IDI_ANDROID_PULSE:		//�û�����
		{
			//��������
			DWORD dwUserID=0L;
			CAndroidUserItem * pAndroidUserItem=NULL;
			POSITION Position=m_AndroidUserItemMap.GetStartPosition();

			//�û�����
			while (Position!=NULL)
			{
				//��ȡ����
				m_AndroidUserItemMap.GetNextAssoc(Position,dwUserID,pAndroidUserItem);

				//ʱ�䴦��
				try
				{
					ASSERT_ERR(pAndroidUserItem!=NULL);
					if ( pAndroidUserItem!=NULL && pAndroidUserItem->m_pIServerUserItem!=NULL) 
						pAndroidUserItem->OnTimerPulse(dwTimerID,dwBindParameter);
				}
				catch (...)
				{
					ASSERT_ERR(FALSE);
					DISK_LOG(TEXT("��Ϸ���� �ͷŻ����� 4"));
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
			//��Ҫ�Ļ�������Ŀ
			dwUserCount = m_wAutoAndroidCount - dwUserCount;

			int nTotal = m_AndroidUserItemMap.GetCount() + m_wStockCount; //�������+���ֵ(δ����������)


			if (dwUserCount < 0) dwUserCount = 0;

			if (nTotal > dwUserCount) //�Ƴ����������
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


				while (Position != NULL) //�Ƴ����л�����
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

						DISK_LOG(TEXT("��Ϸ���� �ͷŻ����� 5"));
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
			else if (nTotal < dwUserCount) //�����ٵĻ�����
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

//�û�״��
WORD CAndroidUserManager::GetAndroidUserInfo(tagAndroidUserInfo & AndroidUserInfo, DWORD dwServiceGender)
{
	//��ȡʱ��
	SYSTEMTIME SystemTime;
	GetLocalTime(&SystemTime);

	//���ñ���
	ZeroMemory(&AndroidUserInfo,sizeof(AndroidUserInfo));

	//��������
	DWORD dwTimeMask=(1L<<SystemTime.wHour);
	POSITION Position=m_AndroidUserItemMap.GetStartPosition();

	//ö�ٶ���
	while (Position!=NULL)
	{
		//��ȡ����
		DWORD dwUserID=0L;
		CAndroidUserItem * pAndroidUserItem=NULL;
		m_AndroidUserItemMap.GetNextAssoc(Position,dwUserID,pAndroidUserItem);

		//Ч�����
		ASSERT_ERR((dwUserID!=0L)&&(pAndroidUserItem!=NULL));
		if ((dwUserID==0L)||(pAndroidUserItem==NULL)) break;

		//���ж�
		if(pAndroidUserItem->m_pIServerUserItem==NULL) continue;

		//�����ж�
		if(pAndroidUserItem->m_AndroidService.dwResidualPlayDraw==0) continue;

		//��������
		IServerUserItem * pIServerUserItem=pAndroidUserItem->m_pIServerUserItem;
		tagAndroidParameter * pAndroidParameter=pAndroidUserItem->GetAndroidParameter();

		//�����ж�
		if ((pAndroidParameter->dwServiceTime&dwTimeMask)==0L) continue;
		if ((pAndroidParameter->dwServiceGender&dwServiceGender)==0L) continue;

		//״̬�ж�
		switch (pIServerUserItem->GetUserStatus())
		{
		case US_FREE:
			{
				ASSERT_ERR(AndroidUserInfo.wFreeUserCount<CountArray(AndroidUserInfo.pIAndroidUserFree));
				AndroidUserInfo.pIAndroidUserFree[AndroidUserInfo.wFreeUserCount++]=pAndroidUserItem;
				//I_TRACE(TEXT("���л�����:%d"),AndroidUserInfo.wFreeUserCount);
				break;
			}
		case US_SIT:
		case US_READY:
			{
				ASSERT_ERR(AndroidUserInfo.wSitdownUserCount<CountArray(AndroidUserInfo.pIAndroidUserSitdown));
				AndroidUserInfo.pIAndroidUserSitdown[AndroidUserInfo.wSitdownUserCount++]=pAndroidUserItem;
				//I_TRACE(TEXT("���»�����:%d"),AndroidUserInfo.wFreeUserCount);
				break;
			}
		case US_PLAYING:
		case US_OFFLINE:
			{
				ASSERT_ERR(AndroidUserInfo.wPlayUserCount<CountArray(AndroidUserInfo.pIAndroidUserPlay));
				AndroidUserInfo.pIAndroidUserPlay[AndroidUserInfo.wPlayUserCount++]=pAndroidUserItem;
				//I_TRACE(TEXT("��Ϸ������:%d"),AndroidUserInfo.wFreeUserCount);
				break;
			}
		}
	}

	return AndroidUserInfo.wFreeUserCount+AndroidUserInfo.wPlayUserCount+AndroidUserInfo.wSitdownUserCount;
}

//��������
bool CAndroidUserManager::SendDataToClient(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//��������
	for (INT_PTR i=0;i<m_AndroidUserItemArray.GetCount();i++)
	{
		//��ȡ����
		CAndroidUserItem * pAndroidUserItem=m_AndroidUserItemArray[i];

		//״̬�ж�
		if (pAndroidUserItem->m_pIServerUserItem==NULL) continue;
		if (pAndroidUserItem->m_AndroidParameter.dwUserID==0L) continue;

		//��Ϣ����
		try
		{
			if (pAndroidUserItem->OnSocketRead(wMainCmdID,wSubCmdID,pData,wDataSize)==false)
			{
				//throw 0;
			}
		}
		catch (...) 
		{
			//�������
			ASSERT_ERR(FALSE);

			DISK_LOG(TEXT("��Ϸ���� �ͷŻ����� 6"));
			//�Ͽ��û�
			WORD wRoundID=pAndroidUserItem->m_wRoundID;
			WORD wAndroidIndex=pAndroidUserItem->m_wAndroidIndex;
			DeleteAndroidUserItem(MAKELONG(wAndroidIndex,wRoundID));
		}
	}

	return true;
}

//��������
bool CAndroidUserManager::SendDataToClient(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{

	//��ȡ����
	WORD wIndex=LOWORD(dwAndroidID);
	CAndroidUserItem * pAndroidUserItem=GetAndroidUserItem(wIndex);

	//����Ч��
	//ASSERT_ERR((pAndroidUserItem!=NULL)&&(pAndroidUserItem->m_wRoundID==HIWORD(dwAndroidID)));
	if ((pAndroidUserItem==NULL)||(pAndroidUserItem->m_wRoundID!=HIWORD(dwAndroidID))) return false;

	//��Ϣ����
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
		DISK_LOG(TEXT("��Ϸ���� �ͷŻ����� 7"));
		DeleteAndroidUserItem(dwAndroidID);
	}

	return true;
}

//��������
bool CAndroidUserManager::SendDataToServer(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//��������
	TCP_Command Command;
	Command.wSubCmdID=wSubCmdID;
	Command.wMainCmdID=wMainCmdID;

	//��Ϣ����
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
		DISK_LOG(TEXT("��Ϸ���� �ͷŻ����� 8"));
		DeleteAndroidUserItem(dwAndroidID);
	}

	return true;
}

//��ȡ����
CAndroidUserItem * CAndroidUserManager::GetAndroidUserItem(WORD wIndex)
{
	//Ч������
	ASSERT_ERR(wIndex>=INDEX_ANDROID);
	if (wIndex<INDEX_ANDROID) return NULL;

	//Ч������
	ASSERT_ERR((wIndex-INDEX_ANDROID)<m_AndroidUserItemArray.GetCount());
	if (((wIndex-INDEX_ANDROID)>=m_AndroidUserItemArray.GetCount())) return NULL;

	//��ȡ����
	WORD wBufferIndex=wIndex-INDEX_ANDROID;
	CAndroidUserItem * pAndroidUserItem=m_AndroidUserItemArray[wBufferIndex];

	return pAndroidUserItem;
}


//�ͷŶ���
VOID CAndroidUserManager::FreeAndroidUserItem(DWORD dwUserID)
{
	//Ч�����
	ASSERT_ERR(dwUserID!=0);
	if (dwUserID==0) return;

	//��������
	CAndroidUserItem * pAndroidUserItem=NULL;
	m_AndroidUserItemMap.Lookup(dwUserID,pAndroidUserItem);

	//�����ж�
	if (pAndroidUserItem==NULL)
	{
		ASSERT_ERR(FALSE);
		return;
	}

	//��λ����
	pAndroidUserItem->RepositUserItem();

	//��������
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

//�������
CAndroidUserItem * CAndroidUserManager::ActiveAndroidUserItem(tagAndroidParameter & AndroidParameter)
{
	//��������
	CAndroidUserItem * pAndroidUserItem=NULL;
	INT_PTR nFreeItemCount=m_AndroidUserItemBuffer.GetCount();

	//��ȡ����
	if (nFreeItemCount>0)
	{
		//��ȡ����
		INT_PTR nItemPostion=nFreeItemCount-1;
		pAndroidUserItem=m_AndroidUserItemBuffer[nItemPostion];

		//ɾ������
		m_AndroidUserItemBuffer.RemoveAt(nItemPostion);
		//m_AndroidUserItemArray.Add(pAndroidUserItem);
	}

	//��������
	if (pAndroidUserItem==NULL)
	{
		//��Ŀ�ж�
		if (m_AndroidUserItemArray.GetCount()>=MAX_ANDROID)
		{
			ASSERT_ERR(FALSE);
			return NULL;
		}

		try
		{
			//��������
			IAndroidUserItemSink * pIAndroidUserItemSink=NULL;
			
			//��������
			if (CServerRule::IsAllowAndroidAttend(m_pGameServiceOption->dwServerRule)==true)
			{
				pIAndroidUserItemSink=(IAndroidUserItemSink *)m_pIGameServiceManager->CreateAndroidUserItemSink(IID_IAndroidUserItemSink,VER_IAndroidUserItemSink);
			}

			//��������
			try
			{
				pAndroidUserItem=new CAndroidUserItem;
			}
			catch (...)
			{
				//�������
				ASSERT_ERR(FALSE);

				//ɾ������
				SafeRelease(pIAndroidUserItemSink);
			}

			//�����ж�
			if (pAndroidUserItem==NULL)
			{
				//�������
				ASSERT_ERR(FALSE);

				//ɾ������
				SafeRelease(pIAndroidUserItemSink);

				return NULL;
			}

			//�����û�
			if ((pIAndroidUserItemSink!=NULL)&&(pIAndroidUserItemSink->Initialization(pAndroidUserItem)==false))
			{
				//�������
				ASSERT_ERR(FALSE);

				//ɾ������
				SafeRelease(pAndroidUserItem);
				SafeRelease(pIAndroidUserItemSink);

				return NULL;
			}

			//��������
			WORD wCurrentIndex=(WORD)m_AndroidUserItemArray.Add(pAndroidUserItem);

			//���ýӿ�
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

	//����ʱ��
	if ((AndroidParameter.dwMaxReposeTime!=0L)&&(AndroidParameter.dwMinReposeTime!=0L))
	{
		//��������
		DWORD dwMaxReposeTime=AndroidParameter.dwMaxReposeTime;
		DWORD dwMinReposeTime=AndroidParameter.dwMinReposeTime;

		//����ʱ��
		if ((dwMaxReposeTime-dwMinReposeTime)>0L)
			pAndroidUserItem->m_AndroidService.dwReposeTime=dwMinReposeTime+rand()%(dwMaxReposeTime-dwMinReposeTime);
		else
			pAndroidUserItem->m_AndroidService.dwReposeTime=dwMinReposeTime;
	}
	else
		pAndroidUserItem->m_AndroidService.dwReposeTime=TIME_ANDROID_REPOSE_TIME;

	//��Ϸ����
	if((AndroidParameter.dwMinPlayDraw!=0L)&&(AndroidParameter.dwMaxPlayDraw!=0L))
	{
		//��������
		DWORD dwMaxPlayDraw=AndroidParameter.dwMaxPlayDraw;
		DWORD dwMinPlayDraw=AndroidParameter.dwMinPlayDraw;

		//��������
		if ((dwMaxPlayDraw-dwMinPlayDraw)>0L)
			pAndroidUserItem->m_AndroidService.dwResidualPlayDraw=dwMinPlayDraw+rand()%(dwMaxPlayDraw-dwMinPlayDraw);
		else
			pAndroidUserItem->m_AndroidService.dwResidualPlayDraw=dwMinPlayDraw;
	}
	else
		pAndroidUserItem->m_AndroidService.dwResidualPlayDraw=ADNDROID_PLAY_DRAW;

	//��������
	if (pAndroidUserItem->m_pIAndroidUserItemSink == NULL && (AndroidParameter.dwServiceGender&ANDROID_SIMULATE) != 0)
		AndroidParameter.dwServiceGender = ANDROID_SIMULATE;
		

	//���ñ���
	pAndroidUserItem->m_AndroidParameter=AndroidParameter;

	//��������
	m_AndroidUserItemMap[AndroidParameter.dwUserID]=pAndroidUserItem;

	return pAndroidUserItem;
}

//��ʼ������
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
//��ȡ�������û�
//IAndroidUserItem* CAndroidUserManager::GetAndroidUser(DWORD dwUserID)
//{
//	CAndroidUserItem* pUser = nullptr;
//	m_AndroidUserItemMap.Lookup(dwUserID, pUser);
//
//	return pUser;
//}


//////////////////////////////////////////////////////////////////////////////////
