#include "StdAfx.h"
#include "ServiceUnits.h"
#include "ControlPacket.h"
#include "AttemperEngineSink.h"
#include <string>
#include <atlimage.h>
#include <afxinet.h>

using namespace std;
//////////////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_LOAD_GAME_LIST			1									//�����б�
#define IDI_CONNECT_CORRESPOND		2									//������ʶ
#define IDI_COLLECT_ONLINE_INFO		3									//ͳ������
#define IDI_BROADCAST_MSG			4									//�㲥��Ϣ
#define IDI_REFRESH_STORE			5									//�����̳�
#define IDI_CHECK_VERIFICATION		6									//��֤��ʧЧ
#define IDI_TIME_RANK				7									//��ȡ����

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CAttemperEngineSink::CAttemperEngineSink()
{
	//״̬����
	m_bNeekCorrespond=true;
	m_bShowServerStatus=false;

	//״̬����
	m_pInitParameter=NULL;
	m_pBindParameter=NULL;

	//�������
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngine=NULL;
	m_pITCPSocketService=NULL;

	//CTraceService::szLogRoot =TEXT("./SLog/LogonServer/");
}

//��������
CAttemperEngineSink::~CAttemperEngineSink()
{
}

//�ӿڲ�ѯ
VOID * CAttemperEngineSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAttemperEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE(IServerUserItemSink, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAttemperEngineSink,Guid,dwQueryVer);	
	return NULL;
}

//�����¼�
bool CAttemperEngineSink::OnAttemperEngineStart(IUnknownEx * pIUnknownEx)
{
	//�󶨲���
	m_pBindParameter=new tagBindParameter[m_pInitParameter->m_wMaxConnect];
	ZeroMemory(m_pBindParameter,sizeof(tagBindParameter)*m_pInitParameter->m_wMaxConnect);

//   	m_ConnParam.SetHost("127.0.0.1");
//   	m_ConnParam.SetServerPort(6379);
//   	m_ConnParam.SetTimeOut(5);
//   	m_ConnParam.SetPassword("aaqgdwxf");
//   	m_RedisClient.Init(m_ConnParam);
//   	m_RedisClient.ConnectDB();
//   	ENUM_REDIS_CONN_STATUS enumRedisConnStatus = m_RedisClient.GetConnStatus();
//   	if (enumRedisConnStatus != REDIS_CONN_RUNNING)
//   	{
//   		I_TRACE(TEXT("Connect Redis DB Failed!"));
//   	}
//  	if (m_RedisClient.AuthDB() == true)
//  	{
//  		//TRACE_LOG(TEXT("Connect Redis DB Success!"), TraceLevel_Warning);
//  	}
//	TRACE_LOG(TEXT("Connect Redis DB Success!"), TraceLevel_Warning);
// 	m_RedisClient.Publish();

	OnAttemperEngineBegin( pIUnknownEx );
	OnAttemperEngineBegin_BCode(pIUnknownEx);

	return true;
}

//ֹͣ�¼�
bool CAttemperEngineSink::OnAttemperEngineConclude(IUnknownEx * pIUnknownEx)
{
	//״̬����
	m_bNeekCorrespond=true;

	//�������
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngine=NULL;
	m_pITCPSocketService=NULL;

	//ɾ������
	SafeDeleteArray(m_pBindParameter);

	//�б����
	m_ServerListManager.ResetServerList();

	m_ServerUserManager.DeleteUserItem();

	return true;
}

//Ӧ���¼�
bool CAttemperEngineSink::OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID)
{
	//��ȡ����
	ASSERT_ERR(LOWORD(dwSocketID)<m_pInitParameter->m_wMaxConnect);
	if (LOWORD(dwSocketID)>=m_pInitParameter->m_wMaxConnect) return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//���ñ���
	pBindParameter->dwSocketID=dwSocketID;
	pBindParameter->dwClientAddr=dwClientAddr;
	pBindParameter->dwActiveTime=(DWORD)time(NULL);

	return true;
}

//�ر��¼�
bool CAttemperEngineSink::OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID)
{
	//�����Ϣ
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	if (pBindParameter==NULL) return false;

	//��ȡ�û�
	IServerUserItem * pIServerUserItem=pBindParameter->pIServerUserItem;
	if ( pIServerUserItem != NULL ) pIServerUserItem->SetUserStatus(US_NULL,INVALID_TABLE,INVALID_CHAIR);
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
				TRACE_LOG(szMsg,TraceLevel_Warning);

				tagUserInfo *pUserInfo = pIUserItem->GetUserInfo();
				pUserInfo->cbUserStatus = US_NULL;
				m_ServerUserManager.DeleteUserItem(pIUserItem);
// 				DBR_GP_LogonOut LogonOut = { 0 };
// 				LogonOut.dwUserID = pIUserItem->GetUserID();
// 				LogonOut.dwInOutIndex = pIUserItem->GetInoutIndex();
// 
// 				m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOGON_OUT, 0, &LogonOut, sizeof(LogonOut));
				break;
			}

		} while (true);
	}
	else TRACE_LOG(TEXT("������ӶϿ�����Դû�ͷ�"),TraceLevel_Warning);

	ZeroMemory((m_pBindParameter+wBindIndex),sizeof(tagBindParameter));

	return false;
}


//�ر��¼�
bool CAttemperEngineSink::OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason)
{
	//Э������
	if (wServiceID==NETWORK_CORRESPOND)
	{
		//�����ж�
		if (m_bNeekCorrespond==true)
		{
			//������ʾ
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("��Э�������������ӹر��ˣ�%ld �����������"),m_pInitParameter->m_wConnectTime);

			//��ʾ��Ϣ
			TRACE_LOG(szDescribe,TraceLevel_Warning);

			//����ʱ��
			ASSERT_ERR(m_pITimerEngine!=NULL);
			m_pITimerEngine->SetTimer(IDI_CONNECT_CORRESPOND,m_pInitParameter->m_wConnectTime*1000L,1,0);

			return true;
		}
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
			TRACE_LOG(szDescribe,TraceLevel_Warning);

			//����ʱ��
			ASSERT_ERR(m_pITimerEngine!=NULL);
			m_pITimerEngine->SetTimer(IDI_CONNECT_CORRESPOND,m_pInitParameter->m_wConnectTime*1000L,1,0);

			return false;
		}

		//��ʾ��Ϣ
		TRACE_LOG(TEXT("����ע����Ϸ��¼������..."),TraceLevel_Normal);

		//��������
		CMD_CS_C_RegisterPlaza RegisterPlaza;
		ZeroMemory(&RegisterPlaza,sizeof(RegisterPlaza));

		//���ñ���
		lstrcpyn(RegisterPlaza.szServerName,m_pInitParameter->m_szServerName,CountArray(RegisterPlaza.szServerName));
		lstrcpyn(RegisterPlaza.szServerAddr,m_pInitParameter->m_ServiceAddress.szAddress,CountArray(RegisterPlaza.szServerAddr));

		//��������
		m_pITCPSocketService->SendData(MDM_CS_REGISTER,SUB_CS_C_REGISTER_PLAZA,&RegisterPlaza,sizeof(RegisterPlaza));

		return true;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
