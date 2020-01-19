#include "StdAfx.h"
#include "ServiceUnits.h"
#include "ControlPacket.h"
#include <atlconv.h>
#include "AttemperEngineSink.h"

//////////////////////////////////////////////////////////////////////////////////
//ʱ���ʶ

#define IDI_LOAD_ANDROID_USER		(IDI_MAIN_MODULE_START+1)			//������Ϣ
#define IDI_REPORT_SERVER_INFO		(IDI_MAIN_MODULE_START+2)			//������Ϣ
#define IDI_CONNECT_CORRESPOND		(IDI_MAIN_MODULE_START+3)			//����ʱ��
#define IDI_GAME_SERVICE_PULSE		(IDI_MAIN_MODULE_START+4)			//��������
#define IDI_DISTRIBUTE_ANDROID		(IDI_MAIN_MODULE_START+5)			//�������
#define IDI_DBCORRESPOND_NOTIFY		(IDI_MAIN_MODULE_START+6)			//����֪ͨ
#define IDI_LOAD_SYSTEM_MESSAGE		(IDI_MAIN_MODULE_START+7)			//ϵͳ��Ϣ
#define IDI_SEND_SYSTEM_MESSAGE		(IDI_MAIN_MODULE_START+8)			//ϵͳ��Ϣ
#define IDI_LOAD_SENSITIVE_WORD		(IDI_MAIN_MODULE_START+9)			//�������д�

#define IDI_ANDRIOND_DELAY_JION		(IDI_MAIN_MODULE_START+10)			//�������ӳټ��أ�ʹ�����˼���ʱ��������Ȼ
#define IDI_REFRESH_ANDROID			(IDI_MAIN_MODULE_START+11)			//ˢ�»�����ʱ���

#define IDI_REDIS_SCROLL_MESSAGE	(IDI_MAIN_MODULE_START + 13)		// ��ȡ������Ϣ
#define IDI_LOAD_SCROLL_MESSAGE		(IDI_MAIN_MODULE_START + 14)		//��ȡ������Ϣ
#define IDI_CLEAR_SCROLL_MESSAGE	(IDI_MAIN_MODULE_START + 15)		//���������Ϣ
#define IDI_LOAD_TIP_MESSAGE		(IDI_MAIN_MODULE_START + 16)		//������Ϣ

#define IDI_USER_EXCEPTION_DETECT	(IDI_MAIN_MODULE_START + 17)		//����쳣���
#define IDI_USER_HEART_DETECT	(IDI_MAIN_MODULE_START + 18)		//����������

#define IDI_DISTRIBUTE_USER		    (IDI_MAIN_MODULE_START+19)			//�����û�

#define IDI_USER_ONLINE_DETECT		(IDI_MAIN_MODULE_START + 20)		//����������

//////////////////////////////////////////////////////////////////////////////////shut
//ʱ�䶨�� ��

#define TIME_LOAD_ANDROID_USER		600L								//���ػ���
#define TIME_DISTRIBUTE_ANDROID		8L									//�����û�
#define TIME_REPORT_SERVER_INFO		30L									//�ϱ�ʱ��
#define TIME_DBCORRESPOND_NOTIFY	3L									//����֪ͨʱ��
#define TIME_LOAD_SYSTEM_MESSAGE	3600L								//ϵͳ��Ϣʱ��
#define TIME_SEND_SYSTEM_MESSAGE	10L								    //ϵͳ��Ϣʱ��
#define TIME_LOAD_SENSITIVE_WORD	5L									//�������д�ʱ��
#define TIME_REFRESH_ANDROID		60L									//������ˢ��ʱ���
#define	TIME_TIMING_AWARD			1800L								//30���Ӷ�ʱ�佱ʱ��
#define	TIME_ANDROID_IS_ALLOWED_SIT_DOWN			45L								//45���ȡһ�λ������Ƿ���������
#define TIME_LOAD_SCROLL_MESSAGE		25L								//��ȡ������Ϣ
#define TIME_CLEAR_SCROLL_MESSAGE		300L							//���������Ϣ
#define TIME_LOAD_TIP_MESSAGE		28L									//������Ϣ
#define TIME_USER_EXCEPTION_DETECT	5L									//�쳣���ʱ��
#define TIME_USER_HEART_DETECT		3L									//�������ʱ��
#define TIME_USER_ONLINE_DETECT		10L									//������Ҳ�ѯ

#define TIME_REDIS_SCROLL_MES		1L									// Redis������Ϣ
#define KIND_FISH					2016
#define KIND_ZJH					6

TCHAR							g_szLogRoot[MAX_PATH];

//////////////////////////////////////////////////////////////////////////
namespace Utils {
	std::string ConvertFromUtf16ToUtf8(const std::wstring& wstr)
	{
		std::string convertedString;
		int requiredSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, 0, 0, 0, 0);
		if (requiredSize > 0)
		{
			std::vector<char> buffer(requiredSize);
			WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &buffer[0], requiredSize, 0, 0);
			convertedString.assign(buffer.begin(), buffer.end() - 1);
		}
		return convertedString;
	}

	std::wstring ConvertFromUtf8ToUtf16(const std::string& str)
	{
		std::wstring convertedString;
		int requiredSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, 0, 0);
		if (requiredSize > 0)
		{
			std::vector<wchar_t> buffer(requiredSize);
			MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &buffer[0], requiredSize);
			convertedString.assign(buffer.begin(), buffer.end() - 1);
		}

		return convertedString;
	}

	//���ֽ�תխ�ֽ�
	std::string ConvertW2A(const std::wstring &wstr)
	{
		setlocale(LC_ALL, ".936");
		size_t nSize = wstr.length() * 2 + 1;
		char *psz = new char[nSize];

		memset(psz, 0, nSize);
		wcstombs(psz, wstr.c_str(), nSize);
		std::string str = psz;
		delete[]psz;

		return str;
	}

	//խ�ֽ�ת���ֽ�
	std::wstring ConvertA2W(const string &str)
	{
		setlocale(LC_ALL, ".936");
		size_t nSize = str.length() + 1;
		wchar_t *wpsz = new wchar_t[nSize];

		memset(wpsz, 0, sizeof(wchar_t)*nSize);
		mbstowcs(wpsz, str.c_str(), nSize);
		std::wstring wstr = wpsz;
		delete[]wpsz;

		return wstr;
	}
}
//////////////////////////////////////////////////////////////////////////////////
//���캯��
CAttemperEngineSink::CAttemperEngineSink()
{
	//״̬����
	m_bCollectUser=false;
	m_bNeekCorrespond=true;

	//������
	m_pNormalParameter=NULL;
	m_pAndroidParameter=NULL;

	//״̬����
	m_pInitParameter=NULL;
	m_pGameParameter=NULL;
	m_pGameServiceAttrib=NULL;
	m_pGameServiceOption=NULL;

	//�������
	m_pITimerEngine=NULL;
	m_pIAttemperEngine=NULL;
	m_pITCPSocketService=NULL;
	m_pITCPNetworkEngine=NULL;
	m_pIGameServiceManager=NULL;

	//��������
	m_pIRecordDataBaseEngine=NULL;
	m_pIKernelDataBaseEngine=NULL;
	m_pIDBCorrespondManager=NULL;

	//��������
	ZeroMemory(&m_DataConfigColumn,sizeof(m_DataConfigColumn));
	ZeroMemory(&m_DataConfigProperty,sizeof(m_DataConfigProperty));

	//��������
	m_pIGameMatchServiceManager=NULL;

}

//��������
CAttemperEngineSink::~CAttemperEngineSink()
{
	//ɾ������
	SafeDeleteArray(m_pNormalParameter);
	SafeDeleteArray(m_pAndroidParameter);

	//ɾ������
	for (INT_PTR i=0;i<m_TableFrameArray.GetCount();i++)
	{
		SafeRelease(m_TableFrameArray[i]);
	}

	//��������
	m_KickUserItemMap.RemoveAll();
	m_DistributeManage.RemoveAll();

	//ClearSystemMessageData();

	return;
}

//�ӿڲ�ѯ
VOID * CAttemperEngineSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IMainServiceFrame,Guid,dwQueryVer);
	QUERYINTERFACE(IAttemperEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE(IServerUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAttemperEngineSink,Guid,dwQueryVer);
	return NULL;
}

//�����¼�
bool CAttemperEngineSink::OnAttemperEngineStart(IUnknownEx * pIUnknownEx)
{
	//����Ϣ
	m_pAndroidParameter=new tagBindParameter[MAX_ANDROID];
	ZeroMemory(m_pAndroidParameter,sizeof(tagBindParameter)*MAX_ANDROID);

	//����Ϣ
	m_pNormalParameter=new tagBindParameter[m_pGameServiceOption->wMaxPlayer];
	ZeroMemory(m_pNormalParameter,sizeof(tagBindParameter)*m_pGameServiceOption->wMaxPlayer);

	//��������guid
	GUID guid;
	CoCreateGuid(&guid);
	StringFromGUID2(guid, m_guidRoom, 40);

	//if( time(NULL)>= 1483200000 )
	//	return true;

	CString strguid(m_guidRoom);

	strguid.Remove(L'{');
	strguid.Remove(L'}');
	ZeroMemory(m_guidRoom, 40 * sizeof WCHAR);
	CopyMemory(m_guidRoom, strguid.GetBuffer(), strguid.GetLength() * sizeof WCHAR);

	//�������
	m_DistributeManage.SetDistributeRule(m_pGameServiceOption->cbDistributeRule);

	//���û���
	if (InitAndroidUser()==false)
	{
		ASSERT_ERR(FALSE);
		return false;
	}

	//��������
	if (InitTableFrameArray()==false)
	{
		ASSERT_ERR(FALSE);
		return false;
	}

	//���ýӿ�
	if (m_ServerUserManager.SetServerUserItemSink(QUERY_ME_INTERFACE(IServerUserItemSink))==false)
	{
		ASSERT_ERR(FALSE);
		return false;
	}

	//��������
	if (m_AndroidUserManager.StartService()==false)
	{
		ASSERT_ERR(FALSE);
		return false;
	}

	if ( m_pGameServiceOption->wServerType != GAME_GENRE_MATCH )
	{
		//ˢ�»�����ʱ�����ʱ��
		m_pITimerEngine->SetTimer(IDI_REFRESH_ANDROID, TIME_REFRESH_ANDROID * 1000L, TIMES_INFINITY, NULL);
		m_pITimerEngine->SetTimer(IDI_DISTRIBUTE_ANDROID,TIME_DISTRIBUTE_ANDROID*1000L,TIMES_INFINITY,NULL);

		//m_pITimerEngine->SetTimer(IDI_LOAD_ANDROID_USER,2*1000L,TIMES_INFINITY,NULL);
	}

	m_pITimerEngine->SetTimer(IDI_DBCORRESPOND_NOTIFY,TIME_DBCORRESPOND_NOTIFY*1000L,TIMES_INFINITY,NULL);

	//m_pITimerEngine->SetTimer(IDI_LOAD_SYSTEM_MESSAGE,80*1000L,TIMES_INFINITY,NULL);
	//m_pITimerEngine->SetTimer(IDI_SEND_SYSTEM_MESSAGE,15*1000L,TIMES_INFINITY,NULL);

	//��ʱ�������д�
	//m_pITimerEngine->SetTimer(IDI_LOAD_SENSITIVE_WORD,TIME_LOAD_SENSITIVE_WORD*1000L,TIMES_INFINITY,NULL);

	//������ģʽ
// 	if ((m_pGameServiceOption->cbDistributeRule&DISTRIBUTE_ALLOW) != 0)
// 	{
// 		m_pITimerEngine->SetTimer(IDI_DISTRIBUTE_USER, m_pGameServiceOption->wDistributeTimeSpace * 1000, TIMES_INFINITY, NULL);
// 	}
	m_pITimerEngine->SetTimer(IDI_DISTRIBUTE_USER, 3 * 1000, TIMES_INFINITY, NULL);


	OnAttemperEngineBegin( pIUnknownEx );
	OnAttemperEngineBegin_BCode( pIUnknownEx );

	return true;
}

//ֹͣ�¼�
bool CAttemperEngineSink::OnAttemperEngineConclude(IUnknownEx * pIUnknownEx)
{
	//ɾ������
	for (INT_PTR i = 0; i < m_TableFrameArray.GetCount(); i++)
	{
		SafeRelease(m_TableFrameArray[i]);
	}

	//��������
	m_KickUserItemMap.RemoveAll();
	m_DistributeManage.RemoveAll();

	//״̬����
	m_bCollectUser=false;
	m_bNeekCorrespond=true;

	//������Ϣ
	m_pInitParameter=NULL;
	m_pGameServiceAttrib=NULL;
	m_pGameServiceOption=NULL;

	//�������
	m_pITimerEngine=NULL;
	m_pITCPSocketService=NULL;
	m_pITCPNetworkEngine=NULL;

	//��������
	m_pIRecordDataBaseEngine=NULL;
	m_pIKernelDataBaseEngine=NULL;

	//������
	SafeDeleteArray(m_pNormalParameter);
	SafeDeleteArray(m_pAndroidParameter);

	//ɾ���û�
	m_TableFrameArray.RemoveAll();
	m_ServerUserManager.DeleteUserItem();
	m_ServerListManager.ResetServerList();
	m_DistributeManage.RemoveAll();

	//ֹͣ����
	m_AndroidUserManager.ConcludeService();

	//ֹͣ����
	if(m_pIGameMatchServiceManager!=NULL)
	{
		m_pIGameMatchServiceManager->StopService();
	}

	//�����Ϣ����
	ClearSystemMessageData();

	//���������
	PostReleaseAllAndroid();

	//��λ�ؼ���
	m_WordsFilter.ResetSensitiveWordArray();

	return true;
}



//�����¼�
bool CAttemperEngineSink::OnAttemperEngineBegin(IUnknownEx * pIUnknownEx)
{
	// ������ע��
	{
		REGISTER_FUNCTION_4(m_mMainFunctions, MDM_GR_LOGON, CAttemperEngineSink::OnTCPNetworkMainLogon, this);	// ��¼����
		REGISTER_FUNCTION_4(m_mMainFunctions, MDM_GR_USER, CAttemperEngineSink::OnTCPNetworkMainUser, this);	// �û�����
		REGISTER_FUNCTION_4(m_mMainFunctions, MDM_GF_GAME, CAttemperEngineSink::OnTCPNetworkMainGame, this);	// ��Ϸ����
		REGISTER_FUNCTION_4(m_mMainFunctions, MDM_GF_FRAME, CAttemperEngineSink::OnTCPNetworkMainFrame, this);	// �������
		REGISTER_FUNCTION_4(m_mMainFunctions, MDM_GR_INSURE, CAttemperEngineSink::OnTCPNetworkMainInsure, this);	// ��������
		REGISTER_FUNCTION_4(m_mMainFunctions, MDM_GR_MANAGE, CAttemperEngineSink::OnTCPNetworkMainManage, this);	// ��������
		REGISTER_FUNCTION_4(m_mMainFunctions, MDM_GR_MATCH, CAttemperEngineSink::OnTCPNetworkMainMatch, this);	// ��������
		REGISTER_FUNCTION_4(m_mMainFunctions, MDM_GR_HEART, CAttemperEngineSink::OnTCPNetworkMainHeart, this);	// ��������
	}

	//���ݿ��¼�ע��
	{
		REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GR_LOGON_SUCCESS, CAttemperEngineSink::OnDBLogonSuccess, this);	// ��¼�ɹ�
		REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GR_LOGON_SUCCESS_H5, CAttemperEngineSink::OnDBLogonSuccessH5, this);	// ��¼�ɹ�
		REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GR_LOGON_FAILURE, CAttemperEngineSink::OnDBLogonFailure, this);	// ��¼ʧ��
		REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GR_GAME_PARAMETER, CAttemperEngineSink::OnDBGameParameter, this);	// ��Ϸ����
		REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GR_GAME_COLUMN_INFO, CAttemperEngineSink::OnDBGameColumnInfo, this);	// �б���Ϣ
		REGISTER_FUNCTION_3(m_mDbFunctions, DBR_GR_GAME_ANDROID_INFO, CAttemperEngineSink::OnDBGameAndroidInfo, this);	// ������Ϣ
		REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GR_GAME_PROPERTY_INFO, CAttemperEngineSink::OnDBGamePropertyInfo, this);	// ������Ϣ
		REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GR_USER_INSURE_INFO, CAttemperEngineSink::OnDBUserInsureInfo, this);	// ������Ϣ
		REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GR_USER_INSURE_SUCCESS, CAttemperEngineSink::OnDBUserInsureSuccess, this);	// ���гɹ�
		REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GR_USER_INSURE_FAILURE, CAttemperEngineSink::OnDBUserInsureFailure, this);	// ����ʧ��
		REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GR_USER_INSURE_USER_INFO, CAttemperEngineSink::OnDBUserInsureUserInfo, this);	// �û���Ϣ
		REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GR_SYSTEM_MESSAGE_RESULT, CAttemperEngineSink::OnDBSystemMessage, this);	// ϵͳ��Ϣ
		REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GR_SENSITIVE_WORDS, CAttemperEngineSink::OnDBSensitiveWords, this);	// �������д�
		REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GR_ANDRIOD_RELEASE_DELTE, CAttemperEngineSink::OnDBAndroidReleaseAndDelete, this);	// �ͷŻ�����
		REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GR_ANDRIOD_APPLY, CAttemperEngineSink::OnDBAndroidApply, this);	// ���������
		REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_UPDATE_ATTRIBUTE, CAttemperEngineSink::OnDBUpdateAttribute, this);	// ˢ��ͨ������

	}

	//������־·��
	ZeroMemory(m_szPath,CountArray(m_szPath));
	CWHService::GetWorkDirectory(m_szPath,CountArray(m_szPath));
	_sntprintf(g_szLogRoot,sizeof(g_szLogRoot),TEXT("%s/SLog/GameServer/%s"),m_szPath,m_pGameServiceOption->szServerName);
	
	//��־�趨
	m_fileLog.SetRoot(TEXT("./SLog/GameServer/%s"), m_pGameServiceOption->szServerName);
	m_UserFileLog.SetRoot(TEXT("./SLog/UserAction/%s__User"), m_pGameServiceOption->szServerName);
	m_ServerFileLog.SetRoot(TEXT("./SLog/UserAction/%s____Server"), m_pGameServiceOption->szServerName);

	
	return true;
}

//�����¼�
bool CAttemperEngineSink::OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize)
{
	switch (wIdentifier)
	{
	case CT_CONNECT_CORRESPOND:		//����Э��
		{
			//��������
			tagAddressInfo * pCorrespondAddress=&m_pInitParameter->m_CorrespondAddress;
			m_pITCPSocketService->Connect(pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//������ʾ
			TCHAR szString[512]=TEXT("");
			_sntprintf(szString,CountArray(szString),TEXT("��������Э�������� [ %s:%d ]"),pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//��ʾ��Ϣ
			TRACE_LOG(szString,TraceLevel_Normal);

			return true;
		}
	case CT_LOAD_SERVICE_CONFIG:	//��������
		{
			ZeroMemory(m_szPath,CountArray(m_szPath));
			CWHService::GetWorkDirectory(m_szPath,CountArray(m_szPath));

			//��������
			m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_PARAMETER,0L,NULL,0L);
			
			//�����б�
			m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_GAME_COLUMN,0L,NULL,0L);

			//���ػ���
			if (m_pGameServiceOption->wServerType != GAME_GENRE_MATCH)
			{
				DBR_GR_GameAndroidApply dbrAndriond = {0};
				dbrAndriond.dwCount = m_pGameServiceOption->wAndroidCount;
				CopyMemory(dbrAndriond.guidRoom, m_guidRoom, 40 * sizeof WCHAR);
				dbrAndriond.dwRoomID = m_pGameServiceOption->wServerID;
				m_pIDBCorrespondManager->PostDataBaseRequest(0l, DBR_GR_ANDRIOD_APPLY, 0l, (void*)&dbrAndriond, sizeof(DBR_GR_GameAndroidApply));
			}

			//���ص���
			m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_GAME_PROPERTY,0L,NULL,0L);

			//������Ϣ
			//if ( m_pGameServiceOption->wServerType != GAME_GENRE_MATCH )
				//m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_SYSTEM_MESSAGE,0L,NULL,0L);


			//������Ϣ
			m_pITimerEngine->SetTimer(IDI_LOAD_TIP_MESSAGE,TIME_LOAD_TIP_MESSAGE * 1000L,TIMES_INFINITY,0);
			// Redis ������Ϣ
			m_pITimerEngine->SetTimer(IDI_REDIS_SCROLL_MESSAGE, TIME_REDIS_SCROLL_MES * 1000L, TIMES_INFINITY, 0);
			

			//������Ϣ
			m_pITimerEngine->SetTimer(IDI_LOAD_SCROLL_MESSAGE,TIME_LOAD_SCROLL_MESSAGE * 1000L,TIMES_INFINITY,0);

			//���������Ϣ
			m_pITimerEngine->SetTimer(IDI_CLEAR_SCROLL_MESSAGE,TIME_CLEAR_SCROLL_MESSAGE * 1000L,TIMES_INFINITY,0);

			//�����쳣��Ҽ��
			m_pITimerEngine->SetTimer(IDI_USER_EXCEPTION_DETECT,TIME_USER_EXCEPTION_DETECT * 1000L,TIMES_INFINITY,0);

			//��������������
			//m_pITimerEngine->SetTimer(IDI_USER_HEART_DETECT,TIME_USER_HEART_DETECT * 1000,TIMES_INFINITY,0);

			//����������Ҳ�ѯ
			m_pITimerEngine->SetTimer(IDI_USER_ONLINE_DETECT, TIME_USER_ONLINE_DETECT * 1000, TIMES_INFINITY, 0);

			return true;
		}
	}

	return false;
}

//�����¼�
bool CAttemperEngineSink::OnEventAttemperData(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	switch (wRequestID)
	{
	case SE_KICK_ALL_USER_REQUEST:	//�߳������û�
		{
			//D_TRACE(TEXT("��ʼ�߳������û�"));

			if (m_pGameServiceOption != NULL && m_pNormalParameter != NULL)
			{
				//��Ϣ����
				CMD_GR_KickAllUser * pKillAllUser=(CMD_GR_KickAllUser *)pData;

				//Ч������
				ASSERT(wDataSize<=sizeof(CMD_GR_KickAllUser));
				if( wDataSize > sizeof(CMD_GR_KickAllUser) ) return true;
				ASSERT(wDataSize==CountStringBuffer(pKillAllUser->szKickMessage));
				if (wDataSize!=CountStringBuffer(pKillAllUser->szKickMessage)) return true;

				TCHAR szMsg[128]={0};
				_sntprintf(szMsg, 128, TEXT("[%s]��ʱά�������Ժ��ٽ�����Ϸ��"), m_pGameServiceAttrib == NULL ? TEXT("����Ϸ") : m_pGameServiceAttrib->szGameName);

				//��ɢ������Ϸ
				for (INT_PTR i=0;i<m_TableFrameArray.GetCount();i++)
				{
					//��ȡ����
					CTableFrame * pTableFrame=m_TableFrameArray[i];
					if ( !pTableFrame->IsGameStarted() ) continue;

					//D_TRACE(TEXT("��ɢ[%s]"), pTableFrame->GetGameServiceAttrib()->szGameName);

					pTableFrame->DismissGame();
				}

				tagBindParameter *pBindParameter = m_pNormalParameter;
				for( INT i = 0; i < m_pGameServiceOption->wMaxPlayer; i++ )
				{
					//Ŀ¼�û�
					IServerUserItem * pITargerUserItem= pBindParameter->pIServerUserItem;
					if (pITargerUserItem==NULL) 
					{
						pBindParameter++;
						continue;
					}

					//D_TRACE(TEXT("�߳�[%s]"), pITargerUserItem->GetNickName());


					//������Ϣ
					SendRoomMessage(pITargerUserItem,szMsg,SMT_CHAT|SMT_EJECT|SMT_CLOSE_LINK|SMT_CLOSE_ROOM);

					pBindParameter++;

					pITargerUserItem->SetUserStatus(US_NULL, pITargerUserItem->GetTableID(), pITargerUserItem->GetChairID());
				} 

				//D_TRACE(TEXT("�߳�������"));
			}

			return true;
		}
	default:
		break;
	}
	return false;
}

//ʱ���¼�
bool CAttemperEngineSink::OnEventTimer(DWORD dwTimerID, WPARAM wBindParam)
{
	//����ʱ��
	if ((dwTimerID>=IDI_MAIN_MODULE_START)&&(dwTimerID<=IDI_MAIN_MODULE_FINISH))
	{
		//ʱ�䴦��
		switch (dwTimerID)
		{
		case IDI_REFRESH_ANDROID:		//ˢ�»�����ʱ���
			{
				do 
				{
					if ( m_pGameServiceOption->wServerType == GAME_GENRE_MATCH ) break;

					WORD wCount = m_AndroidUserManager.GetAndroidCount();
					if ( wCount >= m_pGameServiceOption->wAndroidCount ) break;

					DBR_GR_GameAndroidApply dbrAndriond = {0};
					dbrAndriond.dwCount = m_pGameServiceOption->wAndroidCount - m_AndroidUserManager.GetAndroidCount();
					CopyMemory(dbrAndriond.guidRoom, m_guidRoom, 40 * sizeof WCHAR);
					dbrAndriond.dwRoomID = m_pGameServiceOption->wServerID;
					m_pIDBCorrespondManager->PostDataBaseRequest(0l, DBR_GR_ANDRIOD_APPLY, 0l, (void*)&dbrAndriond, sizeof(DBR_GR_GameAndroidApply));

				} while (0);

				return true;
			}
		case IDI_DISTRIBUTE_USER: //�����û�
		{
			//ִ�з���
			PerformDistribute();

			return true;
		}
		case IDI_USER_EXCEPTION_DETECT:		//����쳣���
			{
				WORD wIndex = 0;
				do 
				{
					IServerUserItem * pIServerUserItem = m_ServerUserManager.EnumUserItem(wIndex++);
					if (pIServerUserItem == NULL) break;

					WORD wTableID = pIServerUserItem->GetTableID();
					WORD wChairID = pIServerUserItem->GetChairID();
					BYTE cbStatus = pIServerUserItem->GetUserStatus();
					BYTE cbClientKind = pIServerUserItem->GetUserInfo()->cbClientKind;
					if ( cbStatus == US_SIT || cbStatus == US_READY )
					{
						CTableFrame * pTableFrame = m_TableFrameArray[wTableID];
						if (pTableFrame == NULL) continue;
						if (pTableFrame->IsGameStarted()) continue;
						if ( pIServerUserItem->IsClientReady() == true ) continue;
						if ( pIServerUserItem->IsAndroidUser() ==false ) continue;

						I_TRACE(TEXT("�Զ���ʼ %s %d-%d ״̬[%d],����[%d]"),pIServerUserItem->GetNickName(),
							pIServerUserItem->GetTableID(),pIServerUserItem->GetChairID(),pIServerUserItem->GetUserStatus(),pIServerUserItem->IsAndroidUser());
						if (cbClientKind == CLIENT_KIND_WEBSOCKET)
						pTableFrame->OnEventSocketFrame(SUB_GF_GAME_OPTION_H5, NULL, 0, pIServerUserItem);
						else pTableFrame->OnEventSocketFrame(SUB_GF_GAME_OPTION, NULL, 0, pIServerUserItem);
					}

					

				} while (true);
				
				return true;
			}
		case IDI_USER_ONLINE_DETECT:
		{
			//return true;
			WORD wIndex = 0;
			BYTE cbUserCount = 0;
			CString cs;
			do 
			{			
				IServerUserItem * pIServerUserItem = m_ServerUserManager.EnumUserItem(wIndex++);
				if (pIServerUserItem == NULL) break;
				if ( pIServerUserItem->IsAndroidUser() ) continue;
				cs.Format(TEXT("���� %s ����[%d-%d],״̬[%d],����[%d]"), pIServerUserItem->GetNickName(), pIServerUserItem->GetTableID(), pIServerUserItem->GetChairID(), pIServerUserItem->GetUserStatus(), pIServerUserItem->IsAndroidUser());
				//I_TRACE(cs.GetBuffer(0));				
				m_fileLog.Log(cs.GetBuffer(0));
				
				DetectKickUser(pIServerUserItem,TRUE);
				cbUserCount++;
			} while (true);
			if (cbUserCount>0)
				m_fileLog.Log(TEXT("----------------------------------------"));
			

			return true;
		}
		case IDI_USER_HEART_DETECT:
			{
				return true;
			}
		case IDI_REPORT_SERVER_INFO:	//������Ϣ
			{
				//��������
				CMD_CS_C_ServerOnLine ServerOnLine;
				ZeroMemory(&ServerOnLine,sizeof(ServerOnLine));

				//���ñ���
				ServerOnLine.dwOnLineCount=m_ServerUserManager.GetUserItemCount();

				//��������
				m_pITCPSocketService->SendData(MDM_CS_SERVICE_INFO,SUB_CS_C_SERVER_ONLINE,&ServerOnLine,sizeof(ServerOnLine));

				return true;
			}
		case IDI_CONNECT_CORRESPOND:	//����Э��
			{
				//��������
				tagAddressInfo * pCorrespondAddress=&m_pInitParameter->m_CorrespondAddress;
				m_pITCPSocketService->Connect(pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

				//������ʾ
				TCHAR szString[512]=TEXT("");
				_sntprintf(szString,CountArray(szString),TEXT("��������Э�������� [ %s:%d ]"),pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

				//��ʾ��Ϣ
				//TRACE_LOG(szString,TraceLevel_Normal);
				I_TRACE(szString);

				return true;
			}
		case IDI_GAME_SERVICE_PULSE:	//����ά��
			{
				return true;
			}
		case IDI_DISTRIBUTE_ANDROID:	//�������
			{
				//��������
				if (m_AndroidUserManager.GetAndroidCount()>0 && m_pGameServiceOption->wServerType!=GAME_GENRE_MATCH)
				{
					//��������
					bool bAllowDynamicJoin=CServerRule::IsAllowDynamicJoin(m_pGameServiceOption->dwServerRule);			//��̬����
					bool bAllowAndroidAttend=CServerRule::IsAllowAndroidAttend(m_pGameServiceOption->dwServerRule);		//��������
					bool bAllowAndroidSimulate=CServerRule::IsAllowAndroidSimulate(m_pGameServiceOption->dwServerRule);	//����ռλ
					bool bAllowAvertCheatMode=(CServerRule::IsAllowAvertCheatMode(m_pGameServiceOption->dwServerRule)&&(m_pGameServiceAttrib->wChairCount < MAX_CHAIR));

					//��̬���û�������Ϊ
					//CString szFileName;
					//szFileName.Format(TEXT("%s/Settings/Android/%d_AndroidOption.ini"),m_szPath,m_pGameServiceOption->dwRoomID);

					//WORD wNotSitDown	= GetPrivateProfileInt(TEXT("Parameter"),TEXT("NotSitDown"),0,(LPCTSTR)szFileName);
					//if ( wNotSitDown != 0 ) return true;

					//ģ�⴦��
					//����ռλ�Ҳ��Ƿ�����ģʽ
					if (bAllowAndroidSimulate==true && bAllowAvertCheatMode==false)
					{
						//����״̬
						tagAndroidUserInfo AndroidSimulate; 
						m_AndroidUserManager.GetAndroidUserInfo(AndroidSimulate,ANDROID_SIMULATE);

						//��������
						if (AndroidSimulate.wFreeUserCount>0)
						{
							for (WORD i=0;i<8;i++)
							{
								//�������
								WORD wTableID=rand()%(__max(m_pGameServiceOption->wTableCount/3,1));

								//��ȡ����
								CTableFrame * pTableFrame=m_TableFrameArray[wTableID];
								if ((pTableFrame->IsGameStarted()==true)&&(bAllowDynamicJoin==false)) continue;

								//����״��
								tagTableUserInfo TableUserInfo;
								WORD wUserSitCount=pTableFrame->GetTableUserInfo(TableUserInfo);

								//�����ж�
								if (TableUserInfo.wTableUserCount>0) continue;
								if ((wUserSitCount>=TableUserInfo.wMinUserCount)&&(m_pGameServiceAttrib->wChairCount<MAX_CHAIR)) continue;

								//�����ж�
								if (AndroidSimulate.wFreeUserCount>=TableUserInfo.wMinUserCount)
								{
									//��������
									WORD wHandleCount=0;
									WORD wWantAndroidCount=TableUserInfo.wMinUserCount;

									//���ݵ���
									if (m_pGameServiceAttrib->wChairCount>TableUserInfo.wMinUserCount)
									{
										WORD wChairCount=m_pGameServiceAttrib->wChairCount;
										WORD wFreeUserCount=AndroidSimulate.wFreeUserCount;
										WORD wOffUserCount=__min(wChairCount,wFreeUserCount)-TableUserInfo.wMinUserCount;
										wWantAndroidCount+=((wOffUserCount > 0) ? (rand()%(wOffUserCount+1)) : 0);
									}

									//���´���
									for (WORD j=0;j<AndroidSimulate.wFreeUserCount;j++)
									{
										//��������
										WORD wChairID=pTableFrame->GetRandNullChairID();

										//��Ч����
										//ASSERT_ERR(wChairID!=INVALID_CHAIR);
										if (wChairID==INVALID_CHAIR) continue;

										//�û�����
										IAndroidUserItem * pIAndroidUserItem=AndroidSimulate.pIAndroidUserFree[j];
										if (pTableFrame->PerformSitDownAction(wChairID,pIAndroidUserItem->GetMeUserItem())==true)
										{
											//���ñ���
											wHandleCount++;

											//����ж�
											if (wHandleCount>=wWantAndroidCount) 
											{
												return true;
											}
										}
									}

									if(wHandleCount > 0) return true;
								}
							}
						}
					}

					//�����
					if (bAllowAndroidAttend==true)
					{
						//����״̬
						tagAndroidUserInfo AndroidPassivity;
						m_AndroidUserManager.GetAndroidUserInfo(AndroidPassivity,ANDROID_PASSIVITY);

						if(bAllowAvertCheatMode) //������ģʽ���
						{
							//���´���
							for (WORD j=0;j<AndroidPassivity.wFreeUserCount;j++)
							{
								IAndroidUserItem * pIAndroidUserItem=AndroidPassivity.pIAndroidUserFree[j];
								if (pIAndroidUserItem->GetMeUserItem()==NULL) continue;
								if (InsertDistribute(pIAndroidUserItem->GetMeUserItem())==true) return true;
							}
						}
						else
						{
							//��������
							if (AndroidPassivity.wFreeUserCount>0)
							{
								//������Ϸ
								if(m_pGameServiceAttrib->wChairCount >= MAX_CHAIR)
								{
									for (INT_PTR i=0;i<(m_pGameServiceOption->wTableCount);i++)
									{
										//��ȡ����
										CTableFrame * pTableFrame=m_TableFrameArray[i];
										if ((pTableFrame->IsGameStarted()==true)&&(bAllowDynamicJoin==false)) continue;

										//����״��
										tagTableUserInfo TableUserInfo;
										WORD wUserSitCount=pTableFrame->GetTableUserInfo(TableUserInfo);

										//�����ж�
										if (wUserSitCount>m_pGameServiceAttrib->wChairCount*2/3) continue;

										//��������
										IServerUserItem * pIServerUserItem=NULL;
										WORD wChairID=pTableFrame->GetRandNullChairID();

										//��Ч����
										ASSERT_ERR(wChairID!=INVALID_CHAIR);
										if (wChairID==INVALID_CHAIR) continue;

										//���´���
										for (WORD j=0;j<AndroidPassivity.wFreeUserCount;j++)
										{
											IAndroidUserItem * pIAndroidUserItem=AndroidPassivity.pIAndroidUserFree[j];
											if (pTableFrame->PerformSitDownAction(wChairID,pIAndroidUserItem->GetMeUserItem())==true) return true;
										}
									}
								}
								else
								{
									for (INT_PTR i=0;i<m_pGameServiceOption->wTableCount;i++)
									{
										//��ȡ����
										CTableFrame * pTableFrame=m_TableFrameArray[i];
										if ((pTableFrame->IsGameStarted()==true)&&(bAllowDynamicJoin==false)) continue;

										//����״��
										tagTableUserInfo TableUserInfo;
										WORD wUserSitCount=pTableFrame->GetTableUserInfo(TableUserInfo);

										//�����ж�
										if (wUserSitCount==0) continue;
										if (TableUserInfo.wTableUserCount==0) continue;
										if ((wUserSitCount>=TableUserInfo.wMinUserCount)&&(rand()%10>5)) continue;

										//��������
										IServerUserItem * pIServerUserItem=NULL;
										WORD wChairID=pTableFrame->GetRandNullChairID();

										//��Ч����
										ASSERT_ERR(wChairID!=INVALID_CHAIR);
										if (wChairID==INVALID_CHAIR) continue;

										//���´���
										for (WORD j=0;j<AndroidPassivity.wFreeUserCount;j++)
										{
											IAndroidUserItem * pIAndroidUserItem=AndroidPassivity.pIAndroidUserFree[j];
											if (pTableFrame->PerformSitDownAction(wChairID,pIAndroidUserItem->GetMeUserItem())==true) return true;
										}
									}
								}
							}
						}
					}

					//�����
					if (bAllowAndroidAttend==true)
					{
						//����״̬
						tagAndroidUserInfo AndroidInitiative;
						m_AndroidUserManager.GetAndroidUserInfo(AndroidInitiative,ANDROID_INITIATIVE);
						WORD wAllAndroidCount = AndroidInitiative.wFreeUserCount+AndroidInitiative.wPlayUserCount+AndroidInitiative.wSitdownUserCount;

						if(bAllowAvertCheatMode)
						{
							//���´���
							for (WORD j=0;j<AndroidInitiative.wFreeUserCount;j++)
							{
								IAndroidUserItem * pIAndroidUserItem=AndroidInitiative.pIAndroidUserFree[j];
								if (pIAndroidUserItem->GetMeUserItem()==NULL) continue;
								//I_TRACE(TEXT("�����˲������"));
								if (InsertDistribute(pIAndroidUserItem->GetMeUserItem())==true) return true;
							}
						}
						else
						{
							//��������
							if (AndroidInitiative.wFreeUserCount>0)
							{
								for (INT_PTR i=0;i<m_pGameServiceOption->wTableCount;i++)
								{
									//��ȡ����
									CTableFrame * pTableFrame=m_TableFrameArray[i];
									if ((pTableFrame->IsGameStarted()==true)&&(bAllowDynamicJoin==false)) continue;

									//����״��
									tagTableUserInfo TableUserInfo;
									WORD wUserSitCount=pTableFrame->GetTableUserInfo(TableUserInfo);

									//�����ж�
									if((m_pGameServiceAttrib->wChairCount<MAX_CHAIR) && wUserSitCount>(TableUserInfo.wMinUserCount-1)) continue;

									//��������
									IServerUserItem * pIServerUserItem=NULL;
									WORD wChairID=pTableFrame->GetRandNullChairID();

									//��Ч����
									ASSERT_ERR(wChairID!=INVALID_CHAIR);
									if (wChairID==INVALID_CHAIR) continue;

									//���´���
									for (WORD j=0;j<AndroidInitiative.wFreeUserCount;j++)
									{
										IAndroidUserItem * pIAndroidUserItem=AndroidInitiative.pIAndroidUserFree[j];
										if (pTableFrame->PerformSitDownAction(wChairID,pIAndroidUserItem->GetMeUserItem())==true) return true;
									}
								}
							}
						}
					}

					if ( bAllowAvertCheatMode == false )
					{
						//��������
						WORD wStandUpCount=0;
						WORD wRandCount=((rand()%3)+1);
						INT_PTR nIndex = rand()%(__max(m_pGameServiceOption->wTableCount,1));
						for (INT_PTR i=nIndex;i<m_pGameServiceOption->wTableCount+nIndex;++i)
						{
							//��ȡ����
							INT_PTR nTableIndex=i%m_pGameServiceOption->wTableCount;
							CTableFrame * pTableFrame=m_TableFrameArray[nTableIndex];
							if (pTableFrame->IsGameStarted()==true) continue;

							//����״��
							tagTableUserInfo TableUserInfo;
							WORD wUserSitCount=pTableFrame->GetTableUserInfo(TableUserInfo);

							//�û�����
							bool bRand = ((rand()%100)>50);
							if (TableUserInfo.wTableAndroidCount==0) continue;
							if ((TableUserInfo.wTableUserCount>0)&&(bAllowAndroidAttend==true) && bRand) continue;
							if (TableUserInfo.wTableAndroidCount>=TableUserInfo.wMinUserCount && bRand) continue;

							//��������
							for (WORD j=0;j<pTableFrame->GetChairCount();j++)
							{
								//��ȡ�û�
								IServerUserItem * pIServerUserItem=pTableFrame->GetTableUserItem(j);
								if (pIServerUserItem==NULL) continue;

								//�û�����
								if ((pIServerUserItem->IsAndroidUser()==true)&&(pTableFrame->PerformStandUpAction(pIServerUserItem)==true))
								{
									wStandUpCount++;
									if(wStandUpCount>=wRandCount)
										return true;
									else
										break;
								}
							}
						}

						//��������
						nIndex = rand()%(__max(m_pGameServiceOption->wTableCount,1));
						for (INT_PTR i=nIndex;i<m_pGameServiceOption->wTableCount+nIndex;++i)
						{
							//��ȡ����
							INT_PTR nTableIndex=i%m_pGameServiceOption->wTableCount;
							CTableFrame * pTableFrame=m_TableFrameArray[nTableIndex];
							if (pTableFrame->IsGameStarted()==true) continue;

							//����״��
							tagTableUserInfo TableUserInfo;
							WORD wUserSitCount=pTableFrame->GetTableUserInfo(TableUserInfo);

							//�û�����
							bool bRand = ((rand()%100)>50);
							if (TableUserInfo.wTableAndroidCount==0) continue;
							if ((TableUserInfo.wTableUserCount>0)&&(bAllowAndroidAttend==true) && bRand) continue;

							//��������
							for (WORD j=0;j<pTableFrame->GetChairCount();j++)
							{
								//��ȡ�û�
								IServerUserItem * pIServerUserItem=pTableFrame->GetTableUserItem(j);
								if (pIServerUserItem==NULL) continue;

								//�û�����
								
								if ((pIServerUserItem->IsAndroidUser()==true)&&(pTableFrame->PerformStandUpAction(pIServerUserItem)==true))
								{
									wStandUpCount++;
									if(wStandUpCount>=wRandCount)
										return true;
									else
										break;
								}
							}
						}
					}
				}	


				return true;
			}
		case IDI_DBCORRESPOND_NOTIFY: //���涨ʱ����
			{
				if(m_pIDBCorrespondManager) m_pIDBCorrespondManager->OnTimerNotify();
				return true;
			}
		case IDI_LOAD_SYSTEM_MESSAGE: //ϵͳ��Ϣ ����
			{
				//�����Ϣ����
				//ClearSystemMessageData();

				//������Ϣ
				//m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_SYSTEM_MESSAGE,0L,NULL,0L);

				return true;
			}
			//������Ϣ
		case IDI_LOAD_TIP_MESSAGE:
			{

// 				GameServer::GSLogonFailure gsLogonFailure;
// 				gsLogonFailure.set_errorcode(152345);
// 				gsLogonFailure.set_describestring(Utils::ConvertFromUtf16ToUtf8(TEXT("faklfdjks���ѿ�˿����ݼӿ��ٶȷ������ǵĿ����ͽ���յ�")).c_str());
// 
// 				std::string serializeStr;
// 				gsLogonFailure.SerializeToString(&serializeStr);
// 		
// 				//��ȡ������Ϣ
// 				m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_TIP_MESSAGE,0L, (void*)serializeStr.c_str(), (WORD)serializeStr.size());
				m_pIDBCorrespondManager->PostDataBaseRequest(0L, DBR_GR_LOAD_TIP_MESSAGE, 0L, NULL,0);

				return true;
			}
			// ��ȡ Redis ϵͳ������Ϣ
// 		case TIME_REDIS_SYSTEM_MESSAGE:
// 			{
// 				return true;
// 			}
			//��ȡ Redis ������Ϣ
		case IDI_REDIS_SCROLL_MESSAGE:
			{
				m_pIDBCorrespondManager->PostDataBaseRequest(0L, DBR_GR_SCROLL_MESSAGE, 0L, NULL, 0);
// 				CString CStrKey;
// 				CStrKey.Format(TEXT("activity:listener:key"));
// 				std::string Key("0"), Valus("0");
// 				Key = CT2A(CStrKey.GetBuffer());
// 				TINT count;
// 				m_RedisClent.hlen(Key, count);
// 				DATAITEM array;
// 				m_RedisClent.hgetall(Key, array);
				//CTraceService::TraceString(strArray->GetAt(i), TraceLevel_Exception);
				
				return true;

//  				CString CStrKey;
//  				CStrKey.Format(TEXT("Fish:%d:ScrollMessage"), m_pGameServiceOption->wRoomType);
//  				std::string Key("0"), Valus("0");
//  				Key = CT2A(CStrKey.GetBuffer());
//  				m_RedisClent.get(Key, Valus);
//  				std::string  NewValus("0");
//  				m_RedisClent.set(Key, NewValus);
//  
//  				CString MesBox(Valus.c_str());
//  				CStringArray* strArray = DivString(MesBox, TEXT("#"));
//  
//  				for (int i = 0; i < strArray->GetSize(); i++)
//  				{
//  					if (strArray->GetAt(i) != TEXT("") && strArray->GetAt(i) != TEXT("0"))
//  					{
//  						CTraceService::TraceString(strArray->GetAt(i), TraceLevel_Exception);
// 
// 						//GameServer::RedisRollMessage  RedisRollMes; CT2A(CStrKey.GetBuffer())
// 						//RedisRollMes.set_RedisRollMsg(Utils::ConvertFromUtf16ToUtf8(CT2A(strArray->GetAt(i).GetBuffer())));
// 						//std::string serializeStr;
// 						//RedisRollMes.SerializeToString(&serializeStr);
// 
// 						//�����û�
// 						WORD wIndex = 0;
// 						do
// 						{
// 							//��ȡ�û�
// 							IServerUserItem * pIServerUserItem = m_ServerUserManager.EnumUserItem(wIndex++);
// 							if (pIServerUserItem == NULL) break;
// 
// 							CString str;
// 							str.Format(TEXT("���û� %s -- %d ���͹�����Ϣ \n	%s"), 
// 								pIServerUserItem->GetNickName(), pIServerUserItem->GetUserID(), strArray->GetAt(i));
// 							CTraceService::TraceString(str, TraceLevel_Exception);
// 							//��������
// 							//SendData(pIServerUserItem, MDM_GF_FRAME, SUB_GF_REDIS_SCROLL_MES, (void*)serializeStr.c_str(), (WORD)serializeStr.size());
// 						} while (true);
//  					}
//  				}
// 
// 	
				

 				return true;
			}
			//��ȡ������Ϣ
		case IDI_LOAD_SCROLL_MESSAGE:
			{
				//��ȡ������Ϣ
				m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_SCROLL_MESSAGE,0L,NULL,0L);

				return true;
			}
			//���������Ϣ
		case IDI_CLEAR_SCROLL_MESSAGE:
			{
				//���������Ϣ
				m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_CLEAR_SCROLL_MESSAGE,0L,NULL,0L);

				return true;
			}
		case IDI_LOAD_SENSITIVE_WORD:	//�������д�
			{
				//Ͷ������
				m_pIRecordDataBaseEngine->PostDataBaseRequest(DBR_GR_LOAD_SENSITIVE_WORDS,0,NULL,0);				
				return true;
			}
		case IDI_SEND_SYSTEM_MESSAGE: //ϵͳ��Ϣ ���ϣ���Ϊ��¼������ϵͳ��Ϣ
			{

				return true;

				//�����ж�
				if(m_SystemMessageList.GetCount()==0) return true;

				//ʱЧ�ж�
				DWORD dwCurrTime = (DWORD)time(NULL);
				POSITION pos = m_SystemMessageList.GetHeadPosition();
				while(pos != NULL)
				{
					POSITION tempPos = pos;
					tagSystemMessage *pTagSystemMessage = m_SystemMessageList.GetNext(pos);
					if(pTagSystemMessage->dwLastTime+pTagSystemMessage->SystemMessage.dwTimeRate < dwCurrTime)
					{
						//��������
						pTagSystemMessage->dwLastTime=dwCurrTime;

						//������Ϣ
						CMD_GR_SendMessage SendMessage = {};
						SendMessage.cbAllRoom = FALSE;
						SendMessage.cbGame = (pTagSystemMessage->SystemMessage.cbMessageType==1)?TRUE:FALSE;
						SendMessage.cbRoom = (pTagSystemMessage->SystemMessage.cbMessageType==2)?TRUE:FALSE;
						if(pTagSystemMessage->SystemMessage.cbMessageType==3)
						{
							SendMessage.cbGame = TRUE;
							SendMessage.cbRoom = TRUE;
						}
						lstrcpyn(SendMessage.szSystemMessage,pTagSystemMessage->SystemMessage.szSystemMessage,CountArray(SendMessage.szSystemMessage));
						SendMessage.wChatLength = lstrlen(SendMessage.szSystemMessage)+1;

						//������Ϣ
						WORD wSendSize = sizeof(SendMessage)-sizeof(SendMessage.szSystemMessage)+CountStringBuffer(SendMessage.szSystemMessage);
						SendSystemMessage(&SendMessage,wSendSize);
					}
				}


				return true;
			}

		case IDI_ANDRIOND_DELAY_JION:
			{
				tagAndroidUserInfo AndroidSimulate; 
				m_AndroidUserManager.GetAndroidUserInfo(AndroidSimulate,ANDROID_SIMULATE);
				bool bAllowDynamicJoin=CServerRule::IsAllowDynamicJoin(m_pGameServiceOption->dwServerRule);

				INT wTableID = wBindParam;
				if (AndroidSimulate.wFreeUserCount > 0)
				{
					CTableFrame * pTableFrame=m_TableFrameArray[wTableID];
					if ((pTableFrame->IsGameStarted()==true)&&(bAllowDynamicJoin==false)) return true;

					//����״��
					tagTableUserInfo TableUserInfo;
					WORD wUserSitCount=pTableFrame->GetTableUserInfo(TableUserInfo);

					//�����ж�
					if (TableUserInfo.wTableUserCount>0) return true;

					//��������
					WORD wChairID=pTableFrame->GetRandNullChairID();

					//��Ч����
					if (wChairID==INVALID_CHAIR) return true;

					//�û�����
					IAndroidUserItem * pIAndroidUserItem=AndroidSimulate.pIAndroidUserFree[0];
					pTableFrame->PerformSitDownAction(wChairID,pIAndroidUserItem->GetMeUserItem());

				}

				return true;
			}
		}

	}

	//����ʱ��
	if ((dwTimerID>=IDI_REBOT_MODULE_START)&&(dwTimerID<=IDI_REBOT_MODULE_FINISH))
	{
		//ʱ�䴦��
		m_AndroidUserManager.OnEventTimerPulse(dwTimerID,wBindParam);

		return true;
	}

	//������ʱ��
	if((dwTimerID>=IDI_MATCH_MODULE_START)&&(dwTimerID<IDI_MATCH_MODULE_FINISH))
	{
		if(m_pIGameMatchServiceManager!=NULL) m_pIGameMatchServiceManager->OnEventTimer(dwTimerID,wBindParam);
		return true;
	}

	//����ʱ��
	if ((dwTimerID>=IDI_TABLE_MODULE_START)&&(dwTimerID<=IDI_TABLE_MODULE_FINISH))
	{
		//���Ӻ���
		DWORD dwTableTimerID=dwTimerID-IDI_TABLE_MODULE_START;
		WORD wTableID=(WORD)(dwTableTimerID/TIME_TABLE_MODULE_RANGE);

		//ʱ��Ч��
		if (wTableID>=(WORD)m_TableFrameArray.GetCount()) 
		{
			ASSERT_ERR(FALSE);
			return false;
		}

		//ʱ��֪ͨ
		CTableFrame * pTableFrame=m_TableFrameArray[wTableID];
		return pTableFrame->OnEventTimer(dwTableTimerID%TIME_TABLE_MODULE_RANGE,wBindParam);
	}

	return false;
}

//���ݿ��¼�
bool CAttemperEngineSink::OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	bool bResult = false;

	do
	{
		auto it = m_mDbFunctions.find(wRequestID);
		if (it == m_mDbFunctions.end())
			break;

		bResult = (it->second)(dwContextID, pData, wDataSize);

	} while (0);


	//�����¼�
	if(wRequestID>=DBO_GR_MATCH_EVENT_START && wRequestID<=DBO_GR_MATCH_EVENT_END)
	{
		//����Ч��
		if(m_pIGameMatchServiceManager==NULL) return false;

		tagBindParameter * pBindParameter=GetBindParameter(LOWORD(dwContextID));
		IServerUserItem *pIServerUserItem=pBindParameter!=NULL?pBindParameter->pIServerUserItem:NULL;

		return m_pIGameMatchServiceManager->OnEventDataBase(wRequestID,pIServerUserItem,pData,wDataSize);
	}

	return bResult;
}

//�ر��¼�
bool CAttemperEngineSink::OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason)
{
	//Э������
	if (wServiceID==NETWORK_CORRESPOND)
	{
		//���ñ���
		m_bCollectUser=false;

		//ɾ��ʱ��
		m_pITimerEngine->KillTimer(IDI_REPORT_SERVER_INFO);

		//�����ж�
		if (m_bNeekCorrespond==true)
		{
			//������ʾ
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("��Э�������������ӹر��ˣ�%ld �����������"),m_pInitParameter->m_wConnectTime);

			//��ʾ��Ϣ
			//TRACE_LOG(szDescribe,TraceLevel_Warning);
			I_TRACE(szDescribe);

			//����ʱ��
			ASSERT_ERR(m_pITimerEngine!=NULL);
			m_pITimerEngine->SetTimer(IDI_CONNECT_CORRESPOND,m_pInitParameter->m_wConnectTime*1000L,1,0);
		}

		return true;
	}

	return false;
}

//�����¼�
bool CAttemperEngineSink::OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode)
{
	//Э������
	if (wServiceID==NETWORK_CORRESPOND)
	{
		//�����ж�
		if (nErrorCode!=0)
		{
			//������ʾ
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("Э������������ʧ�� [ %ld ]��%ld �����������"),
				nErrorCode,m_pInitParameter->m_wConnectTime);

			//��ʾ��Ϣ
			//TRACE_LOG(szDescribe,TraceLevel_Warning);
			I_TRACE(szDescribe);

			//����ʱ��
			ASSERT_ERR(m_pITimerEngine!=NULL);
			m_pITimerEngine->SetTimer(IDI_CONNECT_CORRESPOND,m_pInitParameter->m_wConnectTime*1000L,1,0);

			return false;
		}

		//��ʾ��Ϣ
		TRACE_LOG(TEXT("���ڷ�����Ϸ����ע����Ϣ..."),TraceLevel_Normal);

		//��������
		CMD_CS_C_RegisterServer RegisterServer;
		ZeroMemory(&RegisterServer,sizeof(RegisterServer));

		//����˿�
		CServiceUnits * pServiceUnits=CServiceUnits::g_pServiceUnits;
		RegisterServer.wServerPort=pServiceUnits->m_TCPNetworkEngine->GetCurrentPort();

		//��������
		RegisterServer.wKindID=m_pGameServiceOption->wKindID;
		RegisterServer.wNodeID=m_pGameServiceOption->wNodeID;
		RegisterServer.wSortID=m_pGameServiceOption->wSortID;
		RegisterServer.wServerID=m_pGameServiceOption->wServerID;
		RegisterServer.wServerType = m_pGameServiceOption->wServerType;
		RegisterServer.dwOnLineCount=m_ServerUserManager.GetUserItemCount();
		RegisterServer.dwFullCount=m_pGameServiceOption->wMaxPlayer-RESERVE_USER_COUNT;
		lstrcpyn(RegisterServer.szServerName,m_pGameServiceOption->szServerName,CountArray(RegisterServer.szServerName));
		lstrcpyn(RegisterServer.szServerAddr,m_pInitParameter->m_ServiceAddress.szAddress,CountArray(RegisterServer.szServerAddr));
		RegisterServer.lCellScore = m_pGameServiceOption->lCellScore;									//��Ϸ����
		RegisterServer.cbRoomType = m_pGameServiceOption->wRoomType;

		//��������
		RegisterServer.lRestrictScore	= m_pGameServiceOption->lRestrictScore	;						//���ƻ���
		RegisterServer.lMinTableScore	= m_pGameServiceOption->lMinTableScore	;						//��ͻ���
		RegisterServer.lMinEnterScore	= m_pGameServiceOption->lMinEnterScore	;						//��ͻ���
		RegisterServer.lMaxEnterScore	= m_pGameServiceOption->lMaxEnterScore	;						//��߻���
		RegisterServer.lMinEnterTiLi	= m_pGameServiceOption->lMinEnterTiLi	;						//�������

		RegisterServer.wCardBoxIndex	= m_pGameServiceOption->wCardBoxIndex	;						//��������

					
		//����ʱ�� 
		ASSERT_ERR(m_pITimerEngine!=NULL);
		m_pITimerEngine->SetTimer(IDI_REPORT_SERVER_INFO,TIME_REPORT_SERVER_INFO*1000L,TIMES_INFINITY,0);

		//��������
		ASSERT_ERR(m_pITCPSocketService != NULL);
		m_pITCPSocketService->SendData(MDM_CS_REGISTER, SUB_CS_C_REGISTER_SERVER, &RegisterServer, sizeof(RegisterServer));
		

		return true;
	}

	return true;
}

//��ȡ�¼�
bool CAttemperEngineSink::OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//Э������
	if (wServiceID==NETWORK_CORRESPOND)
	{
		switch (Command.wMainCmdID)
		{
		case MDM_CS_REGISTER:		//ע�����
			{
				return OnTCPSocketMainRegister(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_SERVICE_INFO:	//������Ϣ
			{
				return OnTCPSocketMainServiceInfo(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_USER_COLLECT:	//�û�����
			{
				return OnTCPSocketMainUserCollect(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_MANAGER_SERVICE: //�������
			{
				return OnTCPSocketMainManagerService(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_REMOTE_SERVICE:	//Զ�̷���
			{
				return OnTCPSocketMainRemoteService(Command.wSubCmdID,pData,wDataSize);
			}
		}
	}

	//�������
	ASSERT_ERR(FALSE);

	return true;
}

//Զ�̷���
bool CAttemperEngineSink::OnTCPSocketMainRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID) {
	case  SUB_CS_S_USER_CLEAR:
	{
		ASSERT(wDataSize == sizeof(CMD_CS_S_ClearUser));
		if (wDataSize != sizeof(CMD_CS_S_ClearUser)) return false;

		CMD_CS_S_ClearUser* pClearUser = (CMD_CS_S_ClearUser*)pData;

		IServerUserItem * pIServerUserItem = m_ServerUserManager.SearchUserItem(pClearUser->dwUserID);
		if (pIServerUserItem == NULL) return true;
		WORD wTableID = pIServerUserItem->GetTableID();
		DWORD dwUserID = pIServerUserItem->GetUserID();
		BYTE cbStatus = pIServerUserItem->GetUserStatus();

		I_TRACE(TEXT("׼������UserID[%d],TableID[%d],Status[%d],��ʱ[%d]��"),dwUserID,wTableID, cbStatus,pClearUser->dwLimitTime*60);
		tagKickUser KickUser = { 0 };
		KickUser.dwLimitTime = pClearUser->dwLimitTime * 60;
		KickUser.dwStartKickTime = (DWORD)time(NULL);
		m_KickUserItemMap[dwUserID] = KickUser;

		bool bResult = false;
		bResult = DetectKickUser(pIServerUserItem, TRUE);
		if (bResult)
			I_TRACE(TEXT("�������"));
		else I_TRACE(TEXT("���������Ϸ�У��ӳ�����"));
		return true;
	}
	}
	return true;
}

//Ӧ���¼�
bool CAttemperEngineSink::OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID)
{
	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	//���ñ���
	if (pBindParameter!=NULL)
	{
		pBindParameter->dwSocketID=dwSocketID;
		pBindParameter->dwClientAddr=dwClientAddr;
		pBindParameter->dwActiveTime=(DWORD)time(NULL);

		return true;
	}

	//�������

	ASSERT_ERR(FALSE);
	return false;
}

//�ر��¼�
bool CAttemperEngineSink::OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID)
{
	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
	I_TRACE(TEXT("OnEventTCPNetworkShut SocketID:%d"), wBindIndex);
	g_Log.LogToFile(g_szLogRoot, TEXT("׼���Ͽ����� SocketID[%d]"),dwSocketID);

	if (pBindParameter==NULL) return false;

	//��ȡ�û�
	IServerUserItem * pIServerUserItem=pBindParameter->pIServerUserItem;

	//�û�����
	if (pIServerUserItem!=NULL)
	{
		//��������
		WORD wTableID=pIServerUserItem->GetTableID();

		if ( pIServerUserItem->IsAndroidUser() == false)
		g_Log.LogToFile(g_szLogRoot,TEXT("\nOnEventTCPNetworkShut UserID[%d] NickName[%s]:UserScore[%I64d]"),pIServerUserItem->GetUserID(),pIServerUserItem->GetNickName(),pIServerUserItem->GetUserScore());
		//���ߴ��� ������û�е��ߣ�һ���߳�����
		if (wTableID!=INVALID_TABLE && pIServerUserItem->IsAndroidUser()==false)
		{
			CTableFrame *pTableFrame = m_TableFrameArray[wTableID];
			BYTE cbStartMode = 0;
			if ( pTableFrame ) cbStartMode = pTableFrame->GetStartMode();		

			//����������
			//���泡ǿ�˽�ɢ��Ϸ		
			if( (cbStartMode&START_MODE_TIME_CONTROL && m_pGameServiceAttrib->wChairCount<MAX_CHAIR) ||	
				( m_pGameServiceOption->wRoomType == 4 ))
			{
				if (pTableFrame)
				{
					I_TRACE(TEXT("H5������ӶϿ������������ٶϿ�"));
					if ( pIServerUserItem->GetUserInfo()->cbClientKind == CLIENT_KIND_WEBSOCKET )
						pTableFrame->PerformStandUpActionH5(pIServerUserItem);
					else {
						pTableFrame->PerformStandUpAction(pIServerUserItem);
					}
					pIServerUserItem->SetUserStatus(US_NULL, INVALID_TABLE, INVALID_CHAIR);
				}
				
			}
			else
			{						
				//I_TRACE(TEXT("��� %s ,Status:%d,"),pIServerUserItem->GetNickName(),pIServerUserItem->GetUserStatus());
				//����֪ͨ
				ASSERT_ERR(wTableID<m_pGameServiceOption->wTableCount);
				m_TableFrameArray[wTableID]->OnEventUserOffLine(pIServerUserItem);
				//�����
				pIServerUserItem->DetachBindStatus();
				
			}

		}
		else
		{    
			I_TRACE(TEXT("�ر����ӻص� %s ,Status:%d,%d-%d"), pIServerUserItem->GetNickName(),pIServerUserItem->GetUserStatus(),pIServerUserItem->GetTableID(),pIServerUserItem->GetChairID());
			pIServerUserItem->SetUserStatus(US_NULL,INVALID_TABLE,INVALID_CHAIR);
		}
	}
	else if ( pIServerUserItem == NULL )
	{
		WORD wIndex=0;
		do
		{
			IServerUserItem * pIUserItem=m_ServerUserManager.EnumUserItem(wIndex++);
			if (pIUserItem==NULL) break;
			if ( pIUserItem->GetBindIndex() == wBindIndex) 
			{
				TCHAR szMsg[128] = {0};
				_stprintf(szMsg,TEXT("���%s���ӶϿ�����Դû�ͷ�"),pIUserItem->GetNickName());
				I_TRACE(szMsg);

				pIUserItem->SetUserStatus(US_NULL,INVALID_TABLE,INVALID_CHAIR);
				break;
			}

		} while (true);
	}

	//�����Ϣ
	pBindParameter->pIServerUserItem=NULL;
	ZeroMemory(pBindParameter,sizeof(tagBindParameter));

	return false;
}

//��ȡ�¼�
bool CAttemperEngineSink::OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	bool bResult = false;

	do 
	{
		auto it = m_mMainFunctions.find(Command.wMainCmdID);
		if(it == m_mMainFunctions.end())
			break;

		bResult = (it->second)(Command.wSubCmdID, pData, wDataSize, dwSocketID);

	} while (0);

	return bResult;
}

//������Ϣ
bool CAttemperEngineSink::SendRoomMessage(LPCTSTR lpszMessage, WORD wType)
{
	//��������
	CMD_CM_SystemMessage SystemMessage;
	ZeroMemory(&SystemMessage,sizeof(SystemMessage));

	//��������
	SystemMessage.wType=wType;
	SystemMessage.wLength=lstrlen(lpszMessage)+1;
	lstrcpyn(SystemMessage.szString,lpszMessage,CountArray(SystemMessage.szString));

	//��������
	WORD wHeadSize=sizeof(SystemMessage)-sizeof(SystemMessage.szString);
	WORD wSendSize=wHeadSize+CountStringBuffer(SystemMessage.szString);

	//��������
	m_AndroidUserManager.SendDataToClient(MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&SystemMessage,wSendSize);
	m_pITCPNetworkEngine->SendDataBatch(MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&SystemMessage,wSendSize,BG_COMPUTER);
	m_pITCPNetworkEngine->SendDataBatch(MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&SystemMessage,wSendSize,BG_MOBILE);

	return true;
}

//��Ϸ��Ϣ
bool CAttemperEngineSink::SendGameMessage(LPCTSTR lpszMessage, WORD wType)
{
	//��������
	CMD_CM_SystemMessage SystemMessage;
	ZeroMemory(&SystemMessage,sizeof(SystemMessage));

	//��������
	SystemMessage.wType=wType;
	SystemMessage.wLength=lstrlen(lpszMessage)+1;
	lstrcpyn(SystemMessage.szString,lpszMessage,CountArray(SystemMessage.szString));

	//��������
	WORD wHeadSize=sizeof(SystemMessage)-sizeof(SystemMessage.szString);
	WORD wSendSize=wHeadSize+CountStringBuffer(SystemMessage.szString);

	//��������
	m_AndroidUserManager.SendDataToClient(MDM_GF_FRAME,SUB_GF_SYSTEM_MESSAGE,&SystemMessage,wSendSize);
	m_pITCPNetworkEngine->SendDataBatch(MDM_GF_FRAME,SUB_GF_SYSTEM_MESSAGE,&SystemMessage,wSendSize,BG_COMPUTER);
	m_pITCPNetworkEngine->SendDataBatch(MDM_GF_FRAME,SUB_GF_SYSTEM_MESSAGE,&SystemMessage,wSendSize,BG_MOBILE);

	return true;
}

//������Ϣ
//#define SMT_CHAT						0x0001								//������Ϣ
//#define SMT_EJECT						0x0002								//������Ϣ
//#define SMT_GLOBAL					0x0004								//ȫ����Ϣ
//#define SMT_PROMPT					0x0008								//��ʾ��Ϣ
//#define SMT_TABLE_ROLL				0x0010								//������Ϣ
bool CAttemperEngineSink::SendRoomMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType)
{
	//Ч�����
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	if ( pIServerUserItem->GetUserInfo()->cbClientKind == CLIENT_KIND_WEBSOCKET )
	{
		GameServer::GSSystemMessage gsSystemMessage;
		gsSystemMessage.set_messagetype(wType);
		gsSystemMessage.set_messagestring(Utils::ConvertFromUtf16ToUtf8(lpszMessage));
		std::string serializeStr;
		gsSystemMessage.SerializeToString(&serializeStr);
		
		SendData(pIServerUserItem, MDM_CM_SYSTEM, SUB_CM_SYSTEM_MESSAGE,(void*)serializeStr.c_str(), (WORD)serializeStr.size());
		return true;
	}

	//��������
	if (pIServerUserItem->GetBindIndex()!=INVALID_WORD)
	{
		//��������
		CMD_CM_SystemMessage SystemMessage;
		ZeroMemory(&SystemMessage,sizeof(SystemMessage));

		//��������
		SystemMessage.wType=wType;
		SystemMessage.wLength=lstrlen(lpszMessage)+1;
		lstrcpyn(SystemMessage.szString,lpszMessage,CountArray(SystemMessage.szString));

		//��������
		WORD dwUserIndex=pIServerUserItem->GetBindIndex();
		tagBindParameter * pBindParameter=GetBindParameter(dwUserIndex);

		//��������
		WORD wHeadSize=sizeof(SystemMessage)-sizeof(SystemMessage.szString);
		WORD wSendSize=wHeadSize+CountStringBuffer(SystemMessage.szString);

		//��������
		if (pIServerUserItem->IsAndroidUser()==true)
		{
			//�����û�
			WORD wBindIndex=pIServerUserItem->GetBindIndex();
			tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
			m_AndroidUserManager.SendDataToClient(pBindParameter->dwSocketID,MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&SystemMessage,wSendSize);

			//�رմ���
			if ((wType&(SMT_CLOSE_ROOM|SMT_CLOSE_LINK))!=0) {
				
				m_AndroidUserManager.DeleteAndroidUserItem(pBindParameter->dwSocketID);
			}
		}
		else
		{
			//�����û�
			WORD wBindIndex=pIServerUserItem->GetBindIndex();
			tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
			m_pITCPNetworkEngine->SendData(pBindParameter->dwSocketID,MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&SystemMessage,wSendSize);
		}

		return true;
	}

	return false;
}

//��Ϸ��Ϣ
bool CAttemperEngineSink::SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType)
{
	//Ч�����
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	if ((pIServerUserItem->GetBindIndex()!=INVALID_WORD)&&(pIServerUserItem->IsClientReady()==true))
	{
		//��������
		CMD_CM_SystemMessage SystemMessage;
		ZeroMemory(&SystemMessage,sizeof(SystemMessage));

		//��������
		SystemMessage.wType=wType;
		SystemMessage.wLength=lstrlen(lpszMessage)+1;
		lstrcpyn(SystemMessage.szString,lpszMessage,CountArray(SystemMessage.szString));

		//��������
		WORD dwUserIndex=pIServerUserItem->GetBindIndex();
		tagBindParameter * pBindParameter=GetBindParameter(dwUserIndex);

		//��������
		WORD wHeadSize=sizeof(SystemMessage)-sizeof(SystemMessage.szString);
		WORD wSendSize=wHeadSize+CountStringBuffer(SystemMessage.szString);

		//��������
		if (pIServerUserItem->IsAndroidUser()==true)
		{
			//�����û�
			WORD wBindIndex=pIServerUserItem->GetBindIndex();
			tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
			m_AndroidUserManager.SendDataToClient(pBindParameter->dwSocketID,MDM_GF_FRAME,SUB_GF_SYSTEM_MESSAGE,&SystemMessage,wSendSize);

			//�رմ���
			if ((wType&(SMT_CLOSE_ROOM|SMT_CLOSE_LINK))!=0) {
				
				m_AndroidUserManager.DeleteAndroidUserItem(pBindParameter->dwSocketID);
			}
		}
		else
		{
			//�����û�
			WORD wBindIndex=pIServerUserItem->GetBindIndex();
			tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
			m_pITCPNetworkEngine->SendData(pBindParameter->dwSocketID,MDM_GF_FRAME,SUB_GF_SYSTEM_MESSAGE,&SystemMessage,wSendSize);
		}

		return true;
	}

	return false;
}

//������Ϣ
bool CAttemperEngineSink::SendRoomMessage(DWORD dwSocketID, LPCTSTR lpszMessage, WORD wType, bool bAndroid)
{
	//��������
	CMD_CM_SystemMessage SystemMessage;
	ZeroMemory(&SystemMessage,sizeof(SystemMessage));

	//��������
	SystemMessage.wType=wType;
	SystemMessage.wLength=lstrlen(lpszMessage)+1;
	lstrcpyn(SystemMessage.szString,lpszMessage,CountArray(SystemMessage.szString));

	//��������
	WORD wHeadSize=sizeof(SystemMessage)-sizeof(SystemMessage.szString);
	WORD wSendSize=wHeadSize+CountStringBuffer(SystemMessage.szString);

	//��������
	if (bAndroid)
	{
		//�����û�
		m_AndroidUserManager.SendDataToClient(dwSocketID,MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&SystemMessage,wSendSize);
	}
	else
	{
		//�����û�
		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&SystemMessage,wSendSize);
	}

	return true;
}

bool CAttemperEngineSink::SendMatchMessage(WORD wMainCmdID, WORD wSubCmdID)
{
	//������Ϣ
	ASSERT_ERR(m_pITCPSocketService!=NULL);
	m_pITCPSocketService->SendData(wMainCmdID,wSubCmdID);
	return true;
}

bool CAttemperEngineSink::SendMatchMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//������Ϣ
	ASSERT_ERR(m_pITCPSocketService!=NULL);
	m_pITCPSocketService->SendData(wMainCmdID,wSubCmdID,pData,wDataSize);
	return true;
}

//��������
bool CAttemperEngineSink::SendData(BYTE cbSendMask, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//��������
	if ((cbSendMask&BG_COMPUTER)!=0)
	{
		m_AndroidUserManager.SendDataToClient(wMainCmdID,wSubCmdID,pData,wDataSize);
	}

	//�û�����
	m_pITCPNetworkEngine->SendDataBatch(wMainCmdID,wSubCmdID,pData,wDataSize,cbSendMask);

	return true;
}

//��������
bool CAttemperEngineSink::SendData(DWORD dwSocketID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//��������
	if (LOWORD(dwSocketID)!=INVALID_WORD)
	{
		if (LOWORD(dwSocketID)>=INDEX_ANDROID)
		{
			//�����û�
			//I_TRACE(TEXT("���ͻ���������:SocketID[%d],M_Cmd[%d],S_Cmd[%d]"),dwSocketID,wMainCmdID,wSubCmdID);
			m_AndroidUserManager.SendDataToClient(dwSocketID,wMainCmdID,wSubCmdID,pData,wDataSize);
		}
		else 
		{
			//�����û�
			m_pITCPNetworkEngine->SendData(dwSocketID,wMainCmdID,wSubCmdID,pData,wDataSize);
		}
	}

	return true;
}

//��������
bool CAttemperEngineSink::SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	if (pIServerUserItem->GetBindIndex()!=INVALID_WORD)
	{
		if (pIServerUserItem->IsAndroidUser()==true)
		{
			//�����û�
			WORD wBindIndex=pIServerUserItem->GetBindIndex();
			tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
			m_AndroidUserManager.SendDataToClient(pBindParameter->dwSocketID,wMainCmdID,wSubCmdID,pData,wDataSize);
		}
		else
		{
			//�����û�
			WORD wBindIndex=pIServerUserItem->GetBindIndex();
			tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
			m_pITCPNetworkEngine->SendData(pBindParameter->dwSocketID,wMainCmdID,wSubCmdID,pData,wDataSize);
		}

		return true;
	}

	return false;
}

bool CAttemperEngineSink::PostDataBaseRequest(IServerUserItem* pIServerUserItem, WORD wRequest, VOID *pData, WORD wDataSize)
{
	//Ч�����
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	if (pIServerUserItem->GetBindIndex()!=INVALID_WORD)
	{
		WORD wBindIndex=pIServerUserItem->GetBindIndex();
		tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

		m_pIKernelDataBaseEngine->PostDataBaseRequest(wRequest, pBindParameter->dwSocketID, pData, wDataSize);

		return true;
	}

	return false;
}

//�û�����
bool CAttemperEngineSink::OnEventUserItemScore(IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	//Ч�����
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//�����Լ�д��
	if (m_pGameServiceOption->wServerType == GAME_GENRE_MATCH) return true;

	if ( cbReason == SCORE_REASON_WRITE || cbReason == SCORE_REASON_INSURE || cbReason == SCORE_REASON_PROPERTY || 
		 cbReason == SCORE_REASON_MEDAL )
	{
		tagUserInfo * pUserInfo = pIServerUserItem->GetUserInfo();
		DWORD dwUserID = pUserInfo->dwUserID;
		WORD wTableID = pUserInfo->wTableID;
		DWORD dwWinCount = pUserInfo->dwWinCount;
		DWORD dwLostCount = pUserInfo->dwLostCount;
		DWORD dwDrawCount = pUserInfo->dwDrawCount;
		DWORD dwFleeCount = pUserInfo->dwFleeCount;
		SCORE lUserScore = pUserInfo->lScore;

		WORD wBindIndex = pIServerUserItem->GetBindIndex();
		tagBindParameter * pBindParameter = GetBindParameter(wBindIndex);
		if (pBindParameter == NULL) return true;

		//if (pBindParameter != NULL && pBindParameter->cbClientKind == CLIENT_KIND_WEBSOCKET) 
		{

			GameServer::GSUserScore gsUserScore;
			gsUserScore.set_userid((int32)dwUserID);
			gsUserScore.mutable_userscore()->set_userscore((int64)lUserScore);
			gsUserScore.mutable_userscore()->set_wincount((int32)dwWinCount);
			gsUserScore.mutable_userscore()->set_lostcount((int32)dwLostCount);
			gsUserScore.mutable_userscore()->set_drawcount((int32)dwDrawCount);
			gsUserScore.mutable_userscore()->set_fleecount((int32)dwFleeCount);

			std::string serializeStr;
			gsUserScore.SerializeToString(&serializeStr);
			CTableFrame * pTableFrame = m_TableFrameArray[wTableID];
			if (pTableFrame)
				pTableFrame->SendTableData(INVALID_CHAIR, SUB_GR_USER_SCORE, (void*)serializeStr.c_str(), (WORD)serializeStr.size(), MDM_GR_USER);
		}
// 		else{
// 			CMD_GR_UserScore UserScore;
// 
// 			//��������
// 			UserScore.dwUserID = pUserInfo->dwUserID;
// 			UserScore.UserScore.dwWinCount = pUserInfo->dwWinCount;
// 			UserScore.UserScore.dwLostCount = pUserInfo->dwLostCount;
// 			UserScore.UserScore.dwDrawCount = pUserInfo->dwDrawCount;
// 			UserScore.UserScore.dwFleeCount = pUserInfo->dwFleeCount;
// 			UserScore.UserScore.dwUserMedal = pUserInfo->dwUserMedal;
// 			UserScore.UserScore.dwExperience = pUserInfo->dwExperience;
// 			UserScore.UserScore.lLoveLiness = pUserInfo->lLoveLiness;
// 
// 			//�������
// 			UserScore.UserScore.lGrade = pUserInfo->lGrade;
// 			UserScore.UserScore.lInsure = pUserInfo->lInsure;
// 
// 			//�������	
// 			UserScore.UserScore.lScore = pUserInfo->lScore;
// 
// 			//UserScore.UserScore.lScore += pIServerUserItem->GetTrusteeScore();
// 			//UserScore.UserScore.lScore += pIServerUserItem->GetFrozenedScore();
// 
// 			//��������
// 			SendData(BG_COMPUTER, MDM_GR_USER, SUB_GR_USER_SCORE, &UserScore, sizeof(UserScore));
// 			SendData(BG_MOBILE, MDM_GR_USER, SUB_GR_USER_SCORE, &UserScore, sizeof(UserScore));
// 		}	

		//��ʱд��
		if ((CServerRule::IsImmediateWriteScore(m_pGameServiceOption->dwServerRule)==true)&&(pIServerUserItem->IsVariation()==true))
		{
			if ( pIServerUserItem->IsAutoVariation() == true )
			{
				TCHAR szDescribe[128]=TEXT("");
				if (m_pGameServiceOption->wServerType&(GAME_GENRE_GOLD|GAME_GENRE_SCORE))
					_sntprintf(szDescribe,CountArray(szDescribe),TEXT("��Я���Ľ�Ҳ���") SCORE_STRING TEXT(" ,ϵͳ���Զ�����!"),m_pGameServiceOption->lMinEnterScore);
				SendGameMessage(pIServerUserItem,szDescribe,SMT_CHAT|SMT_EJECT);
			}

			//��������
			DBR_GR_WriteGameScore WriteGameScore;
			ZeroMemory(&WriteGameScore,sizeof(WriteGameScore));

			//�û���Ϣ
			WriteGameScore.dwUserID=pIServerUserItem->GetUserID();
			WriteGameScore.dwDBQuestID=pIServerUserItem->GetDBQuestID();
			WriteGameScore.dwClientAddr=pIServerUserItem->GetClientAddr();
			WriteGameScore.dwInoutIndex=pIServerUserItem->GetInoutIndex();
			WriteGameScore.wTableID = pIServerUserItem->GetTableID();

			//��ȡ����
			pIServerUserItem->DistillVariation(WriteGameScore.VariationInfo);


			//I_TRACE(TEXT("���%s ����д�� %I64d"),pIServerUserItem->GetNickName(),WriteGameScore.VariationInfo.lScore);
			//g_Log.LogToFile(g_szLogRoot,TEXT("\n [�û�д��] UserID[%d], WriteScore[%I64d],WriteRevenue[%I64d]"),pIServerUserItem->GetUserID(),WriteGameScore.VariationInfo.lScore,WriteGameScore.VariationInfo.lRevenue);
			//Ͷ������
			m_pIDBCorrespondManager->PostDataBaseRequest(WriteGameScore.dwUserID,DBR_GR_WRITE_GAME_SCORE,0L,&WriteGameScore,sizeof(WriteGameScore));
			
		}

		//֪ͨ����
		if(pIServerUserItem->GetTableID()!=INVALID_TABLE)
		{
			m_TableFrameArray[pIServerUserItem->GetTableID()]->OnUserScroeNotify(pIServerUserItem->GetChairID(),pIServerUserItem,cbReason);
		}
	}

	if ( cbReason == SCORE_REASON_WRITE )
	{


	}

	return true;
}

//�û�״̬
bool CAttemperEngineSink::OnEventUserItemStatus(IServerUserItem * pIServerUserItem, WORD wOldTableID, WORD wOldChairID, BYTE cbNotifyClient)
{
	//Ч�����
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	DWORD dwUserID = pIServerUserItem->GetUserID();
	WORD wTableID = pIServerUserItem->GetTableID();
	WORD wChairID = pIServerUserItem->GetChairID();
	BYTE cbUserStatus = pIServerUserItem->GetUserStatus();

	WORD wBindIndex = pIServerUserItem->GetBindIndex();
	tagBindParameter * pBindParameter = GetBindParameter(wBindIndex);
	if (pBindParameter == NULL) {
		CString cs;
		cs.Format(TEXT("����Ϊ��....UserID[%d],%d-%d,Status[%d]"), dwUserID, wTableID, wChairID, cbUserStatus);
		I_TRACE(cs.GetBuffer(0));
		m_ServerFileLog.Log(cs.GetBuffer(0));

		return true;
	}
	CString cs1;
	cs1.Format(TEXT("��״̬�仯��ʼ��|����[%d-%d] ���[%d] ״̬[%d]"), wTableID, wChairID, dwUserID, cbUserStatus);
	m_ServerFileLog.Log(cs1.GetBuffer(0));

	//1.ͬ�������Ϣ�·����������
	//2.���������Ϣ�·���ͬ�����
	if (wTableID>=0 && wTableID < m_pGameServiceOption->wTableCount)
	{

		CTableFrame * pTableFrame = m_TableFrameArray[wTableID];
		if (pTableFrame != NULL)
		{
			do 
			{
				if (pIServerUserItem->GetUserStatus() == US_SIT ||
					pIServerUserItem->GetUserStatus() == US_READY || //�ս���ʱ�·�ͬ�����
					pIServerUserItem->GetUserStatus() == US_PLAYING) //��������ʱ�·�ͬ�����
				{
					//if (pIServerUserItem->IsEnterTable() && m_pGameServiceOption->wKindID != 2016) break;

					//���·��Լ�
					if (pIServerUserItem->GetUserInfo()->cbClientKind == CLIENT_KIND_WEBSOCKET)
					{
						SendUserInfoPacketH5(pIServerUserItem, pBindParameter->dwSocketID);
						CString cs;
						cs.Format(TEXT("��������[3],������[100]��| ��ҽ���[%d] ״̬[%d] ����[%d-%d]��������Ϣ���Լ���"), pIServerUserItem->GetUserID(),pIServerUserItem->GetUserStatus(),pIServerUserItem->GetTableID(),pIServerUserItem->GetChairID());
						m_ServerFileLog.Log(cs.GetBuffer(0));
					}

					for ( auto i =0;i<pTableFrame->GetChairCount();++i )
					{
						IServerUserItem* pUserItem = pTableFrame->GetTableUserItem(i);
						if ( pUserItem == NULL ) continue;
						CString cs;
						cs.Format(TEXT("��ͬ���б�����[%d] ����[%d-%d] ���[%d] ״̬[%d]"),pTableFrame->GetTableID(),pUserItem->GetTableID(),pUserItem->GetChairID(),pUserItem->GetUserID(),pUserItem->GetUserStatus());
						m_ServerFileLog.Log(cs.GetBuffer(0));
					}

					for (int i = 0; i < pTableFrame->GetChairCount(); ++i) {
						IServerUserItem* pUserItem = pTableFrame->GetTableUserItem(i);
						if (pUserItem == NULL) continue;
						if (pIServerUserItem->GetUserID() == pUserItem->GetUserID()) continue;  //���˵��Լ�

						

						//���������H5���㲥����������Ҹ�H5
						if (pIServerUserItem->GetUserInfo()->cbClientKind == CLIENT_KIND_WEBSOCKET) {
							WORD wBindIndex = pIServerUserItem->GetBindIndex();
							tagBindParameter * pBindParameter = GetBindParameter(wBindIndex);
							if (pBindParameter)
							{
								SendUserInfoPacketH5(pUserItem, pBindParameter->dwSocketID);  //���˷����Լ�
								CString cs;
								cs.Format(TEXT("��������[3],������[100]��|��ͬ����Ϣ���Լ���ͬ����[%d],����Լ�[%d] "), pUserItem->GetUserID(), pIServerUserItem->GetUserID());
								m_ServerFileLog.Log(cs.GetBuffer(0));
							}

																						  //��������·���ͬ�����
							WORD wTableUserBindIndex = pUserItem->GetBindIndex();
							tagBindParameter* pTableUserBindParam = GetBindParameter(wTableUserBindIndex);
							//ͬ����ұ�����H5
							if (pTableUserBindParam)
							{
								SendUserInfoPacketH5(pIServerUserItem, pTableUserBindParam->dwSocketID); //�Լ���������
								CString cs;
								cs.Format(TEXT("��������[3],������[100]��|���Լ���Ϣ��ͬ����ͬ��[%d],����Լ�[%d] "), pUserItem->GetUserID(), pIServerUserItem->GetUserID());
								m_ServerFileLog.Log(cs.GetBuffer(0));
							}
						}
						else if (pIServerUserItem->IsAndroidUser() == true)
						{
							//�Լ���������
							WORD wTableUserBindIndex = pUserItem->GetBindIndex();
							tagBindParameter* pTableUserBindParam = GetBindParameter(wTableUserBindIndex);
							if (pTableUserBindParam) {
								SendUserInfoPacketH5(pIServerUserItem, pTableUserBindParam->dwSocketID);
								CString cs;
								cs.Format(TEXT("��������[3],������[100]��|�������˷������ˡ�����Լ�[%d] "),pIServerUserItem->GetUserID());
								m_ServerFileLog.Log(cs.GetBuffer(0));
							}
						}
					}

					pIServerUserItem->SetEnterTable(TRUE); //������ɲ����·�ͬ�����
				}
			} while (0);
					

		}
	}

	//3,���״̬����ͬ�����
	if ( pIServerUserItem->GetUserStatus() != US_NULL )
	{
		tagUserInfo* pUserInfo = pIServerUserItem->GetUserInfo();
		WORD wLastTableID = pUserInfo->wLastTableID;
		if (wLastTableID < 0 || wLastTableID >= m_pGameServiceOption->wTableCount) {
			I_TRACE(TEXT("error !!!"));
			return false;

		}

		GameServer::GSUserStatus gsUserStatus;
		gsUserStatus.set_userid(dwUserID);
		gsUserStatus.mutable_userstatus()->set_tableid((int32)wTableID);
		gsUserStatus.mutable_userstatus()->set_chairid((int32)wChairID);
		gsUserStatus.mutable_userstatus()->set_userstatus((GameServer::enumUserStatus)cbUserStatus);
		std::string serializeStr;
		gsUserStatus.SerializeToString(&serializeStr);
		if (pIServerUserItem->GetUserInfo()->cbClientKind == CLIENT_KIND_WEBSOCKET)
		{
			CString cs;
			cs.Format(TEXT("��������[3],������[102]��|��״̬�仯���Լ��� ���[%d] ״̬[%d] ����[%d-%d]"), pIServerUserItem->GetUserID(), cbUserStatus, wTableID, wChairID);
			m_ServerFileLog.Log(cs.GetBuffer(0));

			//���Լ�	
			SendData(pIServerUserItem, MDM_GR_USER, SUB_GR_USER_STATUS, (void*)serializeStr.c_str(), (WORD)serializeStr.size());
		}
		else if (pIServerUserItem->IsAndroidUser())
		{
			CMD_GR_UserStatus UserStatus;
			ZeroMemory(&UserStatus, sizeof(UserStatus));
			UserStatus.dwUserID = dwUserID;
			UserStatus.UserStatus.wTableID = wTableID;
			UserStatus.UserStatus.wChairID = wChairID;
			UserStatus.UserStatus.cbUserStatus = cbUserStatus;
			//���Լ�
			SendData(pIServerUserItem, MDM_GR_USER, SUB_GR_USER_STATUS, &UserStatus, sizeof(UserStatus));
		}


		//���״̬����ͬ�����
		if (cbNotifyClient == TRUE)
		{
			CTableFrame * pTableFrame = m_TableFrameArray[wLastTableID];
			for (int i = 0; i < pTableFrame->GetChairCount(); ++i) {
				if (i == pIServerUserItem->GetUserInfo()->wLastChairID) continue;  //�����Լ�

				IServerUserItem* pUserItem = pTableFrame->GetTableUserItem(i);
				if (pUserItem == NULL) continue;

				CString cs;
				cs.Format(TEXT("��������[3],������[102]��|��״̬�仯��ͬ����ͬ��[%d] ���[%d] ״̬[%d] ����[%d-%d]"), pUserItem->GetUserID(), pIServerUserItem->GetUserID(), cbUserStatus, wTableID, wChairID);
				m_ServerFileLog.Log(cs.GetBuffer(0));

				SendData(pUserItem, MDM_GR_USER, SUB_GR_USER_STATUS, (void*)serializeStr.c_str(), (WORD)serializeStr.size());
			}
		}
	}

	//�뿪�ж�
	if (pIServerUserItem->GetUserStatus()==US_NULL)
	{
		WORD wBindIndex=pIServerUserItem->GetBindIndex();
		tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
		
		//�뿪����
		OnEventUserLogout(pIServerUserItem, 0L);

		if (pBindParameter!=NULL)
		{
			I_TRACE(TEXT("�ر����� %s %d-%d,Status:%d,SocketID:%d"), pIServerUserItem->GetNickName(), pIServerUserItem->GetTableID(), pIServerUserItem->GetChairID(), pIServerUserItem->GetUserStatus(),pIServerUserItem->GetBindIndex());
			//�󶨴���
 			if (pBindParameter->pIServerUserItem==pIServerUserItem)
 			{
 			 	pBindParameter->pIServerUserItem=NULL;
 			 }

			//�ж�����  �����ɿͻ��˶Ͽ�
// 			if (pBindParameter->dwSocketID!=0L)
// 			{
// 				if (LOWORD(pBindParameter->dwSocketID)>=INDEX_ANDROID)
// 				{
// 					//m_AndroidUserManager.DeleteAndroidUserItem(pBindParameter->dwSocketID);
// 				}
// 				else
// 				{
// 					
// 					m_pITCPNetworkEngine->ShutDownSocket(pBindParameter->dwSocketID);
// 				}
// 			}
		}
		
	}
	cs1.Format(TEXT("��״̬�仯������----------------"));
	m_ServerFileLog.Log(cs1.GetBuffer(0));
	return true;
}

//�û�Ȩ��
bool CAttemperEngineSink::OnEventUserItemRight(IServerUserItem *pIServerUserItem, DWORD dwAddRight, DWORD dwRemoveRight,bool bGameRight)
{
	//Ч�����
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();

	DBR_GR_ManageUserRight ManageUserRight= {0};
	ManageUserRight.dwUserID = pUserInfo->dwUserID;
	ManageUserRight.dwAddRight = dwAddRight;
	ManageUserRight.dwRemoveRight = dwRemoveRight;
	ManageUserRight.bGameRight=bGameRight;

	//��������
	m_pIDBCorrespondManager->PostDataBaseRequest(ManageUserRight.dwUserID,DBR_GR_MANAGE_USER_RIGHT,0,&ManageUserRight,sizeof(ManageUserRight));

	return true;
}

//��¼�ɹ�
bool CAttemperEngineSink::OnDBLogonSuccessH5(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//��������
	WORD wBindIndex = LOWORD(dwContextID);
	tagBindParameter * pBindParameter = GetBindParameter(wBindIndex);
	DBO_GR_LogonSuccess * pDBOLogonSuccess = (DBO_GR_LogonSuccess *)pData;

	//��������
	bool bAndroidUser = (wBindIndex >= INDEX_ANDROID);
	bool bMobileUser = (pBindParameter->cbClientKind == CLIENT_KIND_MOBILE);

	//�����ж�
	tagKickUser KickUser = { 0 };
	if (m_KickUserItemMap.Lookup(pDBOLogonSuccess->dwUserID, KickUser) == TRUE)
	{
		//ʱЧ�ж�
		DWORD dwCurrTime = (DWORD)time(NULL);
		//if (dwKickTime + VALID_TIME_KICK_BY_MANAGER > dwCurrTime)
		if (KickUser.dwStartKickTime + KickUser.dwLimitTime > dwCurrTime)
		{
			//����ʧ��
			CString cs;
			cs.Format(TEXT("���ѱ�����Ա�������,%d�����ڲ��ܽ��룡"),KickUser.dwLimitTime/60);
			SendLogonFailure(cs.GetBuffer(0), 0, pBindParameter->dwSocketID);
			I_TRACE(cs.GetBuffer(0));
			//�������
			PerformUnlockScore(pDBOLogonSuccess->dwUserID, pDBOLogonSuccess->dwInoutIndex, LER_SERVER_CONDITIONS);

			return true;
		}
		else
		{
			//�Ƴ����
			m_KickUserItemMap.RemoveKey(pDBOLogonSuccess->dwUserID);
		}
	}
	I_TRACE(TEXT("-----------------��¼�ɹ�SocketID:%d----------"), dwContextID);
	g_Log.LogToFile(g_szLogRoot, TEXT("��¼�ɹ�SocketID[%d]"), dwContextID);

	//����Ҫ��ֵ��������ΪNULL
	if (pBindParameter->pIServerUserItem != NULL)
	{
		pBindParameter->pIServerUserItem = NULL;
	}

	if (pBindParameter->dwSocketID != dwContextID)
	{
		PerformUnlockScore(pDBOLogonSuccess->dwUserID, pDBOLogonSuccess->dwInoutIndex, LER_NORMAL);

		return true;
	}

	//�л��ж�
	IServerUserItem * pIServerUserItem = m_ServerUserManager.SearchUserItem(pDBOLogonSuccess->dwUserID);
	if (pIServerUserItem != NULL)
	{
		I_TRACE(TEXT("�л�����SocketID:%d"),dwContextID);
		//�л��û�
		SwitchUserItemConnectH5(pIServerUserItem, pDBOLogonSuccess->szMachineID, wBindIndex);

		if (pIServerUserItem->IsAndroidUser() == false)
		{
			g_Log.LogToFile(g_szLogRoot, TEXT("\n UserID[%d]-[���ݿ���Ϣ] NickName[%s]:UserScore[%I64d]"), pDBOLogonSuccess->dwUserID, pDBOLogonSuccess->szNickName, pDBOLogonSuccess->lScore);
			g_Log.LogToFile(g_szLogRoot, TEXT("\n UserID[%d]-[�ڴ���Ϣ] NickName[%s]:UserScore[%I64d]"), pIServerUserItem->GetUserID(), pIServerUserItem->GetNickName(), pIServerUserItem->GetUserScore());
		}

		return true;
	}

	//���һ���
	if (bAndroidUser == true && m_pGameServiceOption->wRoomType != 4)
	{
		//���һ���
		DWORD dwUserID = pDBOLogonSuccess->dwUserID;
		IAndroidUserItem * pIAndroidUserItem = m_AndroidUserManager.SearchAndroidUserItem(dwUserID, dwContextID);

		//�޸Ļ���

		if (pIAndroidUserItem != NULL && m_pGameServiceAttrib->wChairCount < MAX_CHAIR)
		{
			//���û���������Я�����
			if (m_pGameServiceOption->wServerType != GAME_GENRE_MATCH&&GAME_GENRE_SCORE != m_pGameServiceOption->wServerType)
			{
				if (m_pGameServiceOption->lMinEnterScore <= 10000)
					pDBOLogonSuccess->lScore = (long long)((float)rand() / 32767.0f * (float)(10000) * 3);
				else pDBOLogonSuccess->lScore = (long long)((float)rand() / 32767.0f * (float)(m_pGameServiceOption->lMinEnterScore) * 3);
			}
			if (m_pGameServiceOption->lMinEnterScore != 0 && pDBOLogonSuccess->lScore <= m_pGameServiceOption->lMinEnterScore)
			{
				if (m_pGameServiceAttrib->wChairCount == MAX_CHAIR)
					pDBOLogonSuccess->lScore = pDBOLogonSuccess->lScore + m_pGameServiceOption->lMinEnterScore * 3;
				else
					pDBOLogonSuccess->lScore += m_pGameServiceOption->lMinEnterScore;
			}


			//�������֣���������
			if (m_pGameServiceOption->lMaxEnterScore && pDBOLogonSuccess->lScore >= m_pGameServiceOption->lMaxEnterScore) pDBOLogonSuccess->lScore = m_pGameServiceOption->lMaxEnterScore;
		}

		if (pIAndroidUserItem != NULL && m_pGameServiceAttrib->wChairCount >= MAX_CHAIR)
		{
			//pDBOLogonSuccess->lScore = (long long)((float)rand()/32767.0f * (float)(1000000)*3);
			pDBOLogonSuccess->lScore = (long long)((float)rand() / 32767.0f * (float)(10000000) * 3);

		}
	}

	//��ͷ���
	if ((m_pGameServiceOption->lMinEnterScore != 0L) && (pDBOLogonSuccess->lScore < m_pGameServiceOption->lMinEnterScore))
	{
		I_TRACE(TEXT("��������SocketID:%d"), dwContextID);
		//����ʧ��
		TCHAR szMsg[128] = TEXT("");
		_sntprintf(szMsg, CountArray(szMsg), TEXT("��ǰ����׼��%0.2f������Ҳ����޷����룡"), double(m_pGameServiceOption->lMinEnterScore) / 1000);
		SendLogonFailure(szMsg, 0, pBindParameter->dwSocketID);
		//�������
		PerformUnlockScore(pDBOLogonSuccess->dwUserID, pDBOLogonSuccess->dwInoutIndex, LER_SERVER_CONDITIONS);

		return true;
	}

	//�����ж�
	WORD wMaxPlayer = m_pGameServiceOption->wMaxPlayer;
	DWORD dwOnlineCount = m_ServerUserManager.GetUserItemCount();
	I_TRACE(TEXT("�����������:%d,��������[%d],��������[%d]"), wMaxPlayer, RESERVE_USER_COUNT, dwOnlineCount);
	if ((pDBOLogonSuccess->cbMasterOrder == 0) && (dwOnlineCount > (DWORD)(wMaxPlayer - RESERVE_USER_COUNT)))
	{
		I_TRACE(TEXT("�����ж�SocketID:%d"), dwContextID);
		//����ʧ��
		SendLogonFailure(TEXT("��Ǹ�����ڴ˷����Ѿ���������ͨ��Ҳ��ܼ��������ˣ�"), 0, pBindParameter->dwSocketID);

		//�������
		PerformUnlockScore(pDBOLogonSuccess->dwUserID, pDBOLogonSuccess->dwInoutIndex, LER_SERVER_FULL);

		if (bAndroidUser)
			m_AndroidUserManager.DeleteAndroidUserItem(pBindParameter->dwSocketID);

		return true;
	}


	//�û�����
	tagUserInfo UserInfo;
	tagUserInfoPlus UserInfoPlus;
	ZeroMemory(&UserInfo, sizeof(UserInfo));
	ZeroMemory(&UserInfoPlus, sizeof(UserInfoPlus));

	//��������
	UserInfo.wFaceID = pDBOLogonSuccess->wFaceID;
	UserInfo.dwUserID = pDBOLogonSuccess->dwUserID;
	UserInfo.dwGameID = pDBOLogonSuccess->dwGameID;
	UserInfo.dwGroupID = pDBOLogonSuccess->dwGroupID;
	UserInfo.dwCustomID = pDBOLogonSuccess->dwCustomID;

	lstrcpyn(UserInfo.szNickName, pDBOLogonSuccess->szNickName, CountArray(UserInfo.szNickName));

	//�û�����
	UserInfo.cbGender = pDBOLogonSuccess->cbGender;
	UserInfo.cbMemberOrder = pDBOLogonSuccess->cbMemberOrder;
	UserInfo.cbMasterOrder = pDBOLogonSuccess->cbMasterOrder;
	lstrcpyn(UserInfo.szGroupName, pDBOLogonSuccess->szGroupName, CountArray(UserInfo.szGroupName));
	lstrcpyn(UserInfo.szUnderWrite, pDBOLogonSuccess->szUnderWrite, CountArray(UserInfo.szUnderWrite));

	//״̬����
	UserInfo.cbUserStatus = US_FREE;
	UserInfo.wTableID = INVALID_TABLE;
	UserInfo.wChairID = INVALID_CHAIR;

	//������Ϣ
	UserInfo.lScore = pDBOLogonSuccess->lScore;
	UserInfo.lInsure = pDBOLogonSuccess->lInsure;
	UserInfo.lRechargeScore = pDBOLogonSuccess->lRechargeScore;
	UserInfo.lExchangeScore = pDBOLogonSuccess->lExchangeScore;
	UserInfo.lRevenue = pDBOLogonSuccess->lRevenue;
	UserInfo.dwWinCount = pDBOLogonSuccess->dwWinCount;
	UserInfo.dwLostCount = pDBOLogonSuccess->dwLostCount;
	UserInfo.dwDrawCount = pDBOLogonSuccess->dwDrawCount;
	UserInfo.dwFleeCount = pDBOLogonSuccess->dwFleeCount;
	UserInfo.dwUserMedal = pDBOLogonSuccess->dwUserMedal;
	UserInfo.dwExperience = pDBOLogonSuccess->dwExperience;
	UserInfo.lLoveLiness = pDBOLogonSuccess->lLoveLiness;
	UserInfo.wAvatarID = pDBOLogonSuccess->wAvatarID;
	//add new ��������
	UserInfo.cbClientKind = pBindParameter->cbClientKind;

	//��¼��Ϣ
	UserInfoPlus.dwLogonTime = (DWORD)time(NULL);
	UserInfoPlus.dwInoutIndex = pDBOLogonSuccess->dwInoutIndex;

	//�û�Ȩ��
	UserInfoPlus.dwUserRight = pDBOLogonSuccess->dwUserRight;
	UserInfoPlus.dwMasterRight = pDBOLogonSuccess->dwMasterRight;

	//��������
	UserInfoPlus.bMobileUser = bMobileUser;
	UserInfoPlus.bAndroidUser = bAndroidUser;
	UserInfoPlus.lRestrictScore = m_pGameServiceOption->lRestrictScore;
	lstrcpyn(UserInfoPlus.szPassword, pDBOLogonSuccess->szPassword, CountArray(UserInfoPlus.szPassword));

	//������Ϣ
	UserInfoPlus.wBindIndex = wBindIndex;
	UserInfoPlus.dwClientAddr = pBindParameter->dwClientAddr;
	lstrcpyn(UserInfoPlus.szMachineID, pDBOLogonSuccess->szMachineID, CountArray(UserInfoPlus.szMachineID));

	//�볡����
	UserInfoPlus.lInitBlood = m_pGameServiceOption->dwBloodScore;
	UserInfoPlus.lEnterPower = m_pGameServiceOption->lMinEnterTiLi;
	UserInfoPlus.lEnterScore = m_pGameServiceOption->lMinEnterScore;
	UserInfoPlus.lMaxEnterScore = m_pGameServiceOption->lMaxEnterScore;
	UserInfoPlus.cbRoomType = (BYTE)m_pGameServiceOption->wRoomType;
	if (m_pGameServiceOption->dwRoomID)
		UserInfoPlus.cbBloodRoom = m_pGameServiceOption->wRoomKind;

	//�����û�
	m_ServerUserManager.InsertUserItem(&pIServerUserItem, UserInfo, UserInfoPlus);

	//�����ж�
	if (pIServerUserItem == NULL)
	{
		//�������
		ASSERT_ERR(FALSE);

		//�������
		PerformUnlockScore(pDBOLogonSuccess->dwUserID, pDBOLogonSuccess->dwInoutIndex, LER_SERVER_FULL);

		//�Ͽ��û�
		if (bAndroidUser == true)
		{

			m_AndroidUserManager.DeleteAndroidUserItem(dwContextID);
		}
		else
		{
			m_pITCPNetworkEngine->ShutDownSocket(dwContextID);
		}

		return true;
	}

	//�����û�
	pBindParameter->pIServerUserItem = pIServerUserItem;

	//��¼�¼�
	OnEventUserLogonH5(pIServerUserItem, false);

	//�����û�
	if (m_bCollectUser == true)
	{
		//��������
		CMD_CS_C_UserEnter UserEnter;
		ZeroMemory(&UserEnter, sizeof(UserEnter));

		//���ñ���
		UserEnter.dwUserID = pIServerUserItem->GetUserID();
		UserEnter.dwGameID = pIServerUserItem->GetGameID();
		lstrcpyn(UserEnter.szNickName, pIServerUserItem->GetNickName(), CountArray(UserEnter.szNickName));

		//������Ϣ
		UserEnter.cbGender = pIServerUserItem->GetGender();
		UserEnter.cbMemberOrder = pIServerUserItem->GetMemberOrder();
		UserEnter.cbMasterOrder = pIServerUserItem->GetMasterOrder();

		//������Ϣ
		ASSERT_ERR(m_pITCPSocketService != NULL);
		m_pITCPSocketService->SendData(MDM_CS_USER_COLLECT, SUB_CS_C_USER_ENTER, &UserEnter, sizeof(UserEnter));
	}
	if (pIServerUserItem->IsAndroidUser() == false)
		g_Log.LogToFile(g_szLogRoot, TEXT("\n OnDBLogonSuccessH5 UserID[%d],NickName[%s]:UserScore[%I64d]"), pIServerUserItem->GetUserID(), pIServerUserItem->GetNickName(), pIServerUserItem->GetUserScore());
	return true;
}

//��¼�ɹ�
bool CAttemperEngineSink::OnDBLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//��������
	WORD wBindIndex=LOWORD(dwContextID);
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
	DBO_GR_LogonSuccess * pDBOLogonSuccess=(DBO_GR_LogonSuccess *)pData;

	//����Ҫ��ֵ��������ΪNULL
	if ( pBindParameter->pIServerUserItem!=NULL )
	{
		pBindParameter->pIServerUserItem=NULL;
	}

	if ( pBindParameter->dwSocketID!=dwContextID )
	{
		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_NORMAL);

		return true;
	}

	//��������
	bool bAndroidUser=(wBindIndex>=INDEX_ANDROID);
	bool bMobileUser=(pBindParameter->cbClientKind==CLIENT_KIND_MOBILE);

	//�����ж�
	//DWORD dwKickTime;
	//if(m_KickUserItemMap.Lookup(pDBOLogonSuccess->dwUserID,dwKickTime)==TRUE)
	//{
	//	//ʱЧ�ж�
	//	DWORD dwCurrTime = (DWORD)time(NULL);
	//	if(dwKickTime+VALID_TIME_KICK_BY_MANAGER > dwCurrTime)
	//	{
	//		//����ʧ��
	//		SendLogonFailure(TEXT("���ѱ�����Ա�������,1Сʱ֮�ڲ��ܽ��룡"),0,pBindParameter->dwSocketID);

	//		//�������
	//		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_SERVER_CONDITIONS);

	//		return true;
	//	}
	//	else
	//	{
	//		//�Ƴ����
	//		m_KickUserItemMap.RemoveKey(pDBOLogonSuccess->dwUserID);
	//	}
	//}

	//�л��ж�
	IServerUserItem * pIServerUserItem=m_ServerUserManager.SearchUserItem(pDBOLogonSuccess->dwUserID);
	if (pIServerUserItem!=NULL)
	{
		//�л��û�
		SwitchUserItemConnect(pIServerUserItem,pDBOLogonSuccess->szMachineID,wBindIndex,pDBOLogonSuccess->cbDeviceType,pDBOLogonSuccess->wBehaviorFlags,pDBOLogonSuccess->wPageTableCount);

		//������� ������������������뿪 ����bug
		//PerformUnlockScore(pIServerUserItem->GetUserID(),pIServerUserItem->GetInoutIndex(),LER_USER_IMPACT);

		if ( pIServerUserItem->IsAndroidUser() == false)
		{
			g_Log.LogToFile(g_szLogRoot,TEXT("\n OnDBLogonSuccess1 UserID[%d],NickName[%s]:UserScore[%I64d]"),pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->szNickName,pDBOLogonSuccess->lScore);
			g_Log.LogToFile(g_szLogRoot,TEXT("\n OnDBLogonSuccess1 UserID[%d],NickName[%s]:UserScore[%I64d]"),pIServerUserItem->GetUserID(),pIServerUserItem->GetNickName(),pIServerUserItem->GetUserScore());
		}
		
		return true;
	}

	//���һ���
// 	if (bAndroidUser==true && m_pGameServiceOption->wRoomType != 4)
// 	{
// 		//���һ���
// 		DWORD dwUserID=pDBOLogonSuccess->dwUserID;
// 		IAndroidUserItem * pIAndroidUserItem=m_AndroidUserManager.SearchAndroidUserItem(dwUserID,dwContextID);
// 
// 		//�޸Ļ���
// 
// 		if (pIAndroidUserItem!=NULL && m_pGameServiceAttrib->wChairCount < MAX_CHAIR)
// 		{
// 			//���û���������Я�����
// 				if (m_pGameServiceOption->wServerType!=GAME_GENRE_MATCH&&GAME_GENRE_SCORE!=m_pGameServiceOption->wServerType)
// 				{
// 					if ( m_pGameServiceOption->lMinEnterScore <= 10000 )
// 						pDBOLogonSuccess->lScore = (long long)((float)rand()/32767.0f * (float)(10000)*3);
// 					else pDBOLogonSuccess->lScore = (long long)((float)rand()/32767.0f * (float)(m_pGameServiceOption->lMinEnterScore)*3);
// 				}
// 				if ( m_pGameServiceOption->lMinEnterScore!=0 && pDBOLogonSuccess->lScore <= m_pGameServiceOption->lMinEnterScore)
// 				{
// 					if ( m_pGameServiceAttrib->wChairCount==MAX_CHAIR )
// 					 pDBOLogonSuccess->lScore= pDBOLogonSuccess->lScore+m_pGameServiceOption->lMinEnterScore*3;
// 					else
// 					 pDBOLogonSuccess->lScore+=m_pGameServiceOption->lMinEnterScore;
// 				}
// 				
// 
// 				//�������֣���������
// 				//if (  m_pGameServiceOption->lMaxEnterScore && pDBOLogonSuccess->lScore >= m_pGameServiceOption->lMaxEnterScore ) pDBOLogonSuccess->lScore = m_pGameServiceOption->lMaxEnterScore;
// 		}
// 
// 		if (pIAndroidUserItem!=NULL && m_pGameServiceAttrib->wChairCount >= MAX_CHAIR)
// 		{
// 			//pDBOLogonSuccess->lScore = (long long)((float)rand()/32767.0f * (float)(1000000)*3);
// 			pDBOLogonSuccess->lScore = (long long)((float)rand()/32767.0f * (float)(10000000)*3);
// 
// 		}
// 	}

	//��ͷ���
	if ((m_pGameServiceOption->lMinEnterScore!=0L)&&(pDBOLogonSuccess->lScore<m_pGameServiceOption->lMinEnterScore))
	{
		//����ʧ��
		TCHAR szMsg[128]=TEXT("");
		_sntprintf(szMsg,CountArray(szMsg), TEXT("��Ǹ��������Ϸ��ҵ��ڵ�ǰ��Ϸ�������ͽ�����%0.2f�����ܽ��뵱ǰ��Ϸ���䣡"), double(m_pGameServiceOption->lMinEnterScore)/1000);
		SendLogonFailure(szMsg,0,pBindParameter->dwSocketID);
		I_TRACE(TEXT("������ ��ǰ��[%I64d],׼���[%I64d]"),pDBOLogonSuccess->lScore, m_pGameServiceOption->lMinEnterScore);
		//�������
		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_SERVER_CONDITIONS);

		return true;
	}

	//�����ж�
	WORD wMaxPlayer=m_pGameServiceOption->wMaxPlayer;
	DWORD dwOnlineCount=m_ServerUserManager.GetUserItemCount();
	if ((pDBOLogonSuccess->cbMasterOrder==0)&&(dwOnlineCount>(DWORD)(wMaxPlayer-RESERVE_USER_COUNT)))
	{
		//����ʧ��
		SendLogonFailure(TEXT("��Ǹ�����ڴ˷����Ѿ���������ͨ��Ҳ��ܼ��������ˣ�"),0,pBindParameter->dwSocketID);

		//�������
		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_SERVER_FULL);

		if ( bAndroidUser )
			m_AndroidUserManager.DeleteAndroidUserItem(pBindParameter->dwSocketID);

		return true;
	}


	//�û�����
	tagUserInfo UserInfo;
	tagUserInfoPlus UserInfoPlus;
	ZeroMemory(&UserInfo,sizeof(UserInfo));
	ZeroMemory(&UserInfoPlus,sizeof(UserInfoPlus));

	//��������
	UserInfo.wFaceID=pDBOLogonSuccess->wFaceID;
	UserInfo.dwUserID=pDBOLogonSuccess->dwUserID;
	UserInfo.dwGameID=pDBOLogonSuccess->dwGameID;
	UserInfo.dwGroupID=pDBOLogonSuccess->dwGroupID;
	UserInfo.dwCustomID=pDBOLogonSuccess->dwCustomID;
	
// 	if (CUserRight::CanKillOutUser(pDBOLogonSuccess->dwUserRight)==true )
// 	{
// 		TCHAR vc[32]={0};
// 		INT nTmp=0;
// 		for(int i=0;i<8;i++)
// 		{
// 			if(rand()%2==0)//����
// 			{
// 				vc[i]=TEXT('0')+rand()%9+1;
// 			}
// 			else//��ĸ
// 			{
// 				nTmp=rand()%26;
// 				while(nTmp==14)
// 				{
// 					nTmp=rand()%26;
// 				}			
// 				vc[i]=nTmp+65;	
// 			}
// 		}
// 		lstrcpyn(UserInfo.szNickName,vc,CountArray(UserInfo.szNickName));
// 		g_Log.LogToFile(g_szLogRoot,TEXT("\n OnDBLogonSuccess gm��nickName:%s"),UserInfo.szNickName);
// 	}
// 	else
		lstrcpyn(UserInfo.szNickName,pDBOLogonSuccess->szNickName,CountArray(UserInfo.szNickName));

	//�û�����
	UserInfo.cbGender=pDBOLogonSuccess->cbGender;
	UserInfo.cbMemberOrder=pDBOLogonSuccess->cbMemberOrder;
	UserInfo.cbMasterOrder=pDBOLogonSuccess->cbMasterOrder;
	lstrcpyn(UserInfo.szGroupName,pDBOLogonSuccess->szGroupName,CountArray(UserInfo.szGroupName));
	lstrcpyn(UserInfo.szUnderWrite,pDBOLogonSuccess->szUnderWrite,CountArray(UserInfo.szUnderWrite));

	//״̬����
	UserInfo.cbUserStatus=US_FREE;
	UserInfo.wTableID=INVALID_TABLE;
	UserInfo.wChairID=INVALID_CHAIR;

	//������Ϣ
	UserInfo.lScore=pDBOLogonSuccess->lScore;
	UserInfo.lInsure=pDBOLogonSuccess->lInsure;
	UserInfo.lRechargeScore = pDBOLogonSuccess->lRechargeScore;
	UserInfo.lExchangeScore = pDBOLogonSuccess->lExchangeScore;
	UserInfo.lRevenue = pDBOLogonSuccess->lRevenue;
	UserInfo.dwWinCount=pDBOLogonSuccess->dwWinCount;
	UserInfo.dwLostCount=pDBOLogonSuccess->dwLostCount;
	UserInfo.dwDrawCount=pDBOLogonSuccess->dwDrawCount;
	UserInfo.dwFleeCount=pDBOLogonSuccess->dwFleeCount;
	UserInfo.dwUserMedal=pDBOLogonSuccess->dwUserMedal;
	UserInfo.dwExperience=pDBOLogonSuccess->dwExperience;
	UserInfo.lLoveLiness=pDBOLogonSuccess->lLoveLiness;
	UserInfo.wAvatarID = pDBOLogonSuccess->wAvatarID;
	UserInfo.cbClientKind = pBindParameter->cbClientKind;
	//��¼��Ϣ
	UserInfoPlus.dwLogonTime=(DWORD)time(NULL);
	UserInfoPlus.dwInoutIndex=pDBOLogonSuccess->dwInoutIndex;

	//�û�Ȩ��
	UserInfoPlus.dwUserRight=pDBOLogonSuccess->dwUserRight;
	UserInfoPlus.dwMasterRight=pDBOLogonSuccess->dwMasterRight;

	//��������
	UserInfoPlus.bMobileUser=bMobileUser;
	UserInfoPlus.bAndroidUser=bAndroidUser;
	UserInfoPlus.lRestrictScore=m_pGameServiceOption->lRestrictScore;
	lstrcpyn(UserInfoPlus.szPassword,pDBOLogonSuccess->szPassword,CountArray(UserInfoPlus.szPassword));

	//������Ϣ
	UserInfoPlus.wBindIndex=wBindIndex;
	UserInfoPlus.dwClientAddr=pBindParameter->dwClientAddr;
	lstrcpyn(UserInfoPlus.szMachineID,pDBOLogonSuccess->szMachineID,CountArray(UserInfoPlus.szMachineID));

	//�볡����
	UserInfoPlus.lInitBlood		= m_pGameServiceOption->dwBloodScore;
	UserInfoPlus.lEnterPower	= m_pGameServiceOption->lMinEnterTiLi;
	UserInfoPlus.lEnterScore	= m_pGameServiceOption->lMinEnterScore;
	UserInfoPlus.lMaxEnterScore	= m_pGameServiceOption->lMaxEnterScore;
	UserInfoPlus.cbRoomType		= (BYTE)m_pGameServiceOption->wRoomType;
	if ( m_pGameServiceOption->dwRoomID )
	UserInfoPlus.cbBloodRoom	= m_pGameServiceOption->wRoomKind;

	//�����û�
	m_ServerUserManager.InsertUserItem(&pIServerUserItem,UserInfo,UserInfoPlus);

	//�����ж�
	if (pIServerUserItem==NULL)
	{
		//�������
		ASSERT_ERR(FALSE);

		//�������
		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_SERVER_FULL);

		//�Ͽ��û�
		if (bAndroidUser==true)
		{
			
			m_AndroidUserManager.DeleteAndroidUserItem(dwContextID);
		}
		else
		{
			m_pITCPNetworkEngine->ShutDownSocket(dwContextID);
		}

		return true;
	}

	//�����û�
	pBindParameter->pIServerUserItem=pIServerUserItem;

	//��¼�¼�
	OnEventUserLogon(pIServerUserItem,false);

	//�����û�
	if (m_bCollectUser==true)
	{
		//��������
		CMD_CS_C_UserEnter UserEnter;
		ZeroMemory(&UserEnter,sizeof(UserEnter));

		//���ñ���
		UserEnter.dwUserID=pIServerUserItem->GetUserID();
		UserEnter.dwGameID=pIServerUserItem->GetGameID();
		lstrcpyn(UserEnter.szNickName,pIServerUserItem->GetNickName(),CountArray(UserEnter.szNickName));

		//������Ϣ
		UserEnter.cbGender=pIServerUserItem->GetGender();
		UserEnter.cbMemberOrder=pIServerUserItem->GetMemberOrder();
		UserEnter.cbMasterOrder=pIServerUserItem->GetMasterOrder();

		//������Ϣ
		ASSERT_ERR(m_pITCPSocketService!=NULL);
		m_pITCPSocketService->SendData(MDM_CS_USER_COLLECT,SUB_CS_C_USER_ENTER,&UserEnter,sizeof(UserEnter));
	}
	if ( pIServerUserItem->IsAndroidUser() == false)
	g_Log.LogToFile(g_szLogRoot,TEXT("\n OnDBLogonSuccess2 UserID[%d],NickName[%s]:UserScore[%I64d]"),pIServerUserItem->GetUserID(),pIServerUserItem->GetNickName(),pIServerUserItem->GetUserScore());
	else {
		//I_TRACE(TEXT("������[%d],UserID[%d],����[%I64d]"), pIServerUserItem->IsAndroidUser(),pIServerUserItem->GetUserID(), pIServerUserItem->GetUserScore());
	}
	return true;
}

//��¼ʧ��
bool CAttemperEngineSink::OnDBLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	tagBindParameter * pBindParameter=GetBindParameter(LOWORD(dwContextID));
	if ((pBindParameter->dwSocketID!=dwContextID)/*||(pBindParameter->pIServerUserItem!=NULL)*/) return true;

	

	//���ʹ���
	DBO_GR_LogonFailure * pLogonFailure=(DBO_GR_LogonFailure *)pData;
	//SendLogonFailure(pLogonFailure->szDescribeString,pLogonFailure->lResultCode,dwContextID);
	//�·���¼ʧ����Ϣ

	if (pBindParameter && pBindParameter->cbClientKind == CLIENT_KIND_WEBSOCKET)
	{
		GameServer::GSLogonFailure gsLogonFailure;
		gsLogonFailure.set_errorcode(pLogonFailure->lResultCode);
		gsLogonFailure.set_describestring(Utils::ConvertFromUtf16ToUtf8(pLogonFailure->szDescribeString));
		std::string serializeStr;
		gsLogonFailure.SerializeToString(&serializeStr);
		SendData(pBindParameter->dwSocketID, MDM_GR_LOGON, SUB_GR_LOGON_FAILURE, (void*)serializeStr.c_str(), (WORD)serializeStr.size());
		return true;
	}

	//��������
	CMD_GR_LogonFailure LogonFailure;
	ZeroMemory(&LogonFailure,sizeof(LogonFailure));

	//��������
	LogonFailure.lErrorCode=pLogonFailure->lResultCode;
	lstrcpyn(LogonFailure.szDescribeString,pLogonFailure->szDescribeString,CountArray(LogonFailure.szDescribeString));

	//��������
	WORD wDataSize2=CountStringBuffer(LogonFailure.szDescribeString);
	WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);

	//��������
	SendData(dwContextID,MDM_GR_LOGON,SUB_GR_LOGON_FAILURE,&LogonFailure,wHeadSize+wDataSize2);

	//�Ͽ�����
	if (LOWORD(dwContextID)>=INDEX_ANDROID)
	{
		//CTraceService::TraceString(TEXT("OnDBLogonFailure �����˵�¼ʧ��"),TraceLevel_Info);
		I_TRACE(TEXT("OnDBLogonFailure �����˵�¼ʧ��"));
		m_AndroidUserManager.DeleteAndroidUserItem(dwContextID);
	}
	else
	{
		g_Log.LogToFile(g_szLogRoot,TEXT("\nOnDBLogonFailure "));
		m_pITCPNetworkEngine->ShutDownSocket(dwContextID);
	}
	
	return true;
}

//������Ϣ
bool CAttemperEngineSink::OnDBGameParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT_ERR(wDataSize==sizeof(DBO_GR_GameParameter));
	if (wDataSize!=sizeof(DBO_GR_GameParameter)) return false;

	//��������
	DBO_GR_GameParameter * pGameParameter=(DBO_GR_GameParameter *)pData;

	//������Ϣ
	m_pGameParameter->wMedalRate=pGameParameter->wMedalRate;
	m_pGameParameter->wRevenueRate=pGameParameter->wRevenueRate;

	//�汾��Ϣ
	m_pGameParameter->dwClientVersion=pGameParameter->dwClientVersion;
	m_pGameParameter->dwServerVersion=pGameParameter->dwServerVersion;

	//�汾Ч��
	if (VERSION_EFFICACY==TRUE)
	{
		//�汾�ж�
		bool bVersionInvalid=false;
		if (m_pGameParameter->dwClientVersion!=m_pGameServiceAttrib->dwClientVersion) bVersionInvalid=true;
		if (m_pGameParameter->dwServerVersion!=m_pGameServiceAttrib->dwServerVersion) bVersionInvalid=true;

		//��ʾ��Ϣ
		if (bVersionInvalid==true)
		{
			TRACE_LOG(TEXT("ƽ̨���ݿ��������汾ע����Ϣ�뵱ǰ����汾��Ϣ��һ��"),TraceLevel_Warning);
		}
	}

	return true;
}

//�б���Ϣ
bool CAttemperEngineSink::OnDBGameColumnInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//��������
	DBO_GR_GameColumnInfo * pGameColumnInfo=(DBO_GR_GameColumnInfo *)pData;
	WORD wHeadSize=sizeof(DBO_GR_GameColumnInfo)-sizeof(pGameColumnInfo->ColumnItemInfo);

	//Ч�����
	ASSERT_ERR((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pGameColumnInfo->cbColumnCount*sizeof(pGameColumnInfo->ColumnItemInfo[0]))));
	if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pGameColumnInfo->cbColumnCount*sizeof(pGameColumnInfo->ColumnItemInfo[0])))) return false;

	//���ݴ���
	if (pGameColumnInfo->cbColumnCount==0)
	{
		//Ĭ���б�
	}
	else
	{
		//��������
		m_DataConfigColumn.cbColumnCount=pGameColumnInfo->cbColumnCount;
		CopyMemory(m_DataConfigColumn.ColumnItem,pGameColumnInfo->ColumnItemInfo,pGameColumnInfo->cbColumnCount*sizeof(pGameColumnInfo->ColumnItemInfo[0]));
	}

	return true;
}

//������Ϣ
bool CAttemperEngineSink::OnDBGameAndroidInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//��������
	DBO_GR_GameAndroidInfo * pGameAndroidInfo=(DBO_GR_GameAndroidInfo *)pData;
	WORD wHeadSize=sizeof(DBO_GR_GameAndroidInfo)-sizeof(pGameAndroidInfo->AndroidParameter);

	//Ч�����
	ASSERT_ERR((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pGameAndroidInfo->wAndroidCount*sizeof(pGameAndroidInfo->AndroidParameter[0]))));
	if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pGameAndroidInfo->wAndroidCount*sizeof(pGameAndroidInfo->AndroidParameter[0])))) return false;

	//���û���
	if (pGameAndroidInfo->lResultCode==DB_SUCCESS)
	{
		m_AndroidUserManager.SetAndroidStock(pGameAndroidInfo->AndroidParameter,pGameAndroidInfo->wAndroidCount);
	}

	return true;
}

//ˢ��ͨ������
bool CAttemperEngineSink::OnDBUpdateAttribute(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_UPDATE_ATTRIBUTE,pData,wDataSize);

	return true;
}

//���������
bool CAttemperEngineSink::OnDBAndroidApply(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	DBO_GR_GameAndroidApply * pGameAndroidInfo=(DBO_GR_GameAndroidApply *)pData;
	WORD wHeadSize=sizeof(DBO_GR_GameAndroidApply)-sizeof(pGameAndroidInfo->AndroidParameter);

	//Ч�����
	ASSERT_ERR((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pGameAndroidInfo->wAndroidCount*sizeof(pGameAndroidInfo->AndroidParameter[0]))));
	if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pGameAndroidInfo->wAndroidCount*sizeof(pGameAndroidInfo->AndroidParameter[0])))) return false;

	if (pGameAndroidInfo->lResultCode == DB_SUCCESS)
	{
		m_AndroidUserManager.SetAndroidStock(pGameAndroidInfo->AndroidParameter,pGameAndroidInfo->wAndroidCount);
	}

	return true;
}

bool CAttemperEngineSink::OnDBAndroidReleaseAndDelete(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	DBO_GR_GameAndriodRelease* pAndroid = (DBO_GR_GameAndriodRelease *)pData;

	if (wDataSize != sizeof DBO_GR_GameAndriodRelease) return false;

	tagBindParameter* pTagBind = GetBindParameter(pAndroid->wBindId);

	m_AndroidUserManager.DeleteAndroidUserItem(pTagBind->dwSocketID);

	return true;
}

//������Ϣ
bool CAttemperEngineSink::OnDBGamePropertyInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//��������
	DBO_GR_GamePropertyInfo * pGamePropertyInfo=(DBO_GR_GamePropertyInfo *)pData;
	WORD wHeadSize=sizeof(DBO_GR_GamePropertyInfo)-sizeof(pGamePropertyInfo->PropertyInfo);

	//Ч�����
	ASSERT_ERR((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pGamePropertyInfo->cbPropertyCount*sizeof(pGamePropertyInfo->PropertyInfo[0]))));
	if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pGamePropertyInfo->cbPropertyCount*sizeof(pGamePropertyInfo->PropertyInfo[0])))) return false;

	//��ȡ״̬
	CServiceUnits * pServiceUnits=CServiceUnits::g_pServiceUnits;
	enServiceStatus ServiceStatus=pServiceUnits->GetServiceStatus();

	//���õ���
	if (pGamePropertyInfo->lResultCode==DB_SUCCESS)
	{
		//���ù���
		m_GamePropertyManager.SetGamePropertyInfo(pGamePropertyInfo->PropertyInfo,pGamePropertyInfo->cbPropertyCount);

		//��������
		m_DataConfigProperty.cbPropertyCount=pGamePropertyInfo->cbPropertyCount;
		CopyMemory(m_DataConfigProperty.PropertyInfo,pGamePropertyInfo->PropertyInfo,pGamePropertyInfo->cbPropertyCount*sizeof(pGamePropertyInfo->PropertyInfo[0]));
	}

	//�¼�֪ͨ
	if (ServiceStatus!=ServiceStatus_Service && m_pGameServiceOption->wServerType != GAME_GENRE_MATCH)
	{
		CP_ControlResult ControlResult;
		ControlResult.cbSuccess=ER_SUCCESS;
		SendUIControlPacket(UI_SERVICE_CONFIG_RESULT,&ControlResult,sizeof(ControlResult));
	}

	return true;
}

//������Ϣ
bool CAttemperEngineSink::OnDBUserInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	tagBindParameter * pBindParameter=GetBindParameter(LOWORD(dwContextID));
	if ((pBindParameter->dwSocketID!=dwContextID)||(pBindParameter->pIServerUserItem==NULL)) return true;

	//��ȡ�û�
	ASSERT_ERR(GetBindUserItem(LOWORD(dwContextID))!=NULL);
	IServerUserItem * pIServerUserItem=GetBindUserItem(LOWORD(dwContextID));

	//��������
	DBO_GR_UserInsureInfo * pUserInsureInfo=(DBO_GR_UserInsureInfo *)pData;

	//��������
	CMD_GR_S_UserInsureInfo UserInsureInfo;
	ZeroMemory(&UserInsureInfo,sizeof(UserInsureInfo));

	//��������
	UserInsureInfo.cbActivityGame=pUserInsureInfo->cbActivityGame;
	UserInsureInfo.wRevenueTake=pUserInsureInfo->wRevenueTake;
	UserInsureInfo.wRevenueTransfer=pUserInsureInfo->wRevenueTransfer;
	UserInsureInfo.wServerID=pUserInsureInfo->wServerID;
	UserInsureInfo.lUserInsure=pUserInsureInfo->lUserInsure;
	UserInsureInfo.lUserScore+=pIServerUserItem->GetUserScore();
	UserInsureInfo.lUserScore+=pIServerUserItem->GetTrusteeScore();
	UserInsureInfo.lUserScore+=pIServerUserItem->GetFrozenedScore();
	UserInsureInfo.lTransferPrerequisite=pUserInsureInfo->lTransferPrerequisite;

	//��������
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GR_INSURE,SUB_GR_USER_INSURE_INFO,&UserInsureInfo,sizeof(UserInsureInfo));

	return true;
}

//���гɹ�
bool CAttemperEngineSink::OnDBUserInsureSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	tagBindParameter * pBindParameter=GetBindParameter(LOWORD(dwContextID));
	if ((pBindParameter->dwSocketID!=dwContextID)||(pBindParameter->pIServerUserItem==NULL)) return true;

	//��ȡ�û�
	ASSERT_ERR(GetBindUserItem(LOWORD(dwContextID))!=NULL);
	IServerUserItem * pIServerUserItem=GetBindUserItem(LOWORD(dwContextID));

	//��������
	DBO_GR_UserInsureSuccess * pUserInsureSuccess=(DBO_GR_UserInsureSuccess *)pData;

	//��������
	SCORE lFrozenedScore=pUserInsureSuccess->lFrozenedScore;
	SCORE lInsureRevenue=pUserInsureSuccess->lInsureRevenue;
	SCORE lVariationScore=pUserInsureSuccess->lVariationScore;
	SCORE lVariationInsure=pUserInsureSuccess->lVariationInsure;

	//�ⶳ����
	if ((lFrozenedScore>0L)&&(pIServerUserItem->UnFrozenedUserScore(lFrozenedScore)==false))
	{
		ASSERT_ERR(FALSE);
		return false;
	}

	//���в���
	if (pIServerUserItem->ModifyUserInsure(lVariationScore,lVariationInsure,lInsureRevenue)==false)
	{
		ASSERT_ERR(FALSE);
		return false;
	}

	//��������
	CMD_GR_S_UserInsureSuccess UserInsureSuccess;
	ZeroMemory(&UserInsureSuccess,sizeof(UserInsureSuccess));

	//�������
	UserInsureSuccess.cbActivityGame=pUserInsureSuccess->cbActivityGame;
	UserInsureSuccess.lUserScore=pIServerUserItem->GetUserScore()+pIServerUserItem->GetTrusteeScore();
	UserInsureSuccess.lUserInsure=pUserInsureSuccess->lSourceInsure+pUserInsureSuccess->lVariationInsure;
	lstrcpyn(UserInsureSuccess.szDescribeString,pUserInsureSuccess->szDescribeString,CountArray(UserInsureSuccess.szDescribeString));

	//��������
	WORD wDescribe=CountStringBuffer(UserInsureSuccess.szDescribeString);
	WORD wHeadSize=sizeof(UserInsureSuccess)-sizeof(UserInsureSuccess.szDescribeString);
	SendData(pIServerUserItem,MDM_GR_INSURE,SUB_GR_USER_INSURE_SUCCESS,&UserInsureSuccess,wHeadSize+wDescribe);

	return true;
}

//����ʧ��
bool CAttemperEngineSink::OnDBUserInsureFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	tagBindParameter * pBindParameter=GetBindParameter(LOWORD(dwContextID));
	if ((pBindParameter->dwSocketID!=dwContextID)||(pBindParameter->pIServerUserItem==NULL)) return true;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwContextID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//���ʹ���
	DBO_GR_UserInsureFailure * pUserInsureFailure=(DBO_GR_UserInsureFailure *)pData;
	SendInsureFailure(pIServerUserItem,pUserInsureFailure->szDescribeString,pUserInsureFailure->lResultCode,pUserInsureFailure->cbActivityGame);

	//�ⶳ����
	if ((pUserInsureFailure->lFrozenedScore>0L)&&(pIServerUserItem->UnFrozenedUserScore(pUserInsureFailure->lFrozenedScore)==false))
	{
		ASSERT_ERR(FALSE);
		return false;
	}

	return true;
}

//�û���Ϣ
bool CAttemperEngineSink::OnDBUserInsureUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	tagBindParameter * pBindParameter=GetBindParameter(LOWORD(dwContextID));
	if ((pBindParameter->dwSocketID!=dwContextID)||(pBindParameter->pIServerUserItem==NULL)) return true;

	//��ȡ�û�
	ASSERT_ERR(GetBindUserItem(LOWORD(dwContextID))!=NULL);
	IServerUserItem * pIServerUserItem=GetBindUserItem(LOWORD(dwContextID));

	//��������
	DBO_GR_UserTransferUserInfo * pTransferUserInfo=(DBO_GR_UserTransferUserInfo *)pData;

	//��������
	CMD_GR_S_UserTransferUserInfo UserTransferUserInfo;
	ZeroMemory(&UserTransferUserInfo,sizeof(UserTransferUserInfo));

	//�������
	UserTransferUserInfo.cbActivityGame=pTransferUserInfo->cbActivityGame;
	UserTransferUserInfo.dwTargetGameID=pTransferUserInfo->dwGameID;
	lstrcpyn(UserTransferUserInfo.szNickName,pTransferUserInfo->szNickName,CountArray(UserTransferUserInfo.szNickName));

	//��������
	SendData(pIServerUserItem,MDM_GR_INSURE,SUB_GR_USER_TRANSFER_USER_INFO,&UserTransferUserInfo,sizeof(UserTransferUserInfo));

	return true;
}

//ϵͳ��Ϣ
bool CAttemperEngineSink::OnDBSystemMessage(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//����У��
	ASSERT_ERR(wDataSize==sizeof(DBR_GR_SystemMessage));
	if(wDataSize!=sizeof(DBR_GR_SystemMessage)) return false;

	//��ȡ����
	DBR_GR_SystemMessage * pSystemMessage = (DBR_GR_SystemMessage *)pData;
	if(pSystemMessage==NULL) return false;

	//�ظ��ж�
	POSITION pos = m_SystemMessageList.GetHeadPosition();
	while(pos != NULL)
	{
		POSITION tempPos = pos;
		tagSystemMessage *pTagSystemMessage = m_SystemMessageList.GetNext(pos);
		if(pTagSystemMessage->SystemMessage.cbMessageID == pSystemMessage->cbMessageID)
		{
			//��������
			pTagSystemMessage->dwLastTime=0;
			CopyMemory(&pTagSystemMessage->SystemMessage, pSystemMessage, sizeof(DBR_GR_SystemMessage));

			return true;
		}
	}

	//���ڱ���
	tagSystemMessage  *pSendMessage=new tagSystemMessage;
	ZeroMemory(pSendMessage, sizeof(tagSystemMessage));

	//���ñ���
	CopyMemory(&pSendMessage->SystemMessage,pSystemMessage, sizeof(DBR_GR_SystemMessage));

	//��¼��Ϣ
	m_SystemMessageList.AddTail(pSendMessage);

	return true;
}

//�������д�
bool CAttemperEngineSink::OnDBSensitiveWords(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//��ʼ����
	if(dwContextID==0xfffe)
	{
		m_WordsFilter.ResetSensitiveWordArray();
		m_pITimerEngine->KillTimer(IDI_LOAD_SENSITIVE_WORD);
		return true;			
	}

	//�������
	if(dwContextID==0xffff)
	{
		m_WordsFilter.FinishAdd();
		return true;
	}

	//�������д�
	const TCHAR *pWords=(const TCHAR*)pData;
	m_WordsFilter.AddSensitiveWords(pWords);
	return true;
}

//ע���¼�
bool CAttemperEngineSink::OnTCPSocketMainRegister(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_REGISTER_FAILURE:		//ע��ʧ��
		{
			//��������
			CMD_CS_S_RegisterFailure * pRegisterFailure=(CMD_CS_S_RegisterFailure *)pData;

			//Ч�����
			ASSERT_ERR(wDataSize>=(sizeof(CMD_CS_S_RegisterFailure)-sizeof(pRegisterFailure->szDescribeString)));
			if (wDataSize<(sizeof(CMD_CS_S_RegisterFailure)-sizeof(pRegisterFailure->szDescribeString))) return false;

			//�رմ���
			m_bNeekCorrespond=false;
			m_pITCPSocketService->CloseSocket();

			//��ʾ��Ϣ
			if (lstrlen(pRegisterFailure->szDescribeString)>0)
			{
				TRACE_LOG(pRegisterFailure->szDescribeString,TraceLevel_Exception);
			}

			//�¼�֪ͨ
			CP_ControlResult ControlResult;
			ControlResult.cbSuccess=ER_FAILURE;
			SendUIControlPacket(UI_CORRESPOND_RESULT,&ControlResult,sizeof(ControlResult));

			return true;
		}
	}

	return true;
}

//�б��¼�
bool CAttemperEngineSink::OnTCPSocketMainServiceInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_SERVER_INFO:		//������Ϣ
		{
			//�����б�
			m_ServerListManager.DisuseServerItem();

			return true;
		}
	case SUB_CS_S_SERVER_ONLINE:	//��������
		{
			//Ч�����
			ASSERT_ERR(wDataSize==sizeof(CMD_CS_S_ServerOnLine));
			if (wDataSize!=sizeof(CMD_CS_S_ServerOnLine)) return false;

			//��������
			CMD_CS_S_ServerOnLine * pServerOnLine=(CMD_CS_S_ServerOnLine *)pData;

			//���ҷ���
			CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(pServerOnLine->wServerID);

			//��������
			if (pGameServerItem!=NULL)
			{
				pGameServerItem->m_GameServer.dwOnLineCount=pServerOnLine->dwOnLineCount;
			}

			return true;
		}
	case SUB_CS_S_SERVER_INSERT:	//�������
		{
			//Ч�����
			ASSERT_ERR(wDataSize%sizeof(tagGameServer)==0);
			if (wDataSize%sizeof(tagGameServer)!=0) return false;

			//��������
			WORD wItemCount=wDataSize/sizeof(tagGameServer);
			tagGameServer * pGameServer=(tagGameServer *)pData;

			//��������
			for (WORD i=0;i<wItemCount;i++)
			{
				m_ServerListManager.InsertGameServer(pGameServer++);
			}

			return true;
		}
	case SUB_CS_S_SERVER_MODIFY:	//�����޸�
		{
			//Ч�����
			ASSERT_ERR(wDataSize==sizeof(CMD_CS_S_ServerModify));
			if (wDataSize!=sizeof(CMD_CS_S_ServerModify)) return false;

			//��������
			CMD_CS_S_ServerModify * pServerModify=(CMD_CS_S_ServerModify *)pData;

			//���ҷ���
			CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(pServerModify->wServerID);

			//���÷���
			if (pGameServerItem!=NULL)
			{
				pGameServerItem->m_GameServer.wNodeID=pServerModify->wNodeID;
				pGameServerItem->m_GameServer.wSortID=pServerModify->wSortID;
				pGameServerItem->m_GameServer.wServerPort=pServerModify->wServerPort;
				pGameServerItem->m_GameServer.dwOnLineCount=pServerModify->dwOnLineCount;
				pGameServerItem->m_GameServer.dwFullCount=pServerModify->dwFullCount;
				lstrcpyn(pGameServerItem->m_GameServer.szServerName,pServerModify->szServerName,CountArray(pGameServerItem->m_GameServer.szServerName));
				lstrcpyn(pGameServerItem->m_GameServer.szServerAddr,pServerModify->szServerAddr,CountArray(pGameServerItem->m_GameServer.szServerAddr));
			}

			return true;
		}
	case SUB_CS_S_SERVER_REMOVE:	//����ɾ��
		{
			//Ч�����
			ASSERT_ERR(wDataSize==sizeof(CMD_CS_S_ServerRemove));
			if (wDataSize!=sizeof(CMD_CS_S_ServerRemove)) return false;

			//��������
			CMD_CS_S_ServerRemove * pServerRemove=(CMD_CS_S_ServerRemove *)pData;

			//��������
			m_ServerListManager.DeleteGameServer(pServerRemove->wServerID);

			return true;
		}
	case SUB_CS_S_SERVER_FINISH:	//�������
		{
			//�����б�
			m_ServerListManager.CleanServerItem();

			//�¼�����
			CP_ControlResult ControlResult;
			ControlResult.cbSuccess=ER_SUCCESS;
			SendUIControlPacket(UI_CORRESPOND_RESULT,&ControlResult,sizeof(ControlResult));

			return true;
		}
	}

	return true;
}

//�����¼�
bool CAttemperEngineSink::OnTCPSocketMainUserCollect(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_COLLECT_REQUEST:	//�û�����
		{
			//��������
			CMD_CS_C_UserEnter UserEnter;
			ZeroMemory(&UserEnter,sizeof(UserEnter));

			//�����û�
			WORD wIndex=0;
			do
			{
				//��ȡ�û�
				IServerUserItem * pIServerUserItem=m_ServerUserManager.EnumUserItem(wIndex++);
				if (pIServerUserItem==NULL) break;

				//���ñ���
				UserEnter.dwUserID=pIServerUserItem->GetUserID();
				UserEnter.dwGameID=pIServerUserItem->GetGameID();
				lstrcpyn(UserEnter.szNickName,pIServerUserItem->GetNickName(),CountArray(UserEnter.szNickName));

				//������Ϣ
				UserEnter.cbGender=pIServerUserItem->GetGender();
				UserEnter.cbMemberOrder=pIServerUserItem->GetMemberOrder();
				UserEnter.cbMasterOrder=pIServerUserItem->GetMasterOrder();

				//��������
				ASSERT_ERR(m_pITCPSocketService!=NULL);
				m_pITCPSocketService->SendData(MDM_CS_USER_COLLECT,SUB_CS_C_USER_ENTER,&UserEnter,sizeof(UserEnter));

			} while (true);

			//�㱨���
			m_bCollectUser=true;
			m_pITCPSocketService->SendData(MDM_CS_USER_COLLECT,SUB_CS_C_USER_FINISH);

			return true;
		}
	}

	return true;
}

//�������
bool CAttemperEngineSink::OnTCPSocketMainManagerService(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_SYSTEM_MESSAGE:	//ϵͳ��Ϣ
		{
			//��Ϣ����
			SendSystemMessage((CMD_GR_SendMessage *)pData, wDataSize);

			return true;
		}
	case SUB_CS_S_PROPERTY_TRUMPET:  //������Ϣ
		{
			//��������
			m_pITCPNetworkEngine->SendDataBatch(MDM_GR_USER,SUB_GR_PROPERTY_TRUMPET,pData,wDataSize,BG_COMPUTER);

			return true;
		}
	case SUB_CS_S_GLAD_MESSAGE:		//ϲ����Ϣ
		{
			//D_TRACE(TEXT("��Ϸ�������յ�ϲ����Ϣ"));

			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_S_SendGladMsg));
			if (wDataSize!=sizeof(CMD_CS_S_SendGladMsg)) return false;

			//��������
			CMD_CS_S_SendGladMsg * pGladMsg=(CMD_CS_S_SendGladMsg *)pData;

			CMD_GR_SendGladMsg gladMsg;
			CopyMemory(&gladMsg, pGladMsg, sizeof(gladMsg));
			//��������
			m_pITCPNetworkEngine->SendDataBatch(MDM_GR_USER,SUB_GR_GLAD_MESSAGE,&gladMsg, sizeof(gladMsg),0);

			return true;
		}
	}

	return true;
}

//��¼����
bool CAttemperEngineSink::OnTCPNetworkMainLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GR_LOGON_USERID:		//I D ��¼
		{
			return OnTCPNetworkSubLogonUserID(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_LOGON_MOBILE:		//�ֻ���¼
		{
			return OnTCPNetworkSubLogonMobile(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_LOGON_ACCOUNTS:		//�ʺŵ�¼
		{
			return OnTCPNetworkSubLogonAccounts(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_LOGON_TOKEN:
		{
			return OnTCPNetworkSubLogonTokenID(pData, wDataSize, dwSocketID);
		}
	case SUB_GR_LOGON_TOKEN_LUA:
		{
			return OnTCPNetworkSubLogonTokenIDByLua(pData, wDataSize, dwSocketID);
		}
	}

	return true;
}

//�û�����
bool CAttemperEngineSink::OnTCPNetworkMainUser(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GR_USER_LOOKON:		//�û��Թ�
		{
			return OnTCPNetworkSubUserLookon(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_USER_SITDOWN:		//�û�����
		{
			return OnTCPNetworkSubUserSitDown(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_USER_SITDOWN_H5:   //H5����
		{
			return OnTCPNetworkSubUserSitDownH5(pData, wDataSize, dwSocketID);
		}
	case SUB_GR_USER_STANDUP:		//�û�����
		{
			return OnTCPNetworkSubUserStandUp(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_USER_STANDUP_H5:	//H5����
		{
			return OnTCPNetworkSubUserStandUpH5(pData, wDataSize, dwSocketID);
		}
	case SUB_GR_USER_CHAIR_REQ:    //�û�����
		{
			return OnTCPNetworkSubUserChangeTable(pData, wDataSize, dwSocketID);
		}
	case SUB_GR_USER_CHANGE_TABLE_H5:  //H5����
		{
			return OnTCPNetworkSubUserChangeTableH5(pData, wDataSize, dwSocketID);
		}
	case SUB_GR_USER_CHAIR_INFO_REQ: //���������û���Ϣ
		{
			return OnTCPNetworkSubChairUserInfoReq(pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//���д���
bool CAttemperEngineSink::OnTCPNetworkMainInsure(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GR_QUERY_INSURE_INFO:		//���в�ѯ
		{
			return OnTCPNetworkSubQueryInsureInfo(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_SAVE_SCORE_REQUEST:		//�������
		{
			return OnTCPNetworkSubSaveScoreRequest(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_TAKE_SCORE_REQUEST:		//ȡ������
		{
			return OnTCPNetworkSubTakeScoreRequest(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_TRANSFER_SCORE_REQUEST:	//ת������
		{
			return OnTCPNetworkSubTransferScoreRequest(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_QUERY_USER_INFO_REQUEST:	//��ѯ�û�
		{
			return OnTCPNetworkSubQueryUserInfoRequest(pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//������
bool CAttemperEngineSink::OnTCPNetworkMainManage(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GR_QUERY_OPTION:		//��ѯ����
		{
			return OnTCPNetworkSubQueryOption(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_OPTION_SERVER:		//��������
		{
			return OnTCPNetworkSubOptionServer(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_KILL_USER:          //�߳��û�
		{
			return OnTCPNetworkSubManagerKickUser(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_LIMIT_USER_CHAT:	//��������
		{
			return OnTCPNetworkSubLimitUserChat(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_KICK_ALL_USER:		//�߳��û�
		{
			return OnTCPNetworkSubKickAllUser(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_SEND_MESSAGE:		//������Ϣ
		{
			return OnTCPNetworkSubSendMessage(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_DISMISSGAME:        //��ɢ��Ϸ
		{
			return OnTCPNetworkSubDismissGame(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_SEND_WARNING:		//���;���
		{
			return OnTCPNetworkSubSendWarning(pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//��������
bool CAttemperEngineSink::OnTCPNetworkMainMatch(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//��ȡ��Ϣ
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//Ч��ӿ�
	ASSERT_ERR(m_pIGameMatchServiceManager!=NULL);
	if (m_pIGameMatchServiceManager==NULL) return false;

	//��Ϣ����
	return m_pIGameMatchServiceManager->OnEventSocketMatch(wSubCmdID,pData,wDataSize,pIServerUserItem,dwSocketID);
}

//��Ϸ����
bool CAttemperEngineSink::OnTCPNetworkMainGame(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//��ȡ��Ϣ
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//�������
	WORD wTableID=pIServerUserItem->GetTableID();
	WORD wChairID=pIServerUserItem->GetChairID();
	if ((wTableID==INVALID_TABLE)||(wChairID==INVALID_CHAIR)) return true;

	//��Ϣ���� 
	CTableFrame * pTableFrame=m_TableFrameArray[wTableID];
	if (pTableFrame) {
		bool bResult = pTableFrame->OnEventSocketGame(wSubCmdID, pData, wDataSize, pIServerUserItem);
		if ( bResult == false )
			I_TRACE(TEXT("��Ϸ��Ϣʧ�� SubCmdID:%d,TableID:%d"), wSubCmdID, wTableID);
		return bResult;
	}
	return false;
}

//��ܴ���
bool CAttemperEngineSink::OnTCPNetworkMainFrame(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//��ȡ��Ϣ
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//�������
	WORD wTableID=pIServerUserItem->GetTableID();
	WORD wChairID=pIServerUserItem->GetChairID();
	if ((wTableID==INVALID_TABLE)||(wChairID==INVALID_CHAIR)) return true;

	//��Ϣ���� 
	CTableFrame * pTableFrame=m_TableFrameArray[wTableID];
	if ( pTableFrame )
	{
		bool bResult = pTableFrame->OnEventSocketFrame(wSubCmdID, pData, wDataSize, pIServerUserItem);
		if (bResult == false)
			I_TRACE(TEXT("�����Ϣʧ�� SubCmdID:%d,TableID:%d"), wSubCmdID, wTableID);
		return bResult;
	}
	return false;
}

//I D ��¼ �����˵�¼
bool CAttemperEngineSink::OnTCPNetworkSubLogonUserID(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT_ERR(wDataSize>=sizeof(CMD_GR_LogonUserID));
	if (wDataSize<sizeof(CMD_GR_LogonUserID)) return false;

	//������Ϣ
	CMD_GR_LogonUserID * pLogonUserID=(CMD_GR_LogonUserID *)pData;
	pLogonUserID->szPassword[CountArray(pLogonUserID->szPassword)-1]=0;
	pLogonUserID->szMachineID[CountArray(pLogonUserID->szMachineID)-1]=0;

	//����Ϣ
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIBindUserItem=GetBindUserItem(wBindIndex);
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	//�ظ��ж�
	if ((pBindParameter==NULL)||(pIBindUserItem!=NULL))
	{ 
		ASSERT_ERR(FALSE);
		return false;
	}

	//�����ж�
	if(pLogonUserID->wKindID != m_pGameServiceOption->wKindID)
	{
		//����ʧ��
		SendLogonFailure(TEXT("�ܱ�Ǹ������Ϸ�����Ѿ��ر��ˣ�������������룡"),LOGON_FAIL_SERVER_INVALIDATION,dwSocketID);
		return true;
	}

	////�����˺����˲�����
	//IServerUserItem * pIServerUserItem=m_ServerUserManager.SearchUserItem(pLogonUserID->dwUserID);
	//if (pIServerUserItem!=NULL)
	//{
	//	if((pIServerUserItem->IsAndroidUser() && (pBindParameter->dwClientAddr!=0L))
	//		|| (!pIServerUserItem->IsAndroidUser() && (pBindParameter->dwClientAddr==0L)))
	//	{
	//		SendRoomMessage(dwSocketID, TEXT("���˺����ڴ˷�����Ϸ���Ҳ������߳�������ѯ����Ա��"), SMT_CHAT|SMT_EJECT|SMT_GLOBAL|SMT_CLOSE_ROOM,(pBindParameter->dwClientAddr==0L));
	//		return true;
	//	}
	//}

	//�汾��Ϣ
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;

	//�л��ж�
// 	if((pIServerUserItem!=NULL)&&(pIServerUserItem->ContrastLogonPass(pLogonUserID->szPassword)==true))
// 	{
// 		SwitchUserItemConnect(pIServerUserItem,pLogonUserID->szMachineID,wBindIndex);
// 		return true;
// 	}

	//��������
	DBR_GR_LogonUserID LogonUserID;
	ZeroMemory(&LogonUserID,sizeof(LogonUserID));

	//��������
	LogonUserID.dwUserID=pLogonUserID->dwUserID;
	LogonUserID.dwClientAddr=pBindParameter->dwClientAddr;
	lstrcpyn(LogonUserID.szPassword,pLogonUserID->szPassword,CountArray(LogonUserID.szPassword));
	lstrcpyn(LogonUserID.szMachineID,pLogonUserID->szMachineID,CountArray(LogonUserID.szMachineID));

	//Ͷ������
	m_pIDBCorrespondManager->PostDataBaseRequest(LogonUserID.dwUserID, DBR_GR_LOGON_USERID, dwSocketID, &LogonUserID, sizeof(LogonUserID));

	return true;
}

//�ֻ���¼
bool CAttemperEngineSink::OnTCPNetworkSubLogonMobile(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT_ERR(wDataSize >= sizeof(CMD_GR_LogonMobile));
	if (wDataSize < sizeof(CMD_GR_LogonMobile)) return false;

	//������Ϣ
	CMD_GR_LogonMobile * pLogonUserID = (CMD_GR_LogonMobile*)pData;
	pLogonUserID->szPassword[CountArray(pLogonUserID->szPassword) - 1] = 0;
	pLogonUserID->szMachineID[CountArray(pLogonUserID->szMachineID) - 1] = 0;

	//����Ϣ
	WORD wBindIndex = LOWORD(dwSocketID);
	IServerUserItem * pIBindUserItem = GetBindUserItem(wBindIndex);
	tagBindParameter * pBindParameter = GetBindParameter(wBindIndex);

	if (pBindParameter == NULL)
	{
		ASSERT_ERR(FALSE);
		return false;
	}

	if ( pIBindUserItem != NULL)
	{
		return false;
	}
	//�ظ��ж�
// 	if ((pBindParameter == NULL) || (pIBindUserItem != NULL))
// 	{
// 		ASSERT_ERR(FALSE);
// 		return false;
// 	}

	//�汾��Ϣ
	pBindParameter->cbClientKind = CLIENT_KIND_MOBILE;

// 	IServerUserItem * pIServerUserItem = m_ServerUserManager.SearchUserItem(pLogonUserID->dwUserID);
// 	//�л��ж�
// 	if ((pIServerUserItem != NULL) && (pIServerUserItem->ContrastLogonPass(pLogonUserID->szPassword) == true))
// 	{
// 		SwitchUserItemConnect(pIServerUserItem, pLogonUserID->szMachineID, wBindIndex);
// 		return true;
// 	}

	//��������
	DBR_GR_LogonUserID LogonUserID;
	ZeroMemory(&LogonUserID, sizeof(LogonUserID));

	//��������
	LogonUserID.dwUserID = pLogonUserID->dwUserID;
	LogonUserID.dwClientAddr = pBindParameter->dwClientAddr;
	lstrcpyn(LogonUserID.szPassword, pLogonUserID->szPassword, CountArray(LogonUserID.szPassword));
	lstrcpyn(LogonUserID.szMachineID, pLogonUserID->szMachineID, CountArray(LogonUserID.szMachineID));

	//Ͷ������
	m_pIDBCorrespondManager->PostDataBaseRequest(LogonUserID.dwUserID, DBR_GR_LOGON_USERID, dwSocketID, &LogonUserID, sizeof(LogonUserID));
	
	g_Log.LogToFile(g_szLogRoot,TEXT("\n OnTCPNetworkSubLogonMobile UserID[%d]"),LogonUserID.dwUserID);
	return true;
}

bool CAttemperEngineSink::OnTCPNetworkSubLogonTokenIDByLua(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT_ERR(wDataSize >= sizeof(CMD_GR_LogonTokenID));
	if (wDataSize < sizeof(CMD_GR_LogonTokenID)) return false;

	//������Ϣ
	CMD_GR_LogonTokenID * pLogonTokenID = (CMD_GR_LogonTokenID*)pData;
	pLogonTokenID->szTokenID[CountArray(pLogonTokenID->szTokenID) - 1] = 0;
	pLogonTokenID->szMachineID[CountArray(pLogonTokenID->szMachineID) - 1] = 0;

	//����Ϣ
	WORD wBindIndex = LOWORD(dwSocketID);
	IServerUserItem * pIBindUserItem = GetBindUserItem(wBindIndex);
	tagBindParameter * pBindParameter = GetBindParameter(wBindIndex);

	if (pBindParameter == NULL)
	{
		ASSERT_ERR(FALSE);
		return false;
	}

	if (pIBindUserItem != NULL)
	{
		return false;
	}


	//�汾��Ϣ
	pBindParameter->cbClientKind = CLIENT_KIND_MOBILE;

	//��������
	DBR_GR_LogonTokenID LogonTokenID;
	ZeroMemory(&LogonTokenID, sizeof(LogonTokenID));

	//��������
	LogonTokenID.dwUserID = pLogonTokenID->dwUserID;
	LogonTokenID.dwClientAddr = pBindParameter->dwClientAddr;
	LogonTokenID.cbDeviceType = pLogonTokenID->cbDeviceType;
	LogonTokenID.dwMerchantID = pLogonTokenID->dwMerchantID;
	lstrcpyn(LogonTokenID.szTokenID, pLogonTokenID->szTokenID, CountArray(LogonTokenID.szTokenID));
	lstrcpyn(LogonTokenID.szMachineID, pLogonTokenID->szMachineID, CountArray(LogonTokenID.szMachineID));
	lstrcpyn(LogonTokenID.szClientIP, pLogonTokenID->szClientIP, CountArray(LogonTokenID.szClientIP));

	//Ͷ������
	m_pIDBCorrespondManager->PostDataBaseRequest(LogonTokenID.dwUserID, DBR_GR_LOGON_MOBILE, dwSocketID, &LogonTokenID, sizeof(LogonTokenID));

	I_TRACE(TEXT("OnTCPNetworkSubLogonTokenIDByLua %d"), LogonTokenID.dwUserID);
	g_Log.LogToFile(g_szLogRoot, TEXT("\n OnTCPNetworkSubLogonTokenIDByLua UserID[%d]"), LogonTokenID.dwUserID);
	return true;
}

//Token��¼
bool CAttemperEngineSink::OnTCPNetworkSubLogonTokenID(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//����Ϣ
	WORD wBindIndex = LOWORD(dwSocketID);
	IServerUserItem * pIBindUserItem = GetBindUserItem(wBindIndex);
	tagBindParameter * pBindParameter = GetBindParameter(wBindIndex);

	if (pBindParameter == NULL)
	{
		ASSERT_ERR(FALSE);
		return false;
	}

	if (pIBindUserItem != NULL)
	{
		return false;
	}
	I_TRACE(TEXT("------------------�����¼SocketID:%d--------------------------"),dwSocketID);
	//�汾��Ϣ
	pBindParameter->cbClientKind = CLIENT_KIND_WEBSOCKET;

	//��������
	DBR_GR_LogonTokenID LogonTokenID;
	ZeroMemory(&LogonTokenID, sizeof(LogonTokenID));



	GameServer::GSLogonTokenID gsLogonTokenID;
	gsLogonTokenID.ParseFromArray(pData, wDataSize);

	//��������
	LogonTokenID.cbDeviceType = gsLogonTokenID.devicetype();
	LogonTokenID.dwClientAddr = pBindParameter->dwClientAddr;;
	LogonTokenID.dwMerchantID = gsLogonTokenID.merchantid();
	LogonTokenID.dwUserID = gsLogonTokenID.userid();

	lstrcpyn(LogonTokenID.szClientIP, Utils::ConvertA2W(gsLogonTokenID.clientip()).c_str(), CountArray(LogonTokenID.szClientIP));
	lstrcpyn(LogonTokenID.szMachineID, Utils::ConvertA2W(gsLogonTokenID.machineid()).c_str(), CountArray(LogonTokenID.szMachineID));
	lstrcpyn(LogonTokenID.szTokenID, Utils::ConvertA2W(gsLogonTokenID.tokenid()).c_str(), CountArray(LogonTokenID.szTokenID));

	//Ͷ������
	m_pIDBCorrespondManager->PostDataBaseRequest(LogonTokenID.dwUserID, DBR_GR_LOGON_TOKENID, dwSocketID, &LogonTokenID, sizeof(LogonTokenID));

	I_TRACE(TEXT("OnTCPNetworkSubLogonTokenID %d"), LogonTokenID.dwUserID);
	//g_Log.LogToFile(g_szLogRoot, TEXT("\n UserID[%d]-[�����¼]-SocketID[%d]"), LogonTokenID.dwUserID, dwSocketID);
	CString cs;
	cs.Format(TEXT("��������[1],������[4]��|����¼���û�[%d],BindIndex[%d] SocketID[%d],IP[%d]"), LogonTokenID.dwUserID, wBindIndex, dwSocketID, LogonTokenID.dwClientAddr);
	m_UserFileLog.Log(cs.GetBuffer(0));
	return true;
}

//�ʺŵ�¼
bool CAttemperEngineSink::OnTCPNetworkSubLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT_ERR(wDataSize>=sizeof(CMD_GR_LogonAccounts));
	if (wDataSize<sizeof(CMD_GR_LogonAccounts)) return false;

	//������Ϣ
	CMD_GR_LogonAccounts * pLogonAccounts=(CMD_GR_LogonAccounts *)pData;
	pLogonAccounts->szPassword[CountArray(pLogonAccounts->szPassword)-1]=0;
	pLogonAccounts->szAccounts[CountArray(pLogonAccounts->szAccounts)-1]=0;
	pLogonAccounts->szMachineID[CountArray(pLogonAccounts->szMachineID)-1]=0;

	//����Ϣ
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIBindUserItem=GetBindUserItem(wBindIndex);
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	//�ظ��ж�
	if (pIBindUserItem!=NULL)
	{ 
		ASSERT_ERR(FALSE);
		return false;
	}

	////�����˺����˲�����
	//IServerUserItem * pIServerUserItem=m_ServerUserManager.SearchUserItem(pLogonAccounts->szAccounts);
	//if (pIServerUserItem!=NULL)
	//{
	//	if((pIServerUserItem->IsAndroidUser() && (pBindParameter->dwClientAddr!=0L))
	//		|| (!pIServerUserItem->IsAndroidUser() && (pBindParameter->dwClientAddr==0L)))
	//	{
	//		SendRoomMessage(dwSocketID, TEXT("���˺����ڴ˷�����Ϸ���Ҳ������߳�������ѯ����Ա��"), SMT_CHAT|SMT_EJECT|SMT_GLOBAL|SMT_CLOSE_ROOM,(pBindParameter->dwClientAddr==0L));
	//		return false;
	//	}
	//}

	//�汾��Ϣ
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;

	//�л��ж�
// 	if ((pIServerUserItem!=NULL)&&(pIServerUserItem->ContrastLogonPass(pLogonAccounts->szPassword)==true))
// 	{
// 		SwitchUserItemConnect(pIServerUserItem,pLogonAccounts->szMachineID,wBindIndex);
// 		return true;
// 	}

	//��������
	DBR_GR_LogonAccounts LogonAccounts;
	ZeroMemory(&LogonAccounts,sizeof(LogonAccounts));

	//��������
	LogonAccounts.dwClientAddr=pBindParameter->dwClientAddr;
	lstrcpyn(LogonAccounts.szAccounts,pLogonAccounts->szAccounts,CountArray(LogonAccounts.szAccounts));
	lstrcpyn(LogonAccounts.szPassword,pLogonAccounts->szPassword,CountArray(LogonAccounts.szPassword));
	lstrcpyn(LogonAccounts.szMachineID,pLogonAccounts->szMachineID,CountArray(LogonAccounts.szMachineID));

	//Ͷ������
	//m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOGON_ACCOUNTS,dwSocketID,&LogonAccounts,sizeof(LogonAccounts));
	m_pIKernelDataBaseEngine->PostDataBaseRequest(DBR_GR_LOGON_ACCOUNTS,dwSocketID,&LogonAccounts,sizeof(LogonAccounts));

	g_Log.LogToFile(g_szLogRoot,TEXT("\n OnTCPNetworkSubLogonAccounts Account[%d]"),LogonAccounts.szAccounts);
	return true;
}

//�û��Թ�
bool CAttemperEngineSink::OnTCPNetworkSubUserLookon(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	return true;
}

//�û�����
bool CAttemperEngineSink::OnTCPNetworkSubUserSitDownH5(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//��ȡ�û�
	WORD wBindIndex = LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem = GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT_ERR(pIServerUserItem != NULL);
	if (pIServerUserItem == NULL) return false;

	CString cs;
	cs.Format(TEXT("��������[3],������[15]��|�����¡��û�[%d][%s],BindIndex[%d],SocketID[%d]"),pIServerUserItem->GetUserID(),pIServerUserItem->GetNickName(), wBindIndex, wBindIndex);
	m_UserFileLog.Log(cs.GetBuffer(0));

	// lchq 2019 10 24 ��������
	if (pIServerUserItem->GetTableID() != INVALID_TABLE&&pIServerUserItem->GetChairID() != INVALID_CHAIR)
	{
		if (pIServerUserItem->GetUserStatus() >= US_PLAYING && m_pGameServiceOption->wKindID != KIND_FISH)
		{
			//��������
			WORD wTableID = pIServerUserItem->GetTableID();
			WORD wChairID = pIServerUserItem->GetChairID();

			pIServerUserItem->SetHeartTime((DWORD)time(NULL));
			I_TRACE(TEXT("������� %s %d-%d Status:%d"), pIServerUserItem->GetNickName(), pIServerUserItem->GetTableID(), pIServerUserItem->GetChairID(), pIServerUserItem->GetUserStatus());
			
			pIServerUserItem->SetEnterTable(FALSE);
			pIServerUserItem->SetUserStatus(US_PLAYING, wTableID, wChairID);
			return true;
		}
	}

	GameServer::GSUserSitDown gsUserSitDown;
	gsUserSitDown.ParseFromArray(pData, wDataSize);

	CMD_GR_UserSitDown UserSitDown = { 0 };
	UserSitDown.wTableID = gsUserSitDown.tableid();
	UserSitDown.wChairID = gsUserSitDown.chairid();
	lstrcpyn(UserSitDown.szPassword, Utils::ConvertA2W(gsUserSitDown.tabletokenid()).c_str(), CountArray(UserSitDown.szPassword));

	//��ҵ�ǰ��Ϣ
	WORD wTableID = pIServerUserItem->GetTableID();
	WORD wChairID = pIServerUserItem->GetChairID();
	BYTE cbUserStatus = pIServerUserItem->GetUserStatus();

	//�ظ��ж�
	if ((UserSitDown.wTableID < m_pGameServiceOption->wTableCount) && (UserSitDown.wChairID < m_pGameServiceAttrib->wChairCount))
	{
		CTableFrame * pTableFrame = m_TableFrameArray[UserSitDown.wTableID];
		if (pTableFrame->GetTableUserItem(UserSitDown.wChairID) == pIServerUserItem) return true;
	}

	//�û��ж�
	if (cbUserStatus == US_PLAYING)
	{
		if ( m_pGameServiceOption->wKindID != KIND_FISH)
			SendRequestFailureH5(pIServerUserItem, TEXT("��������Ϸ�У���ʱ�����뿪�����Ƚ�����ǰ��Ϸ��"), REQUEST_FAILURE_NORMAL);
		return true;
	}

	//�Ѿ�
	if (wTableID != INVALID_TABLE)
	{
		CTableFrame * pTableFrame = m_TableFrameArray[wTableID];
		if (pTableFrame != NULL && pTableFrame->PerformStandUpActionH5(pIServerUserItem) == false) return true;
	}

	//�������
	WORD wRequestTableID = UserSitDown.wTableID;
	WORD wRequestChairID = UserSitDown.wChairID;

	//��̬����
	bool bDynamicJoin = true;
	if (m_pGameServiceAttrib->cbDynamicJoin == FALSE) bDynamicJoin = false;
	if (CServerRule::IsAllowDynamicJoin(m_pGameServiceOption->dwServerRule) == false) bDynamicJoin = false;

	//ϵͳ����
	if (wRequestTableID >= m_TableFrameArray.GetCount())
	{
		//��ʼ����
		WORD wStartTableID = rand() % m_pGameServiceOption->wTableCount;
		//��ʱ�����
		//wStartTableID = 0;
		//Ѱ��λ��
		for (WORD i = wStartTableID; i<m_TableFrameArray.GetCount() + wStartTableID; i++)
		{
			WORD wStartID = 0;
			if (m_pGameServiceOption->wKindID != KIND_FISH)
				wStartID = i % m_pGameServiceOption->wTableCount;

			//��Ϸ״̬
			if ((m_TableFrameArray[wStartID]->IsGameStarted() == true) && (bDynamicJoin == false))continue;

			if (m_pGameServiceOption->wKindID != KIND_FISH)
			{
				tagTableUserInfo userInfo = { 0 };
				m_TableFrameArray[wStartID]->GetTableUserInfo(userInfo);

				//Ѱ�ҿ�λ������
				if (userInfo.wTableUserCount > 0) continue;
			}		

			//��ȡ��λ
			WORD wNullChairID = m_TableFrameArray[wStartID]->GetNullChairID();

			//�������
			if (wNullChairID != INVALID_CHAIR)
			{
				wRequestTableID = wStartID;
				wRequestChairID = wNullChairID;

				break;
			}
		}

		//����ж�
		if ((wRequestTableID == INVALID_CHAIR) || (wRequestChairID == INVALID_CHAIR))
		{
			SendRequestFailureH5(pIServerUserItem, TEXT("��ǰ��Ϸ�����Ѿ�����Ϊ���ˣ���ʱû�п������������λ�ã����Ժ����ԣ�"), REQUEST_FAILURE_QUIT);
			return true;
		}
	}

	//���ӵ���
	if (wRequestChairID >= m_pGameServiceAttrib->wChairCount)
	{
		//Ч�����
		ASSERT_ERR(wRequestTableID<m_TableFrameArray.GetCount());
		if (wRequestTableID >= m_TableFrameArray.GetCount()) return false;

		//���ҿ�λ
		wRequestChairID = m_TableFrameArray[wRequestTableID]->GetNullChairID();

		//����ж�
		if (wRequestChairID == INVALID_CHAIR)
		{
			SendRequestFailureH5(pIServerUserItem, TEXT("���ڴ���Ϸ����ʱû�п������������λ���ˣ���ѡ���������Ϸ����"), REQUEST_FAILURE_QUIT);
			return true;
		}
	}

	//���´���
	CTableFrame * pTableFrame = m_TableFrameArray[wRequestTableID];
	if (pTableFrame != NULL)
	{
		bool bResult = false;
		bResult = pTableFrame->PerformSitDownActionH5(wRequestChairID, pIServerUserItem, UserSitDown.szPassword);
		//�����������
		if (bResult) {
			I_TRACE(TEXT("H5���� nickName:%s,TableID:%d,ChairID:%d"),pIServerUserItem->GetNickName(),pIServerUserItem->GetTableID(),pIServerUserItem->GetChairID());
		}
	}

	return true;
}

//�û�����
bool CAttemperEngineSink::OnTCPNetworkSubUserSitDown(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{

	//Ч�����
	ASSERT_ERR(wDataSize==sizeof(CMD_GR_UserSitDown));
	if (wDataSize!=sizeof(CMD_GR_UserSitDown)) return false;

	//Ч������
	CMD_GR_UserSitDown * pUserSitDown=(CMD_GR_UserSitDown *)pData;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;


	//��Ϣ����
	WORD wTableID=pIServerUserItem->GetTableID();
	WORD wChairID=pIServerUserItem->GetChairID();
	BYTE cbUserStatus=pIServerUserItem->GetUserStatus();

	//�ظ��ж�
	if ((pUserSitDown->wTableID<m_pGameServiceOption->wTableCount)&&(pUserSitDown->wChairID<m_pGameServiceAttrib->wChairCount))
	{
		CTableFrame * pTableFrame=m_TableFrameArray[pUserSitDown->wTableID];
		if (pTableFrame->GetTableUserItem(pUserSitDown->wChairID)==pIServerUserItem) return true;
	}

	//�û��ж�
	if (cbUserStatus==US_PLAYING)
	{
		SendRequestFailure(pIServerUserItem,TEXT("��������Ϸ�У���ʱ�����뿪�����Ƚ�����ǰ��Ϸ��"), REQUEST_FAILURE_NORMAL);
		return true;
	}

	//�뿪����
	if (wTableID!=INVALID_TABLE)
	{
		CTableFrame * pTableFrame=m_TableFrameArray[wTableID];
		if (pTableFrame != NULL && pTableFrame->PerformStandUpAction(pIServerUserItem)==false) return true;
	}

// 	if (CServerRule::IsAllowAvertCheatMode(m_pGameServiceOption->dwServerRule) && (m_pGameServiceAttrib->wChairCount < MAX_CHAIR))
// 	{
// 		I_TRACE(TEXT("�������"));
// 		//�������
// 		InsertDistribute(pIServerUserItem);
// 		return true;
// 	}

	{
		CString szFileName;
		szFileName.Format(TEXT("%s/Settings/RoomID/UserID/%d.ini"),m_szPath,pIServerUserItem->GetUserID());

		DWORD dwLimitSitDown = 0;
		dwLimitSitDown	= GetPrivateProfileInt(TEXT("Parameter"),TEXT("LimitSitDown"),0,(LPCTSTR)szFileName);

		if ( dwLimitSitDown !=0 )
		{
			SendRequestFailure(pIServerUserItem,TEXT("��ǰ��Ϸ�����Ѿ�����Ϊ���ˣ���ʱû�п������������λ�ã����Ժ����ԣ�"), REQUEST_FAILURE_QUIT);
			return true;
		}
	}

	//�������
	WORD wRequestTableID=pUserSitDown->wTableID;
	WORD wRequestChairID=pUserSitDown->wChairID;

	//��̬����
	bool bDynamicJoin=true;
	if (m_pGameServiceAttrib->cbDynamicJoin==FALSE) bDynamicJoin=false;
	if (CServerRule::IsAllowDynamicJoin(m_pGameServiceOption->dwServerRule)==false) bDynamicJoin=false;

	//gm��׷�ٺ�������� gm�����£�����������������Ӻ�
// 	{
// 		if (CUserRight::CanKillOutUser(pIServerUserItem->GetUserRight())==true)
// 		{
// 			WORD wIndex=0;
// 			WORD wTableID=INVALID_TABLE;
// 			WORD wNullChairID=INVALID_CHAIR;
// 			BYTE cbStopSearch = 0;
// 			do
// 			{
// 				IServerUserItem * pIUserItem=m_ServerUserManager.EnumUserItem(wIndex++);
// 				if (pIUserItem==NULL) break;
// 				//�ȶ�λ���ӣ��ٶ�λ�����Ƿ�����
// 				if (CUserRight::IsGameCheatUser(pIUserItem->GetUserRight())==true)
// 					wTableID = pIUserItem->GetTableID();
// 				if ( wTableID <= m_TableFrameArray.GetCount())
// 					wNullChairID=m_TableFrameArray[wTableID]->GetNullChairID();
// 				if ( wNullChairID != INVALID_CHAIR ) //�п�����
// 				{
// 					wRequestTableID = wTableID;
// 					wRequestChairID = wNullChairID;
// 					CTableFrame * pTableFrame=m_TableFrameArray[wRequestTableID];
// 					if ( pTableFrame != NULL )
// 						pTableFrame->PerformSitDownAction(wRequestChairID,pIServerUserItem,pUserSitDown->szPassword);
// 					
// 					//�Ѿ��ҵ����������ֹͣ����
// 					cbStopSearch = 1;
// 					break;
// 				}
// 
// 			} while (true);
// 			if ( cbStopSearch )
// 				return true;
// 		}
// 	}

	//���ӵ���
	if (wRequestTableID>=m_TableFrameArray.GetCount())
	{
		//��ʼ����
		WORD wStartTableID= rand() % m_pGameServiceOption->wTableCount;
		wStartTableID = 0;
		//DWORD dwServerRule=m_pGameServiceOption->dwServerRule;
		//if ((CServerRule::IsAllowAvertCheatMode(dwServerRule)==true)&&(m_pGameServiceAttrib->wChairCount<MAX_CHAIR)) wStartTableID=1;
		//if ((CServerRule::IsAllowAvertCheatMode(dwServerRule)==true)&&(m_pGameServiceAttrib->wChairCount>=MAX_CHAIR)) wStartTableID=0;

		//Ѱ��λ��
		for (WORD i=wStartTableID;i<m_TableFrameArray.GetCount()+wStartTableID;i++)
		{
			WORD wStartID = 0;
			wStartID = i % m_pGameServiceOption->wTableCount;
			//��Ϸ״̬
			if ((m_TableFrameArray[wStartID]->IsGameStarted()==true)&&(bDynamicJoin==false))continue;

			//��ȡ��λ
			WORD wNullChairID=m_TableFrameArray[wStartID]->GetNullChairID();

			//�������
			if (wNullChairID!=INVALID_CHAIR)
			{
				bool bLimitSameIP = false;
				/*
				if ( m_pGameServiceOption->wRoomType != 4 && CUserRight::CanKillOutUser(pIServerUserItem->GetUserRight())==false )
				{
					CString szFileName;
					szFileName.Format(TEXT("%s/Settings/RoomID/%d.ini"),m_szPath,m_pGameServiceOption->dwRoomID);

					DWORD dwAllowSameIP = 0;
					dwAllowSameIP	= GetPrivateProfileInt(TEXT("Parameter"),TEXT("AllowSameIP"),0,(LPCTSTR)szFileName);


					//�ж������������IP�Ƿ���ͬ
					//dwAllowSameIP=0 һ������  1������
					if (  dwAllowSameIP==1)
					{
						DWORD dwUserIP=pIServerUserItem->GetClientAddr();
						for( WORD j=0;j<m_pGameServiceAttrib->wChairCount;++j )
						{
							CTableFrame * pTableFrame=m_TableFrameArray[i];
							IServerUserItem * pITableUserItem=pTableFrame->GetTableUserItem(j);
							if ((pITableUserItem!=NULL)&&(pITableUserItem != pIServerUserItem)&&(!pITableUserItem->IsAndroidUser())&&(pITableUserItem->GetMasterOrder()==0)&&(pITableUserItem->GetClientAddr()==dwUserIP))
							{
								//D_TRACE(TEXT("����������ͬ"));
								bLimitSameIP = true;
								break;
							}
						}
					}

				}

				//IPͬ����ʶ
				if ( bLimitSameIP ) continue;
				*/
				wRequestTableID= wStartID;
				wRequestChairID=wNullChairID;

				break;
			}
		}

		//����ж�
		if ((wRequestTableID==INVALID_CHAIR)||(wRequestChairID==INVALID_CHAIR))
		{
			SendRequestFailure(pIServerUserItem,TEXT("��ǰ��Ϸ�����Ѿ�����Ϊ���ˣ���ʱû�п������������λ�ã����Ժ����ԣ�"), REQUEST_FAILURE_QUIT);
			return true;
		}
	}

	//���ӵ���
	if (wRequestChairID>=m_pGameServiceAttrib->wChairCount)
	{
		//Ч�����
		ASSERT_ERR(wRequestTableID<m_TableFrameArray.GetCount());
		if (wRequestTableID>=m_TableFrameArray.GetCount()) return false;

		//���ҿ�λ
		wRequestChairID=m_TableFrameArray[wRequestTableID]->GetNullChairID();

		//����ж�
		if (wRequestChairID==INVALID_CHAIR)
		{
			SendRequestFailure(pIServerUserItem,TEXT("���ڴ���Ϸ����ʱû�п������������λ���ˣ���ѡ���������Ϸ����"), REQUEST_FAILURE_QUIT);
			return true;
		}
	}

	//���´���
	CTableFrame * pTableFrame=m_TableFrameArray[wRequestTableID];
	if ( pTableFrame != NULL )
	{
		bool bResult = false;
		bResult = pTableFrame->PerformSitDownAction(wRequestChairID, pIServerUserItem, pUserSitDown->szPassword);
		//�����������
		if (bResult) {
			I_TRACE(TEXT("nickName:%s,LastTableID:%d"), pIServerUserItem->GetNickName(), pIServerUserItem->GetTableID());
		}
	}

	return true;
}

//�û�����
bool CAttemperEngineSink::OnTCPNetworkSubUserStandUpH5(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	//ASSERT_ERR(wDataSize == sizeof(CMD_GR_UserStandUp));
	//if (wDataSize != sizeof(CMD_GR_UserStandUp)) return false;

	GameServer::GSUserStandUp gsUserStandUp;
	gsUserStandUp.ParseFromArray(pData, wDataSize);
	CMD_GR_UserStandUp UserStandUp = { 0 };
	UserStandUp.wTableID = gsUserStandUp.tableid();
	UserStandUp.wChairID = gsUserStandUp.chairid();
	UserStandUp.cbForceLeave = (BYTE)gsUserStandUp.forceleave();

	//Ч������
	//CMD_GR_UserStandUp * pUserStandUp = (CMD_GR_UserStandUp *)&UserStandUp;
	//if (pUserStandUp->wChairID >= m_pGameServiceAttrib->wChairCount) return false;
	//if (pUserStandUp->wTableID >= (WORD)m_TableFrameArray.GetCount()) return false;

	//��ȡ�û�
	WORD wBindIndex = LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem = GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT_ERR(pIServerUserItem != NULL);
	if (pIServerUserItem == NULL) return false;

	DeleteDistribute(pIServerUserItem);

	//��Ϣ����
	WORD wTableID = pIServerUserItem->GetTableID();
	WORD wChairID = pIServerUserItem->GetChairID();
	//if ((wTableID != pUserStandUp->wTableID) || (wChairID != pUserStandUp->wChairID)) return true;


	//�û��ж�
 	if ( pIServerUserItem->GetUserStatus() == US_PLAYING && m_pGameServiceOption->wKindID != KIND_FISH)
 	{
 		SendRequestFailureH5(pIServerUserItem, TEXT("��������Ϸ�У���ʱ�����뿪�����Ƚ�����ǰ��Ϸ��"), REQUEST_FAILURE_NORMAL);
 		return true;
 	}

	//�뿪����
	if (wTableID != INVALID_TABLE)
	{
		CTableFrame * pTableFrame = m_TableFrameArray[wTableID];
		if ( pTableFrame )
		{
			I_TRACE(TEXT("�뿪���� UserID[%d] cbStatus[%d]"), pIServerUserItem->GetUserID(), pIServerUserItem->GetUserStatus());

			if (pTableFrame->PerformStandUpAction(pIServerUserItem,TRUE) == false)
			{
				m_UserFileLog.Log(TEXT("���[%d] ����[%d-%d] ״̬[%d] ��������ʧ��"),pIServerUserItem->GetUserID(),pIServerUserItem->GetTableID(),pIServerUserItem->GetChairID(),pIServerUserItem->GetUserStatus());
			 }
		}
	}
	GameServer::GSUserLeave gsUserLeave;
	gsUserLeave.set_userid(gsUserStandUp.userid());
	std::string serializeStr;
	gsUserLeave.SerializeToString(&serializeStr);
	SendData(dwSocketID, MDM_GR_USER, SUB_GR_USER_LEAVE, (void*)serializeStr.c_str(), (WORD)serializeStr.size());
	I_TRACE(TEXT("�����뿪 UserID[%d] cbStatus[%d]"), pIServerUserItem->GetUserID(), pIServerUserItem->GetUserStatus());
	CString cs;
	cs.Format(TEXT("��������[3],������[16]��|���뿪���û�[%d][%s],BindIndex[%d],SocketID[%d]"), pIServerUserItem->GetUserID(),pIServerUserItem->GetNickName(), wBindIndex, dwSocketID);
	m_UserFileLog.Log(cs.GetBuffer(0));

	pIServerUserItem->SetUserStatus(US_NULL, INVALID_TABLE, INVALID_CHAIR);

	return true;
}

//�û�����
bool CAttemperEngineSink::OnTCPNetworkSubUserStandUp(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT_ERR(wDataSize==sizeof(CMD_GR_UserStandUp));
	if (wDataSize!=sizeof(CMD_GR_UserStandUp)) return false;

	//Ч������
	CMD_GR_UserStandUp * pUserStandUp=(CMD_GR_UserStandUp *)pData;
	if (pUserStandUp->wChairID>=m_pGameServiceAttrib->wChairCount) return false;
	if (pUserStandUp->wTableID>=(WORD)m_TableFrameArray.GetCount()) return false;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//test
	DeleteDistribute(pIServerUserItem);


	//if(CServerRule::IsAllowAvertCheatMode(m_pGameServiceOption->dwServerRule)&&(m_pGameServiceAttrib->wChairCount < MAX_CHAIR))
	//{
	//	//ȡ������
	//	DeleteDistribute(pIServerUserItem);

	//	if(pUserStandUp->wTableID==INVALID_TABLE) return true;
	//}

	//��Ϣ����
	WORD wTableID=pIServerUserItem->GetTableID();
	WORD wChairID=pIServerUserItem->GetChairID();
	if ((wTableID!=pUserStandUp->wTableID)||(wChairID!=pUserStandUp->wChairID)) return true;


	//�û��ж�
	if (/*(pUserStandUp->cbForceLeave==FALSE)&&*/(pIServerUserItem->GetUserStatus()==US_PLAYING))
	{
		SendRequestFailure(pIServerUserItem,TEXT("��������Ϸ�У���ʱ�����뿪�����Ƚ�����ǰ��Ϸ��"), REQUEST_FAILURE_NORMAL);
		return true;
	}

	//�뿪����
	if (wTableID!=INVALID_TABLE)
	{
		CTableFrame * pTableFrame=m_TableFrameArray[wTableID];
		if (pTableFrame != NULL && pTableFrame->PerformStandUpAction(pIServerUserItem)==false) return true;
	}

	return true;
}

//H5����
bool CAttemperEngineSink::OnTCPNetworkSubUserChangeTableH5(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//��ȡ�û�
	WORD wBindIndex = LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem = GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT_ERR(pIServerUserItem != NULL);
	if (pIServerUserItem == NULL) return false;

	CString cs;
	cs.Format(TEXT("��������[3],������[17]��|���������û�[%d][%s] BindIndex[%d] SocketID[%d] ״̬[%d] ����[%d-%d] "), pIServerUserItem->GetUserID(),pIServerUserItem->GetNickName(), wBindIndex, dwSocketID, pIServerUserItem->GetUserStatus(), pIServerUserItem->GetTableID(), pIServerUserItem->GetChairID());
	m_UserFileLog.Log(cs.GetBuffer(0));

	if (DetectKickUser(pIServerUserItem,TRUE)) return true;

	//�û�״̬
	if (pIServerUserItem->GetUserStatus() == US_PLAYING && m_pGameServiceOption->wKindID != KIND_FISH)
	{
		SendRequestFailureH5(pIServerUserItem, TEXT("��������Ϸ�У���ʱ�����뿪�����Ƚ�����ǰ��Ϸ��"), REQUEST_FAILURE_NORMAL);
		return true;
	}

	WORD wRequestTableID = INVALID_TABLE;
	WORD wRequestChairID = INVALID_CHAIR;
	bool bRet = false;
	//��������
	for (INT_PTR i = 0; i < (m_pGameServiceOption->wTableCount); i++)
	{
		//����ͬ��
		if (i == pIServerUserItem->GetTableID())continue;

		//��ȡ����
		CTableFrame * pTableFrame = m_TableFrameArray[i];
		if (pTableFrame->IsGameStarted() == true) continue;
		if (pTableFrame->IsTableLocked()) continue;

		tagTableUserInfo userInfo = { 0 };
		pTableFrame->GetTableUserInfo(userInfo);

		//Ѱ�ҿ�λ������
		if (userInfo.wTableUserCount > 0) continue;

		//��ȡ��λ
		WORD wChairID = pTableFrame->GetNullChairID();

		if (wChairID == INVALID_CHAIR) continue;

		wRequestTableID = i;
		wRequestChairID = wChairID;
	
		//����ж�
		if ((wRequestTableID == INVALID_CHAIR) || (wRequestChairID == INVALID_CHAIR))
		{
			SendRequestFailureH5(pIServerUserItem, TEXT("��ǰ��Ϸ�����Ѿ�����Ϊ���ˣ���ʱû�п������������λ�ã����Ժ����ԣ�"), REQUEST_FAILURE_QUIT);
			return true;
		}

		//�뿪����
		if (pIServerUserItem->GetTableID() != INVALID_TABLE)
		{
			CTableFrame * pTableFrame = m_TableFrameArray[pIServerUserItem->GetTableID()];
			if (pTableFrame->PerformStandUpActionH5(pIServerUserItem) == false) return true;
		}

		//�û�����
		bRet = pTableFrame->PerformSitDownActionH5(wChairID, pIServerUserItem);
		if (bRet)
		{
			I_TRACE(TEXT("H5���� %s %d-%d Status:%d"),pIServerUserItem->GetNickName(),pIServerUserItem->GetTableID(),pIServerUserItem->GetChairID(),pIServerUserItem->GetUserStatus());
		}
		return true;
	}

	//ʧ��
	SendRequestFailureH5(pIServerUserItem, TEXT("û�ҵ��ɽ������Ϸ����"), REQUEST_FAILURE_QUIT);
	return true;
}

bool CAttemperEngineSink::OnTCPNetworkSubUserChangeTable(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//�û�״̬
	if(pIServerUserItem->GetUserStatus() == US_PLAYING)
	{
		//ʧ��
		SendRequestFailure(pIServerUserItem,TEXT("��������Ϸ�У���ʱ�����뿪�����Ƚ�����ǰ��Ϸ��"),REQUEST_FAILURE_NORMAL);
		return true;
	}

	WORD wRequestTableID = INVALID_TABLE;
	WORD wRequestChairID = INVALID_CHAIR;

	//��������
	for (INT_PTR i=0;i<(m_pGameServiceOption->wTableCount);i++)
	{
		//����ͬ��
		if(i == pIServerUserItem->GetTableID())continue;

		//��ȡ����
		CTableFrame * pTableFrame=m_TableFrameArray[i];
		if (pTableFrame->IsGameStarted()==true) continue;
		if(pTableFrame->IsTableLocked()) continue;

		//��Ч����
		WORD wChairID=pTableFrame->GetRandNullChairID();
		if (wChairID==INVALID_CHAIR) continue;


		if (wChairID!=INVALID_CHAIR)
		{
			CString szFileName;
			szFileName.Format(TEXT("%s/Settings/RoomID/%d.ini"),m_szPath,m_pGameServiceOption->dwRoomID);

			DWORD dwAllowSameIP = 0;
			dwAllowSameIP	= GetPrivateProfileInt(TEXT("Parameter"),TEXT("AllowSameIP"),0,(LPCTSTR)szFileName);

			bool bLimitSameIP = false;
			//�ж������������IP�Ƿ���ͬ
			if (  dwAllowSameIP==0)
			{
				DWORD dwUserIP=pIServerUserItem->GetClientAddr();
				for( WORD j=0;j<m_pGameServiceAttrib->wChairCount;++j )
				{
					CTableFrame * pTableFrame=m_TableFrameArray[i];
					IServerUserItem * pITableUserItem=pTableFrame->GetTableUserItem(j);
					if ((pITableUserItem!=NULL)&&(pITableUserItem != pIServerUserItem)&&(!pITableUserItem->IsAndroidUser())&&(pITableUserItem->GetMasterOrder()==0)&&(pITableUserItem->GetClientAddr()==dwUserIP))
					{
						bLimitSameIP = true;
						break;
					}
				}
			}
			//IPͬ����ʶ
			if ( bLimitSameIP ) continue;

			wRequestTableID = i;
			wRequestChairID = wChairID;
		}

		//����ж�
		if ((wRequestTableID==INVALID_CHAIR)||(wRequestChairID==INVALID_CHAIR))
		{
			SendRequestFailure(pIServerUserItem,TEXT("��ǰ��Ϸ�����Ѿ�����Ϊ���ˣ���ʱû�п������������λ�ã����Ժ����ԣ�"), REQUEST_FAILURE_QUIT);
			return true;
		}

		//�뿪����
		if (pIServerUserItem->GetTableID()!=INVALID_TABLE)
		{
			CTableFrame * pTableFrame=m_TableFrameArray[pIServerUserItem->GetTableID()];
			if (pTableFrame->PerformStandUpAction(pIServerUserItem)==false) return true;
		}

		//�û�����
		pTableFrame->PerformSitDownAction(wChairID,pIServerUserItem);
		return true;
	}

	//ʧ��
	m_TableFrameArray[0]->SendRequestFailure(pIServerUserItem,TEXT("û�ҵ��ɽ������Ϸ����"),REQUEST_FAILURE_NORMAL);
	return true;
}

//�������λ��
bool CAttemperEngineSink::OnTCPNetworkSubUserChairReq(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//�û�״̬
	if(pIServerUserItem->GetUserStatus() == US_PLAYING)
	{
		//ʧ��
		SendRequestFailure(pIServerUserItem,TEXT("��������Ϸ�У���ʱ�����뿪�����Ƚ�����ǰ��Ϸ��"),REQUEST_FAILURE_NORMAL);
		return true;
	}

	WORD wRequestTableID = INVALID_TABLE;
	WORD wRequestChairID = INVALID_CHAIR;

	//��������
	for (INT_PTR i=0;i<(m_pGameServiceOption->wTableCount);i++)
	{
		//����ͬ��
		if(i == pIServerUserItem->GetTableID())continue;

		//��ȡ����
		CTableFrame * pTableFrame=m_TableFrameArray[i];
		if (pTableFrame->IsGameStarted()==true) continue;
		if(pTableFrame->IsTableLocked()) continue;

		//��Ч����
		WORD wChairID=pTableFrame->GetRandNullChairID();
		if (wChairID==INVALID_CHAIR) continue;


		if (wChairID!=INVALID_CHAIR)
		{
			CString szFileName;
			szFileName.Format(TEXT("%s/Settings/RoomID/%d.ini"),m_szPath,m_pGameServiceOption->dwRoomID);

			DWORD dwAllowSameIP = 0;
			dwAllowSameIP	= GetPrivateProfileInt(TEXT("Parameter"),TEXT("AllowSameIP"),0,(LPCTSTR)szFileName);

			bool bLimitSameIP = false;
			//�ж������������IP�Ƿ���ͬ
			if (  dwAllowSameIP==0)
			{
				DWORD dwUserIP=pIServerUserItem->GetClientAddr();
				for( WORD j=0;j<m_pGameServiceAttrib->wChairCount;++j )
				{
					CTableFrame * pTableFrame=m_TableFrameArray[i];
					IServerUserItem * pITableUserItem=pTableFrame->GetTableUserItem(j);
					if ((pITableUserItem!=NULL)&&(pITableUserItem != pIServerUserItem)&&(!pITableUserItem->IsAndroidUser())&&(pITableUserItem->GetMasterOrder()==0)&&(pITableUserItem->GetClientAddr()==dwUserIP))
					{
						bLimitSameIP = true;
						break;
					}
				}
			}
			//IPͬ����ʶ
			if ( bLimitSameIP ) continue;

			wRequestTableID = i;
			wRequestChairID = wChairID;
		}

		//����ж�
		if ((wRequestTableID==INVALID_CHAIR)||(wRequestChairID==INVALID_CHAIR))
		{
			SendRequestFailure(pIServerUserItem,TEXT("��ǰ��Ϸ�����Ѿ�����Ϊ���ˣ���ʱû�п������������λ�ã����Ժ����ԣ�"), REQUEST_FAILURE_QUIT);
			return true;
		}

		//�뿪����
		if (pIServerUserItem->GetTableID()!=INVALID_TABLE)
		{
			CTableFrame * pTableFrame=m_TableFrameArray[pIServerUserItem->GetTableID()];
			if (pTableFrame->PerformStandUpAction(pIServerUserItem)==false) return true;
		}

		//�û�����
		pTableFrame->PerformSitDownAction(wChairID,pIServerUserItem);
		return true;
	}

	//
	if(pIServerUserItem->GetTableID() != INVALID_TABLE)
	{
		//��ȡ����
		CTableFrame * pTableFrame=m_TableFrameArray[pIServerUserItem->GetTableID()];
		if (pTableFrame->IsGameStarted()==false && pTableFrame->IsTableLocked()==false)
		{
			//��Ч����
			WORD wChairID=pTableFrame->GetRandNullChairID();
			if (wChairID!=INVALID_CHAIR)
			{
				//�뿪����
				if (pIServerUserItem->GetTableID()!=INVALID_TABLE)
				{
					CTableFrame * pTableFrame=m_TableFrameArray[pIServerUserItem->GetTableID()];
					if (pTableFrame->PerformStandUpAction(pIServerUserItem)==false) return true;
				}

				//�û�����
				pTableFrame->PerformSitDownAction(wChairID,pIServerUserItem);
				return true;
			}
		}
	}

	//ʧ��
	m_TableFrameArray[0]->SendRequestFailure(pIServerUserItem,TEXT("û�ҵ��ɽ������Ϸ����"),REQUEST_FAILURE_NORMAL);
	return true;
}

//���������û���Ϣ
bool CAttemperEngineSink::OnTCPNetworkSubChairUserInfoReq(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT_ERR(wDataSize==sizeof(CMD_GR_ChairUserInfoReq));
	if (wDataSize!=sizeof(CMD_GR_ChairUserInfoReq)) return false;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	CMD_GR_ChairUserInfoReq * pUserInfoReq = (CMD_GR_ChairUserInfoReq *)pData;
	if(pUserInfoReq->wTableID == INVALID_TABLE) return true;
	if(pUserInfoReq->wTableID >= m_pGameServiceOption->wTableCount)return true;

	//������Ϣ
	WORD wChairCout = m_TableFrameArray[pUserInfoReq->wTableID]->GetChairCount();
	for(WORD wIndex = 0; wIndex < wChairCout; wIndex++)
	{
		//��ȡ�û�
		if(pUserInfoReq->wChairID != INVALID_CHAIR && wIndex != pUserInfoReq->wChairID)continue;
		IServerUserItem * pTagerIServerUserItem=m_TableFrameArray[pUserInfoReq->wTableID]->GetTableUserItem(wIndex);
		if(pTagerIServerUserItem==NULL)continue;

		//��������
		BYTE cbBuffer[SOCKET_TCP_PACKET]={0};
		tagMobileUserInfoHead * pUserInfoHead=(tagMobileUserInfoHead *)cbBuffer;
		CSendPacketHelper SendPacket(cbBuffer+sizeof(tagMobileUserInfoHead),sizeof(cbBuffer)-sizeof(tagMobileUserInfoHead));
		tagUserInfo *pUserInfo = pTagerIServerUserItem->GetUserInfo();

		//�û�����
		pUserInfoHead->wFaceID=pUserInfo->wFaceID;
		pUserInfoHead->dwGameID=pUserInfo->dwGameID;
		pUserInfoHead->dwUserID=pUserInfo->dwUserID;
		pUserInfoHead->dwCustomID=pUserInfo->dwCustomID;

		//�û�����
		pUserInfoHead->cbGender=pUserInfo->cbGender;
		pUserInfoHead->cbMemberOrder=pUserInfo->cbMemberOrder;

		//�û�״̬
		pUserInfoHead->wTableID=pUserInfo->wTableID;
		pUserInfoHead->wChairID=pUserInfo->wChairID;
		pUserInfoHead->cbUserStatus=pUserInfo->cbUserStatus;

		//�û�����
		pUserInfoHead->dwWinCount=pUserInfo->dwWinCount;
		pUserInfoHead->dwLostCount=pUserInfo->dwLostCount;
		pUserInfoHead->dwDrawCount=pUserInfo->dwDrawCount;
		pUserInfoHead->dwFleeCount=pUserInfo->dwFleeCount;
		pUserInfoHead->dwExperience=pUserInfo->dwExperience;

		//�û��ɼ�
		pUserInfoHead->lScore=pUserInfo->lScore;
		pUserInfoHead->lScore+=pIServerUserItem->GetTrusteeScore();
		pUserInfoHead->lScore+=pIServerUserItem->GetFrozenedScore();

		//������Ϣ
		SendPacket.AddPacket(pUserInfo->szNickName,DTP_GR_NICK_NAME);

		//������Ϣ
		WORD wHeadSize=sizeof(tagMobileUserInfoHead);
		SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_USER_ENTER,cbBuffer,wHeadSize+SendPacket.GetDataSize());
	}

	return true;
}



//��ѯ����
bool CAttemperEngineSink::OnTCPNetworkSubQueryInsureInfo(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT_ERR(wDataSize==sizeof(CMD_GR_C_QueryInsureInfoRequest));
	if (wDataSize!=sizeof(CMD_GR_C_QueryInsureInfoRequest)) return false;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	CMD_GR_C_QueryInsureInfoRequest * pQueryInsureInfoRequest = (CMD_GR_C_QueryInsureInfoRequest *)pData;

	//�����ж�
	ASSERT_ERR((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)!=0);
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)==0)
	{
		SendInsureFailure(pIServerUserItem,TEXT("�˷����ֹ��Ϸ��ȡ���ѯ����ʧ�ܣ�"),0L,pQueryInsureInfoRequest->cbActivityGame);

	}


	//��������
	DBR_GR_QueryInsureInfo QueryInsureInfo;
	ZeroMemory(&QueryInsureInfo,sizeof(QueryInsureInfo));

	//��������
	QueryInsureInfo.cbActivityGame=pQueryInsureInfoRequest->cbActivityGame;
	QueryInsureInfo.dwUserID=pIServerUserItem->GetUserID();
	QueryInsureInfo.dwClientAddr=pIServerUserItem->GetClientAddr();

	//Ͷ������
	m_pIDBCorrespondManager->PostDataBaseRequest(QueryInsureInfo.dwUserID,DBR_GR_QUERY_INSURE_INFO,dwSocketID,&QueryInsureInfo,sizeof(QueryInsureInfo));

	return true;
}

//�������
bool CAttemperEngineSink::OnTCPNetworkSubSaveScoreRequest(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT_ERR(wDataSize==sizeof(CMD_GR_C_SaveScoreRequest));
	if (wDataSize!=sizeof(CMD_GR_C_SaveScoreRequest)) return false;

	//�����ж�
	ASSERT_ERR((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)!=0);
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)==0) return false;

	//��������
	CMD_GR_C_SaveScoreRequest * pSaveScoreRequest=(CMD_GR_C_SaveScoreRequest *)pData;

	//Ч�����
	ASSERT_ERR(pSaveScoreRequest->lSaveScore>0L);
	if (pSaveScoreRequest->lSaveScore<=0L) return false;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//�����ж�
	if(pSaveScoreRequest->cbActivityGame == FALSE && CServerRule::IsForfendSaveInRoom(m_pGameServiceOption->dwServerRule))
	{
		//��������
		SendInsureFailure(pIServerUserItem,TEXT("�˷����ֹ������������ʧ�ܣ�"),0L,pSaveScoreRequest->cbActivityGame);
		return true;
	}

	//�����ж�
	if(pSaveScoreRequest->cbActivityGame == TRUE && CServerRule::IsForfendSaveInGame(m_pGameServiceOption->dwServerRule))
	{
		//��������
		SendInsureFailure(pIServerUserItem,TEXT("�˷����ֹ��Ϸ���������ʧ�ܣ�"),0L,pSaveScoreRequest->cbActivityGame);
		return true;
	}

	//��������
	SCORE lConsumeQuota=0L;
	SCORE lUserWholeScore=pIServerUserItem->GetUserScore()+pIServerUserItem->GetTrusteeScore();

	//��ȡ�޶�
	if (pIServerUserItem->GetTableID()!=INVALID_TABLE)
	{
		WORD wTableID=pIServerUserItem->GetTableID();
		lConsumeQuota=m_TableFrameArray[wTableID]->QueryConsumeQuota(pIServerUserItem);
	}
	else
	{
		lConsumeQuota=pIServerUserItem->GetUserScore()+pIServerUserItem->GetTrusteeScore();
	}

	//�޶��ж�
	if (pSaveScoreRequest->lSaveScore>lConsumeQuota)
	{
		if (lConsumeQuota<lUserWholeScore)
		{
			//������ʾ
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("������������Ϸ�У�Ԫ���ɴ�����Ϊ %I64d���������ʧ�ܣ�"),lConsumeQuota);

			//��������
			SendInsureFailure(pIServerUserItem,szDescribe,0L,pSaveScoreRequest->cbActivityGame);
		}
		else
		{
			//��������
			SendInsureFailure(pIServerUserItem,TEXT("���Ľ�����㣬�������ʧ�ܣ�"),0L,pSaveScoreRequest->cbActivityGame);
		}

		return true;
	}

	//��������
	if (pIServerUserItem->FrozenedUserScore(pSaveScoreRequest->lSaveScore)==false)
	{
		ASSERT_ERR(FALSE);
		return false;
	}

	//��������
	DBR_GR_UserSaveScore UserSaveScore;
	ZeroMemory(&UserSaveScore,sizeof(UserSaveScore));

	//��������
	UserSaveScore.cbActivityGame=pSaveScoreRequest->cbActivityGame;
	UserSaveScore.dwUserID=pIServerUserItem->GetUserID();
	UserSaveScore.lSaveScore=pSaveScoreRequest->lSaveScore;
	UserSaveScore.dwClientAddr=pIServerUserItem->GetClientAddr();
	lstrcpyn(UserSaveScore.szMachineID,pIServerUserItem->GetMachineID(),CountArray(UserSaveScore.szMachineID));

	//Ͷ������
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_USER_SAVE_SCORE,dwSocketID,&UserSaveScore,sizeof(UserSaveScore));

	return true;
}

//ȡ������
bool CAttemperEngineSink::OnTCPNetworkSubTakeScoreRequest(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT_ERR(wDataSize==sizeof(CMD_GR_C_TakeScoreRequest));
	if (wDataSize!=sizeof(CMD_GR_C_TakeScoreRequest)) return false;

	//�����ж�
	ASSERT_ERR((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)!=0);
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)==0) return false;

	//��������
	CMD_GR_C_TakeScoreRequest * pTakeScoreRequest=(CMD_GR_C_TakeScoreRequest *)pData;
	pTakeScoreRequest->szInsurePass[CountArray(pTakeScoreRequest->szInsurePass)-1]=0;

	//Ч�����
	ASSERT_ERR(pTakeScoreRequest->lTakeScore>0L);
	if (pTakeScoreRequest->lTakeScore<=0L) return false;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//�����ж�
	if(pTakeScoreRequest->cbActivityGame == FALSE && CServerRule::IsForfendTakeInRoom(m_pGameServiceOption->dwServerRule))
	{
		//��������
		SendInsureFailure(pIServerUserItem,TEXT("�˷����ֹ����ȡ�ȡ������ʧ�ܣ�"),0L,pTakeScoreRequest->cbActivityGame);
		return true;
	}

	//�����ж�
	if(pTakeScoreRequest->cbActivityGame == TRUE && CServerRule::IsForfendTakeInGame(m_pGameServiceOption->dwServerRule))
	{
		//��������
		SendInsureFailure(pIServerUserItem,TEXT("�˷����ֹ��Ϸȡ�ȡ������ʧ�ܣ�"),0L,pTakeScoreRequest->cbActivityGame);
		return true;
	}

	//��������
	DBR_GR_UserTakeScore UserTakeScore;
	ZeroMemory(&UserTakeScore,sizeof(UserTakeScore));

	//��������
	UserTakeScore.cbActivityGame=pTakeScoreRequest->cbActivityGame;
	UserTakeScore.dwUserID=pIServerUserItem->GetUserID();
	UserTakeScore.lTakeScore=pTakeScoreRequest->lTakeScore;
	UserTakeScore.dwClientAddr=pIServerUserItem->GetClientAddr();
	lstrcpyn(UserTakeScore.szPassword,pTakeScoreRequest->szInsurePass,CountArray(UserTakeScore.szPassword));
	lstrcpyn(UserTakeScore.szMachineID,pIServerUserItem->GetMachineID(),CountArray(UserTakeScore.szMachineID));

	//Ͷ������
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_USER_TAKE_SCORE,dwSocketID,&UserTakeScore,sizeof(UserTakeScore));

	return true;
}

//ת������
bool CAttemperEngineSink::OnTCPNetworkSubTransferScoreRequest(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT_ERR(wDataSize==sizeof(CMD_GP_C_TransferScoreRequest));
	if (wDataSize!=sizeof(CMD_GP_C_TransferScoreRequest)) return false;

	//�����ж�
	ASSERT_ERR((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)!=0);
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)==0) return false;

	//��������
	CMD_GP_C_TransferScoreRequest * pTransferScoreRequest=(CMD_GP_C_TransferScoreRequest *)pData;
	pTransferScoreRequest->szNickName[CountArray(pTransferScoreRequest->szNickName)-1]=0;
	pTransferScoreRequest->szInsurePass[CountArray(pTransferScoreRequest->szInsurePass)-1]=0;

	//Ч�����
	ASSERT_ERR(pTransferScoreRequest->lTransferScore>0L);
	if (pTransferScoreRequest->lTransferScore<=0L) return false;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	DBR_GR_UserTransferScore UserTransferScore;
	ZeroMemory(&UserTransferScore,sizeof(UserTransferScore));

	//��������
	UserTransferScore.cbActivityGame=pTransferScoreRequest->cbActivityGame;
	UserTransferScore.dwUserID=pIServerUserItem->GetUserID();
	UserTransferScore.dwClientAddr=pIServerUserItem->GetClientAddr();
	UserTransferScore.cbByNickName=pTransferScoreRequest->cbByNickName;
	UserTransferScore.lTransferScore=pTransferScoreRequest->lTransferScore;
	lstrcpyn(UserTransferScore.szNickName,pTransferScoreRequest->szNickName,CountArray(UserTransferScore.szNickName));
	lstrcpyn(UserTransferScore.szMachineID,pIServerUserItem->GetMachineID(),CountArray(UserTransferScore.szMachineID));
	lstrcpyn(UserTransferScore.szPassword,pTransferScoreRequest->szInsurePass,CountArray(UserTransferScore.szPassword));

	//Ͷ������
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_USER_TRANSFER_SCORE,dwSocketID,&UserTransferScore,sizeof(UserTransferScore));

	return true;
}

//��ѯ�û�����
bool CAttemperEngineSink::OnTCPNetworkSubQueryUserInfoRequest(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT_ERR(wDataSize==sizeof(CMD_GR_C_QueryUserInfoRequest));
	if (wDataSize!=sizeof(CMD_GR_C_QueryUserInfoRequest)) return false;

	//�����ж�
	ASSERT_ERR((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)!=0);
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)==0) return false;

	//��������
	CMD_GR_C_QueryUserInfoRequest * pQueryUserInfoRequest=(CMD_GR_C_QueryUserInfoRequest *)pData;
	pQueryUserInfoRequest->szNickName[CountArray(pQueryUserInfoRequest->szNickName)-1]=0;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	DBR_GR_QueryTransferUserInfo QueryTransferUserInfo;
	ZeroMemory(&QueryTransferUserInfo,sizeof(QueryTransferUserInfo));

	//��������
	QueryTransferUserInfo.cbActivityGame=pQueryUserInfoRequest->cbActivityGame;
	QueryTransferUserInfo.cbByNickName=pQueryUserInfoRequest->cbByNickName;
	lstrcpyn(QueryTransferUserInfo.szNickName,pQueryUserInfoRequest->szNickName,CountArray(QueryTransferUserInfo.szNickName));
	QueryTransferUserInfo.dwUserID=pIServerUserItem->GetUserID();

	//Ͷ������
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_QUERY_TRANSFER_USER_INFO,dwSocketID,&QueryTransferUserInfo,sizeof(QueryTransferUserInfo));

	return true;
}

//��ѯ����
bool CAttemperEngineSink::OnTCPNetworkSubQueryOption(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT_ERR((pIServerUserItem!=NULL)&&(pIServerUserItem->GetMasterOrder()>0));
	if ((pIServerUserItem==NULL)||(pIServerUserItem->GetMasterOrder()==0)) return false;

	//��������
	CMD_GR_OptionCurrent OptionCurrent;
	ZeroMemory(&OptionCurrent,sizeof(OptionCurrent));

	//�ҽ�����
	OptionCurrent.ServerOptionInfo.wKindID=m_pGameServiceOption->wKindID;
	OptionCurrent.ServerOptionInfo.wNodeID=m_pGameServiceOption->wNodeID;
	OptionCurrent.ServerOptionInfo.wSortID=m_pGameServiceOption->wSortID;

	//��������
	OptionCurrent.ServerOptionInfo.wRevenueRatio=m_pGameServiceOption->wRevenueRatio;
	OptionCurrent.ServerOptionInfo.lServiceScore=m_pGameServiceOption->lServiceScore;
	OptionCurrent.ServerOptionInfo.lRestrictScore=m_pGameServiceOption->lRestrictScore;
	OptionCurrent.ServerOptionInfo.lMinTableScore=m_pGameServiceOption->lMinTableScore;
	OptionCurrent.ServerOptionInfo.lMinEnterScore=m_pGameServiceOption->lMinEnterScore;
	OptionCurrent.ServerOptionInfo.lMaxEnterScore=m_pGameServiceOption->lMaxEnterScore;
	OptionCurrent.ServerOptionInfo.lMinEnterTiLi=m_pGameServiceOption->lMinEnterTiLi;

	//��Ա����
	OptionCurrent.ServerOptionInfo.cbMinEnterMember=m_pGameServiceOption->cbMinEnterMember;
	OptionCurrent.ServerOptionInfo.cbMaxEnterMember=m_pGameServiceOption->cbMaxEnterMember;

	//��������
	OptionCurrent.ServerOptionInfo.dwServerRule=m_pGameServiceOption->dwServerRule;
	lstrcpyn(OptionCurrent.ServerOptionInfo.szServerName,m_pGameServiceOption->szServerName,CountArray(OptionCurrent.ServerOptionInfo.szServerName));

	//�������
	OptionCurrent.dwRuleMask|=SR_FORFEND_GAME_CHAT;
	OptionCurrent.dwRuleMask|=SR_FORFEND_ROOM_CHAT;
	OptionCurrent.dwRuleMask|=SR_FORFEND_WISPER_CHAT;
	OptionCurrent.dwRuleMask|=SR_FORFEND_WISPER_ON_GAME;

	//�������
	OptionCurrent.dwRuleMask|=SR_FORFEND_ROOM_ENTER;
	OptionCurrent.dwRuleMask|=SR_FORFEND_GAME_ENTER;
	OptionCurrent.dwRuleMask|=SR_FORFEND_GAME_LOOKON;

	//���й���
	OptionCurrent.dwRuleMask|=SR_FORFEND_TAKE_IN_ROOM;
	OptionCurrent.dwRuleMask|=SR_FORFEND_TAKE_IN_GAME;
	OptionCurrent.dwRuleMask|=SR_FORFEND_SAVE_IN_ROOM;
	OptionCurrent.dwRuleMask|=SR_FORFEND_SAVE_IN_GAME;

	//��������
	//OptionCurrent.dwRuleMask|=SR_RECORD_GAME_TRACK;
	OptionCurrent.dwRuleMask|=SR_FORFEND_GAME_RULE;
	OptionCurrent.dwRuleMask|=SR_FORFEND_LOCK_TABLE;
	OptionCurrent.dwRuleMask|=SR_ALLOW_ANDROID_SIMULATE;

	//�������
	if (m_pGameServiceAttrib->cbDynamicJoin==TRUE) OptionCurrent.dwRuleMask|=SR_ALLOW_DYNAMIC_JOIN;
	if (m_pGameServiceAttrib->cbAndroidUser==TRUE) OptionCurrent.dwRuleMask|=SR_ALLOW_ANDROID_ATTEND;
	//if (m_pGameServiceAttrib->cbOffLineTrustee==TRUE) OptionCurrent.dwRuleMask|=SR_ALLOW_OFFLINE_TRUSTEE;

	//ģʽ����
	if ((m_pGameServiceOption->wServerType&(GAME_GENRE_GOLD|GAME_GENRE_MATCH|GAME_GENRE_SCORE))==0) OptionCurrent.dwRuleMask|=SR_RECORD_GAME_SCORE;
	if ((m_pGameServiceOption->wServerType&(GAME_GENRE_GOLD|GAME_GENRE_MATCH|GAME_GENRE_SCORE))==0) OptionCurrent.dwRuleMask|=SR_IMMEDIATE_WRITE_SCORE;

	//��������
	SendData(pIServerUserItem,MDM_GR_MANAGE,SUB_GR_OPTION_CURRENT,&OptionCurrent,sizeof(OptionCurrent));

	return true;
}

//��������
bool CAttemperEngineSink::OnTCPNetworkSubOptionServer(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT_ERR(wDataSize==sizeof(CMD_GR_ServerOption));
	if (wDataSize!=sizeof(CMD_GR_ServerOption)) return false;

	//��������
	CMD_GR_ServerOption * pServerOption=(CMD_GR_ServerOption *)pData;
	tagServerOptionInfo * pServerOptionInfo=&pServerOption->ServerOptionInfo;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT_ERR((pIServerUserItem!=NULL)&&(pIServerUserItem->GetMasterOrder()>0));
	if ((pIServerUserItem==NULL)||(pIServerUserItem->GetMasterOrder()==0)) return false;

	//��������
	bool bModifyServer=false;

	//�ҽӽڵ�
	if (m_pGameServiceOption->wNodeID!=pServerOptionInfo->wNodeID)
	{
		bModifyServer=true;
		m_pGameServiceOption->wNodeID=pServerOptionInfo->wNodeID;
	}

	//�ҽ�����
	if ((pServerOptionInfo->wKindID!=0)&&(m_pGameServiceOption->wKindID!=pServerOptionInfo->wKindID))
	{
		bModifyServer=true;
		m_pGameServiceOption->wKindID=pServerOptionInfo->wKindID;
	}

	//�ҽ�����
	if ((pServerOptionInfo->wSortID!=0)&&(m_pGameServiceOption->wSortID!=pServerOptionInfo->wSortID))
	{
		bModifyServer=true;
		m_pGameServiceOption->wSortID=pServerOptionInfo->wSortID;
	}

	//��������
	if ((pServerOptionInfo->szServerName[0]!=0)&&(lstrcmp(m_pGameServiceOption->szServerName,pServerOptionInfo->szServerName)!=0))
	{
		bModifyServer=true;
		lstrcpyn(m_pGameServiceOption->szServerName,pServerOptionInfo->szServerName,CountArray(m_pGameServiceOption->szServerName));
	}

	//˰������
	m_pGameServiceOption->wRevenueRatio=pServerOptionInfo->wRevenueRatio;
	m_pGameServiceOption->lServiceScore=pServerOptionInfo->lServiceScore;

	//��������
	m_pGameServiceOption->lRestrictScore=pServerOptionInfo->lRestrictScore;
	m_pGameServiceOption->lMinTableScore=pServerOptionInfo->lMinTableScore;
	m_pGameServiceOption->lMinEnterScore=pServerOptionInfo->lMinEnterScore;
	m_pGameServiceOption->lMaxEnterScore=pServerOptionInfo->lMaxEnterScore;
	m_pGameServiceOption->lMinEnterTiLi=pServerOptionInfo->lMinEnterTiLi;

	//��Ա����
	m_pGameServiceOption->cbMinEnterMember=pServerOptionInfo->cbMinEnterMember;
	m_pGameServiceOption->cbMaxEnterMember=pServerOptionInfo->cbMaxEnterMember;

	//�������
	CServerRule::SetForfendGameChat(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendGameChat(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendRoomChat(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendRoomChat(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendWisperChat(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendWisperChat(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendWisperOnGame(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendWisperOnGame(pServerOptionInfo->dwServerRule));

	//�������
	CServerRule::SetForfendRoomEnter(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendRoomEnter(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendGameEnter(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendGameEnter(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendGameLookon(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendGameLookon(pServerOptionInfo->dwServerRule));

	//���й���
	CServerRule::SetForfendTakeInRoom(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendTakeInRoom(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendTakeInGame(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendTakeInGame(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendSaveInRoom(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendSaveInRoom(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendSaveInGame(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendSaveInGame(pServerOptionInfo->dwServerRule));

	//��������
	CServerRule::SetRecordGameTrack(m_pGameServiceOption->dwServerRule,CServerRule::IsRecordGameTrack(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendGameRule(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendGameRule(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendLockTable(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendLockTable(pServerOptionInfo->dwServerRule));

	//��̬����
	if (m_pGameServiceAttrib->cbDynamicJoin==TRUE)
	{
		CServerRule::SetAllowDynamicJoin(m_pGameServiceOption->dwServerRule,CServerRule::IsAllowDynamicJoin(pServerOptionInfo->dwServerRule));
	}

	//��������
	if (m_pGameServiceAttrib->cbAndroidUser==TRUE)
	{
		CServerRule::SetAllowAndroidAttend(m_pGameServiceOption->dwServerRule,CServerRule::IsAllowAndroidAttend(pServerOptionInfo->dwServerRule));
	}

	//�����й�
	if (m_pGameServiceAttrib->cbOffLineTrustee==TRUE)
	{
		CServerRule::SetAllowOffLineTrustee(m_pGameServiceOption->dwServerRule,CServerRule::IsAllowOffLineTrustee(pServerOptionInfo->dwServerRule));
	}

	//��¼�ɼ�
	if ((m_pGameServiceOption->wServerType&(GAME_GENRE_GOLD|GAME_GENRE_MATCH|GAME_GENRE_SCORE))==0)
	{
		CServerRule::SetRecordGameScore(m_pGameServiceOption->dwServerRule,CServerRule::IsRecordGameScore(pServerOptionInfo->dwServerRule));
	}

	//����д��
	if ((m_pGameServiceOption->wServerType&(GAME_GENRE_GOLD|GAME_GENRE_MATCH|GAME_GENRE_SCORE))==0)
	{
		CServerRule::SetImmediateWriteScore(m_pGameServiceOption->dwServerRule,CServerRule::IsImmediateWriteScore(pServerOptionInfo->dwServerRule));
	}

	//��������
	CServiceUnits * pServiceUnits=CServiceUnits::g_pServiceUnits;
	pServiceUnits->RectifyServiceParameter();

	//�����޸�
	if (bModifyServer==true)
	{
		//��������
		CMD_CS_C_ServerModify ServerModify;
		ZeroMemory(&ServerModify,sizeof(ServerModify));

		//����˿�
		ServerModify.wServerPort=pServiceUnits->m_TCPNetworkEngine->GetCurrentPort();

		//������Ϣ
		ServerModify.wKindID=m_pGameServiceOption->wKindID;
		ServerModify.wNodeID=m_pGameServiceOption->wNodeID;
		ServerModify.wSortID=m_pGameServiceOption->wSortID;
		ServerModify.dwOnLineCount=m_ServerUserManager.GetUserItemCount();
		ServerModify.dwFullCount=m_pGameServiceOption->wMaxPlayer-RESERVE_USER_COUNT;
		lstrcpyn(ServerModify.szServerName,m_pGameServiceOption->szServerName,CountArray(ServerModify.szServerName));
		lstrcpyn(ServerModify.szServerAddr,m_pInitParameter->m_ServiceAddress.szAddress,CountArray(ServerModify.szServerAddr));

		//��������
		m_pITCPSocketService->SendData(MDM_CS_SERVICE_INFO,SUB_CS_C_SERVER_MODIFY,&ServerModify,sizeof(ServerModify));
	}

	//������Ϣ
	SendRoomMessage(pIServerUserItem,TEXT("��ǰ��Ϸ����������ġ�����ֵ��״̬���������޸ĳɹ�"),SMT_CHAT|SMT_EJECT);

	//�����Ϣ
	TCHAR szBuffer[128]=TEXT("");
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("Զ���޸ķ�������֪ͨ ����Ա %s [ %ld ]"),pIServerUserItem->GetNickName(),pIServerUserItem->GetUserID());

	//�����Ϣ
	TRACE_LOG(szBuffer,TraceLevel_Info);

	return true;
}

//�߳��û�
bool CAttemperEngineSink::OnTCPNetworkSubManagerKickUser(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT_ERR(wDataSize==sizeof(CMD_GR_KickUser));
	if (wDataSize!=sizeof(CMD_GR_KickUser)) return false;

	//��������
	CMD_GR_KickUser * pKickUser=(CMD_GR_KickUser *)pData;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT_ERR((pIServerUserItem!=NULL)&&(pIServerUserItem->GetMasterOrder()>0));
	if ((pIServerUserItem==NULL)||(pIServerUserItem->GetMasterOrder()==0)) return false;

	//Ȩ���ж�
	ASSERT_ERR(CMasterRight::CanKillUser(pIServerUserItem->GetMasterRight())==true);
	if (CMasterRight::CanKillUser(pIServerUserItem->GetMasterRight())==false) return false;

	//Ŀ���û�
	IServerUserItem * pITargetUserItem = m_ServerUserManager.SearchUserItem(pKickUser->dwTargetUserID);
	if(pITargetUserItem==NULL) return true;

	//�û�״̬
	if(pITargetUserItem->GetUserStatus()==US_PLAYING) return true;

	//�߳���¼
	tagKickUser KickUser = { 0 };
	KickUser.dwStartKickTime = (DWORD)time(NULL);
	KickUser.dwLimitTime = 600;
	m_KickUserItemMap[pITargetUserItem->GetUserID()]= KickUser;

	//��������
	WORD wTargerTableID = pITargetUserItem->GetTableID();
	if(wTargerTableID != INVALID_TABLE)
	{
		//������Ϣ
		SendGameMessage(pITargetUserItem,TEXT("���ѱ�����Ա�������ӣ�"),SMT_CHAT|SMT_CLOSE_GAME);

		CTableFrame * pTableFrame=m_TableFrameArray[wTargerTableID];
		if (pTableFrame->PerformStandUpAction(pITargetUserItem)==false) return true;
	}

	//����֪ͨ
	LPCTSTR pszMessage=TEXT("���ѱ�����Ա�������Ϸ���䣡");
	SendRoomMessage(pITargetUserItem,pszMessage,SMT_CHAT|SMT_EJECT|SMT_GLOBAL|SMT_CLOSE_ROOM);

	pITargetUserItem->SetUserStatus(US_NULL,INVALID_TABLE,INVALID_CHAIR);

	return true;
}

//��������
bool CAttemperEngineSink::OnTCPNetworkSubLimitUserChat(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч������
	ASSERT_ERR(wDataSize==sizeof(CMD_GR_LimitUserChat));
	if (wDataSize!=sizeof(CMD_GR_LimitUserChat)) return false;

	//��Ϣ����
	CMD_GR_LimitUserChat * pLimitUserChat=(CMD_GR_LimitUserChat *)pData;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);
	if (pIServerUserItem==NULL) return false;

	//Ȩ���ж�
	//ASSERT_ERR(CMasterRight::CanLimitUserChat(pIServerUserItem->GetMasterRight())==true);
	//if (CMasterRight::CanLimitUserChat(pIServerUserItem->GetMasterRight())==false) return false;

	//Ŀ���û�
	IServerUserItem * pITargerUserItem=m_ServerUserManager.SearchUserItem(pLimitUserChat->dwTargetUserID);
	if (pITargerUserItem==NULL) return true;

	//��������
	DWORD dwAddRight = 0, dwRemoveRight = 0;

	//��������
	if (pLimitUserChat->cbLimitFlags==OSF_ROOM_CHAT)
	{
		if (CMasterRight::CanLimitRoomChat(pIServerUserItem->GetMasterRight())==false) return false;

		if( pLimitUserChat->cbLimitValue == TRUE )
			dwAddRight |= UR_CANNOT_ROOM_CHAT;
		else
			dwRemoveRight |= UR_CANNOT_ROOM_CHAT;
	}

	//��Ϸ����
	if (pLimitUserChat->cbLimitFlags==OSF_GAME_CHAT)
	{
		if (CMasterRight::CanLimitGameChat(pIServerUserItem->GetMasterRight())==false) return false;

		if( pLimitUserChat->cbLimitValue == TRUE )
			dwAddRight |= UR_CANNOT_GAME_CHAT;
		else
			dwRemoveRight |= UR_CANNOT_GAME_CHAT;
	}

	//����˽��
	if (pLimitUserChat->cbLimitFlags==OSF_ROOM_WISPER)
	{
		if (CMasterRight::CanLimitWisper(pIServerUserItem->GetMasterRight())==false) return false;

		if( pLimitUserChat->cbLimitValue == TRUE )
			dwAddRight |= UR_CANNOT_WISPER;
		else
			dwRemoveRight |= UR_CANNOT_WISPER;
	}

	//��������
	if(pLimitUserChat->cbLimitFlags==OSF_SEND_BUGLE)
	{
		if (CMasterRight::CanLimitUserChat(pIServerUserItem->GetMasterRight())==false) return false;

		if(pLimitUserChat->cbLimitValue == TRUE)
			dwAddRight |= UR_CANNOT_BUGLE;
		else
			dwRemoveRight |= UR_CANNOT_BUGLE;
	}

	if( dwAddRight != 0 || dwRemoveRight != 0 )
	{
		pITargerUserItem->ModifyUserRight(dwAddRight,dwRemoveRight);

		//����֪ͨ
		CMD_GR_ConfigUserRight cur = {0};
		cur.dwUserRight = pITargerUserItem->GetUserRight();

		SendData( pITargerUserItem,MDM_GR_CONFIG,SUB_GR_CONFIG_USER_RIGHT,&cur,sizeof(cur) );

		//������Ϣ
		SendRoomMessage(pIServerUserItem,TEXT("�û�����Ȩ�����óɹ���"),SMT_CHAT);
	}
	else return false;

	return true;
}

//�߳������û�
bool CAttemperEngineSink::OnTCPNetworkSubKickAllUser(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//��Ϣ����
	CMD_GR_KickAllUser * pKillAllUser=(CMD_GR_KickAllUser *)pData;

	//Ч������
	ASSERT_ERR(wDataSize<=sizeof(CMD_GR_KickAllUser));
	if( wDataSize > sizeof(CMD_GR_KickAllUser) ) return false;
	ASSERT_ERR(wDataSize==CountStringBuffer(pKillAllUser->szKickMessage));
	if (wDataSize!=CountStringBuffer(pKillAllUser->szKickMessage)) return false;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);
	if (pIServerUserItem==NULL) return false;

	//Ȩ���ж�
	ASSERT_ERR(CMasterRight::CanKillUser(pIServerUserItem->GetMasterRight())==true);
	if (CMasterRight::CanKillUser(pIServerUserItem->GetMasterRight())==false) return false;

	//��ɢ������Ϸ
	for (INT_PTR i=0;i<m_TableFrameArray.GetCount();i++)
	{
		//��ȡ����
		CTableFrame * pTableFrame=m_TableFrameArray[i];
		if ( !pTableFrame->IsGameStarted() ) continue;

		pTableFrame->DismissGame();
	}

	tagBindParameter *pBindParameter = m_pNormalParameter;
	for( INT i = 0; i < m_pGameServiceOption->wMaxPlayer; i++ )
	{
		//Ŀ¼�û�
		IServerUserItem * pITargerUserItem= pBindParameter->pIServerUserItem;
		if (pITargerUserItem==NULL || pITargerUserItem==pIServerUserItem ) 
		{
			pBindParameter++;
			continue;
		}

		//������Ϣ
		SendRoomMessage(pITargerUserItem,pKillAllUser->szKickMessage,SMT_CHAT|SMT_EJECT|SMT_CLOSE_LINK|SMT_CLOSE_ROOM);

		pBindParameter++;
	} 

	return true;
}

//������Ϣ
bool CAttemperEngineSink::OnTCPNetworkSubSendMessage(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);
	if (pIServerUserItem==NULL) return false;

	//Ȩ���ж�
	ASSERT_ERR(CMasterRight::CanIssueMessage(pIServerUserItem->GetMasterRight())==true);
	if (CMasterRight::CanIssueMessage(pIServerUserItem->GetMasterRight())==false) return false;

	//��Ϣ����
	return SendSystemMessage((CMD_GR_SendMessage *)pData, wDataSize);
}

//��ɢ��Ϸ
bool CAttemperEngineSink::OnTCPNetworkSubDismissGame(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч������
	ASSERT_ERR(wDataSize==sizeof(CMD_GR_DismissGame));
	if (wDataSize!=sizeof(CMD_GR_DismissGame)) return false;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);
	if (pIServerUserItem==NULL) return false;

	//Ȩ���ж�
	//ASSERT_ERR(CMasterRight::CanDismissGame(pIServerUserItem->GetMasterRight())==true);
	//if (CMasterRight::CanDismissGame(pIServerUserItem->GetMasterRight())==false) return false;

	//��Ϣ����
	CMD_GR_DismissGame * pDismissGame=(CMD_GR_DismissGame *)pData;

	//Ч������
	if(pDismissGame->wDismissTableNum >= m_TableFrameArray.GetCount()) return true;

	//��ɢ��Ϸ
	CTableFrame *pTableFrame=m_TableFrameArray[pDismissGame->wDismissTableNum];
	if(pTableFrame)
	{
		//if(pTableFrame->IsGameStarted()) 
		pTableFrame->DismissGame();
	}

	return true;
}

bool CAttemperEngineSink::OnTCPNetworkSubSendWarning(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч������
	ASSERT_ERR(wDataSize==sizeof(CMD_GR_SendWarning));
	if (wDataSize!=sizeof(CMD_GR_SendWarning)) return false;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);
	if (pIServerUserItem==NULL) return false;

	return true;
}

//�û���¼
VOID CAttemperEngineSink::OnEventUserLogonH5(IServerUserItem * pIServerUserItem, bool bAlreadyOnLine)
{
	//��ȡ����
	WORD wBindIndex = pIServerUserItem->GetBindIndex();
	bool bAndroidUser = pIServerUserItem->IsAndroidUser();
	tagBindParameter * pBindParameter = GetBindParameter(wBindIndex);

	GameServer::GSLogonSuccess logonSuccess;
	logonSuccess.set_userright((int64)pIServerUserItem->GetUserRight());
	logonSuccess.set_masterright((int64)pIServerUserItem->GetMasterRight());
	std::string serializeStr;
	logonSuccess.SerializeToString(&serializeStr);
	SendData(pBindParameter->dwSocketID, MDM_GR_LOGON, SUB_GR_LOGON_SUCCESS, (void*)serializeStr.c_str(), (WORD)serializeStr.size());

	//��¼���
	GameServer::GSLogonFinish LogonFinish;
	LogonFinish.set_resultcode(0);
	LogonFinish.set_resultstr("logonFinish");
	serializeStr.clear();
	LogonFinish.SerializeToString(&serializeStr);
	SendData(pBindParameter->dwSocketID, MDM_GR_LOGON, SUB_GR_LOGON_FINISH,(void*)serializeStr.c_str(), (WORD)serializeStr.size());

	//�������
	I_TRACE(TEXT("H5��ҵ�¼ userID[%d],nickName[%s] %d-%d Status[%d] SocketID[%d]"), pIServerUserItem->GetUserID(),
		pIServerUserItem->GetNickName(), pIServerUserItem->GetTableID(),pIServerUserItem->GetChairID(),
		pIServerUserItem->GetUserStatus(), pIServerUserItem->GetBindIndex());
	//SendUserInfoPacketH5(pIServerUserItem, pBindParameter->dwSocketID);


	//��������
	if (bAndroidUser == false)
	{
		if (pBindParameter->cbClientKind == CLIENT_KIND_MOBILE)
		{
			m_pITCPNetworkEngine->AllowBatchSend(pBindParameter->dwSocketID, true, BG_MOBILE);
		}
		else
		{
			m_pITCPNetworkEngine->AllowBatchSend(pBindParameter->dwSocketID, true, BG_COMPUTER);
		}
	}

	return;
}

//�û���¼
VOID CAttemperEngineSink::OnEventUserLogon(IServerUserItem * pIServerUserItem, bool bAlreadyOnLine)
{
	//��ȡ����
	WORD wBindIndex=pIServerUserItem->GetBindIndex();
	bool bAndroidUser=pIServerUserItem->IsAndroidUser();
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	//��¼����
	//if (pIServerUserItem->IsMobileUser()==false)
	{
		if ( pIServerUserItem->IsAndroidUser() == false )
		{
			//��������
			CMD_GR_LogonSuccess LogonSuccess;
			CMD_GR_ConfigServerEx ConfigServer;
			ZeroMemory(&LogonSuccess, sizeof(LogonSuccess));
			ZeroMemory(&ConfigServer, sizeof(ConfigServer));

			//��¼�ɹ�
			LogonSuccess.dwUserRight = pIServerUserItem->GetUserRight();
			LogonSuccess.dwMasterRight = pIServerUserItem->GetMasterRight();
			SendData(pBindParameter->dwSocketID, MDM_GR_LOGON, SUB_GR_LOGON_SUCCESS, &LogonSuccess, sizeof(LogonSuccess));

			//��������
			ConfigServer.wTableCount = m_pGameServiceOption->wTableCount;
			ConfigServer.wChairCount = m_pGameServiceAttrib->wChairCount;
			ConfigServer.wServerType = m_pGameServiceOption->wServerType;
			ConfigServer.dwServerRule = m_pGameServiceOption->dwServerRule;
			ConfigServer.lCellScore = m_pGameServiceOption->lCellScore;
			ConfigServer.dwExpValue = m_pGameServiceOption->wRewardExp;
			ConfigServer.dwBloodLower = m_pGameServiceOption->dwBloodLower;

			SendData(pBindParameter->dwSocketID, MDM_GR_CONFIG, SUB_GR_CONFIG_SERVER, &ConfigServer, sizeof(ConfigServer));

			//�б����� 
			WORD wConfigColumnHead = sizeof(m_DataConfigColumn) - sizeof(m_DataConfigColumn.ColumnItem);
			WORD wConfigColumnInfo = m_DataConfigColumn.cbColumnCount * sizeof(m_DataConfigColumn.ColumnItem[0]);
			SendData(pBindParameter->dwSocketID, MDM_GR_CONFIG, SUB_GR_CONFIG_COLUMN, &m_DataConfigColumn, wConfigColumnHead + wConfigColumnInfo);

			//�������
			SendData(pBindParameter->dwSocketID, MDM_GR_CONFIG, SUB_GR_CONFIG_FINISH, NULL, 0);

			//�������
			SendUserInfoPacket(pIServerUserItem, pBindParameter->dwSocketID);

			//�����û�
			WORD wUserIndex = 0;
			IServerUserItem * pIServerUserItemSend = NULL;
			while (true)
			{
				pIServerUserItemSend = m_ServerUserManager.EnumUserItem(wUserIndex++);
				if (pIServerUserItemSend == NULL) break;
				if (pIServerUserItemSend == pIServerUserItem) continue;
				//I_TRACE(TEXT("������� %s ״̬%d,tableID:%d"), pIServerUserItemSend->GetNickName(), pIServerUserItemSend->GetUserStatus(), pIServerUserItemSend->GetTableID());
				SendUserInfoPacket(pIServerUserItemSend, pBindParameter->dwSocketID);
			}

			//����״̬
			CMD_GR_TableInfo TableInfo;
			TableInfo.wTableCount = (WORD)m_TableFrameArray.GetCount();
			ASSERT_ERR(TableInfo.wTableCount < CountArray(TableInfo.TableStatusArray));
			for (WORD i = 0; i < TableInfo.wTableCount; i++)
			{
				CTableFrame * pTableFrame = m_TableFrameArray[i];
				TableInfo.TableStatusArray[i].cbTableLock = pTableFrame->IsTableLocked() ? TRUE : FALSE;
				TableInfo.TableStatusArray[i].cbPlayStatus = pTableFrame->IsTableStarted() ? TRUE : FALSE;
			}

			//����״̬
			WORD wHeadSize = sizeof(TableInfo) - sizeof(TableInfo.TableStatusArray);
			WORD wSendSize = wHeadSize + TableInfo.wTableCount * sizeof(TableInfo.TableStatusArray[0]);
			SendData(pBindParameter->dwSocketID, MDM_GR_STATUS, SUB_GR_TABLE_INFO, &TableInfo, wSendSize);

			//����֪ͨ
			if (bAlreadyOnLine == false)
			{
				SendUserInfoPacket(pIServerUserItem, INVALID_DWORD);
			}
		}

		//��¼���
		//I_TRACE(TEXT("������ҵ�¼nickName:%s,Score:%I64d,IsAndroid:%d"), pIServerUserItem->GetNickName(),pIServerUserItem->GetUserScore() ,pIServerUserItem->IsAndroidUser());
		SendData(pBindParameter->dwSocketID,MDM_GR_LOGON,SUB_GR_LOGON_FINISH,NULL,0);
	}

	//��������
	if (bAndroidUser==false)
	{
		if (pBindParameter->cbClientKind==CLIENT_KIND_MOBILE)
		{
			m_pITCPNetworkEngine->AllowBatchSend(pBindParameter->dwSocketID,true,BG_MOBILE);
		}
		else
		{
			m_pITCPNetworkEngine->AllowBatchSend(pBindParameter->dwSocketID,true,BG_COMPUTER);
		}
	}

	if(m_pIGameMatchServiceManager!=NULL)
	{
		m_pIGameMatchServiceManager->SendMatchInfo(pIServerUserItem);
	}
	return;
}

//�û��뿪
VOID CAttemperEngineSink::OnEventUserLogout(IServerUserItem * pIServerUserItem, DWORD dwLeaveReason)
{
	if ( pIServerUserItem == NULL ) return;

	//��������
	DBR_GR_LeaveGameServer LeaveGameServer;
	ZeroMemory(&LeaveGameServer,sizeof(LeaveGameServer));

	//��ȡ�ɼ�
	pIServerUserItem->QueryRecordInfo(LeaveGameServer.RecordInfo);
	pIServerUserItem->DistillVariation(LeaveGameServer.VariationInfo);

	//�û���Ϣ
	LeaveGameServer.dwLeaveReason=dwLeaveReason;
	LeaveGameServer.dwUserID=pIServerUserItem->GetUserID();
	LeaveGameServer.dwInoutIndex=pIServerUserItem->GetInoutIndex();
	//����ʱ����Ϊ��Ϸʱ��,�Է���Ϊ��λ
	LeaveGameServer.dwOnLineTimeCount=(DWORD)(time(NULL))-pIServerUserItem->GetLogonTime();

	//������Ϣ
	LeaveGameServer.dwClientAddr=pIServerUserItem->GetClientAddr();
	lstrcpyn(LeaveGameServer.szMachineID,pIServerUserItem->GetMachineID(),CountArray(LeaveGameServer.szMachineID));
	
	WORD wBindIndex = pIServerUserItem->GetBindIndex();
	tagBindParameter * pBindParameter = GetBindParameter(wBindIndex);
	if (pBindParameter) {
		I_TRACE(TEXT("Socket[%d], %s �û��뿪"), pBindParameter->dwSocketID, pIServerUserItem->GetNickName());
		m_ServerFileLog.Log(TEXT("���û��뿪д���ݿ⡿|�û�[%d][%s],����[%I64d] ״̬[%d] ����[%d-%d] BindIndex[%d] SocketID[%d]"),
			pIServerUserItem->GetUserID(),pIServerUserItem->GetNickName(),pIServerUserItem->GetUserScore(),
			pIServerUserItem->GetUserStatus(),pIServerUserItem->GetTableID(),pIServerUserItem->GetChairID(),wBindIndex,pBindParameter->dwSocketID);
	}
	else
	{
		CString cs;
		cs.Format(TEXT("�������뿪[%d],BindIndex[%d]"), pIServerUserItem->GetUserID(), pIServerUserItem->GetBindIndex());
		m_ServerFileLog.Log(cs.GetBuffer(0));
		I_TRACE(cs.GetBuffer(0));
	}
	
	//Ͷ������
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_LEAVE_GAME_SERVER,0L,&LeaveGameServer,sizeof(LeaveGameServer), TRUE);

	//�����û�
	if (m_bCollectUser==true)
	{
		//��������
		CMD_CS_C_UserLeave UserLeave;
		ZeroMemory(&UserLeave,sizeof(UserLeave));

		//���ñ���
		UserLeave.dwUserID=pIServerUserItem->GetUserID();

		//������Ϣ
		m_pITCPSocketService->SendData(MDM_CS_USER_COLLECT,SUB_CS_C_USER_LEAVE,&UserLeave,sizeof(UserLeave));
	}

	DeleteDistribute(pIServerUserItem);

	//���������˳���Ͷ�����ݿ�
	if(m_pIGameMatchServiceManager!=NULL)m_pIGameMatchServiceManager->OnUserQuitGame(pIServerUserItem, 1);

	//ɾ���û�
	m_ServerUserManager.DeleteUserItem(pIServerUserItem);

	return;
}

//����Ԫ��
bool CAttemperEngineSink::PerformUnlockScore(DWORD dwUserID, DWORD dwInoutIndex, DWORD dwLeaveReason)
{
	//��������
	DBR_GR_LeaveGameServer LeaveGameServer;
	ZeroMemory(&LeaveGameServer,sizeof(LeaveGameServer));

	//���ñ���
	LeaveGameServer.dwUserID=dwUserID;
	LeaveGameServer.dwInoutIndex=dwInoutIndex;
	LeaveGameServer.dwLeaveReason=dwLeaveReason;

	//Ͷ������
	m_pIDBCorrespondManager->PostDataBaseRequest(dwUserID,DBR_GR_LEAVE_GAME_SERVER,0L,&LeaveGameServer,sizeof(LeaveGameServer));

	return true;
}

//�汾���
bool CAttemperEngineSink::PerformCheckVersion(DWORD dwPlazaVersion, DWORD dwFrameVersion, DWORD dwClientVersion, DWORD dwSocketID)
{
	return true;
}

//�л�����
bool CAttemperEngineSink::SwitchUserItemConnectH5(IServerUserItem * pIServerUserItem, TCHAR szMachineID[LEN_MACHINE_ID], WORD wTargetIndex)
{
	//Ч�����
	ASSERT_ERR((pIServerUserItem != NULL) && (wTargetIndex != INVALID_WORD));
	if ((pIServerUserItem == NULL) || (wTargetIndex == INVALID_WORD)) return false;

	//�Ͽ��û�
	if (pIServerUserItem->GetBindIndex() != INVALID_WORD)
	{
		//�󶨲���
		WORD wSourceIndex = pIServerUserItem->GetBindIndex();
		tagBindParameter * pSourceParameter = GetBindParameter(wSourceIndex);
		tagBindParameter * pTargetParameter = GetBindParameter(wTargetIndex);
		I_TRACE(TEXT("wSourceIndex[%d],SrcSocketID[%d],wTargetIndex[%d],TargetSocketID[%d]"),wSourceIndex, pSourceParameter->dwSocketID,
			wTargetIndex, pTargetParameter->dwSocketID);
		//�����
		//ASSERT_ERR((pSourceParameter != NULL) && (pSourceParameter->pIServerUserItem == pIServerUserItem));
		if ((pSourceParameter != NULL) && (pSourceParameter->pIServerUserItem == pIServerUserItem)) {
			//����֪ͨ
			LPCTSTR pszMessage = TEXT("�����˺�����һ���ط���¼���������뿪!");
			if (pIServerUserItem->IsAndroidUser() == false)
				SendRequestFailureH5(pIServerUserItem, pszMessage, REQUEST_FAILURE_QUIT);
			pSourceParameter->pIServerUserItem = NULL;
		}	

		//�Ͽ��û�
		if (pIServerUserItem->IsAndroidUser() == true)
		{
			//m_AndroidUserManager.DeleteAndroidUserItem(pSourceParameter->dwSocketID);
		}
		else
		{
			I_TRACE(TEXT("�رվ�����wSourceIndex:%d,dwSocketID:%d"), wSourceIndex, pSourceParameter->dwSocketID);
			//if ( m_pGameServiceOption->wKindID != KIND_ZJH)
			//m_pITCPNetworkEngine->ShutDownSocket(pSourceParameter->dwSocketID);
		}
	}

	//��������
	bool bAndroidUser = (wTargetIndex >= INDEX_ANDROID);
	tagBindParameter * pTargetParameter = GetBindParameter(wTargetIndex);

	//�����û�
	I_TRACE(TEXT("���������� wTargetIndex:%d,dwSocketID:%d"), wTargetIndex, pTargetParameter->dwSocketID);
	pTargetParameter->pIServerUserItem = pIServerUserItem;
	pIServerUserItem->SetUserParameter(pTargetParameter->dwClientAddr, wTargetIndex, szMachineID, bAndroidUser, false);

	//��¼�¼�
	OnEventUserLogonH5(pIServerUserItem, true);

	//״̬�л�
	bool bIsOffLine = false;
	//if (pIServerUserItem->GetUserStatus() == US_OFFLINE || pIServerUserItem->GetUserStatus() == US_PLAYING)
	if (pIServerUserItem->GetUserStatus() >= US_PLAYING && m_pGameServiceOption->wKindID == KIND_FISH)
	{
		//��������
		WORD wTableID = pIServerUserItem->GetTableID();
		WORD wChairID = pIServerUserItem->GetChairID();

		pIServerUserItem->SetHeartTime((DWORD)time(NULL));
		I_TRACE(TEXT("������� %s %d-%d Status:%d"), pIServerUserItem->GetNickName(),pIServerUserItem->GetTableID(),pIServerUserItem->GetChairID(),pIServerUserItem->GetUserStatus());
		//����״̬
		bIsOffLine = true;
		pIServerUserItem->SetEnterTable(FALSE);
		pIServerUserItem->SetUserStatus(US_PLAYING, wTableID, wChairID);
	}

	//�����ж�
	LPCTSTR pszMachineID = pIServerUserItem->GetMachineID();
	bool bSameMachineID = (lstrcmp(pszMachineID, szMachineID) == 0);

	//��ȫ��ʾ
	if ((bAndroidUser == false) && (bIsOffLine == false) && (bSameMachineID == false))
	{
		//���ø�֪�Լ�
		//SendRoomMessage(pIServerUserItem, TEXT("��ע�⣬�����ʺ�����һ�ط������˴���Ϸ���䣬�Է������뿪��"), SMT_EJECT | SMT_CHAT | SMT_GLOBAL);
	}

	return true;
}

//�л�����
bool CAttemperEngineSink::SwitchUserItemConnect(IServerUserItem * pIServerUserItem, TCHAR szMachineID[LEN_MACHINE_ID], WORD wTargetIndex,BYTE cbDeviceType,WORD wBehaviorFlags,WORD wPageTableCount)
{
	//Ч�����
	ASSERT_ERR((pIServerUserItem!=NULL)&&(wTargetIndex!=INVALID_WORD));
	if ((pIServerUserItem==NULL)||(wTargetIndex==INVALID_WORD)) return false;

	//�Ͽ��û�
	if (pIServerUserItem->GetBindIndex()!=INVALID_WORD)
	{
		//����֪ͨ
		LPCTSTR pszMessage=TEXT("��ע�⣬�����ʺ�����һ�ط������˴���Ϸ���䣬�������뿪��");
		if (pIServerUserItem->IsAndroidUser() == false )
			SendRoomMessage(pIServerUserItem,pszMessage,SMT_CHAT|SMT_EJECT|SMT_GLOBAL|SMT_CLOSE_ROOM);

		//�󶨲���
		WORD wSourceIndex=pIServerUserItem->GetBindIndex();
		tagBindParameter * pSourceParameter=GetBindParameter(wSourceIndex);

		//�����
		ASSERT_ERR((pSourceParameter!=NULL)&&(pSourceParameter->pIServerUserItem==pIServerUserItem));
		if ((pSourceParameter!=NULL)&&(pSourceParameter->pIServerUserItem==pIServerUserItem)) pSourceParameter->pIServerUserItem=NULL;

		//�Ͽ��û�
		if (pIServerUserItem->IsAndroidUser()==true)
		{
			//m_AndroidUserManager.DeleteAndroidUserItem(pSourceParameter->dwSocketID);
		}
		else
		{
			m_pITCPNetworkEngine->ShutDownSocket(pSourceParameter->dwSocketID);
		}
	}

	//״̬�л�
	bool bIsOffLine=false;
	if (pIServerUserItem->GetUserStatus()==US_OFFLINE)
	{
		//��������
		WORD wTableID=pIServerUserItem->GetTableID();
		WORD wChairID=pIServerUserItem->GetChairID();

		pIServerUserItem->SetHeartTime((DWORD)time(NULL));
		//����״̬
		bIsOffLine=true;
		pIServerUserItem->SetUserStatus(US_PLAYING,wTableID,wChairID);
	}

	//�����ж�
	LPCTSTR pszMachineID=pIServerUserItem->GetMachineID();
	bool bSameMachineID=(lstrcmp(pszMachineID,szMachineID)==0);

	//��������
	bool bAndroidUser=(wTargetIndex>=INDEX_ANDROID);
	tagBindParameter * pTargetParameter=GetBindParameter(wTargetIndex);

	//�����û�
	pTargetParameter->pIServerUserItem=pIServerUserItem;
	pIServerUserItem->SetUserParameter(pTargetParameter->dwClientAddr,wTargetIndex,szMachineID,bAndroidUser,false);

	//�ֻ���ʶ
	if(pTargetParameter->cbClientKind==CLIENT_KIND_MOBILE)
	{
		pIServerUserItem->SetMobileUser(true);
		SetMobileUserParameter(pIServerUserItem,cbDeviceType,wBehaviorFlags,wPageTableCount);
	}

	//��¼�¼�
	OnEventUserLogon(pIServerUserItem,true);

	//��ȫ��ʾ
	if ((bAndroidUser==false)&&(bIsOffLine==false)&&(bSameMachineID==false))
	{
		SendRoomMessage(pIServerUserItem,TEXT("��ע�⣬�����ʺ�����һ�ط������˴���Ϸ���䣬�Է������뿪��"),SMT_EJECT|SMT_CHAT|SMT_GLOBAL);
	}

	return true;
}

//��¼ʧ��
bool CAttemperEngineSink::SendLogonFailure(LPCTSTR pszString, LONG lErrorCode, DWORD dwSocketID)
{
	tagBindParameter * pBindParameter = GetBindParameter(LOWORD(dwSocketID));
	if ( pBindParameter )
	{
		if ( pBindParameter->cbClientKind == CLIENT_KIND_WEBSOCKET)
		{
			GameServer::GSLogonFailure gsLogonFailure;
			gsLogonFailure.set_errorcode(lErrorCode);
			gsLogonFailure.set_describestring(Utils::ConvertFromUtf16ToUtf8(pszString).c_str());

			std::string serializeStr;
			gsLogonFailure.SerializeToString(&serializeStr);

			

			//�ͻ��˶Ͽ�
			//m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);
			IServerUserItem* pIServerUserItem = pBindParameter->pIServerUserItem;
			if (pIServerUserItem)
				pIServerUserItem->SetUserStatus(US_NULL, INVALID_TABLE, INVALID_CHAIR);

			SendData(dwSocketID, MDM_GR_LOGON, SUB_GR_LOGON_FAILURE, (void*)serializeStr.c_str(), (WORD)serializeStr.size());
			//OnEventUserLogout(pBindParameter->pIServerUserItem, 0L);
		}
		else if (pBindParameter->cbClientKind == CLIENT_KIND_MOBILE)
		{
			//��������
			CMD_GR_LogonFailure LogonFailure;
			ZeroMemory(&LogonFailure, sizeof(LogonFailure));

			//��������
			LogonFailure.lErrorCode = lErrorCode;
			lstrcpyn(LogonFailure.szDescribeString, pszString, CountArray(LogonFailure.szDescribeString));

			//��������
			WORD wDataSize = CountStringBuffer(LogonFailure.szDescribeString);
			WORD wHeadSize = sizeof(LogonFailure) - sizeof(LogonFailure.szDescribeString);

			//��������
			SendData(dwSocketID, MDM_GR_LOGON, SUB_GR_LOGON_FAILURE, &LogonFailure, wHeadSize + wDataSize);

			m_pITCPNetworkEngine->ShutDownSocket(pBindParameter->dwSocketID);
		}
	}

	return true;
}

//����ʧ��
bool CAttemperEngineSink::SendInsureFailure(IServerUserItem * pIServerUserItem, LPCTSTR pszString, LONG lErrorCode,BYTE cbActivityGame)
{
	//Ч�����
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	CMD_GR_S_UserInsureFailure UserInsureFailure;
	ZeroMemory(&UserInsureFailure,sizeof(UserInsureFailure));

	//��������
	UserInsureFailure.cbActivityGame=cbActivityGame;
	UserInsureFailure.lErrorCode=lErrorCode;
	lstrcpyn(UserInsureFailure.szDescribeString,pszString,CountArray(UserInsureFailure.szDescribeString));

	//��������
	WORD wDataSize=CountStringBuffer(UserInsureFailure.szDescribeString);
	WORD wHeadSize=sizeof(UserInsureFailure)-sizeof(UserInsureFailure.szDescribeString);

	//��������
	SendData(pIServerUserItem,MDM_GR_INSURE,SUB_GR_USER_INSURE_FAILURE,&UserInsureFailure,wHeadSize+wDataSize);

	return true;
}

//����ʧ��
bool CAttemperEngineSink::SendRequestFailure(IServerUserItem * pIServerUserItem, LPCTSTR pszDescribe, LONG lErrorCode)
{
	//��������
	CMD_GR_RequestFailure RequestFailure;
	ZeroMemory(&RequestFailure,sizeof(RequestFailure));

	//���ñ���
	RequestFailure.lErrorCode=lErrorCode;
	lstrcpyn(RequestFailure.szDescribeString,pszDescribe,CountArray(RequestFailure.szDescribeString));

	//��������
	WORD wDataSize=CountStringBuffer(RequestFailure.szDescribeString);
	WORD wHeadSize=sizeof(RequestFailure)-sizeof(RequestFailure.szDescribeString);
	SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_REQUEST_FAILURE,&RequestFailure,wHeadSize+wDataSize);

	return true;
}

//����ʧ��
bool CAttemperEngineSink::SendRequestFailureH5(IServerUserItem * pIServerUserItem, LPCTSTR pszDescribe, LONG lErrorCode)
{
	GameServer::GSUserRequestFailure gsUserRequestFailure;
	gsUserRequestFailure.set_errorcode(lErrorCode);
	gsUserRequestFailure.set_describestring(Utils::ConvertFromUtf16ToUtf8(pszDescribe).c_str());

	std::string serializeStr;
	gsUserRequestFailure.SerializeToString(&serializeStr);

	SendData(pIServerUserItem, MDM_GR_USER, SUB_GR_REQUEST_FAILURE, (void*)serializeStr.c_str(), (WORD)serializeStr.size());


	if ( lErrorCode == REQUEST_FAILURE_NETWORK)
	{
		tagBindParameter * pBindParameter = GetBindParameter(pIServerUserItem->GetBindIndex());
		if (pBindParameter == NULL) return true;
		//����ʧ�ܣ��Ͽ�����
		if (pBindParameter->cbClientKind == CLIENT_KIND_WEBSOCKET)
		{
			m_pITCPNetworkEngine->ShutDownSocket(pBindParameter->dwSocketID);
		}
	}
	return true;
}

//����ʧ��
bool CAttemperEngineSink::SendPropertyFailure(IServerUserItem * pIServerUserItem, LPCTSTR pszDescribe, LONG lErrorCode,WORD wRequestArea)
{
	//��������
	CMD_GR_PropertyFailure PropertyFailure;
	ZeroMemory(&PropertyFailure,sizeof(PropertyFailure));

	//���ñ���
	PropertyFailure.lErrorCode=lErrorCode;
	PropertyFailure.wRequestArea=wRequestArea;
	lstrcpyn(PropertyFailure.szDescribeString,pszDescribe,CountArray(PropertyFailure.szDescribeString));

	//��������
	WORD wDataSize=CountStringBuffer(PropertyFailure.szDescribeString);
	WORD wHeadSize=sizeof(PropertyFailure)-sizeof(PropertyFailure.szDescribeString);
	SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_PROPERTY_FAILURE,&PropertyFailure,wHeadSize+wDataSize);

	return true;
}

//�����û�
bool CAttemperEngineSink::SendUserInfoPacketH5(IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
	if (pIServerUserItem == NULL) return false;

	tagUserInfo* pUserInfo = pIServerUserItem->GetUserInfo();
	GameServer::GSUserEnter gsUserEnter;
	gsUserEnter.set_userid((int32)pUserInfo->dwUserID);
	gsUserEnter.set_faceid((int32)pUserInfo->wFaceID);
	gsUserEnter.set_gender((int32)pUserInfo->cbGender);
	gsUserEnter.set_score((int64)pUserInfo->lScore);
	gsUserEnter.set_nickname(Utils::ConvertFromUtf16ToUtf8(pUserInfo->szNickName));
	gsUserEnter.mutable_userstatus()->set_tableid((int32)pUserInfo->wTableID);
	gsUserEnter.mutable_userstatus()->set_chairid((int32)pUserInfo->wChairID);
	gsUserEnter.mutable_userstatus()->set_userstatus((GameServer::enumUserStatus)pUserInfo->cbUserStatus);
	
	std::string serializeStr;
	gsUserEnter.SerializeToString(&serializeStr);
	SendData(dwSocketID, MDM_GR_USER, SUB_GR_USER_ENTER, (void*)serializeStr.c_str(), (WORD)serializeStr.size());

	return true;
}

//�����û�
bool CAttemperEngineSink::SendUserInfoPacket(IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
	//Ч�����
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

// 	tagUserInfo* ptagUserInfo = pIServerUserItem->GetUserInfo();
// 	if (ptagUserInfo->cbClientKind == CLIENT_KIND_WEBSOCKET) {
// 		SendUserInfoPacketH5(pIServerUserItem, dwSocketID);
// 		return true;
// 	}

	//��������
	BYTE cbBuffer[SOCKET_TCP_PACKET];
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();
	tagUserInfoHead * pUserInfoHead=(tagUserInfoHead *)cbBuffer;
	CSendPacketHelper SendPacket(cbBuffer+sizeof(tagUserInfoHead),sizeof(cbBuffer)-sizeof(tagUserInfoHead));

	//�û�����
	pUserInfoHead->wFaceID=pUserInfo->wFaceID;
	pUserInfoHead->dwGameID=pUserInfo->dwGameID;
	pUserInfoHead->dwUserID=pUserInfo->dwUserID;
	pUserInfoHead->dwGroupID=pUserInfo->dwGroupID;
	pUserInfoHead->dwCustomID=pUserInfo->dwCustomID;

	//�û�����
	pUserInfoHead->cbGender=pUserInfo->cbGender;
	pUserInfoHead->cbMemberOrder=pUserInfo->cbMemberOrder;
	pUserInfoHead->cbMasterOrder=pUserInfo->cbMasterOrder;

	//�û�״̬
	pUserInfoHead->wTableID=pUserInfo->wTableID;
	pUserInfoHead->wChairID=pUserInfo->wChairID;
	pUserInfoHead->cbUserStatus=pUserInfo->cbUserStatus;

	//�û�����
	pUserInfoHead->dwWinCount=pUserInfo->dwWinCount;
	pUserInfoHead->dwLostCount=pUserInfo->dwLostCount;
	pUserInfoHead->dwDrawCount=pUserInfo->dwDrawCount;
	pUserInfoHead->dwFleeCount=pUserInfo->dwFleeCount;
	pUserInfoHead->dwUserMedal=pUserInfo->dwUserMedal;
	pUserInfoHead->dwExperience=pUserInfo->dwExperience;
	pUserInfoHead->lLoveLiness=pUserInfo->lLoveLiness;

	//�û�����
	pUserInfoHead->lGrade=pUserInfo->lGrade;
	pUserInfoHead->lInsure=pUserInfo->lInsure;

	//�û��ɼ�
	pUserInfoHead->lScore=pUserInfo->lScore;
// 	if ( m_pGameServiceOption->dwRoomID == 0)
// 	{
// 		pUserInfoHead->lScore+=pIServerUserItem->GetTrusteeScore();
// 		pUserInfoHead->lScore+=pIServerUserItem->GetFrozenedScore();
// 	}
	pUserInfoHead->lRevenue			= pUserInfo->lRevenue;
	pUserInfoHead->dwAvatarID		= pUserInfo->wAvatarID;
	pUserInfoHead->lRechargeScore = pUserInfo->lRechargeScore;
	pUserInfoHead->lExchangeScore = pUserInfo->lExchangeScore;

	//������Ϣ
	SendPacket.AddPacket(pUserInfo->szNickName,DTP_GR_NICK_NAME);
	SendPacket.AddPacket(pUserInfo->szGroupName,DTP_GR_GROUP_NAME);
	SendPacket.AddPacket(pUserInfo->szUnderWrite,DTP_GR_UNDER_WRITE);
	
	//��������
	if (dwSocketID==INVALID_DWORD)
	{
		WORD wHeadSize=sizeof(tagUserInfoHead);
		SendData(BG_COMPUTER, MDM_GR_USER,SUB_GR_USER_ENTER,cbBuffer,wHeadSize+SendPacket.GetDataSize());
		SendData(BG_MOBILE, MDM_GR_USER, SUB_GR_USER_ENTER, cbBuffer, wHeadSize + SendPacket.GetDataSize());
		SendUserInfoPacketBatchToMobileUser(pIServerUserItem);
	}
	else
	{
		WORD wHeadSize=sizeof(tagUserInfoHead);
		SendData(dwSocketID,MDM_GR_USER,SUB_GR_USER_ENTER,cbBuffer,wHeadSize+SendPacket.GetDataSize());
	}

	return true;
}

//�㲥����
bool CAttemperEngineSink::SendPropertyMessage(DWORD dwSourceID,DWORD dwTargerID,WORD wPropertyIndex,WORD wPropertyCount)
{
	//����ṹ
	CMD_GR_S_PropertyMessage  PropertyMessage;
	PropertyMessage.wPropertyIndex=wPropertyIndex;
	PropertyMessage.dwSourceUserID=dwSourceID;
	PropertyMessage.dwTargerUserID=dwTargerID;
	PropertyMessage.wPropertyCount=wPropertyCount;

	//�����û�
	WORD wUserIndex=0;
	IServerUserItem * pIServerUserItemSend=NULL;
	while (true)
	{
		pIServerUserItemSend=m_ServerUserManager.EnumUserItem(wUserIndex++);
		if (pIServerUserItemSend==NULL) break;
		SendData(pIServerUserItemSend,MDM_GR_USER,SUB_GR_PROPERTY_MESSAGE,&PropertyMessage,sizeof(PropertyMessage));
	}

	return true;
}

//����ЧӦ
bool CAttemperEngineSink::SendPropertyEffect(IServerUserItem * pIServerUserItem)
{
	//����У��
	if(pIServerUserItem==NULL) return false;

	//����ṹ
	CMD_GR_S_PropertyEffect  PropertyEffect;
	PropertyEffect.wUserID =pIServerUserItem->GetUserID();
	PropertyEffect.cbMemberOrder=pIServerUserItem->GetMemberOrder();

	//�����û�
	WORD wUserIndex=0;
	IServerUserItem * pIServerUserItemSend=NULL;
	while (true)
	{
		pIServerUserItemSend=m_ServerUserManager.EnumUserItem(wUserIndex++);
		if (pIServerUserItemSend==NULL) break;
		SendData(pIServerUserItemSend,MDM_GR_USER,SUB_GR_PROPERTY_EFFECT,&PropertyEffect,sizeof(PropertyEffect));
	}

	return true;
}

//�����¼�
bool CAttemperEngineSink::OnEventPropertyBuyPrep(WORD cbRequestArea,WORD wPropertyIndex,IServerUserItem *pISourceUserItem,IServerUserItem *pTargetUserItem)
{
	//Ŀ�����
	if ( pTargetUserItem == NULL )
	{
		//������Ϣ
		SendPropertyFailure(pISourceUserItem,TEXT("����ʧ�ܣ���Ҫ���͵�����Ѿ��뿪��"), 0L,cbRequestArea);

		return false;
	}

	//�����ж�
	if ( (m_pGameServiceOption->wServerType == GAME_GENRE_GOLD || m_pGameServiceOption->wServerType == GAME_GENRE_MATCH ) && 
		(wPropertyIndex== PROPERTY_ID_SCORE_CLEAR||wPropertyIndex==PROPERTY_ID_TWO_CARD||wPropertyIndex == PROPERTY_ID_FOUR_CARD||wPropertyIndex == PROPERTY_ID_POSSESS) )
	{
		//������Ϣ
		SendPropertyFailure(pISourceUserItem,TEXT("�˷��䲻����ʹ�ô˵���,����ʧ��"), 0L,cbRequestArea);

		return false;
	}

	//���ҵ���
	tagPropertyInfo * pPropertyInfo=m_GamePropertyManager.SearchPropertyItem(wPropertyIndex);

	//��ЧЧ��
	if(pPropertyInfo==NULL)
	{
		//������Ϣ
		SendPropertyFailure(pISourceUserItem,TEXT("�˵��߻�δ����,����ʧ�ܣ�"), 0L,cbRequestArea);

		return false;
	}

	//�Լ�ʹ��
	if((pPropertyInfo->wIssueArea&PT_SERVICE_AREA_MESELF)==0 && pISourceUserItem==pTargetUserItem) 
	{
		//������Ϣ
		SendPropertyFailure(pISourceUserItem,TEXT("�˵��߲����Լ�ʹ��,����ʧ�ܣ�"), 0L,cbRequestArea);

		return false;
	}

	//���ʹ��
	if((pPropertyInfo->wIssueArea&PT_SERVICE_AREA_PLAYER)==0 && pISourceUserItem!=pTargetUserItem) 
	{
		//������Ϣ
		SendPropertyFailure(pISourceUserItem,TEXT("�˵��߲������͸����,ֻ���Լ�ʹ��,����ʧ�ܣ�"), 0L,cbRequestArea);

		return false;
	}

	//�Թ۷�Χ
	if((pPropertyInfo->wIssueArea&PT_SERVICE_AREA_LOOKON)==0)  
	{
		//��������
		WORD wTableID = pTargetUserItem->GetTableID();
		if(wTableID!=INVALID_TABLE)
		{
			//��������
			WORD wEnumIndex=0;
			IServerUserItem * pIServerUserItem=NULL;

			//��ȡ����
			CTableFrame * pTableFrame=m_TableFrameArray[wTableID];

			//ö���û�
			do
			{
				//��ȡ�û�
				pIServerUserItem=pTableFrame->EnumLookonUserItem(wEnumIndex++);
				if( pIServerUserItem==NULL) break;
				if( pIServerUserItem==pTargetUserItem )
				{
					//������Ϣ
					SendPropertyFailure(pISourceUserItem,TEXT("�˵��߲������͸��Թ��û�,����ʧ�ܣ�"), 0L,cbRequestArea);

					return false;
				}
			} while (true);
		}
	}

	//�����ж�
	switch(wPropertyIndex)
	{
	case PROPERTY_ID_SCORE_CLEAR :			//��������
		{
			//��������
			SCORE lCurrScore = pTargetUserItem->GetUserScore();
			if( lCurrScore >= 0)
			{
				//��������
				TCHAR szMessage[128]=TEXT("");
				if ( pISourceUserItem==pTargetUserItem ) 
					_sntprintf(szMessage,CountArray(szMessage),TEXT("�����ڵĻ����Ѿ��ǷǸ���������Ҫʹ�ø���������ߣ�"));
				else
					_sntprintf(szMessage,CountArray(szMessage),TEXT("[ %s ]���ڵĻ����Ѿ��ǷǸ���������Ҫʹ�ø���������ߣ�"), pTargetUserItem->GetNickName());

				//������Ϣ
				SendPropertyFailure(pISourceUserItem,szMessage, 0L,cbRequestArea);

				return false;
			}
			break;
		}
	case PROPERTY_ID_ESCAPE_CLEAR :			 //��������
		{
			//��������
			DWORD dwCurrFleeCount = pTargetUserItem->GetUserInfo()->dwFleeCount;
			if ( dwCurrFleeCount==0 )
			{
				//��������
				TCHAR szMessage[128]=TEXT("");		
				if ( pISourceUserItem == pTargetUserItem ) 
					_sntprintf(szMessage,CountArray(szMessage),TEXT("�����ڵ��������Ѿ�Ϊ0������Ҫʹ������������ߣ�"));
				else
					_sntprintf(szMessage,CountArray(szMessage),TEXT("[ %s ]���ڵ��������Ѿ�Ϊ0������Ҫʹ������������ߣ�"), pTargetUserItem->GetNickName());

				//������Ϣ
				SendPropertyFailure(pISourceUserItem,szMessage,0L,cbRequestArea);

				return false;
			}
			break;
		}
	}

	return true;
}

//���û�
IServerUserItem * CAttemperEngineSink::GetBindUserItem(WORD wBindIndex)
{
	//��ȡ����
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	//��ȡ�û�
	if (pBindParameter!=NULL)
	{
		return pBindParameter->pIServerUserItem;
	}

	//�������
	ASSERT_ERR(FALSE);

	return NULL;
}

//�󶨲���
tagBindParameter * CAttemperEngineSink::GetBindParameter(WORD wBindIndex)
{
	//��Ч����
	if (wBindIndex==INVALID_WORD) return NULL;

	//��������
	if (wBindIndex<m_pGameServiceOption->wMaxPlayer)
	{
		return m_pNormalParameter+wBindIndex;
	}

	//��������
	if ((wBindIndex>=INDEX_ANDROID)&&(wBindIndex<(INDEX_ANDROID+MAX_ANDROID)))
	{
		return m_pAndroidParameter+(wBindIndex-INDEX_ANDROID);
	}

	//�������
	ASSERT_ERR(FALSE);

	return NULL;
}

//��������
WORD CAttemperEngineSink::GetPropertyType(WORD wPropertyIndex)
{
	switch(wPropertyIndex)
	{
	case PROPERTY_ID_CAR:	case PROPERTY_ID_EGG: 	case PROPERTY_ID_CLAP: 	case PROPERTY_ID_KISS: 	case PROPERTY_ID_BEER:
	case PROPERTY_ID_CAKE: 	case PROPERTY_ID_RING:  case PROPERTY_ID_BEAT: 	case PROPERTY_ID_BOMB:  case PROPERTY_ID_SMOKE:
	case PROPERTY_ID_VILLA: case PROPERTY_ID_BRICK: case PROPERTY_ID_FLOWER: 
		{
			return PT_TYPE_PRESENT;
		};
	case PROPERTY_ID_TWO_CARD: 	case PROPERTY_ID_FOUR_CARD:  case PROPERTY_ID_SCORE_CLEAR:     case PROPERTY_ID_ESCAPE_CLEAR:
	case PROPERTY_ID_TRUMPET:	case PROPERTY_ID_TYPHON:     case PROPERTY_ID_GUARDKICK_CARD:  case PROPERTY_ID_POSSESS:
	case PROPERTY_ID_BLUERING_CARD: case PROPERTY_ID_YELLOWRING_CARD: case PROPERTY_ID_WHITERING_CARD: case PROPERTY_ID_REDRING_CARD:
	case PROPERTY_ID_VIPROOM_CARD: 
		{
			return PT_TYPE_PROPERTY;
		};
	}

	ASSERT_ERR(false);

	return PT_TYPE_ERROR;
}

//���û���
bool CAttemperEngineSink::InitAndroidUser()
{
	//��������
	tagAndroidUserParameter AndroidUserParameter;
	ZeroMemory(&AndroidUserParameter,sizeof(AndroidUserParameter));

	//���ò���
	AndroidUserParameter.pGameParameter=m_pGameParameter;
	AndroidUserParameter.pGameServiceAttrib=m_pGameServiceAttrib;
	AndroidUserParameter.pGameServiceOption=m_pGameServiceOption;

	//�������
	AndroidUserParameter.pITimerEngine=m_pITimerEngine;
	AndroidUserParameter.pIServerUserManager=&m_ServerUserManager;
	AndroidUserParameter.pIGameServiceManager=m_pIGameServiceManager;
	AndroidUserParameter.pITCPNetworkEngineEvent=QUERY_OBJECT_PTR_INTERFACE(m_pIAttemperEngine,ITCPNetworkEngineEvent);
	AndroidUserParameter.pIDabaBaseEngine=m_pIKernelDataBaseEngine;

	CopyMemory(AndroidUserParameter.guiRoom, m_guidRoom, 40 * sizeof WCHAR);

	//���ö���
	if (m_AndroidUserManager.InitAndroidUser(AndroidUserParameter)==false)
	{
		return false;
	}

	return true;
}

//��������
bool CAttemperEngineSink::InitTableFrameArray()
{
	//���Ӳ���
	tagTableFrameParameter TableFrameParameter;
	ZeroMemory(&TableFrameParameter,sizeof(TableFrameParameter));

	//�ں����
	TableFrameParameter.pITimerEngine=m_pITimerEngine;
	TableFrameParameter.pIKernelDataBaseEngine=m_pIKernelDataBaseEngine;
	TableFrameParameter.pIRecordDataBaseEngine=m_pIRecordDataBaseEngine;

	//�������
	TableFrameParameter.pIMainServiceFrame=this;
	TableFrameParameter.pIAndroidUserManager=&m_AndroidUserManager;
	TableFrameParameter.pIGameServiceManager=m_pIGameServiceManager;

	//���ò���
	TableFrameParameter.pGameParameter=m_pGameParameter;
	TableFrameParameter.pGameServiceAttrib=m_pGameServiceAttrib;
	TableFrameParameter.pGameServiceOption=m_pGameServiceOption;

	if(m_pIGameMatchServiceManager!=NULL)
		TableFrameParameter.pIGameMatchServiceManager=QUERY_OBJECT_PTR_INTERFACE(m_pIGameMatchServiceManager,IUnknownEx);

	//��������
	m_TableFrameArray.SetSize(m_pGameServiceOption->wTableCount);
	ZeroMemory(m_TableFrameArray.GetData(),m_pGameServiceOption->wTableCount*sizeof(CTableFrame *));

	//��������
	for (WORD i=0;i<m_pGameServiceOption->wTableCount;i++)
	{
		//��������
		m_TableFrameArray[i]=new CTableFrame;

		//��������
		if (m_TableFrameArray[i]->InitializationFrame(i,TableFrameParameter)==false)
		{
			return false;
		}

		if(m_pIGameMatchServiceManager!=NULL)
			m_pIGameMatchServiceManager->InitTableFrame(QUERY_OBJECT_PTR_INTERFACE((m_TableFrameArray[i]),ITableFrame),i);

		m_AndroidUserManager.InitTableFrame(QUERY_OBJECT_PTR_INTERFACE((m_TableFrameArray[i]),ITableFrame),i);
	}

	if(m_pIGameMatchServiceManager!=NULL)
	{
		if (m_pIGameMatchServiceManager->InitMatchInterface(QUERY_OBJECT_PTR_INTERFACE(m_pIAttemperEngine,ITCPNetworkEngineEvent),m_pIKernelDataBaseEngine,
			(IServerUserManager*)QUERY_OBJECT_INTERFACE(m_ServerUserManager,IServerUserManager),this,m_pITimerEngine,&m_AndroidUserManager, m_guidRoom)==false)
		{
			ASSERT_ERR(FALSE);
			return false;
		}
		// 		if (m_pIGameMatchServiceManager->InitServerUserManager()==false)
		// 		{
		// 			ASSERT_ERR(FALSE);
		// 			return false;
		// 		}
		// 
		// 		if(m_pIGameMatchServiceManager->InitMainServiceFrame(QUERY_ME_INTERFACE(IMainServiceFrame))==false)
		// 		{
		// 			ASSERT_ERR(FALSE);
		// 			return false;
		// 		}

	}

	return true;
}

bool CAttemperEngineSink::SendServerInfo()
{
	CP_ControlResult ControlResult;
	ControlResult.cbSuccess=ER_SUCCESS;
	SendUIControlPacket(UI_SERVICE_CONFIG_RESULT,&ControlResult,sizeof(ControlResult));

	return true;
}

bool CAttemperEngineSink::SendTaskComplete(DWORD dwUserID, TCHAR* szEventCode)
{
	//m_task.DoJob(dwUserID, szEventCode);
	return true;
}

//��������
bool CAttemperEngineSink::SendUIControlPacket(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	//��������
	CServiceUnits * pServiceUnits=CServiceUnits::g_pServiceUnits;
	pServiceUnits->PostControlRequest(wRequestID,pData,wDataSize);

	return true;
}

//�������
bool CAttemperEngineSink::InsertDistribute(IServerUserItem * pIServerUserItem)
{
	//Ч�����
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//״̬�ж�
	ASSERT_ERR(pIServerUserItem->GetUserStatus()<US_PLAYING);
	if (pIServerUserItem->GetUserStatus()>=US_PLAYING) return false;


	//��������
	tagDistributeInfo DistributeInfo;
	ZeroMemory(&DistributeInfo,sizeof(DistributeInfo));

	//���ñ���
	DistributeInfo.pIServerUserItem=pIServerUserItem;
	DistributeInfo.wLastTableID=pIServerUserItem->GetTableID();
	DistributeInfo.pPertainNode=NULL;

	//��������
	if(m_DistributeManage.InsertDistributeNode(DistributeInfo)==false)
	{
		//PerformDistribute();
		return pIServerUserItem->IsAndroidUser()?false:true; 
	}

	//�û�����
	if (pIServerUserItem->GetTableID()!=INVALID_TABLE)
	{
		WORD wTableID=pIServerUserItem->GetTableID();
		m_TableFrameArray[wTableID]->PerformStandUpAction(pIServerUserItem);
	}

	//PerformDistribute(); //�ɶ�ʱ��ִ��

	return true;
}

//ɾ������
bool CAttemperEngineSink::DeleteDistribute(IServerUserItem * pIServerUserItem)
{
	m_DistributeManage.RemoveDistributeNode(pIServerUserItem);
	return true;
}

//ִ�з���
bool CAttemperEngineSink::PerformDistribute()
{
	//����У��
	if(m_DistributeManage.GetCount()<1) return false;

	//�����û�
	while(true)
	{
		//����У��
		if(m_DistributeManage.GetCount()<1) break;

		//��������
		CDistributeInfoArray DistributeInfoArray;

		//��ȡ�û�
		WORD wRandCount = __max(m_pGameServiceAttrib->wChairCount,1);
		WORD wChairCount = rand()%wRandCount;
		WORD wDistributeCount = m_DistributeManage.PerformDistribute(DistributeInfoArray,wChairCount);
		//if(wDistributeCount < m_pGameServiceOption->wMinDistributeUser) break;

		CString szFileName;
		WORD wTableCount=0;

		wTableCount = m_TableFrameArray.GetCount();
		//Ѱ��λ��
		CTableFrame * pCurrTableFrame=NULL;
		for (WORD i=0;i<wTableCount;i++)
		{
			//��ȡ����
			ASSERT_ERR(m_TableFrameArray[i]!=NULL);
			CTableFrame * pTableFrame=m_TableFrameArray[i];
			if ( pTableFrame == NULL ) continue;

			bool bDynamicJoin = true;
			if (m_pGameServiceAttrib->cbDynamicJoin == FALSE) bDynamicJoin = false;
			if (CServerRule::IsAllowDynamicJoin(m_pGameServiceOption->dwServerRule) == false) bDynamicJoin = false;
			if (pTableFrame->IsGameStarted() && bDynamicJoin == false) continue;

			//״̬�ж�
			tagTableUserInfo userInfo = {0};
			DWORD userCount = pTableFrame->GetTableUserInfo(userInfo);

			if (userCount < m_pGameServiceAttrib->wChairCount && userInfo.wTableUserCount != 0)
			{
				pCurrTableFrame = pTableFrame;
				break;
			}
		}

		if (pCurrTableFrame == NULL) break;
		

		//�������
		bool bSitSuccess=true;
		INT_PTR nSitFailedIndex=INVALID_CHAIR;
		for(INT_PTR nIndex=0;nIndex<DistributeInfoArray.GetCount();nIndex++)
		{
			//��������
			WORD wChairID=pCurrTableFrame->GetNullChairID();

			//�����û�
			if (wChairID!=INVALID_CHAIR)
			{
				//��ȡ�û�
				IServerUserItem * pIServerUserItem=DistributeInfoArray[nIndex].pIServerUserItem;

				//�û�����
				if(pCurrTableFrame->PerformSitDownAction(wChairID,pIServerUserItem)==false)
				{
					bSitSuccess=false;
					nSitFailedIndex=nIndex;
					break;
				}
			}
		}

		//���½��
		if(bSitSuccess)
		{
			//�Ƴ����
			while(DistributeInfoArray.GetCount()>0)
			{
				m_DistributeManage.RemoveDistributeNode((tagDistributeNode *)DistributeInfoArray[0].pPertainNode);
				DistributeInfoArray.RemoveAt(0);
			}
		}
		else
		{
			for(INT_PTR nIndex=0;nIndex<DistributeInfoArray.GetCount();nIndex++)
			{
				if(nSitFailedIndex==nIndex)
				{
					m_DistributeManage.RemoveDistributeNode((tagDistributeNode *)DistributeInfoArray[nIndex].pPertainNode);
				}
				else
				{
					//�û�����
					if (DistributeInfoArray[nIndex].pIServerUserItem->GetTableID()!=INVALID_TABLE)
					{
						WORD wTableID=DistributeInfoArray[nIndex].pIServerUserItem->GetTableID();
						m_TableFrameArray[wTableID]->PerformStandUpAction(DistributeInfoArray[nIndex].pIServerUserItem);
					}
				}
			}
		}		
	}

	return true;
}

//���дʹ���
void CAttemperEngineSink::SensitiveWordFilter(LPCTSTR pMsg, LPTSTR pszFiltered, int nMaxLen)
{
	m_WordsFilter.Filtrate(pMsg,pszFiltered,nMaxLen);
}

//���ò���
void CAttemperEngineSink::SetMobileUserParameter(IServerUserItem * pIServerUserItem,BYTE cbDeviceType,WORD wBehaviorFlags,WORD wPageTableCount)
{
	if(wPageTableCount > m_pGameServiceOption->wTableCount)wPageTableCount=m_pGameServiceOption->wTableCount;
	pIServerUserItem->SetMobileUserDeskCount(wPageTableCount);
	pIServerUserItem->SetMobileUserRule(wBehaviorFlags);
}

//Ⱥ������
bool CAttemperEngineSink::SendDataBatchToMobileUser(WORD wCmdTable, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//ö���û�
	WORD wEnumIndex=0;
	while(wEnumIndex<m_ServerUserManager.GetUserItemCount())
	{
		//�����û�
		IServerUserItem *pIServerUserItem=m_ServerUserManager.EnumUserItem(wEnumIndex++);
		if(pIServerUserItem==NULL) continue;
		if(!pIServerUserItem->IsMobileUser()) continue;

		//�������
		WORD wMobileUserRule = pIServerUserItem->GetMobileUserRule();
		WORD wTagerTableID = pIServerUserItem->GetTableID();
		bool bViewModeAll = ((wMobileUserRule&VIEW_MODE_ALL)!=0);
		bool bRecviceGameChat = ((wMobileUserRule&RECVICE_GAME_CHAT)!=0);
		bool bRecviceRoomChat = ((wMobileUserRule&RECVICE_ROOM_CHAT)!=0);
		bool bRecviceRoomWhisper = ((wMobileUserRule&RECVICE_ROOM_WHISPER)!=0);

		//״̬����
		if(pIServerUserItem->GetUserStatus() >= US_SIT)
		{
			if(wTagerTableID != wCmdTable)continue;
		}

		//�������
		if(wSubCmdID==SUB_GR_USER_CHAT || wSubCmdID==SUB_GR_USER_EXPRESSION)
		{
			if(!bRecviceGameChat || !bRecviceRoomChat) continue;
		}
		if(wSubCmdID==SUB_GR_WISPER_CHAT || wSubCmdID==SUB_GR_WISPER_EXPRESSION)
		{
			if(!bRecviceRoomWhisper) continue;
		}

		//���ֿ���
		if(!bViewModeAll)
		{
			//��Ϣ����
			if(wSubCmdID==SUB_GR_USER_ENTER && wCmdTable==INVALID_TABLE) continue;
			if(wSubCmdID==SUB_GR_USER_SCORE && pIServerUserItem->GetUserStatus() < US_SIT) continue;

			//�������
			WORD wTagerDeskPos = pIServerUserItem->GetMobileUserDeskPos();
			WORD wTagerDeskCount = pIServerUserItem->GetMobileUserDeskCount();

			//״̬��Ϣ����
			if(wCmdTable < wTagerDeskPos) continue;
			if(wCmdTable > (wTagerDeskPos+wTagerDeskCount-1)) continue;
		}

		//������Ϣ
		SendData(pIServerUserItem,wMainCmdID,wSubCmdID,pData,wDataSize);
	}

	return true;
}

//Ⱥ���û���Ϣ
bool CAttemperEngineSink::SendUserInfoPacketBatchToMobileUser(IServerUserItem * pIServerUserItem)
{
	//Ч�����
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	BYTE cbBuffer[SOCKET_TCP_PACKET];
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();
	tagMobileUserInfoHead * pUserInfoHead=(tagMobileUserInfoHead *)cbBuffer;
	CSendPacketHelper SendPacket(cbBuffer+sizeof(tagMobileUserInfoHead),sizeof(cbBuffer)-sizeof(tagMobileUserInfoHead));

	//�û�����
	pUserInfoHead->wFaceID=pUserInfo->wFaceID;
	pUserInfoHead->dwGameID=pUserInfo->dwGameID;
	pUserInfoHead->dwUserID=pUserInfo->dwUserID;
	pUserInfoHead->dwCustomID=pUserInfo->dwCustomID;

	//�û�����
	pUserInfoHead->cbGender=pUserInfo->cbGender;
	pUserInfoHead->cbMemberOrder=pUserInfo->cbMemberOrder;

	//�û�״̬
	pUserInfoHead->wTableID=pUserInfo->wTableID;
	pUserInfoHead->wChairID=pUserInfo->wChairID;
	pUserInfoHead->cbUserStatus=pUserInfo->cbUserStatus;

	//�û�����
	pUserInfoHead->dwWinCount=pUserInfo->dwWinCount;
	pUserInfoHead->dwLostCount=pUserInfo->dwLostCount;
	pUserInfoHead->dwDrawCount=pUserInfo->dwDrawCount;
	pUserInfoHead->dwFleeCount=pUserInfo->dwFleeCount;
	pUserInfoHead->dwExperience=pUserInfo->dwExperience;

	//�û��ɼ�
	pUserInfoHead->lScore=pUserInfo->lScore;
	pUserInfoHead->lScore+=pIServerUserItem->GetTrusteeScore();
	pUserInfoHead->lScore+=pIServerUserItem->GetFrozenedScore();

	//������Ϣ
	SendPacket.AddPacket(pUserInfo->szNickName,DTP_GR_NICK_NAME);

	//��������
	WORD wHeadSize=sizeof(tagMobileUserInfoHead);
	SendDataBatchToMobileUser(pUserInfoHead->wTableID,MDM_GR_USER,SUB_GR_USER_ENTER,cbBuffer,wHeadSize+SendPacket.GetDataSize());

	return true;
}

//�������û���Ϣ
bool CAttemperEngineSink::SendViewTableUserInfoPacketToMobileUser(IServerUserItem * pIServerUserItem,DWORD dwUserIDReq)
{
	//��������
	BYTE cbBuffer[SOCKET_TCP_PACKET];
	tagMobileUserInfoHead * pUserInfoHead=(tagMobileUserInfoHead *)cbBuffer;
	WORD wMobileUserRule = pIServerUserItem->GetMobileUserRule();
	WORD wTagerTableID = pIServerUserItem->GetTableID();
	WORD wTagerDeskPos = pIServerUserItem->GetMobileUserDeskPos();
	WORD wTagerDeskCount = pIServerUserItem->GetMobileUserDeskCount();
	bool bViewModeAll = ((wMobileUserRule&VIEW_MODE_ALL)!=0);
	if(wTagerDeskCount==0) wTagerDeskCount=1;

	//ö���û�
	WORD wEnumIndex=0;
	while(wEnumIndex<m_ServerUserManager.GetUserItemCount())
	{
		//�����û�
		IServerUserItem *pIUserItem=m_ServerUserManager.EnumUserItem(wEnumIndex++);
		if(pIUserItem==NULL || (dwUserIDReq==INVALID_CHAIR && pIUserItem==pIServerUserItem)) continue;
		if(dwUserIDReq != INVALID_CHAIR && pIUserItem->GetUserID() != dwUserIDReq) continue;

		//���ֿ���
		if(dwUserIDReq==INVALID_CHAIR && !bViewModeAll)
		{
			if(pIUserItem->GetTableID() < wTagerDeskPos) continue;
			if(pIUserItem->GetTableID() > (wTagerDeskPos+wTagerDeskCount-1)) continue;
		}

		//�������
		tagUserInfo * pUserInfo=pIUserItem->GetUserInfo();
		ZeroMemory(cbBuffer,sizeof(cbBuffer));
		CSendPacketHelper SendPacket(cbBuffer+sizeof(tagMobileUserInfoHead),sizeof(cbBuffer)-sizeof(tagMobileUserInfoHead));

		//�û�����
		pUserInfoHead->wFaceID=pUserInfo->wFaceID;
		pUserInfoHead->dwGameID=pUserInfo->dwGameID;
		pUserInfoHead->dwUserID=pUserInfo->dwUserID;
		pUserInfoHead->dwCustomID=pUserInfo->dwCustomID;

		//�û�����
		pUserInfoHead->cbGender=pUserInfo->cbGender;
		pUserInfoHead->cbMemberOrder=pUserInfo->cbMemberOrder;

		//�û�״̬
		pUserInfoHead->wTableID=pUserInfo->wTableID;
		pUserInfoHead->wChairID=pUserInfo->wChairID;
		pUserInfoHead->cbUserStatus=pUserInfo->cbUserStatus;

		//�û�����
		pUserInfoHead->dwWinCount=pUserInfo->dwWinCount;
		pUserInfoHead->dwLostCount=pUserInfo->dwLostCount;
		pUserInfoHead->dwDrawCount=pUserInfo->dwDrawCount;
		pUserInfoHead->dwFleeCount=pUserInfo->dwFleeCount;
		pUserInfoHead->dwExperience=pUserInfo->dwExperience;

		//�û��ɼ�
		pUserInfoHead->lScore=pUserInfo->lScore;
		pUserInfoHead->lScore+=pIUserItem->GetTrusteeScore();
		pUserInfoHead->lScore+=pIUserItem->GetFrozenedScore();

		//������Ϣ
		SendPacket.AddPacket(pUserInfo->szNickName,DTP_GR_NICK_NAME);

		//��������
		WORD wHeadSize=sizeof(tagMobileUserInfoHead);
		SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_USER_ENTER,cbBuffer,wHeadSize+SendPacket.GetDataSize());
	}

	return true;
}

//�ֻ�������¼
bool CAttemperEngineSink::MobileUserImmediately(IServerUserItem * pIServerUserItem)
{
	//��������
	for (INT_PTR i=0;i<(m_pGameServiceOption->wTableCount);i++)
	{
		//��ȡ����
		CTableFrame * pTableFrame=m_TableFrameArray[i];
		if (pTableFrame->IsGameStarted()==true) continue;
		if(pTableFrame->IsTableLocked()) continue;

		//��Ч����
		WORD wChairID=pTableFrame->GetRandNullChairID();
		if (wChairID==INVALID_CHAIR) continue;

		//�û�����
		pTableFrame->PerformSitDownAction(wChairID,pIServerUserItem);
		return true;
	}

	//ʧ��
	m_TableFrameArray[0]->SendRequestFailure(pIServerUserItem,TEXT("û�ҵ��ɽ������Ϸ����"),REQUEST_FAILURE_NORMAL);
	return true;
}

//����ϵͳ��Ϣ
bool CAttemperEngineSink::SendSystemMessage(CMD_GR_SendMessage * pSendMessage, WORD wDataSize)
{
	//��Ϣ����
	ASSERT_ERR(pSendMessage!=NULL);

	//Ч������
	ASSERT_ERR(wDataSize==sizeof(CMD_GR_SendMessage)-sizeof(pSendMessage->szSystemMessage)+sizeof(TCHAR)*pSendMessage->wChatLength);
	if (wDataSize!=sizeof(CMD_GR_SendMessage)-sizeof(pSendMessage->szSystemMessage)+sizeof(TCHAR)*pSendMessage->wChatLength) 
		return false;

	//���з���
	if(pSendMessage->cbAllRoom == TRUE)
	{
		pSendMessage->cbAllRoom=FALSE;
		m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE,SUB_CS_C_SYSTEM_MESSAGE,pSendMessage,wDataSize);
	}
	else
	{
		//����ϵͳ��Ϣ
		if(pSendMessage->cbGame == TRUE)
			SendGameMessage(pSendMessage->szSystemMessage,SMT_CHAT);
		if(pSendMessage->cbRoom == TRUE)
			SendRoomMessage(pSendMessage->szSystemMessage,SMT_CHAT);
	}

	return true;
}

//�����Ϣ����
void CAttemperEngineSink::ClearSystemMessageData()
{
	while(m_SystemMessageList.GetCount() > 0)
	{
		tagSystemMessage * pRqHead = m_SystemMessageList.RemoveHead();
		if(pRqHead)
		{
			delete [] ((BYTE*)pRqHead);
		}
	}
}

void CAttemperEngineSink::PostReleaseAllAndroid()
{
	DBR_GR_GameAndriodAll request = {0};
	CopyMemory(request.guidRoom, m_guidRoom, 40 * sizeof WCHAR);

	m_pIDBCorrespondManager->PostDataBaseRequest(0l, DBR_GR_ANDRIOD_ALL, 0l, (void *)&request, sizeof request);
}


void CAttemperEngineSink::ShutDownSocket(IServerUserItem* pIServerUserItem)
{
	//��ȡ��
	if ( pIServerUserItem == NULL ) return ;
	WORD wBindIndex=pIServerUserItem->GetBindIndex();
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	//�������
	if (pBindParameter!=NULL)
	{
		//�ж�����
		if (pBindParameter->dwSocketID!=0L)
		{
			if (LOWORD(pBindParameter->dwSocketID)>=INDEX_ANDROID)
			{
				//m_AndroidUserManager.DeleteAndroidUserItem(pBindParameter->dwSocketID);
			}
			else
			{
				m_pITCPNetworkEngine->ShutDownSocket(pBindParameter->dwSocketID);
			}
		}
	}
}

// �������
bool CAttemperEngineSink::OnDBPCOperateResult(DWORD dwContextID, VOID* pData, WORD wDataSize)
{
	CMD_GP_OperateResult OperateResult = { 0 };

	// ��������
	DBO_GP_OperateResult * pOperateResult = (DBO_GP_OperateResult *)pData;
	OperateResult.cbStringLength = (wDataSize - sizeof(WORD) - sizeof(DWORD)) / 2;
	OperateResult.wSubCommandID = pOperateResult->wSubCommandID;
	lstrcpyn(OperateResult.szDescribe, pOperateResult->szDescribeString, sizeof(OperateResult.szDescribe));

	int wSendSize = 1 + 2  + CountStringBuffer(OperateResult.szDescribe);

	// ��������
	if (pOperateResult->dwResultCode == 0)
		m_pITCPNetworkEngine->SendData(dwContextID, MDM_GF_FRAME, SUB_GF_OPERATE_SUCCESS, &OperateResult, wSendSize);
	else
		m_pITCPNetworkEngine->SendData(dwContextID, MDM_GF_FRAME, SUB_GF_OPERATE_FAILURE, &OperateResult, wSendSize);

	return true;
}

bool CAttemperEngineSink::DetectKickUser(IServerUserItem * pIServerUserItem, BYTE cbQuickKick)
{
	if (pIServerUserItem == NULL) return false;
	if ( m_pGameServiceOption->wKindID != KIND_FISH && pIServerUserItem->GetUserStatus() == US_PLAYING) return false;
	//�����ж�
	DWORD dwUserID = pIServerUserItem->GetUserID();
	WORD wTableID = pIServerUserItem->GetTableID();
	WORD wBindIndex = pIServerUserItem->GetBindIndex();
	DWORD dwInoutIndex = pIServerUserItem->GetInoutIndex();
	tagBindParameter* pBindParameter = GetBindParameter(wBindIndex);
	if (pBindParameter == NULL) return false;

	tagKickUser KickUser = { 0 };
	if ( cbQuickKick && (m_KickUserItemMap.Lookup(dwUserID, KickUser) == TRUE)) {
		//��������
		if (wTableID != INVALID_TABLE)
		{
			CTableFrame * pTableFrame = m_TableFrameArray[wTableID];
			if (pTableFrame && pTableFrame->PerformStandUpAction(pIServerUserItem) == false) {}
		}

		SendLogonFailure(TEXT("���ѱ�����Ա�������,���Ժ��ٽ��룡"), 0, pBindParameter->dwSocketID);
		I_TRACE(TEXT("����Ա���������[%d]"), dwUserID);
		return true;
	}

// 	DWORD dwKickTime;
// 	if (m_KickUserItemMap.Lookup(dwUserID, dwKickTime) == TRUE)
// 	{
// 		//ʱЧ�ж�
// 		DWORD dwCurrTime = (DWORD)time(NULL);
// 		if ( dwKickTime > dwCurrTime ) 
// 		{
// 			//��������
// 			if (wTableID != INVALID_TABLE)
// 			{
// 				CTableFrame * pTableFrame = m_TableFrameArray[wTableID];
// 				if (pTableFrame && pTableFrame->PerformStandUpAction(pIServerUserItem) == false) {  }
// 			}
// 
// 			SendLogonFailure(TEXT("���ѱ�����Ա�������,���Ժ��ٽ��룡"), 0, pBindParameter->dwSocketID);
// 
// 			//PerformUnlockScore(dwUserID, dwInoutIndex, LER_SERVER_CONDITIONS);
// 
// 			I_TRACE(TEXT("����Ա�����[%d]"), dwUserID);
// 
// 			return true;
// 		}
// 		else
// 		{
// 			//�Ƴ����
// 			m_KickUserItemMap.RemoveKey(dwUserID);		
// 		}
// 	}
	return false;
}

//�������� ����
bool CAttemperEngineSink::OnTCPNetworkMainHeart(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//WORD wBindIndex=LOWORD(dwSocketID);
	//IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	//ASSERT_ERR(pIServerUserItem!=NULL);
	//if (pIServerUserItem==NULL) return false;

	if ( wSubCmdID == SUB_GR_HEART )
	{
		DWORD dwCurTime = (DWORD)time(NULL);
		//I_TRACE(TEXT("%s ��������,%d"),pIServerUserItem->GetNickName(),dwCurTime);
		//pIServerUserItem->SetHeartTime(dwCurTime);

		GameServer::GSUserHeart gsUserHeart;
		gsUserHeart.set_userid(1);
		std::string serializeStr;
		gsUserHeart.SerializeToString(&serializeStr);
		SendData(dwSocketID,MDM_GR_HEART,SUB_GR_HEART, (void*)serializeStr.c_str(), (WORD)serializeStr.size());
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////////////
